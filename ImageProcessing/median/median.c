#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                    
#include <unistd.h>

#include "bmpHeader.h"

#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

int main(int argc, char** argv) 
{
	FILE* fp; 
	BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
	BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
	RGBQUAD *palrgb;
	ubyte *inimg, *padimg, *outimg;
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

	printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     
	printf("Image Size : %d\n", imageSize);

	printf("%d\n", __LINE__);
	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	fread(inimg, sizeof(ubyte), imageSize, fp); 
	fclose(fp);

	printf("%d\n", __LINE__);
	int padSize = (bmpInfoHeader.biWidth + 2) * elemSize;
	int addSize = (padSize + bmpInfoHeader.biHeight)*2;
	padimg = (ubyte*)malloc(sizeof(ubyte)*(imageSize + addSize));

	memset(padimg, 0, (sizeof(ubyte)*imageSize + addSize));
	for(y = 0; y < bmpInfoHeader.biHeight; y++) {
		for(x = 0; x < bmpInfoHeader.biWidth * elemSize; x+=elemSize) {
	    		for(z = 0; z < elemSize; z++) {
				padimg[(x+elemSize)+(y+1)*padSize+z]=inimg[x+y*size+z];
	    		}
		}
	}

	for(y = 0; y < bmpInfoHeader.biHeight; y++) { 
		for(z = 0; z < elemSize; z++) {
	   		padimg[0+(y+1)*padSize+z]=inimg[0+y*size+z];
	    		padimg[padSize-elemSize+(y+1)*padSize+z]=inimg[size-elemSize+y*size+z];
		}
	}

	for(x = 0; x < bmpInfoHeader.biWidth*elemSize; x++) { 
		padimg[elemSize+x]=inimg[x];
		padimg[elemSize+x+(bmpInfoHeader.biHeight+1)*padSize]=inimg[x+(bmpInfoHeader.biHeight-1)*size];
	}

	for(z = 0; z < elemSize; z++) {
		padimg[z]=inimg[z];
		padimg[padSize-elemSize+z]=inimg[size-elemSize+z];
		padimg[(bmpInfoHeader.biHeight+1)*padSize+z]=inimg[(bmpInfoHeader.biHeight-1)*size+z];
		padimg[(bmpInfoHeader.biHeight+1)*padSize+padSize-elemSize+z]=inimg[(bmpInfoHeader.biHeight-1)*size+size-elemSize+z];
	}
	printf("%d\n", __LINE__);
	// define the kernel
	int *kernel;
	kernel=(int*)malloc(sizeof(int)*9);
	memset(kernel, 0, sizeof(int)*9);
	memset(outimg, 0, sizeof(ubyte)*imageSize);
	for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
		for(x = elemSize; x < padSize; x+=elemSize) {
	    		for(z = 0; z < elemSize; z++) {
				for(int i = -1; i < 2; i++) {
		    			for(int j = -1; j < 2; j++) {
						kernel[(j+1)+(i+1)*3]=padimg[(x+i*elemSize)+(y+j)*padSize+z];
		    			}
				}
				
				// sort 
				int temp;
				for(int k=0; k<9; k++) {
					for(int l=0; l<8; l++) {
						if(kernel[l]>kernel[l+1]) {
							temp = kernel[l];
							kernel[l] = kernel[l+1];
							kernel[l+1] = temp;
						}
					}

				}
				outimg[(x-elemSize)+(y-1)*size+z] = kernel[4];
	    		}
		}
	}         

	/***** write bmp *****/ 
	if((fp=fopen(argv[2], "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	printf("%d\n", __LINE__);
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(outimg, sizeof(ubyte), imageSize, fp);

	fclose(fp); 

	free(inimg); 
	free(outimg);
	free(padimg);

	return 0;
}
