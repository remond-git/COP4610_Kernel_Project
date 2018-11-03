ifneq ($(KERNELRELEASE),)
  obj-y := sys_issue_request.o sys_start_elevator.o sys_stop_elevator.c
  obj-m := module.o syscalls.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -f *.ko *.o Module* *mod*
