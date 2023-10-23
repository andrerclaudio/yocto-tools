#include "commons.h"
#include "hal_functions.h"


int main (int argc, char **argv) {

    char value_a = 'a';
    char value_b = 'b';
    
    while (true)
    {
        save_settings(value_a, value_b);
        load_settings();
    }

    return 0;
}
