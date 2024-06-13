#ifndef __VIEWER_H__
#define __VIEWER_H__
#include "../math/geometry.h"

/**
 * @brief Fonction d'initialisation des boutons
 *
 */
void button_init();

/**
 * @brief Fonction principale de traitement des boutons
 * @note Cette fonction est appelée en boucle
 *
 */
void viewer_process_main();

/**
 * @brief Get the current polar coord of the object to display
 *
 * @param polar_coord_three_dims polar coord to fill
 */
void get_polar_coord(polar_coord_three_dims_t *polar_coord_three_dims);

#endif
