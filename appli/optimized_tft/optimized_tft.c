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

#define HIGHINT(x)				((uint8_t)(((x) >> 8) & 0xFF))
#define LOWINT(x)				((uint8_t)((x) & 0xFF))

#define ILI9341_GRAM				0x2C
#define ILI9341_CS_RESET()			HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, 0)
#define ILI9341_WRX_SET()			HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, 1)

typedef struct
{
    uint16_t color;
} pixel_t;

static pixel_t pixels_tab[ILI9341_WIDTH][ILI9341_HEIGHT];
static pixel_t last_pixels_tab[ILI9341_WIDTH][ILI9341_HEIGHT];

/**
 * @brief  Refresh the screen with the new pixels stored in the pixels_tab
 * @retval None
 */
void OPTFT_refresh()
{
    /* Send data */
    for (uint16_t x = 0; x <= ILI9341_WIDTH; x++)
    {
        for (uint16_t y = 0; y <= ILI9341_HEIGHT; y++)
        {
            if (pixels_tab[x][y].color != last_pixels_tab[x][y].color)
            {
                ILI9341_DrawPixel(x, y, pixels_tab[x][y].color)
            }
        }
    }
}

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  color: Color of pixel
 * @retval None
 */
inline void OPTFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	pixels_tab[x][y].color = color;
}

/**
 * @brief  Fills chosen area with a color
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Color to be used in fill
 * @retval None
 */
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    for (uint16_t x = x0; x <= x1; x++)
    {
        for (uint16_t y = y0; y <= y1; y++)
        {
            OPTFT_DrawPixel(x, y, color);
        }
    }
}

/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  color: Line color
 * @retval None
 */
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    //WindowMax();
    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;


    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) OPTFT_Fill(x0, y0, x0, y1, color);
        else OPTFT_Fill(x0, y1, x0, y0, color);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) OPTFT_Fill(x0, y0, x1, y0, color);
        else  OPTFT_Fill(x1, y0, x0, y0, color);
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

        	OPTFT_DrawPixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        OPTFT_DrawPixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
        	OPTFT_DrawPixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        OPTFT_DrawPixel(x0, y0, color);
    }
    return;
}
