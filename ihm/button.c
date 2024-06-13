/*
 * button.c
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */
#include "button.h"

#include "../stm32f4xx/stm32f4xx_gpio.h"
#include "../stm32f4xx/stm32f4xx.h"

#define GPIO_BUTTON_LEFT GPIOE
#define PIN_BUTTON_LEFT GPIO_Pin_9

#define GPIO_BUTTON_RIGHT GPIOE
#define PIN_BUTTON_RIGHT GPIO_Pin_10

#define GPIO_BUTTON_UP GPIOE
#define PIN_BUTTON_UP GPIO_Pin_11

#define GPIO_BUTTON_DOWN GPIOE
#define PIN_BUTTON_DOWN GPIO_Pin_12

#define GPIO_BUTTON_CENTER GPIOE
#define PIN_BUTTON_CENTER GPIO_Pin_13

void BUTTONS_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	//Initialisation du port du bouton bleu (carte Nucleo)
	GPIO_Init(GPIO_BUTTON_LEFT, &GPIO_InitStructure);
	GPIO_Init(GPIO_BUTTON_RIGHT, &GPIO_InitStructure);
	GPIO_Init(GPIO_BUTTON_UP, &GPIO_InitStructure);
	GPIO_Init(GPIO_BUTTON_DOWN, &GPIO_InitStructure);
	GPIO_Init(GPIO_BUTTON_CENTER, &GPIO_InitStructure);
}


uint8_t BUTTON_left_read(void)
{
	return !GPIO_ReadInputDataBit(GPIO_BUTTON_LEFT, PIN_BUTTON_LEFT);
}

uint8_t BUTTON_right_read(void)
{
	return !GPIO_ReadInputDataBit(GPIO_BUTTON_RIGHT, PIN_BUTTON_RIGHT);
}

uint8_t BUTTON_up_read(void)
{
	return !GPIO_ReadInputDataBit(GPIO_BUTTON_UP, PIN_BUTTON_UP);
}

uint8_t BUTTON_down_read(void)
{
	return !GPIO_ReadInputDataBit(GPIO_BUTTON_DOWN, PIN_BUTTON_DOWN);
}

uint8_t BUTTON_center_read(void)
{
	return GPIO_ReadInputDataBit(GPIO_BUTTON_CENTER, PIN_BUTTON_CENTER);
}
