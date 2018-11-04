#include <linux/module.h> // Dynamic loading of modules into the kernel.
#include <linux/init.h> // Macros for module initialization.
#include <linux/proc_fs.h> // File system structure and calls.
#include <linux/slab.h> // Memory allocation functions: kmalloc() and kfree().
#include <linux/uaccess.h> // Memory copy.
#include <linux/string.h> // String functions.
#include <linux/time.h> // Time types.

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Display the value of xtime on each proc read.");

#define MODULE_NAME "timed"
#define MODULE_PERMISSIONS 0644
#define MODULE_PARENT NULL
#define STRING_LENGTH 256

static struct file_operations fileOperations; // Points to proc file definitions.
static char *message1;
static char *message2;
static int readProc;
static int procReadNumber = 0;
struct timespec currentTime;
long elapsedSeconds;
long elapsedNanoseconds;

static struct Time {
    long seconds;
    long nanoseconds;
} time;

int OpenProc(struct inode *sp_inode, struct file *sp_file) {
    printk(KERN_INFO "OpenProc() called.\n");
    readProc = 1;

    procReadNumber++;
    
    message1 = kmalloc(sizeof(char) * STRING_LENGTH, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
    message2 = kmalloc(sizeof(char) * STRING_LENGTH, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
    if (message1 == NULL || message2 == NULL) {
        printk(KERN_WARNING "Error: could not allocate memory for a string.");

        return -ENOMEM;
    }
    
    currentTime = current_kernel_time();
    sprintf(message1, "Current time: %ld.%ld\n", currentTime.tv_sec, currentTime.tv_nsec);

    if (currentTime.tv_nsec - time.nanoseconds < 0) {
        elapsedSeconds = currentTime.tv_sec - time.seconds - 1;
        elapsedNanoseconds = currentTime.tv_nsec - time.nanoseconds + 1000000000;
    } else {
        elapsedSeconds = currentTime.tv_sec - time.seconds;
        elapsedNanoseconds = currentTime.tv_nsec - time.nanoseconds;
    }

    if (procReadNumber > 1) {
        sprintf(message2, "Elapsed time: %ld.%ld\n", elapsedSeconds, elapsedNanoseconds);
        strcat(message1, message2);
    }

    time.seconds = currentTime.tv_sec;
    time.nanoseconds = currentTime.tv_nsec;
    
    return 0;
}

ssize_t ReadProc(struct file *sp_file, char __user *buffer, size_t size, loff_t *offset) {
    int length = strlen(message1);

    readProc = !readProc;
    if (readProc) {
        return 0;
    }

    printk(KERN_INFO "ReadProc() called.\n");
    copy_to_user(buffer, message1, length);

    return length;
}

int ReleaseProc(struct inode *sp_inode, struct file *sp_file) {
    printk(KERN_NOTICE "ReleaseProc() called.\n");
    kfree(message1);
    kfree(message2);

    return 0;
}

static int InitializeModule(void) {
    printk(KERN_NOTICE "Creating /proc/%s.\n", MODULE_NAME);

    fileOperations.open = OpenProc;
    fileOperations.read = ReadProc;
    fileOperations.release = ReleaseProc;

    if (!proc_create(MODULE_NAME, MODULE_PERMISSIONS, MODULE_PARENT, &fileOperations)) {
        printk(KERN_WARNING "Error: could not create proc entry.\n");
        remove_proc_entry(MODULE_NAME, MODULE_PARENT);

        return -ENOMEM;
    }

    return 0;
}

static void ExitModule(void) {
    printk(KERN_NOTICE "Removing /proc/%s.\n", MODULE_NAME);
    remove_proc_entry(MODULE_NAME, MODULE_PARENT);
}

module_init(InitializeModule);
module_exit(ExitModule);
