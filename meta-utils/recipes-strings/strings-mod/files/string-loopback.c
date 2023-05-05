#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro");
MODULE_DESCRIPTION("String loopback.");
MODULE_VERSION("1.0");

#define PROC_ENTRY_FILENAME "str_loopback"
#define LPBK_MAX_USER_SIZE 1024

static struct proc_dir_entry *lpbk_proc = NULL;
static char data_buffer[LPBK_MAX_USER_SIZE];

static char welc_msg_buffer[] = "The proc file is alive!\n";

ssize_t lpbk_read(struct file *file, char __user *user, size_t size, loff_t *off)
{
    return simple_read_from_buffer(user, size, off, welc_msg_buffer, sizeof(welc_msg_buffer));
}

ssize_t lpbk_write(struct file *file, const char __user *user, size_t size, loff_t *off)
{
    memset(data_buffer, 0x0, sizeof(data_buffer));

    if (size > LPBK_MAX_USER_SIZE)
    {
        size = LPBK_MAX_USER_SIZE;
    }

    if (strncpy_from_user(data_buffer, user, size) < 0)
    {
        return -EFAULT;
    }

    printk(KERN_INFO "---> '%s'\n", data_buffer);

    return size;
}

static const struct proc_ops lpbk_proc_fops =
    {
        .proc_read = lpbk_read,
        .proc_write = lpbk_write,
};

static int __init lpbk_init(void)
{
    lpbk_proc = proc_create(PROC_ENTRY_FILENAME, 0666, NULL, &lpbk_proc_fops);
    if (lpbk_proc == NULL)
    {
        printk(KERN_ERR "Failed to create proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "String Loopback module loaded.\n");

    return 0;
}

static void __exit lpbk_exit(void)
{
    proc_remove(lpbk_proc);
    printk(KERN_INFO "String Loopback module unloaded.\n");
}

module_init(lpbk_init);
module_exit(lpbk_exit);
