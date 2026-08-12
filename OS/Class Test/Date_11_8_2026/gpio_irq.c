/* gpio_irq.c - simple GPIO interrupt char driver (template)
 * Usage: build as module. Set gpio_num module param if needed.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

static int gpio_num = 17; /* default GPIO */
module_param(gpio_num, int, 0444);
MODULE_PARM_DESC(gpio_num, "GPIO number for button");

static int irq_num = 0;
static unsigned long irq_count = 0;
static int major;

static irqreturn_t button_isr(int irq, void *dev_id)
{
    irq_count++;
    return IRQ_HANDLED;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    /* Commands:
     * 0 - enable interrupt
     * 1 - disable interrupt
     * 2 - get count (copy to user)
     * 3 - reset count
     */
    int op = (int)cmd;

    if (op == 0) {
        enable_irq(irq_num);
    } else if (op == 1) {
        disable_irq(irq_num);
    } else if (op == 2) {
        if (copy_to_user((unsigned long __user *)arg, &irq_count, sizeof(irq_count)))
            return -EFAULT;
    } else if (op == 3) {
        irq_count = 0;
    } else {
        return -ENOTTY;
    }

    return 0;
}

static int dev_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = dev_ioctl,
    .open = dev_open,
    .release = dev_release,
};

static int __init ginit(void)
{
    if (gpio_request(gpio_num, "gpio_irq_pin")) {
        printk(KERN_ALERT "gpio request failed\n");
        return -1;
    }

    gpio_direction_input(gpio_num);

    irq_num = gpio_to_irq(gpio_num);

    if (request_irq(irq_num, button_isr,
                    IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
                    "gpio_button", NULL)) {
        gpio_free(gpio_num);
        printk(KERN_ALERT "irq request failed\n");
        return -1;
    }

    major = register_chrdev(0, "gpio_irq", &fops);
    printk(KERN_INFO "gpio_irq loaded major=%d irq=%d\n", major, irq_num);

    return 0;
}

static void __exit gexit(void)
{
    free_irq(irq_num, NULL);
    gpio_free(gpio_num);
    unregister_chrdev(major, "gpio_irq");
    printk(KERN_INFO "gpio_irq unloaded\n");
}

module_init(ginit);
module_exit(gexit);
MODULE_LICENSE("GPL");
