/* ============================================================================
 *  master/i2c_master.c
 *  --------------------------------------------------------------------------
 *  Implementation of the I2C master state machine.
 * ==========================================================================*/

#include "i2c_master.h"
#include "i2c_protocol.h"
#include "i2c_arbitration.h"
#include <string.h>

void i2c_master_init(i2c_master_t *master, int bus_slot)
{
    if (master == NULL) return;
    memset(master, 0, sizeof(*master));
    master->bus_slot = bus_slot;
    master->state    = MASTER_IDLE;
    master->phase    = PHASE_IDLE;
}

bool i2c_master_queue(i2c_master_t *master, const i2c_transaction_t *txn)
{
    if (master == NULL || txn == NULL) return false;
    if (master->txn_queued) return false;
    
    memcpy(&master->current_txn, txn, sizeof(i2c_transaction_t));
    master->current_txn.completed = false;
    master->current_txn.success = false;
    master->txn_queued = true;
    return true;
}

void i2c_master_inject_parity_error(i2c_master_t *master)
{
    if (master == NULL) return;
    master->inject_parity_error = true;
}

void i2c_master_tick(i2c_master_t *master, i2c_bus_t *bus)
{
    if (master == NULL || bus == NULL) return;

    /* If we lost arbitration, we stay silent until the next transaction. */
    if (master->state == MASTER_ARBITRATION_LOST) {
        if (!master->txn_queued) {
            master->state = MASTER_IDLE;
        }
        return;
    }

    if (master->state == MASTER_IDLE) {
        if (master->txn_queued && i2c_bus_scl(bus) == I2C_HIGH && i2c_bus_sda(bus) == I2C_HIGH) {
            master->state = MASTER_STARTING;
            master->phase = PHASE_START;
        } else {
            return;
        }
    }

    /* Wait for SCL to be HIGH to detect clock stretching by slave.
     * In this simple simulator, the master "waits" by not advancing its
     * state machine if SCL is LOW when it expects it to be HIGH. */
    if (i2c_bus_scl(bus) == I2C_LOW && (master->phase != PHASE_START && master->phase != PHASE_STOP)) {
        /* Slave might be stretching SCL. */
        return;
    }

    uint64_t now = i2c_bus_now(bus);
    bool tick_is_edge = (now % 2 != 0);
    
    /* Simplified bit-banger:
     * Even ticks (0, 2, 4...): Data stable, SCL might be HIGH.
     * Odd ticks (1, 3, 5...): SCL transitions, Data might change.
     */
    
    i2c_level_t sda = i2c_bus_sda(bus);
    i2c_level_t scl = i2c_bus_scl(bus);

    switch (master->phase) {
        case PHASE_START:
            /* START: SCL HIGH, SDA 1->0 */
            if (tick_is_edge) {
                if (sda == I2C_HIGH) {
                    i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_LOW);
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    master->phase = PHASE_ADDR_BITS;
                    master->bit_count = 0;
                    master->tx_shift = (uint8_t)((master->current_txn.slave_addr << 1) | master->current_txn.rw);
                    master->state = MASTER_TX_ADDR;
                }
            }
            break;

        case PHASE_ADDR_BITS:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    /* Drive next bit. */
                    drive_t bit = (master->tx_shift & 0x80) ? DRIVE_HIGH : DRIVE_LOW;
                    i2c_bus_drive_sda(bus, master->bus_slot, bit);
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    /* Check arbitration. */
                    drive_t intent = (master->tx_shift & 0x80) ? DRIVE_HIGH : DRIVE_LOW;
                    if (i2c_arbitration_check(bus, master->bus_slot, intent)) {
                        master->state = MASTER_ARBITRATION_LOST;
                        master->current_txn.arbitration_lost = true;
                        master->current_txn.completed = true;
                        master->txn_queued = false;
                        i2c_bus_release(bus, master->bus_slot);
                        return;
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    master->tx_shift <<= 1;
                    master->bit_count++;
                    if (master->bit_count == 8) {
                        master->phase = PHASE_ADDR_PAR;
                    }
                }
            }
            break;

        case PHASE_ADDR_PAR:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    uint8_t par = i2c_even_parity((uint8_t)((master->current_txn.slave_addr << 1) | master->current_txn.rw));
                    if (master->inject_parity_error) {
                        par = !par;
                        master->inject_parity_error = false;
                    }
                    i2c_bus_drive_sda(bus, master->bus_slot, par ? DRIVE_HIGH : DRIVE_LOW);
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    master->phase = PHASE_ADDR_ACK;
                }
            }
            break;

        case PHASE_ADDR_ACK:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH); /* release for slave */
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    if (sda == I2C_LOW) {
                        i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                        master->byte_index = 0;
                        if (master->current_txn.rw == I2C_RW_WRITE) {
                            master->state = MASTER_TX_DATA;
                            master->phase = PHASE_DATA_BITS;
                            master->tx_shift = master->current_txn.tx_data[0];
                            master->bit_count = 0;
                        } else {
                            master->state = MASTER_RX_DATA;
                            master->phase = PHASE_DATA_BITS;
                            master->bit_count = 0;
                            master->rx_shift = 0;
                        }
                    } else {
                        /* NACK or no slave. */
                        master->phase = PHASE_STOP;
                        i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    }
                }
            }
            break;

        case PHASE_DATA_BITS:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    if (master->state == MASTER_TX_DATA) {
                        drive_t bit = (master->tx_shift & 0x80) ? DRIVE_HIGH : DRIVE_LOW;
                        i2c_bus_drive_sda(bus, master->bus_slot, bit);
                    } else {
                        i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH);
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    if (master->state == MASTER_RX_DATA) {
                        master->rx_shift = (uint8_t)((master->rx_shift << 1) | (sda == I2C_HIGH ? 1 : 0));
                    } else {
                        /* Arbitration check for data writes. */
                        drive_t intent = (master->tx_shift & 0x80) ? DRIVE_HIGH : DRIVE_LOW;
                        if (i2c_arbitration_check(bus, master->bus_slot, intent)) {
                            master->state = MASTER_ARBITRATION_LOST;
                            master->current_txn.arbitration_lost = true;
                            master->current_txn.completed = true;
                            master->txn_queued = false;
                            i2c_bus_release(bus, master->bus_slot);
                            return;
                        }
                        master->tx_shift <<= 1;
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    master->bit_count++;
                    if (master->bit_count == 8) {
                        master->phase = PHASE_DATA_PAR;
                    }
                }
            }
            break;

        case PHASE_DATA_PAR:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    if (master->state == MASTER_TX_DATA) {
                        uint8_t par = i2c_even_parity(master->current_txn.tx_data[master->byte_index]);
                        if (master->inject_parity_error) {
                            par = !par;
                            master->inject_parity_error = false;
                        }
                        i2c_bus_drive_sda(bus, master->bus_slot, par ? DRIVE_HIGH : DRIVE_LOW);
                    } else {
                        i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH);
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    if (master->state == MASTER_RX_DATA) {
                        uint8_t expected_par = i2c_even_parity(master->rx_shift);
                        uint8_t actual_par = (sda == I2C_HIGH ? 1 : 0);
                        master->parity_error = (expected_par != actual_par);
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                    master->phase = PHASE_DATA_ACK;
                }
            }
            break;

        case PHASE_DATA_ACK:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    if (master->state == MASTER_RX_DATA) {
                        /* Master ACKs if not last byte and parity OK. */
                        if (!master->parity_error && (master->byte_index + 1 < master->current_txn.tx_len)) {
                            i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_LOW);
                        } else {
                            i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH); /* NACK */
                        }
                    } else {
                        i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH); /* Release for slave ACK */
                    }
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    if (master->state == MASTER_TX_DATA) {
                        if (sda == I2C_LOW) {
                            master->byte_index++;
                            if (master->byte_index < master->current_txn.tx_len) {
                                master->phase = PHASE_DATA_BITS;
                                master->bit_count = 0;
                                master->tx_shift = master->current_txn.tx_data[master->byte_index];
                                i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                            } else {
                                master->current_txn.success = true;
                                master->phase = PHASE_STOP;
                                i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                            }
                        } else {
                            /* Slave NACK'd (parity error or buffer full). */
                            master->phase = PHASE_STOP;
                            i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                        }
                    } else { /* MASTER_RX_DATA */
                        master->current_txn.rx_data[master->byte_index] = master->rx_shift;
                        master->current_txn.rx_len++;
                        master->byte_index++;
                        if (master->byte_index < master->current_txn.tx_len && !master->parity_error) {
                            master->phase = PHASE_DATA_BITS;
                            master->bit_count = 0;
                            master->rx_shift = 0;
                            i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                        } else {
                            master->current_txn.success = !master->parity_error;
                            master->phase = PHASE_STOP;
                            i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_LOW);
                        }
                    }
                }
            }
            break;

        case PHASE_STOP:
            if (tick_is_edge) {
                if (scl == I2C_LOW) {
                    i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_LOW);
                    i2c_bus_drive_scl(bus, master->bus_slot, DRIVE_HIGH);
                } else {
                    i2c_bus_drive_sda(bus, master->bus_slot, DRIVE_HIGH);
                    master->phase = PHASE_DONE;
                }
            }
            break;

        case PHASE_DONE:
            master->current_txn.completed = true;
            master->txn_queued = false;
            master->state = MASTER_IDLE;
            master->phase = PHASE_IDLE;
            i2c_bus_release(bus, master->bus_slot);
            break;

        default:
            break;
    }
}
