#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "geometry.h"

/**
 * @brief Fonction de projection orthogonale d'un dessin en 3D sur un dessin en 2D
 *
 * @param drawing 3D drawing to project
 * @param drawing_2d 2D drawing to project on, will be filled with the result
 */
void projection_orthogonal(drawing_three_dims_t *drawing, drawing_two_dims_t *drawing_2d);


#endif
