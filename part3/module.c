#include <linux/module.h> // Dynamic loading of modules into the kernel.
#include <linux/init.h> // Macros for module initualization.
#include <linux/proc_fs.h> // File system structure and calls.

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An elevator simulator utilizing first-in first-out scheduling.");

#define MODULE_NAME "Elevator"
#define MODULE_PERMISSIONS 0644
#define MODULE_PARENT NULL

static struct file_operations fileOperations; // Points to proc file definitions.

static int InitializeModule(void) {
    printk(KERN_NOTICE "Creating /proc/%s.\n", MODULE_NAME);

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
