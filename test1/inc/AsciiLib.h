
#ifndef __AsciiLib_H
#define __AsciiLib_H  

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
/* Private define ------------------------------------------------------------*/
//#define  ASCII_8X16_MS_Gothic
#define  ASCII_8X16_System

/* Private function prototypes -----------------------------------------------*/
void GetASCIICode(unsigned char* pBuffer,unsigned char ASCII);

#endif 


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
