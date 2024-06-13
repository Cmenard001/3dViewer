/*
 * systick.h
 *
 *  Created on: 4 mai 2016
 *      Author: Nirgal
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stdbool.h"
#include "stdint.h"

typedef void(*callback_fun_t)(void);	//Type pointeur sur fonction

void Systick_init(void);

//Routine d'interruption appelée automatiquement à chaque ms.
void SysTick_Handler(void);


bool Systick_add_callback_function(callback_fun_t func);


bool Systick_remove_callback_function(callback_fun_t func);

uint32_t SYSTICK_get_time_us(void);

void Systick_delay(uint32_t ms);

#endif /* SYSTICK_H_ */
