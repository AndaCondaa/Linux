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

	palrgb = (RGBQUAD*)malloc(sizeof(RGBQUAD)*bmpInfoHeader.biClrUsed);
	fread(palrgb, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, fp);

	size = widthbytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth);

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * size;
	}

	float Bpp = bmpInfoHeader.biBitCount / 8.;

	printf("%f\n", Bpp);

	inimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*Bpp); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*3); 
	fread(inimg, sizeof(BYTE), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*Bpp , fp); 
	
	fclose(fp);

	int mask=0b0;
	for(i=0; i<bmpInfoHeader.biBitCount; i++) {
		mask |= 0b1 << i;
	}

	int pos = 0;
	for(i=0; i<bmpInfoHeader.biHeight*bmpInfoHeader.biWidth*Bpp; i++) { 
		for (int k = (8-bmpInfoHeader.biBitCount); k >= 0; k-=bmpInfoHeader.biBitCount) {
				int result = inimg[i] >> k & mask;
				outimg[pos++] = palrgb[result].rgbBlue;
				outimg[pos++] = palrgb[result].rgbGreen;
				outimg[pos++] = palrgb[result].rgbRed;
		}		
	};	  
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*3;
	bmpInfoHeader.biClrUsed = 0;
	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfoHeader.SizeImage;
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*3, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 

	return 0;
}
