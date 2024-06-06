#ifndef __OPTIMIZED_TFT_H
#define __OPTIMIZED_TFT_H

#include <stdint.h>

void OPTFT_refresh();
void OPTFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);


#endif
