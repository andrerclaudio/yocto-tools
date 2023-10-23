#ifndef __COMMONS_H__
#define __COMMONS_H__


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <string.h>


/* Public types --------------------------------------------------------------*/
typedef enum ReturnCodes {

    SUCCESS =   ( 0 ),
	FAIL =      ( INT8_MIN )    
    

} ReturnCodes_t;


#endif