#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define FOLDER_NAME "fortytwo"
#define FOO_NAME "foo"
#define ID_NAME "id"
#define JIFFIES_NAME "jiffies"

static struct dentry	*fortytwo_dir;
static struct dentry	*id_file;
static struct dentry	*foo_file;
char					foo_buffer[PAGE_SIZE];
size_t					foo_size;
DEFINE_MUTEX(mutex);

static ssize_t read_foo(struct file *file, char __user *user_buffer,
                		size_t size, loff_t *offset)
{
	mutex_lock(&mutex);
	if (foo_size == 0) {
		mutex_unlock(&mutex);
		return (-1);
	}
	if (!user_buffer) {
		mutex_unlock(&mutex);
		return (-EINVAL);
	}
	if (*offset >= size) {
		mutex_unlock(&mutex);
		return (0);
	}
	if (copy_to_user(user_buffer, foo_buffer, size)) {
		mutex_unlock(&mutex);
		return (-EFAULT);
	}
	mutex_unlock(&mutex);
	return (size);
}

static ssize_t write_foo(struct file *filp, const char *buffer,
						size_t length, loff_t *offset)
{
	mutex_lock(&mutex);
	if (length > PAGE_SIZE) {
		mutex_unlock(&mutex);
		return (-EOVERFLOW);
	}
	memset(foo_buffer, 0, PAGE_SIZE);
	if (copy_from_user(foo_buffer, buffer, length)) {
		mutex_unlock(&mutex);
		return (-EFAULT);
	}
	mutex_unlock(&mutex);
	return (length);
}

static ssize_t read_id(struct file *file, char __user *user_buffer,
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

static ssize_t write_id(struct file *filp, const char *buffer,
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

static struct file_operations foo_ops = {
	.read = read_foo,
	.write = write_foo,
};

static struct file_operations id_ops = {
	.read = read_id,
	.write = write_id,
};

int init_module(void)
{
	fortytwo_dir = debugfs_create_dir(FOLDER_NAME, NULL);
	if (!fortytwo_dir) {
		printk(KERN_INFO "Failed to create the debugfs directory\n");
		return (-ENOENT);
	}
	printk(KERN_INFO "fortytwo folder created!\n");
	if (BITS_PER_LONG == 64)
		debugfs_create_u64(JIFFIES_NAME, 444, fortytwo_dir, (u64*) &jiffies);
	else
		debugfs_create_u32(JIFFIES_NAME, 444, fortytwo_dir, (u32*) &jiffies);
	id_file = debugfs_create_file(ID_NAME, 666, fortytwo_dir, NULL, &id_ops);
	if (!id_file) {
		debugfs_remove_recursive(fortytwo_dir);
		return (-ENOENT);
	}
	foo_file = debugfs_create_file(FOO_NAME, 644, fortytwo_dir, NULL, &foo_ops);
	if (!foo_file) {
		debugfs_remove_recursive(fortytwo_dir);
		return (-ENOENT);
	}
	printk(KERN_INFO "id, jiffies and foo are created!\n");
	return (0);
}

void cleanup_module(void)
{
	debugfs_remove_recursive(fortytwo_dir);
	printk(KERN_INFO "Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psaulnie");
MODULE_DESCRIPTION("Task07 of little-penguin-1");
MODULE_VERSION("1.0");
