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
char str[3];
char str1[3];
int k;
int h;
int main(void)
{

	DHT11initTIM4();
	LCD_Initializtion();
	LCD_Clear(White);
	GUI_Text(40,50,"TFTWHEATHER",Black,White);
	delay_ms(10000);

	while (1)
	{
		DHT11Read(&Rh,&RhDec,&Temp,&TempDec,&ChkSum);
		LCD_Clear(White);
		k = Temp;
		str[0] = (k/10)+'0';
		str[1] = (k%10)+'0';

		GUI_Text(20,20,"Temperature: ",Black,White);
		GUI_Text(20,40, str ,Black,White);

		h=Rh;
		str1[0] = (h/10)+'0';
		str1[1] = (h%10)+'0';

		GUI_Text(20,80,"Humidty: ",Black,White);
		GUI_Text(20,100,str1,Black,White);

		DHT11_delay_us(5);

	}
}

