#include "trigo.h"

#include <stdint.h>
#include "geometry_unity.h"
#include <math.h>

#define NB_COS 256

#define PI_FLOAT (3.14159265358979323846f)

#define ANGLE_TO_COSINDEX(angle) ((uint32_t)(NB_COS*angle/(2*PI_INT)))
#define COSINDEX_TO_ANGLE(cosindex) (angle_t)(2*PI_INT*cos_tab_index/NB_COS)

/**
 * @brief Tableau global de cosinus
 *
 */
dist_t cos_tab[NB_COS];


/**
 * @brief Fonction pour initialiser le tableau de cosinus
 * @todo Il serait mieux que le tableau de cosinus soit en flash
 *       pour éviter de le recalculer à chaque démarage
 *
 */
static void cos_init();
static void cos_init()
{
    for (uint32_t cos_tab_index=0 ; cos_tab_index < NB_COS ; cos_tab_index++)
    {
        cos_tab[cos_tab_index] = QUANTUM * cosf(2*PI_FLOAT*cos_tab_index/NB_COS);
    }
}

void trigo_init()
{
    cos_init();
}

dist_t cos_int(angle_t angle)
{
    return cos_tab[ANGLE_TO_COSINDEX(angle)];
}

dist_t sin_int(angle_t angle)
{
    return cos_int(angle - PI_INT/2);
}

dist_t tan_int(angle_t angle)
{
    return sin_int(angle)/cos_int(angle);
}


angle_t acos_int(dist_t dist)
{
    // On cherche l'index du cosinus le plus proche de la distance demandée
    for (uint32_t cos_tab_index=0 ; cos_tab_index < NB_COS ; cos_tab_index++)
    {
        if (cos_tab[cos_tab_index] > (dist%QUANTUM))
        {
            return COSINDEX_TO_ANGLE(cos_tab_index);
        }
    }
    // normalement on ne devrait jamais arriver ici
    return 0;
}

angle_t asin_int(dist_t dist)
{
    return (PI_INT/2 - acos_int(dist));
}

angle_t atan_int(dist_t dist)
{
    return (angle_t)asin_int(dist/sqrt(dist*dist + 1));
}
