#include "drawing.h"

#include <stdbool.h>
#include <stddef.h>

#include "geometry.h"

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


    },
    .nb_segment = 26
};

static bool drawing_has_changed = true;
static drawing_type_t current_drawing = 0;

bool is_drawing_changed()
{
    bool last_drawing_has_changed = drawing_has_changed;
    drawing_has_changed = false;
    return last_drawing_has_changed;
}

void increment_current_drawing()
{
    drawing_has_changed = true;
    current_drawing = (current_drawing + 1) % DRAWING_COUNT;
}

drawing_three_dims_t *get_drawing(drawing_type_t drawing_type)
{
    switch(drawing_type)
    {
        case DRAWING_CUBE:
            return &cube;
        case DRAWING_HOUSE:
            return &house;
        default:
            return NULL;
    }
    return NULL;
}

drawing_three_dims_t *get_current_drawing()
{
    return get_drawing(current_drawing);
}
