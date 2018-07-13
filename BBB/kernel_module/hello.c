#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	printk(KERN_ALERT "Hello BBB\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Goodbye BBB\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

