#ifndef __DRAWING_H__
#define __DRAWING_H__

#include "geometry.h"
#include <stdbool.h>

typedef enum
{
    DRAWING_CUBE = 0,
    DRAWING_HOUSE,
    DRAWING_COUNT
} drawing_type_t;

/**
 * @brief Fonction qui retourne si le dessin a chang�
 *
 * @return true le dessin a chang�
 * @return false le dessin n'a pas chang�
 */
bool is_drawing_changed();

/**
 * @brief Fonction pour incr�menter le dessin courant
 *
 */
void increment_current_drawing();

/**
 * @brief Get the drawing object
 *
 * @param drawing_type The drawing you want to get
 *
 * @return drawing_three_dims_t* The drawing object
 */
drawing_three_dims_t *get_drawing(drawing_type_t drawing_type);

/**
 * @brief Get the current drawing object
 *
 * @return drawing_three_dims_t* The current drawing object
 */
drawing_three_dims_t *get_current_drawing();

#endif
