#ifndef __PI_INT_H__
#define __PI_INT_H__

#include "geometry_unity.h"

/**
 * @brief Fonction d'initialisation des fonctions trigo
 *
 */
void trigo_init();

/**
 * @brief Fonctions pour obtenir cos, sin et tan d'un angle
 *
 * @param angle angle duquel obtenir le cosinus
 * @return dist_t la distance calculée
 */
dist_t cos_int(angle_t angle);
dist_t sin_int(angle_t angle);
dist_t tan_int(angle_t angle);

/**
 * @brief Fonctions reciproques de cos, sin et tan
 *
 * @param dist la distance duquel obtenir l'angle
 * @return angle_t l'angle calculé
 */
angle_t acos_int(dist_t dist);
angle_t asin_int(dist_t dist);
angle_t atan_int(dist_t dist);

#endif
