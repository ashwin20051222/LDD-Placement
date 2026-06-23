/* ============================================================================
 *  arbitration/i2c_arbitration.c
 *  --------------------------------------------------------------------------
 *  Implementation of lossless multi-master arbitration.
 * ==========================================================================*/

#include "i2c_arbitration.h"

bool i2c_arbitration_check(const i2c_bus_t *bus, int master_slot, drive_t master_sda_intent)
{
    /* Arbitration only happens when a master is transmitting data or address
     * (not during START/STOP or ACK slots, though technically START/STOP
     * can also collide).
     *
     * The rule: if master intended HIGH but bus is LOW, arbitration lost.
     * This check must be performed when SCL is HIGH (stable data period). */
    if (i2c_bus_scl(bus) == I2C_HIGH) {
        if (master_sda_intent == DRIVE_HIGH && i2c_bus_sda(bus) == I2C_LOW) {
            return true;
        }
    }
    return false;
}
