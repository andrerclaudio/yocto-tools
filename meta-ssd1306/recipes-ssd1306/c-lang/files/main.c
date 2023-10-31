#include <getopt.h>
#include "commons.h"
#include "ssd1306.h"

// Status return
#define __NO_OP                     (0)
#define __FAIL                      (1)

// Arguments coutner feedback
#define __ARGC_NULL                 (-1)

#define LINE_BUFFER_MAX_SIZE        (32)
#define MSG_BUFFER_MAX_SIZE         (32 * 128)

typedef struct display_settings {
    
    int clear_all;
    int clear_line;
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
    printf("\t------------ SSD1306 Driver ------------\n\n");
    printf("\t-h\tHelp message.\n");
    printf("\t-c\tClear (line number or all).\n");
    printf("\t-d\tDisplay OFF [0] or ON [1].\n");
    printf("\t-f\tSmall font 5x7 [0] or Normal font 8x8 [1].\n");
    printf("\t-i\tDisplay Normal [0] or Invert [1].\n");
    printf("\t-r\tDisplay rotation Normal [0] or Rotate [180].\n");
    printf("\t-l\tPrint your line to display.\n");
    printf("\t-m\tPrint your strings to display.\n");
    printf("\t-n\tI2C device node address (0,1,2..., default 2).\n");
    printf("\t-x\tX position.\n");
    printf("\t-y\tY position.\n\n\n");
}

int main (int argc, char **argv) {
    
    // Arg variable controller
    int cmd_opt = 0;
    // Display settings 
    display_settings_t display;

    // Set defaults
    display.clear_all = -1;
    display.clear_line = -1;
    display.status = -1;
    display.font = 0;
    display.inverted = -1;
    display.orientation = -1;
    display.i2c_node_address = 2;

    // Fill the buffers with zeros
    memset(display.line, 0, LINE_BUFFER_MAX_SIZE);
    memset(display.msg, 0, MSG_BUFFER_MAX_SIZE);

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
                if (optarg) display.clear_line = atoi(optarg);
                else display.clear_all = 1;
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
    
    // open the I2C device node and check connection
    rc = ssd1306_init(display.i2c_node_address);

    if (rc != 0)
    {
        printf("No oled attached to /dev/i2c-%d\n", display.i2c_node_address);
        return 1;
    }

    // init oled module if there is not any previous setting saved
    if (ssd1306_oled_load_resolution()) rc += ssd1306_oled_default_config(32, 128);

    // clear display
    if (display.clear_all > -1) rc += ssd1306_oled_clear_screen();
    else if (display.clear_line > -1) rc += ssd1306_oled_clear_line(display.clear_line);
    
    // set rotate orientation
    if (display.orientation > -1) rc += ssd1306_oled_set_rotate(display.orientation);
    
    // set oled inverted
    if (display.inverted > -1) rc += ssd1306_oled_display_flip(display.inverted);
    
    // set display on off
    if (display.status > -1) rc += ssd1306_oled_onoff(display.status);
    
    // set cursor XY
    if (display.x > -1 && display.y > -1) rc += ssd1306_oled_set_XY(display.x, display.y);
    else if (display.x > -1) rc += ssd1306_oled_set_X(display.x);
    else if (display.y > -1) rc += ssd1306_oled_set_Y(display.y);
    
    // print text
    if (display.msg[0] != 0) rc += ssd1306_oled_write_string(display.font, display.msg);
    else if (display.line[0] != 0) rc += ssd1306_oled_write_line(display.font, display.line);
    
    // close the I2C device node
    ssd1306_end();
    
    return rc;
}
