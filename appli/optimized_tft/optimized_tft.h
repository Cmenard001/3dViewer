#ifndef __OPTIMIZED_TFT_H
#define __OPTIMIZED_TFT_H

#include <stdint.h>

#define PIXEL_OFF 0
#define PIXEL_ON 1

typedef int8_t pixel_state_t;

void OPTFT_init();
void OPTFT_refresh();
void OPTFT_DrawPixel(uint16_t x, uint16_t y, pixel_state_t pixel_state);
void OPTFT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_state_t pixel_state);
void OPTFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, pixel_state_t pixel_state);


#endif
