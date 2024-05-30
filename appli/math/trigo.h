#ifndef __PI_INT_H__
#define __PI_INT_H__

#include "geometry_unity.h"

/**
 * @brief Fonction d'initialisation des fonctions trigo
 *
 */
void trigo_init();

/**
 * @brief Fonctions pour obtenir le cosinus et le sinus d'un angle
 *
 * @param angle angle duquel obtenir le cosinus
 * @return dist_t la distance calculée
 */
inline dist_t cos_int(angle_t angle);
inline dist_t sin_int(angle_t angle);


#endif
