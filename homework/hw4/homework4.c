/*
 * HomeWork #4
 * 배열로 작성된 코드를 포인터로 변경하세요.
 * 변경사항 : 65행
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
	unsigned char *inImage, *outImage; 
	unsigned int widthBytes, index;

	// 명령행 인수의 수 검증
	if (argc != 3) {
		fprintf(stderr, "This file(%s) needs three of Command Line Arguments.\n", __FILE__);
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
	widthBytes = paddingWidthBits(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	// 이미지의 사이즈 계산
	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBytes * bmpInfoHeader.biHeight;
	}
	
	// Read/Write를 위한 버퍼 공간 생성 및 초기화
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	outImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);
	memset(outImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	// 이미지를 읽어서 출력 버퍼공간에 복사
	for (int i = 0; i < bmpInfoHeader.SizeImage; i++) {
		*(outImage + i) = *(inImage + i);
	}

	// 출력 파일 Open
	if ((file = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[2]);
		return -1;
	}

	// 출력 파일 작성
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(outImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);

	// 동적 메모리 해제
	free(inImage);
	free(outImage);

	return 0;
}

// 패딩을 고려한 가로 한 줄의 전체 바이트 수를 계산하는 함수
unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
