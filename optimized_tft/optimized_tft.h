#ifndef __OPTIMIZED_TFT_H
#define __OPTIMIZED_TFT_H

#include <stdint.h>

/**
 * @brief Fonction de rafraichissement de l'écran
 *
 */
void OPTFT_refresh();

/**
 * @brief Fonction de dessin d'un pixel
 *
 * @param x Coordonnée x du pixel
 * @param y Coordonnée y du pixel
 * @param color Couleur du pixel
 */
void OPTFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Fonction de dessin d'un rectangle
 *
 * @param x0 Coordonnée x du coin supérieur gauche
 * @param y0 Coordonnée y du coin supérieur gauche
 * @param x1 Coordonnée x du coin inférieur droit
 * @param y1 Coordonnée y du coin inférieur droit
 * @param color Couleur du rectangle
 */
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * @brief Fonction de dessin d'une ligne
 *
 * @param x0 Coordonnée x du point de départ
 * @param y0 Coordonnée y du point de départ
 * @param x1 Coordonnée x du point d'arrivée
 * @param y1 Coordonnée y du point d'arrivée
 * @param color Couleur de la ligne
 */
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


#endif
