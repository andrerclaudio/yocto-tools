#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <gpio.h>
#include <stdio.h>

#ifndef CONSUMER
#define CONSUMER "Consumer"
#endif

#define PROC_ENTRY_FILENAME "statectrl"
#define CTRL_MAX_USER_SIZE 1024

typedef enum
{

    CLOSE_CHIP = 0,
    RELEASE_LINE,

} gpiod_finish_t;

static struct gpiod_chip *chip;
static struct gpiod_line *line;

static struct proc_dir_entry *stateCtrl_proc = NULL;
static char data_buffer[CTRL_MAX_USER_SIZE];

static char welc_msg_buffer[] = "The proc file is alive!\n";

static const struct proc_ops stateCtrl_proc_fops =
    {
        .proc_read = stateCtrl_read,
        .proc_write = stateCtrl_write,
};

static void gpiod_finish(gpiod_finish_t hw_identifier)
{
    /*
    Release the requested hardware passing the hardware which should be close.
    */

    switch (hw_identifier)
    {
    default:
    case RELEASE_LINE:
        // If you got problems to set the pin direction, release the line and close the chip
        printk(KERN_INFO "Line released!");
        gpiod_line_release(line);
    case CLOSE_CHIP:
        // If you got problems to get the line, close the chip
        printk(KERN_INFO "Chip closed!");
        gpiod_chip_close(chip);
    }
}

ssize_t stateCtrl_read(struct file *file, char __user *user, size_t size, loff_t *off)
{
    return simple_read_from_buffer(user, size, off, welc_msg_buffer, sizeof(welc_msg_buffer));
}

ssize_t stateCtrl_write(struct file *file, const char __user *user, size_t size, loff_t *off)
{
    unsigned char val;
    int ret;

    memset(data_buffer, 0x0, sizeof(data_buffer));

    if (size > CTRL_MAX_USER_SIZE)
    {
        size = CTRL_MAX_USER_SIZE;
    }

    if (strncpy_from_user(data_buffer, user, size) < 0)
    {
        return -EFAULT;
    }

    printk(KERN_INFO "---> '%s'\n", data_buffer);

    switch (data_buffer)
    {
    case "0":
        val = 0;
        break;

    case "1":
        val = 1;
        break;

    default:
        val = 0;
        break;
    }

    ret = gpiod_line_set_value(line, val);
    if (ret < 0)
    {
        printk(KERN_ERR "Set line output failed\n");
        gpiod_finish(RELEASE_LINE);
        return EXIT_FAILURE;
    }

    return size;
}

static int __init statectrl_init(void)
{
    stateCtrl_proc = proc_create(PROC_ENTRY_FILENAME, 0666, NULL, &stateCtrl_proc_fops);
    if (stateCtrl_proc == NULL)
    {
        printk(KERN_ERR "Failed to create proc entry\n");
        return -ENOMEM;
    }

    chip = gpiod_chip_open_by_name(chipname);
    if (!chip)
    {
        printk(KERN_ERR "Open chip failed\n");
        return EXIT_FAILURE;
    }

    line = gpiod_chip_get_line(chip, line_num);
    if (!line)
    {
        printk(KERN_ERR "Get line failed\n");
        gpiod_finish(CLOSE_CHIP);
        return EXIT_FAILURE;
    }

    if (gpiod_line_request_output(line, CONSUMER, 0) < 0)
    {
        printk(KERN_ERR "Request line as output failed\n");
        gpiod_finish(RELEASE_LINE);
        return EXIT_FAILURE;
    }

    printk(KERN_INFO "Led state control module loaded.\n");
    return EXIT_SUCCESS;
}

static void __exit statectrl_exit(void)
{
    gpiod_finish(RELEASE_LINE);
    proc_remove(stateCtrl_proc);
    printk(KERN_INFO "Led state control module unloaded.\n");
    return EXIT_SUCCESS;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro");
MODULE_DESCRIPTION("Change the led state");
MODULE_VERSION("1.0");

module_init(statectrl_init);
module_exit(statectrl_exit);
