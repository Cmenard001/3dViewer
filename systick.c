/*
 * systick.c
 *
 *  Created on: 4 mai 2016
 *      Author: Nirgal
 */


#include "systick.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "stddef.h"

#define MAX_CALLBACK_FUNCTION_NB	16

//Tableau de pointeurs sur fonctions qui doivent être appelées périodiquement (1ms) par l'IT systick.
static callback_fun_t callback_functions[MAX_CALLBACK_FUNCTION_NB];
static bool initialized = false;
static volatile uint32_t absolute_time = 0;

void Systick_init(void)
{
	uint8_t i;
	for(i = 0; i<MAX_CALLBACK_FUNCTION_NB; i++)
		callback_functions[i] = NULL;
//	HAL_NVIC_SetPriority(SysTick_IRQn , 0,  0);
	RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
		NVIC_SetPriority(SysTick_IRQn, 0);
	initialized = true;
}

//Routine d'interruption appelée automatiquement à chaque ms.
void SysTick_Handler(void)
{
	//On se doit de faire appel aux deux fonctions de la HAL...
//	HAL_IncTick();
//	HAL_SYSTICK_IRQHandler();
	absolute_time++;

	if(!initialized)
		Systick_init();

	uint8_t i;
	for(i = 0; i<MAX_CALLBACK_FUNCTION_NB; i++)
	{
		if(callback_functions[i])
			(*callback_functions[i])();		//Appels des fonctions.
	}
}

//Ajout d'une fonction callback dans le tableau, si une place est disponible
bool Systick_add_callback_function(callback_fun_t func)
{
	uint8_t i;
	if(!initialized)
		Systick_init();

	for(i = 0; i<MAX_CALLBACK_FUNCTION_NB; i++)
	{
		if(!callback_functions[i])	//On a trouvé une place libre ?
		{
			callback_functions[i] = func;
			return true;
		}
	}
	return false;	//Pas de place libre !

}

//Retrait d'une fonction callback, si elle existe
bool Systick_remove_callback_function(callback_fun_t func)
{
	uint8_t i;
	if(!initialized)
		Systick_init();
	for(i = 0; i<MAX_CALLBACK_FUNCTION_NB; i++)
	{
		if(callback_functions[i] == func)	//On a trouvé la fonction à retirer ! ?
		{
			callback_functions[i] = NULL;
			return true;
		}
	}
	return false;	//On a pas trouvé la fonction à retirer
}

uint32_t SYSTICK_get_time_us(void)
{
	uint32_t t_us;
	static uint32_t previous_t_us = 0;
	__disable_irq();
	//t_us = 1000 - SysTick->VAL / 64;
	t_us = absolute_time * 1000 + 1000 - SysTick->VAL / 64;
	__enable_irq();


	if(previous_t_us >= t_us)
		t_us += 1000;
	previous_t_us = t_us ;

	return t_us;
}


void Systick_delay(uint32_t ms)
{
	uint32_t end_time;
	end_time = absolute_time + ms;
	while(absolute_time < end_time);
}

