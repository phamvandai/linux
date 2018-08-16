/**
 * Previous example is simple is error prone 
 * This example uses wait queue (list of processes waiting for an even)
 * <linux/wait.h>
 * wait_queue_head_t
 * 
 * DECLARE_WAIT_QUEUE_HEAD(wait_queue_name)
 * or wait_queue_head_t my_queue
 * init_waitqueue_head(&my_queue)
 * 
 * APIs:
 * wait_event
 * wait_event_interruptible
 * wait_event_timeout
 * wait_event_interruptible_timeout
 *
 * wake_up(wake_queue_head_t *)
 * wake_up_interruptible(wait_queue_head_t *)
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define FIRST_MINOR 0
#define MINOR_CNT   1

static char flag = 'n';
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static DECLARE_WAIT_QUEUE_HEAD(wq);

int open(struct inode *inode, struct file *filp)
{
    printk("Open device\n");
    return 0;
}

int release(struct inode *inode, struct file *filp)
{
    printk("Close device\n");
    return 0;
}

ssize_t read(struct file *filp, char *buff, size_t count, loff_t *offp)
{
    printk("Read...\n");
    printk("Scheduling out\n");
    wait_event_interruptible(wq, flag == 'y');
    flag = 'n';
    printk("Woken up\n");
    return 0;
}

ssize_t write(struct file *filp, const char *buff, size_t count, loff_t *offp)
{
    printk("Write...\n");
    if(copy_from_user(&flag, buff, 1)) {
        return -EFAULT;
    }
    printk("%c", flag);
    wake_up_interruptible(&wq);

    return count;
}

struct file_operations fops = {
    read: read,
    write: write,
    open: open,
    release: release
};

static int __init wq_init(void)
{
	int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "SCD")) < 0)
	{
		return ret;
	}
	printk("Major Nr: %d\n", MAJOR(dev));

	cdev_init(&c_dev, &fops);

	if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
	{
		unregister_chrdev_region(dev, MINOR_CNT);
		return ret;
	}

	if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv")))
	{
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, MINOR_CNT);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "mychar%d", 0)))
	{
		class_destroy(cl);
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, MINOR_CNT);
		return PTR_ERR(dev_ret);
	}
	return 0;    
}

static void __exit wq_cleanup(void)
{
    printk("Cleanup module\n");
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(wq_init);
module_exit(wq_cleanup);

MODULE_LICENSE("GPL");



