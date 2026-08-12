/* chardev_ioctl.c - simple char driver with ioctl
 * Build with the provided Makefile (in repo root)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev_ioctl"
#define IOCTL_MAGIC 'k'
#define IOCTL_SET_VALUE _IOW(IOCTL_MAGIC, 1, int)
#define IOCTL_GET_VALUE _IOR(IOCTL_MAGIC, 2, int)
#define IOCTL_RESET     _IO(IOCTL_MAGIC, 3)
#define IOCTL_GET_STATUS _IOR(IOCTL_MAGIC, 4, int)

static int major;
static int dev_value = 0;
static int dev_status = 0;

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "chardev: open\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "chardev: release\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    char kbuf[32];
    int l = snprintf(kbuf, sizeof(kbuf), "%d\n", dev_value);

    if (*off >= l)
        return 0;

    if (len > (size_t)(l - *off))
        len = l - *off;

    if (copy_to_user(buf, kbuf + *off, len))
        return -EFAULT;

    *off += len;
    return len;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    char kbuf[32];

    if (len >= sizeof(kbuf))
        return -EFAULT;

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0';
    sscanf(kbuf, "%d", &dev_value);
    dev_status = 1;

    return len;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int tmp;

    switch (cmd) {
    case IOCTL_SET_VALUE:
        if (copy_from_user(&tmp, (int __user *)arg, sizeof(int)))
            return -EFAULT;
        dev_value = tmp;
        dev_status = 1;
        break;

    case IOCTL_GET_VALUE:
        if (copy_to_user((int __user *)arg, &dev_value, sizeof(int)))
            return -EFAULT;
        break;

    case IOCTL_RESET:
        dev_value = 0;
        dev_status = 0;
        break;

    case IOCTL_GET_STATUS:
        if (copy_to_user((int __user *)arg, &dev_status, sizeof(int)))
            return -EFAULT;
        break;

    default:
        return -ENOTTY;
    }

    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Register failed\n");
        return major;
    }

    printk(KERN_INFO "chardev_ioctl loaded, major=%d\n", major);
    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "chardev_ioctl unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE("GPL");
