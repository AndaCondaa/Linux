/*
 * HomeWork #2
 * BMP 파일 헤더만 이용해서 BMP 파일을 복사하세요. 
 * 단, 복사되는 파일은 임의의 팔렛트 256개를 추가해야 합니다.
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
	unsigned char *inImage; 
	unsigned int widthBits, index;

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
	
	inImage = (unsigned char*)malloc \
		(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	return 0;
}

unsigned int paddingWidthBits(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
