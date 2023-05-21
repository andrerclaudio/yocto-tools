/* Library includes */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>

/* Local definitions */
#define DRIVER_NAME "kdriverled"
#define LED_OFF 0
#define LED_ON 1

/* Macros for GPIO pin number calculation */
#define CHIP_NUMBER 5
#define LINE_NUMBER 9
#define IMX_GPIO_NUMBER(port, index) ((((port)-1) * 32) + ((index)&31))

/* Local struct definitions */
struct kdriverled_data_st
{
	struct gpio_desc *desc;
	struct led_classdev led_cdev;
};

static struct kdriverled_data_st *kdriverled_data;

// Global variable for GPIO pin number
static int pin_number = IMX_GPIO_NUMBER(CHIP_NUMBER, LINE_NUMBER);

// Function to set the LED status
static void kdriverled_setled(unsigned int status)
{
	if (status == LED_ON)
		gpiod_set_value(kdriverled_data->desc, LED_ON);
	else
		gpiod_set_value(kdriverled_data->desc, LED_OFF);
}

// Function to handle LED state change
static void kdriverled_change_state(struct led_classdev *led_cdev, enum led_brightness brightness)
{
	if (brightness)
		kdriverled_setled(LED_ON);
	else
		kdriverled_setled(LED_OFF);
}

// Module initialization function
static int __init kdriverled_init(void)
{
	kdriverled_data = kzalloc(sizeof(*kdriverled_data), GFP_KERNEL);
	if (!kdriverled_data)
		return -ENOMEM;

	if (gpio_request(pin_number, DRIVER_NAME))
	{
		pr_err("%s: Error registering GPIO.\n", DRIVER_NAME);
		goto ret_err_gpio_request;
	}

	kdriverled_data->desc = gpio_to_desc(pin_number);

	kdriverled_data->led_cdev.name = "ipe:red:user";
	kdriverled_data->led_cdev.brightness_set = kdriverled_change_state;

	if (led_classdev_register(NULL, &kdriverled_data->led_cdev))
	{
		pr_err("%s: Error registering the device.\n", DRIVER_NAME);
		goto ret_err_led_classdev_register;
	}

	gpiod_direction_output(kdriverled_data->desc, 0);

	pr_info("%s: Initialized!\n", DRIVER_NAME);
	return 0;

ret_err_led_classdev_register:
	gpio_free(pin_number);
ret_err_gpio_request:
	kfree(kdriverled_data);
	return -1;
}

// Module exit function
static void __exit kdriverled_exit(void)
{
	led_classdev_unregister(&kdriverled_data->led_cdev);
	gpio_free(pin_number);
	kfree(kdriverled_data);
	pr_info("%s: Exiting!\n", DRIVER_NAME);
}

module_init(kdriverled_init);
module_exit(kdriverled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro Claudio");
MODULE_VERSION("1.0");