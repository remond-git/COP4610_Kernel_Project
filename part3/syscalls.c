#include <syscalls.h>
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

#define numFloors 10

struct queueEntries {
  struct list_head list;
  int m_type;
  int m_startFloor;
  int m_destFloor;
};

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

char *str;

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
  stop_s = 1;
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
  int i = 0;
  while(i < numFloors) {
    INIT_LIST_HEAD(&passengerQueue[i]);
    i++;
  }
  INIT_LIST_HEAD(&elevList);
}

//Anytime accessing crit section of list, lock then unlock using appropriate mutex

void queuePassenger(int type, int start, int end) {
  struct queueEntries *newEntry;
  newEntry = kmalloc(sizeof(struct queueEntries), kstuff);
  newEntry->m_type = type; //queues with type, start floor, dest floor
  newEntry->m_startFloor = start;
  newEntry->m_destFloor = end;
  mutex_lock_interruptible(&passengerQueueMutex);
  list_add_tail(&newEntry->list, &passengerQueue[start - 1]);
  mutex_unlock(&passengerQueueMutex);
  PrintQueue();
}

void PrintQueue(void) //prints the queue at each floor
{
  struct list_head *pos;
  struct queueEntries* entry;
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

char* queueToString(void) {
  /*struct queueEntries *entry;
  struct list_head *pos;
  char str[2048];

  char* message;
  int i = 0;
  while(i < numFloors) {
    sprintf(message, "Floor: %d\n", i);
    strcat(str, message);
    i++;
  }*/
  str = "Mew";
  return str;
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

int elevatorMove(int floor) {
  if(floor == currFloor) {
    return 0;
  }
  else {
    printk("Now moving floor to %d\n", floor);
    ssleep(2);
    currFloor = floor;
    return 1;
  }
}

int elevatorRun(void *data) {
  while(kthread_should_stop()) { //keeps running until thread should stop
    if(mainDirection == IDLE) {
      nextDirection = UP;
      if(ifLoad() && !stop_s) {
	mainDirection = LOADING;
      }
      else {
        mainDirection = UP;
	      nextFloor = currFloor + 1;
      }
    }
    else if(mainDirection == UP) {
      elevatorMove(nextFloor);
      if(currFloor == numFloors) {
        nextDirection = DOWN;
	mainDirection = DOWN;
      }
      if((ifLoad() && !stop_s) || ifUnload()) {
        mainDirection = LOADING;
      }
      else if(currFloor == numFloors) {
	nextFloor = currFloor - 1;
      }
      else {
	nextFloor = currFloor + 1;
      }
    }
    else if(mainDirection == DOWN) {
      elevatorMove(nextFloor);
      if(currFloor == 1) {
        nextDirection = UP;
        mainDirection = UP;
      }
      if((elevListSize() && stop_s) && currFloor == 1) { //if reached bottom
        mainDirection = OFFLINE;
	stop_s = 0;
	nextDirection = UP;
      }
      else if(currFloor == 1) {
        nextFloor = currFloor + 1;
      }
      else {
        nextFloor = currFloor - 1;
      }
    }
    else if(mainDirection == LOADING) {
      ssleep(1);
      unloadPassengers();
      while(ifLoad() && !stop_s) {
	//load the single passenger(currFloor)
      }
      mainDirection = nextDirection;
      if(mainDirection == DOWN) {
        if(currFloor == 1) {
	         nextDirection = UP;
	         mainDirection = UP;
	         nextFloor = currFloor + 1;
	      }
	      else {
	         nextFloor = currFloor - 1;
	      }
      }
      else {
	       if(currFloor == numFloors) {
           nextDirection = DOWN;
           mainDirection = DOWN;
           nextFloor = currFloor - 1;
         }
        else {
          nextFloor = currFloor + 1;
        }
      }
    }
  }
  return 0;
}

int ifLoad(void) { //if a load should happen when elevator is not at capacity and going in right direction
  struct queueEntries *entry; //returns 1 if entry should load, 0 otherwise
  struct list_head *pos;
  mutex_lock_interruptible(&passengerQueueMutex);
  list_for_each (pos, &passengerQueue[currFloor - 1]) {
    entry = list_entry(pos, struct queueEntries, list);
    if((passengWeights(entry->m_type) + elevWeight() <= 16) &&
      ((entry->m_destFloor > currFloor && nextDirection == UP) ||
      (entry->m_destFloor < currFloor && nextDirection == DOWN))) {
        mutex_unlock(&passengerQueueMutex);
        return 1;
    }
  }
  mutex_unlock(&passengerQueueMutex);
  return 0;
}


int ifUnload(void) { //checks if elevator should unload (passenger wants to get off)
  struct queueEntries *entry; //returns 1 if any passenger reached dest floor, 0 otherwise
  struct list_head *pos;
  if(elevListSize() == 8) {
    return 0;
  }
  mutex_lock_interruptible(&elevatorListMutex);
  list_for_each (pos, &elevList) {
    entry = list_entry(pos, struct queueEntries, list);
    if(entry->m_destFloor == currFloor) {
      mutex_unlock(&elevatorListMutex);
      return 1;
    }
  }
  mutex_unlock(&elevatorListMutex);
  return 0;
}

void unloadPassengers(void) {
  struct queueEntries *entry;
  struct list_head *pos, *q;
  mutex_lock_interruptible(&elevatorListMutex);
  list_for_each_safe(pos, q, &elevList)  {
    entry = list_entry(pos, struct queueEntries, list);
    if (entry->m_destFloor == currFloor)  { //unloads only if passenger is at correct floor
      printk("Unloaded Passenger!\n");
      passengersServiced++;
      passengersServFloor[entry->m_startFloor - 1]++;
      list_del(pos);
      kfree(entry);
    }
  }
  mutex_unlock(&elevatorListMutex);
}

int elevWeight(void) //returns total weight of elevator
{
  struct queueEntries* entry;
  struct list_head* pos;
  int weight = 0;
  mutex_lock_interruptible(&elevatorListMutex);
  list_for_each(pos, &elevList) {
    entry = list_entry(pos, struct queueEntries, list);
    weight += passengWeights(entry->m_type);
  }
  mutex_unlock(&elevatorListMutex);
  return weight;
}

/*
void loadPassenger(int floor) {
  int weight = elevWeight();
  struct queueEntries *entry;
  struct list_head *pos, *q;
  int i = floor - 1;
  mutex_lock_interruptible(&passengerQueueMutex);
  list_for_each_safe(pos, q, &passenger_queue[i]) {
    entry = list_entry(pos, struct queueEntries, list);
    if((entry->m_startFloor == floor) && ((passengWeights(entry->m_type) + weight) <= 16)) {
      struct queueEntries *n;
      n = kmalloc(sizeof(struct queueEntries), kstuff);
      n->m_type = entry->m_type;
      n->startFloor = entry->startFloor;
      n->destFloor = entry->destFloor;
      mutex_lock_interruptible(&elevatorListMutex);
      list_add_tail(&n->list, &elevList);
      list_del(pos);
      kfree(entry);
      mutex_unlock(&elevListMutex);
      mutex_unlock(&passengerQueueMutex);
      return;
    }
  }
  mutex_unlock(passengerQueueMutex);
}*/

int elevListSize(void) {
  struct queueEntries *entry;
  struct list_head *pos;
  int i = 0;
  mutex_lock_interruptible(&elevatorListMutex);
  list_for_each(pos, &elevList) {
    entry = list_entry(pos, struct queueEntries, list);
    if(entry->m_type == 2) {
      i += 2;
    }
    else {
      i++;
    }
  }
  mutex_unlock(&elevatorListMutex);
  return i;
}
