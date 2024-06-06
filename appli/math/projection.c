#include "projection.h"

#include "geometry.h"

#include <stdint.h>
#include <stdlib.h>

drawing_two_dims_t *projection_orthogonal(drawing_three_dims_t *drawing)
{
    static drawing_two_dims_t drawing_2d;
    drawing_2d.nb_segment = drawing->nb_segment;
    drawing_2d.segment = (segment_two_dims_t*)malloc(drawing->nb_segment * sizeof(segment_two_dims_t));
    // on garde uniquement les coordonnées x et z des segments
    for(uint16_t i = 0; i < drawing_2d.nb_segment; i++)
    {
        drawing_2d.segment[i].p1.x = drawing->segment[i].p1.x;
        drawing_2d.segment[i].p1.y = drawing->segment[i].p1.z;
        drawing_2d.segment[i].p2.x = drawing->segment[i].p2.x;
        drawing_2d.segment[i].p2.y = drawing->segment[i].p2.z;
    }
    return &drawing_2d;
}
