CC := gcc
MODE ?= release

SRC_DIR := src
APP_DIR := apps
INC_DIR := include
BUILD_ROOT := build
BUILD_DIR := $(BUILD_ROOT)/$(MODE)
BIN_DIR := bin
MODE_STAMP := $(BIN_DIR)/.mode

CFLAGS_COMMON := -Wall -Wextra -I$(INC_DIR) -MMD -MP
CFLAGS_debug := -g -O0 -DDEBUG
CFLAGS_release := -O2 -DNDEBUG
CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_$(MODE))

APP_FILES := $(wildcard $(APP_DIR)/*_main.c)
COMMON_SRC := $(wildcard $(SRC_DIR)/common.c $(SRC_DIR)/file_ops.c)

COMMON_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMMON_SRC))
ADD_OBJS := $(BUILD_DIR)/add_main.o $(BUILD_DIR)/add_student.o $(COMMON_OBJS)
SEARCH_OBJS := $(BUILD_DIR)/search_main.o $(BUILD_DIR)/search_student.o $(COMMON_OBJS)
DISPLAY_OBJS := $(BUILD_DIR)/display_main.o $(BUILD_DIR)/display_student.o $(COMMON_OBJS)
APP_OBJS := $(patsubst $(APP_DIR)/%.c,$(BUILD_DIR)/%.o,$(APP_FILES))
TARGET_OBJS := $(sort $(ADD_OBJS) $(SEARCH_OBJS) $(DISPLAY_OBJS))
DEPS := $(TARGET_OBJS:.o=.d) $(APP_OBJS:.o=.d)

TARGETS := $(BIN_DIR)/add_student $(BIN_DIR)/search_student $(BIN_DIR)/display_student

.PHONY: all debug release clean distclean run FORCE

all: $(TARGETS)

debug:
	$(MAKE) MODE=debug all

release:
	$(MAKE) MODE=release all

$(BIN_DIR)/add_student: $(MODE_STAMP) $(ADD_OBJS) | $(BIN_DIR)
	$(CC) $(filter %.o,$^) -o $@

$(BIN_DIR)/search_student: $(MODE_STAMP) $(SEARCH_OBJS) | $(BIN_DIR)
	$(CC) $(filter %.o,$^) -o $@

$(BIN_DIR)/display_student: $(MODE_STAMP) $(DISPLAY_OBJS) | $(BIN_DIR)
	$(CC) $(filter %.o,$^) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(APP_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

$(MODE_STAMP): FORCE | $(BIN_DIR)
	@current="$$(cat $@ 2>/dev/null || true)"; \
	if [ "$$current" != "$(MODE)" ]; then \
		printf '%s\n' "$(MODE)" > $@; \
	fi

run: $(BIN_DIR)/display_student
	./$(BIN_DIR)/display_student

clean:
	rm -rf $(BUILD_ROOT) $(BIN_DIR)

distclean: clean
	rm -f students.dat

-include $(DEPS)
