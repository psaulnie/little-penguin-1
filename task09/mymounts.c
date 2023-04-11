#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/list.h>  
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <asm/current.h>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

static struct proc_dir_entry *ent;

static int show_mymounts(struct seq_file* m, void* v)
{
	struct mount *mnt;

	// list_for_each_entry(mnt, &current->nsproxy->)
	seq_putc(m, 'c');
	seq_putc(m, '\n');
	return (0);
}

static int open_procfile(struct inode* inode, struct file* file)
{
    return single_open(file, show_mymounts, NULL);
}

static struct proc_ops file_ops = {
	.proc_open = open_procfile,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release
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