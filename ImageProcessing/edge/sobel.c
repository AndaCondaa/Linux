#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BYTE	unsigned char
#define BASE    16

typedef struct tagRGBQUAD {
	BYTE    rgbBlue; 
	BYTE    rgbGreen; 
	BYTE    rgbRed; 
	BYTE    rgbReserved; 
} RGBQUAD; 

#define widthbytes(bits)   (((bits)+31)/32*4)

int main(int argc, char** argv)
{
	FILE *fp;
	RGBQUAD palrgb[256];
	
	unsigned short int type;  
	unsigned int file_size;   
	unsigned short int reserved1; 
	unsigned short int reserved2; 
	unsigned int offset;   
	unsigned int header_size;  
	int width,height;    
	unsigned short int planes;  
	unsigned short int bits;  
	unsigned int compression;  
	unsigned int imagesize;   
	int hresolution,vresolution; 
	unsigned int ncolors;   
	unsigned int importantcolors; 
	char input[128], output[128];
	
	float r, g, b, gray;
	
	int c, i, j, size, index;
	
	unsigned char *inimg, *midimg, *outimg;
    double hedge, vedge;
	
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);
	
	if((fp = fopen(input, "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		exit(EXIT_FAILURE);
	}
	
	fread(&type, sizeof(unsigned short int), 1, fp);
	fread(&file_size, sizeof(unsigned int), 1, fp);
	fread(&reserved1, sizeof(unsigned short int), 1, fp);
	fread(&reserved2, sizeof(unsigned short int), 1, fp);
	fread(&offset, sizeof(unsigned int), 1, fp);
	fread(&header_size, sizeof(unsigned int), 1, fp);
	fread(&width, sizeof(int), 1, fp);
	fread(&height, sizeof(int), 1, fp);
	fread(&planes, sizeof(unsigned short int), 1, fp);
	fread(&bits, sizeof(unsigned short int), 1, fp);
	fread(&compression, sizeof(unsigned int), 1, fp);
	fread(&imagesize, sizeof(unsigned int), 1, fp);
	fread(&hresolution, sizeof(int), 1, fp);
	fread(&vresolution, sizeof(int), 1, fp);
	fread(&ncolors, sizeof(unsigned int), 1, fp);
	fread(&importantcolors, sizeof(unsigned int), 1, fp);
	
	size = widthbytes(bits * width);
	
	if(!imagesize) imagesize = height * size;
	inimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	midimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	outimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	fread(inimg, sizeof(BYTE), imagesize, fp);
	fclose(fp);
	
	for(i = 0; i < height; i++) {
		index = (height-i-1) * size; 
		for(j = 0; j < width; j++) { 
			r = (float)inimg[index+3*j+2];
			g = (float)inimg[index+3*j+1];
			b = (float)inimg[index+3*j+0];
			gray=(r*0.3F)+(g*0.59F)+(b*0.11F);
			
			midimg[index+3*j] = midimg[index+3*j+1] = midimg[index+3*j+2] = gray;
		};
	};
	
#if 1		// Sobel
	for(i = 1; i < height - 1; i++) {
		index = (height-i-1) * size; 
		for(j = 1; j < width - 1; j++) { 
			hedge = midimg[index-1+3*(j+1)]-midimg[index-1+3*(j-1)] \
			+ 2*(midimg[index+3*(j+1)]-midimg[index+3*(j-1)]) \
			+ midimg[index+1+3*(j+1)]-midimg[index+1+3*(j-1)];
			
			//			ch[y-1][x+1]-ch[y-1][x-1] + 2*(ch[y][x+1]-ch[y][x-1]) + ch[y+1][x+1]-ch[y+1][x-1];
			vedge = midimg[index-1+3*(j-1)]-midimg[index+1+3*(j-1)] \
			+ 2*(midimg[index-1+3*(j)]-midimg[index+1+3*(j)]) \
			+ midimg[index-1+3*(j+1)]-midimg[index+1+3*(j+1)];
			//			ch[y-1][x-1]-ch[y+1][x-1]+2*(ch[y-1][x]-ch[y+1][x])+ch[y-1][x+1]-ch[y+1][x+1];
			c=sqrt(hedge*hedge+vedge*vedge);
			if (c>255) c=255; else c = 0; 
			outimg[index+3*j] = outimg[index+3*j+1] = outimg[index+3*j+2] = c;
		};
	};
#endif	   
	offset += 256*sizeof(RGBQUAD); 
	
	if((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		exit(EXIT_FAILURE);
	}
	
	fwrite(&type, sizeof(unsigned short int), 1, fp);
	fwrite(&file_size, sizeof(unsigned int), 1, fp);
	fwrite(&reserved1, sizeof(unsigned short int), 1, fp);
	fwrite(&reserved2, sizeof(unsigned short int), 1, fp);
	fwrite(&offset, sizeof(unsigned int), 1, fp);
	fwrite(&header_size, sizeof(unsigned int), 1, fp);
	fwrite(&width, sizeof(int), 1, fp);
	fwrite(&height, sizeof(int), 1, fp);
	fwrite(&planes, sizeof(unsigned short int), 1, fp);
	fwrite(&bits, sizeof(unsigned short int), 1, fp);
	fwrite(&compression, sizeof(unsigned int), 1, fp);
	fwrite(&imagesize, sizeof(unsigned int), 1, fp);
	fwrite(&hresolution, sizeof(int), 1, fp);
	fwrite(&vresolution, sizeof(int), 1, fp);
	fwrite(&ncolors, sizeof(unsigned int), 1, fp);
	fwrite(&importantcolors, sizeof(unsigned int), 1, fp);
	fwrite(palrgb, sizeof(unsigned int), 256, fp);
	fwrite(outimg, sizeof(unsigned char), imagesize, fp);
	
	free(inimg);
	free(midimg);
	free(outimg);
	
	fclose(fp);
	
	return 0;
}
