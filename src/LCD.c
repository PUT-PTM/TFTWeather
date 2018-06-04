/*********************************************************************************************************
 *
 * Project			   : TFTWeather
 * File                : LCD.c
 * Version             : V1.0
 * By                  : Artur Bronka
 * 					   : Lukasz Dobek
 *
 *********************************************************************************************************/


#include "LCD.h"
#include "AsciiLib.h"

void LCD_Delay(unsigned char i)
{
	while(i--);
}

// Configuration of PINs on STM32F4 Discovery board.
// Scheme of all PINs in LCD.h file.
static void LCD_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC , ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
// ---------------------------------------------------------------------------

// Section of functions used to write and read raw data.
__inline void LCD_WriteIndex(uint16_t index)
{
	Clr_Rs;			//RS=0
	Set_nRd;		//RD=0
	LCD_Delay(0); 	//Delay
	GPIOB->ODR = index;

	LCD_Delay(0);	//Delay
	Clr_nWr;		//WR=0
	Set_nWr;		//WR=1
}

__inline void LCD_WriteData(uint16_t data)
{
	Set_Rs;         //RS=1
	LCD_Delay(0);	//Delay
	GPIOB->ODR = data;

	LCD_Delay(0);	//Delay
	Clr_nWr;	 	//WR=0
	Set_nWr;		//WR=1
}

__inline uint16_t LCD_ReadData(void)
{
	uint16_t value;
	Set_Rs;

	Set_nWr;
	Clr_nRd;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	value = GPIOB->IDR;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    Set_nRd;
    return value;
}


__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
	LCD_WriteData(LCD_RegValue);
	Set_Cs;
}


__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	uint16_t LCD_RAM;
	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
	LCD_RAM = LCD_ReadData();
	Set_Cs;
	return LCD_RAM;
}
// ---------------------------------------------------------------------------

void delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for( i = 0; i < ms; i++ )
	{
		for( j = 0; j < 1141; j++ );
	}
}

void LCD_Initializtion(void)
{
	uint16_t DeviceCode;	// Be aware as your LCD might have another DeviceCode.
	LCD_Configuration();
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);

	delay_ms(100);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_3);

	DeviceCode = LCD_ReadReg(0x0000);

	if( DeviceCode == 33576 || DeviceCode == 0x9328 )	// You might need to change this if statement accordingly to your device.
	{
	LCD_WriteReg(0x00e7,0x0010);
	LCD_WriteReg(0x0000,0x0001);
	LCD_WriteReg(0x0001,(0<<10)|(1<<8));
	LCD_WriteReg(0x0002,0x0700);

	// Display orientation configuration.
	#if (DISP_ORIENTATION == 0)
	LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3));
	#elif (DISP_ORIENTATION == 90)
	LCD_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4)|(1<<3));
	#elif (DISP_ORIENTATION == 180)
	LCD_WriteReg(0x0003,(1<<12)|(0<<5)|(0<<4)|(0<<3));
	#elif (DISP_ORIENTATION == 270)
	LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(0<<4)|(1<<3));
	#endif

	LCD_WriteReg(0x0004,0x0000);
	LCD_WriteReg(0x0008,0x0207);
	LCD_WriteReg(0x0009,0x0000);
	LCD_WriteReg(0x000a,0x0000);
	LCD_WriteReg(0x000c,0x0001);
	LCD_WriteReg(0x000d,0x0000);
	LCD_WriteReg(0x000f,0x0000);
	LCD_WriteReg(0x0010,0x0000);
	LCD_WriteReg(0x0011,0x0007);
	LCD_WriteReg(0x0012,0x0000);
	LCD_WriteReg(0x0013,0x0000);
	delay_ms(50);


	LCD_WriteReg(0x0010,0x1590);
	LCD_WriteReg(0x0011,0x0227);
	delay_ms(50);


	LCD_WriteReg(0x0012,0x009c);
	delay_ms(50);


	LCD_WriteReg(0x0013,0x1900);
	LCD_WriteReg(0x0029,0x0023);
	LCD_WriteReg(0x002b,0x000e);
	delay_ms(50);

	delay_ms(50);
	LCD_WriteReg(0x0030,0x0007);
	LCD_WriteReg(0x0031,0x0707);
	LCD_WriteReg(0x0032,0x0006);
	LCD_WriteReg(0x0035,0x0704);
	LCD_WriteReg(0x0036,0x1f04);
	LCD_WriteReg(0x0037,0x0004);
	LCD_WriteReg(0x0038,0x0000);
	LCD_WriteReg(0x0039,0x0706);
	LCD_WriteReg(0x003c,0x0701);
	LCD_WriteReg(0x003d,0x000f);
	delay_ms(50);

	LCD_WriteReg(0x0050,0x0000);
	LCD_WriteReg(0x0051,0x00ef);
	LCD_WriteReg(0x0052,0x0000);
	LCD_WriteReg(0x0053,0x013f);
	LCD_WriteReg(0x0060,0xa700);
	LCD_WriteReg(0x0061,0x0001);
	LCD_WriteReg(0x006a,0x0000);
	LCD_WriteReg(0x0080,0x0000);
	LCD_WriteReg(0x0081,0x0000);
	LCD_WriteReg(0x0082,0x0000);
	LCD_WriteReg(0x0083,0x0000);
	LCD_WriteReg(0x0084,0x0000);
	LCD_WriteReg(0x0085,0x0000);
	LCD_WriteReg(0x0090,0x0010);
	LCD_WriteReg(0x0092,0x0600);
	LCD_WriteReg(0x0093,0x0003);
	LCD_WriteReg(0x0095,0x0110);
	LCD_WriteReg(0x0097,0x0000);
	LCD_WriteReg(0x0098,0x0000);
	LCD_WriteReg(0x0007,0x0133);
	}
	delay_ms(50);
}

// Sets pointer to said position, given in parameters.
void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
	uint16_t temp;
	#if (DISP_ORIENTATION == 0)
	#elif (DISP_ORIENTATION == 90)
	temp = Xpos;
	Xpos =Ypos;
	Ypos = MAX_X -1 -    temp;
	#elif (DISP_ORIENTATION == 180)
	Xpos = MAX_X -1 -    Xpos;
	Ypos = MAX_Y -1 -    Ypos;
	#elif (DISP_ORIENTATION == 270)
	temp = Ypos;
	Ypos = Xpos;
	Xpos = MAX_Y -1 -    temp;
	#endif
	LCD_WriteReg(0x0020, Xpos);
	LCD_WriteReg(0x0021, Ypos);
}
// ---------------------------------------------------------------------------

// Fills entire screen with color given in parameter.
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;
	LCD_SetCursor(0,0);
	Clr_Cs;
	LCD_WriteIndex(0x0022);

	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}

	Set_Cs;
}
// ---------------------------------------------------------------------------

// Puts a single char on screen.
// Parameters: Xpos - X position
//			   Ypos - Y position
//			   asciiChar - ascii character
//			   charColor - color of an ascii character
// 			   bgColor - background color of an ascii character
// ---------------------------------------------------------------------------
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t asciiChar, uint16_t charColor, uint16_t bgColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,asciiChar);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bgColor );
            }
        }
    }
}
// ---------------------------------------------------------------------------

// Puts an entire string (or char* [look at main function for an example of usage])
// Parameters in this function are really similar to parameters in PutChar function.
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t charColor, uint16_t bgColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        PutChar( Xpos, Ypos, TempChar, charColor, bgColor );
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        }
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}
// ---------------------------------------------------------------------------

// Draws line from one point to another in certain color.
// Paramters: x0,y0 - first point
//		      x1,y1 - second point
//			  color - color in which line will be drawn
// ---------------------------------------------------------------------------
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      y0++;
    }
    while( y1 >= y0 );
    return;
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 );
		return;
  }

  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx;
	 	  }
      else
      {
			  temp += 2 * dy;
			}
    }
    LCD_SetPoint(x0,y0,color);
  }
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);
      y0++;
      if( temp > 0 )
      {
        x0++;
        temp+=2*dy-2*dx;
      }
      else
			{
        temp += 2 * dy;
			}
    }
    LCD_SetPoint(x0,y0,color);
	}
}
// ---------------------------------------------------------------------------


void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

