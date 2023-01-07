/*
 * HomeWork #1
 * BMP ���� ���(å 480 ������)�� �̿��ؼ� BMP ������ �а� �ȼ��� ���� ����ϴ� ���α׷��� �ۼ��ϼ���.
 * Input: BMP ���� �̸�
 * Output: (255, 255, 255)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpHeader.h"

unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width);

int main(int argc, char **argv)
{
	// ���� ����
	FILE *file;								
	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	unsigned char *inImage; 
	unsigned int widthBytes, index;

	// ����� �μ��� �� ����
	if (argc != 2) {
		fprintf(stderr, "This file(%s) needs two of Command Line Arguments.\n", __FILE__);
		return -1;
	}

	// BMP ���� Open
	if ((file = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[1]);
		return -1;
	}

	// BMP ������ ��� �б�
	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	// 24��Ʈ �̹����� ����Ͽ����� ���� - 24��Ʈ�� �ƴ� ��� ����
	if (bmpInfoHeader.biBitCount != 24) {
		perror("You can use only 24bit image\n");
		return -1;	
	}
	
	// �е��� ����� ������ ����Ʈ �� ��� 
	widthBytes = paddingWidthBytes(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	// �̹����� ������ ���
	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBytes * bmpInfoHeader.biHeight;
	}
	
	// ���� ������ ����, �̹��� �����͸� �б�
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	// ��� �̹��� �������� �»�� �ȼ����� ���ϴ� �������� RGB�� ���
	// BMP ������ �̹����� ���Ϲ����� ���·� �����Ͱ� ����ǹǷ�, �̸� ����Ͽ� ���
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

// �е��� ����� ���� �� ���� ��ü ����Ʈ ���� ����ϴ� �Լ�
unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
