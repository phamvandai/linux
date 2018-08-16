/**
 * Reader - writer
 * 1 writer - many reader. 
 * When writing is in process, readers blocked.
 */

/**
 * Synchronization without locking
 * <asm/atomic.h>
 * atomic_set, atomic_read, atomic_add, atomic_sub, atomic_inc, atomic_dec
 * 
 * Atomic bit operations: <asm/bitops.h>
 * set_bit, clear_bit, change_bit
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/rwsem.h>

#define FIRST_MINOR 0
#define MINOR_CNT   1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static struct task_struct *task;
static struct rw_semaphore rw_sem;

static char data[1024];
static int opened_cnt = 0;

int open(struct inode *inode, struct file *filp)
{
    printk("Open device\n");
    opened_cnt++;
    task = current;
    return 0;
}

int release(struct inode *inode, struct file *filp)
{
    printk("Close device\n");
    opened_cnt--;
    return 0;
}

ssize_t read(struct file *filp, char *buff, size_t count, loff_t *offp)
{
    printk("Waiting for read semaphore\n");
    down_read(&rw_sem);
    printk("Data %s, reading %d bytes\n", data, count);
    
    size_t readCnt = count > 1024 ? 1024 : count;

    long copied = copy_to_user(buff, &data[0], readCnt);
    printk("Copied to user %d bytes\n", copied);
    ssleep(10);
    up_read(&rw_sem);

    return readCnt;
}

ssize_t write(struct file *filp, const char *buff, size_t count, loff_t *offp)
{
    printk("Waiting for write semaphore...\n");
    down_write(&rw_sem);
    printk("Got semaphore for write %d bytes\n", count);

    size_t writeCnt = count > 1024 ? 1024 : count;
    long copied = copy_from_user(&data[0], buff, writeCnt);
    printk("Copied from user %d bytes\n", copied);
    up_write(&rw_sem);
    return count;
}

struct file_operations fops = {
    read: read,
    write: write,
    open: open,
    release : release
};

static int rw_sem_init(void)
{
    int ret;
    struct device *dev_ret;

    if((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "rws")) < 0) {
        return ret;
    }

    printk("Major Nr: %d\n", MAJOR(dev));

    cdev_init(&c_dev, &fops);

    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) {
        unregister_chrdev_region(dev, MINOR_CNT);
        return ret;
    }

    if(IS_ERR(cl = class_create(THIS_MODULE, "chardrv"))) {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }

    if(IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "mychar%d", 0))) {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    init_rwsem(&rw_sem);
    return 0;
}

static void rw_sem_cleanup(void)
{
    printk("Cleanup module\n");
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

MODULE_LICENSE("GPL");
module_init(rw_sem_init);
module_exit(rw_sem_cleanup);