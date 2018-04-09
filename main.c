/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "dht11.h"

u8 Rh,RhDec,Temp,TempDec,ChkSum;

int main (void)
{

    SystemInit();//168mhz
    DHT11initTIM2();

    while(1){
       	DHT11Read(&Rh,&RhDec,&Temp,&TempDec,&ChkSum);
		DHT11_delay_us(50000);
    }
  }
