#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "bmpHeader.h"

#define BYTE unsigned char

#define widthbytes(bits) ((((bits)+31)/32)*4)


inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
	return(value > max?max:value < min? min: value);
}

int main(int argc, char** argv) {
	FILE* fp; 
	RGBQUAD *palrgb;

	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	char input[128], output[128];
	
	int i, j, size; 
	int index; 
	float r,g,b,gray;
	int index2;
	
	unsigned char *inimg, *outimg;
	
	strcpy(input, argv[1]); 
	strcpy(output, argv[2]);
	
	if((fp=fopen(input, "rb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	if(bmpInfoHeader.biBitCount == 8 && bmpInfoHeader.biClrUsed == 0)
		bmpInfoHeader.biClrUsed = 256;


	size = widthbytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth);

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * size;
	}
	printf("BItCount : %d\n", bmpInfoHeader.biBitCount);

	float Bpp = bmpInfoHeader.biBitCount / 8.;

	printf("%f\n", Bpp);

	inimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.SizeImage); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.SizeImage); 
	fread(inimg, sizeof(BYTE), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*Bpp , fp); 
	
	fclose(fp);
	
	for(i=0; i<bmpInfoHeader.SizeImage; i+=3) { 
		for(int z = 0; z<3 ; z++) 
			outimg[i+z] = inimg[i+z];
	};	  


	for(i=0; i<10000; i++){
		int noise = rand() & 0b11111111;
		int pos = rand() % (bmpInfoHeader.biHeight*bmpInfoHeader.biWidth);
		for(int z = 0; z<3; z++) {
			outimg[pos*3+z] = clip(outimg[pos*3+z]+noise, 0, 255);
		}
	}

	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.SizeImage, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 

	return 0;
}
