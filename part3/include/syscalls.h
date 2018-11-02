#ifndef __ELEVATOR_SYSCALLS_H
#define __ELEVATOR_SYSCALLS_H

void elevator_syscalls_create(void);
void elevator_syscalls_remove(void);
void initQueue(void);
void queuePassenger(int type, int start, int end);
void PrintQueue(void);
int passengWeights(int type); //returns weight for type
int passengQueueSize(int floor); //counts entries in queue
int passengQueueWeights(int floor); //counts total weight in queue
int elevatorMove(int floor);
int elevWeight(void); //weight of elevator
int elevatorRun(void); //Actually runs the elevator
int ifLoad(void); //If load should occur, returns 1 if true, returns 0 otherwise
int ifUnload(void); //If unload should occur, returns 1 true, returns 0 otherwise
void unloadPassengers(void); //Unloads each passenger (frees them)

#endif
