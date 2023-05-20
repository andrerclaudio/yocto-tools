#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro");
MODULE_DESCRIPTION("A simple gpio driver for setting a LED and reading a button");

#define CHIP_NUMBER 5
#define LINE_NUMBER 9
#define IMX_GPIO_NR(port, index)    ((((port)-1)*32)+((index)&31))
#define DRIVER_NAME "my_gpio_driver"
#define DRIVER_CLASS "MyModuleClass"

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

/* Global variables */
static int pin_number = IMX_GPIO_NR(CHIP_NUMBER, LINE_NUMBER);

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
	char tmp[3] = " \n";

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(tmp));

	/* Read value of button */
	printk("Value of button: %d\n", gpio_get_value(pin_number));
	tmp[0] = gpio_get_value(pin_number) + '0';

	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;
}

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
	char value;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(value));

	/* Copy data to user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	/* Setting the LED */
	switch (value)
	{
	case '0':
		gpio_set_value(pin_number, 0);
		break;
	case '1':
		gpio_set_value(pin_number, 1);
		break;
	default:
		printk("Invalid value!\n");
		break;
	}

	/* Calculate data */
	delta = to_copy - not_copied;

	return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance)
{
	printk("dev_nr - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance)
{
	printk("dev_nr - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void)
{
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0)
	{
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create device class */
	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL)
	{
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */
	if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL)
	{
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if (cdev_add(&my_device, my_device_nr, 1) == -1)
	{
		printk("Registering of device to kernel failed!\n");
		goto AddError;
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

	return 0;

GpioPinError:
	gpio_free(pin_number);
AddError:
	device_destroy(my_class, my_device_nr);

	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void)
{
	gpio_set_value(pin_number, 0);
	gpio_free(pin_number);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);