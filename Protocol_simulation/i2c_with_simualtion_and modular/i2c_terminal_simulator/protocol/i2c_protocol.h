/* ============================================================================
 *  protocol/i2c_protocol.h
 *  --------------------------------------------------------------------------
 *  I2C protocol framing and bit-level helper functions.
 *
 *  This module provides the low-level logic for:
 *    - Identifying START and STOP conditions on the bus.
 *    - Generating the custom 10-tick frame (8 data + 1 parity + 1 ACK).
 *    - Computing and validating even parity.
 * ==========================================================================*/

#ifndef I2C_PROTOCOL_H
#define I2C_PROTOCOL_H

#include "i2c_types.h"
#include "i2c_bus.h"

/* ----------------------------------------------------------------------------
 *  Condition Detection
 * --------------------------------------------------------------------------*/

/* Returns true if a START condition just occurred (SDA fell while SCL high). */
bool i2c_protocol_is_start(const i2c_bus_t *bus);

/* Returns true if a STOP condition just occurred (SDA rose while SCL high). */
bool i2c_protocol_is_stop(const i2c_bus_t *bus);

/* ----------------------------------------------------------------------------
 *  Frame Helpers
 * --------------------------------------------------------------------------*/

/* Compute the next phase in a transaction group. Groups are 10 ticks:
 * 0..7 (bits), 8 (parity), 9 (ACK). */
frame_phase_t i2c_protocol_next_phase(frame_phase_t current);

/* Get the bit index (7..0) for the current tick within a byte phase.
 * Returns -1 if not in a data/addr bit phase. */
int i2c_protocol_bit_index(frame_phase_t phase, uint64_t tick);

#endif /* I2C_PROTOCOL_H */
