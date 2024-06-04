#include "print_drawing.h"

#include "../../lib/bsp/tft_ili9341/stm32f1_ili9341.h"

#define BACKGROUND_COLOR ILI9341_COLOR_WHITE
#define DRAWING_COLOR ILI9341_COLOR_BLACK

/**
 * @brief Last drawing printed
 *
 */

void draw_init()
{
    ILI9341_Init();
    ILI9341_Rotate(3);
    ILI9341_Fill(BACKGROUND_COLOR);
}

void print_drawing(drawing_two_dims_t *drawing)
{
    static drawing_two_dims_t last_drawing = (drawing_two_dims_t){.segment = NULL, .nb_segment = 0};
    // on efface, l'ancien dessin
    for(uint16_t i = 0; i < last_drawing.nb_segment; i++)
    {
        ILI9341_DrawLine(   last_drawing.segment[i].p1.x,
                            last_drawing.segment[i].p1.y,
                            last_drawing.segment[i].p2.x,
                            last_drawing.segment[i].p2.y,
                            BACKGROUND_COLOR);
    }

    // on dessine le nouveau dessin
    last_drawing = *drawing;
    for(uint16_t i = 0; i < drawing->nb_segment; i++)
    {
        ILI9341_DrawLine(   drawing->segment[i].p1.x,
                            drawing->segment[i].p1.y,
                            drawing->segment[i].p2.x,
                            drawing->segment[i].p2.y,
                            DRAWING_COLOR);
    }
}
