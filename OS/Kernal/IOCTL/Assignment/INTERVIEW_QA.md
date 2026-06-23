# Interview Q&A - GPIO IRQ Driver

## Table of Contents
1. [Basic Concepts](#basic-concepts)
2. [ioctl Specific](#ioctl-specific)
3. [Interrupt Handling](#interrupt-handling)
4. [poll() & Event Notification](#poll--event-notification)
5. [Synchronization](#synchronization)
6. [Architecture & Design](#architecture--design)
7. [Tricky Questions](#tricky-questions)

---

## Basic Concepts

### Q1: What is ioctl and why is it needed?

**A:** ioctl (Input/Output Control) is a system call for device-specific operations not covered by standard syscalls like read/write/open.

**Why needed:**
- Standard syscalls are generic (read/write data)
- ioctl provides custom commands for devices
- Examples: set baud rate, configure GPIO, control camera

```c
// Standard syscall - generic
ssize_t read(int fd, void *buf, size_t count);

// ioctl - device-specific
int ioctl(int fd, unsigned long request, ...);
```

**Real examples:**
- Serial port: set baud rate via ioctl
- GPIO: enable/disable interrupt via ioctl
- Network interface: set IP address via ioctl

---

### Q2: Explain the user→kernel communication in ioctl.

**A:** Data flows between user and kernel space via `copy_from_user()` and `copy_to_user()`:

```
User Space              Kernel Space
             

int pin = 17
ioctl(fd, GPIO_SET_PIN, &pin)
        ↓               ↓
        call ioctl syscall
        ↓               ↓
              copy_from_user(&pin, arg, 4)
                       ↓
              gpio_pin = 17;  ← stored
                       ↓
        
```

**Why `copy_from_user()`?**
- Direct dereferencing of user pointers crashes kernel
- User memory may not be paged in
- User could modify memory concurrently
- Kernel must validate data carefully

---

### Q3: What's the difference between _IO, _IOR, _IOW, _IOWR?

**A:**

| Macro | Direction | Usage | Example |
|-------|-----------|-------|---------|
| `_IO` | None | Control-only commands | GPIO_ENABLE_IRQ |
| `_IOR` | Read from kernel | Get data from driver | GPIO_GET_VALUE |
| `_IOW` | Write to kernel | Send data to driver | GPIO_SET_VALUE |
| `_IOWR` | Both | Read AND write | GET_AND_CLEAR_STATUS |

**Example:**
```c
#define GPIO_MAGIC 'G'
#define GPIO_SET_PIN    _IOW(GPIO_MAGIC, 1, int)  // Kernel reads user data
#define GPIO_GET_EVENT  _IOR(GPIO_MAGIC, 5, int)  // Kernel writes to user
#define GPIO_ENABLE_IRQ _IO(GPIO_MAGIC, 3)        // No data transfer
```

---

## ioctl Specific

### Q4: Why is a "magic number" needed in ioctl commands?

**A:** Magic number prevents command collisions between different drivers:

```c
#define GPIO_MAGIC 'G'      // GPIO driver
#define SERIAL_MAGIC 'S'    // Serial driver
#define NET_MAGIC 'N'       // Network driver

// Each driver has separate namespace
#define GPIO_SET    _IOW(GPIO_MAGIC, 1, int)    // 0x40041G01
#define SERIAL_SET  _IOW(SERIAL_MAGIC, 1, int)  // 0x40041S01
```

**Without magic numbers:**
- Cmd 1 from GPIO might conflict with Cmd 1 from serial driver
- Kernel couldn't distinguish which driver to route command to

**Benefits:**
- Unique command namespace per driver
- Kernel can validate magic number
- Prevents accidental data corruption

---

### Q5: How does the kernel route ioctl to the correct driver?

**A:** Via the file descriptor's file_operations structure:

```
ioctl(fd, cmd, arg)
    ↓
Kernel VFS layer looks up fd
    ↓
Gets file_operations from inode
    ↓
Calls unlocked_ioctl() function pointer
    ↓
Driver's ioctl handler processes cmd
    ↓
Returns result
```

**Example:**
```c
// File descriptor points to device
int fd = open("/dev/gpioirqdev", O_RDWR);
//           ↓ inode has struct file_operations fops

// ioctl uses that file_operations struct
ioctl(fd, GPIO_SET_PIN, &pin);
// → calls fops.unlocked_ioctl(file, GPIO_SET_PIN, (long)&pin)
```

---

### Q6: What happens if ioctl gets an invalid command?

**A:** Driver returns -EINVAL error:

```c
static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case GPIO_SET_PIN:
        // Handle command
        break;
    
    default:
        return -EINVAL;  // Unknown command
    }
    
    return 0;  // Success
}
```

**User space:**
```c
int ret = ioctl(fd, INVALID_CMD);
if (ret < 0) {
    perror("ioctl");  // "Invalid argument"
}
```

---

## Interrupt Handling

### Q7: Explain the flow when a GPIO interrupt occurs.

**A:**

```
1. Hardware GPIO pin changes (rising/falling edge)
   ↓
2. CPU receives IRQ signal
   ↓
3. CPU calls gpio_irq_handler() (kernel provides context)
   ↓
4. Handler sets event_flag = 1
   ↓
5. Handler calls wake_up_interruptible(&gpio_wq)
   ↓
6. Waiting process is awakened
   ↓
7. poll() returns to user space
   ↓
8. User app calls ioctl(GPIO_GET_EVENT) to read event
```

---

### Q8: Why must IRQ handlers be fast and non-blocking?

**A:** IRQ handlers run in interrupt context with restrictions:

**Restrictions:**
-  Cannot sleep (no mutex, no wait, no I/O)
-  Cannot access pageable memory
-  Cannot use semaphores
-  Cannot call most kernel functions
-  Can use spinlock
-  Can access memory already paged in
-  Can call wake_up()

**Why?**
- System is paused while IRQ handler runs
- Long handlers delay other tasks/IRQs
- No page faults allowed in IRQ context

```c
//  BAD - sleeps in IRQ handler
static irqreturn_t bad_handler(int irq, void *dev)
{
    mutex_lock(&lock);  // CRASH! Can't sleep in IRQ
    return IRQ_HANDLED;
}

//  GOOD - just stores event
static irqreturn_t good_handler(int irq, void *dev)
{
    event_flag = 1;  // Just set flag
    wake_up_interruptible(&wq);  // Wake sleeper
    return IRQ_HANDLED;
}
```

---

### Q9: What's the difference between IRQ_HANDLED and IRQ_NONE?

**A:**

| Return | Meaning | When to Use |
|--------|---------|-------------|
| `IRQ_HANDLED` | IRQ was processed | If your device triggered this IRQ |
| `IRQ_NONE` | IRQ not handled | If this isn't your device's IRQ |

**Example (shared IRQ):**
```c
static irqreturn_t gpio_irq_handler(int irq, void *dev)
{
    // Check if this is really our device's interrupt
    if (!device_interrupt_pending())
        return IRQ_NONE;  // Not ours, check next handler
    
    // Process interrupt
    event_flag = 1;
    wake_up_interruptible(&gpio_wq);
    
    return IRQ_HANDLED;  // Was ours
}
```

---

### Q10: How does request_irq work?

**A:** Registers an interrupt handler with the kernel:

```c
int ret = request_irq(
    irq_number,              // Which IRQ line (e.g., GPIO_IRQ = 17)
    gpio_irq_handler,        // Handler function pointer
    IRQF_TRIGGER_RISING,     // When to trigger (rising edge)
    "gpio_driver",           // Name (for debugging)
    (void *)gpio_device      // Device identifier
);

if (ret < 0) {
    printk("Failed to request IRQ %d\n", irq_number);
    return ret;
}

// Later, free the IRQ
free_irq(irq_number, (void *)gpio_device);
```

**Parameters:**
- `irq_number`: From platform data or device tree
- `handler`: Function to call when interrupt fires
- `flags`: IRQF_TRIGGER_RISING, IRQF_SHARED, etc.
- `name`: Visible in `/proc/interrupts`
- `dev_id`: For shared IRQs to identify device

---

## poll() & Event Notification

### Q11: How does poll() know when data is available?

**A:** Via wait queues and wake_up():

```
1. poll() called
   ↓
2. Driver's poll() handler called
   ↓
3. poll_wait() adds process to wait queue
   ↓
4. poll() returns (no data yet)
   ↓
5. Process sleeps
   ↓
   [Interrupt arrives]
   ↓
6. IRQ handler calls wake_up_interruptible()
   ↓
7. Process wakes up
   ↓
8. poll() is called again automatically
   ↓
9. Driver's poll() handler checks event_flag
   ↓
10. poll() returns with POLLIN set
   ↓
11. User app gets event
```

---

### Q12: What's the difference between poll(), select(), and epoll()?

**A:**

| Function | Max FDs | Efficiency | Use Case |
|----------|---------|-----------|----------|
| `poll()` | ~1000 | O(n) | Moderate # of FDs |
| `select()` | ~64 | O(n) | Few FDs, portable |
| `epoll()` | ~100k | O(1) | Many FDs, high-performance |

**Example:**
```c
// poll() - moderate use
struct pollfd fds[10];
poll(fds, 10, -1);  // Check 10 FDs

// epoll() - high-performance
int ep = epoll_create(100);
epoll_wait(ep, events, 100, -1);  // Efficient for many FDs
```

---

### Q13: What happens if poll() returns but event_flag is 0?

**A:** Spurious wakeup - should check event_flag:

```c
//  BAD - assumes poll() means event ready
int ret = poll(&pfd, 1, -1);
if (ret > 0) {
    ioctl(fd, GPIO_GET_EVENT, &event);  // But event_flag might be 0!
}

//  GOOD - check event_flag after poll
int ret = poll(&pfd, 1, -1);
if (ret > 0 && (pfd.revents & POLLIN)) {
    int event;
    ioctl(fd, GPIO_GET_EVENT, &event);
    if (event) {
        // Real event
    }
}
```

**Why check revents?**
- poll() might return due to error (POLLERR)
- poll() might return for other reasons
- Should verify POLLIN is actually set

---

### Q14: Why is poll() more efficient than busy-waiting?

**A:** CPU usage comparison:

```c
//  Busy loop - 100% CPU
while (1) {
    ioctl(fd, GPIO_GET_EVENT, &event);
    if (event) break;
    // ← CPU spinning here, wasting power
}

//  poll() - 0% CPU until event
struct pollfd pfd = {fd, POLLIN};
poll(&pfd, 1, -1);  // Process sleeps here
if (pfd.revents & POLLIN) {
    ioctl(fd, GPIO_GET_EVENT, &event);
}
```

**Benefits of poll():**
- Process is scheduled out (not using CPU)
- Kernel wakes only when event occurs
- No power wasted on checking
- Scales to thousands of FDs

---

## Synchronization

### Q15: Why is the mutex lock needed around event_flag?

**A:** To prevent race conditions:

```c
//  Without mutex - race condition
static int event_flag = 0;

// IRQ handler                User app
event_flag = 1;              read event_flag;
                             // Might read old value!

//  With mutex - atomic
static DEFINE_MUTEX(gpio_lock);

// IRQ handler
mutex_lock(&gpio_lock);
event_flag = 1;
mutex_unlock(&gpio_lock);

// User app
mutex_lock(&gpio_lock);
value = event_flag;
mutex_unlock(&gpio_lock);
```

**Race condition scenario:**
```
Time | CPU 1 (IRQ) | CPU 2 (User)

1    | event = 1   |
2    |             | read event → 1 
3    |             | event = 0
4    |             | read event → 0 
5    | read event  | (too late!)
```

---

### Q16: Why use mutex in driver and spinlock in IRQ handler?

**A:**

```c
// Process context - use mutex (can sleep)
static DEFINE_MUTEX(gpio_lock);
mutex_lock(&gpio_lock);
{
    // Safe to use: ioctl, allocate memory, etc.
    gpio_pin = new_pin;
}
mutex_unlock(&gpio_lock);

// IRQ context - use spinlock (cannot sleep)
static DEFINE_SPINLOCK(gpio_lock_irq);
spin_lock(&gpio_lock_irq);
{
    // Cannot sleep, no ioctl, no memory allocation
    event_flag = 1;
}
spin_unlock(&gpio_lock_irq);
```

**Why different?**
- **Mutex**: Process waits for lock (sleeps, inefficient in IRQ)
- **Spinlock**: Busy waits (okay for short sections in IRQ)
- **IRQ context**: Cannot sleep (IRQ is not preemptible)

---

## Architecture & Design

### Q17: Walk me through the full architecture of this driver.

**A:**

```
                    User Space
                    
                  gpio_user_app.c
                        
         
                                     
                                     
      open()         poll()         ioctl()
                                     
         
                        
                        
                 System Call Interface
                        
                        
                 Kernel Space (VFS)
                        
         
                                     
                                     
    File Ops:      Poll Handler    ioctl Handler
    open/close     wait_queue      switch(cmd)
                                     
         
                        
                        
              GPIO IRQ Driver State
                   event_flag
                   gpio_pin
                   irq_count
                   waitqueue
                   mutex_lock
                        
                        
                  IRQ Handler
                   Hardware interrupt
                   Set event_flag
                   wake_up()
                   IRQ_HANDLED
                        
                        
                  Hardware GPIO
```

---

### Q18: How would you extend this for multiple GPIO pins?

**A:** Use per-device structures:

```c
struct gpio_device {
    int pin;
    int gpio_pin_number;
    int event_flag;
    int irq_count;
    struct mutex lock;
    wait_queue_head_t wq;
};

// Instead of static globals
static int gpio_pin = 17;  //  Limited to one device

// Use structure array
#define MAX_GPIO_DEVICES 4
static struct gpio_device devices[MAX_GPIO_DEVICES];

// Manage multiple devices
static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int device_id = iminor(file->f_inode);  // Get minor number
    struct gpio_device *dev = &devices[device_id];
    
    mutex_lock(&dev->lock);
    {
        // Handle ioctl for this specific device
        switch (cmd) {
        case GPIO_SET_PIN:
            dev->gpio_pin_number = pin;
            break;
        }
    }
    mutex_unlock(&dev->lock);
}
```

---

### Q19: What error conditions should be handled?

**A:**

```c
// 1. Device already in use
case GPIO_ENABLE_IRQ:
    if (dev->irq_enabled) {
        return -EBUSY;  // Already enabled
    }
    break;

// 2. Invalid parameters
case GPIO_SET_EDGE:
    if (edge < 0 || edge > 2) {
        return -EINVAL;  // Invalid edge type
    }
    break;

// 3. Permission denied
case GPIO_WRITE:
    if (!dev->irq_enabled || dev->direction != OUTPUT) {
        return -EPERM;  // Not permitted
    }
    break;

// 4. Resource temporarily unavailable
if (!resources_available) {
    return -EAGAIN;
}

// 5. Data too large
case GPIO_SET_CONFIG:
    if (config_size > MAX_CONFIG) {
        return -E2BIG;
    }
    break;
```

---

## Tricky Questions

### Q20: What happens if user-space doesn't call poll(), just repeatedly calls ioctl()?

**A:** It's inefficient but works (busy-waiting pattern):

```c
// Inefficient polling
while (1) {
    int event;
    ioctl(fd, GPIO_GET_EVENT, &event);
    if (event) {
        process_event();
    }
    // ← CPU at 100%, wasting power
}
```

**Better approach:**
```c
// Use poll() for efficiency
struct pollfd pfd = {fd, POLLIN};
while (1) {
    if (poll(&pfd, 1, -1) > 0 && (pfd.revents & POLLIN)) {
        int event;
        ioctl(fd, GPIO_GET_EVENT, &event);
        process_event();
    }
}
```

---

### Q21: What if IRQ handler is called while user app is in ioctl()?

**A:** Synchronization primitives handle this:

```c
// User app in ioctl()
mutex_lock(&gpio_lock);          // Acquires lock
{
    event_flag = 0;  // Clear event
}
mutex_unlock(&gpio_lock);

// Meanwhile, IRQ arrives
// IRQ handler tries mutex_lock(&gpio_lock)
// → Blocks until user ioctl releases lock
// → Safe! No race condition
```

**Timeline:**
```
User App         IRQ Handler
     
ioctl called
  mutex_lock    
  event_flag=0   
             IRQ!
                 gpio_irq_handler()
                   mutex_lock → blocks
  mutex_unlock
                   mutex_lock  (now has lock)
                   event_flag = 1
                   mutex_unlock
  poll() returns
```

---

### Q22: What if copy_from_user() fails?

**A:** Return -EFAULT error:

```c
static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int value;
    
    switch (cmd) {
    case GPIO_SET_PIN:
        // Copy from user space
        if (copy_from_user(&value, (int __user *)arg, sizeof(value))) {
            return -EFAULT;  // User pointer was invalid
        }
        gpio_pin = value;
        break;
    }
    
    return 0;
}
```

**User space:**
```c
int *bad_ptr = NULL;
ioctl(fd, GPIO_SET_PIN, bad_ptr);  // Returns -EFAULT
perror("ioctl");  // "Bad address"
```

---

### Q23: Can the driver be accessed from multiple processes simultaneously?

**A:** Yes, but must handle concurrency:

```c
// Process 1              Process 2
open("/dev/gpioirqdev");  open("/dev/gpioirqdev");
poll(fd1, ...);           poll(fd2, ...);
                          ioctl(fd2, GPIO_SET_PIN, ...);
ioctl(fd1, ...) 
```

**Handled by:**
- Each process gets own `struct file`
- Shared state protected by mutex
- poll() on fd1 independent of fd2

```c
// Both share this protected state
static int gpio_pin = 17;        // Both see same value
static DEFINE_MUTEX(gpio_lock);  // Protects access
```

---

### Q24: What about memory leaks in the driver?

**A:** Kernel memory must be freed:

```c
// Allocate
void *buffer = kmalloc(size, GFP_KERNEL);
if (!buffer)
    return -ENOMEM;

// Use
memcpy(buffer, data, size);

// Must free!
kfree(buffer);  // Or causes memory leak

//  Better pattern
void *buffer = kmalloc(size, GFP_KERNEL);
if (!buffer)
    return -ENOMEM;

if (error) {
    kfree(buffer);  // Free on error
    return -ERROR;
}

kfree(buffer);  // Free on success
return 0;
```

---

### Q25: How to debug if interrupts aren't working?

**A:**

```bash
# 1. Check if module is loaded
lsmod | grep gpio_irq_driver

# 2. Check kernel logs
dmesg | grep -i gpio

# 3. Check IRQ statistics
cat /proc/interrupts | grep gpio

# 4. Check if device node exists
ls -la /dev/gpioirqdev

# 5. Run with strace
strace -e trace=ioctl,poll ./gpio_user_app

# 6. Check IRQ handler was called
dmesg | grep "GPIO IRQ occurred"

# 7. Simulate IRQ (if hardware doesn't trigger)
echo "1" > /sys/kernel/debug/gpio/pin_17/trigger
```

---

**Last Updated:** May 27, 2026  
**Version:** 1.0  
**Difficulty:** Interview-Level 
