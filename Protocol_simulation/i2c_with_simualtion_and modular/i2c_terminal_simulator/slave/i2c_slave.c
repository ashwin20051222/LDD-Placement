/* ============================================================================
 *  slave/i2c_slave.c
 *  --------------------------------------------------------------------------
 *  Implementation of the I2C slave state machine.
 * ==========================================================================*/

#include "i2c_slave.h"
#include "i2c_protocol.h"
#include <string.h>

void i2c_slave_init(i2c_slave_t *slave, int bus_slot, uint8_t addr7)
{
    if (slave == NULL) return;
    memset(slave, 0, sizeof(*slave));
    slave->bus_slot = bus_slot;
    slave->addr7    = addr7;
    slave->state    = SLAVE_IDLE;
    slave->phase    = PHASE_IDLE;
}

void i2c_slave_tick(i2c_slave_t *slave, i2c_bus_t *bus)
{
    if (slave == NULL || bus == NULL) return;

    /* Detect START/STOP regardless of state. */
    if (i2c_protocol_is_start(bus)) {
        slave->state     = SLAVE_IDLE;
        slave->phase     = PHASE_ADDR_BITS;
        slave->bit_count = 0;
        slave->rx_shift  = 0;
        return;
    }
    if (i2c_protocol_is_stop(bus)) {
        slave->state = SLAVE_IDLE;
        slave->phase = PHASE_IDLE;
        return;
    }

    /* Handle clock stretching. */
    if (slave->stretch_ticks > 0) {
        i2c_bus_drive_scl(bus, slave->bus_slot, DRIVE_LOW);
        slave->stretch_ticks--;
        return;
    }

    /* Bit-banging logic. Most I2C actions happen on SCL edges. */
    bool scl_rising = i2c_bus_scl_just_rose(bus);
    bool scl_falling = i2c_bus_scl_just_fell(bus);
    i2c_level_t sda = i2c_bus_sda(bus);

    switch (slave->phase) {
        case PHASE_ADDR_BITS:
            if (scl_rising) {
                slave->rx_shift = (uint8_t)((slave->rx_shift << 1) | (sda == I2C_HIGH ? 1 : 0));
                slave->bit_count++;
                if (slave->bit_count == 8) {
                    slave->phase = PHASE_ADDR_PAR;
                }
            }
            break;

        case PHASE_ADDR_PAR:
            if (scl_rising) {
                uint8_t expected_par = i2c_even_parity(slave->rx_shift);
                uint8_t actual_par = (sda == I2C_HIGH ? 1 : 0);
                slave->parity_error = (expected_par != actual_par);
                slave->phase = PHASE_ADDR_ACK;
            }
            break;

        case PHASE_ADDR_ACK:
            if (scl_falling) {
                /* Drive ACK if address matches and parity is OK. */
                uint8_t addr = slave->rx_shift >> 1;
                if (!slave->parity_error && addr == slave->addr7) {
                    i2c_bus_drive_sda(bus, slave->bus_slot, DRIVE_LOW);
                    slave->state = (slave->rx_shift & 0x01) ? SLAVE_TX : SLAVE_RX;
                }
            } else if (scl_rising) {
                if (slave->state == SLAVE_IDLE) {
                    slave->phase = PHASE_IDLE; /* didn't match */
                } else {
                    slave->phase = (slave->state == SLAVE_RX) ? PHASE_DATA_BITS : PHASE_DATA_BITS;
                    slave->bit_count = 0;
                    if (slave->state == SLAVE_TX) {
                        slave->tx_shift = slave->memory[slave->mem_ptr];
                    }
                }
            }
            break;

        case PHASE_DATA_BITS:
            if (slave->state == SLAVE_RX) {
                if (scl_rising) {
                    slave->rx_shift = (uint8_t)((slave->rx_shift << 1) | (sda == I2C_HIGH ? 1 : 0));
                    slave->bit_count++;
                    if (slave->bit_count == 8) {
                        slave->phase = PHASE_DATA_PAR;
                    }
                }
            } else if (slave->state == SLAVE_TX) {
                if (scl_falling) {
                    drive_t bit = (slave->tx_shift & 0x80) ? DRIVE_HIGH : DRIVE_LOW;
                    i2c_bus_drive_sda(bus, slave->bus_slot, bit);
                    slave->tx_shift <<= 1;
                } else if (scl_rising) {
                    slave->bit_count++;
                    if (slave->bit_count == 8) {
                        slave->phase = PHASE_DATA_PAR;
                    }
                }
            }
            break;

        case PHASE_DATA_PAR:
            if (slave->state == SLAVE_RX) {
                if (scl_rising) {
                    uint8_t expected_par = i2c_even_parity(slave->rx_shift);
                    uint8_t actual_par = (sda == I2C_HIGH ? 1 : 0);
                    slave->parity_error = (expected_par != actual_par);
                    slave->phase = PHASE_DATA_ACK;
                }
            } else if (slave->state == SLAVE_TX) {
                if (scl_falling) {
                    uint8_t par = i2c_even_parity(slave->memory[slave->mem_ptr]);
                    i2c_bus_drive_sda(bus, slave->bus_slot, par ? DRIVE_HIGH : DRIVE_LOW);
                } else if (scl_rising) {
                    slave->phase = PHASE_DATA_ACK;
                }
            }
            break;

        case PHASE_DATA_ACK:
            if (slave->state == SLAVE_RX) {
                if (scl_falling) {
                    if (!slave->parity_error) {
                        i2c_bus_drive_sda(bus, slave->bus_slot, DRIVE_LOW);
                        slave->memory[slave->mem_ptr++] = slave->rx_shift;
                    }
                } else if (scl_rising) {
                    slave->bit_count = 0;
                    slave->phase = PHASE_DATA_BITS;
                }
            } else if (slave->state == SLAVE_TX) {
                if (scl_rising) {
                    if (sda == I2C_LOW) {
                        slave->mem_ptr++;
                        slave->bit_count = 0;
                        slave->phase = PHASE_DATA_BITS;
                        slave->tx_shift = slave->memory[slave->mem_ptr];
                    } else {
                        slave->state = SLAVE_IDLE;
                        slave->phase = PHASE_IDLE;
                    }
                }
            }
            break;

        default:
            break;
    }
}

void i2c_slave_poke(i2c_slave_t *slave, uint8_t offset, uint8_t value)
{
    if (slave == NULL) return;
    slave->memory[offset] = value;
}

void i2c_slave_stretch(i2c_slave_t *slave, int ticks)
{
    if (slave == NULL) return;
    slave->stretch_ticks = ticks;
}
