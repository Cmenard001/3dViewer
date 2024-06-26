#ifndef __GEOMETRY_UNITY_H__
#define __GEOMETRY_UNITY_H__

#include <stdint.h>

typedef int16_t dist_t;
typedef uint16_t angle_t;

/**
 * @brief Macro d�fissant la valeur de PI en entier
 * 0pi = 0
 * pi = (2^16-1)/2
 * En dehors de la plage, on a un overflow donc �a fait un modulo
 *
 */
#define PI_INT ((1<<(8*sizeof(angle_t)))/2)

/**
 * @brief Quantum pour cos et sin. Equivalent � 1
 *
 */
#define QUANTUM 100


#endif
