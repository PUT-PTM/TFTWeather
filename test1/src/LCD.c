#include "LCD.h"
#include "AsciiLib.h"

uint16_t test;
/* Command writing function
*/

void LCD_Delay(unsigned char i)
{
	while(i--);
}

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

__inline void LCD_WriteIndex(uint16_t index)
{
Clr_Rs;
//RS=0
Set_nRd;
//RD=0
LCD_Delay(0);
      //Delay
GPIOB->ODR = index;
 /* index is an command�s
address*/
LCD_Delay(0);
     //Delay
Clr_nWr;
//WR=0
Set_nWr;
//WR=1
}
/* Data writing function
 */
__inline void LCD_WriteData(uint16_t data)
{
Set_Rs;               //RS=1
LCD_Delay(0);
        //Delay


GPIOB->ODR = data;


/*   Data writing address*/
LCD_Delay(0);
      //Delay
Clr_nWr;
//WR=0
Set_nWr;
//WR=1
}
/* Data reading function
 */
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
   //Set PB0-   PB15 as input pin
value = GPIOB->IDR;
        //Reads data
  //  Set PB0-PB15as output pin
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    Set_nRd;
    return value;
}
/****************************************************************
**************
Write data to a specified address, LCD_Reg
 indicates register address
while
LCD_RegValue
indicates register value.
*****************************************************************
*************/
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
Clr_Cs;
LCD_WriteIndex(LCD_Reg);
        //Writing command, LCD_Reg is anaddress to be written in.
LCD_WriteData(LCD_RegValue);
    //Writes data.
Set_Cs;
}
/****************************************************************
**************
Read data from a specified address, LCD_Reg indicates register address.
This function will return a value from the address.
*****************************************************************
*************/
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
uint16_t LCD_RAM;
Clr_Cs;
LCD_WriteIndex(LCD_Reg);
    //Writing command, LCD_Reg is an addressto be read from.
LCD_RAM = LCD_ReadData();  //Reads data
Set_Cs;
return LCD_RAM;
}
//That�s the basic read-and-writefunctions by IO emulation. If youwant use FSMC from STM32 to control the LCD, you can read another demoLCD + TouchPanel(8080 FSMC)3.2inch 320x240 Touch LCD (C)

/****************************************************************
**************
This is LCD initialization function. The initialization value of the
LCD is provided by the factory. So usually you can copy them directly
to initialize LCD. Please refer to ILI9325 datasheets for more details.
*****************************************************************
*************/

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
uint16_t DeviceCode;
LCD_Configuration();                  //LCD Initialization
GPIO_ResetBits(GPIOC, GPIO_Pin_0);
/* LCD reset*/
delay_ms(100);
GPIO_SetBits(GPIOC, GPIO_Pin_0);
GPIO_SetBits(GPIOA, GPIO_Pin_3);
/* Enable bac
k light
*/
DeviceCode = LCD_ReadReg(0x0000);
/* Reads
ID */
if( DeviceCode == 33576 || DeviceCode == 0x9328 )
{
LCD_WriteReg(0x00e7,0x0010);
LCD_WriteReg(0x0000,0x0001);
LCD_WriteReg(0x0001,(0<<10)|(1<<8));
LCD_WriteReg(0x0002,0x0700);
test = DeviceCode;

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
/* Power On sequence */
LCD_WriteReg(0x0010,0x0000);
LCD_WriteReg(0x0011,0x0007);
LCD_WriteReg(0x0012,0x0000);
LCD_WriteReg(0x0013,0x0000);
delay_ms(50);
/* delay 50 ms */

LCD_WriteReg(0x0010,0x1590);
LCD_WriteReg(0x0011,0x0227);
delay_ms(50);
 /* delay 50 ms */
LCD_WriteReg(0x0012,0x009c);
delay_ms(50);
/* delay 50 ms */
LCD_WriteReg(0x0013,0x1900);
LCD_WriteReg(0x0029,0x0023);
LCD_WriteReg(0x002b,0x000e);
delay_ms(50);
/* delay 50 ms */
delay_ms(50);
/* delay 50 ms */
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
/* delay 50 ms */
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
/* display on sequence */
LCD_WriteReg(0x0007,0x0133);
}
delay_ms(50);

}
/****************************************************************
**************
Set window coordinate.
*****************************************************************
************/
static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
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
// Sets the horizontal position X
LCD_WriteReg(0x0021, Ypos);
// Sets the vertical position Y
}

void LCD_Clear(uint16_t Color)
{
uint32_t index=0;
LCD_SetCursor(0,0);
    //Set cursor coordinate X, Y
Clr_Cs;
LCD_WriteIndex(0x0022);//Start to write data into GRAM
for( index = 0; index < MAX_X * MAX_Y; index++ )
{
LCD_WriteData(Color);
}
Set_Cs;
}

void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
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
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}

void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );
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

	/* Bresenham's line algorithm  */
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

static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;

	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;

	rgb =  (b<<11) + (g<<5) + (r<<0);

	return( rgb );
}

void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}


//
//uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
//{
//	uint16_t dummy;
//
//	LCD_SetCursor(Xpos,Ypos);
//	Clr_Cs;
//	LCD_WriteIndex(0x0022);
//
//	switch( LCD_Code )
//	{
//		case ST7781:
//		case LGDP4531:
//		case LGDP4535:
//		case SSD1289:
//		case SSD1298:
//      dummy = LCD_ReadData();
//      dummy = LCD_ReadData();
//      Set_Cs;
// 		  return  dummy;
//    case HX8347A:
//	  case HX8347D:
//    {
//      uint8_t red,green,blue;
//      red = LCD_ReadData()>>3;
//      green = LCD_ReadData()>>3;
//      blue = LCD_ReadData()>>2;
//      dummy = ( green << 11 ) | (blue << 5 ) | red;
//		}
//      Set_Cs;
//      return  dummy;
//    default:	/* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x9919 */
//      dummy = LCD_ReadData();
//      dummy = LCD_ReadData();
//      Set_Cs;
//      return  LCD_BGR2RGB( dummy );
//	}
//}

