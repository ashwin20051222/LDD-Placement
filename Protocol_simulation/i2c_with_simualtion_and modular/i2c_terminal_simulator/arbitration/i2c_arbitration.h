/* ============================================================================
 *  arbitration/i2c_arbitration.h
 *  --------------------------------------------------------------------------
 *  Lossless multi-master arbitration logic.
 *
 *  In I2C, arbitration happens on the SDA line while SCL is HIGH. A master
 *  loses arbitration if it drives SDA HIGH (releases it) but senses SDA is
 *  LOW (driven by another master).
 * ==========================================================================*/

#ifndef I2C_ARBITRATION_H
#define I2C_ARBITRATION_H

#include "i2c_types.h"
#include "i2c_bus.h"

/* Check if a master has lost arbitration at the current tick.
 * Returns true if arbitration was lost. */
bool i2c_arbitration_check(const i2c_bus_t *bus, int master_slot, drive_t master_sda_intent);

#endif /* I2C_ARBITRATION_H */
