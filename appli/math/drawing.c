#include "drawing.h"

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

    },
    .nb_segment = 0
};

static drawing_type_t current_drawing = 0;

void increment_current_drawing()
{
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
