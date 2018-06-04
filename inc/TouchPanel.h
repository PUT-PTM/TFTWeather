/*********************************************************************************************************
*
* File                : TouchPanel.h
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
 * File                : TouchPanel.h
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

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

#define	CHX 	0x90
#define	CHY 	0xd0

#define TP_CS(x)	x ? GPIO_SetBits(GPIOA,GPIO_Pin_2): GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define TP_INT_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

Coordinate *Read_Ads7846(void);
uint16_t XPT2046_Press(void);

void TP_Init(void);	
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);

static uint16_t read_IRQ(void);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

#endif
