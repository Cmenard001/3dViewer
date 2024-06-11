/*
 *	Club Robot ESEO 2010 - 2011
 *	???
 *
 *	Fichier : QS_spi.h
 *	Package : Qualité Soft
 *	Description : Fonction SPI
 *	Auteur : Ronan
 *  Licence : CeCILL-C (voir LICENCE.txt)
 *	Version 20100929
 */

/** ----------------  Defines possibles  --------------------
 *	USE_SPI1					: Activation de l'SPI 1
 *	USE_SPI2					: Activation de l'SPI 2
 *
 *	SPI2_ON_DMA					: Activation du DMA sur l'SPI 2
 */


 #ifndef QS_SPI_H
	#define QS_SPI_H

	#include "../config/config_use.h"
	#include "../stm32f4xx/stm32f4xx_spi.h"

	typedef enum{
		SPI_DATA_SIZE_8_BIT,
		SPI_DATA_SIZE_16_BIT
	}spi_data_size_e;

	void SPI_init(void);

	// @ref SPI_BaudRate_Prescaler
	void SPI_setBaudRate(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler);

	// @ref SPI_Clock_Phase
	void SPI_setCPHA(SPI_TypeDef* SPIx, uint16_t SPI_CPHA);

	// @ref SPI_Clock_Polarity
	void SPI_setCPOL(SPI_TypeDef* SPIx, uint16_t SPI_CPOL);

	// @ref SPI_MSB_LSB_transmission
	void SPI_setFirstBit(SPI_TypeDef* SPIx, uint16_t SPI_FirstBit);

	void SPI_setDataSize(SPI_TypeDef* SPIx, spi_data_size_e spi_data_size);

	uint16_t SPI_exchange(SPI_TypeDef* SPIx, uint16_t data);
	void SPI_write(SPI_TypeDef* SPIx, uint16_t data);
	uint16_t SPI_read(SPI_TypeDef* SPIx);

	#ifdef USE_SPI1
		uint16_t SPI1_exchange(uint16_t data);
		void SPI1_write(uint16_t data);
		uint16_t SPI1_read();
		void SPI1_setDataSize(spi_data_size_e spi_data_size);
		void SPI1_DMA_send16BitLoop(uint16_t data, uint32_t count);
		#define SPI1_put_byte SPI1_write

		void SPI1_DMA_send16BitLoop(uint16_t data, uint32_t count);
		void SPI1_DMA_send16BitArray(uint16_t *data, uint32_t count);
		void SPI1_DMA_send8BitArray(uint8_t *data, uint32_t count);
	#endif /* def USE_SPI1 */

	#ifdef USE_SPI2
		uint16_t SPI2_exchange(uint16_t data);
		void SPI2_write(uint16_t data);
		uint16_t SPI2_read();
		#define SPI2_put_byte SPI2_write

		void SPI2_DMA_send16BitLoop(uint16_t data, uint32_t count);
		void SPI2_DMA_send16BitArray(uint16_t *data, uint32_t count);
		void SPI2_DMA_send8BitArray(uint8_t *data, uint32_t count);
	#endif /* def USE_SPI2 */

#endif /* ndef QS_SPI_H */
