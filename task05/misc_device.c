#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>

static ssize_t read_device(struct file *file, char __user *user_buffer,
                		size_t size, loff_t *offset)
{
	uint8_t	*data = "psaulnie\n";
	size_t	len = strlen(data);

	if (!user_buffer)
		return (-EINVAL);
	if (*offset >= 8)
		return (0);
	if (size > len)
		size = len;
	if (copy_to_user(user_buffer, data, size))
		return (-EFAULT);
	return (size);
}

static ssize_t write_device(struct file *filp, const char *buffer,
						size_t length, loff_t *offset)
{
	char	user_data[9];

	if (length != 9)
		return (-EINVAL);
	if (copy_from_user(user_data, buffer, length))
		return (-EFAULT);
	else if (memcmp(user_data, "psaulnie", 8))
		return (-EINVAL);
	return (length);
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
	int status;
	
	status = misc_register(&fortytwo_device);
	if (status)
		printk(KERN_ALERT "Error at retrieving the major number.\n");
	else
		printk(KERN_INFO "Character device registered.\n");
	return (status);
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