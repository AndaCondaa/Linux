/*
 * HomeWork #2
 * BMP 파일 헤더만 이용해서 BMP 파일을 복사하세요. 
 * 단, 복사되는 파일은 임의의 팔레트 256개를 추가해야 합니다.
 * 팔레트를 추가하면서 사용법을 확인하기 위해, 8비트 그레이스케일로 변환하는 코드 작성
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

#define PAL_COUNT 256			// 추가할 팔레트의 수

unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width);

int main(int argc, char **argv)
{
	// 변수 선언
	FILE *file;
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *inImage, *outImage; 
	RGBQUAD *palette;
	unsigned int widthBytes, pixelCount, index;
	float Bpp;
	
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
	widthBytes = paddingWidthBytes(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);
	Bpp = bmpInfoHeader.biBitCount / 8.;		// 픽셀 당 바이트 수 (채널 수)
	pixelCount = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight;	// 픽셀 수
	
	// 이미지의 사이즈 계산
	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBytes * bmpInfoHeader.biHeight;
	}
	
	// Read/Write를 위한 버퍼공간 생성 및 초기화
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage); 
	outImage = (unsigned char*)malloc(sizeof(unsigned char) * pixelCount * 1);
	memset(inImage, 0, bmpInfoHeader.SizeImage);
	memset(outImage, 0, pixelCount);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	// 팔레트 생성
	// 그레이스케일 이미지를 만들기 위해 RGB값이 동일한 256개의 팔레트 생성
	palette = (RGBQUAD*)malloc(sizeof(RGBQUAD) * PAL_COUNT);
	for (int i = 0; i < PAL_COUNT; i++) {
		palette[i].rgbBlue = i;
		palette[i].rgbGreen = i;
		palette[i].rgbRed = i;
	}

	// RGB값을 읽어서, 그레이스케일로 변환
	for (int y = 0; y < bmpInfoHeader.biHeight; y++) {
		for (int x = 0; x < bmpInfoHeader.biWidth; x++) { 
			index = (x * Bpp) + (y * widthBytes);
			int r = inImage[index+2];
			int g = inImage[index+1];
			int b = inImage[index];
			int gray = (0.299 * r) + (0.587 * g) + (0.144 * b);
			outImage[x + (y * bmpInfoHeader.biWidth)] = gray;
		}
	}

	// 출력 파일 Open
	if ((file = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[2]);
		return -1;
	}

	// 헤더 정보 변경
	bmpInfoHeader.biBitCount = 8;	// 8bit 이미지
	bmpInfoHeader.SizeImage = bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1;	// 이미지의 사이즈
	bmpInfoHeader.biClrUsed = 0;	// 8bit 이미지에서 팔레트를 256개 모두 사용한 경우, 0으로 설정
	bmpHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * PAL_COUNT + bmpInfoHeader.SizeImage;
	bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * PAL_COUNT;	// offset 변경

	// 출력 파일 작성
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(palette, sizeof(RGBQUAD), PAL_COUNT, file); 
	fwrite(outImage, sizeof(unsigned char), bmpInfoHeader.biWidth*bmpInfoHeader.biHeight*1, file);
	fclose(file);

	// 동적 메모리 해제
	free(inImage); 
	free(outImage);
	free(palette);

	return 0;
}

// 패딩을 고려한 가로 한 줄의 전체 바이트 수를 계산하는 함수
unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
