#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BYTE	unsigned char
#define widthbytes(bits)   (((bits)+31)/32*4)

#ifndef M_PI
#define M_PI	3.141592654
#endif

typedef struct tagRGBQUAD {
	BYTE    rgbBlue; 
	BYTE    rgbGreen; 
	BYTE    rgbRed; 
	BYTE    rgbReserved; 
} RGBQUAD; 

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
	int width, height;    
	unsigned short int planes;  
	unsigned short int bits;  
	unsigned int compression;  
	unsigned int imagesize;   
	int hresolution,vresolution; 
	unsigned int ncolors, importantcolors; 
	char input[128], output[128];
	
	int i, j, size, index;
	double radius, cos_value, sin_value;
	int centerX, centerY;
	int degree = 45;
	
	unsigned char *inimg, *outimg;
	
	strcpy(input, argv[1]);
	strcpy(output, argv[2]);
	
	if((fp = fopen(input, "rb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		return -1;
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
	outimg = (BYTE*)malloc(sizeof(BYTE)*imagesize);
	fread(inimg, sizeof(BYTE), imagesize, fp);
	fclose(fp);
	
	radius = degree*(M_PI/180.0f);
	sin_value = sin(radius);
	cos_value = cos(radius);
	centerX = height/2;
	centerY = width/2;
	
	for(i = 0 ; i < height; i++) { 
		index = (height-i-1) * size; 
		for(j = 0 ; j < width; j++) { 
			double new_x;
			double new_y;
			new_x = (i-centerX)*cos_value - (j-centerY)*sin_value + centerX;
			new_y = (i-centerX)*sin_value + (j-centerY)*cos_value + centerY; 
			
			if(new_x <0 || new_x > height) {
				outimg[index+3*j+0] = 0;
				outimg[index+3*j+1] = 0;
				outimg[index+3*j+2] = 0;
			} else if (new_y <0 || new_y > width) {
				outimg[index+3*j+0] = 0;
				outimg[index+3*j+1] = 0;
				outimg[index+3*j+2] = 0;
			} else {
				outimg[index+3*j+0] = inimg[(int)(height-new_x-1)*size+(int)new_y*3+0];
				outimg[index+3*j+1] = inimg[(int)(height-new_x-1)*size+(int)new_y*3+1];
				outimg[index+3*j+2] = inimg[(int)(height-new_x-1)*size+(int)new_y*3+2];
			}
		};
	};
	
	//offset += 256*sizeof(RGBQUAD); 
	
	if((fp = fopen(output, "wb")) == NULL) {
		fprintf(stderr, "Error : Failed to open file...\n");
		return -1;
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
	//fwrite(palrgb, sizeof(unsigned int), 256, fp);
	fwrite(outimg, sizeof(unsigned char), imagesize, fp);
	
	free(inimg);
	free(outimg);
	
	fclose(fp);
	
	return -1;
}
