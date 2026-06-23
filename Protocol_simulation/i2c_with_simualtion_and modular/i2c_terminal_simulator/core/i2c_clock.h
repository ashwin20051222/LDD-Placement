/* ============================================================================
 *  core/i2c_clock.h
 *  --------------------------------------------------------------------------
 *  Simple virtual tick clock for the I2C protocol terminal simulator.
 *
 *  The simulation is synchronous and driven by a single global counter of
 *  "half-period ticks".
 *
 *  - Tick 0: Bus idle.
 *  - Odd ticks: SCL transitions (or would transition) to a new state.
 *  - Even ticks: SDA is sampled or stable.
 *
 *  This file is header-only to ensure that all modules share the same
 *  static clock state without needing a separate compilation unit.
 * ==========================================================================*/

#ifndef I2C_CLOCK_H
#define I2C_CLOCK_H

#include <stdint.h>

/* The global tick counter. We use a static variable in a header-only file
 * which is normally a bad idea, but here we only ever link the final
 * simulator binary once, and the bus module is the primary owner. */
static uint64_t g_i2c_tick_counter = 0;

/* Initialise the clock to zero. */
static inline void i2c_clock_init(void)
{
    g_i2c_tick_counter = 0;
}

/* Advance the clock by one half-period. Returns the new tick value. */
static inline uint64_t i2c_clock_tick(void)
{
    return ++g_i2c_tick_counter;
}

/* Get the current simulation tick. */
static inline uint64_t i2c_clock_now(void)
{
    return g_i2c_tick_counter;
}

#endif /* I2C_CLOCK_H */
