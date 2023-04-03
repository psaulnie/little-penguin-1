#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

// #define DEVICE_NAME "fortytwo"
#define BUFFER_SIZE 1024

static int	major_number;
// static char	device_buffer[BUFFER_SIZE];

static ssize_t read_device(struct file *file, char __user *user_buffer,
                		size_t size, loff_t *offset)
{
	uint8_t	*data = "psaulnie\n";
	size_t	len = strlen(data);
	printk(KERN_INFO "yo\n");
	if (size > len)
		size = len;
	if (copy_to_user(user_buffer, data, size))
		return (-1);
	return (size);
}

static ssize_t write_device(struct file *filp, const char *buffer,
						size_t length, loff_t *offset)
{
		printk(KERN_INFO "o\n");

	return (0);
}

static struct file_operations file_ops = {
	.read = read_device,
	.write = write_device,
};

static struct miscdevice fortytwo_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fortytwo",
    .fops = &file_ops
};

static int __init init_ft(void)
{
	printk(KERN_INFO "Init module.\n");
	major_number = misc_register(&fortytwo_device);
	if (major_number < 0) {
		printk(KERN_ALERT "Error at retrieving the major number.\n");
		return (major_number);
	}
	printk(KERN_INFO "Character device registered.\n");
	return (0);
}

static void __exit cleanup_ft(void)
{
	misc_deregister(&fortytwo_device);
	printk(KERN_INFO "Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psaulnie");
MODULE_DESCRIPTION("Task05 of little-penguin-1");
MODULE_VERSION("1.0");

module_init(init_ft);
module_exit(cleanup_ft);