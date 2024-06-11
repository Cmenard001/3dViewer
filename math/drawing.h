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

bool is_drawing_changed();

/**
 * @brief Get a drawing object
 *
 * @param drawing_type drawing you want to get
 */
void get_drawing(drawing_type_t drawing_type, drawing_three_dims_t *drawing);

/**
 * @brief Increment the current drawing object
 *
 */
void increment_current_drawing();

/**
 * @brief Get the current drawing object
 *
 * @param drawing_type drawing you want to get
 */
void get_current_drawing(drawing_three_dims_t *drawing);

#endif
