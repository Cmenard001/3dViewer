/*
 *	Club Robot 2014
 *
 *	Fichier : main.c
 *	Package : IHM
 *	Description : fonction principale
 *	Auteur : Anthony
 *	Version 20080924
 */

#include <stdint.h>
#include "./stm32f4xx/system_stm32f4xx.h"
#include "./math/trigo.h"
#include "./math/math_high_level.h"
#include "./math/geometry_unity.h"
#include "./ihm/viewer.h"
#include "./spi/ports.h"
#include "systick.h"
#include "LCD/low layer/ili9341.h"

volatile uint8_t t_ms = 0;

void initialisation(void)
{
	PORTS_init();

	Systick_init();

	// Initialisation du tableau de cosinus
	trigo_init();

	// Initialisation des boutons
	button_init();

	// Initialisation de la high level math
	math_init();
}

int main (void)
{

	initialisation();

	while(1)
	{
		static uint16_t i = 0;
		i = (i + 1) % 240;
		ILI9341_drawLine(i, 50, 200, 120, ILI9341_COLOR_RED);
		// viewer_process_main();
		math_process_main();
	}

	return 0;
}


void MAIN_process_it(uint8_t ms){
	t_ms += ms;
}
