#include "print_drawing.h"

#include "../LCD/low layer/ili9341.h"

#include <malloc.h>

#define OPTIMIZE_TFT
#ifdef OPTIMIZE_TFT
    #include "../optimized_tft/optimized_tft.h"
#endif

#define BACKGROUND_COLOR ILI9341_COLOR_WHITE
#define DRAWING_COLOR ILI9341_COLOR_BLACK

/**
 * @brief Last drawing printed
 *
 */
void draw_init()
{
    ILI9341_init();
#ifdef OPTIMIZE_TFT
    OPTFT_init(BACKGROUND_COLOR);
#else
    ILI9341_fill(BACKGROUND_COLOR);
#endif
}

void print_drawing(drawing_two_dims_t *drawing)
{
    static drawing_two_dims_t last_drawing = (drawing_two_dims_t){.segment = NULL, .nb_segment = 0};

    // on efface, l'ancien dessin
    for(uint16_t i = 0; i < last_drawing.nb_segment; i++)
    {
#ifdef OPTIMIZE_TFT
        OPTFT_DrawLine( last_drawing.segment[i].p1.x,
                        last_drawing.segment[i].p1.y,
                        last_drawing.segment[i].p2.x,
                        last_drawing.segment[i].p2.y,
                        BACKGROUND_COLOR);
#else
        ILI9341_drawLine(   last_drawing.segment[i].p1.x,
                            ILI9341_WIDTH - last_drawing.segment[i].p1.y,
                            last_drawing.segment[i].p2.x,
                            ILI9341_WIDTH - last_drawing.segment[i].p2.y,
                            BACKGROUND_COLOR);
#endif
    }

    // on dessine le nouveau dessin
    for(uint16_t i = 0; i < drawing->nb_segment; i++)
    {
#ifdef OPTIMIZE_TFT
        OPTFT_DrawLine( drawing->segment[i].p1.x,
                        drawing->segment[i].p1.y,
                        drawing->segment[i].p2.x,
                        drawing->segment[i].p2.y,
                        DRAWING_COLOR);
#else
        ILI9341_drawLine(   drawing->segment[i].p1.x,
                            ILI9341_WIDTH - drawing->segment[i].p1.y,
                            drawing->segment[i].p2.x,
                            ILI9341_WIDTH - drawing->segment[i].p2.y,
                            DRAWING_COLOR);

#endif
    }

#ifdef OPTIMIZE_TFT
    // on met à jour l'écran
    OPTFT_refresh();
#endif

    last_drawing.nb_segment = drawing->nb_segment;
    if (last_drawing.segment != NULL)
    {
        free(last_drawing.segment);
    }
    last_drawing.segment = (segment_two_dims_t *)malloc(last_drawing.nb_segment * sizeof(segment_two_dims_t));
    for(uint16_t i = 0; i < last_drawing.nb_segment; i++)
    {
        last_drawing.segment[i] = drawing->segment[i];
    }
}
