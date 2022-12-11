/*
 * HomeWork #3
 * Change the double loop(for) to the single loop(for)
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmpHeader.h"

#define ubyte unsigned char

int main(int argc, char **argv)
{
	FILE *file;
	
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	ubyte *inimg, *outimg;
	
	file = fopen(argv[1], "rb");

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	
	if(!bmpInfoHeader.SizeImage)
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth *\
			 bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;

	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);

	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, file);
	fclose(file);

	for(int i = 0; i < bmpInfoHeader.SizeImage; i++)
		outimg[i] = inimg[i];
	

	file = fopen(argv[2], "w");

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, file);

	fclose(file);

	return 0;
}
