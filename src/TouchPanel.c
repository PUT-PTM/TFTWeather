/*********************************************************************************************************
 *
 * Project			   : TFTWeather
 * File                : TouchPanel.c
 * Version             : V1.0
 * By                  : Artur Bronka
 * 					   : Lukasz Dobek
 *
 *********************************************************************************************************/

#include "TouchPanel.h"
#include "LCD.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/*
 *
 * Variables section.
 *
 */

Matrix matrix ;
Coordinate  display ;
Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {{30, 45},{290, 45},{160, 210}};

#define THRESHOLD 2


// Initialization of original TouchPanel driver. Configuration of SPI.
static void ADS7843_SPI_Init(void) 
{ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_Cmd(SPI1, DISABLE);

	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
} 

// Touch panel initialization.
void TP_Init(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	TP_CS(1);
	ADS7843_SPI_Init();
} 

static void DelayUS(vu32 cnt)
{
	uint16_t i;
	for(i = 0;i<cnt;i++)
	{
		uint8_t us = 12;
		while (us--)
		{
			;
		}
	}
}


static void WR_CMD (uint8_t cmd)  
{ 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1,cmd);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);
} 

static int RD_AD(void)  
{ 
	unsigned short buf,temp;
	/* Wait for SPI1 Tx buffer empty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/* Send SPI1 data */
	SPI_I2S_SendData(SPI1,0x0000);
	/* Wait for SPI3 data reception */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	/* Read SPI1 received data */
	temp=SPI_I2S_ReceiveData(SPI1);
	buf=temp<<8;
	DelayUS(1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	/* Send SPI1 data */
	SPI_I2S_SendData(SPI1,0x0000);
	/* Wait for SPI1 data reception */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	/* Read SPI1 received data */
	temp=SPI_I2S_ReceiveData(SPI1);
	buf |= temp;
	buf>>=3;
	buf&=0xfff;
	return buf;
} 

int Read_X(void)  
{  
	int i;
	TP_CS(0);
	DelayUS(1);
	WR_CMD(CHX);
	DelayUS(1);
	i=RD_AD();
	TP_CS(1);
	return i;
} 

int Read_Y(void)  
{  
	int i;
	TP_CS(0);
	DelayUS(1);
	WR_CMD(CHY);
	DelayUS(1);
	i=RD_AD();
	TP_CS(1);
	return i;
} 

void TP_GetAdXY(int *x,int *y)  
{ 
	int adx,ady;
	adx=Read_X();
	DelayUS(1);
	ady=Read_Y();
	*x=adx;
	*y=ady;
} 


void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
	LCD_SetPoint(Xpos,Ypos,Blue);     /* Center point */
	LCD_SetPoint(Xpos+1,Ypos,Blue);
	LCD_SetPoint(Xpos,Ypos+1,Blue);
	LCD_SetPoint(Xpos+1,Ypos+1,Blue);
}	

// Function that draws cross. Used in calibration of touch panel.
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
	LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,Red);
	LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,Red);
	LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,Red);
	LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,Red);

	LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,White);
	LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,White);

	LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,White);
	LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,White);

	LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,White);
	LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,White);

	LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,White);
	LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,White);
}	

// Reads coordinates on touch.
Coordinate *Read_Ads7846(void)
{
	static Coordinate  screen;
	int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
	uint8_t count=0;
	int buffer[2][9]={{0},{0}};

	do
	{
		TP_GetAdXY(TP_X,TP_Y);
		buffer[0][count]=TP_X[0];
		buffer[1][count]=TP_Y[0];
		count++;
	}
	while(!TP_INT_IN&& count<9);
	if(count==9)
	{

		temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
		temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
		temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];

		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);

		if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

		if(m0<m1)
		{
			if(m2<m0)
				screen.x=(temp[0]+temp[2])/2;
			else
				screen.x=(temp[0]+temp[1])/2;
		}
		else if(m2<m1)
			screen.x=(temp[0]+temp[2])/2;
		else
			screen.x=(temp[1]+temp[2])/2;

		/* Average Y  */
		temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
		temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
		temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];
		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);
		if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

		if(m0<m1)
		{
			if(m2<m0)
				screen.y=(temp[0]+temp[2])/2;
			else
				screen.y=(temp[0]+temp[1])/2;
		}
		else if(m2<m1)
			screen.y=(temp[0]+temp[2])/2;
		else
			screen.y=(temp[1]+temp[2])/2;

		return &screen;
	}
	return 0;
}

FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
		Coordinate * screenPtr,
		Matrix * matrixPtr)
{

	FunctionalState retTHRESHOLD = ENABLE ;
	/* K��(X0��X2) (Y1��Y2)��(X1��X2) (Y0��Y2) */
	matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
			((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	if( matrixPtr->Divider == 0 )
	{
		retTHRESHOLD = DISABLE;
	}
	else
	{
		matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
				((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
		matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
				((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
		matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
				(screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
				(screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
		matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
				((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
		matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
				((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
		matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
				(screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
				(screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
	}
	return( retTHRESHOLD ) ;
}

FunctionalState getDisplayPoint(Coordinate * displayPtr,
		Coordinate * screenPtr,
		Matrix * matrixPtr )
{
	FunctionalState retTHRESHOLD =ENABLE ;

	if( matrixPtr->Divider != 0 )
	{
		displayPtr->x = ( (matrixPtr->An * screenPtr->x) +
				(matrixPtr->Bn * screenPtr->y) +
				matrixPtr->Cn
		) / matrixPtr->Divider ;
		displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) +
				(matrixPtr->En * screenPtr->y) +
				matrixPtr->Fn
		) / matrixPtr->Divider ;
	}
	else
	{
		retTHRESHOLD = DISABLE;
	}
	return(retTHRESHOLD);
} 

// Touch panel calibration.
void TouchPanel_Calibrate(void)
{
	uint8_t i;
	Coordinate * Ptr;

	for(i=0;i<3;i++)
	{
		LCD_Clear(Black);
		GUI_Text(44,10,"Touch crosshair to calibrate",Red,Black);
		delay_ms(500);
		DrawCross(DisplaySample[i].x,DisplaySample[i].y);
		do
		{
			Ptr=Read_Ads7846();
		}
		while( Ptr == (void*)0 );
		ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
	}
	setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
	LCD_Clear(Black);
} 

// Returns data needed to read user's touch.
static uint16_t read_IRQ(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
}

uint16_t XPT2046_Press(void)
{
	return read_IRQ();
}
// ---------------------------------------------------------------------------

