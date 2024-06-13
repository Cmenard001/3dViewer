#include "drawing.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "geometry.h"

/**
 * @brief Empty drawing
 *
 */
static drawing_three_dims_t empty_drawing = { .nb_segment = 0, .segment = NULL };

/**
 * @brief Simple cube drawing
 *
 */
static drawing_three_dims_t cube =
{
    .segment = (segment_three_dims_t[])
    {
        {
            .p1 = { .x = 1000, .y = -1000, .z = +1000 },
            .p2 = { .x = -1000, .y = -1000, .z = 1000 }
        },
        {
            .p1 = { .x = 1000, .y = 1000, .z = 1000 },
            .p2 = { .x = -1000, .y = 1000, .z = 1000 }
        },
        {
            .p1 = { .x = 1000, .y = 1000, .z = -1000 },
            .p2 = { .x = -1000, .y = 1000, .z = -1000 }
        },
        {
            .p1 = { .x = 1000, .y = -1000, .z = -1000 },
            .p2 = { .x = -1000, .y = -1000, .z = -1000 }
        },

        {
            .p1 = { .x = 1000, .y = -1000, .z = -1000 },
            .p2 = { .x = 1000, .y = 1000, .z = -1000 }
        },
        {
            .p1 = { .x = 1000, .y = -1000, .z = 1000 },
            .p2 = { .x = 1000, .y = 1000, .z = 1000 }
        },
        {
            .p1 = { .x = -1000, .y = -1000, .z = +1000 },
            .p2 = { .x = -1000, .y = 1000, .z = 1000 }
        },
        {
            .p1 = { .x = -1000, .y = -1000, .z = -1000 },
            .p2 = { .x = -1000, .y = 1000, .z = -1000 }
        },

        {
            .p1 = { .x = -1000, .y = 1000, .z = 1000 },
            .p2 = { .x = -1000, .y = 1000, .z = -1000 }
        },
        {
            .p1 = { .x = -1000, .y = -1000, .z = 1000 },
            .p2 = { .x = -1000, .y = -1000, .z = -1000 }
        },
        {
            .p1 = { .x = 1000, .y = -1000, .z = 1000 },
            .p2 = { .x = 1000, .y = -1000, .z = -1000 }
        },
        {
            .p1 = { .x = 1000, .y = 1000, .z = 1000 },
            .p2 = { .x = 1000, .y = 1000, .z = -1000 }
        }
    },
    .nb_segment = 12
};

/**
 * @brief House drawing
 *
 */
static drawing_three_dims_t house =
{
    .segment = (segment_three_dims_t[])
    {
        // cube
        {
            .p1 = { .x = 50, .y = -50, .z = +50 },
            .p2 = { .x = -50, .y = -50, .z = 50 }
        },
        {
            .p1 = { .x = 50, .y = 50, .z = 50 },
            .p2 = { .x = -50, .y = 50, .z = 50 }
        },
        {
            .p1 = { .x = 50, .y = 50, .z = -50 },
            .p2 = { .x = -50, .y = 50, .z = -50 }
        },
        {
            .p1 = { .x = 50, .y = -50, .z = -50 },
            .p2 = { .x = -50, .y = -50, .z = -50 }
        },

        {
            .p1 = { .x = 50, .y = -50, .z = -50 },
            .p2 = { .x = 50, .y = 50, .z = -50 }
        },
        {
            .p1 = { .x = 50, .y = -50, .z = 50 },
            .p2 = { .x = 50, .y = 50, .z = 50 }
        },
        {
            .p1 = { .x = -50, .y = -50, .z = +50 },
            .p2 = { .x = -50, .y = 50, .z = 50 }
        },
        {
            .p1 = { .x = -50, .y = -50, .z = -50 },
            .p2 = { .x = -50, .y = 50, .z = -50 }
        },

        {
            .p1 = { .x = -50, .y = 50, .z = 50 },
            .p2 = { .x = -50, .y = 50, .z = -50 }
        },
        {
            .p1 = { .x = -50, .y = -50, .z = 50 },
            .p2 = { .x = -50, .y = -50, .z = -50 }
        },
        {
            .p1 = { .x = 50, .y = -50, .z = 50 },
            .p2 = { .x = 50, .y = -50, .z = -50 }
        },
        {
            .p1 = { .x = 50, .y = 50, .z = 50 },
            .p2 = { .x = 50, .y = 50, .z = -50 }
        },

        // roof
        {
            .p1 = { .x = 50, .y = 50, .z = 50 },
            .p2 = { .x = 0, .y = 0, .z = 100 }
        },
        {
            .p1 = { .x = 50, .y = -50, .z = 50 },
            .p2 = { .x = 0, .y = 0, .z = 100 }
        },
        {
            .p1 = { .x = -50, .y = -50, .z = 50 },
            .p2 = { .x = 0, .y = 0, .z = 100 }
        },
        {
            .p1 = { .x = -50, .y = 50, .z = 50 },
            .p2 = { .x = 0, .y = 0, .z = 100 }
        },

        // door
        {
            .p1 = { .x = 40, .y = 50, .z = -50 },
            .p2 = { .x = 40, .y = 50, .z = 20 }
        },
        {
            .p1 = { .x = 40, .y = 50, .z = 20 },
            .p2 = { .x = 10, .y = 50, .z = 20 }
        },
        {
            .p1 = { .x = 10, .y = 50, .z = 20 },
            .p2 = { .x = 10, .y = 50, .z = -50 }
        },

        // window
        {
            .p1 = { .x = -10, .y = 50, .z = -20 },
            .p2 = { .x = -40, .y = 50, .z = -20 }
        },
        {
            .p1 = { .x = -40, .y = 50, .z = -20 },
            .p2 = { .x = -40, .y = 50, .z = 10 }
        },
        {
            .p1 = { .x = -40, .y = 50, .z = 10 },
            .p2 = { .x = -10, .y = 50, .z = 10 }
        },
        {
            .p1 = { .x = -10, .y = 50, .z = 10 },
            .p2 = { .x = -10, .y = 50, .z = -20 }
        },

        // back door
        {
            .p1 = { .x = -40, .y = -50, .z = -50 },
            .p2 = { .x = -40, .y = -50, .z = 20 }
        },
        {
            .p1 = { .x = -40, .y = -50, .z = 20 },
            .p2 = { .x = -10, .y = -50, .z = 20 }
        },
        {
            .p1 = { .x = -10, .y = -50, .z = 20 },
            .p2 = { .x = -10, .y = -50, .z = -50 }
        },

        // garden
        {
            .p1 = { .x = -50, .y = -50, .z = -50 },
            .p2 = { .x = -50, .y = -100, .z = -50 }
        },
        {
            .p1 = { .x = -50, .y = -100, .z = -50 },
            .p2 = { .x = 50, .y = -100, .z = -50 }
        },
        {
            .p1 = { .x = 50, .y = -100, .z = -50 },
            .p2 = { .x = 50, .y = -50, .z = -50 }
        },

        {
            .p1 = { .x = -50, .y = -50, .z = -30 },
            .p2 = { .x = -50, .y = -100, .z = -30 }
        },
        {
            .p1 = { .x = -50, .y = -100, .z = -30 },
            .p2 = { .x = 50, .y = -100, .z = -30 }
        },
        {
            .p1 = { .x = 50, .y = -100, .z = -30 },
            .p2 = { .x = 50, .y = -50, .z = -30 }
        },

        {
            .p1 = { .x = -50, .y = -100, .z = -50 },
            .p2 = { .x = -50, .y = -100, .z = -30 }
        },
        {
            .p1 = { .x = 50, .y = -100, .z = -50 },
            .p2 = { .x = 50, .y = -100, .z = -30 }
        },

        // balcony
        {
            .p1 = { .x = -10, .y = 50, .z = -20 },
            .p2 = { .x = -10, .y = 55, .z = -21 }
        },
        {
            .p1 = { .x = -10, .y = 55, .z = -21 },
            .p2 = { .x = -40, .y = 55, .z = -21 }
        },
        {
            .p1 = { .x = -40, .y = 55, .z = -21 },
            .p2 = { .x = -40, .y = 50, .z = -20 }
        },

    },
    .nb_segment = 37
};

// Variable globale à mettre à true si on change le dessin courant
static bool drawing_has_changed = true;
// Dessin courant
static drawing_type_t current_drawing = 0;

/**
 * @brief Fonction qui retourne si le dessin a changé
 *
 * @return true le dessin a changé
 * @return false le dessin n'a pas changé
 */
bool is_drawing_changed()
{
    bool last_drawing_has_changed = drawing_has_changed;
    drawing_has_changed = false;
    return last_drawing_has_changed;
}

/**
 * @brief Fonction pour incrémenter le dessin courant
 *
 */
void increment_current_drawing()
{
    drawing_has_changed = true;
    current_drawing = (current_drawing + 1) % DRAWING_COUNT;
}

/**
 * @brief Get the drawing object
 *
 * @param drawing_type The drawing you want to get
 * @param drawing The drawing object who will be filled
 */
drawing_three_dims_t *get_drawing(drawing_type_t drawing_type)
{
    drawing_three_dims_t *original_drawing;
    switch(drawing_type)
    {
        case DRAWING_CUBE:
            return &cube;

        case DRAWING_HOUSE:
            return &house;

        default:
            break;
    }
    return &empty_drawing;
}

/**
 * @brief Get the current drawing object
 *
 * @param drawing
 */
drawing_three_dims_t *get_current_drawing()
{
    return get_drawing(current_drawing);
}
