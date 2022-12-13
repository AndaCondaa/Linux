/*
 * HomeWork #1
 * Make Program for printing Pixels' Values(BGR) by using only Bitmap Headers(P480)
 * Input: BMP File name 
 * Output: (255, 255, 255)
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmpHeader.h"

int main(int argc, char **argv)
{
	//Variable declaration 
	FILE *file;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *image; 

	//Open the input file
	file = fopen(argv[1], "rb");

	//Read bitmap headers
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	//Check the bit count whether it is 24bit or not 
	if(bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24bit image\n");
		return -1;	
	}

	
	if (bmpInfoHeader.SizeImage == 0)
		bmpInfoHeader.SizeImage = \
			bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8; 

	image = (unsigned char*)malloc\
		(sizeof(unsigned char) * bmpInfoHeader.SizeImage);

	fread(image, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	
	for(int i=0; i < bmpInfoHeader.SizeImage; i+=bmpInfoHeader.biBitCount/8) {
		printf("(%d, %d, %d)\n", image[i], image[i+1], image[i+2]);
	}

	fclose(file);

	return 0;
}

