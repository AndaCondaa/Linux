/*
 * HomeWork #1
 * BMP 파일 헤더(책 480 페이지)만 이용해서 BMP 파일을 읽고 픽셀의 값을 출력하는 프로그램을 작성하세요.
 * Input: BMP 파일 이름
 * Output: (255, 255, 255)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width);

int main(int argc, char **argv)
{
	// 변수 선언
	FILE *file;								
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *inImage; 
	unsigned int widthBytes, index;

	// 명령행 인수의 수 검증
	if (argc != 2) {
		fprintf(stderr, "This file(%s) needs two of Command Line Arguments.\n", __FILE__);
		return -1;
	}

	// BMP 파일 Open
	if ((file = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[1]);
		return -1;
	}

	// BMP 파일의 헤더 읽기
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	// 24비트 이미지를 사용하였는지 검증 - 24비트가 아닐 경우 종료
	if (bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24bit image\n");
		return -1;	
	}
	
	// 패딩을 고려한 가로의 바이트 수 계산 
	widthBytes = paddingWidthBytes(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	// 이미지의 사이즈 계산
	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBytes * bmpInfoHeader.biHeight;
	}
	
	// 버퍼 공간을 만들어서, 이미지 데이터를 읽기
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	// 출력 이미지 기준으로 좌상단 픽셀부터 우하단 방향으로 RGB값 출력
	// BMP 파일은 이미지가 상하반전된 형태로 데이터가 저장되므로, 이를 고려하여 출력
	for (int y=bmpInfoHeader.biHeight-1; y >= 0 ; y--) {
		for (int x=0; x < bmpInfoHeader.biWidth; x++) { 
			index = (x*3) + (y*widthBytes); 
			printf("(r = %3d, g = %3d, b = %3d)\n",
	       			inImage[index+2], inImage[index+1], inImage[index]);
		}
	}

	free(inImage);

	return 0;
}

// 패딩을 고려한 가로 한 줄의 전체 바이트 수를 계산하는 함수
unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
