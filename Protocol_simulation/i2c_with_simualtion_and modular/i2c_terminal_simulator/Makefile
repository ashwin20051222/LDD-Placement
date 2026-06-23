# =============================================================================
#  I2C Protocol Terminal Simulator
#  Modular Makefile with automatic dependency tracking
# =============================================================================

# ---- Toolchain --------------------------------------------------------------
CC      := gcc
RM      := rm -f
RMDIR   := rm -rf
MKDIR   := mkdir -p

# ---- Project ----------------------------------------------------------------
TARGET  := i2c_sim
BUILD   := build

# ---- Compiler flags ---------------------------------------------------------
CSTD    := -std=c11
WARN    := -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align \
           -Wstrict-prototypes -Wmissing-prototypes -Wno-unused-parameter
OPT     := -O2
DEP     := -MMD -MP
INC     := -I. -Icore -Ibus -Imaster -Islave -Iarbitration -Iprotocol -Iui

CFLAGS  := $(CSTD) $(WARN) $(OPT) $(DEP) $(INC)
LDFLAGS :=
LDLIBS  :=

# ---- Sources ----------------------------------------------------------------
SRCS := \
    main.c \
    bus/i2c_bus.c \
    master/i2c_master.c \
    slave/i2c_slave.c \
    arbitration/i2c_arbitration.c \
    protocol/i2c_protocol.c \
    ui/i2c_terminal_ui.c

OBJS := $(SRCS:%.c=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

# ---- Default rule -----------------------------------------------------------
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "  LD     $@"
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD)/%.o: %.c
	@$(MKDIR) $(dir $@)
	@echo "  CC     $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# ---- Auto-generated dependency files ---------------------------------------
-include $(DEPS)

# ---- Phony helpers ---------------------------------------------------------
.PHONY: clean distclean run rebuild
clean:
	@echo "  CLEAN  $(BUILD)/"
	@$(RMDIR) $(BUILD)

distclean: clean
	@echo "  CLEAN  $(TARGET)"
	@$(RM) $(TARGET)

rebuild: distclean all

run: $(TARGET)
	./$(TARGET)
