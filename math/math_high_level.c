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
#include "../LCD/low layer/ili9341.h"
#include "../LCD/low layer/fonts.h"
#include "../ihm/viewer.h"

void math_init()
{
    // on initialise l'écan
    draw_init();
}

void math_process_main()
{
    static drawing_three_dims_t drawing = (drawing_three_dims_t){.nb_segment = 0, .segment = NULL};
    // on récupère le dessin à dessiner
    bool drawing_changed = is_drawing_changed();
    if (drawing_changed)
    {
        // on récupère le dessin
        get_current_drawing(&drawing);
    }

    // on recupère l'orientation dans laquelle on doit mettre le dessin
    polar_coord_three_dims_t polar_coord_three_dims;
    get_polar_coord(&polar_coord_three_dims);
    angle_three_dims_t angle = (angle_three_dims_t){.angle_x = (polar_coord_three_dims.beta),
                                                    .angle_y = 0,
                                                    .angle_z = (polar_coord_three_dims.alpha)};


    static bool first_turn = true;
    static angle_three_dims_t last_angle;
    //if (   last_angle.angle_x != angle.angle_x
    //    || last_angle.angle_y != angle.angle_y
    //    || last_angle.angle_z != angle.angle_z
    //    || drawing_changed
    //    || first_turn)
    {
        first_turn = false;
        last_angle = angle;
        // on tourne le dessin dans l'orientation souhaitée
        rotate(&drawing, angle);

        // on projette le dessin sur un plan
        static drawing_two_dims_t drawing_2d = (drawing_two_dims_t){.nb_segment = 0, .segment = NULL};
        projection_orthogonal(&drawing, &drawing_2d);

        // on redimensionne le dessin pour qu'il apparaisse à la bonne taille et à la bonne place
        resize_drawing(&drawing_2d, ILI9341_WIDTH);

        // on dessine le dessin
        print_drawing(&drawing_2d);
    }
}
