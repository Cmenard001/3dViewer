#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <stdint.h>
#include "geometry_unity.h"

/**
 * @brief Point en 2 dimensions
 *
 */
typedef struct
{
    dist_t x;
    dist_t y;
} point_two_dims_t;

/**
 * @brief Point en 3 dimensions
 *
 */
typedef struct
{
    dist_t x;
    dist_t y;
    dist_t z;
} point_three_dims_t;

/**
 * @brief Angle en 3 dimensions
 *
 */
typedef struct
{
    angle_t angle_x;
    angle_t angle_y;
    angle_t angle_z;
} angle_three_dims_t;

/**
 * @brief Segment en 2 dimensions
 *
 */
typedef struct
{
    point_two_dims_t p1;
    point_two_dims_t p2;
} segment_two_dims_t;

/**
 * @brief Segment en 3 dimensions
 *
 */
typedef struct
{
    point_three_dims_t p1;
    point_three_dims_t p2;
} segment_three_dims_t;

/**
 * @brief Dessin en 3 dimensions
 *
 */
typedef struct
{
    segment_three_dims_t *segment;
    uint16_t nb_segment;
} drawing_three_dims_t;

/**
 * @brief Dessin en 2 dimensions
 *
 */
typedef struct
{
    segment_two_dims_t *segment;
    uint16_t nb_segment;
} drawing_two_dims_t;

/**
 * @brief Coordonnées polaires en 3 dimensions (il manque gamma mais on l'utilise pas)
 *
 */
typedef struct
{
    angle_t alpha;
    angle_t beta;
} polar_coord_three_dims_t;


#define TWO_D_PROJECT_X x
#define TWO_D_PROJECT_Y z

#define PROJECT_POINT(point) (point_two_dims_t){.x = point.TWO_D_PROJECT_X, .y = point.TWO_D_PROJECT_Y}
#define PROJECT_SEGMENT(segment) (segment_two_dims_t){.p1 = PROJECT_POINT(segment.p1), .p2 = PROJECT_POINT(segment.p2)}

#endif
