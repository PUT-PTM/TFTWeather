/*********************************************************************************************************
*
* File                : LCD.h
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  :
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/*********************************************************************************************************
 *
 * Project			   : TFTWeather
 * File                : LCD.h
 * Version             : V1.0
 * By                  : Artur Bronka
 * 					   : Lukasz Dobek
 *
 *********************************************************************************************************/

/*
 * Pin Configuration:
 * 5V			VDD
 * GND			GND
 * D0-D15		PB0-PB15
 * LCD_CS		PC6
 * LCD_RS		PC7
 * LCD_WR		PC1
 * LCD_RD		PC2
 * LCD_RESET	PC0
 * NC			Not Connected
 * BL_VDD5V		3.3V
 * BL_GND		GND
 * BL_PWM		PA3
 * LCDTP_IRQ	PA4
 * LCDTP_CS		PA2
 * LCDTP_CLK	PA5
 * LCDTP_DIN	PA7
 * LCDTP_DOUT	PA6
 * 3.3V			3.3V
 * GND			GND
 */

#ifndef __GLCD_H
#define __GLCD_H


#include <math.h>
#include "AsciiLib.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define USE_16BIT_PMP

// Display orientation - it's dangerous to mess around with.
#define DISP_ORIENTATION					90

// It depends whether display orientation is set to 90/270 or 0/180 - it makes LCD set to either horizontal or vertical display.
#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320

#endif

// ---------------------------------------------------------------------------

#define Set_Cs        GPIOC->BSRRL = GPIO_Pin_6
#define Clr_Cs        GPIOC->BSRRH = GPIO_Pin_6

#define Set_Rs        GPIOC->BSRRL = GPIO_Pin_7
#define Clr_Rs        GPIOC->BSRRH = GPIO_Pin_7

#define Set_nWr       GPIOC->BSRRL = GPIO_Pin_1
#define Clr_nWr       GPIOC->BSRRH = GPIO_Pin_1

#define Set_nRd       GPIOC->BSRRL = GPIO_Pin_2
#define Clr_nRd       GPIOC->BSRRH = GPIO_Pin_2

/* LCD color */
// You can define your own colors here - it uses RGB565 instead of RGB888 so be careful.
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

// Function that easily converts normal RGB (ex. 255, 0, 255 in standard RGB given to that function returns beautiful pink color.
#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))
// Example of usage (will return Orange color):
#define Orange		RGB565CONVERT(255,191,0)


void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);

void PutChar(uint16_t Xpos,uint16_t Ypos,uint8_t c,uint16_t charColor,uint16_t bkColor);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);

void LCD_WriteIndex(uint16_t index);
void LCD_WriteData(uint16_t data);
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);

void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );
void delay_ms(uint16_t ms);

uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
uint16_t LCD_ReadData(void);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);

#endif

