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
#include <stdbool.h>

#define COMPRESS_COLOR(color)		((uint8_t)((color > 0x7FFF) ? 0xFF : 0x00))
#define DECOMPRESS_COLOR(color)		((uint16_t)((color > 0x7F) ? 0xFFFF : 0x0000))

typedef enum
{
    PIXELS_TAB = 0,
    LAST_PIXELS_TAB
} pixels_tab_e;

typedef uint8_t compressed_color_t;

static compressed_color_t pixels[ILI9341_WIDTH][ILI9341_WIDTH];
static compressed_color_t last_pixels[ILI9341_WIDTH][ILI9341_WIDTH];

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
    if (x < 0 || y < 0 || x >= ILI9341_WIDTH || y >= ILI9341_WIDTH)
    {
        return 0;
    }
    switch (pixel_tab)
    {
    case PIXELS_TAB:
        return pixels[x][y];
        break;

    case LAST_PIXELS_TAB:
        return last_pixels[x][y];
        break;

    default:
        return 0;
        break;
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
    if (x < 0 || y < 0 || x >= ILI9341_WIDTH || y >= ILI9341_WIDTH)
    {
        return;
    }
    switch (pixel_tab)
    {
    case PIXELS_TAB:
        pixels[x][y] = color;
        break;

    case LAST_PIXELS_TAB:
        last_pixels[x][y] = color;
        break;

    default:
        break;
    }
}

void OPTFT_init(uint16_t background_color)
{
    // On initialise l'écran
    ILI9341_init();
    // On remplit l'écran avec la couleur de fond
    ILI9341_fill(background_color);
    // On initialise les tableaux de pixels
    for (uint16_t x = 0; x < ILI9341_WIDTH; x++)
    {
        for (uint16_t y = 0; y < ILI9341_WIDTH; y++)
        {
            set_pixel(x, y, PIXELS_TAB, COMPRESS_COLOR(background_color));
            set_pixel(x, y, LAST_PIXELS_TAB, COMPRESS_COLOR(background_color));
        }
    }
}

/**
 * @brief  Refresh the screen with the new pixels stored in the pixels_tab
 * @retval None
 */
void OPTFT_refresh()
{
    /* Send data */
    for (uint16_t y = 0; y < ILI9341_WIDTH; y++)
    {
        uint16_t x0 = 0;
        uint16_t x1 = 0;
        bool is_line = false;
        bool end_line = false;
        compressed_color_t current_line_color = 0;
        for (uint16_t x = 0; x < ILI9341_WIDTH; x++)
        {
            // On récupère la couleur du pixel actuel
            compressed_color_t current_pixel_color = get_pixel(x, y, PIXELS_TAB);
            if (current_pixel_color == 0)
            {
            	for(int i = 0;i < 1;i++);
            }

            if (current_pixel_color != get_pixel(x, y, LAST_PIXELS_TAB))
            {
                // Le pixel doit être mis à jour
                x1 = x;
                if (!is_line)
                {
                    // On commence une nouvelle ligne
                    is_line = true;
                    current_line_color = current_pixel_color;
                    x0 = x;
                }
                if (current_line_color != current_pixel_color)
                {
                    // La couleur du pixel est différente de la couleur de la ligne
                    // Donc c'est une nouvelle ligne
                    // On dessine la ligne précédente
                    ILI9341_drawFilledRectangle(x0, y, x1, y, DECOMPRESS_COLOR(current_line_color));
                    // On commence une nouvelle ligne
                    is_line = true;
                    current_line_color = current_pixel_color;
                    x0 = x;
                }
            }
            else if(is_line)
            {
                // On a fini une ligne
                // Donc on la dessine
                ILI9341_drawFilledRectangle(x0, y, x1, y, DECOMPRESS_COLOR(current_line_color));
                is_line = false;
            }

            // On met à jour le tableau des derniers pixels
            set_pixel(x, y, LAST_PIXELS_TAB, current_pixel_color);
        }
        // fin de la ligne, on dessine la dernière ligne si besoin
        if (is_line)
        {
            ILI9341_drawFilledRectangle(x0, y, x1, y, DECOMPRESS_COLOR(get_pixel(ILI9341_WIDTH-1, y, PIXELS_TAB)));
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
