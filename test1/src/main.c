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
#include "LCD.h"
#include "dht11.h"
#define MAX_STRLEN 12
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string

uint8_t *Rh,*RhDec,*Temp,*TempDec,*ChkSum;

int main(void)
{

	DHT11initTIM4();
	char str[20];
	LCD_Initializtion();
	LCD_Clear(White);
	GUI_Text(40,50,"TFTWHEATHER",Black,White);

	delay_ms(10000);
	while (1)
	{
		DHT11Read(&Rh,&RhDec,&Temp,&TempDec,&ChkSum);
		LCD_Clear(White);
			GUI_Text(20,20,"Temperature: ",Black,White);
			GUI_Text(20,40,Temp,Black,White);

			GUI_Text(20,80,"Humidty: ",Black,White);
			//GUI_Text(20,100,Rh,Black,White);



		DHT11_delay_us(5);

	}
}

