/*
 *	Club Robot ESEO 2010 - 2011
 *	???
 *
 *	Fichier : QS_spi.c
 *	Package : Qualité Soft
 *	Description : Fonction SPI
 *	Auteur : Ronan
 *  Licence : CeCILL-C (voir LICENCE.txt)
 *	Version 20100929
 */

#include <stdbool.h>

#include "spi.h"
#include "ports.h"
#include "../stm32f4xx/stm32f4xx_dma.h"
#include "../config/config_use.h"

#define SPI1_SPI_HANDLE SPI1
#define SPI1_S../config/PI_CLOCK  RCC_APB2Periph_SPI1

#define SPI2_SPI_HANDLE SPI2
#define SPI2_SPI_CLOCK  RCC_APB1Periph_SPI2

/**
 * @brief  Check SPI busy status
 */
#define SPI_IS_BUSY(SPIx)                   (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0)

/**
 * @brief  SPI wait till end
 */
#define SPI_WAIT_TX(SPIx)                   while ((SPIx->SR & SPI_FLAG_TXE) == 0 || (SPIx->SR & SPI_FLAG_BSY))
#define SPI_WAIT_RX(SPIx)                   while ((SPIx->SR & SPI_FLAG_RXNE) == 0 || (SPIx->SR & SPI_FLAG_BSY))

#if defined(USE_SPI1) || defined(USE_SPI2)

	#define SPI1_DMA_STREAM	DMA2_Stream3
	#define SPI1_DMA_CHAN	DMA_Channel_3

	#define SPI2_DMA_STREAM	DMA1_Stream4
	#define SPI2_DMA_CHAN	DMA_Channel_0

	#ifdef USE_SPI1
		static uint8_t SPI1_InitDMA_send16BitLoop(uint16_t value, uint16_t count);
		static uint8_t SPI1_InitDMA_send16bitArray(uint16_t *data, uint16_t count);
		static uint8_t SPI1_InitDMA_send8bitArray(uint8_t *data, uint16_t count);
		static void SPI1_waitEndDMA(void);
	#endif

	#ifdef USE_SPI2
		static uint8_t SPI2_InitDMA_send16BitLoop(uint16_t value, uint16_t count);
		static uint8_t SPI2_InitDMA_send16bitArray(uint16_t *data, uint16_t count);
		static uint8_t SPI2_InitDMA_send8bitArray(uint8_t *data, uint16_t count);
		static void SPI2_waitEndDMA(void);
	#endif

	static volatile SPI_InitTypeDef SPI_InitStructure[2];
	static volatile bool initialized = false;

	void SPI_init(void)
	{
		if(initialized)
			return;
		initialized = true;

		PORTS_spi_init();

		SPI_InitStructure[0].SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure[0].SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure[0].SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure[0].SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure[0].SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure[0].SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure[0].SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure[0].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStructure[0].SPI_CRCPolynomial = 7;

		SPI_InitStructure[1].SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure[1].SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure[1].SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure[1].SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure[1].SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure[1].SPI_CPHA = SPI_CPHA_1Edge;
		SPI_InitStructure[1].SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure[1].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
		SPI_InitStructure[1].SPI_CRCPolynomial = 7;

		#ifdef SPI1_ON_DMA
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		#endif

		#ifdef SPI2_ON_DMA
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
		#endif

		#ifdef USE_SPI1
			RCC_APB2PeriphClockCmd(SPI1_SPI_CLOCK, ENABLE);
			SPI_Init(SPI1_SPI_HANDLE, (SPI_InitTypeDef*)&(SPI_InitStructure[0]));
			SPI_Cmd(SPI1_SPI_HANDLE, ENABLE);
		#endif /* def USE_SPI1 */

		#ifdef USE_SPI2
			RCC_APB1PeriphClockCmd(SPI2_SPI_CLOCK, ENABLE);
			SPI_Init(SPI2_SPI_HANDLE, (SPI_InitTypeDef*)&(SPI_InitStructure[1]));
			SPI_Cmd(SPI2_SPI_HANDLE, ENABLE);
		#endif /* def USE_SPI2 */

	}

	void SPI_setCPOL(SPI_TypeDef* SPIx, uint16_t SPI_CPOL){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_setCPOL : Erreur SPIx non conforme\n");
			return;
		}

		if(IS_SPI_CPOL(SPI_CPOL) == false){
			// error_printf("SPI_setCPOL : Erreur SPI_CPOL non conforme\n");
			return;
		}

		SPI_Cmd(SPIx, DISABLE);
		SPI_DeInit(SPIx);

		switch((uint32_t)SPIx){
			case (uint32_t)SPI1:
				SPI_InitStructure[0].SPI_CPOL = SPI_CPOL;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[0]));
				break;

			case (uint32_t)SPI2:
				SPI_InitStructure[1].SPI_CPOL = SPI_CPOL;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[1]));
				break;

			default :
				// error_printf("SPI_setCPOL : Erreur SPIx non implémenté\n");
				break;
		}
		SPI_Cmd(SPIx, ENABLE);
	}

	void SPI_setFirstBit(SPI_TypeDef* SPIx, uint16_t SPI_FirstBit){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_setFirstBit : Erreur SPIx non conforme\n");
			return;
		}

		if(IS_SPI_FIRST_BIT(SPI_FirstBit) == false){
			// error_printf("SPI_setFirstBit : Erreur SPI_FirstBit non conforme\n");
			return;
		}

		SPI_Cmd(SPIx, DISABLE);
		SPI_DeInit(SPIx);

		switch((uint32_t)SPIx){
			case (uint32_t)SPI1:
				SPI_InitStructure[0].SPI_FirstBit = SPI_FirstBit;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[0]));
				break;

			case (uint32_t)SPI2:
				SPI_InitStructure[1].SPI_FirstBit = SPI_FirstBit;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[1]));
				break;

			default :
				// error_printf("SPI_setFirstBit : Erreur SPIx non implémenté\n");
				break;
		}
		SPI_Cmd(SPIx, ENABLE);
	}

	void SPI_setCPHA(SPI_TypeDef* SPIx, uint16_t SPI_CPHA){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_setCPHA : Erreur SPIx non conforme\n");
			return;
		}

		if(IS_SPI_CPHA(SPI_CPHA) == false){
			// error_printf("SPI_setCPHA : Erreur SPI_CPHA non conforme\n");
			return;
		}

		SPI_Cmd(SPIx, DISABLE);
		SPI_DeInit(SPIx);

		switch((uint32_t)SPIx){
			case (uint32_t)SPI1:
				SPI_InitStructure[0].SPI_CPHA = SPI_CPHA;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[0]));
				break;

			case (uint32_t)SPI2:
				SPI_InitStructure[1].SPI_CPHA = SPI_CPHA;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[1]));
				break;

			default :
				// error_printf("SPI_setCPHA : Erreur SPIx non implémenté\n");
				break;
		}
		SPI_Cmd(SPIx, ENABLE);
	}

	void SPI_setBaudRate(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_setBaudRate : Erreur SPIx non conforme\n");
			return;
		}

		if(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler) == false){
			// error_printf("SPI_setBaudRate : Erreur SPI_BaudRatePrescaler non conforme\n");
			return;
		}

		SPI_Cmd(SPIx, DISABLE);
		SPI_DeInit(SPIx);

		switch((uint32_t)SPIx){
			case (uint32_t)SPI1:
				SPI_InitStructure[0].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[0]));
				break;

			case (uint32_t)SPI2:
				SPI_InitStructure[1].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler;
				SPI_Init(SPIx, (SPI_InitTypeDef*)&(SPI_InitStructure[1]));
				break;

			default :
				// error_printf("SPI_setBaudRate : Erreur SPIx non implémenté\n");
				break;
		}
		SPI_Cmd(SPIx, ENABLE);
	}

	void SPI_setDataSize(SPI_TypeDef* SPIx, spi_data_size_e spi_data_size){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_setDataSize : Erreur SPIx non conforme\n");
			return;
		}

		switch(spi_data_size){
			case SPI_DATA_SIZE_8_BIT:
				SPI_DataSizeConfig(SPIx, SPI_DataSize_8b);
				break;

			case SPI_DATA_SIZE_16_BIT:
				SPI_DataSizeConfig(SPIx, SPI_DataSize_16b);
				break;

			default:
				// error_printf("SPI_setDataSize argument erroné : %d\n", spi_data_size);
				break;
		}
	}

	uint16_t SPI_exchange(SPI_TypeDef* SPIx, uint16_t data){
		if(IS_SPI_ALL_PERIPH(SPIx) == false){
			// error_printf("SPI_exchange : Erreur SPIx non conforme\n");
			return 0;
		}

		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return 0;
		}

		/* Wait busy */
		SPI_WAIT_TX(SPIx);

		/* Fill output buffer with data */
		SPIx->DR = data;

		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);

		/* Read data register */
		return SPIx->DR;
	}

	inline void SPI_write(SPI_TypeDef* SPIx, uint16_t data){
		SPI_exchange(SPIx, data);
	}

	inline uint16_t SPI_read(SPI_TypeDef* SPIx){
		return SPI_exchange(SPIx, 0x00);
	}


#endif  /* defined(USE_SPI1) || defined(USE_SPI2) */

#ifdef USE_SPI1

	void SPI1_setDataSize(spi_data_size_e spi_data_size)
	{
		SPI_setDataSize(SPI1_SPI_HANDLE, spi_data_size);
	}

	uint16_t SPI1_exchange(uint16_t data){
		return SPI_exchange(SPI1_SPI_HANDLE, data);
	}

	void SPI1_write(uint16_t data){
		SPI_exchange(SPI1_SPI_HANDLE, data);
	}

	uint16_t SPI1_read(){
		return SPI_exchange(SPI1_SPI_HANDLE, 0x00);
	}

	void SPI1_DMA_send16BitLoop(uint16_t data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI1_InitDMA_send16BitLoop(data, (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI1_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	void SPI1_DMA_send16BitArray(uint16_t *data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI1_InitDMA_send16bitArray((uint16_t *)(data + index), (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI1_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	void SPI1_DMA_send8BitArray(uint8_t *data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI1_InitDMA_send8bitArray((uint8_t *)(data + index), (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI1_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	static void SPI1_waitEndDMA(void){
		while(SPI1_DMA_STREAM->NDTR);
		DMA_DeInit(SPI1_DMA_STREAM);
	}

	static uint8_t SPI1_InitDMA_send16BitLoop(uint16_t value, uint16_t count){

		static uint16_t dummy;

		dummy = value;

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI1_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&dummy;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI1_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI1_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI1_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI1->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

	static uint8_t SPI1_InitDMA_send16bitArray(uint16_t *data, uint16_t count){

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI1_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI1_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI1_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI1_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI1->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

	static uint8_t SPI1_InitDMA_send8bitArray(uint8_t *data, uint16_t count){

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI1_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI1_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI1_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI1_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI1->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

#endif /* defined(USE_SPI2) */

#ifdef USE_SPI2

	void SPI2_setDataSize(spi_data_size_e spi_data_size)
	{
		return SPI_setDataSize(SPI2_SPI_HANDLE, spi_data_size);
	}

	uint16_t SPI2_exchange(uint16_t data){
		return SPI_exchange(SPI2_SPI_HANDLE, data);
	}

	void SPI2_write(uint16_t data){
		SPI_exchange(SPI2_SPI_HANDLE, data);
	}

	uint16_t SPI2_read(){
		return SPI_exchange(SPI2_SPI_HANDLE, 0x00);
	}

	void SPI2_DMA_send16BitLoop(uint16_t data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI2_InitDMA_send16BitLoop(data, (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI2_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	void SPI2_DMA_send16BitArray(uint16_t *data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI2_InitDMA_send16bitArray((uint16_t *)(data + index), (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI2_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	void SPI2_DMA_send8BitArray(uint8_t *data, uint32_t count){
		if(!initialized){
			// error_printf("SPI non initialisé ! Appeller SPI_init\n");
			return;
		}
		uint32_t index = 0;

		count++;	// Correction condition de fin DMA

		do{
			SPI2_InitDMA_send8bitArray((uint8_t *)(data + index), (count - index > 0xFFFF) ? 0xFFFF : count - index);
			SPI2_waitEndDMA();
			index += 0xFFFF;
		}while(count > index);
	}

	static void SPI2_waitEndDMA(void){
		while(SPI2_DMA_STREAM->NDTR);
		DMA_DeInit(SPI2_DMA_STREAM);
	}

	static uint8_t SPI2_InitDMA_send16BitLoop(uint16_t value, uint16_t count){

		static uint16_t dummy;

		dummy = value;

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI2_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&dummy;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI2_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI2_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI2_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI2->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

	static uint8_t SPI2_InitDMA_send16bitArray(uint16_t *data, uint16_t count){

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI2_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI2_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI2_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI2_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI2->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

	static uint8_t SPI2_InitDMA_send8bitArray(uint8_t *data, uint16_t count){

		DMA_InitTypeDef DMA_InitStructure;

		DMA_StructInit(&DMA_InitStructure);

		DMA_InitStructure.DMA_Channel = SPI2_DMA_CHAN;

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;

		DMA_InitStructure.DMA_BufferSize = count;

		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;

		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

		/* Deinit first TX stream */
		DMA_ClearFlag(SPI2_DMA_STREAM, DMA_FLAG_TCIF0 | DMA_FLAG_HTIF0 | DMA_FLAG_TEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_FEIF0);

		/* Init TX stream */
		DMA_Init(SPI2_DMA_STREAM, &DMA_InitStructure);

		/* Enable TX stream */
		SPI2_DMA_STREAM->CR |= DMA_SxCR_EN;

		/* Enable SPI TX DMA */
		SPI2->CR2 |= SPI_CR2_TXDMAEN;

		/* Return OK */
		return 1;
	}

#endif /* defined(USE_SPI2) */

