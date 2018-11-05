all:
	gcc -o part1.x part1.c
	gcc -o emptyProgram.x emptyProgram.c
	strace -o part1.log ./part1.x
	strace -o emptyProgram.log ./emptyProgram.x
clean:
	rm *.x *.log
