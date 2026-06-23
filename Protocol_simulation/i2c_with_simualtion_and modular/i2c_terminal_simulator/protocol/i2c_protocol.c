/* ============================================================================
 *  protocol/i2c_protocol.c
 *  --------------------------------------------------------------------------
 *  Implementation of I2C protocol framing and detection logic.
 * ==========================================================================*/

#include "i2c_protocol.h"
#include <stdio.h>

bool i2c_protocol_is_start(const i2c_bus_t *bus)
{
    /* START: SDA falls while SCL is HIGH. */
    return i2c_bus_scl(bus) == I2C_HIGH && i2c_bus_sda_just_fell(bus);
}

bool i2c_protocol_is_stop(const i2c_bus_t *bus)
{
    /* STOP: SDA rises while SCL is HIGH. */
    return i2c_bus_scl(bus) == I2C_HIGH && i2c_bus_sda_just_rose(bus);
}

frame_phase_t i2c_protocol_next_phase(frame_phase_t current)
{
    switch (current) {
        case PHASE_IDLE:      return PHASE_START;
        case PHASE_START:     return PHASE_ADDR_BITS;
        case PHASE_ADDR_BITS: return PHASE_ADDR_PAR;
        case PHASE_ADDR_PAR:  return PHASE_ADDR_ACK;
        case PHASE_ADDR_ACK:  return PHASE_DATA_BITS;
        case PHASE_DATA_BITS: return PHASE_DATA_PAR;
        case PHASE_DATA_PAR:  return PHASE_DATA_ACK;
        case PHASE_DATA_ACK:  return PHASE_DATA_BITS; /* continues to next byte */
        case PHASE_STOP:      return PHASE_DONE;
        case PHASE_DONE:      return PHASE_IDLE;
        default:              return PHASE_IDLE;
    }
}

int i2c_protocol_bit_index(frame_phase_t phase, uint64_t tick)
{
    /* In this simulator, we assume bits are sent in 8 consecutive ticks
     * within their respective phases. */
    if (phase == PHASE_ADDR_BITS || phase == PHASE_DATA_BITS) {
        /* This is simplified; the actual bit-banger state machine in the
         * master/slave will handle the sub-tick counting.
         * However, if we want a helper, we need to know the offset. */
        return 7 - (int)(tick % 8);
    }
    return -1;
}
