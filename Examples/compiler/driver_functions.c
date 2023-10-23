#include "commons.h"
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

static int file_i2c = 0;

ReturnCodes_t _i2c_init(int i2c, int dev_addr)
{
	if (file_i2c == 0)
	{
		char filename[32];
  		sprintf(filename, "/dev/i2c-%d", i2c); // I2C bus number passed
		file_i2c = open(filename, O_RDWR);
		if (file_i2c < 0)
		{
			file_i2c = 0;
			return FAIL;
		}
		if (ioctl(file_i2c, I2C_SLAVE, dev_addr) < 0) // set slave address
		{
			close(file_i2c);
			file_i2c = 0;
			return FAIL;
		}
		return SUCCESS;
	}
	
	// assume done init already
	return SUCCESS;
}

ReturnCodes_t _i2c_close()
{
	if (file_i2c != 0)
	{
		close(file_i2c);
		file_i2c = 0;
		return SUCCESS;
	}
	
	return 1;
}

ReturnCodes_t _i2c_write(uint8_t* ptr, int16_t len)
{
	if (!file_i2c) return FAIL;
	if (ptr == NULL) return FAIL;
	if (!len) return FAIL;
				
	write(file_i2c, ptr, len);
	
	return SUCCESS;
}

ReturnCodes_t _i2c_read(uint8_t *ptr, int16_t len)
{
	if (!file_i2c) return FAIL;
	if (ptr == NULL) return FAIL;
	if (!len) return FAIL;
				
	read(file_i2c, ptr, len);

	return SUCCESS;
}
