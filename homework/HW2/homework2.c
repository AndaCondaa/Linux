/*
 * HomeWork #2
 * Copy the BITMAP File by using only Bitmap headers. Add Pal(256).
 */

#include <stdio.h>
#include <stdlib.h>

#define ubyte unsigned char

int main(int argc, char **argv)
{
	FILE *file;

	BITMAPFILEHEADER bmpHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	ubyte *inimg, *outimg;
	RGBQUAD *pal;

	file = fopen(argv[1], "rb");

	fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, file);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);


}
