#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdint.h>
#include "geometry_unity.h"

typedef struct
{
    dist_t x;
    dist_t y;
} point_two_dims_t;

typedef struct
{
    dist_t x;
    dist_t y;
    dist_t z;
} point_three_dims_t;

typedef struct
{
    angle_t angle_x;
    angle_t angle_y;
    angle_t angle_z;
} angle_three_dims_t;

typedef struct
{
    point_two_dims_t p1;
    point_two_dims_t p2;
} segment_two_dims_t;

typedef struct
{
    point_three_dims_t p1;
    point_three_dims_t p2;
} segment_three_dims_t;

typedef struct
{
    segment_three_dims_t *segment;
    uint16_t nb_segment;
} drawing_three_dims_t;

typedef struct
{
    segment_two_dims_t *segment;
    uint16_t nb_segment;
} drawing_two_dims_t;



typedef struct
{
    angle_t alpha;
    angle_t beta;
} polar_coord_three_dims_t;


#endif
