#ifndef __DRIVER_FUNCTIONS_H__
#define __DRIVER_FUNCTIONS_H__

#include "commons.h"

ReturnCodes_t _i2c_init(int i2c, int dev_addr);
ReturnCodes_t _i2c_close();
ReturnCodes_t _i2c_write(uint8_t* ptr, int16_t len);
ReturnCodes_t _i2c_read(uint8_t *ptr, int16_t len);

#endif
