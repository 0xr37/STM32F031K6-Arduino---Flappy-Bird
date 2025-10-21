#include <stm32f031x6.h>
#include "display.h"
#include <stdio.h>
#include "prbs.h"
#include "serial.h"
#define BLUE 33020
#define MINSCREENX 0
#define MINSCREENY 0
#define MAXSCREENX 127
#define MAXSCREENY 159
#define PIPEAMT 3
#define PIPEWIDTH 14
#define PIPEHEIGHT 4
#define BIRDWIDTH 17
#define BIRDHEIGHT 12
#define spawnGap 50
#define bHeight 74
#define bWidth 32
#define bStart 65
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

uint16_t background[]={
    123,
};

void putImage2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image, int hOrientation, int vOrientation)
{
    uint16_t Colour;
	  uint32_t offset = 0;
	char str[50];
    openAperture2(x, y, x + width - 1, y + height - 1); // open the dimensions of the image for writing
    DCHigh2();
	// eputs("test1");

	if (vOrientation == 0)
	{
		for (int k = 0; k < height; k++) // y loop
		{
			offset=k*width;
			for (int j = 0; j < width; j++) // x loop
			{
				Colour = Image[offset+j];
				if (Colour == 5536){
					if (y > bStart && y < (bStart + bHeight)){
						
						int index = x % 32;
						int imIndex = (y - bStart - 1)  * bWidth + index;
						Colour = background[imIndex];
						// sprintf(str, "%d : %d | %d : %d = %d\n\r", x, y, index, imIndex, Colour);
						// eputs(str);
						// eputs("test2");
					}
					else if(y < bStart){
						Colour = 2510;
					}
					else if (y > (bStart + bHeight - 1)){
						Colour = 12562;
					}
				}
				
				transferSPI162(Colour);
				x += 1;
			}
			y += 1;
		}
	}
	else
	{
		for (y = 0; y < height; y++)
		{
				offset=(height-(y+1))*width;
				for (x = 0; x < width; x++)
				{
					Colour = Image[offset+x];
					transferSPI162(Colour);
				}
		}
	}
}







void putImage2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image, int hOrientation, int vOrientation)
{
    uint16_t Colour;
    uint32_t offset = 0;
    uint16_t baseX = x;
    uint16_t baseY = y;

    openAperture2(x, y, x + width - 1, y + height - 1); // open the dimensions of the image for writing
    DCHigh2();

    if (vOrientation == 0)
    {
        for (y = 0; y < height; y++)
        {
            offset = y * width;
            for (x = 0; x < width; x++)
            {
                Colour = Image[offset + x];
                uint16_t screenX = baseX + x;
                uint16_t screenY = baseY + y;

                if (Colour == 5536) {
                    if (screenY >= bStart && screenY < (bStart + bHeight)) {
                        int index = screenX % bWidth;
                        int imIndex = (screenY - bStart) * bWidth + index;
                        Colour = background[imIndex];
                    } else if (screenY < bStart) {
                        Colour = 2510;
                    } else {
                        Colour = 12562;
                    }
                }

                transferSPI162(Colour);
            }
        }
    }
    else
    {
        for (int k = 0; k < height; k++)
        {
            offset = (height - (k + 1)) * width;
            for (int j = 0; j < width; j++)
            {
                Colour = Image[offset + j];
                uint16_t screenX = baseX + j;
                uint16_t screenY = baseY + k;

                if (Colour == 5536) {
                    if (screenY >= bStart && screenY < (bStart + bHeight)) {
                        int index = screenX % bWidth;
                        int imIndex = (screenY - bStart) * bWidth + index;
                        Colour = background[imIndex];
                    } else if (screenY < bStart) {
                        Colour = 2510;
                    } else {
                        Colour = 12562;
                    }
                }

                transferSPI162(Colour);
            }
        }
    }
}