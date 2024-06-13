/*
 *  Club Robot ESEO 2009 - 2010
 *  Chomp
 *
 *  Fichier : QS_ports.c
 *  Package : Qualité Soft
 *  Description : Gestion des entrées-sorties GPIO
 *  Auteur : Gwenn
 *  Version 20100418
 */

#include "ports.h"
#include "../stm32f4xx/stm32f4xx_gpio.h"
#include "../stm32f4xx/stm32f4xx_adc.h"
#include "../config/config_use.h"
#include "../config/config_pin.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static void PORTS_set_pull(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, port_opt_e pull_up);

void PORTS_init(void){
	static bool initialized = false;
	if(initialized)
		return;

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Horloges périphériques: tout ce qui est utilisé */
	RCC_AHB1PeriphClockCmd(
		  RCC_AHB1Periph_GPIOA
		| RCC_AHB1Periph_GPIOB
		| RCC_AHB1Periph_GPIOC
		| RCC_AHB1Periph_GPIOD
		| RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* GPIOA */
	uint32_t port_a_mask =
			(PORT_IO_A_0  << 0 ) |
			(PORT_IO_A_1  << 1 ) |
			(PORT_IO_A_2  << 2 ) |
			(PORT_IO_A_3  << 3 ) |
			(PORT_IO_A_4  << 4 ) |
			(PORT_IO_A_5  << 5 ) |
			(PORT_IO_A_6  << 6 ) |
			(PORT_IO_A_7  << 7 ) |
			(PORT_IO_A_8  << 8 ) |
			(PORT_IO_A_9  << 9 ) |
			(PORT_IO_A_10 << 10) |
			(PORT_IO_A_11 << 11) |
			(PORT_IO_A_12 << 12) |
			(PORT_IO_A_13 << 13) |
			(PORT_IO_A_14 << 14) |
			(PORT_IO_A_15 << 15);
	GPIO_InitStructure.GPIO_Pin = ((uint32_t)port_a_mask) & 0xFFFF9FFF;	//JTMS-SWDIO, JTCK-SWCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = (~((uint32_t)port_a_mask)) & 0xFFFF9FFF;	//JTMS-SWDIO, JTCK-SWCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	PORTS_set_pull(GPIOA, GPIO_Pin_0, PORT_OPT_A_0);
	PORTS_set_pull(GPIOA, GPIO_Pin_1, PORT_OPT_A_1);
	PORTS_set_pull(GPIOA, GPIO_Pin_2, PORT_OPT_A_2);
	PORTS_set_pull(GPIOA, GPIO_Pin_3, PORT_OPT_A_3);
	PORTS_set_pull(GPIOA, GPIO_Pin_4, PORT_OPT_A_4);
	PORTS_set_pull(GPIOA, GPIO_Pin_5, PORT_OPT_A_5);
	PORTS_set_pull(GPIOA, GPIO_Pin_6, PORT_OPT_A_6);
	PORTS_set_pull(GPIOA, GPIO_Pin_7, PORT_OPT_A_7);
	PORTS_set_pull(GPIOA, GPIO_Pin_8, PORT_OPT_A_8);
	PORTS_set_pull(GPIOA, GPIO_Pin_9, PORT_OPT_A_9);
	PORTS_set_pull(GPIOA, GPIO_Pin_10, PORT_OPT_A_10);
	PORTS_set_pull(GPIOA, GPIO_Pin_11, PORT_OPT_A_11);
	PORTS_set_pull(GPIOA, GPIO_Pin_12, PORT_OPT_A_12);
	PORTS_set_pull(GPIOA, GPIO_Pin_13, PORT_OPT_A_13);
	PORTS_set_pull(GPIOA, GPIO_Pin_14, PORT_OPT_A_14);
	PORTS_set_pull(GPIOA, GPIO_Pin_15, PORT_OPT_A_15);

	/* GPIOB */
	uint32_t port_b_mask =
			(PORT_IO_B_0  << 0 ) |
			(PORT_IO_B_1  << 1 ) |
			(PORT_IO_B_2  << 2 ) |
			(PORT_IO_B_3  << 3 ) |
			(PORT_IO_B_4  << 4 ) |
			(PORT_IO_B_5  << 5 ) |
			(PORT_IO_B_6  << 6 ) |
			(PORT_IO_B_7  << 7 ) |
			(PORT_IO_B_8  << 8 ) |
			(PORT_IO_B_9  << 9 ) |
			(PORT_IO_B_10 << 10) |
			(PORT_IO_B_11 << 11) |
			(PORT_IO_B_12 << 12) |
			(PORT_IO_B_13 << 13) |
			(PORT_IO_B_14 << 14) |
			(PORT_IO_B_15 << 15);
	GPIO_InitStructure.GPIO_Pin = ((uint32_t)port_b_mask) & 0xFFFFFFFF; //JTDO/TRACESWO,
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = (~((uint32_t)port_b_mask)) & 0xFFFFFFFF; //JTDO/TRACESWO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PORTS_set_pull(GPIOB, GPIO_Pin_0, PORT_OPT_B_0);
	PORTS_set_pull(GPIOB, GPIO_Pin_1, PORT_OPT_B_1);
	PORTS_set_pull(GPIOB, GPIO_Pin_2, PORT_OPT_B_2);
	PORTS_set_pull(GPIOB, GPIO_Pin_3, PORT_OPT_B_3);
	PORTS_set_pull(GPIOB, GPIO_Pin_4, PORT_OPT_B_4);
	PORTS_set_pull(GPIOB, GPIO_Pin_5, PORT_OPT_B_5);
	PORTS_set_pull(GPIOB, GPIO_Pin_6, PORT_OPT_B_6);
	PORTS_set_pull(GPIOB, GPIO_Pin_7, PORT_OPT_B_7);
	PORTS_set_pull(GPIOB, GPIO_Pin_8, PORT_OPT_B_8);
	PORTS_set_pull(GPIOB, GPIO_Pin_9, PORT_OPT_B_9);
	PORTS_set_pull(GPIOB, GPIO_Pin_10, PORT_OPT_B_10);
	PORTS_set_pull(GPIOB, GPIO_Pin_11, PORT_OPT_B_11);
	PORTS_set_pull(GPIOB, GPIO_Pin_12, PORT_OPT_B_12);
	PORTS_set_pull(GPIOB, GPIO_Pin_13, PORT_OPT_B_13);
	PORTS_set_pull(GPIOB, GPIO_Pin_14, PORT_OPT_B_14);
	PORTS_set_pull(GPIOB, GPIO_Pin_15, PORT_OPT_B_15);

	/* GPIOC */
	uint32_t port_c_mask =
			(PORT_IO_C_0  << 0 ) |
			(PORT_IO_C_1  << 1 ) |
			(PORT_IO_C_2  << 2 ) |
			(PORT_IO_C_3  << 3 ) |
			(PORT_IO_C_4  << 4 ) |
			(PORT_IO_C_5  << 5 ) |
			(PORT_IO_C_6  << 6 ) |
			(PORT_IO_C_7  << 7 ) |
			(PORT_IO_C_8  << 8 ) |
			(PORT_IO_C_9  << 9 ) |
			(PORT_IO_C_10 << 10) |
			(PORT_IO_C_11 << 11) |
			(PORT_IO_C_12 << 12) |
			(PORT_IO_C_13 << 13) |
			(PORT_IO_C_14 << 14) |
			(PORT_IO_C_15 << 15);
	GPIO_InitStructure.GPIO_Pin = (uint32_t)port_c_mask;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ~((uint32_t)port_c_mask);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PORTS_set_pull(GPIOC, GPIO_Pin_0, PORT_OPT_C_0);
	PORTS_set_pull(GPIOC, GPIO_Pin_1, PORT_OPT_C_1);
	PORTS_set_pull(GPIOC, GPIO_Pin_2, PORT_OPT_C_2);
	PORTS_set_pull(GPIOC, GPIO_Pin_3, PORT_OPT_C_3);
	PORTS_set_pull(GPIOC, GPIO_Pin_4, PORT_OPT_C_4);
	PORTS_set_pull(GPIOC, GPIO_Pin_5, PORT_OPT_C_5);
	PORTS_set_pull(GPIOC, GPIO_Pin_6, PORT_OPT_C_6);
	PORTS_set_pull(GPIOC, GPIO_Pin_7, PORT_OPT_C_7);
	PORTS_set_pull(GPIOC, GPIO_Pin_8, PORT_OPT_C_8);
	PORTS_set_pull(GPIOC, GPIO_Pin_9, PORT_OPT_C_9);
	PORTS_set_pull(GPIOC, GPIO_Pin_10, PORT_OPT_C_10);
	PORTS_set_pull(GPIOC, GPIO_Pin_11, PORT_OPT_C_11);
	PORTS_set_pull(GPIOC, GPIO_Pin_12, PORT_OPT_C_12);
	PORTS_set_pull(GPIOC, GPIO_Pin_13, PORT_OPT_C_13);
	PORTS_set_pull(GPIOC, GPIO_Pin_14, PORT_OPT_C_14);
	PORTS_set_pull(GPIOC, GPIO_Pin_15, PORT_OPT_C_15);

	/* GPIOD */
	uint32_t port_d_mask =
			(PORT_IO_D_0  << 0 ) |
			(PORT_IO_D_1  << 1 ) |
			(PORT_IO_D_2  << 2 ) |
			(PORT_IO_D_3  << 3 ) |
			(PORT_IO_D_4  << 4 ) |
			(PORT_IO_D_5  << 5 ) |
			(PORT_IO_D_6  << 6 ) |
			(PORT_IO_D_7  << 7 ) |
			(PORT_IO_D_8  << 8 ) |
			(PORT_IO_D_9  << 9 ) |
			(PORT_IO_D_10 << 10) |
			(PORT_IO_D_11 << 11) |
			(PORT_IO_D_12 << 12) |
			(PORT_IO_D_13 << 13) |
			(PORT_IO_D_14 << 14) |
			(PORT_IO_D_15 << 15);
	GPIO_InitStructure.GPIO_Pin = (uint32_t)port_d_mask;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ~((uint32_t)port_d_mask);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	PORTS_set_pull(GPIOD, GPIO_Pin_0, PORT_OPT_D_0);
	PORTS_set_pull(GPIOD, GPIO_Pin_1, PORT_OPT_D_1);
	PORTS_set_pull(GPIOD, GPIO_Pin_2, PORT_OPT_D_2);
	PORTS_set_pull(GPIOD, GPIO_Pin_3, PORT_OPT_D_3);
	PORTS_set_pull(GPIOD, GPIO_Pin_4, PORT_OPT_D_4);
	PORTS_set_pull(GPIOD, GPIO_Pin_5, PORT_OPT_D_5);
	PORTS_set_pull(GPIOD, GPIO_Pin_6, PORT_OPT_D_6);
	PORTS_set_pull(GPIOD, GPIO_Pin_7, PORT_OPT_D_7);
	PORTS_set_pull(GPIOD, GPIO_Pin_8, PORT_OPT_D_8);
	PORTS_set_pull(GPIOD, GPIO_Pin_9, PORT_OPT_D_9);
	PORTS_set_pull(GPIOD, GPIO_Pin_10, PORT_OPT_D_10);
	PORTS_set_pull(GPIOD, GPIO_Pin_11, PORT_OPT_D_11);
	PORTS_set_pull(GPIOD, GPIO_Pin_12, PORT_OPT_D_12);
	PORTS_set_pull(GPIOD, GPIO_Pin_13, PORT_OPT_D_13);
	PORTS_set_pull(GPIOD, GPIO_Pin_14, PORT_OPT_D_14);
	PORTS_set_pull(GPIOD, GPIO_Pin_15, PORT_OPT_D_15);

	/* GPIOE */
	uint32_t port_e_mask =
			(PORT_IO_E_0  << 0 ) |
			(PORT_IO_E_1  << 1 ) |
			(PORT_IO_E_2  << 2 ) |
			(PORT_IO_E_3  << 3 ) |
			(PORT_IO_E_4  << 4 ) |
			(PORT_IO_E_5  << 5 ) |
			(PORT_IO_E_6  << 6 ) |
			(PORT_IO_E_7  << 7 ) |
			(PORT_IO_E_8  << 8 ) |
			(PORT_IO_E_9  << 9 ) |
			(PORT_IO_E_10 << 10) |
			(PORT_IO_E_11 << 11) |
			(PORT_IO_E_12 << 12) |
			(PORT_IO_E_13 << 13) |
			(PORT_IO_E_14 << 14) |
			(PORT_IO_E_15 << 15);
	GPIO_InitStructure.GPIO_Pin = ((uint32_t)port_e_mask) & 0xFFFFFFFF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = (~((uint32_t)port_e_mask)) & 0xFFFFFFFF;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	PORTS_set_pull(GPIOE, GPIO_Pin_0, PORT_OPT_E_0);
	PORTS_set_pull(GPIOE, GPIO_Pin_1, PORT_OPT_E_1);
	PORTS_set_pull(GPIOE, GPIO_Pin_2, PORT_OPT_E_2);
	PORTS_set_pull(GPIOE, GPIO_Pin_3, PORT_OPT_E_3);
	PORTS_set_pull(GPIOE, GPIO_Pin_4, PORT_OPT_E_4);
	PORTS_set_pull(GPIOE, GPIO_Pin_5, PORT_OPT_E_5);
	PORTS_set_pull(GPIOE, GPIO_Pin_6, PORT_OPT_E_6);
	PORTS_set_pull(GPIOE, GPIO_Pin_7, PORT_OPT_E_7);
	PORTS_set_pull(GPIOE, GPIO_Pin_8, PORT_OPT_E_8);
	PORTS_set_pull(GPIOE, GPIO_Pin_9, PORT_OPT_E_9);
	PORTS_set_pull(GPIOE, GPIO_Pin_10, PORT_OPT_E_10);
	PORTS_set_pull(GPIOE, GPIO_Pin_11, PORT_OPT_E_11);
	PORTS_set_pull(GPIOE, GPIO_Pin_12, PORT_OPT_E_12);
	PORTS_set_pull(GPIOE, GPIO_Pin_13, PORT_OPT_E_13);
	PORTS_set_pull(GPIOE, GPIO_Pin_14, PORT_OPT_E_14);
	PORTS_set_pull(GPIOE, GPIO_Pin_15, PORT_OPT_E_15);

//	PORTS_adc_init(NULL, ADC_SampleTime_28Cycles);

	initialized = true;
}

void PORTS_spi_init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	#ifdef USE_SPI1
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//NSS : seulement en Slave
		//GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI1);	//NSS : seulement en Slave
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	#endif

	#if defined(USE_SPI2) || defined(USE_SPI2_DMA)
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//NSS : seulement en Slave
		//GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		//GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);	//NSS : seulement en Slave
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	#endif
}


static void PORTS_set_pull(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, port_opt_e pull_up)
{
	  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;
	  /* Check the parameters */
	  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	  assert_param(IS_GPIO_PIN(GPIO_Pin));
	  assert_param(IS_GPIO_PUPD((GPIOPuPd_TypeDef)pull_up));

	  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
	  {
		pos = ((uint32_t)0x01) << pinpos;
		/* Get the port pins position */
		currentpin = GPIO_Pin & pos;
		if (currentpin == pos)
		{
		  /* Pull-up Pull down resistor configuration*/
		  GPIOx->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)pinpos * 2));
		  GPIOx->PUPDR |= (((uint32_t)pull_up) << (pinpos * 2));
		}
	  }
}

void PORTS_set_opendrain(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool open_drain)
{
	  uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;
	  /* Check the parameters */
	  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	  assert_param(IS_GPIO_PIN(GPIO_Pin));

	  for (pinpos = 0x00; pinpos < 0x10; pinpos++)
	  {
		pos = ((uint32_t)0x01) << pinpos;
		/* Get the port pins position */
		currentpin = GPIO_Pin & pos;
		if (currentpin == pos)
		{
			GPIOx->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pinpos)) ;
			GPIOx->OTYPER |= (uint16_t)((open_drain) << ((uint16_t)pinpos));
		}
	  }
}

