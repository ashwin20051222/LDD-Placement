/* ============================================================================
 *  bus/i2c_bus.c
 *  --------------------------------------------------------------------------
 *  Wired-AND bus core implementation. See i2c_bus.h for the contract.
 *
 *  Invariants enforced here:
 *    1. SDA and SCL are always resolved to either LOW or HIGH. There is
 *       no third "floating" state — anything not pulled LOW is HIGH.
 *    2. Driver slots default to INACTIVE. The bus never assumes a device
 *       is participating unless it explicitly declares a drive for the
 *       current half-period.
 *    3. Level transitions are recorded with their tick number, so the UI
 *       can render edges even when many ticks elapse between transitions.
 *    4. Logging is fan-out: zero or more sinks. The bus is safe to use
 *       without a sink.
 * ==========================================================================*/

#include "i2c_bus.h"
#include "i2c_clock.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ----------------------------------------------------------------------------
 *  Internal helpers
 * --------------------------------------------------------------------------*/

/* Compute the resolved wire level for a single line by walking every
 * registered driver and applying wired-AND. */
static i2c_level_t resolve_line(const drive_t *votes, const bool *active)
{
    /* Start from HIGH: a fully idle line (no active drivers pulling LOW)
     * is HIGH by virtue of the pull-up resistor. */
    i2c_level_t level = I2C_HIGH;

    for (int i = 0; i < SLOT_COUNT; ++i) {
        if (!active[i]) {
            continue;          /* not on the bus this tick              */
        }
        if (votes[i] == DRIVE_LOW) {
            level = I2C_LOW;   /* any LOW dominates via wired-AND      */
        }
    }
    return level;
}

/* Recompute the resolved SDA and SCL levels from current driver votes.
 * Does NOT advance the tick counter; that happens in i2c_bus_tick(). */
static void recompute_levels(i2c_bus_t *bus)
{
    i2c_level_t new_sda = resolve_line(bus->sda_drive, bus->sda_active);
    i2c_level_t new_scl = resolve_line(bus->scl_drive, bus->scl_active);

    if (new_sda != bus->sda_level) {
        bus->sda_prev          = bus->sda_level;
        bus->sda_level         = new_sda;
        bus->sda_last_edge_tick = i2c_clock_now();
    } else {
        bus->sda_prev = bus->sda_level;
    }
    if (new_scl != bus->scl_level) {
        bus->scl_prev          = bus->scl_level;
        bus->scl_level         = new_scl;
        bus->scl_last_edge_tick = i2c_clock_now();
    } else {
        bus->scl_prev = bus->scl_level;
    }
}

/* Validate a slot index. */
static bool slot_in_range(int slot)
{
    return slot >= 0 && slot < SLOT_COUNT;
}

/* ----------------------------------------------------------------------------
 *  Lifecycle
 * --------------------------------------------------------------------------*/

void i2c_bus_init(i2c_bus_t *bus)
{
    if (bus == NULL) {
        return;
    }
    memset(bus, 0, sizeof(*bus));

    bus->sda_level = I2C_HIGH;       /* pull-ups: idle lines rest HIGH    */
    bus->scl_level = I2C_HIGH;
    bus->sda_prev  = I2C_HIGH;
    bus->scl_prev  = I2C_HIGH;

    for (int i = 0; i < SLOT_COUNT; ++i) {
        bus->sda_drive[i] = DRIVE_HIGH;  /* default: release (high-Z)    */
        bus->scl_drive[i] = DRIVE_HIGH;
        bus->sda_active[i] = false;      /* not on bus until they say so */
        bus->scl_active[i] = false;
    }

    bus->master_count = 0;
    bus->slave_count  = 0;
    bus->log_sink     = NULL;
    bus->log_sink_user = NULL;

    i2c_clock_init();
}

void i2c_bus_reset(i2c_bus_t *bus)
{
    if (bus == NULL) {
        return;
    }
    /* Capture the registry, then wipe everything except it. */
    int  m_count = bus->master_count;
    int  s_count = bus->slave_count;
    int  m_addr[MAX_MASTERS];
    int  s_addr[MAX_SLAVES];
    char m_name[MAX_MASTERS][MAX_DEVICE_NAME];
    char s_name[MAX_SLAVES][MAX_DEVICE_NAME];
    i2c_log_sink_fn sink = bus->log_sink;
    void *sink_user = bus->log_sink_user;

    memcpy(m_addr, bus->master_addr, sizeof(m_addr));
    memcpy(s_addr, bus->slave_addr,  sizeof(s_addr));
    memcpy(m_name, bus->master_name, sizeof(m_name));
    memcpy(s_name, bus->slave_name,  sizeof(s_name));

    i2c_bus_init(bus);

    bus->master_count = m_count;
    bus->slave_count  = s_count;
    memcpy(bus->master_addr, m_addr, sizeof(m_addr));
    memcpy(bus->slave_addr,  s_addr, sizeof(s_addr));
    memcpy(bus->master_name, m_name, sizeof(m_name));
    memcpy(bus->slave_name,  s_name, sizeof(s_name));
    bus->log_sink      = sink;
    bus->log_sink_user = sink_user;

    /* Re-zero driver slots since init() didn't see we wanted to keep the
     * registry alive; do it now in case any participant state lingers. */
    for (int i = 0; i < SLOT_COUNT; ++i) {
        bus->sda_drive[i]  = DRIVE_HIGH;
        bus->scl_drive[i]  = DRIVE_HIGH;
        bus->sda_active[i] = false;
        bus->scl_active[i] = false;
    }
}

/* ----------------------------------------------------------------------------
 *  Device registry
 * --------------------------------------------------------------------------*/

int i2c_bus_register_master(i2c_bus_t *bus, int addr7, const char *name)
{
    if (bus == NULL)                      return MASTER_ID_NONE;
    if (addr7 < 0 || addr7 > I2C_ADDRESS_MAX) return MASTER_ID_NONE;
    if (bus->master_count >= MAX_MASTERS) return MASTER_ID_NONE;

    int slot = bus->master_count++;
    bus->master_addr[slot] = addr7;
    if (name != NULL) {
        strncpy(bus->master_name[slot], name, MAX_DEVICE_NAME - 1);
        bus->master_name[slot][MAX_DEVICE_NAME - 1] = '\0';
    } else {
        snprintf(bus->master_name[slot], MAX_DEVICE_NAME,
                 "master#%d", slot);
    }
    return slot;
}

int i2c_bus_register_slave(i2c_bus_t *bus, int addr7, const char *name)
{
    if (bus == NULL)                      return SLAVE_ID_NONE;
    if (addr7 < 0 || addr7 > I2C_ADDRESS_MAX) return SLAVE_ID_NONE;
    if (bus->slave_count >= MAX_SLAVES)   return SLAVE_ID_NONE;

    /* Reject duplicate addresses — a real bus would also see this as a
     * collision on the very first byte of every transaction. */
    if (i2c_bus_find_slave(bus, addr7) != SLAVE_ID_NONE) {
        return SLAVE_ID_NONE;
    }

    int slot = bus->slave_count++;
    bus->slave_addr[slot] = addr7;
    if (name != NULL) {
        strncpy(bus->slave_name[slot], name, MAX_DEVICE_NAME - 1);
        bus->slave_name[slot][MAX_DEVICE_NAME - 1] = '\0';
    } else {
        snprintf(bus->slave_name[slot], MAX_DEVICE_NAME,
                 "slave#%d", slot);
    }
    return slot;
}

int i2c_bus_find_master(i2c_bus_t *bus, int addr7)
{
    if (bus == NULL) return MASTER_ID_NONE;
    for (int i = 0; i < bus->master_count; ++i) {
        if (bus->master_addr[i] == addr7) return i;
    }
    return MASTER_ID_NONE;
}

int i2c_bus_find_slave(i2c_bus_t *bus, int addr7)
{
    if (bus == NULL) return SLAVE_ID_NONE;
    for (int i = 0; i < bus->slave_count; ++i) {
        if (bus->slave_addr[i] == addr7) return i;
    }
    return SLAVE_ID_NONE;
}

/* ----------------------------------------------------------------------------
 *  Driver votes
 * --------------------------------------------------------------------------*/

void i2c_bus_drive_sda(i2c_bus_t *bus, int slot, drive_t level)
{
    if (bus == NULL || !slot_in_range(slot)) return;
    bus->sda_drive[slot]  = level;
    bus->sda_active[slot] = true;
}

void i2c_bus_drive_scl(i2c_bus_t *bus, int slot, drive_t level)
{
    if (bus == NULL || !slot_in_range(slot)) return;
    bus->scl_drive[slot]  = level;
    bus->scl_active[slot] = true;
}

void i2c_bus_release(i2c_bus_t *bus, int slot)
{
    if (bus == NULL || !slot_in_range(slot)) return;
    bus->sda_drive[slot]  = DRIVE_HIGH;
    bus->scl_drive[slot]  = DRIVE_HIGH;
    bus->sda_active[slot] = false;
    bus->scl_active[slot] = false;
}

/* ----------------------------------------------------------------------------
 *  Tick driver
 * --------------------------------------------------------------------------*/

uint64_t i2c_bus_tick(i2c_bus_t *bus)
{
    if (bus == NULL) return 0ULL;

    recompute_levels(bus);
    (void)i2c_clock_tick();   /* advance the virtual half-period counter  */

    /* After the tick resolves, drivers that did not refresh their vote
     * for the NEXT tick would otherwise keep the line LOW forever. We
     * adopt a "vote is single-shot" policy: any active slot that the
     * bus driver does NOT update next tick is automatically released.
     * This mirrors real I²C, where a device that goes off-bus releases
     * its pull-down transistors.
     *
     * We achieve this by clearing the active flags here; participants
     * re-assert them in their per-tick logic. */
    for (int i = 0; i < SLOT_COUNT; ++i) {
        bus->sda_active[i] = false;
        bus->scl_active[i] = false;
    }
    /* Drives default back to release so a forgotten slot doesn't latch. */
    for (int i = 0; i < SLOT_COUNT; ++i) {
        bus->sda_drive[i] = DRIVE_HIGH;
        bus->scl_drive[i] = DRIVE_HIGH;
    }

    return i2c_clock_now();
}

uint64_t i2c_bus_now(const i2c_bus_t *bus)
{
    if (bus == NULL) return 0ULL;
    return i2c_clock_now();
}

/* ----------------------------------------------------------------------------
 *  Readback
 * --------------------------------------------------------------------------*/

void i2c_bus_levels(const i2c_bus_t *bus,
                    i2c_level_t *out_sda, i2c_level_t *out_scl)
{
    if (bus == NULL) return;
    if (out_sda != NULL) *out_sda = bus->sda_level;
    if (out_scl != NULL) *out_scl = bus->scl_level;
}

i2c_level_t i2c_bus_sda(const i2c_bus_t *bus) { return (bus == NULL) ? I2C_HIGH : bus->sda_level; }
i2c_level_t i2c_bus_scl(const i2c_bus_t *bus) { return (bus == NULL) ? I2C_HIGH : bus->scl_level; }

bool i2c_bus_sda_just_fell(const i2c_bus_t *bus)
{
    if (bus == NULL) return false;
    return bus->sda_prev == I2C_HIGH && bus->sda_level == I2C_LOW;
}
bool i2c_bus_sda_just_rose(const i2c_bus_t *bus)
{
    if (bus == NULL) return false;
    return bus->sda_prev == I2C_LOW && bus->sda_level == I2C_HIGH;
}
bool i2c_bus_scl_just_fell(const i2c_bus_t *bus)
{
    if (bus == NULL) return false;
    return bus->scl_prev == I2C_HIGH && bus->scl_level == I2C_LOW;
}
bool i2c_bus_scl_just_rose(const i2c_bus_t *bus)
{
    if (bus == NULL) return false;
    return bus->scl_prev == I2C_LOW && bus->scl_level == I2C_HIGH;
}

/* ----------------------------------------------------------------------------
 *  Logging
 * --------------------------------------------------------------------------*/

void i2c_bus_set_logger(i2c_bus_t *bus, i2c_log_sink_fn fn, void *user)
{
    if (bus == NULL) return;
    bus->log_sink     = fn;
    bus->log_sink_user = user;
}

void i2c_bus_log(i2c_bus_t *bus,
                 i2c_log_level_t level,
                 int source_id, bool is_master,
                 const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    i2c_bus_logv(bus, level, source_id, is_master, fmt, ap);
    va_end(ap);
}

void i2c_bus_logv(i2c_bus_t *bus,
                  i2c_log_level_t level,
                  int source_id, bool is_master,
                  const char *fmt, va_list ap)
{
    if (bus == NULL) return;
    if (bus->log_sink == NULL) return;   /* nobody listening           */
    if (fmt == NULL) return;

    i2c_log_event_t ev;
    memset(&ev, 0, sizeof(ev));
    ev.tick      = i2c_bus_now(bus);
    ev.level     = level;
    ev.source_id = source_id;
    ev.is_master = is_master;

    i2c_bus_format_tag(ev.tag, sizeof(ev.tag), source_id, is_master);

    vsnprintf(ev.message, sizeof(ev.message), fmt, ap);

    bus->log_sink(&ev, bus->log_sink_user);
}

char *i2c_bus_format_tag(char *out, size_t out_size,
                         int source_id, bool is_master)
{
    if (out == NULL || out_size == 0) return out;
    if (source_id < 0) {
        snprintf(out, out_size, "BUS");
    } else if (is_master) {
        snprintf(out, out_size, "M#%d", source_id);
    } else {
        snprintf(out, out_size, "S#%d", source_id);
    }
    return out;
}
