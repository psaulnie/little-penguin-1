# little-penguin-1
The start of a series of challenges inspired by Eudyptula. You will get acquainted with many points of Kernel development.

# Task00

Retrieve the latest git tree from www.git.kernel.org, mine was [linux-6.3-rc4.tar.gz](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/snapshot/linux-6.3-rc4.tar.gz).
We compile it, ensuring the CONFIG_LOCALVERSION_AUTO option is enabled and boot with it.
Then, we retrieve the logs by putting the dmesg command's output in a file
