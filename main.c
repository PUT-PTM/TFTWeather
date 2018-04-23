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
//#include "bmp280.h"
/*#include "state.h"
#include "clocks.h"
#include "onboard_led.h"
#include "other_stuff.h"
#include "serialtx.h"


float pressure;
int8_t data;
*/

int main(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
		I2C_InitTypeDef  I2C_InitStruct;

		// Enable Clocks
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		// Make sure I2C bus is working
		i2c_helper_unstick(GPIOB, GPIO_Pin_8, GPIO_Pin_9);

		// Enable Clocks (again)
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);

		// Set Pins as Alternate Function: I2C
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

		// Configure and Enable Pins B8 & B9 as Alternate Function and Open Drain
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(GPIOB, &GPIO_InitStruct);

		// Configure and Enable I2C at 400 kHz
		I2C_DeInit(I2C1);
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_ClockSpeed = 400000;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0x77;
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_Init(I2C1, &I2C_InitStruct);

		// Enable I2C
		I2C_Cmd(I2C1, ENABLE);

}
