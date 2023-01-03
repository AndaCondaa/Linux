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

//Cuda kernel for converting RGB image into a GreyScale image
__global__ void convertToGrey(ubyte *rgb, ubyte *out, int rows, int cols, int elemSize) {
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int row = threadIdx.y + blockIdx.y * blockDim.y;

	// Compute for only those threads which map directly to image grid
	if (col < cols && row < rows) {
		int offset = (row * cols + col) * elemSize;

		ubyte r = rgb[offset + 2];
		ubyte g = rgb[offset + 1];
		ubyte b = rgb[offset + 0];

		out[offset+2] = LIMIT_UBYTE(r*0.393 + g*0.769 + b*0.189);
		out[offset+1] = LIMIT_UBYTE(r*0.349 + g*0.686 + b*0.168);
		out[offset+0] = LIMIT_UBYTE(r*0.272 + g*0.534 + b*0.131);
	}
}

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

	int elemSize = bmpInfoHeader.biBitCount/8.;
	int stride = bmpInfoHeader.biWidth * elemSize;

	//widthbytes(bits) (((bits)+31)/32*4)
	int imageSize = stride * bmpInfoHeader.biHeight;

	/* 이미지의 해상도(넓이 × 깊이) */
	printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	printf("Bit Count : %d(%d:%d)\n", bmpInfoHeader.biBitCount, elemSize, stride); /* 픽셀당 비트
											  수(색상) */
	printf("Image Size : %d\n", imageSize);
	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	fread(inimg, sizeof(ubyte), imageSize, fp);

	fclose(fp);
#if 0
	for(int y = 0; y < bmpInfoHeader.biHeight; y++) {
		for(int x = 0; x < stride; x+=elemSize) {
			ubyte b = inimg[x+y*stride+0];
			ubyte g = inimg[x+y*stride+1];
			ubyte r = inimg[x+y*stride+2];
			//outimg[x/elemSize+y*bmpInfoHeader.biWidth]=((66*r+129*g+25*b+128)>>8)+16;
			outimg[x/elemSize+y*bmpInfoHeader.biWidth]=(r*0.299F)+(g*0.587F)+(b*0.114F);
		}
	}
#else
	ubyte *d_inimg = NULL, *d_outimg = NULL;
	//allocate and initialize memory on device
	cudaMalloc(&d_inimg, sizeof(ubyte) * imageSize);
	cudaMalloc(&d_outimg, sizeof(ubyte) * imageSize);
	cudaMemset(d_outimg, 0, sizeof(ubyte) * imageSize);

	//copy host rgb data array to device rgb data array
	cudaMemcpy(d_inimg, inimg, sizeof(ubyte) * imageSize, cudaMemcpyHostToDevice);

	//define block and grid dimensions
	const dim3 dimGrid((int)ceil((bmpInfoHeader.biWidth/32)), (int)ceil((bmpInfoHeader.biHeight)/16));
	const dim3 dimBlock(32, 16);

	//execute cuda kernel
	convertToGrey<<<dimGrid, dimBlock>>>(d_inimg, d_outimg, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth, elemSize);

	//copy computed gray data array from device to host
	cudaMemcpy(outimg, d_outimg, sizeof(ubyte) * imageSize, cudaMemcpyDeviceToHost);
	cudaFree(d_outimg);
	cudaFree(d_inimg);

#endif
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

