/* ============================================================================
 *  master/i2c_master.h
 *  --------------------------------------------------------------------------
 *  I2C master simulation with bit-banging and transaction queuing.
 * ==========================================================================*/

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "i2c_types.h"
#include "i2c_bus.h"

struct i2c_master {
    int                 bus_slot;        /* slot index assigned by the bus  */
    master_state_t      state;           /* current logical state           */
    frame_phase_t       phase;           /* sub-byte phase                  */
    
    i2c_transaction_t   current_txn;     /* currently active transaction    */
    bool                txn_queued;      /* true if a txn is waiting/active */
    
    uint8_t             tx_shift;        /* bit-banging TX register         */
    uint8_t             rx_shift;        /* bit-banging RX register         */
    int                 bit_count;       /* 0..7 bits processed in byte     */
    size_t              byte_index;      /* which byte of the payload       */
    
    bool                parity_error;    /* detected during RX              */
    bool                inject_parity_error; /* debug flag                  */
};

/* Initialise a master structure. */
void i2c_master_init(i2c_master_t *master, int bus_slot);

/* Process one simulation tick. */
void i2c_master_tick(i2c_master_t *master, i2c_bus_t *bus);

/* Queue a new transaction. Returns false if one is already in progress. */
bool i2c_master_queue(i2c_master_t *master, const i2c_transaction_t *txn);

/* Diagnostic: force a parity error on the NEXT byte sent by this master. */
void i2c_master_inject_parity_error(i2c_master_t *master);

#endif /* I2C_MASTER_H */
