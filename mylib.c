//Author: Sonika Finch

#include "mylib.h"

unsigned short *videoBuffer = (unsigned short *) 0x6000000;

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for (int r = 0; r < height; r++) {
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawImage3(int row, int col, int width, int height, const unsigned short *image) {
	for (int r  = 0; r < height; r++) {
		if (width == 1) {
			setPixel(r + row, col, image[r + col]);
		} else {
			DMA[3].src = &image[r * width];
			DMA[3].dst = &videoBuffer[(r + row) * 240 + col];
			DMA[3].cnt = width | DMA_ON;
		}
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}