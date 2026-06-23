/* ============================================================================
 *  main.c
 *  --------------------------------------------------------------------------
 *  Entry point for the I2C protocol terminal simulator.
 * ==========================================================================*/

#include "i2c_types.h"
#include "i2c_bus.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "i2c_terminal_ui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

static i2c_bus_t         g_bus;
static i2c_master_t      g_masters[MAX_MASTERS];
static i2c_slave_t       g_slaves[MAX_SLAVES];
static i2c_terminal_ui_t  g_ui;

static void handle_command(char *line)
{
    char *cmd = strtok(line, " \t\n");
    if (cmd == NULL) return;

    if (strcasecmp(cmd, "help") == 0) {
        i2c_terminal_ui_help();
    } else if (strcasecmp(cmd, "quit") == 0 || strcasecmp(cmd, "exit") == 0) {
        exit(0);
    } else if (strcasecmp(cmd, "step") == 0) {
        char *arg = strtok(NULL, " \t\n");
        int steps = arg ? atoi(arg) : 1;
        for (int i = 0; i < steps; ++i) {
            for (int m = 0; m < MAX_MASTERS; ++m) i2c_master_tick(&g_masters[m], &g_bus);
            for (int s = 0; s < MAX_SLAVES; ++s) i2c_slave_tick(&g_slaves[s], &g_bus);
            i2c_bus_tick(&g_bus);
        }
        i2c_terminal_ui_render(&g_ui);
    } else if (strcasecmp(cmd, "write") == 0) {
        int m_id = atoi(strtok(NULL, " \t\n"));
        int addr = strtol(strtok(NULL, " \t\n"), NULL, 0);
        i2c_transaction_t txn;
        memset(&txn, 0, sizeof(txn));
        txn.slave_addr = (uint8_t)addr;
        txn.rw = I2C_RW_WRITE;
        char *data;
        while ((data = strtok(NULL, " \t\n")) != NULL) {
            txn.tx_data[txn.tx_len++] = (uint8_t)strtol(data, NULL, 0);
        }
        if (i2c_master_queue(&g_masters[m_id], &txn)) {
            printf("[Q] master#%d queued WRITE to 0x%02X\n", m_id, addr);
        }
    } else if (strcasecmp(cmd, "read") == 0) {
        int m_id = atoi(strtok(NULL, " \t\n"));
        int addr = strtol(strtok(NULL, " \t\n"), NULL, 0);
        int len = atoi(strtok(NULL, " \t\n"));
        i2c_transaction_t txn;
        memset(&txn, 0, sizeof(txn));
        txn.slave_addr = (uint8_t)addr;
        txn.rw = I2C_RW_READ;
        txn.tx_len = (size_t)len;
        if (i2c_master_queue(&g_masters[m_id], &txn)) {
            printf("[Q] master#%d queued READ from 0x%02X len=%d\n", m_id, addr, len);
        }
    } else if (strcasecmp(cmd, "run") == 0) {
        bool busy = true;
        while (busy) {
            busy = false;
            for (int m = 0; m < MAX_MASTERS; ++m) {
                i2c_master_tick(&g_masters[m], &g_bus);
                if (g_masters[m].txn_queued) busy = true;
            }
            for (int s = 0; s < MAX_SLAVES; ++s) i2c_slave_tick(&g_slaves[s], &g_bus);
            i2c_bus_tick(&g_bus);
        }
        i2c_terminal_ui_render(&g_ui);
    } else if (strcasecmp(cmd, "slave") == 0) {
        char *sub = strtok(NULL, " \t\n");
        if (strcasecmp(sub, "add") == 0) {
            int addr = strtol(strtok(NULL, " \t\n"), NULL, 0);
            char *name = strtok(NULL, " \t\n");
            i2c_bus_register_slave(&g_bus, addr, name);
            printf("[CFG] slave registered at 0x%02X\n", addr);
        }
    }
}

int main(int argc, char **argv)
{
    i2c_bus_init(&g_bus);
    i2c_terminal_ui_init(&g_ui, &g_bus);

    for (int i = 0; i < MAX_MASTERS; ++i) {
        i2c_bus_register_master(&g_bus, 0, NULL);
        i2c_master_init(&g_masters[i], i);
    }
    
    /* Default topology for demo. */
    i2c_bus_register_slave(&g_bus, 0x42, "EEPROM");
    i2c_slave_init(&g_slaves[0], MAX_MASTERS + 0, 0x42);

    printf("I2C Protocol Terminal Simulator (Custom Parity Variant)\n");
    printf("Type 'help' for commands.\n");

    char line[256];
    while (printf("i2c> ") && fgets(line, sizeof(line), stdin)) {
        handle_command(line);
    }

    return 0;
}
