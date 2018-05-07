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

int i;
int main(void)
{
	//Delay and initialize your system
	LCD_Initializtion();
	//LCD	 initialization
	//LCD touch panel initializaion
	//Clear the LCD with filled with red.
	//You can fill functions to calibrate touch screen.
	/* Infinite loop */
	while (1)
	{
		//You can fill functions to show touch coordinate on the LCD.
		LCD_Clear(Magenta);
		delay_ms(1000);
		LCD_Clear(Red);
		delay_ms(1000);
		LCD_Clear(Cyan);
		delay_ms(1000);
		LCD_Clear(Green);
		delay_ms(1000);
		LCD_Clear(Blue);
		delay_ms(1000);
		LCD_Clear(Blue2);
	}
}

