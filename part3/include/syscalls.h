#ifndef __ELEVATOR_SYSCALLS_H
#define __ELEVATOR_SYSCALLS_H

void elevator_syscalls_create(void);
void elevator_syscalls_remove(void);
void initQueue(void);
void queuePassenger(int type, int start, int end);
void PrintQueue(void);
int passengWeights(int type);
int elevatorMove(int floor);

#endif
