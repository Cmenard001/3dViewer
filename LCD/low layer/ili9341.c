#include "ili9341.h"
#include "../../stm32f4xx/stm32f4xx_spi.h"
#include "../../config/config_pin.h"
#include "../../config/config_use.h"
#include "../../spi/spi.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "../../systick.h"

/* Pin definitions */
#define ILI9341_RST_SET()			GPIO_SetBits(LCD_RESET_PORT)
#define ILI9341_RST_RESET()			GPIO_ResetBits(LCD_RESET_PORT)
#define ILI9341_CS_SET()			GPIO_SetBits(LCD_CS)
#define ILI9341_CS_RESET()			GPIO_ResetBits(LCD_CS)
#define ILI9341_WRX_SET()			GPIO_SetBits(LCD_D_C_PORT)
#define ILI9341_WRX_RESET()			GPIO_ResetBits(LCD_D_C_PORT)

/* Private defines */
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

typedef enum{				// Division ration for internal clocks
	LCD_FRC_DIVA_1	= 0x00,	// Fosc		(default)
	LCD_FRC_DIVA_2	= 0x01,	// Fosc/2
	LCD_FRC_DIVA_4	= 0x02,	// Fosc/4
	LCD_FRC_DIVA_8	= 0x03	// Fosc/8
}FRC_param1_DIVA;

typedef enum{					// Frame Rate
	LCD_FRC_RTNA_61HZ	= 0x1F,	// 61 Hz
	LCD_FRC_RTNA_63HZ	= 0x1E,	// 63 Hz
	LCD_FRC_RTNA_65HZ	= 0x1D,	// 65 Hz
	LCD_FRC_RTNA_68HZ	= 0x1C,	// 68 Hz
	LCD_FRC_RTNA_70HZ	= 0x1B,	// 70 Hz	(default)
	LCD_FRC_RTNA_73HZ	= 0x1A,	// 73 Hz
	LCD_FRC_RTNA_76HZ	= 0x19,	// 76 Hz
	LCD_FRC_RTNA_79HZ	= 0x18,	// 79 Hz
	LCD_FRC_RTNA_83HZ	= 0x17,	// 83 Hz
	LCD_FRC_RTNA_86HZ	= 0x16,	// 86 Hz
	LCD_FRC_RTNA_90HZ	= 0x15,	// 90 Hz
	LCD_FRC_RTNA_95HZ	= 0x14,	// 95 Hz
	LCD_FRC_RTNA_100HZ	= 0x13,	// 100 Hz
	LCD_FRC_RTNA_106HZ	= 0x12,	// 106 Hz
	LCD_FRC_RTNA_112HZ	= 0x11,	// 112 Hz
	LCD_FRC_RTNA_119HZ	= 0x10	// 119 Hz
}FRC_param2_RTNA;

static volatile uint16_t ILI9341_x;
static volatile uint16_t ILI9341_y;
static volatile ILI931_Options_t ILI9341_Opts;
static volatile bool initialised = false;

/* Private functions */
static void ILI9341_initLCD(void);
static void ILI9341_sendData(uint8_t data);
static void ILI9341_sendCommand(uint8_t data);
//static void ILI9341_sendMultipleData(uint8_t *data, uint8_t count);		// Unused function
static void ILI9341_delay(volatile uint32_t delay);
static void ILI9341_setCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
static void ILI9341_INT_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void ILI9341_init(){

	if(initialised)
		return;
	initialised = true;

	/* CS high */
	ILI9341_CS_SET();

	/* Init SPI */
	SPI_init();

	/* Init LCD */
	ILI9341_initLCD();

	/* Set default settings */
	ILI9341_rotate(ILI9341_Orientation_Landscape_2);

	ILI9341_setCursorPosition(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height - 1);

	/* Fill with white color */
	ILI9341_fill(ILI9341_COLOR_WHITE);
}

void ILI9341_setConfig(void){
	SPI_setBaudRate(LCD_SPI, SPI_BaudRatePrescaler_2);
}

void ILI9341_displayOn(void) {
	ILI9341_sendCommand(ILI9341_DISPLAY_ON);
}

void ILI9341_displayOff(void) {
	ILI9341_sendCommand(ILI9341_DISPLAY_OFF);
}

ILI931_Options_t ILI9341_getOptions(void){
	return ILI9341_Opts;
}

void ILI9341_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	ILI9341_setCursorPosition(x, y, x, y);

	ILI9341_sendCommand(ILI9341_GRAM);
	ILI9341_sendData(color >> 8);
	ILI9341_sendData(color & 0xFF);
}

void ILI9341_fill(uint16_t color) {
	/* Fill entire screen */
	ILI9341_INT_fill(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height, color);
}

void ILI9341_rotate(ILI9341_Orientation_t orientation) {
	ILI9341_sendCommand(ILI9341_MAC);
	if (orientation == ILI9341_Orientation_Portrait_1) {
		ILI9341_sendData(0x58);
	} else if (orientation == ILI9341_Orientation_Portrait_2) {
		ILI9341_sendData(0x88);
	} else if (orientation == ILI9341_Orientation_Landscape_1) {
		ILI9341_sendData(0x28);
	} else if (orientation == ILI9341_Orientation_Landscape_2) {
		ILI9341_sendData(0xE8);
	}

	ILI9341_Opts.orientation = orientation;

	if (orientation == ILI9341_Orientation_Portrait_1 || orientation == ILI9341_Orientation_Portrait_2) {
		ILI9341_Opts.width = ILI9341_WIDTH;
		ILI9341_Opts.height = ILI9341_HEIGHT;
	} else {
		ILI9341_Opts.width = ILI9341_HEIGHT;
		ILI9341_Opts.height = ILI9341_WIDTH;
	}
}

void ILI9341_puts(uint16_t x, uint16_t y, char *str, FontDef_t *font, uint16_t foreground, uint32_t background) {
	uint16_t startX = x;

	/* Set X and Y coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight + 1;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}

		/* Put character to LCD */
		ILI9341_putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}



void ILI9341_getStringSize(char *str, FontDef_t *font, uint16_t *width, uint16_t *height) {
	uint16_t w = 0;
	*height = font->FontHeight;
	while (*str++) {
		w += font->FontWidth;
	}
	*width = w;
}

void ILI9341_putc(uint16_t x, uint16_t y, char c, FontDef_t *font, uint16_t foreground, uint32_t background) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}

	/* Draw rectangle for background */
	if(background != ILI9341_TRANSPARENT)
		ILI9341_INT_fill(ILI9341_x, ILI9341_y, ILI9341_x + font->FontWidth, ILI9341_y + font->FontHeight, background);

	/* Draw font data */
	for (i = 0; i < font->FontHeight; i++) {
		b = font->data[(c - 32) * font->FontHeight + i];
		for (j = 0; j < font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				ILI9341_drawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			}
		}
	}

	/* Set new pointer */
	ILI9341_x += font->FontWidth;
}

void ILI9341_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	/* Code by dewoller: https://github.com/dewoller */

	int16_t dx, dy, sx, sy, err, e2;
	uint16_t tmp;

	/* Check for overflow */
	if (x0 >= ILI9341_Opts.width) {
		x0 = ILI9341_Opts.width - 1;
	}
	if (x1 >= ILI9341_Opts.width) {
		x1 = ILI9341_Opts.width - 1;
	}
	if (y0 >= ILI9341_Opts.height) {
		y0 = ILI9341_Opts.height - 1;
	}
	if (y1 >= ILI9341_Opts.height) {
		y1 = ILI9341_Opts.height - 1;
	}

	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	/* Vertical or horizontal line */
	if (dx == 0 || dy == 0) {
		ILI9341_INT_fill(x0, y0, x1, y1, color);
		return;
	}

	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	while (1) {
		ILI9341_drawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void ILI9341_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	ILI9341_drawLine(x0, y0, x1, y0, color); 	//Top
	ILI9341_drawLine(x0, y0, x0, y1, color);	//Left
	ILI9341_drawLine(x1, y0, x1, y1, color);	//Right
	ILI9341_drawLine(x0, y1, x1, y1, color);	//Bottom
}

void ILI9341_drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t tmp;

	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	/* Fill rectangle */
	ILI9341_INT_fill(x0, y0, x1, y1, color);

	/* CS HIGH back */
	ILI9341_CS_SET();
}

void ILI9341_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	ILI9341_drawPixel(x0, y0 + r, color);
	ILI9341_drawPixel(x0, y0 - r, color);
	ILI9341_drawPixel(x0 + r, y0, color);
	ILI9341_drawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9341_drawPixel(x0 + x, y0 + y, color);
		ILI9341_drawPixel(x0 - x, y0 + y, color);
		ILI9341_drawPixel(x0 + x, y0 - y, color);
		ILI9341_drawPixel(x0 - x, y0 - y, color);

		ILI9341_drawPixel(x0 + y, y0 + x, color);
		ILI9341_drawPixel(x0 - y, y0 + x, color);
		ILI9341_drawPixel(x0 + y, y0 - x, color);
		ILI9341_drawPixel(x0 - y, y0 - x, color);
	}
}

void ILI9341_drawFilledCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	ILI9341_drawPixel(x0, y0 + r, color);
	ILI9341_drawPixel(x0, y0 - r, color);
	ILI9341_drawPixel(x0 + r, y0, color);
	ILI9341_drawPixel(x0 - r, y0, color);
	ILI9341_drawLine(x0 - r, y0, x0 + r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9341_drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		ILI9341_drawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		ILI9341_drawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		ILI9341_drawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}
}


void ILI9341_putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img, uint32_t size){
	ILI9341_setCursorPosition(x, y, x + width - 1, y + height - 1);

	/* Set command for GRAM data */
	ILI9341_sendCommand(ILI9341_GRAM);

	/* Send everything */
	ILI9341_CS_RESET();
	ILI9341_WRX_SET();

	/* Go to 16-bit SPI mode */
	SPI_setDataSize(LCD_SPI, SPI_DATA_SIZE_16_BIT);

#ifndef USE_LCD_DMA
	uint32_t i;
	for(i=0; i < size; i++)
		SPI_write(LCD_SPI, img[i]);
#else
#if BOARD_IHM_2020
	SPI1_DMA_send16BitArray((uint16_t *)img, size);
#else
	SPI2_DMA_send16BitArray((uint16_t *)img, size);
#endif
#endif

	SPI_setDataSize(LCD_SPI, SPI_DATA_SIZE_8_BIT);

	ILI9341_CS_SET();
}

void ILI9341_putImageWithTransparence(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img, uint16_t colorTransparence, uint32_t size){
	uint32_t x0, y0;
	for(y0=0; y0 < height; y0++){
		for(x0=0; x0 < width; x0++){
			uint32_t pixel = img[y0 * height + x0];
			if(pixel != colorTransparence)
				ILI9341_drawPixel(x + x0, y + y0, pixel);
		}
	}
}

void ILI9341_putColorInvertedImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t replaceColor, const uint16_t *img, uint16_t colorTransparence, uint32_t size){
	uint32_t i;
	for(i=0; i < size; i++){
		if(img[i] != colorTransparence)
			ILI9341_drawPixel(x + i % width, y + i / width, replaceColor);
	}
}

/***************************************************************
 *                       Fonctions priv�es
 ***************************************************************/

void ILI9341_printf(uint16_t x, uint16_t y, FontDef_t *font, uint16_t foreground, uint32_t background, const char *format, ...){
	char buffer[256];

	va_list args_list;
	va_start(args_list, format);
	vsnprintf(buffer, 256, format, args_list);
	va_end(args_list);

	ILI9341_puts(x, y, buffer, font, foreground, background);
}

static void ILI9341_setCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	ILI9341_sendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_sendData(x1 >> 8);
	ILI9341_sendData(x1 & 0xFF);
	ILI9341_sendData(x2 >> 8);
	ILI9341_sendData(x2 & 0xFF);

	//ILI9341_sendMultipleData((uint8_t []){x1 >> 8, x1 & 0xFF, x2 >> 8, x2 & 0xFF}, 4);

	ILI9341_sendCommand(ILI9341_PAGE_ADDR);
	ILI9341_sendData(y1 >> 8);
	ILI9341_sendData(y1 & 0xFF);
	ILI9341_sendData(y2 >> 8);
	ILI9341_sendData(y2 & 0xFF);

	//ILI9341_sendMultipleData((uint8_t []){y1 >> 8, y1 & 0xFF, y2 >> 8, y2 & 0xFF}, 4);
}

static void ILI9341_INT_fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t pixels_count;

	/* Set cursor position */
	ILI9341_setCursorPosition(x0, y0, x1, y1);

	/* Set command for GRAM data */
	ILI9341_sendCommand(ILI9341_GRAM);

	/* Calculate pixels count */
	pixels_count = (x1 - x0 + 1) * (y1 - y0 + 1);

	/* Send everything */
	ILI9341_CS_RESET();
	ILI9341_WRX_SET();

	/* Go to 16-bit SPI mode */
	SPI_setDataSize(LCD_SPI, SPI_DATA_SIZE_16_BIT);

#ifndef USE_LCD_DMA
	uint32_t i;
	for(i=0;i<pixels_count;i++)
		SPI_write(LCD_SPI, color);
#else
#if BOARD_IHM_2020
	SPI1_DMA_send16BitLoop(color, pixels_count);
#else
	SPI2_DMA_send16BitLoop(color, pixels_count);
#endif

#endif

	ILI9341_CS_SET();

	/* Go back to 8-bit SPI mode */
	SPI_setDataSize(LCD_SPI, SPI_DATA_SIZE_8_BIT);
}


static void ILI9341_delay(volatile uint32_t delay)
{
	//for (; delay != 0; delay--);
	Systick_delay(delay);
}

static void ILI9341_initLCD(void) {
	/* Force reset */
	ILI9341_RST_RESET();
	ILI9341_delay(2);		// > 10us !
	ILI9341_RST_SET();

	/* Delay for RST response */
	ILI9341_delay(7);		// > 5 ms before sending command !

	/* Software reset */
	ILI9341_sendCommand(ILI9341_RESET);
	ILI9341_delay(120);

	ILI9341_sendCommand(ILI9341_SLEEP_OUT);
	ILI9341_delay(7);

	// Power control A
	ILI9341_sendCommand(ILI9341_POWERA);
	ILI9341_sendData(0x39);
	ILI9341_sendData(0x2C);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x34);
	ILI9341_sendData(0x02);

	//ILI9341_sendMultipleData((uint8_t []){0x39, 0x2C, 0x00, 0x34, 0x02}, 5);

	// Power control B
	ILI9341_sendCommand(ILI9341_POWERB);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0xC1);
	ILI9341_sendData(0x30);

	//ILI9341_sendMultipleData((uint8_t []){0x00, 0xC1, 0x30}, 3);

	// Driver timing control A
	ILI9341_sendCommand(ILI9341_DTCA);
	ILI9341_sendData(0x85);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x78);

	//ILI9341_sendMultipleData((uint8_t []){0x85, 0x00, 0x78}, 3);


	// Driver timing control B
	ILI9341_sendCommand(ILI9341_DTCB);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x00);

	//ILI9341_sendMultipleData((uint8_t []){0x00, 0x00}, 2);

	// Power on sequence control
	ILI9341_sendCommand(ILI9341_POWER_SEQ);
	ILI9341_sendData(0x64);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x12);
	ILI9341_sendData(0x81);

	//ILI9341_sendMultipleData((uint8_t []){0x64, 0x03, 0x12, 0x81}, 4);

	// Pump ratio control
	ILI9341_sendCommand(ILI9341_PRC);
	ILI9341_sendData(0x20);

	// Power control 1
	ILI9341_sendCommand(ILI9341_POWER1);
	ILI9341_sendData(0x03);

	// Power control 2
	ILI9341_sendCommand(ILI9341_POWER2);
	ILI9341_sendData(0x10);

	// VCOM control 1
	ILI9341_sendCommand(ILI9341_VCOM1);
	ILI9341_sendData(0x3E);
	ILI9341_sendData(0x28);

	//ILI9341_sendMultipleData((uint8_t []){0x3E, 0x28}, 2);

	// VCOM control 2
	ILI9341_sendCommand(ILI9341_VCOM2);
	ILI9341_sendData(0x86);

	// Memory access control
	ILI9341_sendCommand(ILI9341_MAC);
	ILI9341_sendData(0x48);

	// Pixel format set
	ILI9341_sendCommand(ILI9341_PIXEL_FORMAT);
	ILI9341_sendData(0x55);

	// Frame rate control
	ILI9341_sendCommand(ILI9341_FRC);
	ILI9341_sendData(LCD_FRC_DIVA_1);
	ILI9341_sendData(LCD_FRC_RTNA_119HZ);

	//ILI9341_sendMultipleData((uint8_t []){LCD_FRC_DIVA_1, LCD_FRC_RTNA_119HZ}, 2);

	// Display function control
	ILI9341_sendCommand(ILI9341_DFC);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x82);
	ILI9341_sendData(0x27);

	//ILI9341_sendMultipleData((uint8_t []){0x08, 0x82, 0x27}, 3);

	// 3Gamma function disable
	ILI9341_sendCommand(ILI9341_3GAMMA_EN);
	ILI9341_sendData(0x00);

	// Gamma curve selected
	ILI9341_sendCommand(ILI9341_GAMMA);
	ILI9341_sendData(0x01);

	// Set positive gamma
	ILI9341_sendCommand(ILI9341_PGAMMA);
	ILI9341_sendData(0x0F);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0x2B);
	ILI9341_sendData(0x0C);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x4E);
	ILI9341_sendData(0xF1);
	ILI9341_sendData(0x37);
	ILI9341_sendData(0x07);
	ILI9341_sendData(0x10);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x09);
	ILI9341_sendData(0x00);

	//ILI9341_sendMultipleData((uint8_t []){0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00}, 15);

	// Set negative gama
	ILI9341_sendCommand(ILI9341_NGAMMA);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x14);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x11);
	ILI9341_sendData(0x07);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0xC1);
	ILI9341_sendData(0x48);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x0F);
	ILI9341_sendData(0x0C);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0x36);
	ILI9341_sendData(0x0F);

	//ILI9341_sendMultipleData((uint8_t []){0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F}, 15);

	ILI9341_delay(5);

	ILI9341_sendCommand(ILI9341_DISPLAY_ON);
}

static void ILI9341_sendCommand(uint8_t data) {
	ILI9341_WRX_RESET();
	ILI9341_CS_RESET();
	SPI_write(LCD_SPI, data);
	ILI9341_CS_SET();
}

static void ILI9341_sendData(uint8_t data) {
	ILI9341_WRX_SET();
	ILI9341_CS_RESET();
	SPI_write(LCD_SPI, data);
	ILI9341_CS_SET();
}

// Unused function
/*static void ILI9341_sendMultipleData(uint8_t *data, uint8_t count){
	ILI9341_WRX_SET();
	ILI9341_CS_RESET();

	#ifndef USE_LCD_DMA
		uint32_t i;
		for(i=0;i<count;i++)
			SPI_write(LCD_SPI, data[i]);
	#else
		SPI2_DMA_send8BitArray(data, count);
	#endif

	ILI9341_CS_SET();
}*/
