#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdint.h>
#include "coors.h"

typedef struct
{
    coord_t x;
    coord_t y;
} point_two_dims_t;


typedef struct
{
    coord_t x;
    coord_t y;
    coord_t z;
} point_three_dims_t;

typedef struct
{
    point_two_dims_t origin_point;
    angle_t origin_angle;
} frame_two_dims_t;


typedef struct
{
    point_three_dims_t origin_point;
    angle_t origin_alpha;
    angle_t origin_beta_angle;
} frame_three_dims_t;

#endif
