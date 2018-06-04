/*********************************************************************************************************
 *
 * Project			   : TFTWeather
 * File                : main.c
 * Version             : V1.0
 * By                  : Artur Bronka
 * 					   : Lukasz Dobek
 *
 *********************************************************************************************************/

// Includes.
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "dht11.h"
#include "LCD.h"
#include "AsciiLib.h"
#include "TouchPanel.h"
// ---------------------------------------------------------------------------


/*
 *
 * Variables section.
 *
 */


// Variables used to save temperature and other values read from DHT11 sensor.
uint8_t *Rh,*RhDec,*Temp,*TempDec,*ChkSum;
// ---------------------------------------------------------------------------

// Variables used to properly change temperature and humidity values from DHT11 sensor.
char tempStr[3];
char humiStr[3];
int t;
int h;
// ---------------------------------------------------------------------------

// Variable used to properly detect touches on TPanel.
Coordinate * Ptr;
// ---------------------------------------------------------------------------

// Counter variable needed to switch between temperature and humidity value.
int counter=0;
// ---------------------------------------------------------------------------

/*
 *
 * Functions section.
 *
 */


// Function that draws TFTWeather magnificent logo.
void drawTFTLogo(uint16_t color, uint16_t bgColor);
// Functions that waits for user's tap.
void waitForTap();
// Pretty self-explanatory.
void drawSun();
void drawSnowflake();
void drawCloud();
void drawSunCloud();
// Function responsible for selecting proper background.
void pickBG();
// ---------------------------------------------------------------------------

// Main function.
int main(void)
{
	// Initialization section.
	LCD_Initializtion();
	TP_Init();
	DHT11initTIM4();

	// Calibrating touch panel.
	TouchPanel_Calibrate();

	// Each LCD_Clear function usage means, that the LCD screen will fill with color given in the parameter.
	// In this one example, we are using RGB565CONVERT function that transforms value from standard RGB to RGB used by LCD screen.
	LCD_Clear(RGB565CONVERT(128, 128, 128));

	drawTFTLogo(Black, RGB565CONVERT(128, 128, 128));

	waitForTap();

	DHT11Read(&Rh,&RhDec,&Temp,&TempDec,&ChkSum);

	pickBG();

	// That's where the magic happens.
	while(1)
	{
		// Displaying temperature, humidity and selecting proper background and image, related to temperature.
		switch(counter)
		{
			case 0:
			{
				t = Temp;

				tempStr[0] = (t/10)+'0';
				tempStr[1] = (t%10)+'0';

				if(Temp > 25){
					GUI_Text(20,20,"Temperature: ",Black,RGB565CONVERT(255, 153, 51));
					GUI_Text(20,40, tempStr ,Black,RGB565CONVERT(255, 153, 51));
					drawSun();
				}
				else if(Temp<=25 && Temp>15)
				{
					GUI_Text(20,20,"Temperature: ",Black,RGB565CONVERT(102, 255, 51));
					GUI_Text(20,40, tempStr ,Black,RGB565CONVERT(102, 255, 51));
					drawSunCloud();
				}
				else if(Temp<=15 && Temp>5)
				{
					GUI_Text(20,20,"Temperature: ",Black,RGB565CONVERT(153, 204, 255));
					GUI_Text(20,40, tempStr ,Black,RGB565CONVERT(153, 204, 255));
					drawCloud();
				}
				else if(Temp<=5 )
				{
					GUI_Text(20,20,"Temperature: ",Black,White);
					GUI_Text(20,40, tempStr ,Black,White);
					drawSnowflake();
				}

				waitForTap();

				pickBG();
				Ptr=0;
				counter++;

				break;
			}
			case 1:
			{
				h=Rh;
				humiStr[0] = (h/10)+'0';
				humiStr[1] = (h%10)+'0';

				if(Temp > 25){
					GUI_Text(20,20,"Humidity: ",Black,RGB565CONVERT(255, 153, 51));
					GUI_Text(20,40,humiStr,Black,RGB565CONVERT(255, 153, 51));
					drawSun();
				}
				else if(Temp<=25 && Temp>15)
				{
					GUI_Text(20,20,"Humidity: ",Black,RGB565CONVERT(102, 255, 51));
					GUI_Text(20,40,humiStr,Black,RGB565CONVERT(102, 255, 51));
					drawSunCloud();
				}
				else if(Temp<=15 && Temp>5)
				{
					GUI_Text(20,20,"Humidity: ",Black,RGB565CONVERT(153, 204, 255));
					GUI_Text(20,40,humiStr,Black,RGB565CONVERT(153, 204, 255));
					drawCloud();
				}
				else if(Temp<=5 )
				{
					GUI_Text(20,20,"Humidity: ",Black,White);
					GUI_Text(20,40,humiStr,Black,White);
					drawSnowflake();
				}

				waitForTap();

				pickBG();
				Ptr=0;
				counter++;
				break;
			}
			case 2:
			{
				counter=0;
				break;
			}
			default:
				break;
			}
	}
	// ---------------------------------------------------------------------------

}


void drawTFTLogo(uint16_t color, uint16_t bgColor){
	LCD_DrawLine(160,90,160,150,color);
	LCD_DrawLine(130,120,190,120, color);

	GUI_Text(143,100,"T",color,bgColor);
	GUI_Text(173,100,"F",color,bgColor);
	GUI_Text(143,130,"T",color,bgColor);
	GUI_Text(173,130,"W",color,bgColor);
}


void waitForTap(){
	do	{
		Ptr=Read_Ads7846();
	}
	while( Ptr == (void*)0 );
}

void drawSun(){
	GUI_Text(120,40, "          |" ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,56, "          |   ." ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,72, "   `.  *  |     .'" ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,88, "     `. ._|_* .'  .,",Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,104, "   . * .'   `.  *" ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,120, "-------|     |-------" ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,136, "   .  *`.___.' *  ." ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,152, "      .'  |* `.  *" ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,168, "    .' *  |  . `." ,Black,RGB565CONVERT(255, 153, 51));
	GUI_Text(120,184, "        . |" ,Black,RGB565CONVERT(255, 153, 51));
}

void drawSnowflake(){
    GUI_Text(120,40, "     o" ,Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,56, "o    :    o" ,Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,72, "  '.\'/.'" ,Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,88, "  :->@<-:",Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,104, "  .'/.\'." ,Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,120, "o    :    o" ,Black,RGB565CONVERT(255, 153, 51));
    GUI_Text(120,136, "     o" ,Black,RGB565CONVERT(255, 153, 51));
}

void drawCloud(){
    GUI_Text(90,40, "          .-~~~-." ,Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,56, "  .- ~ ~-(       )_ _" ,Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,72, " /                    ~ -." ,Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,88, "|                          ',",Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,104, " \                        .'" ,Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,120, "   ~- ._ ,. ,.,.,., ,.. -~" ,Black,RGB565CONVERT(153, 204, 255));
    GUI_Text(90,136, "           '       '" ,Black,RGB565CONVERT(153, 204, 255));
}

void drawSunCloud(){
    GUI_Text(120,40, "     .-.             " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,56, "  .-(   )-.          " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,72, " (     __) )-.         " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,88, "  `-(       __)       ",Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,104, "    `(____)-',          " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,120, "  - -  :   :  - -" ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,136, "      / `-' \ " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,152, "      / `-' \ " ,Black,RGB565CONVERT(102, 255, 51));
    GUI_Text(120,168, "         .         " ,Black,RGB565CONVERT(102, 255, 51));
}

void pickBG(){
	// Section responsible for displaying proper background, related to different temperatures.
	if(Temp > 25){
		LCD_Clear(RGB565CONVERT(255, 153, 51));
	}
	else if(Temp<=25 && Temp>15)
	{
		LCD_Clear(RGB565CONVERT(102, 255, 51));
	}
	else if(Temp<=15 && Temp>5)
	{
		LCD_Clear(RGB565CONVERT(153, 204, 255));
	}
	else if(Temp<=5 )
	{
		LCD_Clear(White);
	}
	// ---------------------------------------------------------------------------
}
