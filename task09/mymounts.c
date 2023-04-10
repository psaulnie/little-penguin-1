#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/list.h>  
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <asm/current.h>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

static struct proc_dir_entry *ent;

// &current pointer represents the current process
static char	*get_data(void)
{
	char			*data;
	char			*buf = kmalloc(1000, GFP_KERNEL);
	char			*path;
	struct dentry	*curdentry;
	int				first = 1;
	int				pos = 0;

	list_for_each_entry(curdentry, &current->fs->root.mnt->mnt_root->d_subdirs, d_child) {
		if (curdentry->d_flags & DCACHE_MOUNTED) {
			path = dentry_path_raw(curdentry, buf, 1000);
			if (!path) {
				if (!first)
					kfree(data);
				return (NULL);
			}
			if (first) {
				data = kmalloc(strlen(curdentry->d_name.name) * sizeof(char)
							+ strlen(path) * sizeof(char) + 9, GFP_KERNEL);
				first = 0;
			}
			else
				data = krealloc(data, strlen(curdentry->d_name.name)
							* sizeof(char) + strlen(path)
							* sizeof(char) + 9, GFP_KERNEL);
			if (!data) {
				kfree(buf);
				return (NULL);
			}
			memcpy(&data[pos], curdentry->d_name.name, strlen(curdentry->d_name.name));
			pos += strlen(curdentry->d_name.name);
			memcpy(&data[pos], "        ", 8);
			pos += 8;
			memcpy(&data[pos], path, strlen(path));
			pos += strlen(path);
			memcpy(&data[pos], "\n", 1);
			pos++;
		}
	}
	data = krealloc(data, strlen(curdentry->d_name.name) * sizeof(char) + 9, GFP_KERNEL);
	kfree(buf);
	return (data);
}

static ssize_t read_device(struct file *file, char __user *user_buffer,
                		size_t size, loff_t *offset)
{
	char	*data = get_data();
	int		len = strlen(data);

	if (!user_buffer)
		return (-EINVAL);
	if (*offset >= 8)
		return (0);
	if (size > len)
		size = len;
	if (copy_to_user(user_buffer, data, size))
		return (-EFAULT);
	kfree(data);
	return (size);
}

static struct proc_ops file_ops = {
	.proc_read = read_device,
};

static int __init init_ft(void)
{	
	ent = proc_create("mymounts", 0444, NULL, &file_ops);
	if (!ent) {
		printk(KERN_ALERT "Error at creating the proc device.\n");
		return (-EFAULT);
	}
	printk(KERN_INFO "Proc device created.\n");
	return (0);
}

static void __exit cleanup_ft(void)
{
	remove_proc_entry("mymounts", NULL);
	printk(KERN_INFO "Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psaulnie");
MODULE_DESCRIPTION("Task09 of little-penguin-1");
MODULE_VERSION("1.0");

module_init(init_ft);
module_exit(cleanup_ft);