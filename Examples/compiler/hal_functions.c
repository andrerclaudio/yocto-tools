#include "commons.h"
#include "driver_functions.h"
#include "hal_functions.h"
#include "font.h"


const char settings_to_save[] = "/tmp/.settings_to_save";

static char loaded_a = 0;
static char loaded_b = 0;

ReturnCodes_t save_settings(char param_a, char param_b)
{
    FILE* fp;
    
    fp = fopen(settings_to_save, "w");
    
    if (fp == NULL)
    {
        // file create failed
        return FAIL;
    }

    _i2c_init(0, 0x3C);
    _i2c_close();
    
    fprintf(fp, "%hhux%hhu", param_a, param_b);
    fclose(fp);
    
    return SUCCESS;
}

ReturnCodes_t load_settings()
{
    FILE* fp;
    
    fp = fopen(settings_to_save, "r");
    
    if (fp == NULL)
    {
        // file not exists
        return FAIL;
    }
    
    // file exists
    fscanf(fp, "%hhux%hhu", &loaded_a, &loaded_b);
    fclose(fp);
    
    return SUCCESS;
}
