# Makefile for Keyboard Timeout Kernel Module
# This file builds the kernel module

obj-m += keyboard_timeout.o

KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

.PHONY: all clean

all:
	@echo "Building keyboard_timeout kernel module..."
	@echo "Kernel version: $(shell uname -r)"
	make -C $(KDIR) M=$(PWD) modules

clean:
	@echo "Cleaning build files..."
	make -C $(KDIR) M=$(PWD) clean
	@echo "Cleaned successfully"

user_app: user_app.c
	@echo "Compiling user application..."
	gcc user_app.c -o user_app
	@echo "User app compiled: ./user_app"

help:
	@echo "Available targets:"
	@echo "  make              - Build kernel module"
	@echo "  make user_app     - Compile user application"
	@echo "  make clean        - Clean build files"
	@echo "  make help         - Show this help message"
