
# COP 4610: Project 2

- Group members:
  - Emmanuel Gonzalez
  - John Sanchez
  - Andrew Stringfellow

`proj_2_gonzalez_sanchez_stringfellow.tar` contents:
```
part 1
    part1.c
    Makefile
part2
    my_xtime
        my_xtime.c
        Makefile
part3
    include
        syscalls.h
    src
        syscalls.c
        module.c
        sys_issue_request.c
        sys_start_elevator.c
        sys_stop_elevator.c
    Makefile
```

Completed using: Linux (Ubuntu 18.04 running Linux Kernel 4.14.12), Atom (IDE)

To insert the kernel module for Part 2:
`$ sudo insmod my_xtime.ko`

To insert the kernel module for Part 3:
`$ sudo insmod elevator.ko`

To remove the kernel module for Part 2:
`$ sudo rmmod my_xtime`

To remove the kernel module for Part 3:
`$ sudo rmmod elevator`

To build (for Part 2 and 3):
`$ make`

To clean:
`$ make clean`

## Report

### Known bugs & unfinished portions:

### Special considerations:

## Problem Statement

### Part 1
A small C program that makes 10 syscalls more than an empty program.

### Part 2
A kernel module that stores the time since the UNIX epoch (January 1, 1990). On the first `proc` read, only the value of `xtime` is stored. On all subsequent `proc` reads, the difference of the current and last `xtime` values is also stored.

### Part 3
A kernel module that utilizes new syscalls, threading, and locks to simulate an elevator scheduling algorithm.

### Steps taken to solve Part 2
1) Learned how to insert modules into the kernel.
2) Noted the time differences in seconds and nanoseconds between two time intervals.

### Steps taken to solve Part 3

## Division of Labor
- Gonzalez
    - Added ...
- Sanchez
    - Added ...
- Stringfellow
    - Added ...