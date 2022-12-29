/*
 * HomeWork #3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width);

int main(int argc, char **argv)
{
	//Variable declaration 
	FILE *file;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *inImage, *outImage; 
	unsigned int widthBits, index;

	if (argc != 3) {
		fprintf(stderr, "This file(%s) needs three of Command Line Arguments.\n", __FILE__);
		return -1;
	}

	//Open the input file
	if ((file = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[1]);
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
	outImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);
	memset(outImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	for (int i = 0; i < bmpInfoHeader.SizeImage; i++) {
		outImage[i] = inImage[i];
	}

	if ((file = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[2]);
		return -1;
	}

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(outImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);

	fclose(file);

	free(inImage);
	free(outImage);

	return 0;
}

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
