#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                    
#include <unistd.h>
#include <math.h>

#include "bmpHeader.h"

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

int main(int argc, char** argv) 
{
	FILE* fp; 
	BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
	BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
	RGBQUAD *palrgb;
	ubyte *inimg, *padimg, *outimg, *grayimg;
	int x, y, z, imageSize;
	
	if(argc != 3) {
		fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
		return -1;
	}
	   
	/***** read bmp *****/ 
	if((fp=fopen(argv[1], "rb")) == NULL) { 
		fprintf(stderr, "Error : Failed to open file...\n"); 
		return -1;
	}

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	if(bmpInfoHeader.biBitCount != 24) {
		perror("This image file doesn't supports 24bit color\n");
		fclose(fp);
		return -1;
	}

	int elemSize = bmpInfoHeader.biBitCount/8;
	int size = bmpInfoHeader.biWidth*elemSize;
	imageSize = size * bmpInfoHeader.biHeight; 

	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	grayimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	fread(inimg, sizeof(ubyte), imageSize, fp); 
	fclose(fp);

	for(int i=0; i< (bmpInfoHeader.biWidth)*(bmpInfoHeader.biHeight) ; i++) {
		int r = (float)inimg[i*3+2];
		int g = (float)inimg[i*3+1];
		int b = (float)inimg[i*3+0];
		int gray = (r*0.3F)+(g*0.59F)+(b*0.11F);
		grayimg[i*3+2] =  grayimg[i*3+1] = grayimg[i*3] = gray;
	}

	int padSize = (bmpInfoHeader.biWidth + 2) * elemSize;
	int addSize = (padSize + bmpInfoHeader.biHeight)*2;
	padimg = (ubyte*)malloc(sizeof(ubyte)*(imageSize + addSize));

	memset(padimg, 0, (sizeof(ubyte)*imageSize + addSize));
	for(y = 0; y < bmpInfoHeader.biHeight; y++) {
		for(x = 0; x < bmpInfoHeader.biWidth * elemSize; x+=elemSize) {
	    		for(z = 0; z < elemSize; z++) {
				padimg[(x+elemSize)+(y+1)*padSize+z]=grayimg[x+y*size+z];
	    		}
		}
	}

	for(y = 0; y < bmpInfoHeader.biHeight; y++) { 
		for(z = 0; z < elemSize; z++) {
	   		padimg[0+(y+1)*padSize+z]=grayimg[0+y*size+z];
	    		padimg[padSize-elemSize+(y+1)*padSize+z]=grayimg[size-elemSize+y*size+z];
		}
	}

	for(x = 0; x < bmpInfoHeader.biWidth*elemSize; x++) { 
		padimg[elemSize+x]=grayimg[x];
		padimg[elemSize+x+(bmpInfoHeader.biHeight+1)*padSize]=grayimg[x+(bmpInfoHeader.biHeight-1)*size];
	}

	for(z = 0; z < elemSize; z++) {
		padimg[z]=grayimg[z];
		padimg[padSize-elemSize+z]=grayimg[size-elemSize+z];
		padimg[(bmpInfoHeader.biHeight+1)*padSize+z]=grayimg[(bmpInfoHeader.biHeight-1)*size+z];
		padimg[(bmpInfoHeader.biHeight+1)*padSize+padSize-elemSize+z]=grayimg[(bmpInfoHeader.biHeight-1)*size+size-elemSize+z];
	}

	// define the kernel
	int xKernel[3][3] = {{-1, 0, 0},
		             {0, 1, 0},
		             {0, 0, 0}}; 
	int yKernel[3][3] = {{0, 0 , -1},
		             {0, 1, 0}, 
		             {0, 0, 0}};

	memset(outimg, 0, sizeof(ubyte)*imageSize);
	
	int xSum = 0;
	int ySum = 0;
	// xKernel
	for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
		for(x = 3; x < (bmpInfoHeader.biWidth+2)*3; x+=3) {
			for(z =0; z<3; z++) {
				xSum = 0;
				ySum = 0;
				for(int i = -1; i < 2; i++) {
					for(int j = -1; j < 2; j++) {
						xSum += xKernel[i+1][j+1]*padimg[(x+i*3)+(y+j)*(bmpInfoHeader.biWidth+2)*3+z];
						ySum += yKernel[j+1][i+1]*padimg[(x+i*3)+(y+j)*(bmpInfoHeader.biWidth+2)*3+z];
					}
				}
				int value = sqrt((xSum*xSum)+(ySum*ySum));
				outimg[(x-3)+(y-1)*size+z] = value;
			}
		}
	}         

	/***** write bmp *****/ 
	if((fp=fopen(argv[2], "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}
	
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(outimg, sizeof(ubyte), imageSize, fp);

	fclose(fp); 

	free(inimg); 
	free(outimg);
	free(padimg);

	return 0;
}
