#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "bmpHeader.h"

#define widthBytes(bits) ((((bits)+31)/32)*4)

#define ubyte unsigned char

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg;
    
    if((fp=fopen(argv[1], "rb")) == NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    printf("%d\n" , bmpInfoHeader.biClrUsed);
    printf("%d\n" , bmpInfoHeader.biBitCount);
    if(bmpInfoHeader.biBitCount == 8 && bmpInfoHeader.biClrUsed == 0) {
	printf("line : %d\n", __LINE__);
	bmpInfoHeader.biClrUsed = 256;
    }
    printf("%d\n" , bmpInfoHeader.biClrUsed);
    palrgb = (RGBQUAD*)malloc(sizeof(RGBQUAD)*bmpInfoHeader.biClrUsed);
    fread(palrgb, sizeof(RGBQUAD), bmpInfoHeader.biClrUsed, fp); 

    if(!bmpInfoHeader.SizeImage){
        bmpInfoHeader.SizeImage = widthBytes(bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth)*bmpInfoHeader.biHeight; 
    }

    float elemSize = bmpInfoHeader.biBitCount / 8.;

    inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.biWidth * bmpInfoHeader.biHeight*elemSize); 
    outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3);
    fread(inimg, sizeof(ubyte), bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * elemSize, fp); 
    
    fclose(fp);
    
    int mask = 0b0;
    for(int x = 0; x < bmpInfoHeader.biBitCount; x++) {
	mask |= 0b1 << x;
    }

    int pos = 0; 
    for(int x = 0; x < bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * elemSize; x++) {      
	for(int i = (8- bmpInfoHeader.biBitCount); i >= 0; i-=bmpInfoHeader.biBitCount)
	{
        	int res = inimg[x] >> i & mask;
	        outimg[pos++]=palrgb[res].rgbBlue;
        	outimg[pos++]=palrgb[res].rgbGreen;
		outimg[pos++]=palrgb[res].rgbRed;
	}
    }         
     
    /***** write bmp *****/ 
    if((fp=fopen(argv[2], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3;
    bmpInfoHeader.biClrUsed = 0;
    bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfoHeader.SizeImage;
    bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3, fp);

    free(inimg); 
    free(outimg);
    
    fclose(fp);

    return 0;
}
