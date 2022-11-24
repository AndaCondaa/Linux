#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define FBDEVICE "/dev/fb1"

extern inline unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b)
{
	return (unsigned short)(((r>>3)<<11)|((g>>2)<<5)|(b>>3));
}

int main(void)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	unsigned short *pfb;
	int fbfd, size;

	fbfd = open(FBDEVICE, O_RDWR);
	if(fbfd < 0){
		perror("Error: cannot open framebuffer device");
		return EXIT_FAILURE;
	}

	ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
	if(strcmp(finfo.id, "RPi-Sense FB") != 0) {
		printf("%s\n", "Error: RPi-Sense FB not found");
		close(fbfd); 
		return EXIT_FAILURE;
	}

	if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
		perror("Error reading fixed information");
		return EXIT_FAILURE;
	}

	size = vinfo.xres * vinfo.yres * sizeof(short);

	pfb = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (pfb == MAP_FAILED) {
		close(fbfd);
		perror("Error mmapping the file");
		return EXIT_FAILURE;
	}

	memset(pfb, 0, size);
	//*(pfb+4+0*vinfo.xres) = makepixel(55, 0, 0);
	//*(pfb+5+0*vinfo.xres) = makepixel(55, 0, 0);
	//*(pfb+4+1*vinfo.xres) = makepixel(55, 0, 0);
	//*(pfb+5+1*vinfo.xres) = makepixel(55, 0, 0);
	//*(pfb+4+2*vinfo.xres) = makepixel(55, 0, 0);
	//*(pfb+5+2*vinfo.xres) = makepixel(55, 0, 0);





	if (munmap(pfb, size) == -1) {
		perror("Error un_mmapping the file");
	}

	close(fbfd);

	return EXIT_SUCCESS;
}

	
