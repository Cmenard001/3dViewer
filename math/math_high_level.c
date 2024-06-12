#include "math_high_level.h"

#include <stdlib.h>
#include <stdbool.h>

#include "geometry_unity.h"
#include "geometry.h"
#include "drawing.h"
#include "rotate.h"
#include "resize_drawing.h"
#include "print_drawing.h"
#include "../LCD/low layer/ili9341.h"
#include "../LCD/low layer/fonts.h"
#include "../ihm/viewer.h"
#include <assert.h>

void math_init()
{
    // on initialise l'écan
    draw_init();
}

void math_process_main()
{
    static drawing_three_dims_t *original_drawing;
    // on récupère le dessin à dessiner
    bool drawing_changed = is_drawing_changed();
    if (drawing_changed)
    {
        // on récupère le dessin
        original_drawing = get_current_drawing();
    }

    // on recupère l'orientation dans laquelle on doit tourner le dessin
    polar_coord_three_dims_t polar_coord_three_dims;
    get_polar_coord(&polar_coord_three_dims);
    angle_three_dims_t angle = (angle_three_dims_t){.angle_x = (polar_coord_three_dims.beta),
                                                    .angle_y = 0,
                                                    .angle_z = (polar_coord_three_dims.alpha)};


    static angle_three_dims_t last_angle;
    if (   last_angle.angle_x != angle.angle_x
        || last_angle.angle_y != angle.angle_y
        || last_angle.angle_z != angle.angle_z
        || drawing_changed)
    {
        last_angle = angle;

        // on fait une copie du dessin
        drawing_three_dims_t drawing;
        drawing = (drawing_three_dims_t)
        {
            .segment = malloc(original_drawing->nb_segment * sizeof(segment_three_dims_t)),
            .nb_segment = original_drawing->nb_segment
        };
        assert(drawing.segment != NULL);
        for (uint16_t i = 0; i < original_drawing->nb_segment; i++)
        {
            drawing.segment[i] = original_drawing->segment[i];
        }

        // on tourne le dessin dans l'orientation souhaitée
        rotate(&drawing, angle);

        // on redimensionne le dessin pour qu'il apparaisse à la bonne taille
        resize_drawing(&drawing, ILI9341_WIDTH);

        // on dessine le dessin
        print_drawing(&drawing);

        // on libère la mémoire
        free(drawing.segment);
    }
}
