#include <stdio.h>
#include <stdlib.h>

#include "bmpHeader.h"

int main(int argc, char **argv)
{
	FILE *inputFile;

	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *image; 

	inputFile = fopen(argv[1], "rb");
	//inputFile = open(argv[1], O_RDONLY);

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, inputFile);

	if(bmpInfoHeader.biBitCount != 24) {
		perror("you can use only 24bit image\n");
		return -1;	
	}

	if (bmpInfoHeader.SizeImage == 0)
		bmpInfoHeader.SizeImage = \
			bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8; 

	image = (unsigned char*)malloc\
		(sizeof(unsigned char) * bmpInfoHeader.SizeImage);

	fread(image, sizeof(unsigned char), bmpInfoHeader.SizeImage, inputFile);
	
	for(int i=0; i < bmpInfoHeader.SizeImage; i+=bmpInfoHeader.biBitCount/8) {
		printf("(%d, %d, %d)\n", image[i], image[i+1], image[i+2]);
	}

	


	fclose(inputFile);


	return 0;
}

