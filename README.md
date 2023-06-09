# little-penguin-1
The start of a series of challenges inspired by Eudyptula. You will get acquainted with many points of Kernel development.

# Task 00

Retrieve the latest git tree from [git.kernel.org](www.git.kernel.org), mine was [linux-6.3-rc4](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/snapshot/linux-6.3-rc4.tar.gz).

We compile it, ensuring the CONFIG_LOCALVERSION_AUTO option is enabled and boot with it.

Then, we retrieve the logs by putting the dmesg command's output in a file

# Task 01

A linux module code must contains at least two functions:
  - init_module
  - cleanup_module

To init the module we execute the insmod command (install module) following the .ko we obtain by compiling our code.

To remove the module we execute the rmmod command (remove module) following the .ko

# Task 02

To patch the Makefile :
```
cp Makefile Makefile.orig
vim Makefile # Edit what needs to be patched, here the EXTRAVERSION variable at the beggining
diff -up Makefile{.orig,} > Makefile.patch
```
Then to patch an old Makefile:
```
patch Makefile.orig Makefile.patch
```

# Task 03

The coding style of the linux kernel can be found [here](https://www.kernel.org/doc/html/v4.10/process/coding-style.html)

# Task 04

We need to add a rule in /etc/udev/rules.d/

`ATTR{bInterfaceProtocol}=="01"` represents a keyboard, if we write "02", it will be a mouse.

# Task 05

To make a misc character device, we need to use the `misc_register()` function with two structs:

```
static struct file_operations file_ops = {
	.read = read_device,
	.write = write_device,
};

static struct miscdevice fortytwo_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "fortytwo",
    .fops = &file_ops
};
```
It will create a device file in /dev named fortytwo.

To read from, we use in the `read_device()` function : `copy_to_user()`.

To write to, we use in the `write_device()` function : `copy_from_user()`.

Don't forget to unregister your device when using rmmod by putting the function `misc_deregister()` in the module exit function.

# Task 06

No difficulties for this task, just download the most recent [linux-next source code](https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git/).

# Task 07

To create a directory in /sys/kernel/debug we first need to mount debugfs using this command:
```
mount -t debugfs none /sys/kernel/debug/
```
Then, to create a subdirectory in it, we use this command:
```
struct dentry *debugfs_create_dir(const char *name, struct dentry *parent);
```

# Task 08

The working code will write in reverse the text wrote in the buffer:

```
$> echo "Hello World!" > /dev/reverse
$> cat /dev/reverse

!dlroW olleH
```

# Task 09

To create a proc file, we use the `proc_create()` function and the proc_ops file structure.

You need to loop through all the mounted points using the `list_for_each_entry()` loop.

Then, using the `seq_putc()`, `seq_puts()` and `seq_path()` functions, you can write in the buffer of the proc file.
