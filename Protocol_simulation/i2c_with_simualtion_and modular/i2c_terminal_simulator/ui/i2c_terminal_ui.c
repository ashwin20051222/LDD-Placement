/* ============================================================================
 *  ui/i2c_terminal_ui.c
 *  --------------------------------------------------------------------------
 *  Implementation of the ASCII waveform and terminal log.
 * ==========================================================================*/

#include "i2c_terminal_ui.h"
#include <stdio.h>
#include <string.h>

void i2c_terminal_ui_init(i2c_terminal_ui_t *ui, i2c_bus_t *bus)
{
    if (ui == NULL) return;
    memset(ui, 0, sizeof(*ui));
    ui->bus = bus;
    i2c_bus_set_logger(bus, i2c_terminal_ui_log_sink, ui);
}

void i2c_terminal_ui_log_sink(const i2c_log_event_t *event, void *user)
{
    i2c_terminal_ui_t *ui = (i2c_terminal_ui_t *)user;
    if (ui == NULL || event == NULL) return;
    
    ui->logs[ui->log_head] = *event;
    ui->log_head = (ui->log_head + 1) % MAX_LOG_LINES;
    if (ui->log_count < MAX_LOG_LINES) ui->log_count++;
}

void i2c_terminal_ui_render(i2c_terminal_ui_t *ui)
{
    /* In a real implementation, this would redraw the screen.
     * Here we just print the latest log and a simple waveform snippet. */
    
    /* Update waveform history. */
    ui->wave[ui->wave_ptr].sda = i2c_bus_sda(ui->bus);
    ui->wave[ui->wave_ptr].scl = i2c_bus_scl(ui->bus);
    ui->wave[ui->wave_ptr].tick = i2c_bus_now(ui->bus);
    ui->wave_ptr = (ui->wave_ptr + 1) % MAX_WAVE_HISTORY;

    /* Simple render: just print the last few log lines. */
    printf("\n--- Simulation Tick: %lu ---\n", (unsigned long)i2c_bus_now(ui->bus));
    
    int start = (ui->log_head - 1 + MAX_LOG_LINES) % MAX_LOG_LINES;
    if (ui->log_count > 0) {
        printf("[%s] %s\n", ui->logs[start].tag, ui->logs[start].message);
    }
}

void i2c_terminal_ui_help(void)
{
    printf("I2C Simulator Help:\n");
    printf("  write <m_id> <addr> <data...> - Queue a write transaction\n");
    printf("  read <m_id> <addr> <len>      - Queue a read transaction\n");
    printf("  step [n]                      - Advance simulation by n ticks\n");
    printf("  run                           - Run until transactions complete\n");
    printf("  quit                          - Exit simulator\n");
}

void i2c_terminal_ui_clear(void)
{
    printf("\033[H\033[J");
}
