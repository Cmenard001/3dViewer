/*
 *  Club Robot ESEO 2009 - 2010
 *  Chomp
 *
 *  Fichier : QS_ports.h
 *  Package : Qualité Soft
 *  Description : Gestion des entrées-sorties GPIO
 *  Auteur : Gwenn
 *  Version 20100418
 */

/** ----------------  Defines possibles  --------------------
 *	PORT_IO_A_x					: x (0 à 15) / Configuration IO du port A
 *	PORT_IO_B_x					: x (0 à 15) / Configuration IO du port B
 *	PORT_IO_C_x					: x (0 à 15) / Configuration IO du port C
 *	PORT_IO_D_x					: x (0 à 15) / Configuration IO du port D
 *	PORT_IO_E_x					: x (0 à 15) / Configuration IO du port E
 *
 * ----------------  Choses à savoir  --------------------
 *	USE_ANx						: x (0 à 15) / Configuration de l'ADC numéro x
 *	USE_PWMx					: x (1 à 4)	 / Configuration de la PWM numéro x
 *	USE_QUEIx					: x (1 à 2)	 / Configuration du module QUEI x
 *	USE_I2Cx					: x (1 à 2)	 / Configuration de l'I2C x
 *
 *	Pour la configuration des ports utiliser PORT_IO_OUTPUT ou PORT_IO_INPUT
 */

#ifndef QS_PORTS_H
	#define QS_PORTS_H

	#include "../stm32f4xx/stm32f4xx_gpio.h"
	#include "../stm32f4xx/stm32f4xx_i2c.h"

	typedef enum{
		 PORT_IO_INPUT = ((uint32_t)1),
		 PORT_IO_OUTPUT = ((uint32_t)0)
	}port_tris_e;

	typedef enum{
		PORT_OPT_NO_PULL = GPIO_PuPd_NOPULL,
		PORT_OPT_PULL_UP = GPIO_PuPd_UP,
		PORT_OPT_PULL_DOWN = GPIO_PuPd_DOWN
	}port_opt_e;


	#define toggle_led_2(port, pin)   GPIO_WriteBit(port, pin,(GPIO_ReadOutputDataBit(port, pin)?Bit_RESET:Bit_SET))
	#define toggle_led(port_comma_pin)   GPIO_WriteBit(port_comma_pin,(GPIO_ReadOutputDataBit(port_comma_pin)?Bit_RESET:Bit_SET))

	void PORTS_init(void);

	void PORTS_spi_init();

#endif /* ndef QS_PORTS_H */
