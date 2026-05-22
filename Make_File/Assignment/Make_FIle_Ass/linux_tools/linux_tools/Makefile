CC := gcc
MODE ?= release

SRC_DIR := src
INC_DIR := include
BUILD_ROOT := build
BUILD_DIR := $(BUILD_ROOT)/$(MODE)
BIN_DIR := bin
INSTALL_DIR := $(BIN_DIR)/installed
MODE_STAMP := $(BIN_DIR)/.mode
PACKAGE := linux_tools.tar.gz

CFLAGS_COMMON := -Wall -Wextra -I$(INC_DIR) -MMD -MP
CFLAGS_debug := -g -O0 -DDEBUG
CFLAGS_release := -O2 -DNDEBUG
CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_$(MODE))

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
PROGRAM_SRC := $(filter-out $(SRC_DIR)/utils.c,$(SRC_FILES))
PROGRAMS := $(patsubst $(SRC_DIR)/%.c,%,$(PROGRAM_SRC))
TARGETS := $(patsubst %,$(BIN_DIR)/%,$(PROGRAMS))

UTIL_OBJ := $(BUILD_DIR)/utils.o
PROGRAM_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(PROGRAM_SRC))
DEPS := $(UTIL_OBJ:.o=.d) $(PROGRAM_OBJS:.o=.d)

.PHONY: all clean install uninstall debug release package FORCE
.SECONDARY: $(UTIL_OBJ) $(PROGRAM_OBJS)

all: $(TARGETS)

debug:
	$(MAKE) MODE=debug all

release:
	$(MAKE) MODE=release all

$(TARGETS): $(BIN_DIR)/%: $(MODE_STAMP) $(BUILD_DIR)/%.o $(UTIL_OBJ) | $(BIN_DIR)
	$(CC) $(filter %.o,$^) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR) $(INSTALL_DIR):
	mkdir -p $@

$(MODE_STAMP): FORCE | $(BIN_DIR)
	@current="$$(cat $@ 2>/dev/null || true)"; \
	if [ "$$current" != "$(MODE)" ]; then \
		printf '%s\n' "$(MODE)" > $@; \
	fi

install: all | $(INSTALL_DIR)
	cp $(TARGETS) $(INSTALL_DIR)/

uninstall:
	rm -f $(patsubst %,$(INSTALL_DIR)/%,$(PROGRAMS))

package: all
	tar -czf $(PACKAGE) $(BIN_DIR) include src Makefile

clean:
	rm -rf $(BUILD_ROOT) $(BIN_DIR) $(PACKAGE)

-include $(DEPS)
