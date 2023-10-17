#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define __HELP                      (0)
#define __FAIL                      (-1)

#define __ARGC_NULL                 (-1)

#define OLED_TYPE_BUFFER_SIZE       (10)

void print_help(void) {

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
    
    int cmd_opt = 0;

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
                return __HELP;

            case 'c':
                if (optarg)
                {
                    // clear_line = atoi(optarg);
                }
                else
                {
                    // clear_all = 1;
                }
                break;

            case 'd':
                // display = atoi(optarg);
                break;

            case 'f':
                // font = atoi(optarg);
                break;

            case 'h':
                print_help();
                return __HELP;

            case 'i':
                // inverted = atoi(optarg);
                break;

            case 'l':
                // strncpy(line, optarg, sizeof(line));
                break;

            case 'm':
                // strncpy(msg, optarg, sizeof(msg));
                break;

            case 'n':
                // i2c_node_address = (uint8_t)atoi(optarg);
                break;

            case 'r':
                // orientation = atoi(optarg);
                // if (orientation != 0 && orientation != 180)
                // {
                //     printf("orientation value must be 0 or 180\n");
                //     return 1;
                // }
                break;

            case 'x':
                // x = atoi(optarg);
                break;

            case 'y':
                // y = atoi(optarg);
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
}
