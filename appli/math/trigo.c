#include "trigo.h"

#include <stdint.h>
#include <math.h>

#define NB_COS 256

#define ANGLE_TO_COSINDEX(angle) (NB_COS*angle/(2*PI_INT))
#define COSINDEX_TO_ANGLE(cosindex) (2*PI_INT*cos_tab_index/NB_COS)

/**
 * @brief Tableau global de cosinus
 *
 */
dist_t cos_tab[NB_COS];


/**
 * @brief Fonction pour initialiser le tableau de cosinus
 * @todo Il serait mieux que le tableau de cosinus soit en flash
 *       pour eviter de la recalculer à chaque démarage
 *
 */
static void cos_init();
static void cos_init()
{
    for (uint32_t cos_tab_index=0 ; cos_tab_index < NB_COS ; cos_tab_index++)
    {
        cos_tab[cos_tab_index] = 1;//QUANTUM * cosf(COSINDEX_TO_ANGLE(cos_tab_index)/PI_INT);
    }
}

void trigo_init()
{
    cos_init();
}

inline dist_t cos_int(angle_t angle)
{
    return cos_tab[ANGLE_TO_COSINDEX(angle)];
}

inline dist_t sin_int(angle_t angle)
{
    return cos_int(angle - PI_INT/2);
}

inline dist_t tan_int(angle_t angle)
{
    return sin_int(angle)/cos_int(angle);
}


inline angle_t acos_int(dist_t dist)
{
    // On cherche l'index du cosinus le plus proche de la distance demandée
    for (uint32_t cos_tab_index=0 ; cos_tab_index < NB_COS ; cos_tab_index++)
    {
        if (cos_tab[cos_tab_index] > dist)
        {
            return COSINDEX_TO_ANGLE(cos_tab_index);
        }
    }
}

inline angle_t asin_int(dist_t dist)
{
    return (PI_INT/2 - acos_int(dist));
}

inline angle_t atan_int(dist_t dist)
{
    return asin_int(dist/sqrt(dist*dist + 1));
}
