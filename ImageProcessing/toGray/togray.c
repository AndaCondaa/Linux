#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "bmpHeader.h"

#define BYTE unsigned char

#define widthbytes(bits) ((((bits)+31)/32)*4)

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

	palrgb = (RGBQUAD*)malloc(sizeof(RGBQUAD)*256);
	//fread(palrgb, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, fp);

	size = widthbytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth);

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * size;
	}
	printf("BItCount : %d\n", bmpInfoHeader.biBitCount);

	float Bpp = bmpInfoHeader.biBitCount / 8.;

	printf("%f\n", Bpp);

	inimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*Bpp); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1); 
	fread(inimg, sizeof(BYTE), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*Bpp , fp); 
	
	fclose(fp);

	for(int k = 0; k < 256; k++) {
		palrgb[k].rgbRed=k;
		palrgb[k].rgbGreen=k;
		palrgb[k].rgbBlue=k;
	}	

	int pos = 0;
	for(i=0; i<bmpInfoHeader.biHeight*bmpInfoHeader.biWidth; i++) { 
		int r = inimg[(i*3)+2];
		int g = inimg[(i*3)+1];	
		int b = inimg[(i*3)];
		int gray = (0.299*r) + (0.587*g) + (0.144*b);
		outimg[i] = gray;	
	};	  
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	bmpInfoHeader.biBitCount = 8;
	bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1;
	bmpInfoHeader.biClrUsed = 0;
	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + bmpInfoHeader.SizeImage;
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(palrgb, sizeof(RGBQUAD), 256, fp); 
	fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 

	return 0;
}
