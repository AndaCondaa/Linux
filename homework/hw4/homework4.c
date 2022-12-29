/*
 * HomeWork #4
 * �迭�� �ۼ��� �ڵ带 �����ͷ� �����ϼ���.
 * ������� : 65��
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
	unsigned char *inImage, *outImage; 
	unsigned int widthBytes, index;

	// ����� �μ��� �� ����
	if (argc != 3) {
		fprintf(stderr, "This file(%s) needs three of Command Line Arguments.\n", __FILE__);
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
	widthBytes = paddingWidthBits(bmpInfoHeader.biBitCount, bmpInfoHeader.biWidth);

	// �̹����� ������ ���
	if (!bmpInfoHeader.SizeImage) {
		bmpInfoHeader.SizeImage = widthBytes * bmpInfoHeader.biHeight;
	}
	
	// Read/Write�� ���� ���� ���� ���� �� �ʱ�ȭ
	inImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	outImage = (unsigned char*)malloc(sizeof(unsigned char) * bmpInfoHeader.SizeImage);
	memset(inImage, 0, bmpInfoHeader.SizeImage);
	memset(outImage, 0, bmpInfoHeader.SizeImage);

	fread(inImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);
	
	// �̹����� �о ��� ���۰����� ����
	for (int i = 0; i < bmpInfoHeader.SizeImage; i++) {
		*(outImage + i) = *(inImage + i);
	}

	// ��� ���� Open
	if ((file = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Failed to open file(%s)\n", argv[2]);
		return -1;
	}

	// ��� ���� �ۼ�
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	fwrite(outImage, sizeof(unsigned char), bmpInfoHeader.SizeImage, file);
	fclose(file);

	// ���� �޸� ����
	free(inImage);
	free(outImage);

	return 0;
}

// �е��� ����� ���� �� ���� ��ü ����Ʈ ���� ����ϴ� �Լ�
unsigned int paddingWidthBytes(unsigned short bitCount, unsigned int width)
{
	unsigned int widthBits = bitCount * width;
	unsigned int padWidth = ((widthBits+31)/32)*4;

	return padWidth;
}
