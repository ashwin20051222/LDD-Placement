/* ============================================================================
 *  slave/i2c_slave.h
 *  --------------------------------------------------------------------------
 *  I2C slave simulation with memory bank and custom parity check.
 * ==========================================================================*/

#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include "i2c_types.h"
#include "i2c_bus.h"

struct i2c_slave {
    int             bus_slot;            /* slot index assigned by the bus  */
    uint8_t         addr7;               /* 7-bit address                   */
    slave_state_t   state;               /* current logical state           */
    frame_phase_t   phase;               /* sub-byte phase                  */
    
    uint8_t         memory[MAX_BUFFER];  /* 256-byte memory bank            */
    uint8_t         mem_ptr;             /* current memory address pointer  */
    
    uint8_t         rx_shift;            /* bit-banging RX register         */
    uint8_t         tx_shift;            /* bit-banging TX register         */
    int             bit_count;           /* 0..7 bits processed in byte     */
    
    bool            parity_error;        /* true if last RX'd parity failed */
    int             stretch_ticks;       /* pending clock stretch duration  */
};

/* Initialise a slave structure. */
void i2c_slave_init(i2c_slave_t *slave, int bus_slot, uint8_t addr7);

/* Process one simulation tick. The slave reads the bus and updates its
 * drive votes for the NEXT tick. */
void i2c_slave_tick(i2c_slave_t *slave, i2c_bus_t *bus);

/* Diagnostic: poke a value into slave memory. */
void i2c_slave_poke(i2c_slave_t *slave, uint8_t offset, uint8_t value);

/* Diagnostic: force clock stretching on the next operation. */
void i2c_slave_stretch(i2c_slave_t *slave, int ticks);

#endif /* I2C_SLAVE_H */
