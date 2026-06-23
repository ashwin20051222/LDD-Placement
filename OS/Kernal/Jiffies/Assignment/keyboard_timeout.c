/*
 * keyboard_timeout.c
 * 
 * Kernel module that demonstrates keyboard interrupt timeout using jiffies.
 * Features:
 * - Hooks keyboard IRQ (IRQ 1)
 * - Tracks keyboard activity using jiffies
 * - Detects inactivity timeout (5 seconds)
 * - Exposes character device for user-space communication
 * 
 * Author: Linux Device Driver Student
 * License: GPL
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "kb_timeout"
#define KEYBOARD_IRQ 1
#define TIMEOUT_SEC 5

static int major;
static struct timer_list my_timer;
static unsigned long last_activity;
static int timeout_occured = 0;

/**
 * keyboard_irq_handler() - Interrupt handler for keyboard IRQ
 * @irq: IRQ number
 * @dev_id: Device identifier
 * 
 * Called every time a key is pressed. Updates last_activity time.
 */
static irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    last_activity = jiffies;

    printk(KERN_INFO "Keyboard Activity Detected at jiffies = %lu\n", 
           last_activity);

    return IRQ_HANDLED;
}

/**
 * timer_callback() - Periodic timer callback to check for timeout
 * @t: Timer list structure
 * 
 * Checks if keyboard has been inactive for TIMEOUT_SEC seconds.
 * If timeout detected, sets timeout_occured flag.
 * Reschedules itself to run every 1 second (HZ jiffies).
 */
static void timer_callback(struct timer_list *t)
{
    unsigned long elapsed;

    elapsed = jiffies - last_activity;

    /* Compare with timeout (converted to jiffies) */
    if (elapsed > secs_to_jiffies(TIMEOUT_SEC))
    {
        printk(KERN_INFO "Keyboard Timeout Occurred (elapsed: %lu jiffies)\n", 
               elapsed);

        timeout_occured = 1;

        /* Reset activity time */
        last_activity = jiffies;
    }

    /* Restart timer to run after 1 second */
    mod_timer(&my_timer, jiffies + HZ);
}

/**
 * driver_read() - Read function for character device
 * @file: File structure
 * @buf: User-space buffer to copy data to
 * @len: Length of data to read
 * @off: File offset
 * 
 * Reads timeout status from driver. Returns "Keyboard timeout detected"
 * if timeout occurred, otherwise "No timeout".
 */
static ssize_t driver_read(struct file *file,
                           char __user *buf,
                           size_t len,
                           loff_t *off)
{
    char message[100];
    int bytes;

    /* Prevent multiple reads of same data */
    if (*off > 0)
        return 0;

    if (timeout_occured)
    {
        bytes = sprintf(message, "Keyboard timeout detected\n");
        timeout_occured = 0;  /* Reset flag after reading */
    }
    else
    {
        bytes = sprintf(message, "No timeout\n");
    }

    copy_to_user(buf, message, bytes);

    *off = bytes;

    return bytes;
}

/* File operations structure */
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read  = driver_read,
};

/**
 * keyboard_timeout_init() - Module initialization
 * 
 * - Registers character device
 * - Registers keyboard interrupt handler (IRQ 1)
 * - Sets up timer for periodic timeout checking
 */
static int __init keyboard_timeout_init(void)
{
    int result;

    printk(KERN_INFO "========================================\n");
    printk(KERN_INFO "Keyboard Timeout Driver Initialization\n");
    printk(KERN_INFO "========================================\n");

    last_activity = jiffies;

    /* Register character device */
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0)
    {
        printk(KERN_ERR "Failed to register character device\n");
        return major;
    }

    printk(KERN_INFO "Character Device Registered\n");
    printk(KERN_INFO "Major Number = %d\n", major);
    printk(KERN_INFO "Create device with: sudo mknod /dev/%s c %d 0\n", 
           DEVICE_NAME, major);

    /* Register keyboard IRQ */
    result = request_irq(KEYBOARD_IRQ,
                         keyboard_irq_handler,
                         IRQF_SHARED,
                         "keyboard_timeout",
                         (void *)&keyboard_irq_handler);

    if (result)
    {
        printk(KERN_ERR "Failed to register keyboard IRQ\n");
        unregister_chrdev(major, DEVICE_NAME);
        return result;
    }

    printk(KERN_INFO "Keyboard IRQ (IRQ 1) Registered\n");

    /* Initialize and setup timer */
    timer_setup(&my_timer, timer_callback, 0);
    mod_timer(&my_timer, jiffies + HZ);

    printk(KERN_INFO "Timer Started (checking every 1 second)\n");
    printk(KERN_INFO "Timeout Duration: %d seconds\n", TIMEOUT_SEC);
    printk(KERN_INFO "========================================\n");

    return 0;
}

/**
 * keyboard_timeout_exit() - Module cleanup
 * 
 * - Stops and removes timer
 * - Frees keyboard IRQ
 * - Unregisters character device
 */
static void __exit keyboard_timeout_exit(void)
{
    printk(KERN_INFO "========================================\n");
    printk(KERN_INFO "Keyboard Timeout Driver Cleanup\n");
    printk(KERN_INFO "========================================\n");

    del_timer(&my_timer);
    printk(KERN_INFO "Timer Stopped\n");

    free_irq(KEYBOARD_IRQ, (void *)&keyboard_irq_handler);
    printk(KERN_INFO "Keyboard IRQ Released\n");

    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Character Device Unregistered\n");

    printk(KERN_INFO "Driver Removed Successfully\n");
    printk(KERN_INFO "========================================\n");
}

module_init(keyboard_timeout_init);
module_exit(keyboard_timeout_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Linux Device Driver Student");
MODULE_DESCRIPTION("Keyboard Interrupt Timeout Using Jiffies");
MODULE_VERSION("1.0");
