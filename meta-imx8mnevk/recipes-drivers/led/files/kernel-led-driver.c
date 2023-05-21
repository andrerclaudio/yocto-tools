/* Library includes */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>

/* Local definitions */
#define LED_OFF 0
#define LED_ON 1

/* Local struct definitions */
struct kdriverled_data_st
{
    struct gpio_desc *desc;
    struct led_classdev led_cdev;
};

static struct kdriverled_data_st *kdriverled_data;

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

static int kdriverled_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    struct device_node *child = NULL;
    int result, gpio;

    child = of_get_next_child(np, NULL);
    kdriverled_data = devm_kzalloc(&pdev->dev, sizeof(*kdriverled_data), GFP_KERNEL);

    if (!kdriverled_data)
        return -ENOMEM;

    gpio = of_get_gpio(child, 0);

    result = devm_gpio_request(&pdev->dev, gpio, pdev->name);
    if (result)
    {
		dev_err(&pdev->dev, "Failed to request GPIO: %d\n", result);
        return result;
    }

    kdriverled_data->desc = gpio_to_desc(gpio);
    kdriverled_data->led_cdev.name = of_get_property(child, "label", NULL);
    kdriverled_data->led_cdev.brightness_set = kdriverled_change_state;

    gpiod_direction_output(kdriverled_data->desc, 0);

    dev_info(&pdev->dev, "Initialized.\n");
    return 0;
}

static int kdriverled_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Exiting.\n");
    return 0;
}

static const struct of_device_id of_kdriverled_match[] = {

    {.compatible = "agnes,kdriverled"},
    {},
};

static struct platform_driver kdriverled_driver = {

    .driver = {
        .name = "kdriverled",
        .owner = THIS_MODULE,
        .of_match_table = of_kdriverled_match,
    },

    .probe = kdriverled_probe,
    .remove = kdriverled_remove,
};

module_platform_driver(kdriverled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andre Ribeiro Claudio");
MODULE_VERSION("1.0");