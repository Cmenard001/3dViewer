#include "drawing.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "geometry.h"

/**
 * @brief Simple cube drawing
 *
 */
const drawing_three_dims_t cube =
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
const drawing_three_dims_t house =
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
void get_drawing(drawing_type_t drawing_type, drawing_three_dims_t *drawing)
{
    drawing_three_dims_t *original_drawing;
    switch(drawing_type)
    {
        case DRAWING_CUBE:
            original_drawing =  &cube;
            break;

        case DRAWING_HOUSE:
            original_drawing = &house;
            break;

        default:
            break;
    }

    if (drawing->segment == NULL)
    {
        drawing->nb_segment = original_drawing->nb_segment;
        // on alloue la mémoire pour le premier dessin
        drawing->segment = (segment_three_dims_t *)malloc(original_drawing->nb_segment * sizeof(segment_three_dims_t));
    }
    else if (drawing->nb_segment != original_drawing->nb_segment)
    {
        // on met à jour le nombre de segments si il a changé
        drawing->nb_segment = original_drawing->nb_segment;
        // on désalloue la mémoire
        free(drawing->segment);
        // on réalloue la mémoire pour le nouveau dessin
        drawing->segment = (segment_three_dims_t *)malloc(drawing->nb_segment * sizeof(segment_three_dims_t));
    }
    // on copie les segments
    for (int i = 0; i < drawing->nb_segment; i++)
    {
        drawing->segment[i] = original_drawing->segment[i];
    }
}

/**
 * @brief Get the current drawing object
 *
 * @param drawing
 */
void get_current_drawing(drawing_three_dims_t *drawing)
{
    get_drawing(current_drawing, drawing);
}
