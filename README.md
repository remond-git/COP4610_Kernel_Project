# COP 4610: Project 2

- Group members:
  - Emmanuel Gonzalez
  - John Sanchez
  - Andrew Stringfellow

## Problem Statement

#### Part 1
A small C program that makes 10 syscalls more than an empty program.

#### Part 2
A kernel module that stores the time since the UNIX epoch (January 1, 1990). On the first `proc` read, only the value of `xtime` is stored. On all subsequent `proc` reads, the difference of the current and last `xtime` values is also stored.

#### Part 3
A kernel module that utilizes new syscalls, threading, and locks to simulate an elevator scheduling algorithm.

## Archive Contents

Filename: `proj_2_gonzalez_sanchez_stringfellow.tar`

```
part 1
    Makefile // Compiles both part1.c and emptyProgram.c and generates their respective log files.
    part1.c // A C file that contains 10 more system calls than the empty program.
    emptyProgram.c // A C file that contains a basic, empty main function.
part2
    Makefile // Compiles the my_xtime kernel module.
    my_xtime.c // A kernel module that stores the time since the UNIX epoch and the elapsed time.
part3
    Makefile // Compiles the kernel module and creates elevator.ko.
    include
        syscalls.h // Function declarations for syscalls.c.
    src
        syscalls.c // The elevator scheduling algorithm implementation.
        module.c // Handles the initialization and exiting of the kernel module along with the writing of output to the proc file.
        sys_start_elevator.c // Wrapper and stub for the start_elevator syscall.
        sys_issue_request.c // Wrapper and stub for the issue_request syscall.
        sys_stop_elevator.c // Wrapper and stub for the stop_elevator syscall.
```

Completed using: Ubuntu 18.04 LTS running Linux kernel 4.14.12

## Build Instructions
To build (for Part 1, 2, and 3): `$ make`

To clean: `$ make clean`

To compare the log files for Part 1: `$ vimdiff emptyProgram.log part1.log`

To insert the kernel module for Part 2: `$ sudo insmod my_xtime.ko`

To remove the kernel module for Part 2: `$ sudo rmmod my_xtime`

To insert the kernel module for Part 3: `$ sudo insmod elevator.ko`

To remove the kernel module for Part 3: `$ sudo rmmod elevator`

## Known Bugs and Unfinished Portions
- Part 3
  - If illegitimate passengers are added to the queue, the passengers get stuck in the elevator not wanting to get off.
  - Sometimes during the stress test, the elevator randomly pauses before resuming operation.
  - Sometimes the computer freezes after adding the module but then resumes.

## Division of Labor
- Emmanuel Gonzalez
    - Created the kernel module for Part 2.
    - Created the listing functions.
- John Sanchez
    - Implemented elevator run function and mutexes.
    - Added needed variables.
    - Added structures to syscall file.
    - Fixed the calculation of elapsed time for the kernel module in Part 2.
- Andrew Stringfellow
    - Set up the machine and compiled the kernel.
    - Set up the module file.
    - Fixed the calculation of elapsed time for the kernel module in Part 2.