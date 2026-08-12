/* irq_circbuf.c - interrupt-driven char device with circular buffer (template)
 * Build with Makefile. Driver captures events in ISR into circular buffer.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/spinlock.h>

#define BUF_SIZE 256

static char circ[BUF_SIZE];
static int head = 0;
static int tail = 0;

static spinlock_t lock;
static wait_queue_head_t wq;
static int major;

static int dev_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    int copied = 0;

    if (wait_event_interruptible(wq, head != tail))
        return -ERESTARTSYS;

    spin_lock(&lock);

    while (len && head != tail) {
        if (copy_to_user(buf + copied, &circ[tail], 1)) {
            spin_unlock(&lock);
            return -EFAULT;
        }

        tail = (tail + 1) % BUF_SIZE;
        copied++;
        len--;
    }

    spin_unlock(&lock);
    return copied;
}

static unsigned int dev_poll(struct file *file, struct poll_table_struct *p)
{
    unsigned int mask = 0;

    poll_wait(file, &wq, p);

    spin_lock(&lock);
    if (head != tail)
        mask |= POLLIN | POLLRDNORM;
    spin_unlock(&lock);

    return mask;
}

/* Dummy ISR to push an event char to buffer - replace with real ISR */
static irqreturn_t dummy_isr(int irq, void *dev)
{
    spin_lock(&lock);
    circ[head] = 'E';
    head = (head + 1) % BUF_SIZE;
    spin_unlock(&lock);

    wake_up_interruptible(&wq);
    return IRQ_HANDLED;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .poll = dev_poll,
};

static int __init init_mod(void)
{
    spin_lock_init(&lock);
    init_waitqueue_head(&wq);

    major = register_chrdev(0, "irq_circbuf", &fops);
    printk(KERN_INFO "irq_circbuf loaded major=%d\n", major);

    /* In real hardware, request_irq here */

    return 0;
}

static void __exit exit_mod(void)
{
    unregister_chrdev(major, "irq_circbuf");
    printk(KERN_INFO "irq_circbuf unloaded\n");
}

module_init(init_mod);
module_exit(exit_mod);
MODULE_LICENSE("GPL");
