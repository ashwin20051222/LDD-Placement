# GPIO IRQ Driver with ioctl & poll() - Complete Guide

##  Project Overview

This is a **complete GPIO interrupt driver** for Linux kernel with:
-  **ioctl Interface** - Device control commands
-  **poll() Support** - Non-blocking event waiting
-  **IRQ Handling** - Interrupt processing
-  **Thread-Safe** - Mutex protection
-  **Production Ready** - Proper error handling

### What This Driver Does
- Simulates a GPIO interrupt pin (like a button or sensor)
- Allows user-space apps to configure GPIO settings via **ioctl**
- Triggers interrupts and wakes user-space processes with **poll**
- Counts IRQ events and provides real-time notifications

---

##  Project Structure

```
Assignment/
 gpio_irq_driver.c          # Kernel module source
 gpio_user_app.c            # User application (waits for interrupts)
 gpio_test_trigger.c        # Test trigger application (optional)
 Makefile                   # Build instructions
 README.md                  # This file
 CONCEPTS.md                # Detailed technical concepts
 INTERVIEW_QA.md            # Interview Q&A guide
```

---

##  Prerequisites

### System Requirements
- Linux kernel with development headers
- `make` and `gcc` installed
- Root/sudo access for module installation

### Check Your System

```bash
# Check kernel version
uname -r

# Check if kernel headers are installed
ls /lib/modules/$(uname -r)/build

# Check for build tools
which gcc make
```

### Install Missing Tools (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install -y build-essential linux-headers-$(uname -r)

# Verify installation
gcc --version
make --version
```

---

##  Building the Module

### Step 1: Navigate to Project Directory

```bash
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/IOCTL/Assignment"
```

### Step 2: Build the Kernel Module

```bash
# Clean old builds
make clean

# Build the module
make

# Expected output:
# make -C /lib/modules/5.x.x-xx-generic/build M=/path/to/Assignment modules
# ...
# Building modules, stage 2.
# ...
```

### Troubleshooting Build Errors

**Error: `-bash: make: command not found`**
```bash
sudo apt install build-essential
```

**Error: `fatal error: linux/module.h: No such file or directory`**
```bash
sudo apt install linux-headers-$(uname -r)
```

**Error: `permission denied`**
```bash
# Make sure you have write permissions
ls -la | head -5
```

---

##  Installing the Module

### Step 1: Load the Kernel Module

```bash
# Load the module
sudo insmod gpio_irq_driver.ko

# Verify it's loaded
lsmod | grep gpio_irq_driver

# Expected output:
# gpio_irq_driver         16384  0
```

### Step 2: Check Kernel Logs

```bash
# View kernel messages
dmesg | tail -5

# Expected output:
# [xxxxx.xxxxx] GPIO IRQ ioctl driver loaded.
# [xxxxx.xxxxx] Device Name: /dev/gpioirqdev
# [xxxxx.xxxxx] Major Number: 242
# [xxxxx.xxxxx] Create device node: sudo mknod /dev/gpioirqdev c 242 0
```

### Step 3: Create Device Node

```bash
# Get the major number from dmesg
MAJOR=$(grep gpioirqdev /proc/devices | awk '{print $1}')
echo "Major number: $MAJOR"

# Create device node
sudo mknod /dev/gpioirqdev c $MAJOR 0

# Set permissions
sudo chmod 666 /dev/gpioirqdev

# Verify device was created
ls -la /dev/gpioirqdev

# Expected output:
# crw-rw-rw- 1 root root 242, 0 May 27 09:30 /dev/gpioirqdev
```

### Automatic Device Node Creation

Use the Makefile helper:

```bash
# Automatically create device node with correct major number
make setup-auto

# Output:
# Creating device node with major number XXX
# Device node created successfully
```

### Troubleshooting Installation

**Module fails to load**
```bash
# Check system logs
sudo dmesg | tail -20

# Try verbose loading
sudo insmod gpio_irq_driver.ko debug=1

# Check module dependencies
modinfo gpio_irq_driver.ko
```

**Cannot create device node**
```bash
# Remove old device node
sudo rm -f /dev/gpioirqdev

# Recreate with correct permissions
sudo mknod /dev/gpioirqdev c $(grep gpioirqdev /proc/devices | awk '{print $1}') 0
sudo chmod 666 /dev/gpioirqdev
```

---

##  Running the Applications

### Option 1: Monitor GPIO Interrupts (Default)

```bash
# Compile user application
gcc -o gpio_user_app gpio_user_app.c

# Run it
./gpio_user_app

# Expected output:
# ====================================
#  GPIO IRQ Driver - User Application 
# ====================================
# 
# [+] Opening device: /dev/gpioirqdev
# [+] Device opened successfully (fd=3)
# [+] Setting GPIO pin: 17
# [+] Setting edge type: 0 (0=rising, 1=falling, 2=both)
# [+] Enabling GPIO IRQ
# 
# ====================================
#  Waiting for GPIO interrupts...
#  (Press Ctrl+C to exit)
# ====================================
#
# [IRQ #1] GPIO INTERRUPT DETECTED!
#          Event: 1 (timestamp: 1748343892000 ms)
# (waiting for more interrupts...)
```

### Option 2: Monitor GPIO with Specific Pin & Edge

```bash
# Monitor pin 26 with falling edge
./gpio_user_app 26 1

# Monitor pin 18 with both edges
./gpio_user_app 18 2
```

### Option 3: Run Test Trigger (Simulates Interrupts)

In **one terminal**, start the user app:
```bash
./gpio_user_app
```

In **another terminal**, trigger simulated interrupts:
```bash
# Compile test trigger
gcc -o gpio_test_trigger gpio_test_trigger.c

# Trigger 5 events with 2-second delay
./gpio_test_trigger 5 2

# Output:
# ====================================
#  GPIO Test Trigger Application
# ====================================
# 
# [+] Opening device: /dev/gpioirqdev
# [+] Device opened successfully
# [+] Setting GPIO pin: 17
# [+] Enabling GPIO IRQ
# 
# ====================================
# [Event 1] Simulating GPIO interrupt...
#          (Waiting 2 seconds before next event)
# [Event 2] Simulating GPIO interrupt...
# ...
```

---

##  Complete Terminal Workflow Example

### Terminal 1: Build & Load Module

```bash
# Change to project directory
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/IOCTL/Assignment"

# Build
make clean && make

# Load module
sudo insmod gpio_irq_driver.ko

# Create device node
make setup-auto

# Follow kernel logs in real-time
make dmesg-follow
```

### Terminal 2: Compile & Run User App

```bash
# Change to project directory
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/IOCTL/Assignment"

# Compile user app
gcc -o gpio_user_app gpio_user_app.c

# Run (waits for interrupts)
./gpio_user_app
```

### Terminal 3: Trigger Events

```bash
# Change to project directory
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/IOCTL/Assignment"

# Compile test trigger
gcc -o gpio_test_trigger gpio_test_trigger.c

# Run test (triggers 10 interrupts)
./gpio_test_trigger 10 1
```

---

##  ioctl Commands Reference

### Command Format

```c
#define GPIO_MAGIC 'G'
#define GPIO_SET_PIN     _IOW(GPIO_MAGIC, 1, int)
#define GPIO_SET_EDGE    _IOW(GPIO_MAGIC, 2, int)
#define GPIO_ENABLE_IRQ  _IO(GPIO_MAGIC, 3)
#define GPIO_DISABLE_IRQ _IO(GPIO_MAGIC, 4)
#define GPIO_GET_EVENT   _IOR(GPIO_MAGIC, 5, int)
```

### Commands Explained

| Command | Type | Purpose | Argument |
|---------|------|---------|----------|
| `GPIO_SET_PIN` | _IOW | Set GPIO pin number | `int pin_number` |
| `GPIO_SET_EDGE` | _IOW | Set edge type | `int edge` (0=rising, 1=falling, 2=both) |
| `GPIO_ENABLE_IRQ` | _IO | Enable interrupts | None |
| `GPIO_DISABLE_IRQ` | _IO | Disable interrupts | None |
| `GPIO_GET_EVENT` | _IOR | Read event status | `int *event` |

### Usage Example

```c
int fd = open("/dev/gpioirqdev", O_RDWR);
int pin = 17;
int edge = 0;  // rising edge

ioctl(fd, GPIO_SET_PIN, &pin);
ioctl(fd, GPIO_SET_EDGE, &edge);
ioctl(fd, GPIO_ENABLE_IRQ);  // No argument needed

// Wait for interrupt with poll()
struct pollfd pfd = {fd, POLLIN};
int ret = poll(&pfd, 1, -1);

// Get event
int event;
ioctl(fd, GPIO_GET_EVENT, &event);

close(fd);
```

---

##  Testing Scenarios

### Test 1: Basic Module Loading

```bash
# Load and verify
sudo insmod gpio_irq_driver.ko
lsmod | grep gpio_irq_driver
sudo rmmod gpio_irq_driver
dmesg | tail -10
```

**Expected Result**: Module loads/unloads without errors

---

### Test 2: Device Node Creation

```bash
# Load module
sudo insmod gpio_irq_driver.ko

# Get major number and create node
MAJOR=$(grep gpioirqdev /proc/devices | awk '{print $1}')
sudo mknod /dev/gpioirqdev c $MAJOR 0
ls -la /dev/gpioirqdev

# Test read/write permissions
echo "test" | sudo dd of=/dev/gpioirqdev 2>&1 | head -5
```

**Expected Result**: Device node exists with correct permissions

---

### Test 3: User Application

```bash
# Terminal 1: Load module and follow logs
sudo insmod gpio_irq_driver.ko
make dmesg-follow

# Terminal 2: Run user app
gcc -o gpio_user_app gpio_user_app.c
./gpio_user_app

# Press Ctrl+C to stop
```

**Expected Result**: App runs without segfaults, waits for interrupts

---

### Test 4: End-to-End with Trigger

```bash
# Terminal 1: Load and follow logs
sudo insmod gpio_irq_driver.ko
make dmesg-follow &

# Terminal 2: Run user app
gcc -o gpio_user_app gpio_user_app.c
./gpio_user_app &

# Terminal 3: Trigger events
gcc -o gpio_test_trigger gpio_test_trigger.c
./gpio_test_trigger 5 1
```

**Expected Result**: User app detects all 5 triggered interrupts

---

##  Debugging & Troubleshooting

### View Kernel Logs

```bash
# Real-time kernel logs
dmesg -f kern --level info,notice,warn,err | tail -f

# Or with our Makefile helper
make dmesg-follow

# Filter by driver name
dmesg | grep gpioirqdev
```

### Check Module Status

```bash
# List loaded modules
lsmod | grep gpio

# Get module info
modinfo gpio_irq_driver.ko

# Check module parameters
cat /sys/module/gpio_irq_driver/parameters/*

# View module in proc
ls -la /proc/modules | grep gpio
```

### Debug User Application

```bash
# Compile with debug symbols
gcc -g -o gpio_user_app gpio_user_app.c

# Run with strace to trace system calls
strace ./gpio_user_app

# Use gdb debugger
gdb ./gpio_user_app
(gdb) run
(gdb) backtrace
(gdb) quit
```

### Check Device Node

```bash
# Verify device exists
ls -la /dev/gpioirqdev

# Check permissions
stat /dev/gpioirqdev

# Remove and recreate if needed
sudo rm -f /dev/gpioirqdev
sudo mknod /dev/gpioirqdev c $(grep gpioirqdev /proc/devices | awk '{print $1}') 0
sudo chmod 666 /dev/gpioirqdev
```

---

##  Cleanup & Uninstallation

### Remove Module

```bash
# Unload the module
sudo rmmod gpio_irq_driver

# Remove device node
sudo rm -f /dev/gpioirqdev

# Clean build files
make clean

# Verify removal
lsmod | grep gpio
ls /dev/gpioirqdev  # Should show "No such file"
```

### Full Cleanup Script

```bash
#!/bin/bash
echo "Removing GPIO IRQ driver..."
sudo rmmod gpio_irq_driver 2>/dev/null || true
sudo rm -f /dev/gpioirqdev
make clean
echo "Cleanup complete"
```

---

##  Technical Concepts

### How poll() Works

```
User App              Kernel Driver
         
poll(fd)  > poll_wait()
(blocks)              registers in waitqueue

                      [IRQ occurs]
                      gpio_irq_handler()
                      wake_up_interruptible()
                      
poll() returns < woken up
(data ready)          event_flag = 1
```

### How ioctl Works

```
User App                    Kernel
               
ioctl(fd, cmd, arg) > unlocked_ioctl()
                           switch(cmd) {
                             case GPIO_SET_PIN:
                             case GPIO_ENABLE_IRQ:
                           }
                  <  return 0/error
```

### Thread Safety

The driver uses mutex locking to prevent race conditions:

```c
mutex_lock(&gpio_lock);
{
    // Protected section
    gpio_value = new_value;
    event_flag = 1;
}
mutex_unlock(&gpio_lock);
```

---

##  Learning Resources

### Inside This Project

1. **gpio_irq_driver.c** - Main kernel module
   - Shows IRQ handler implementation
   - Demonstrates ioctl interface
   - Shows poll support
   - Thread-safe design with mutexes

2. **gpio_user_app.c** - User-space application
   - ioctl system call usage
   - poll() for event waiting
   - Signal handling
   - Proper error handling

3. **Makefile** - Build automation
   - Kernel module compilation
   - Device node management
   - Debugging helpers

### Key Kernel APIs Used

```c
// Character device registration
register_chrdev(major, name, &fops);
unregister_chrdev(major, name);

// ioctl dispatch
static long (*unlocked_ioctl)(struct file *, unsigned int, unsigned long);

// Data transfer between user/kernel
copy_from_user(kernel_ptr, user_ptr, size);
copy_to_user(user_ptr, kernel_ptr, size);

// Interrupt handling
wake_up_interruptible(&waitqueue);
poll_wait(file, &waitqueue, wait_table);

// Synchronization
DEFINE_MUTEX(lock);
mutex_lock(&lock);
mutex_unlock(&lock);
```

---

##  Related Commands

### Build System

```bash
make              # Build the kernel module
make clean        # Remove build artifacts
make help         # Show available targets
make setup-auto   # Automatically create device node
make dmesg-follow # Follow kernel logs in real-time
```

### Module Management

```bash
sudo insmod gpio_irq_driver.ko           # Load module
sudo rmmod gpio_irq_driver               # Unload module
lsmod                                     # List loaded modules
modinfo gpio_irq_driver.ko                # Show module info
dmesg | tail -20                          # View kernel logs
```

### User Application

```bash
gcc -o gpio_user_app gpio_user_app.c      # Compile user app
./gpio_user_app                           # Run with defaults
./gpio_user_app 26 1 5000                 # Pin 26, falling edge, 5s timeout
./gpio_user_app -h                        # Show help
```

---

##  Important Notes

### Security Considerations
- Device node permissions should be restricted for production use
- Only authorized users should access GPIO devices
- Always validate ioctl arguments in kernel

### Performance Notes
- poll() is more efficient than busy-waiting
- Mutex overhead is minimal for typical usage
- IRQ handlers should be kept short

### Hardware Considerations
- This driver uses simulated GPIO (no real hardware access)
- For real hardware, replace simulation with actual GPIO API
- Use `gpiod` library for modern embedded systems

---

##  Quick Reference

### Common Commands

```bash
# Full workflow
cd /home/silicondoc/Documents/Class\ file/LDD/Kernal/IOCTL/Assignment
make clean && make
sudo insmod gpio_irq_driver.ko
make setup-auto
gcc -o gpio_user_app gpio_user_app.c
./gpio_user_app

# In another terminal
gcc -o gpio_test_trigger gpio_test_trigger.c
./gpio_test_trigger 10 1

# Cleanup
sudo rmmod gpio_irq_driver
make clean
```

### Troubleshooting Checklist

- [ ] Kernel headers installed: `ls /lib/modules/$(uname -r)/build`
- [ ] Module builds: `make clean && make`
- [ ] Module loads: `sudo insmod gpio_irq_driver.ko`
- [ ] Device node exists: `ls -la /dev/gpioirqdev`
- [ ] User app compiles: `gcc -o gpio_user_app gpio_user_app.c`
- [ ] User app runs: `./gpio_user_app`
- [ ] Kernel logs show no errors: `dmesg | tail -10`

---

##  File Descriptions

| File | Purpose |
|------|---------|
| `gpio_irq_driver.c` | Kernel module source with ioctl/IRQ/poll |
| `gpio_user_app.c` | User application that monitors interrupts |
| `gpio_test_trigger.c` | Simulates GPIO interrupt events |
| `Makefile` | Build system and helper targets |
| `README.md` | This comprehensive guide |

---

##  Interview Preparation

This project demonstrates:
-  Kernel module development
-  ioctl interface design
-  Interrupt handling
-  User/kernel communication
-  Synchronization primitives (mutex)
-  Event notification (poll/waitqueue)
-  Device driver architecture
-  Proper error handling

**Interview Questions You Can Answer:**
- "How does ioctl work?"
- "Explain poll() and select()"
- "How do you handle synchronization in kernel?"
- "What are edge cases in GPIO handling?"
- "How would you make this thread-safe?"

---

##  Further Reading

### Linux Kernel Documentation
- LDD (Linux Device Drivers) book
- Linux kernel source: `/usr/src/linux-headers-*/`
- Kernel API documentation

### Related Topics
- Character device drivers
- Network interfaces (netlink)
- sysfs interface
- GPIO subsystem
- Real-time extensions

---

##  Support

For issues or questions:
1. Check kernel logs: `dmesg | tail -20`
2. Review this README section by section
3. Verify all prerequisites are installed
4. Test each component independently

---

**Last Updated:** May 27, 2026  
**Version:** 1.0  
**Status:** Production Ready 
