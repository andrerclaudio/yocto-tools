#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CHIP_NAME "/dev/gpiochip4"
#define INPUT_LINE_NUMBER 8
#define OUTPUT_LINE_NUMBER 9
#define INPUT_CONSUMER_NAME "Button"
#define OUTPUT_CONSUMER_NAME "Led"
#define LED_ON 1
#define LED_OFF 0
#define LED_INITIAL_STATE LED_ON
#define TOGGLE_QTY 10

void sleep_mseconds(unsigned int value)
{

    struct timespec req;
    req.tv_sec = value / 1000;
    req.tv_nsec = (value % 1000) * 1000000; // Convert milliseconds to nanoseconds

    nanosleep(&req, NULL);
}

int main(int argc, char **argv)
{
    unsigned int delay = 100; // Sleep for 100 milliseconds
    bool last_led_sate = LED_INITIAL_STATE;
    struct gpiod_chip *chip;
    struct gpiod_line *input_line;
    struct gpiod_line *output_line;
    int req, value;

    // Open the GPIO chip
    chip = gpiod_chip_open(CHIP_NAME);
    if (!chip)
    {
        fprintf(stderr, "Unable to open %s: %s\n", CHIP_NAME, strerror(errno));
        return EXIT_FAILURE;
    }

    // Get the GPIO line "INPUT"
    input_line = gpiod_chip_get_line(chip, INPUT_LINE_NUMBER);
    if (!input_line)
    {
        fprintf(stderr, "Unable to get chip line %d: %s\n", INPUT_LINE_NUMBER, strerror(errno));
        goto CHIP_CLOSE;
    }
    // Get the GPIO line "OUTPUT"
    output_line = gpiod_chip_get_line(chip, OUTPUT_LINE_NUMBER);
    if (!output_line)
    {
        fprintf(stderr, "Unable to get chip line %d: %s\n", OUTPUT_LINE_NUMBER, strerror(errno));
        goto CHIP_CLOSE;
    }

    // Request the GPIO line as an input with a specific consumer name
    req = gpiod_line_request_input(input_line, INPUT_CONSUMER_NAME);
    if (req)
    {
        fprintf(stderr, "Unable to request line as input [%s]: %s\n", INPUT_CONSUMER_NAME, strerror(errno));
        goto CHIP_CLOSE;
    }

    // Request the GPIO line as an output with a specific consumer name
    req = gpiod_line_request_output(output_line, OUTPUT_CONSUMER_NAME, LED_INITIAL_STATE);
    if (req)
    {
        fprintf(stderr, "Unable to request line as output [%s]: %s\n", OUTPUT_CONSUMER_NAME, strerror(errno));
        goto CHIP_CLOSE;
    }

    for (int i = 0; i < TOGGLE_QTY; i++)
    {
        last_led_sate ^= 1;
        req = gpiod_line_set_value(output_line, last_led_sate);
        if (req)
        {
            fprintf(stderr, "Set line output failed: %s\n", strerror(errno));
            goto CHIP_CLOSE;
        }
        printf("GPIO \toutput [%d] value is: %d\n", OUTPUT_LINE_NUMBER, last_led_sate);
        sleep_mseconds(delay);
    }

    // Read the value of the GPIO line
    value = gpiod_line_get_value(input_line);
    printf("GPIO \t input [%d] value is: %d\n", INPUT_LINE_NUMBER, value);

    // Close the chip descriptor
    gpiod_line_release(input_line);
    gpiod_line_release(output_line);
    gpiod_chip_close(chip);
    return EXIT_SUCCESS;

CHIP_CLOSE:
    gpiod_chip_close(chip);
    return EXIT_FAILURE;
}