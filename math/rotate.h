#ifndef __ROTATE_H__
#define __ROTATE_H__

#include "geometry.h"

/**
 * @brief Fonction pour effectuer une rotation sur un dessin en 3D
 *
 * @param drawing Dessin à tourner, il va donc être modifié
 * @param angle Angle de rotation en 3D
 */
void rotate(drawing_three_dims_t *drawing, angle_three_dims_t angle);


#endif
