/**
 * @file optimized_tft.c
 * @author Cyprien Ménard
 * @brief Fichier de gestion de l'affichage optimisé sur l'écran TFT
 * @note Ce fichier est extrêmement inspiré de "stm32f1_ili9341.c"
 * @date 2024-06-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "optimized_tft.h"

#include "../../lib/bsp/tft_ili9341/stm32f1_ili9341.h"
#include "../../lib/hal/inc/stm32f1xx_hal.h"
#include "../../lib/bsp/stm32f1_spi.h"
#include "../../lib/bsp/stm32f1_gpio.h"

#include <stdint.h>
#include <malloc.h>
#include <stdbool.h>

#define HIGHINT(x)				((uint8_t)(((x) >> 8) & 0xFF))
#define LOWINT(x)				((uint8_t)((x) & 0xFF))

#define ILI9341_GRAM				0x2C
#define ILI9341_CS_RESET()			HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, 0)
#define ILI9341_WRX_SET()			HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, 1)

#define BACKGROUND_COLOR ILI9341_COLOR_WHITE
#define DRAWING_COLOR ILI9341_COLOR_BLACK

#define NB_BITS_PER_BYTE 8

static int8_t pixels[ILI9341_WIDTH * ILI9341_HEIGHT / NB_BITS_PER_BYTE];
static int8_t last_pixels[ILI9341_WIDTH * ILI9341_HEIGHT / NB_BITS_PER_BYTE];


static inline bool get_bit(int8_t* tab, int8_t index);
static inline bool get_bit(int8_t* tab, int8_t index)
{
    unsigned int kn,in;
    kn=(index+NB_BITS_PER_BYTE-1)/NB_BITS_PER_BYTE;
    in=(index-1)%NB_BITS_PER_BYTE;
    return (1 & (tab[kn] >> in));
}

static inline void set_bit(int8_t* tab, int8_t index, bool val);
static inline void set_bit(int8_t* tab, int8_t index, bool val)
{
    unsigned int kn,in;
    kn=(index+NB_BITS_PER_BYTE-1)/NB_BITS_PER_BYTE;
    in=(index-1)%NB_BITS_PER_BYTE;
    if (val)
    {
        tab[kn]= tab[kn] | (1 << in);
    }
    else
    {
        tab[kn]= tab[kn] & ~(1 << in);
    }
}

void OPTFT_init()
{
    ILI9341_Init();
    ILI9341_Rotate(3);
    for (uint16_t x = 0; x <= ILI9341_WIDTH; x++)
    {
        for (uint16_t y = 0; y <= ILI9341_HEIGHT; y++)
        {
            OPTFT_DrawPixel(x, y, PIXEL_OFF);
        }
    }
    OPTFT_refresh();
}

/**
 * @brief  Refresh the screen with the new pixels stored in the pixels_tab
 * @retval None
 */
void OPTFT_refresh()
{
    /* Set cursor position */
    ILI9341_SetCursorPosition(0, 0, 0, 0);

    /* Set command for GRAM data */
    ILI9341_SendCommand(ILI9341_GRAM);

    /* Send everything */
    ILI9341_CS_RESET();
    ILI9341_WRX_SET();

    /* Go to 16-bit SPI mode */
    TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);

    /* Send data */
    for (uint16_t x = 0; x <= ILI9341_WIDTH; x++)
    {
        for (uint16_t y = 0; y <= ILI9341_HEIGHT; y++)
        {
            pixel_state_t pixel_state = get_bit(pixels, x + y * ILI9341_WIDTH);
            pixel_state_t last_pixel_state = get_bit(last_pixels, x + y * ILI9341_WIDTH);
            if (pixel_state != last_pixel_state)
            {
                uint8_t datas[2];
                uint8_t new_color = (pixel_state == PIXEL_ON) ? DRAWING_COLOR : BACKGROUND_COLOR;
                datas[1] = HIGHINT(new_color);
                datas[0] = LOWINT(new_color);
                SPI_WriteMultiNoRegister(ILI9341_SPI, datas, 1);
                set_bit(last_pixels, x + y * ILI9341_WIDTH, pixel_state);
            }
        }
    }
}

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  pixel_state: New state of pixel
 * @retval None
 */
inline void OPTFT_DrawPixel(uint16_t x, uint16_t y, pixel_state_t pixel_state)
{
    set_bit(pixels, x + y * ILI9341_WIDTH, true);
}

/**
 * @brief  Fills chosen area with a pixel state
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  pixel_state: state to be used in fill
 * @retval None
 */
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_state_t pixel_state)
{
    for (uint16_t x = x0; x <= x1; x++)
    {
        for (uint16_t y = y0; y <= y1; y++)
        {
            OPTFT_DrawPixel(x, y, pixel_state);
        }
    }
}

/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  pixel_state: Line pixel state
 * @retval None
 */
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_state_t pixel_state)
{
    //WindowMax();
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;


    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) OPTFT_Fill(x0, y0, x0, y1, pixel_state);
        else OPTFT_Fill(x0, y1, x0, y0, pixel_state);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) OPTFT_Fill(x0, y0, x1, y0, pixel_state);
        else  OPTFT_Fill(x1, y0, x0, y0, pixel_state);
        return;
    }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

        	OPTFT_DrawPixel(x0, y0, pixel_state);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        OPTFT_DrawPixel(x0, y0, pixel_state);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
        	OPTFT_DrawPixel(x0, y0, pixel_state);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        OPTFT_DrawPixel(x0, y0, pixel_state);
    }
    return;
}
