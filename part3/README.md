# Part 3 - Elevator
 
##### A scheduling algorithm for a hotel elevator.
 
 
 
## Adding to Kernel Instructions:
 
### Module Files:
#### Basically drop the whole folder under the test kernel like this:
/usr/src/test_kernel/part3/ <br />
 
### Files to modify (core kernel):
#### – /usr/src/test_kernel/arch/x86/entry/syscalls/syscall_64.tbl 
 
Go to Line:342 and find System call table, add lines to system call table for system calls. <br />
 <br />
327 64 preadv2 sys_preadv2 <br />
328 64 pwritev2 sys_pwritev2 <br />
329 common pkey_mprotect sys_pkey_mprotect <br />
330 common pkey_alloc sys_pkey_alloc <br />
331 common pkey_free sys_pkey_free <br />
332 common statx sys_statx <br />
353 common start_elevator sys_start_elevator  <----- <br />
354 common issue_request sys_issue_request <----- <br />
355 common stop_elevator sys_stop_elevator <----- <br />
 <br />
#### – /usr/src/test_kernel/include/linux/syscalls.h 
 <br />
At the end of the document add the function thingy like so: <br />
 <br />
 asmlinkage long sys_statx(int dfd, const char __user *path, unsigned flags, <br />
● unsigned mask, struct statx __user *buffer); <br />
●
● asmlinkage long sys_issue_request(int passenger_type, int start_floor, int destination_floor); <----- <br />
● asmlinkage long sys_start_elevator(void) <------ <br />
● asmlinkage long sys_stop_elevator(void) <------- <br />
● <br />
● #endif <br />

#### – /usr/src/test_kernel/Makefile
<br />
Go to Line:953 <br />
 <br />
Add the folder with the system call (part 3 or elevator or whatever we call it) to the directory list <br />
 <br />
ifeq ($(KBUILD_EXTMOD),)                                           <br />
 core-y += kernel/ certs mm/ fs/ ipc/ security/ crypto/ block/ example3_syscall/ <-----------------
 <br />
 vmlinux-dirs := $(patsubst %/,%,$(filter %/, $(init-y) $(init-m) \ <br />
 $(core-y) $(core-m) $(drivers-y) $(drivers-m) \ <br />
$(net-y) $(net-m) $(libs-y) $(libs-m) $(virt-y))) <br />
 <br />
vmlinux-alldirs := $(sort $(vmlinux-dirs) $(patsubst %/,%,$(filter %/, \ <br />
$(init-) $(core-) $(drivers-) $(net-) $(libs-) $(virt-))))  <br />
 
<br />
