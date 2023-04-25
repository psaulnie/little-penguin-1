#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <linux/poll.h>
#include <linux/mnt_namespace.h>
#include <../fs/mount.h>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 

static struct proc_dir_entry *ent;

static int show_mymounts(struct seq_file* m, void* v)
{
	struct mount *mnt;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		struct path			mnt_path = { .dentry = mnt->mnt.mnt_root, .mnt = &mnt->mnt};
		struct super_block	*sb = mnt_path.dentry->d_sb;

		if (!strcmp(mnt->mnt_devname, "rootfs"))
			continue ;
		if (sb->s_op->show_devname)
			sb->s_op->show_devname(m, mnt_path.dentry);
		else if (mnt->mnt_devname)
	    	seq_puts(m, mnt->mnt_devname);
	    seq_putc(m, ' ');
       	seq_path(m, &mnt_path, " \t\n\\");
        seq_putc(m, '\n');
	}
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