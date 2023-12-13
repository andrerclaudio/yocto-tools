#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/input.h>

#define     LED_BRIGHTNESS  "/sys/class/leds/Led/brightness"  
#define     INPUT_EVENT     "/dev/input/event1"
#define     PUSHED          0
#define     RELEASED        1

#define     LED_ON          1
#define     LED_OFF         0

void sleep_mseconds(unsigned int value) {

    struct timespec req;
    req.tv_sec = value / 1000;
    req.tv_nsec = (value % 1000) * 1000000; // Convert milliseconds to nanoseconds

    nanosleep(&req, NULL);
}

void change_led_state (bool state) {

    int tmp;
    char char_state = state ? '1' : '0';

    /* Change LED state */
    tmp = open(LED_BRIGHTNESS, O_WRONLY);
    if (tmp < 0) {
        printf("\nOpen " LED_BRIGHTNESS " failed!\n");
        exit(1);
    }
    if (write(tmp, &char_state, 1) != 1) {
        printf("Error turning default state on LED.");
        exit(1);
    }
    close(tmp);
}

int main()
{
    struct input_event ev;
    int tmp;
    int key_code;
    int size = sizeof(ev);
    bool last_state = LED_OFF;

    unsigned int delay = 10; // Sleep for 10 milliseconds

    printf("\n*** Starting the application ***\n");

    /* Configure LED */
    tmp = open(LED_BRIGHTNESS, O_WRONLY);
    if (tmp < 0) {
        printf("\nOpen " LED_BRIGHTNESS " failed!\n");
        return 1;
    }
    if (write(tmp, "0", 1) != 1) {
        printf("Error turning default state on LED.");
        return 1;
    }
    close(tmp);    
    printf("Configured LED for use\n");

  
    /* Read event1 */
    tmp = open(INPUT_EVENT, O_RDONLY);
    if (tmp < 0) {
        printf("\nOpen " INPUT_EVENT " failed!\n");
        return 1;
    }

    printf("Success to open " INPUT_EVENT "!\n");
    fflush(stdout);

    /* Read and parse event, update global variable */
    while (1) {

        if (read(tmp, &ev, size) < size) {
            printf("\nReading from " INPUT_EVENT " failed!\n");
            return 1;
        }

        if (ev.value == PUSHED && ev.type == 1) {

            key_code = ev.code;

            if (key_code == KEY_LIGHTS_TOGGLE) { 

                printf("User button was pressed!\n");
                fflush(stdout);

                // Toggle the led state
                last_state ^= 1;
                change_led_state(last_state);

            }

            sleep_mseconds(delay);
        }
    }
}