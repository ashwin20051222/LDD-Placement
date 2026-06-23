/* ============================================================================
 *  core/i2c_types.h
 *  --------------------------------------------------------------------------
 *  Project-wide type definitions, enums and compile-time constants for the
 *  I2C protocol terminal simulator (custom variant with even-parity bit).
 *
 *  This file is header-only. All concrete data structures used across module
 *  boundaries are declared here so that callers can include exactly one
 *  central header for the shared vocabulary of the simulator.
 * ==========================================================================*/

#ifndef I2C_CORE_TYPES_H
#define I2C_CORE_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* ----------------------------------------------------------------------------
 *  Compile-time configuration constants
 * --------------------------------------------------------------------------*/
#define I2C_ADDRESS_MAX     (127)   /* 7-bit address space (0..127)         */
#define MAX_MASTERS         (4)     /* Maximum concurrent bus masters       */
#define MAX_SLAVES          (8)     /* Maximum slaves wired to the bus      */
#define MAX_BUFFER          (256)   /* Slave memory bank size (bytes)       */
#define MAX_TX_PAYLOAD      (64)    /* Max bytes a master may queue per txn */
#define MAX_DEVICE_NAME     (24)    /* For human-friendly logging           */
#define MAX_LOG_LINES       (128)   /* Ring-buffer depth in UI logger       */
#define MAX_LOG_LINE_LEN    (160)   /* Max chars per log line               */
#define MAX_WAVE_HISTORY    (64)    /* Visible width of ASCII waveform      */

/* ----------------------------------------------------------------------------
 *  Logical bus levels
 *
 *  Lines idle HIGH (pulled up). A device "drives" the line by pulling it
 *  LOW. Anything not pulled LOW resolves to HIGH via wired-AND.
 * --------------------------------------------------------------------------*/
typedef enum {
    I2C_LOW  = 0,
    I2C_HIGH = 1
} i2c_level_t;

/* ----------------------------------------------------------------------------
 *  R/W bit semantics (low bit of address byte)
 * --------------------------------------------------------------------------*/
typedef enum {
    I2C_RW_WRITE = 0,  /* Master writes data to slave  */
    I2C_RW_READ  = 1   /* Master reads data from slave */
} i2c_rw_t;

/* ----------------------------------------------------------------------------
 *  ACK / NACK convention
 *
 *  ACK is encoded by the receiver pulling SDA LOW during the ack slot.
 *  NACK is encoded by the receiver leaving SDA released (HIGH).
 * --------------------------------------------------------------------------*/
typedef enum {
    I2C_ACK  = 0,
    I2C_NACK = 1
} i2c_ack_t;

/* ----------------------------------------------------------------------------
 *  Master state machine
 * --------------------------------------------------------------------------*/
typedef enum {
    MASTER_IDLE              = 0,
    MASTER_STARTING          = 1,
    MASTER_TX_ADDR           = 2,
    MASTER_TX_DATA           = 3,
    MASTER_RX_DATA           = 4,
    MASTER_STOPPING          = 5,
    MASTER_ARBITRATION_LOST  = 6
} master_state_t;

/* ----------------------------------------------------------------------------
 *  Slave state machine
 * --------------------------------------------------------------------------*/
typedef enum {
    SLAVE_IDLE         = 0,
    SLAVE_ADDR_MATCHED = 1,
    SLAVE_RX           = 2,
    SLAVE_TX           = 3
} slave_state_t;

/* ----------------------------------------------------------------------------
 *  Sub-state cursor used by the bit-banger
 *
 *  A frame after START always proceeds in 10-tick groups:
 *      ticks  0..7  -> 8 payload bits (MSB first)
 *      tick   8     -> EVEN PARITY BIT (custom modification)
 *      tick   9     -> ACK / NACK slot
 *
 *  For the address byte the "payload" is { addr[6..0], R/W }.
 * --------------------------------------------------------------------------*/
typedef enum {
    PHASE_IDLE      = 0,
    PHASE_START     = 1,
    PHASE_ADDR_BITS = 2,   /* 8 bits of (addr<<1 | rw)                 */
    PHASE_ADDR_PAR  = 3,   /* parity bit for the address byte           */
    PHASE_ADDR_ACK  = 4,   /* slave ack for address                     */
    PHASE_DATA_BITS = 5,   /* 8 bits of payload                         */
    PHASE_DATA_PAR  = 6,   /* parity bit for the data byte              */
    PHASE_DATA_ACK  = 7,   /* receiver ack (parity-gated)               */
    PHASE_STOP      = 8,
    PHASE_DONE      = 9
} frame_phase_t;

/* ----------------------------------------------------------------------------
 *  Direction of a single bit on the wire (used for logging)
 * --------------------------------------------------------------------------*/
typedef enum {
    BIT_DIR_MASTER_TX = 0,
    BIT_DIR_SLAVE_TX  = 1,
    BIT_DIR_IDLE      = 2
} bit_direction_t;

/* ----------------------------------------------------------------------------
 *  Opaque forward declarations for cross-module pointers
 * --------------------------------------------------------------------------*/
struct i2c_bus;
struct i2c_master;
struct i2c_slave;
struct i2c_terminal_ui;

typedef struct i2c_bus          i2c_bus_t;
typedef struct i2c_master       i2c_master_t;
typedef struct i2c_slave        i2c_slave_t;
typedef struct i2c_terminal_ui  i2c_terminal_ui_t;

/* ----------------------------------------------------------------------------
 *  A single queued master transaction request
 * --------------------------------------------------------------------------*/
typedef struct {
    uint8_t  slave_addr;                 /* 7-bit slave address              */
    i2c_rw_t rw;                         /* read or write                    */
    uint8_t  tx_data[MAX_TX_PAYLOAD];    /* outbound payload (writes)        */
    size_t   tx_len;                     /* number of bytes to send/read     */
    uint8_t  rx_data[MAX_TX_PAYLOAD];    /* inbound payload (reads)          */
    size_t   rx_len;                     /* bytes actually received          */
    bool     completed;                  /* true once master returned IDLE   */
    bool     success;                    /* true if no NACK / no arb-loss    */
    bool     arbitration_lost;           /* set if arbitration was lost      */
} i2c_transaction_t;

/* ----------------------------------------------------------------------------
 *  Helper: compute even parity over the low 8 bits of @byte.
 *
 *  Returns 1 if the number of set bits is odd (so total bits incl. parity
 *  becomes even), else 0. Defined static-inline because it is used in tight
 *  loops by master, slave and protocol modules alike.
 * --------------------------------------------------------------------------*/
static inline uint8_t i2c_even_parity(uint8_t byte)
{
    byte ^= (uint8_t)(byte >> 4);
    byte ^= (uint8_t)(byte >> 2);
    byte ^= (uint8_t)(byte >> 1);
    return (uint8_t)(byte & 0x01u);
}

/* ----------------------------------------------------------------------------
 *  Helper: wired-AND of two levels (LOW dominates).
 * --------------------------------------------------------------------------*/
static inline i2c_level_t i2c_wired_and(i2c_level_t a, i2c_level_t b)
{
    return (a == I2C_LOW || b == I2C_LOW) ? I2C_LOW : I2C_HIGH;
}

#endif /* I2C_CORE_TYPES_H */
