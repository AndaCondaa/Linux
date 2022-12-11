/*
 * HomeWork #2
 * Copy the BITMAP File by using only Bitmap headers. Add Pal(256).
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
	RGBQUAD *pal;

	file = fopen(argv[1], "rb");

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	
	if(bmpInfoHeader.biBitCount != 24){
		perror("You can use only 24 bits BITMAP images");
		return -1;
	}

	if(!bmpInfoHeader.SizeImage)
		bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth *\
				 bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;
	
	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.SizeImage);
	pal = (RGBQUAD*)malloc(sizeof(RGBQUAD) * 256);

	fread(inimg, sizeof(ubyte), bmpInfoHeader.SizeImage, file);
	fclose(file);

	for(int i = 0; i < 256; i++){
		pal[i].rgbBlue = pal[i].rgbGreen = pal[i].rgbRed = i;
	}

	for(int j = 0; j < bmpInfoHeader.biHeight; j++){
		for (int i = 0; i < bmpInfoHeader.biWidth; i++){
			for(int z = 0; z < 3; z++){
				outimg[(i*3)+(j*bmpInfoHeader.biWidth*3)+z] 
					= inimg[(i*3)+(j*bmpInfoHeader.biWidth*3)+z];
			}

		}
	}


	bmpInfoHeader.biClrUsed = 256;
	bmpHeader.bfSize += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;
	bmpHeader.bfOffBits += sizeof(RGBQUAD) * bmpInfoHeader.biClrUsed;

	file = fopen(argv[2], "w");

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(pal, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, file);
	fwrite(outimg, sizeof(ubyte), bmpInfoHeader.SizeImage, file);

	fclose(file);

	free(inimg);
	free(outimg);
	free(pal);

	return 0;
}
