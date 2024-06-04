#include "math_high_level.h"

#include <stdlib.h>

#include "geometry_unity.h"
#include "geometry.h"
#include "drawing.h"
#include "rotate.h"
#include "projection.h"
#include "resize_drawing.h"
#include "print_drawing.h"
#include "../../lib/bsp/tft_ili9341/stm32f1_ili9341.h"

void math_init()
{
    // on initialise l'écan
    draw_init();
}

void math_process_main()
{
    // on récupère le dessin à dessiner
    drawing_three_dims_t *drawing;
    drawing = get_drawing(&drawing);

    // on recupère l'orientation dans laquelle on doit mettre le dessin
    angle_three_dims_t angle = (angle_three_dims_t){ .angle_x = PI_INT/3, .angle_y = PI_INT/3, .angle_z = 0 };

    // on tourne le dessin dans l'orientation souhaitée
    rotate(drawing, angle);

    // on projette le dessin sur un plan
    drawing_two_dims_t *drawing_2d;
    drawing_2d = projection_orthogonal(drawing);

    // on redimensionne le dessin pour qu'il apparaisse à la bonne taille et à la bonne place
    resize_drawing(drawing_2d, ILI9341_WIDTH);

    // on dessine le dessin
    print_drawing(drawing_2d);

    // on libère la mémoire
    free(drawing_2d);
}
