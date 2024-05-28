#ifndef __COORD_H__
#define __COORD_H__


typedef int16_t coord_t;
typedef int16_t angle_t;

/**
 * @brief Macro d�fissant la valeur de PI en entier
 * 0pi = 0
 * pi = 2^16/2
 * -pi = -2^16/2
 * En dehors de la plage, on a un overflow donc �a fait un modulo
 *
 */
#define PI_INT ((1<<sizeof(angle_t))/2);


#endif
