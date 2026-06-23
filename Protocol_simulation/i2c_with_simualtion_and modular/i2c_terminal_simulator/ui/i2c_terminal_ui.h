/* ============================================================================
 *  ui/i2c_terminal_ui.h
 *  --------------------------------------------------------------------------
 *  ASCII terminal UI for the I2C simulator.
 * ==========================================================================*/

#ifndef I2C_TERMINAL_UI_H
#define I2C_TERMINAL_UI_H

#include "i2c_types.h"
#include "i2c_bus.h"

struct i2c_terminal_ui {
    i2c_bus_t       *bus;
    
    /* Log ring buffer. */
    i2c_log_event_t logs[MAX_LOG_LINES];
    int             log_head;
    int             log_count;
    
    /* Waveform history. */
    struct {
        i2c_level_t sda;
        i2c_level_t scl;
        uint64_t    tick;
    } wave[MAX_WAVE_HISTORY];
    int             wave_ptr;
    
    bool            no_color;
};

void i2c_terminal_ui_init(i2c_terminal_ui_t *ui, i2c_bus_t *bus);
void i2c_terminal_ui_render(i2c_terminal_ui_t *ui);
void i2c_terminal_ui_log_sink(const i2c_log_event_t *event, void *user);

/* REPL commands. */
void i2c_terminal_ui_help(void);
void i2c_terminal_ui_clear(void);

#endif /* I2C_TERMINAL_UI_H */
