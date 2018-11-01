//#include <ElevSysCalls.h>
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

extern int stop_;
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

/*void createElevSyscalls(void) {
  issueRequest = &issueRequest;
  startElevator = &startElevator;
  stopElevator = &stopElevator;
}

void removeElevSyscalls(void) {
  issueRequest = NULL;
  startElevator = NULL;
  stopElevator = NULL;
}*/

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
