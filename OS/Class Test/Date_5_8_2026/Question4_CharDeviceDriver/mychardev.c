#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#define DEVICE_NAME "mychardev"
#define BUF_SIZE 1024

static char *kbuf;
static size_t data_size = 0;

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychardev: Device Opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychardev: Device Closed\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    ssize_t to_copy = data_size - *ppos;
    if (to_copy <= 0) return 0;
    if (to_copy > count) to_copy = count;
    if (copy_to_user(buf, kbuf + *ppos, to_copy))
        return -EFAULT;
    *ppos += to_copy;
    printk(KERN_INFO "mychardev: Data Read (%zu bytes)\n", to_copy);
    return to_copy;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count >= BUF_SIZE) return -EINVAL;
    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;
    data_size = count;
    kbuf[data_size] = '\0';
    printk(KERN_INFO "mychardev: Data Written (%zu bytes)\n", count);
    return count;
}

static const struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &my_fops,
};

static int __init my_init(void)
{
    kbuf = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kbuf) return -ENOMEM;
    data_size = 0;
    misc_register(&my_misc_device);
    printk(KERN_INFO "mychardev: Module loaded, device /dev/%s created\n", DEVICE_NAME);
    return 0;
}

static void __exit my_exit(void)
{
    misc_deregister(&my_misc_device);
    kfree(kbuf);
    printk(KERN_INFO "mychardev: Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Simple character device driver /dev/mychardev");
