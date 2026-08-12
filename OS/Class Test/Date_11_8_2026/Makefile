# Simple Makefile to build kernel modules listed

obj-m += chardev_ioctl.o gpio_irq.o irq_circbuf.o
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: all clean

all:
	$(MAKE) -C "$(KDIR)" M="$(PWD)" modules

clean:
	$(MAKE) -C "$(KDIR)" M="$(PWD)" clean
