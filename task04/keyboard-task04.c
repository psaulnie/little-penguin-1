#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init keyboard(void)
{
	printk(KERN_INFO "keyboard plugged in\n");
	return (0);
}

static void __exit cleanup(void)
{
	return ;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psaulnie");
MODULE_DESCRIPTION("Task04 of little-penguin-1");
MODULE_VERSION("1.0");

module_init(keyboard);
module_exit(cleanup);
