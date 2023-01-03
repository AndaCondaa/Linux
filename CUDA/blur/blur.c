#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> /* USHRT_MAX 상수를 위해서 사용한다. */
#include <math.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)
#define widthbytes(bits) (((bits)+31)/32*4)

typedef unsigned char ubyte;

int main(int argc, char** argv)
{
	FILE* fp;
	BITMAPFILEHEADER bmpHeader; /* BMP FILE INFO */
	BITMAPINFOHEADER bmpInfoHeader; /* BMP IMAGE INFO */
	ubyte *inimg, *outimg;

	if(argc != 3) {
		fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
		return -1;
	}

	/***** read bmp *****/
	if((fp=fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...₩n");
		return -1;
	}

	/* BITMAPFILEHEADER 구조체의 데이터 */
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	/* BITMAPINFOHEADER 구조체의 데이터 */
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	/* 트루 컬러를 지원하면 변환할 수 없다. */
	if(bmpInfoHeader.biBitCount != 24) {
		perror("This image file doesn't supports 24bit color\n");
		fclose(fp);
		return -1;
	}

	int elemSize = bmpInfoHeader.biBitCount / 8;
	int size = bmpInfoHeader.biWidth * elemSize;

	//widthbytes(bits) (((bits)+31)/32*4)
	int imageSize = size * bmpInfoHeader.biHeight;

	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	fread(inimg, sizeof(ubyte), imageSize, fp);

	fclose(fp);

	float kernel[3][3] = {
		{1/9.0, 1/9.0, 1/9.0},
		{1/9.0, 1/9.0, 1/9.0},
		{1/9.0, 1/9.0, 1/9.0}
	};

	/*
	float sum = 0.0;
	for (int y = 0; y < bmpInfoHeader.biHeight; y++) {
		for (int x = 0; x < bmpInfoHeader.biWidth; x++) {
			for (int z = 0; z < 2; z++) {
				sum = 0.0;
				if ((x>0) && (x<bmpInfoHeader.biWidth-1) && (y>0) && (y<bmpInfoHeader.biHeight)) {
					for (int i = -1; i < 2; i++) {
						for (int j = -1; j < 2; j++) {
							sum += (kernel[i+1][j+1]) * (inimg[(x-i*3)+(y-j)*bmpInfoHeader.biWidth*3+z]);
						}
					}
					outimg[(x)+(y*bmpInfoHeader.biWidth*3)+z] = sum;
				} else {
					//outimg[(x+(y*bmpInfoHeader.biWidth))*elemSize+z] = inimg[(x+(y*bmpInfoHeader.biWidth))*elemSize+z];
				}
			}
		}
	}
	*/


	ubyte arr[9] = {0,};

	memset(outimg, 0, sizeof(ubyte)*imageSize);
    	for(int y = 0; y < bmpInfoHeader.biHeight ; y++) { 
       		for(int x = 0; x < (bmpInfoHeader.biWidth) * elemSize; x+=elemSize) {
            		for(int z = 0; z < elemSize; z++) {
                		float sum = 0.0;
				if ((x>0 && x<(bmpInfoHeader.biWidth-1)*elemSize) && (y>0 && y<bmpInfoHeader.biHeight-1)) {
                			for(int i = -1; i < 2; i++) {
                    				for(int j = -1; j < 2; j++) {
                        				sum += kernel[i+1][j+1]*inimg[(x-i*elemSize)+(y-j)*size+z];
                    				}
                			}
                			outimg[x+y*size+z] = sum;
            			} else if (y == 0) {
					if (x == 0) {
						arr[0] = arr[1] = arr[3] = arr[4] = inimg[x+y*size+z];
						arr[2] = arr[5] = inimg[x+1*elemSize+y*size+z];
						arr[6] = arr[7] = inimg[x+(y+1)*size+z];
						arr[8] = inimg[x+1*elemSize+(y+1)*size+z];
					} else if ((x>0) && (x<(bmpInfoHeader.biWidth-1)*elemSize)) {
						arr[0] = arr[3] = inimg[x-1*elemSize+y*size+z];
						arr[1] = arr[4] = inimg[x+y*size+z];
						arr[2] = arr[5] = inimg[x+1*elemSize+y*size+z];
						arr[6] = inimg[x-1*elemSize+(y+1)*size+z];
						arr[7] = inimg[x+(y+1)*size+z];
						arr[8] = inimg[x+1*elemSize+(y+1)*size+z];
					} else {
						arr[0] = arr[3] = inimg[x-1*elemSize+y*size+z];
						arr[1] = arr[2] = arr[4] = arr[5] = inimg[x+y*size+z];
						arr[6] = inimg[x-1*elemSize+(y+1)*size+z];
						arr[7] = arr[8] = inimg[x+(y+1)*size+z];
					}				
					for (int k = 0; k < 9; k++) {
						sum += arr[k];
					}
					outimg[x+y*size+z] = sum / 9.;
        			} else if (y == (bmpInfoHeader.biWidth-1)) {

				} else {
					if (x == 0) {
					} else {
					}
				}
    			}         
		}
	}


	/***** write bmp *****/
	if((fp=fopen(argv[2], "wb"))==NULL) {
		fprintf(stderr, "Error : Failed to open file...₩n");
		return -1;
	}

	/* BITMAPFILEHEADER 구조체의 데이터 */
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	/* BITMAPINFOHEADER 구조체의 데이터 */
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	//fwrite(inimg, sizeof(ubyte), imageSize, fp);
	fwrite(outimg, sizeof(ubyte), imageSize, fp);
	fclose(fp);

	free(inimg);
	free(outimg);

	return 0;                        
}
