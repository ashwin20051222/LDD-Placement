obj-m += gpio_irq_driver.o

# Get current kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	@echo "Please run as root to install:"
	@echo "sudo insmod gpio_irq_driver.ko"

uninstall:
	@echo "Please run as root to uninstall:"
	@echo "sudo rmmod gpio_irq_driver"

setup:
	@echo "Setting up device nodes..."
	@echo "Run: sudo mknod /dev/gpioirqdev c $$(grep gpioirqdev /proc/devices | awk '{print $$1}') 0"

setup-auto:
	@major=$$(grep gpioirqdev /proc/devices | awk '{print $$1}'); \
	if [ -z "$$major" ]; then \
		echo "Device not loaded. Run: sudo insmod gpio_irq_driver.ko"; \
	else \
		echo "Creating device node with major number $$major"; \
		if [ -e /dev/gpioirqdev ]; then \
			sudo rm -f /dev/gpioirqdev; \
		fi; \
		sudo mknod /dev/gpioirqdev c $$major 0; \
		sudo chmod 666 /dev/gpioirqdev; \
		echo "Device node created successfully"; \
	fi

dmesg-follow:
	dmesg -f kern --level info,notice,warn,err | tail -f

help:
	@echo "Usage:"
	@echo "  make              - Build the kernel module"
	@echo "  make clean        - Clean build files"
	@echo "  make install      - Show installation command"
	@echo "  make uninstall    - Show uninstall command"
	@echo "  make setup-auto   - Automatically create device node"
	@echo "  make dmesg-follow - Follow kernel logs in real-time"
	@echo "  make help         - Show this help message"

.PHONY: all clean install uninstall setup setup-auto dmesg-follow help
