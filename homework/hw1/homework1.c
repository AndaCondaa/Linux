/*
 * HomeWork #1
 * Make Program for printing Pixels' Values(BGR) by using only Bitmap Headers(P480)
 * Input: BMP File name 
 * Output: (255, 255, 255)
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
	unsigned char *image; 
	unsigned int widthBits;

	//Open the input file
	file = fopen(argv[1], "rb");

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
	
	image = (unsigned char*)malloc \
		(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(image, 0, bmpInfoHeader.SizeImage);

	fread(image, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	for (int y=bmpInfoHeader.biHeight-1; y >= 0 ; y--) {
		for (int x=0; x < bmpInfoHeader.biWidth; x++) { 
			printf("(r = %3d, g = %3d, b = %3d)\n",
	       			image[(x*3+2)+(y*widthBits)], image[(x*3+1)+(y*widthBits)], image[(x*3)+(y*widthBits)]);
		}
	}

	return 0;
}

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
