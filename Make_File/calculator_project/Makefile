# Compiler
CC = gcc

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
LIB_DIR = lib

# Target
TARGET = calculator

# Static Library
LIBRARY = $(LIB_DIR)/libcalc.a

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Dependency files
DEPS = $(OBJS:.o=.d)

# Compiler Flags
CFLAGS = -I$(INC_DIR) -Wall -MMD -MP

# Debug Flags
DEBUG_FLAGS = -g -O0

# Release Flags
RELEASE_FLAGS = -O2

# Archive tool
AR = ar
ARFLAGS = rcs

# Default target
all: release

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: directories $(TARGET)

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: directories $(TARGET)

# Create required directories
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

# Final executable
$(TARGET): $(LIBRARY) $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o -L$(LIB_DIR) -lcalc -o $(TARGET)

# Static library creation
$(LIBRARY): $(filter-out $(BUILD_DIR)/main.o,$(OBJS))
	$(AR) $(ARFLAGS) $@ $^

# Object file generation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(LIB_DIR)
	rm -f $(TARGET)

# Phony targets
.PHONY: all debug release clean directories
