#ifndef __PRINT_DRAWING_H__
#define __PRINT_DRAWING_H__

#include "geometry.h"

/**
 * @brief Fonction d'initialisation de l'affichage
 *
 */
void draw_init();

/**
 * @brief Fonction pour afficher un dessin en 2d sur l'écran
 *
 * @param drawing dessin à afficher
 */
void print_drawing(drawing_three_dims_t *drawing);



#endif
