waitdisks
=========

Waitdisks is a small Unix utility. All it does is wait for the local
device nodes to appear, and exits with success; or failure if the
timeout elapses or an error occurs.

Its use is with traditional init systems to wait until udevd populates
the device tree with all the disk devices before attempting to fsck or
mount them.

Its usage is:

	waitdisks [ -f file ]  [ -t timeout_secs ]

The -f option lets you specify a different file (besides /etc/fstab)
to scan for fstab entries to watch for. The -t option lets you specify
a different timeout (default is 30 sec.).

It doesn't depend on anything but Linux (for now) and libc.
