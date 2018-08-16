#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>


#define FIRST_MINOR     0
#define MINOR_CNT       1


static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int my_open(struct inode *i, struct file *f)
{
    printk("Open device\n");
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    printk("Close device\n");
    return 0;
}

static char c = 'A';
static struct semaphore my_sem;

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    if(down_interruptible(&my_sem)) {
        printk("Unable to accquire semaphore\n");
        return -1;
    }
    printk("Read unblocked by write\n");
    return 0;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    up(&my_sem);
    if(copy_from_user(&c, buf + len - 1, 1)) {
        return -EFAULT;
    }
    printk("Trigger semaphore...\n");
    return len;
}

static struct file_operations driver_fops = {
    .owner = THIS_MODULE,
    .open  = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write
};

static int __init concurency_init(void)
{
    int ret;
    struct device *dev_ret;

    // Alloc region
    if((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "my_sem")) < 0) {
        return ret;
    }

    // Init cdev
    cdev_init(&c_dev, &driver_fops);

    // Add cdev
    if((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) {
        unregister_chrdev_region(dev, MINOR_CNT);
        return ret;
    }

    // Create class 
    if (IS_ERR(cl = class_create(THIS_MODULE, "char"))) {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }

    // Create device 
    if(IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "mysem%d", FIRST_MINOR))) {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    sema_init(&my_sem, 0);
    return 0;
}

static void __exit concurency_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);

}

MODULE_LICENSE("GPL");
module_init(concurency_init);
module_exit(concurency_exit);

