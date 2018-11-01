//#include <syscalls.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

#define kstuff (__GFP_RECLAIM | __GFP_IO | __GFP_FS)

#define OFFLINE 0
#define IDLE 1
#define UP 2
#define DOWN 3
#define LOADING 4

extern int numFloors = 10;

struct queueEntries {
  struct list_head list;
  int m_type;
  int m_startFloor;
  int m_destFloor;
}

struct list_head passengerQueue[numFloors];
struct list_head elevList;

extern int stop_s;
extern int mainDirection;
extern int nextDirection;
extern int currFloor;
extern int nextFloor;
extern int numPassengers;
extern int numWeight;
extern int waitPassengers;
extern int passengersServiced;
extern int passengersServFloor[numFloors];

extern struct task_struct* elevator_thread;
extern struct mutex passengerQueueMutex;
extern struct mutex elevatorListMutex;

extern long (*STUB_start_elevator)(void);
  long start_elevator(void) {
  if(stop_s) {
    stop_s = 0;
    return 0;
  }
  else if(mainDirection == OFFLINE) {
    printk("Starting elevator\n");
    mainDirection = IDLE;
    return 0;
  }
  else {
    return 1;
  }
}

extern long (*STUB_issue_request)(int,int,int);
  long issue_request(int passenger_type, int start_floor, int destination_floor) {
  printk("New request: %d, %d => %d\n", passenger_type, start_floor, destination_floor);
  if(start_floor == destination_floor) {
    passengersServiced++;
    passengersServFloor[start_floor - 1]++;
  }
  else {
    queuePassenger(passenger_type, start_floor, destination_floor);
  }
  return 0;
}

extern long (*STUB_stop_elevator)(void);
  long stop_elevator(void) {
  printk("Stopping elevator\n");
  if(stop_s) {
    return 1;
  }
  else {
    stop_s = 1
  }
  return 0;
}

void elevator_syscalls_create(void) {
  STUB_issue_request = &(issue_request);
  STUB_start_elevator = &(start_elevator);
  STUB_stop_elevator = &(stop_elevator);
}

void elevator_syscalls_remove(void) {
  STUB_issue_request = NULL;
  STUB_start_elevator = NULL;
  STUB_stop_elevator = NULL;
}

void initQueue(void) {
  int i;
  while(i < numFloors) {
    INIT_LIST_HEAD(&passengerQueue[i]);
    i++;
  }
  INIT_LIST_HEAD(&elevList);
}

void queuePassenger(int type, int start, int end) {
  struct queueEntries *newEntry;
  newEntry = kmalloc(sizeof(struct queueEntries), kstuff);
  newEntry->m_type = type;
  newEntry->m_startFloor = start;
  newEntry->m_destFloor = end;
  mutex_lock_interruptible(&passengerQueueMutex);
  list_add_tail(&newEntry->list, &passengerQueue[start - 1]);
  mutex_unlock(&passengerQueueMutex);
  PrintQueue();
}

void PrintQueue(void)
{
  struct list_head *pos;
  struct queueEntry* entry;
  int currentPos = 0;
  int i = 0;
  printk("Passenger Queue:\n");
  mutex_lock_interruptible(&passengerQueueMutex);
  while(i < numFloors)
  {
    printk("Floor: %d\n", i+1);
    list_for_each(pos, &passengerQueue[i]) {
      entry = list_entry(pos, struct queueEntries, list);
      printk("Queue pos: %d\nType: %d\nStart Floor: %d\nDest Floor: %d\n",
	currentPos, entry->m_type, entry->m_startFloor,
	entry->m_destFloor);
	++currentPos;
    }
    i++;
  }
  mutex_unlock(&passengerQueueMutex);
  printk("\n");
}

int passengWeights(int type) {
  if(type == 0)
    return 2;
  else if (type == 1)
    return 1;
  else if (type == 2 || type == 3)
    return 4;
  else
    return 0;
}
