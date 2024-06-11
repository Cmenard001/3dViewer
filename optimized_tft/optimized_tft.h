#ifndef __OPTIMIZED_TFT_H
#define __OPTIMIZED_TFT_H

#include <stdint.h>

/**
 * @brief Fonction de rafraichissement de l'�cran
 *
 */
void OPTFT_refresh();

/**
 * @brief Fonction de dessin d'un pixel
 *
 * @param x Coordonn�e x du pixel
 * @param y Coordonn�e y du pixel
 * @param color Couleur du pixel
 */
void OPTFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Fonction de dessin d'un rectangle
 *
 * @param x0 Coordonn�e x du coin sup�rieur gauche
 * @param y0 Coordonn�e y du coin sup�rieur gauche
 * @param x1 Coordonn�e x du coin inf�rieur droit
 * @param y1 Coordonn�e y du coin inf�rieur droit
 * @param color Couleur du rectangle
 */
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * @brief Fonction de dessin d'une ligne
 *
 * @param x0 Coordonn�e x du point de d�part
 * @param y0 Coordonn�e y du point de d�part
 * @param x1 Coordonn�e x du point d'arriv�e
 * @param y1 Coordonn�e y du point d'arriv�e
 * @param color Couleur de la ligne
 */
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


#endif
