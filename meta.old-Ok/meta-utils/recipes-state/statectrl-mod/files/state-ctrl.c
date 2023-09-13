#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#ifndef CONSUMER
    #define CONSUMER "Consumer"
#endif

#define PROC_ENTRY_FILENAME         "statectrl"
#define CTRL_MAX_USER_SIZE          1024
#define CHIP_NUMBER                 5
#define LINE_NUMBER                 9
#define IMX_GPIO_NR(port, index)    ((((port)-1)*32)+((index)&31))

/* Global variables */
static int pin_number = IMX_GPIO_NR(CHIP_NUMBER, LINE_NUMBER);

static struct proc_dir_entry *stateCtrl_proc = NULL;
static char data_buffer[CTRL_MAX_USER_SIZE];

static char welc_msg_buffer[] = "The proc file is alive!\n";

static ssize_t stateCtrl_read(struct file *file, char __user *user, size_t size, loff_t *off);
static ssize_t stateCtrl_write(struct file *file, const char __user *user, size_t size, loff_t *off);

static const struct proc_ops stateCtrl_proc_fops =
    {
        .proc_read = stateCtrl_read,
        .proc_write = stateCtrl_write,
};

ssize_t stateCtrl_read(struct file *file, char __user *user, size_t size, loff_t *off)
{
    return simple_read_from_buffer(user, size, off, welc_msg_buffer, sizeof(welc_msg_buffer));
}

ssize_t stateCtrl_write(struct file *file, const char __user *user, size_t size, loff_t *off)
{
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


    if (strcmp(data_buffer, "0") == 0)
    {
        gpio_set_value(pin_number, 0);
    }
    else if (strcmp(data_buffer, "1") == 0)
    {
        gpio_set_value(pin_number, 1);
    }
    else 
    {
        printk(KERN_ERR "Invalid value passed!\n");
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

	/* GPIO init */
	if (gpio_request(pin_number, "iMX8"))
	{
		printk("Can not allocate GPIO.\n");
		goto AddError;
	}

	/* Set GPIO direction */
	if (gpio_direction_output(pin_number, 0))
	{
		printk("Can not set GPIO to output!\n");
		goto GpioPinError;
	}

    printk(KERN_INFO "Led state control module loaded.\n");
    return 0;

GpioPinError:
	gpio_free(pin_number);
AddError:    
return -1;
}

static void __exit statectrl_exit(void)
{
    gpio_set_value(pin_number, 0);
	gpio_free(pin_number);
    proc_remove(stateCtrl_proc);
    printk(KERN_INFO "Led state control module unloaded.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro");
MODULE_DESCRIPTION("Change the led state");
MODULE_VERSION("1.0");

module_init(statectrl_init);
module_exit(statectrl_exit);
