# Keyboard Interrupt Timeout Using Jiffies - Assignment

## Overview

This assignment demonstrates how Linux kernel uses **jiffies** for timing operations, specifically for detecting keyboard inactivity and triggering timeouts.

### What This Program Does

- **Kernel Module** captures keyboard interrupts (IRQ 1)
- **Jiffies-based Timer** checks for inactivity every second
- **Timeout Detection** flags when keyboard is inactive for 5 seconds
- **Character Device** exposes status to user-space applications
- **User Application** monitors and displays timeout events

---

## Prerequisites

Before running this assignment, ensure you have:

1. **Linux kernel headers** installed
2. **Build tools** (gcc, make)
3. **Root/sudo access** (required for kernel operations)
4. **Text editor** (nano, vim, or similar)

### Installing Prerequisites (Ubuntu/Debian)

```bash
# Update package manager
sudo apt update

# Install kernel headers for your kernel version
sudo apt install linux-headers-$(uname -r)

# Install build essentials
sudo apt install build-essential

# Verify installation
gcc --version
make --version
```

---

## Project Structure

```
Assignment/
├── keyboard_timeout.c       # Kernel module source code
├── user_app.c              # User-space application
├── Makefile                # Build configuration
└── README.md              # This file
```

---

## Step-by-Step Compilation and Execution Guide

### Step 1: Navigate to Assignment Folder

```bash
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/Jiffies/Assignment"
```

### Step 2: Compile the Kernel Module

```bash
make
```

**Expected Output:**
```
Building keyboard_timeout kernel module...
Kernel version: 5.15.0-56-generic
make -C /lib/modules/5.15.0-56-generic/build M=/home/... modules
  CC [M]  /home/.../keyboard_timeout.o
  MODPOST /home/.../Module.symvers
  CC [M]  /home/.../keyboard_timeout.mod.o
  LD [M]  /home/.../keyboard_timeout.ko
```

**Output Files Generated:**
- `keyboard_timeout.ko` - Compiled kernel module (binary)
- `keyboard_timeout.o` - Object file
- `keyboard_timeout.mod.o` - Module object file

### Step 3: Compile the User Application

```bash
make user_app
```

**Expected Output:**
```
Compiling user application...
gcc user_app.c -o user_app
User app compiled: ./user_app
```

**Output File:**
- `user_app` - Compiled user-space executable

### Step 4: Load the Kernel Module

```bash
sudo insmod keyboard_timeout.ko
```

**What happens:**
- Module initializes and registers IRQ 1 (keyboard)
- Sets up a timer for periodic timeout checking
- Creates a character device

### Step 5: Check Kernel Logs

```bash
dmesg | tail -20
```

**Expected Output:**
```
[timestamp] ========================================
[timestamp] Keyboard Timeout Driver Initialization
[timestamp] ========================================
[timestamp] Character Device Registered
[timestamp] Major Number = 240
[timestamp] Create device with: sudo mknod /dev/kb_timeout c 240 0
[timestamp] Keyboard IRQ (IRQ 1) Registered
[timestamp] Timer Started (checking every 1 second)
[timestamp] Timeout Duration: 5 seconds
```

**Note the Major Number** (in this example: 240)

### Step 6: Create Device File

Get the major number from dmesg output, then create the device file:

```bash
# Replace 240 with your actual major number
sudo mknod /dev/kb_timeout c 240 0

# Verify device creation
ls -l /dev/kb_timeout
```

**Expected Output:**
```
crw-r--r-- 1 root root 240, 0 May 27 10:15 /dev/kb_timeout
```

### Step 7: Grant Permissions (Optional but Recommended)

To run the user app without sudo:

```bash
sudo chmod 666 /dev/kb_timeout
```

### Step 8: Run the User Application

**In a new terminal window** (keep the first one open to monitor dmesg):

```bash
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/Jiffies/Assignment"
./user_app
```

**Expected Output:**
```
========================================
Keyboard Timeout Monitor
========================================
Opening device: /dev/kb_timeout
Device opened successfully (fd = 3)
========================================
Monitoring keyboard activity...
(Press keys to reset timeout, or wait 5 seconds for timeout)
Press Ctrl+C to stop monitoring
========================================

[  1] No timeout
[  2] No timeout
[  3] No timeout
```

### Step 9: Test Timeout Detection

**Scenario 1: Type to Reset Timeout**

```
While typing in the first terminal:

[  1] No timeout
[  2] No timeout
[  3] No timeout

# (switch to first terminal and press keys)

[  4] No timeout          # Resets with each keypress
[  5] No timeout
```

**Scenario 2: Trigger Timeout**

```
Without pressing keys for 5 seconds:

[  1] No timeout
[  2] No timeout
[  3] No timeout
[  4] No timeout
[  5] No timeout

# (After 5 seconds of inactivity)

[  6] Keyboard timeout detected   # Timeout triggered!
[  7] No timeout
[  8] No timeout
```

### Step 10: Monitor Kernel Logs in Real-Time

In another terminal, watch kernel logs as they happen:

```bash
dmesg -w
```

**You'll see logs like:**
```
[timestamp] Keyboard Activity Detected at jiffies = 429201890
[timestamp] Keyboard Activity Detected at jiffies = 429201950
[timestamp] Keyboard Activity Detected at jiffies = 429202010
[timestamp] Keyboard Timeout Occurred (elapsed: 5120 jiffies)
```

### Step 11: Unload the Module

When finished testing:

```bash
sudo rmmod keyboard_timeout
```

**Expected Output in dmesg:**
```
[timestamp] ========================================
[timestamp] Keyboard Timeout Driver Cleanup
[timestamp] ========================================
[timestamp] Timer Stopped
[timestamp] Keyboard IRQ Released
[timestamp] Character Device Unregistered
[timestamp] Driver Removed Successfully
```

### Step 12: Clean Up (Optional)

Remove compiled files:

```bash
make clean
```

Also remove device file:

```bash
sudo rm /dev/kb_timeout
```

---

## Complete Execution Flow (Quick Reference)

### Terminal 1: Module Setup

```bash
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/Jiffies/Assignment"

# Compile
make
make user_app

# Load module
sudo insmod keyboard_timeout.ko

# Check major number
dmesg | tail -5

# Create device (replace 240 with your major number)
sudo mknod /dev/kb_timeout c 240 0

# Grant permissions
sudo chmod 666 /dev/kb_timeout

# Watch logs
dmesg -w
```

### Terminal 2: Run User Application

```bash
cd "/home/silicondoc/Documents/Class file/LDD/Kernal/Jiffies/Assignment"

# Run application
./user_app

# Type to reset timeout or wait 5 seconds for timeout
```

### Terminal 1 (After Testing): Cleanup

```bash
# Unload module
sudo rmmod keyboard_timeout

# Remove device file
sudo rm /dev/kb_timeout

# Clean build files
make clean
```

---

## Understanding Jiffies in This Code

### Key Jiffies Concepts Used

**1. Storing Time**
```c
last_activity = jiffies;  // Store current kernel tick count
```

**2. Calculating Elapsed Time**
```c
unsigned long elapsed = jiffies - last_activity;  // Difference in ticks
```

**3. Converting Seconds to Jiffies**
```c
secs_to_jiffies(TIMEOUT_SEC)  // Portable conversion (depends on HZ)
```

**4. Safe Comparison**
```c
if (elapsed > secs_to_jiffies(TIMEOUT_SEC))  // Safe comparison
```

### How Timeout Works

If HZ = 1000 (1000 ticks per second):

```
Time Flow:
  last_activity = 5000 jiffies
  
  After 1 second:
  current jiffies = 6000
  elapsed = 6000 - 5000 = 1000 jiffies = 1 second ✓
  
  After 5 seconds (no keypress):
  current jiffies = 10000
  elapsed = 10000 - 5000 = 5000 jiffies = 5 seconds ✓
  → TIMEOUT TRIGGERED!
  
  Key pressed:
  last_activity = 10000 (updated)
  elapsed resets
```

---

## Troubleshooting

### Issue: Permission Denied When Inserting Module

**Solution:**
```bash
# Use sudo
sudo insmod keyboard_timeout.ko
```

### Issue: Module Not Found or Failed to Load

**Solution:**
```bash
# Check if module exists
ls -l keyboard_timeout.ko

# Check kernel logs for error
dmesg | tail -20

# Try recompiling
make clean
make
sudo insmod keyboard_timeout.ko
```

### Issue: Device File Creation Fails

**Solution:**
```bash
# Get correct major number
dmesg | grep "Major Number"

# Example output: Major Number = 240
# Then create with correct number
sudo mknod /dev/kb_timeout c 240 0
```

### Issue: User App Says "Failed to open /dev/kb_timeout"

**Possible causes:**
1. Device file not created
2. Module not loaded
3. Permission denied

**Solution:**
```bash
# Check device exists
ls -l /dev/kb_timeout

# Check module loaded
lsmod | grep keyboard_timeout

# Fix permissions
sudo chmod 666 /dev/kb_timeout

# Try again
./user_app
```

### Issue: "Keyboard Activity Detected" Not Appearing

**Possible causes:**
1. IRQ 1 may be handled by other drivers
2. Different keyboard setup (USB keyboard uses different IRQ)

**Solution:**
```bash
# Check IRQ usage
cat /proc/interrupts | grep -i keyboard

# Check kernel logs
dmesg | tail -20
```

---

## Important Notes

### IRQ 1 - Standard PS/2 Keyboard

- IRQ 1 is for legacy PS/2 keyboards
- USB keyboards use different IRQs
- The module uses `IRQF_SHARED` flag to share the IRQ with other handlers

### Timeout Value

- Default timeout is **5 seconds** (`TIMEOUT_SEC 5`)
- To modify, edit `keyboard_timeout.c` line 25:
```c
#define TIMEOUT_SEC 5  // Change this value
```
Then recompile: `make`

### Timer Resolution

- Timer checks every **1 second** (HZ jiffies)
- Actual timeout may vary slightly due to scheduler
- This is not hard real-time accurate

---

## Assignment Questions to Explore

After running the code, try to answer:

1. **What is jiffies?**
   - Answer: Global kernel tick counter incremented at each timer interrupt

2. **Why use jiffies instead of sleep()?**
   - Answer: Interrupt handlers cannot sleep; jiffies provide non-blocking timing

3. **What happens when jiffies overflows?**
   - Answer: It wraps around; Linux macros handle overflow safely

4. **Why check timeout in timer callback instead of IRQ handler?**
   - Answer: IRQ handlers should be short; timeout logic runs outside interrupt context

5. **What is HZ and how does it affect jiffies?**
   - Answer: HZ = ticks per second; affects jiffy duration (1 jiffy = 1/HZ seconds)

6. **Why use character device instead of printing to kernel log?**
   - Answer: Allows structured user-space communication; multiple applications can monitor

---

## Enhancement Ideas

1. **Make timeout configurable** via /proc interface
2. **Add specific key detection** - detect only certain keys
3. **Use wait queues** - replace polling with blocking I/O
4. **Add /proc interface** - make timeout adjustable without recompilation
5. **Use hrtimers** - for higher precision timing
6. **Add debouncing** - ignore key chatter
7. **Log statistics** - count timeouts, track patterns

---

## References

- Linux Kernel Jiffies Documentation: `include/linux/jiffies.h`
- Kernel Timer Guide: `include/linux/timer.h`
- Character Device Driver: Linux Device Drivers, 3rd Edition
- Interrupt Handling: Linux Kernel Internals

---

## Author

- **Course**: Linux Device Driver (LDD)
- **Topic**: Jiffies and Kernel Timing
- **Date**: 2026

---

## License

This code is provided under the **GPL (GNU Public License)** v2.0

```
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
```

---

## Support

For issues or questions:
1. Check kernel logs: `dmesg | tail -20`
2. Verify module loaded: `lsmod | grep keyboard_timeout`
3. Check device exists: `ls -l /dev/kb_timeout`
4. Review this README for troubleshooting section

---

**Good luck with your Linux Device Driver assignment! 🐧**
