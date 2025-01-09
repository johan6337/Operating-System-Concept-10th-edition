#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>


#define PROC_NAME "jiffies"

static ssize_t proc_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
    char buffer[32];
    int len;

    // Prevent multiple reads from the same offset
    if (*ppos > 0 || count < 32)
        return 0;
    unsigned long int i = jiffies / HZ;
    // Get the current jiffies value and write to the buffer
    len = snprintf(buffer, sizeof(buffer), "%lu\n", i);
    // Copy the data to user space
    if (copy_to_user(ubuf, buffer, len))
        return -EFAULT;

    *ppos = len; // Update the file position
    return len;
}

// File operations structure for /proc/jiffies
static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

static int __init jiffies_module_init(void) {
    // Create the /proc/jiffies entry
    if (!proc_create(PROC_NAME, 0, NULL, &proc_ops)) {
        pr_err("Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit jiffies_module_exit(void) {
    // Remove the /proc/jiffies entry
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

module_init(jiffies_module_init);
module_exit(jiffies_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel module to display time since boot in /proc");