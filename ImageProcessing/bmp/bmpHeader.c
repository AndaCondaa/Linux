#include <stdio.h>

#include "bmpHeader.h"

int readBmp (char *filename, unsigned char **data, int *cols, int *rows)
{
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	FILE *fp;

	fp = fopen(filename, "rb");
	if(fp == NULL) {
		perror("ERROR\n");
		return -1;
	}

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	if(bmpInfoHeader.biBitCount != 24) {
		perror("This image file doesn't supports 24bit color\n");
		fclose(fp);
		return -1;
	}

	*cols = bmpInfoHeader.biWidth;
	*rows = bmpInfoHeader.biHeight;

	printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);

	fseek(fp, bmpHeader.bfOffBits, SEEK_SET);
	fread(*data, 1, bmpHeader.bfSize-bmpHeader.bfOffBits, fp);

	fclose(fp);
	
	return 0;
}
