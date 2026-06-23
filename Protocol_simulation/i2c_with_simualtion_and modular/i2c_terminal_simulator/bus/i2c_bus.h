/* ============================================================================
 *  bus/i2c_bus.h
 *  --------------------------------------------------------------------------
 *  Wired-AND bus core for the I2C protocol terminal simulator.
 *
 *  Responsibilities:
 *    - Own the simulation tick clock (delegating to core/i2c_clock.h)
 *    - Track which masters and slaves are wired to the bus
 *    - Aggregate the SDA and SCL lines from all participants using
 *      strict open-drain wired-AND logic
 *    - Provide a single-step API that bit-bangers call from their state
 *      machines (master, slave, arbitration)
 *    - Forward event records to the UI via a callback sink
 *
 *  Non-responsibilities:
 *    - Generating START / STOP conditions (protocol module)
 *    - Driving bit-level state machines (master / slave modules)
 *    - Rendering output (UI module)
 * ==========================================================================*/

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "i2c_types.h"
#include <stdarg.h>

/* ----------------------------------------------------------------------------
 *  Log severity levels (forwarded to UI by the bus)
 * --------------------------------------------------------------------------*/
typedef enum {
    I2C_LOG_DEBUG = 0,
    I2C_LOG_INFO  = 1,
    I2C_LOG_WARN  = 2,
    I2C_LOG_ERR   = 3
} i2c_log_level_t;

/* ----------------------------------------------------------------------------
 *  A single driver "vote" for a given line. We store the level the device
 *  wants to *drive* the line to; the bus converts releases (HIGH) via
 *  wired-AND against the active LOW pulls.
 *
 *  Encoding per slot:
 *      0 = the device is currently pulling the line LOW
 *      1 = the device is releasing the line (high-impedance / pull-up)
 * --------------------------------------------------------------------------*/
typedef enum {
    DRIVE_LOW  = 0,
    DRIVE_HIGH = 1
} drive_t;

/* ----------------------------------------------------------------------------
 *  Identifies which participant drove a particular line transition.
 *  Used purely for logging / debugging; has no effect on bus behaviour.
 * --------------------------------------------------------------------------*/
typedef enum {
    BUS_DRIVER_NONE  = -1,
    BUS_DRIVER_M0    = 0,
    BUS_DRIVER_M1    = 1,
    BUS_DRIVER_M2    = 2,
    BUS_DRIVER_M3    = 3,
    BUS_DRIVER_S0    = 4,
    BUS_DRIVER_S1    = 5,
    BUS_DRIVER_S2    = 6,
    BUS_DRIVER_S3    = 7,
    BUS_DRIVER_S4    = 8,
    BUS_DRIVER_S5    = 9,
    BUS_DRIVER_S6    = 10,
    BUS_DRIVER_S7   = 11
} bus_driver_id_t;

/* ----------------------------------------------------------------------------
 *  Slot indices for masters and slaves. The bus itself stores them in two
 *  separate flat arrays of size MAX_MASTERS / MAX_SLAVES. We expose plain
 *  integers and macros instead of typed handles to keep the API C-friendly
 *  and to match the spec's "MAX_MASTERS=4" / "MAX_SLAVES=8" bounds.
 * --------------------------------------------------------------------------*/
#define MASTER_ID_NONE  (-1)
#define SLAVE_ID_NONE   (-1)
#define SLOT_COUNT      (MAX_MASTERS + MAX_SLAVES)

/* ----------------------------------------------------------------------------
 *  Event record passed to the UI's log sink.
 *
 *  We pack everything by value to keep the bus allocation-free.
 * --------------------------------------------------------------------------*/
typedef struct {
    uint64_t        tick;          /* current tick at the time of the event  */
    i2c_log_level_t level;         /* severity                                */
    int             source_id;     /* master or slave slot, or -1 for "bus"   */
    bool            is_master;     /* true if source is a master, false slave */
    char            tag[16];       /* short tag, e.g. "M#0", "S#42", "BUS"   */
    char            message[160];  /* printf-style formatted body             */
} i2c_log_event_t;

/* ----------------------------------------------------------------------------
 *  Callback signature the bus uses to push log events to the UI.
 *  Set via i2c_bus_set_logger().
 * --------------------------------------------------------------------------*/
typedef void (*i2c_log_sink_fn)(const i2c_log_event_t *event, void *user);

/* ----------------------------------------------------------------------------
 *  The bus aggregate.
 *
 *  We keep all state inline (no heap allocations) so the simulator never
 *  fails due to malloc() and is reproducible across runs.
 * --------------------------------------------------------------------------*/
struct i2c_bus {
    /* -- resolved wire levels for the *current* tick ---------------------- */
    i2c_level_t sda_level;        /* what the bus actually shows on SDA    */
    i2c_level_t scl_level;        /* what the bus actually shows on SCL    */

    /* -- driver votes (one entry per device on the bus) ------------------- */
    drive_t     sda_drive[SLOT_COUNT];   /* per-device drive intent on SDA */
    drive_t     scl_drive[SLOT_COUNT];   /* per-device drive intent on SCL */
    bool        sda_active[SLOT_COUNT];  /* false => device not on bus    */
    bool        scl_active[SLOT_COUNT];  /* false => device not driving   */

    /* -- last edge information (for waveform rendering) ------------------- */
    i2c_level_t sda_prev;
    i2c_level_t scl_prev;
    uint64_t    sda_last_edge_tick;
    uint64_t    scl_last_edge_tick;

    /* -- participant registry -------------------------------------------- */
    int         master_count;            /* number of master slots in use   */
    int         slave_count;             /* number of slave slots in use    */
    int         master_addr[MAX_MASTERS];/* 7-bit address of each master    */
    int         slave_addr [MAX_SLAVES]; /* 7-bit address of each slave     */
    char        master_name[MAX_MASTERS][MAX_DEVICE_NAME];
    char        slave_name [MAX_SLAVES][MAX_DEVICE_NAME];

    /* -- logging --------------------------------------------------------- */
    i2c_log_sink_fn log_sink;            /* may be NULL                       */
    void            *log_sink_user;
};

/* ============================================================================
 *  Lifecycle
 * ==========================================================================*/

/* Initialise the bus: all lines HIGH, all drivers released, no devices
 * registered, no log sink. Idempotent — safe to call multiple times. */
void i2c_bus_init(i2c_bus_t *bus);

/* Reset bus state while preserving the registered participants. Clears
 * driver votes, edge history, and tick counter. */
void i2c_bus_reset(i2c_bus_t *bus);

/* ============================================================================
 *  Device registry
 * ==========================================================================*/

/* Register a master. Returns the master's slot index (0..MAX_MASTERS-1)
 * or -1 if all slots are taken. The @name is copied internally and may
 * be NULL (an auto-name is generated). */
int  i2c_bus_register_master(i2c_bus_t *bus, int addr7, const char *name);

/* Register a slave. Returns slot index (0..MAX_SLAVES-1) or -1 on full.
 * @addr7 must be in [0, I2C_ADDRESS_MAX]. */
int  i2c_bus_register_slave(i2c_bus_t *bus, int addr7, const char *name);

/* Lookup helpers. Return MASTER_ID_NONE / SLAVE_ID_NONE if not found. */
int  i2c_bus_find_master(i2c_bus_t *bus, int addr7);
int  i2c_bus_find_slave (i2c_bus_t *bus, int addr7);

/* ============================================================================
 *  Driver votes
 * ==========================================================================*/

/* Declare that @slot (a master or slave index, see SLOT_COUNT split above)
 * is present on the bus and intends to drive @line to @level.
 *
 * The bus does NOT recompute the wire until i2c_bus_tick() is called.
 * This split is essential: it lets every participant declare their vote
 * for the upcoming half-period before the bus resolves. */
void  i2c_bus_drive_sda(i2c_bus_t *bus, int slot, drive_t level);
void  i2c_bus_drive_scl(i2c_bus_t *bus, int slot, drive_t level);

/* A participant that has finished its transaction (master in IDLE, slave
 * in SLAVE_IDLE) should call this to mark its votes as inactive so they
 * don't accidentally keep the line LOW. */
void  i2c_bus_release(i2c_bus_t *bus, int slot);

/* ============================================================================
 *  Tick driver
 * ==========================================================================*/

/* Advance the simulation by one half-period. Performs:
 *    1. Aggregate the per-slot drive votes via wired-AND.
 *    2. Compare against the previous level; if changed, update edge history.
 *    3. Bump the global tick counter.
 *    4. Return the new tick number.
 *
 * This is the ONLY function that mutates the resolved SDA/SCL levels. */
uint64_t i2c_bus_tick(i2c_bus_t *bus);

/* Current tick number (alias for i2c_clock_now but routed through the bus
 * so other modules don't need to include core/i2c_clock.h). */
uint64_t i2c_bus_now(const i2c_bus_t *bus);

/* ============================================================================
 *  Readback
 * ==========================================================================*/

/* Get the resolved wire levels for the most recently completed tick. */
void  i2c_bus_levels(const i2c_bus_t *bus,
                     i2c_level_t *out_sda, i2c_level_t *out_scl);

/* Convenience accessors. */
i2c_level_t i2c_bus_sda(const i2c_bus_t *bus);
i2c_level_t i2c_bus_scl(const i2c_bus_t *bus);

/* True if a level transition occurred on @line since the previous tick. */
bool        i2c_bus_sda_just_fell(const i2c_bus_t *bus);
bool        i2c_bus_sda_just_rose(const i2c_bus_t *bus);
bool        i2c_bus_scl_just_fell(const i2c_bus_t *bus);
bool        i2c_bus_scl_just_rose(const i2c_bus_t *bus);

/* ============================================================================
 *  Logging
 * ==========================================================================*/

/* Install / remove a log sink. Pass NULL to detach. */
void  i2c_bus_set_logger(i2c_bus_t *bus, i2c_log_sink_fn fn, void *user);

/* Emit a log event. Variadic, printf-style body. @source_id may be -1 if
 * the event is not attributable to a single device. */
void  i2c_bus_log(i2c_bus_t *bus,
                  i2c_log_level_t level,
                  int source_id, bool is_master,
                  const char *fmt, ...);

void  i2c_bus_logv(i2c_bus_t *bus,
                   i2c_log_level_t level,
                   int source_id, bool is_master,
                   const char *fmt, va_list ap);

/* Helper: build a short tag like "M#0" or "S#42" into @out (must be >= 8
 * bytes). Returns @out. */
char *i2c_bus_format_tag(char *out, size_t out_size,
                         int source_id, bool is_master);

#endif /* I2C_BUS_H */
