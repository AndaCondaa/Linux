/*
 * HomeWork #2
 * BMP 파일 헤더만 이용해서 BMP 파일을 복사하세요. 
 * 단, 복사되는 파일은 임의의 팔렛트 256개를 추가해야 합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

#define PAL_COUNT 256

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width);

int main(int argc, char **argv)
{
	//Variable declaration 
	FILE *file;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *inImage, *outImage; 
	RGBQUAD *palette;
	unsigned int widthBits, pixelCount, index;
	float Bpp;
	
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
	
	Bpp = bmpInfoHeader.biBitCount / 8.;
	pixelCount = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight;
	widthBits = paddingWidthBits(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBits * bmpInfoHeader.biHeight;
	}
	
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage); 
	outImage = (unsigned char*)malloc(sizeof(unsigned char) * pixelCount);
	memset(inImage, 0, bmpInfoHeader.SizeImage);
	memset(outImage, 0, pixelCount);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	palette = (RGBQUAD*)malloc(sizeof(RGBQUAD) * PAL_COUNT);
	for (int i = 0; i < PAL_COUNT; i++) {
		palette[i].rgbBlue = i;
		palette[i].rgbGreen = i;
		palette[i].rgbRed = i;
	}


	for (int y = bmpInfoHeader.biHeight-1; y >= 0 ; y--) {
		for (int x = 0; x < bmpInfoHeader.biWidth; x++) { 
			index = (x * Bpp) + (y * widthBits);
			int r = inImage[index+2];
			int g = inImage[index+1];
			int b = inImage[index];
			int gray = (0.299 * r) + (0.587 * g) + (0.144 * b);
			outImage[x + (y * bmpInfoHeader.biWidth)] = gray;
		}
	}

	if ((file = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[2]);
		return -1;
	}

	bmpInfoHeader.biBitCount = 8;
	bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1;
	bmpInfoHeader.biClrUsed = 0;
	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * PAL_COUNT + bmpInfoHeader.SizeImage;
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * PAL_COUNT;

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(palette, sizeof(RGBQUAD), PAL_COUNT, file); 
	fwrite(outImage, sizeof(unsigned char), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1, file);
	
	free(inImage); 
	free(outImage);
	free(palette);

	fclose(file);

	return 0;
}

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
