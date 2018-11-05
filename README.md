# proj_2

COP 4610 Project 2 : Kernel Project

The members of this project are:
1) Emmanuel Gonzalez
2) John Sanchez
3) Andrew Stringfellow

proj_2_gonzalez_sanchez_stringfellow.tar contents:
  part1
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

Completed using: linprog, linux(ubuntu), Atom(IDE)

To Insert Module of Part 2:
$> sudo insmod my_xtime.ko

To Insert Module of Part 3:
$> sudo insmod elevator.ko

To Remove Module of Part 2:
$> sudo rmmod my_xtime

To Remove Module of Part 3:
$> sudo rmmod elevator

To Build (in part 2 and 3):
$> make

To Clean:
$> make clean

Known Bugs & Unfinished Portions:

Special Considerations:


==============================
==============================
Report
==============================
==============================



==============================
Problem Statement
==============================

Part 1:
In this part we are implementing ten syscalls to be tracked by the kernel...

Part 2:
In this part we are implementing a timer module that counts the time between two intervals...

Part 3:
In this part we are implementing three syscalls that operates an elevator which services passengers based on weight and desired floor; includes mutexes.

==============================
Steps Taken To Solve Part 2
==============================
1) Learned how to insert modules into the kernel.
2) Noted the time differences in seconds and nanoseconds between two
time intervals.
3) Implemented 
4) Implemented Built-Ins
5) Implemented Input Redirection
6) Implemented Environment Variables
7) Implemented piping
8) Implemented Background Processing
9) Ran tests to make sure all requirements were working and satisfied
10) Completed

==============================
Steps Taken To Solve Part 3
==============================
1) Experimented with bash on the linprog server.
2) Took notes on output for required built ins, piping , I/O, etc.
3) Implemented User Input / Parsing to get commands/arguments
4) Implemented Built-Ins
5) Implemented Input Redirection
6) Implemented Environment Variables
7) Implemented piping
8) Implemented Background Processing
9) Ran tests to make sure all requirements were working and satisfied
10) Completed



==============================
Division of Labor
==============================

Gonzalez:
• Parsing
• Environment Variables
• Prompt
• Path Resolution
• Built In: io
• Built In: pwd

Sanchez:
• Execution
• Piping
• Built-In: cd
• Built-In: exit
• Error Handling

Money:
• Background Processing
• I/O Redirection