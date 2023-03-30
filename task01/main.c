#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int init_module(void)
{
	printk(KERN_INFO "Hello world !\n");
	return (0);
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psaulnie");
MODULE_DESCRIPTION("Task01 of little-penguin-1");
MODULE_VERSION("1.0");
