#include <getopt.h>
#include "commons.h"
#include "ssd1306.h"

// Status return
#define __NO_OP                     (0)
#define __FAIL                      (1)

// Arguments coutner feedback
#define __ARGC_NULL                 (-1)

#define LINE_BUFFER_MAX_SIZE        (32)
#define MSG_BUFFER_MAX_SIZE         (128)

typedef struct display_settings {
    
    int clear;
    int status;
    int font;
    int inverted;
    int orientation;
    int x;
    int y;
    char line[LINE_BUFFER_MAX_SIZE];
    char msg[MSG_BUFFER_MAX_SIZE];
    uint8_t i2c_node_address;

} display_settings_t;


static void print_help(void) {

    printf("\n");
    printf("---- SSD1306 Driver ---- \n\n");
    printf("\t-h\tHelp message\n");
    printf("\t-c\tClear [X]line number or [0]all)\t\t\t\t[default: all]\n");
    printf("\t-d\tDisplay [0]OFF or [1]ON\t\t\t\t\t[default: ON]\n");
    printf("\t-f\t[0]Small font 5x7 or [1]Normal font 8x8\t\t\t[default: normal]\n");
    printf("\t-i\tDisplay orientation [0]Normal or [1]invert\t\t[default: normal]\n");
    printf("\t-l\tPrint your line to display\n");
    printf("\t-m\tPrint your strings to display\n");
    printf("\t-n\tI2C device node address (0,1,2...)\t\t\t[default: 0]\n");
    printf("\t-r\tDisplay rotation [0]Normal or [180]Rotate\t\t[default: normal]\n");
    printf("\t-x\tX position\n");
    printf("\t-y\tY position\n\n\n");
}

int main (int argc, char **argv) {
    
    // Arg variable controller
    int cmd_opt = 0;
    // Display settings 
    display_settings_t display;

    while(cmd_opt != __ARGC_NULL) {

        cmd_opt = getopt(argc, argv, "c::d:f:hi:l:m:n:r:x:y:");

        if (argc < 2) {

            printf("Arguments must be passed. Enter with 'main -h' to see to get help!\n");
            return __FAIL;
        }

        /* Lets parse */
        switch (cmd_opt) {

            default:

                print_help();
                return __FAIL;

            case 'c':

                if (optarg) display.clear = atoi(optarg);                
                else display.clear = 1;
                break;

            case 'd':
                display.status = atoi(optarg);
                break;

            case 'f':
                display.font = atoi(optarg);
                break;

            case 'h':
                print_help();
                return __NO_OP;

            case 'i':
                display.inverted = atoi(optarg);
                break;

            case 'l':
                strncpy(display.line, optarg, LINE_BUFFER_MAX_SIZE);
                break;

            case 'm':
                strncpy(display.msg, optarg, MSG_BUFFER_MAX_SIZE);
                break;

            case 'n':
                display.i2c_node_address = (uint8_t)atoi(optarg);
                break;

            case 'r':
                display.orientation = atoi(optarg);
                if (display.orientation != 0 && display.orientation != 180)
                {
                    printf("Orientation value must be 0 or 180\n");
                    return __FAIL;
                }
                break;

            case 'x':
                display.x = atoi(optarg);
                break;

            case 'y':
                display.y = atoi(optarg);
                break;

            case __ARGC_NULL:
                // Do nothing
                break;

            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                if (optopt == 'I')
                {
                    printf("prams -%c missing oled type (128x64/128x32/64x48)\n", optopt);
                    return __FAIL;
                }
                else if (optopt == 'd' || optopt == 'f' || optopt == 'i')
                {
                    printf("prams -%c missing 0 or 1 fields\n", optopt);
                    return __FAIL;
                }
                else if (optopt == 'l' || optopt == 'm')
                {
                    printf("prams -%c missing string\n", optopt);
                    return __FAIL;
                }
                else if (optopt == 'n')
                {
                    printf("prams -%c missing 0,1,2... I2C device node number\n", optopt);
                    return __FAIL;
                }
                else if (optopt == 'r')
                {
                    printf("prams -%c missing 0 or 180 fields\n", optopt);
                    return __FAIL;
                }
                else if (optopt == 'x' || optopt == 'y')
                {
                    printf("prams -%c missing coordinate values\n", optopt);
                    return __FAIL;
                }
                break;
        }
    }

    uint8_t rc = 0;
    
    // open the I2C device node
    rc = ssd1306_init(display.i2c_node_address);

    if (rc != 0)
    {
        printf("no oled attached to /dev/i2c-%d\n", display.i2c_node_address);
        return 1;
    }

}
