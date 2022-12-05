#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

inline unsigned char clip(int value, int min, int max);
unsigned char clip(int value, int min, int max)
{
	return(value > max? max : value < min? min: value);
}

int main(int argc, char** argv) 
{
	FILE* fp; 
	BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
	BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
	RGBQUAD *palrgb;
	ubyte *inimg, *outimg, *padding;
	int x, y, z, imageSize;

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

	int elemSize = bmpInfoHeader.biBitCount/8;
	int size = bmpInfoHeader.biWidth*elemSize;
	imageSize = size * bmpInfoHeader.biHeight; 

	/* 이미지의 해상도(넓이 × 깊이) */
	printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
	printf("Image Size : %d\n", imageSize);

	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
	padding = (ubyte*)malloc(sizeof(ubyte)*(bmpInfoHeader.biWidth+2)*(bmpInfoHeader.biHeight+2)*3);
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	fread(inimg, sizeof(ubyte), imageSize, fp); 

	fclose(fp);

	//define padding image
	memset(padding, 0, sizeof(ubyte)*(bmpInfoHeader.biWidth+2)*(bmpInfoHeader.biHeight+2)*3);
	for(int j = 0; j < bmpInfoHeader.biHeight+2; j++) {
		for (int i = 0; i < bmpInfoHeader.biWidth+2; i++){
			for(int z = 0; z < 3; z++) {
				if (i == 0 && (j>0 && j<bmpInfoHeader.biHeight+1)) {
					padding[(j*(bmpInfoHeader.biWidth+2)*3)+z] = inimg[((j-1)*bmpInfoHeader.biWidth*3)+z];
 				} else if (i == bmpInfoHeader.biWidth+1 && (j>0 && j<bmpInfoHeader.biHeight+1)) {
					padding[i*3+(j*(bmpInfoHeader.biWidth+2)*3)+z] = inimg[(i-2)*3+((j-1)*bmpInfoHeader.biWidth*3)+z];
				} else if (j == 0 && (i>0 && i<bmpInfoHeader.biWidth+1)) {
					padding[(i*3)+z] = inimg[((i-1)*3)+z];
				} else if (j == bmpInfoHeader.biHeight+1 && (i>0 && i<bmpInfoHeader.biWidth+1)) {
					padding[i*3+(j*(bmpInfoHeader.biWidth+2)*3)+z] = inimg[(i-1)*3+((j-2)*bmpInfoHeader.biWidth*3)+z];
				} else {
					padding[i*3+(j*bmpInfoHeader.biWidth+2)*3+z] = inimg[(i-1)*3+((j-1)*bmpInfoHeader.biWidth*3)+z];
				}
			}
		}
	}

	
	// define the kernel
	float kernel[3][3] = { {0, -1, 0},
			       {-1, 5, -1},
			       {0, -1, 0} };
	memset(outimg, 0, sizeof(ubyte)*imageSize);
	for(y=1; y < bmpInfoHeader.biHeight+1; y++) {
		for(x=1; x < (bmpInfoHeader.biWidth+1); x++) {
			for(z=0; z < elemSize; z++) {
				float sum = 0.0;
				for (int i = -1; i < 2; i++) {
					for(int j = -1; j < 2; j++) {
						sum += kernel[i+1][j+1]*padding[(x+i)*3+(y+j)*(bmpInfoHeader.biWidth+2)*3+z];
					}
				}
				outimg[(x-1)*3+(y-1)*(bmpInfoHeader.biWidth*3)+z] = clip(sum, 0, 255);
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

	fwrite(outimg, sizeof(ubyte), imageSize, fp);
	fclose(fp); 

	free(inimg); 
	free(padding);
	free(outimg);

	return 0;
}
