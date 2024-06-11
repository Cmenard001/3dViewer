/**
 * @file optimized_tft.c
 * @author Cyprien Ménard
 * @brief Fichier de gestion de l'affichage optimisé sur l'écran TFT
 * @note Ce fichier est partiellement inspiré de "stm32f1_ili9341.c"
 * @date 2024-06-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "optimized_tft.h"

#include <stdint.h>

#include "../LCD/low layer/ili9341.h"
#include "../spi/spi.h"
#include "../stm32f4xx/stm32f4xx_gpio.h"

#include <stdint.h>

#define COMPRESS_COLOR(color)		((compressed_color_t)(((color >> 8) & 0xF8) | ((color >> 5) & 0x07)))
#define DECOMPRESS_COLOR(color)		((uint16_t)(((color & 0xF8) << 8) | ((color & 0x07) << 5)))

#define SIZE_TAB (2*ILI9341_WIDTH*ILI9341_HEIGHT)
#define CCM_RAM_SIZE 65535
#define EXCEEDING_CCM_RAM_SIZE (SIZE_TAB-CCM_RAM_SIZE)

#define INDEX(x, y, pixel_tab) (pixel_tab*ILI9341_HEIGHT*ILI9341_WIDTH + x*ILI9341_HEIGHT + y)

typedef enum
{
    LAST_PIXELS_TAB = 0,
    PIXELS_TAB
} pixels_tab_e;

typedef uint8_t compressed_color_t;

// tableaux à stocker dans la CCM et dans la RAM pour l'excédent
__attribute__((section(".ccm"))) compressed_color_t ccm_ram[CCM_RAM_SIZE/sizeof(compressed_color_t)];
compressed_color_t exceeding_ccm_ram[(EXCEEDING_CCM_RAM_SIZE)/sizeof(compressed_color_t)];

/**
 * @brief Accesseur du tableau "magique" de pixels
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 *
 * @return compressed_color_t Couleur du pixel
 *
 */
compressed_color_t get_pixel(uint16_t x, uint16_t y, pixels_tab_e pixel_tab)
{
    uint32_t index = INDEX(x, y, pixel_tab);
    if (index < CCM_RAM_SIZE)
    {
        return ccm_ram[index];
    }
    else
    {
        return exceeding_ccm_ram[index - CCM_RAM_SIZE];
    }
}

/**
 * @brief Fonction pour écrire dans le tableau "magique" de pixels
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 * @param color Couleur du pixel
 *
 */
void set_pixel(uint16_t x, uint16_t y, pixels_tab_e pixel_tab, compressed_color_t color)
{
    uint32_t index = INDEX(x, y, pixel_tab);
    if (index < CCM_RAM_SIZE)
    {
        ccm_ram[index] = color;
    }
    else
    {
        exceeding_ccm_ram[index - CCM_RAM_SIZE] = color;
    }
}


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
            if (get_pixel(x, y, PIXELS_TAB) != get_pixel(x, y, LAST_PIXELS_TAB))
            {
                ILI9341_drawPixel(x, y, DECOMPRESS_COLOR(get_pixel(x, y, PIXELS_TAB)));
                // On met à jour le tableau des derniers pixels
                set_pixel(x, y, LAST_PIXELS_TAB, get_pixel(x, y, PIXELS_TAB));
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
    set_pixel(x, y, PIXELS_TAB, COMPRESS_COLOR(color));
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
