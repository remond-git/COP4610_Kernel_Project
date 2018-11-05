#ifndef __ELEVATOR_SYSCALLS_H
#define __ELEVATOR_SYSCALLS_H

void elevator_syscalls_create(void);
void elevator_syscalls_remove(void);
void initQueue(void);
void queuePassenger(int type, int start, int end);
void PrintQueue(void);
char* queueToString(void);
int passengWeights(int type); //returns weight for type
int passengerQueueSize(int floor); //counts size in queue of floor
int passengerQueueWeight(int floor); //counts total weight in queue of floor
int elevatorMove(int floor);
int elevWeight(void); //weight of elevator
int elevSize(void);
int elevatorRun(void); //Actually runs the elevator
int ifLoad(void); //If load should occur, returns 1 if true, returns 0 otherwise
int ifUnload(void); //If unload should occur, returns 1 true, returns 0 otherwise
void loadPassenger(int floor);
void unloadPassengers(void); //Unloads each passenger (frees them)

#endif
