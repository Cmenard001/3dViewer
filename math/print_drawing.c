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
#ifdef OPTIMIZE_TFT
    OPTFT_init(BACKGROUND_COLOR);
#else
    ILI9341_init();
    ILI9341_fill(BACKGROUND_COLOR);
#endif
}

segment_two_dims_t three_dims_segments_to_screen(segment_three_dims_t segment)
{

    // on projette le point sur un plan (l'écran)
    segment_two_dims_t two_dim_segment = PROJECT_SEGMENT(segment);
    // on translatte le dessin pour qu'il soit centré
    two_dim_segment.p1.x += ILI9341_WIDTH / 2;
    two_dim_segment.p1.y += ILI9341_WIDTH / 2;
    two_dim_segment.p2.x += ILI9341_WIDTH / 2;
    two_dim_segment.p2.y += ILI9341_WIDTH / 2;
    return two_dim_segment;
}

void print_drawing(drawing_three_dims_t *drawing)
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
                            last_drawing.segment[i].p1.y,
                            last_drawing.segment[i].p2.x,
                            last_drawing.segment[i].p2.y,
                            BACKGROUND_COLOR);
#endif
    }

    // on dessine le nouveau dessin
    for(uint16_t i = 0; i < drawing->nb_segment; i++)
    {
        segment_two_dims_t two_dim_segment = three_dims_segments_to_screen(drawing->segment[i]);
        // on dessine le segment
#ifdef OPTIMIZE_TFT
        OPTFT_DrawLine( two_dim_segment.p1.x,
                        two_dim_segment.p1.y,
                        two_dim_segment.p2.x,
                        two_dim_segment.p2.y,
                        DRAWING_COLOR);
#else
        ILI9341_drawLine(   two_dim_segment.p1.x,
                            two_dim_segment.p1.y,
                            two_dim_segment.p2.x,
                            two_dim_segment.p2.y,
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
        last_drawing.segment[i] = three_dims_segments_to_screen(drawing->segment[i]);
    }
}
