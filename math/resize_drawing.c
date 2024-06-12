#include "resize_drawing.h"

#include <math.h>

#include "geometry_unity.h"
#include "geometry.h"

#define DECAL 100

static dist_t dist_from_origin(point_two_dims_t point);
static dist_t dist_from_origin(point_two_dims_t point)
{
    return (dist_t)sqrt(point.x * point.x + point.y * point.y);
}

void resize_drawing(drawing_three_dims_t *drawing, dist_t width)
{
    // on cherche le point le plus éloigné de l'origine
    dist_t max_dist = 1; // on initialise à 1 pour éviter une division par 0
    for(int segment_index = 0; segment_index < drawing->nb_segment; segment_index++)
    {
        dist_t current_dist = dist_from_origin(PROJECT_SEGMENT(drawing->segment[segment_index]).p1);
        if (current_dist > max_dist)
        {
            max_dist = current_dist;
        }
        current_dist = dist_from_origin(PROJECT_SEGMENT(drawing->segment[segment_index]).p2);
        if (current_dist > max_dist)
        {
            max_dist = current_dist;
        }
    }
    // puis on calcule le rapport de façon à ce que le point le plus éloigné soit à la distance width de l'origine
    int32_t ratio = DECAL*width / (max_dist*2);
    // on applique le ratio à tous les points
    for(int segment_index = 0; segment_index < drawing->nb_segment; segment_index++)
    {
        drawing->segment[segment_index].p1.x = drawing->segment[segment_index].p1.x*ratio/DECAL;
        drawing->segment[segment_index].p1.y = drawing->segment[segment_index].p1.y*ratio/DECAL;
        drawing->segment[segment_index].p1.z = drawing->segment[segment_index].p1.z*ratio/DECAL;
        drawing->segment[segment_index].p2.x = drawing->segment[segment_index].p2.x*ratio/DECAL;
        drawing->segment[segment_index].p2.y = drawing->segment[segment_index].p2.y*ratio/DECAL;
        drawing->segment[segment_index].p2.z = drawing->segment[segment_index].p2.z*ratio/DECAL;
    }
}
