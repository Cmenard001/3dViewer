#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdint.h>
#include "geometry_unity.h.h"

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
