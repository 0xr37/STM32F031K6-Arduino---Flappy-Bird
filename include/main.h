#include <stdint.h>

uint16_t getBackgroundPixel(uint16_t screenX, uint16_t screenY);
void putImageV2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image);
void fillBackground(uint16_t x,uint16_t y,uint16_t width, uint16_t height);
