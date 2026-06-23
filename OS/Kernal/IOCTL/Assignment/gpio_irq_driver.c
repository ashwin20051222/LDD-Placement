#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/mutex.h>

#define DEVICE_NAME "gpioirqdev"
#define GPIO_MAGIC 'G'

#define GPIO_SET_PIN     _IOW(GPIO_MAGIC, 1, int)
#define GPIO_SET_EDGE    _IOW(GPIO_MAGIC, 2, int)
#define GPIO_ENABLE_IRQ  _IO(GPIO_MAGIC, 3)
#define GPIO_DISABLE_IRQ _IO(GPIO_MAGIC, 4)
#define GPIO_GET_EVENT   _IOR(GPIO_MAGIC, 5, int)

static int major;

/* Simulated GPIO IRQ state */
static int gpio_pin = 17;
static int irq_enabled = 0;
static int event_flag = 0;
static int irq_count = 0;
static int edge_type = 0; /* 0=rising, 1=falling, 2=both */

/* Mutex for thread-safety */
static DEFINE_MUTEX(gpio_lock);

/* wait queue for poll/select */
static DECLARE_WAIT_QUEUE_HEAD(gpio_wq);

/* ============ IRQ Handler (SIMULATED) ============ */
static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    if (!irq_enabled)
        return IRQ_NONE;

    mutex_lock(&gpio_lock);
    {
        irq_count++;
        event_flag = 1;
        printk(KERN_INFO "GPIO IRQ occurred! count=%d\n", irq_count);
        wake_up_interruptible(&gpio_wq);
    }
    mutex_unlock(&gpio_lock);

    return IRQ_HANDLED;
}

/* ============ POLL Function ============ */
static unsigned int gpio_poll(struct file *file, poll_table *wait)
{
    unsigned int mask = 0;

    poll_wait(file, &gpio_wq, wait);

    mutex_lock(&gpio_lock);
    if (event_flag)
        mask |= POLLIN | POLLRDNORM;
    mutex_unlock(&gpio_lock);

    return mask;
}

/* ============ IOCTL Handler ============ */
static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int val;
    int ret = 0;

    switch (cmd) {

    case GPIO_SET_PIN:
        if (copy_from_user(&val, (int __user *)arg, sizeof(val)))
            return -EFAULT;

        mutex_lock(&gpio_lock);
        gpio_pin = val;
        printk(KERN_INFO "GPIO: pin set to %d\n", gpio_pin);
        mutex_unlock(&gpio_lock);
        break;

    case GPIO_SET_EDGE:
        if (copy_from_user(&val, (int __user *)arg, sizeof(val)))
            return -EFAULT;

        if (val < 0 || val > 2)
            return -EINVAL;

        mutex_lock(&gpio_lock);
        edge_type = val;
        printk(KERN_INFO "GPIO: edge type set to %d (0=rising, 1=falling, 2=both)\n", edge_type);
        mutex_unlock(&gpio_lock);
        break;

    case GPIO_ENABLE_IRQ:
        mutex_lock(&gpio_lock);
        if (irq_enabled) {
            printk(KERN_WARNING "GPIO IRQ already enabled\n");
            ret = -EBUSY;
        } else {
            irq_enabled = 1;
            event_flag = 0;
            printk(KERN_INFO "GPIO IRQ enabled on pin %d\n", gpio_pin);
        }
        mutex_unlock(&gpio_lock);
        break;

    case GPIO_DISABLE_IRQ:
        mutex_lock(&gpio_lock);
        irq_enabled = 0;
        printk(KERN_INFO "GPIO IRQ disabled\n");
        mutex_unlock(&gpio_lock);
        break;

    case GPIO_GET_EVENT:
        mutex_lock(&gpio_lock);
        {
            val = event_flag;
            if (copy_to_user((int __user *)arg, &val, sizeof(val))) {
                ret = -EFAULT;
            } else {
                if (event_flag)
                    printk(KERN_INFO "GPIO event read (count=%d)\n", irq_count);
                event_flag = 0;
            }
        }
        mutex_unlock(&gpio_lock);
        break;

    default:
        return -EINVAL;
    }

    return ret;
}

/* ============ File Operations ============ */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = gpio_ioctl,
    .poll = gpio_poll,
};

/* ============ Module Init ============ */
static int __init gpio_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ERR "GPIO IRQ driver: failed to register\n");
        return major;
    }

    printk(KERN_INFO "GPIO IRQ ioctl driver loaded.\n");
    printk(KERN_INFO "Device Name: /dev/%s\n", DEVICE_NAME);
    printk(KERN_INFO "Major Number: %d\n", major);
    printk(KERN_INFO "Create device node: sudo mknod /dev/%s c %d 0\n", DEVICE_NAME, major);

    return 0;
}

/* ============ Module Exit ============ */
static void __exit gpio_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "GPIO IRQ driver unloaded\n");
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Linux Developer");
MODULE_DESCRIPTION("GPIO IRQ Driver with ioctl and poll support");
