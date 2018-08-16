
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/delay.h>

/**
 * APIs:
 * set_current_state(TASK_INTERRUPTIBLE)
 * sched()
 * wake_up_process(task_struct *ts)
 */

#define FIRST_MINOR     0
#define MINOR_CNT       1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static struct task_struct *sleeping_task;
static char flag;

static int open(struct inode *inode, struct file *filp)
{
    printk("Open device\n");
    return 0;
}

static int release(struct inode *inode, struct file *filp)
{
    printk("Close device\n");
    return 0;
}

ssize_t read(struct file *filp, char *buff, size_t count, loff_t *offp)
{
    printk("Read\n");
    printk("Scheduling out\n");

    // Save current sleeping task 
    sleeping_task = current; 

    _sleep: 
    // Block if flag is not set to 'w'
    if (flag != 'w') {
        // Set current process block interruptible
        set_current_state(TASK_INTERRUPTIBLE);

        // Yield 
        schedule();
    }

    if (flag == 'w') {
        printk("Woken up\n");
    }
    else {
        printk("Interrupted by signal\n");
        goto _sleep;
    }

    // Reset flag
    flag = 'x';

    printk("Woken up\n");
    return 0;
}

ssize_t write(struct file *filp, const char *buff, size_t count, loff_t *offp)
{
    printk("Write\n");
    __get_user(flag, buff);
    printk("Write flag %c\n", flag);
    wake_up_process(sleeping_task);
    return count;
}

struct file_operations fops = {
    .read = read,
    .write = write,
    .open = open,
    .release = release
};

static int __init sched_init(void)
{
    int ret;
    struct device *dev_ret;
    if((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "wqd")) < 0) {
        return ret;
    }

    printk("Major number: %d\n", MAJOR(dev));

    cdev_init(&c_dev, &fops);

    if((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) {
        unregister_chrdev_region(dev, MINOR_CNT);
        return ret;
    }

    if(IS_ERR(cl = class_create(THIS_MODULE, "chardrv"))) {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }

    if(IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "mychardev%d", 0))) {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit sched_cleanup(void)
{
    printk("Cleanup module\n");
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(sched_init);
module_exit(sched_cleanup);

MODULE_LICENSE("GPL");