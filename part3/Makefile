ELEVATOR_MODULE = /usr/src/test_kernel/elevator
.PHONY: compile insert remove start issue stop watch_proc clean

compile: producer.c consumer.c wrappers.h
	gcc -o producer.x producer.c
	gcc -o consumer.x consumer.c

insert:
	make -C $(ELEVATOR_MODULE) && sudo insmod $(ELEVATOR_MODULE)/elevator.ko
remove:
	sudo rmmod elevator

start: compile
	./consumer.x --start
issue: compile
	./producer.x
stop: compile
	./consumer.x --stop

watch_proc:
	while [ 1 ]; do \
		clear; clear; \
		cat /proc/elevator; \
		sleep 1; \
	done

clean:
	rm *.x
