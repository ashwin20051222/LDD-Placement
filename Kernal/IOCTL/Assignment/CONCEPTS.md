# Technical Concepts - GPIO IRQ Driver

## Table of Contents
1. [ioctl System Call](#ioctl-system-call)
2. [poll() Function](#poll-function)
3. [Interrupt Handling](#interrupt-handling)
4. [Synchronization](#synchronization)
5. [Data Flow](#data-flow)
6. [Kernel APIs](#kernel-apis)

---

## ioctl System Call

### What is ioctl?

**ioctl** = "Input/Output Control"

It's a system call used for device-specific operations that aren't covered by standard system calls like `read()`, `write()`, `open()`, `close()`.

### Why ioctl is Needed?

Standard syscalls cannot:
-  Configure hardware registers
-  Change device modes
-  Send custom commands to drivers
-  Perform control operations (reset, set baud rate, etc.)

Solution: Kernel exposes a **command-based interface via ioctl**

### User-Space API

```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long request, ...);
```

**Parameters:**
- `fd` - File descriptor of device
- `request` - Command code (ioctl command)
- `...` - Optional argument (pointer/values)

**Return Value:**
- `0` or positive - Success
- `-1` - Error (check errno)

### Kernel-Side Handler

Inside the driver:

```c
long (*unlocked_ioctl)(struct file *file, unsigned int cmd, unsigned long arg);
```

**Arguments:**
- `file` - Device file pointer
- `cmd` - ioctl command
- `arg` - User data (pointer or value)

### ioctl Command Encoding

Linux provides macros to encode commands:

```c
#define _IO(type, nr)        // no argument
#define _IOR(type, nr, data) // read from kernel
#define _IOW(type, nr, data) // write to kernel
#define _IOWR(type, nr, data)// both read/write
```

**Example:**
```c
#define MY_MAGIC 'k'

#define IOCTL_SET_VALUE _IOW(MY_MAGIC, 1, int)
#define IOCTL_GET_VALUE _IOR(MY_MAGIC, 2, int)
#define IOCTL_RESET     _IO(MY_MAGIC, 3)
```

### Why Magic Number?

```c
#define GPIO_MAGIC 'G'
```

Purpose:
- Avoid conflicts between different drivers
- Each driver gets a unique command namespace
- Ensures kernel validates ioctl commands

### Command Types Explained

| Macro | Direction | Purpose |
|-------|-----------|---------|
| `_IO` | None | Simple commands without data |
| `_IOR` | Read from kernel | Get data FROM kernel |
| `_IOW` | Write to kernel | Send data TO kernel |
| `_IOWR` | Both | Both read and write |

### Example Usage

```c
// User space
int fd = open("/dev/gpioirqdev", O_RDWR);

// Write (send data to kernel)
int pin = 17;
ioctl(fd, GPIO_SET_PIN, &pin);

// Read (get data from kernel)
int event;
ioctl(fd, GPIO_GET_EVENT, &event);

// Control (no data)
ioctl(fd, GPIO_ENABLE_IRQ);

close(fd);
```

---

## poll() Function

### What is poll()?

`poll()` allows a process to wait for events on multiple file descriptors without blocking.

### User-Space API

```c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

**Parameters:**
- `fds` - Array of `pollfd` structures
- `nfds` - Number of file descriptors to monitor
- `timeout` - Timeout in milliseconds
  - `-1` = block indefinitely
  - `0` = return immediately
  - `> 0` = wait up to timeout milliseconds

**Return Value:**
- `0` = timeout
- `> 0` = number of ready file descriptors
- `-1` = error

### pollfd Structure

```c
struct pollfd {
    int fd;       // File descriptor
    short events; // Requested events
    short revents;// Returned events
};
```

### Event Flags

**Common events:**
- `POLLIN` - Data available to read
- `POLLOUT` - Ready to write
- `POLLERR` - Error occurred
- `POLLHUP` - Connection closed
- `POLLRDNORM` - Normal data available

### Example Usage

```c
int fd = open("/dev/gpioirqdev", O_RDWR);

struct pollfd pfd;
pfd.fd = fd;
pfd.events = POLLIN;  // Wait for readable data

// Wait indefinitely for event
int ret = poll(&pfd, 1, -1);

if (ret > 0) {
    if (pfd.revents & POLLIN) {
        printf("Event ready!\n");
        // Read the event
        int event;
        ioctl(fd, GPIO_GET_EVENT, &event);
    }
}

close(fd);
```

### poll() vs Busy Loop

** Busy Loop (BAD)**
```c
while (1) {
    ioctl(fd, GPIO_GET_EVENT, &event);
    if (event) break;
    // CPU: 100% - burning power!
}
```

** poll() (GOOD)**
```c
struct pollfd pfd = {fd, POLLIN};
poll(&pfd, 1, -1);  // Sleep until ready
if (pfd.revents & POLLIN) {
    ioctl(fd, GPIO_GET_EVENT, &event);
    // CPU: 0% - sleeping efficiently!
}
```

### Benefits of poll()

1. **CPU Efficient** - Process sleeps until event
2. **Multiple FDs** - Monitor multiple devices at once
3. **Timeout Support** - Avoid infinite waits
4. **Atomic** - Kernel handles atomicity
5. **Portable** - Standard POSIX interface

---

## Interrupt Handling

### What is an IRQ?

**IRQ** = Interrupt Request

Hardware event that signals the CPU to stop current task and process the interrupt.

### IRQ Handler

```c
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    // Handle interrupt
    // Must be fast and non-blocking
    
    // Wake up sleeping processes
    wake_up_interruptible(&waitqueue);
    
    return IRQ_HANDLED;
}
```

**Requirements:**
- Must complete quickly (typically < 1ms)
- Cannot perform blocking operations (no sleep, no locks)
- Cannot access pageable memory
- Cannot use mutex (use spinlock instead)

### IRQ Handler Return Values

```c
IRQ_NONE    // Interrupt not handled
IRQ_HANDLED // Interrupt successfully handled
IRQ_RETVAL(x) // Conditional (IRQ_RETVAL(x) = IRQ_HANDLED if x, else IRQ_NONE)
```

### Requesting an IRQ

```c
// In real hardware
request_irq(
    irq_number,           // IRQ number
    gpio_irq_handler,     // Handler function
    IRQF_TRIGGER_RISING,  // Trigger on rising edge
    "gpio_irq",           // Name
    dev                   // Device identifier
);

// Later, free the IRQ
free_irq(irq_number, dev);
```

### IRQ Trigger Types

```c
IRQF_TRIGGER_RISING   // Rising edge (0 → 1)
IRQF_TRIGGER_FALLING  // Falling edge (1 → 0)
IRQF_TRIGGER_HIGH     // High level
IRQF_TRIGGER_LOW      // Low level
```

### IRQ Flags

```c
IRQF_SHARED      // Multiple drivers share IRQ
IRQF_DISABLED    // Disable IRQ during handler
IRQF_SAMPLE_RANDOM // Contribute to entropy pool
```

---

## Synchronization

### Race Conditions

**Problem:** Multiple processes accessing shared data simultaneously

```c
// BAD - Race condition!
static int counter = 0;

// Process A          Process B
counter++;           counter++;
// Result: counter = 1 (should be 2!)
```

### Mutex (Mutual Exclusion)

**Solution:** Use locks to ensure exclusive access

```c
static DEFINE_MUTEX(gpio_lock);

// Critical section
mutex_lock(&gpio_lock);
{
    // Only one process here at a time
    counter++;
}
mutex_unlock(&gpio_lock);
```

### Spinlock (For IRQ Context)

```c
static DEFINE_SPINLOCK(gpio_spinlock);

// In IRQ handler (cannot use mutex)
spin_lock(&gpio_spinlock);
{
    event_flag = 1;
}
spin_unlock(&gpio_spinlock);
```

### Locking Best Practices

1. **Lock duration** - Keep locked sections short
2. **Nesting** - Avoid nested locks (deadlock risk)
3. **Context awareness** - Use spinlock in IRQ, mutex in process context
4. **Error handling** - Always unlock on error paths

```c
// Good pattern
mutex_lock(&gpio_lock);
{
    if (error_condition)
        goto unlock;
    
    // Do work
    
    unlock:
    ; // Label for error paths
}
mutex_unlock(&gpio_lock);
```

---

## Data Flow

### Interrupt Flow

```
Hardware GPIO Pin
    ↓ (rising/falling edge)
Interrupt Signal
    ↓
CPU receives IRQ
    ↓
gpio_irq_handler() called
    ↓
event_flag = 1
wake_up_interruptible(&gpio_wq)
    ↓
Waiting process woken up
    ↓
poll() returns
```

### ioctl Flow

```
User Application
    ↓
ioctl(fd, GPIO_SET_PIN, &pin)
    ↓
VFS Layer
    ↓
Driver's unlocked_ioctl()
    ↓
switch(cmd) {
    case GPIO_SET_PIN:
        copy_from_user(&pin, arg, sizeof(pin))
        store_value(&pin)
    }
    ↓
Return to user space
```

### Data Transfer Between User/Kernel

```c
// User space → Kernel space
int value = 42;
ioctl(fd, GPIO_SET_VALUE, &value);
    ↓
// Kernel side
struct gpio_data data;
copy_from_user(&data, (int __user *)arg, sizeof(data));

// Kernel space → User space
int kernel_value = 99;
ioctl(fd, GPIO_GET_VALUE, &read_value);
    ↓
// Kernel side
copy_to_user((int __user *)arg, &kernel_value, sizeof(kernel_value));
    ↓
// User space
printf("Got value: %d\n", read_value);  // Will be 99
```

---

## Kernel APIs

### Character Device Registration

```c
// Register character device
int major = register_chrdev(
    0,              // Major number (0 = auto-assign)
    "device_name",  // Device name
    &file_operations // Operations structure
);

// Unregister
unregister_chrdev(major, "device_name");
```

### File Operations Structure

```c
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .unlocked_ioctl = device_ioctl,
    .poll = device_poll,
};
```

### Data Transfer APIs

```c
// Copy FROM user space TO kernel
int copy_from_user(
    void *to,              // Kernel buffer
    const void __user *from,// User buffer
    unsigned long n        // Number of bytes
);

// Copy FROM kernel TO user space
int copy_to_user(
    void __user *to,       // User buffer
    const void *from,      // Kernel buffer
    unsigned long n        // Number of bytes
);

// Returns: 0 on success, number of bytes not copied on error
```

### Wait Queue APIs

```c
// Declare static wait queue
static DECLARE_WAIT_QUEUE_HEAD(my_queue);

// Wait for event (can be interrupted)
wait_event_interruptible(my_queue, condition);

// Wake up all waiting processes
wake_up_interruptible(&my_queue);

// Register in poll wait queue (from poll handler)
poll_wait(file, &my_queue, wait_table);
```

### Logging

```c
// Kernel logging
printk(KERN_INFO "Info message\n");
printk(KERN_ERR "Error message\n");
printk(KERN_DEBUG "Debug message\n");

// Log levels
KERN_EMERG   // Emergency
KERN_ALERT   // Alert
KERN_CRIT    // Critical
KERN_ERR     // Error
KERN_WARNING // Warning
KERN_NOTICE  // Notice
KERN_INFO    // Info
KERN_DEBUG   // Debug
```

---

## Interview Key Points

### ioctl vs Other Interfaces

| Interface | When to Use | Pros | Cons |
|-----------|-----------|------|------|
| **ioctl** | Device control | Flexible, simple | Not standardized |
| **sysfs** | Configuration | Standardized, simple | Limited to text |
| **netlink** | Network protocols | Scalable, async | Complex |
| **procfs** | Debugging info | Easy to implement | Deprecated |

### Why ioctl is Legacy but Important

- Historical: Used before sysfs/netlink
- Real-world: Still used in vendor BSPs
- Educational: Best for learning device I/O control
- Interview: Shows understanding of kernel interfaces

### Modern Alternatives

1. **gpiod** - Modern GPIO API (preferred)
2. **sysfs** - Simple configuration
3. **netlink** - Network-oriented
4. **debugfs** - Debugging interface

### Common Interview Questions

**Q1: How does ioctl differ from read/write?**
- ioctl: Custom commands for device-specific operations
- read/write: Stream-oriented data transfer

**Q2: When would poll() block?**
- When no data is available and fd is registered in wait queue

**Q3: What's a race condition?**
- Multiple processes access shared data without synchronization

**Q4: Why use wake_up_interruptible in IRQ?**
- To wake processes waiting on poll()
- Allows efficient event notification

**Q5: Why mutex instead of spinlock?**
- Mutex: Process context, can sleep
- Spinlock: IRQ context, cannot sleep

---

**Last Updated:** May 27, 2026  
**Version:** 1.0
