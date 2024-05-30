#ifndef __GEOMETRY_UNITY_H__
#define __GEOMETRY_UNITY_H__

#include <stdint.h>

typedef int16_t dist_t;
typedef uint16_t angle_t;

/**
 * @brief Macro défissant la valeur de PI en entier
 * 0pi = 0
 * pi = 2^16/2
 * En dehors de la plage, on a un overflow donc ça fait un modulo
 *
 */
#define PI_INT ((1<<sizeof(angle_t))/2)

#define QUANTUM 100


#endif
