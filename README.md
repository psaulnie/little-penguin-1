# little-penguin-1
The start of a series of challenges inspired by Eudyptula. You will get acquainted with many points of Kernel development.

# Task 00

Retrieve the latest git tree from www.git.kernel.org, mine was [linux-6.3-rc4](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/snapshot/linux-6.3-rc4.tar.gz).

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
