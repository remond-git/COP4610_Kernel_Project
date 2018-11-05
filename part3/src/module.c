#include <linux/module.h> // Dynamic loading of modules into the kernel.
#include <linux/init.h> // Macros for module initialization.
#include <linux/proc_fs.h> // File system structure and calls.
#include <linux/slab.h> // Memory allocation functions: kmalloc() and kfree().
#include <linux/uaccess.h> // Memory copy.
#include <linux/kthread.h> // Multi-threading functions.
#include <linux/mutex.h> // Mutual exclusion/lock functions.
#include <syscalls.h> // Function declarations.

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An elevator simulator utilizing first-in, first-out scheduling.");

// Used for proc file system.
#define MODULE_NAME "elevator"
#define MODULE_PERMISSIONS 0644
#define MODULE_PARENT NULL

// The possible states of the elevator.
#define OFFLINE 0
#define IDLE 1
#define UP 2
#define DOWN 3
#define LOADING 4

#define numFloors 10

int stop_s;
int mainDirection;
int nextDirection;
int currFloor;
int nextFloor;
int numPassengers;
int numWeight;
int waitPassengers;
int passengersServiced;
int passengersServFloor[numFloors];

int i;
int rp;
char * message;

struct mutex passengerQueueMutex;
struct mutex elevatorListMutex;

struct task_struct* elevator_thread;

static struct file_operations fileOperations; // Points to proc file definitions.

int OpenModule(struct inode *sp_inode, struct file *sp_file) {
   printk(KERN_NOTICE "OpenModule Called\n");
   rp = 1;
   message = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   if(message == NULL) {
     printk("Error: OpenModule");
     return -ENOMEM;
   }
   return 0;
}

char *directionToString(int mainDirection) {
  static char str[32];

  switch (mainDirection) {
    case 0:
      sprintf(str, "OFFLINE");
      break;

    case 1:
      sprintf(str, "IDLE");
      break;

    case 2:
      sprintf(str, "UP");
      break;
    
    case 3:
      sprintf(str, "DOWN");
      break;

    case 4:
      sprintf(str, "LOADING");
      break;
  
    default:
      sprintf(str, "ERROR");
      break;
  }

  return str;
}

ssize_t ReadModule(struct file *sp_file, char __user *buff, size_t size, loff_t *offset) {
  int n;
  numPassengers = elevListSize();
  numWeight = elevWeight();
  n = numWeight % 1;
  if (n) {
    sprintf(message, "Main elevator direction: %s \nCurrent floor: %d \nNext floor: %d \nCurrent passengers: %d \nCurrent Weight: %d.5 units\nPassengers serviced: %d \n, Passengers waiting: %s \n",
directionToString(mainDirection), currFloor, nextFloor, numWeight, numPassengers, passengersServiced, queueToString());
  } else {
    sprintf(message, "Main elevator direction: %s \nCurrent floor: %d\nNext floor: %d\nCurrent passengers: %d\nCurrent Weight: %d units\nPassengers serviced: %d\n, Passengers waiting: %s \n",
directionToString(mainDirection), currFloor, nextFloor, numWeight, numPassengers, passengersServiced, queueToString());
  }

  rp = !rp;
  if (rp) {
    return 0;
  }

  printk(KERN_NOTICE "ReadModule() called.\n");
  copy_to_user(buff, message, strlen(message));

  return strlen(message);
}

int ReleaseModule(struct inode *sp_inode, struct file *sp_file) {
  printk(KERN_NOTICE "ReleaseModule() called.\n");
  kfree(message);

  return 0;
}

static int InitializeModule(void) {
    printk(KERN_NOTICE "Creating /proc/%s.\n", MODULE_NAME);

    fileOperations.open = OpenModule;
    fileOperations.read = ReadModule;
    fileOperations.release = ReleaseModule;

    mainDirection = OFFLINE;  // initialize mainDirection
    nextDirection = UP;
    stop_s = 1;
    currFloor = 1;
    nextFloor = 1;
    numPassengers = 0;
    numWeight = 0;
    waitPassengers = 0;
    for(i = 0; i < 10; i++) {
      passengersServFloor[i] = 0;
    }
    initQueue();
    elevator_syscalls_create();
    mutex_init(&passengerQueueMutex);
    mutex_init(&elevatorListMutex);
    elevator_thread = kthread_run(elevatorRun, NULL, "Elevator Thread");
    if(IS_ERR(elevator_thread)) {
      printk("Error: ElevatorRun\n");
      return PTR_ERR(elevator_thread);
    }
    if(!proc_create(MODULE_NAME, MODULE_PERMISSIONS, NULL, &fileOperations)) {
      printk("Error: proc_create\n");
      remove_proc_entry(MODULE_NAME, NULL);
      return -ENOMEM;
    }
    return 0;
}

static void ExitModule(void) {
    int r;
    remove_proc_entry(MODULE_NAME, NULL);
    elevator_syscalls_remove();
    printk(KERN_NOTICE "Removing /proc/%s.\n", MODULE_NAME);
    r = kthread_stop(elevator_thread);
    if(r != -EINTR) {
      printk("Elevator stopped...\n");
    }
}

module_init(InitializeModule);
module_exit(ExitModule);
