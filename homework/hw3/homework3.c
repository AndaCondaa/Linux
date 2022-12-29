/*
 * HomeWork #3
 * 2중 for 루프를 1중 for 루프로 변경하세요.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width);

int main(int argc, char** argv)
{
	//Variable declaration 
	FILE* file;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char* inImage;
	unsigned int widthBits, index;

	if (argc != 3) {
		fprintf(stderr, "This file(%s) needs three of Command Line Arguments.\n", __FILE__);
		return -1;
	}

	//Open the input file
	if ((file = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file\n");
		return -1;
	}

	//Read bitmap headers
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	//Check the bit count whether it is 24bit or not 
	if (bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24bit image\n");
		return -1;
	}

	widthBits = paddingWidthBits(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBits * bmpInfoHeader.biHeight;
	}

	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);

	for (int y = bmpInfoHeader.biHeight - 1; y >= 0; y--) {
		for (int x = 0; x < bmpInfoHeader.biWidth; x++) {
			index = (x * 3) + (y * widthBits);
			printf("(r = %3d, g = %3d, b = %3d)\n",
				inImage[index + 2], inImage[index + 1], inImage[index]);
		}
	}

	free(inImage);

	return 0;
}

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits + 31) / 32) * 4;

	return padWidth;
}
