### Part 3 - Elevator

A scheduling algorithm for a hotel elevator.

Adding to Kernel Instructions:

Files to modify (core kernel):
– /usr/src/test_kernel/arch/x86/entry/syscalls/syscall_64.tbl

Go to Line:342 and find System call table, add lines to system call table for system calls.

327 64 preadv2 sys_preadv2
328 64 pwritev2 sys_pwritev2
329 common pkey_mprotect sys_pkey_mprotect
330 common pkey_alloc sys_pkey_alloc
331 common pkey_free sys_pkey_free
332 common statx sys_statx
353 common start_elevator sys_start_elevator  <-----
354 common issue_request sys_issue_request <-----
355 common stop_elevator sys_stop_elevator <-----


– /usr/src/test_kernel/include/linux/syscalls.h

At the end of the document add the function thingy like so:

 asmlinkage long sys_statx(int dfd, const char __user *path, unsigned flags,
● unsigned mask, struct statx __user *buffer);
●
● asmlinkage long sys_issue_request(int passenger_type, int start_floor, int destination_floor); <-----
● asmlinkage long sys_start_elevator(void) <------
● asmlinkage long sys_stop_elevator(void) <-------
●
● #endif


– /usr/src/test_kernel/Makefile

Go to Line:953

Add the folder with the system call (part 3 or elevator or whatever we call it) to the directory list

ifeq ($(KBUILD_EXTMOD),)                                          [UNDER HERE]
● core-y += kernel/ certs mm/ fs/ ipc/ security/ crypto/ block/ example3_syscall/
●
● vmlinux-dirs := $(patsubst %/,%,$(filter %/, $(init-y) $(init-m) \
● $(core-y) $(core-m) $(drivers-y) $(drivers-m) \
● $(net-y) $(net-m) $(libs-y) $(libs-m) $(virt-y)))
●
● vmlinux-alldirs := $(sort $(vmlinux-dirs) $(patsubst %/,%,$(filter %/, \
● $(init-) $(core-) $(drivers-) $(net-) $(libs-) $(virt-))))
