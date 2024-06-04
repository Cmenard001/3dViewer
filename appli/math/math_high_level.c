#include "math_high_level.h"

#include <stdlib.h>
#include <stdbool.h>

#include "geometry_unity.h"
#include "geometry.h"
#include "drawing.h"
#include "rotate.h"
#include "projection.h"
#include "resize_drawing.h"
#include "print_drawing.h"
#include "../../lib/bsp/tft_ili9341/stm32f1_ili9341.h"
#include "../../lib/bsp/tft_ili9341/stm32f1_fonts.h"
#include "../ihm/viewer.h"

void math_init()
{
    // on initialise l'écan
    draw_init();
}

void math_process_main()
{
    // on récupère le dessin à dessiner
    static drawing_three_dims_t *original_drawing;
    bool drawing_changed = is_drawing_changed();
    if (drawing_changed)
    {
        original_drawing = get_current_drawing();
    }

    // on en fait une copie
    drawing_three_dims_t drawing;
    drawing.nb_segment = original_drawing->nb_segment;
    drawing.segment = malloc(drawing.nb_segment * sizeof(segment_three_dims_t));
    for (uint16_t i = 0; i < drawing.nb_segment; i++)
    {
        drawing.segment[i] = original_drawing->segment[i];
    }

    // on recupère l'orientation dans laquelle on doit mettre le dessin
    polar_coord_three_dims_t polar_coord_three_dims;
    get_polar_coord(&polar_coord_three_dims);
    angle_three_dims_t angle = (angle_three_dims_t){.angle_x = (polar_coord_three_dims.beta),
                                                    .angle_y = (polar_coord_three_dims.alpha),
                                                    .angle_z = 0};

    // on affiche les angles obtenus
    char str[25];
    sprintf(str, "%d - %d            ", angle.angle_x, angle.angle_y);
    ILI9341_Puts(1, 1, str, &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    static bool first_turn = true;
    static angle_three_dims_t last_angle;
    if (   last_angle.angle_x != angle.angle_x
        || last_angle.angle_y != angle.angle_y
        || last_angle.angle_z != angle.angle_z
        || drawing_changed
        || first_turn)
    {
        first_turn = false;
        last_angle = angle;
        // on tourne le dessin dans l'orientation souhaitée
        rotate(&drawing, angle);

        // on projette le dessin sur un plan
        drawing_two_dims_t *drawing_2d;
        drawing_2d = projection_orthogonal(&drawing);

        // on redimensionne le dessin pour qu'il apparaisse à la bonne taille et à la bonne place
        resize_drawing(drawing_2d, ILI9341_WIDTH);

        // on dessine le dessin
        print_drawing(drawing_2d);

        // on libère la mémoire
        free(drawing_2d->segment);
    }
    free(drawing.segment);
}
