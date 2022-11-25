#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>

#define  FB                      "/dev/fb0"
#define  NO_OF_COLOR   2

typedef unsigned char ubyte;
unsigned short makepixel(ubyte r, ubyte g, ubyte b) {
	return (unsigned short)( ((r>>3)<<11) | ((g>>2)<<5) | (b>>3) );
}
 
int main(int argc, char** argv) 
{
    int fd, status, offset;
    unsigned short pixel;
    unsigned char color[NO_OF_COLOR];
    struct fb_var_screeninfo fbvar;
    struct fb_fix_screeninfo fbfix;

    fd = open(FB, O_RDWR);
    if(fd < 0) {
         printf("Error fbdev open\n"); 
         return -1;
    }

    status = ioctl(fd, FBIOGET_FSCREENINFO, &fbfix);
    status = ioctl(fd, FBIOGET_VSCREENINFO, &fbvar);
 /*   
    for( int y = 0; y<fbvar.yres; y++)
	for(int x =0; x<fbvar.xres; x++) {
		offset = (x + y * fbvar.xres) * NO_OF_COLOR;
		lseek(fd, offset, SEEK_SET);
 		pixel = makepixel(0,0,255);
		write(fd, &pixel, NO_OF_COLOR);
	}
*/
    int width = fbvar.xres_virtual, height = fbvar.yres_virtual;
    for (int y = 0; y < height; y++) { 
	for (int x = 0; x < (width/3); x++) {
		offset = (x + y * width)*NO_OF_COLOR;
		lseek (fd, offset, SEEK_SET);
		pixel = makepixel(255, 0, 0);
		write(fd, &pixel, NO_OF_COLOR);
	}
    }

    for (int y = 0; y < height; y++) { 
	for (int x = (width/3); x < (2*width/3); x++) {
		offset = (x + y * width)*NO_OF_COLOR;
		lseek (fd, offset, SEEK_SET);
		pixel = makepixel(255, 255, 255);
		write(fd, &pixel, NO_OF_COLOR);
	}
    }

    for (int y=0; y < height; y++) { 
	for (int x =0; x < (width/3); x++) {
		offset = (x + y * width)*NO_OF_COLOR;
		lseek (fd, offset, SEEK_SET);
		pixel = makepixel(255, 0, 0);
		write(fd, &pixel, NO_OF_COLOR);
	}
    }

    close(fd);
    return 0;
}

