MODULE_NAME = elevator
PWD := $(shell pwd)

KDIR := /lib/modules/$(shell uname -r)/build
ccflags-y += -I$(src)/include

obj-y := src/sys_start_elevator.o src/sys_issue_request.o src/sys_stop_elevator.o

obj-m := module.o # Compile files as module.

$(MODULE_NAME)-objs += src/syscalls.o
$(MODULE_NAME)-objs += src/module.o
obj-m := $(MODULE_NAME).o

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
