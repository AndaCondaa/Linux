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

    printf("x-resolution : %d\n", fbvar.xres);
    printf("y-resolution : %d\n", fbvar.yres);
    printf("x-resolution(virtual) : %d\n", fbvar.xres_virtual);	
    printf("y-resolution(virtual) : %d\n", fbvar.yres_virtual);
    printf("bpp : %d\n", fbvar.bits_per_pixel);
    printf("length of frame buffer memory : %d\n", fbfix.smem_len);


    if(lseek(fd, offset, SEEK_SET) < 0) {
        printf("ERROR fbdev lseek \n");
        return 1;	   	       /* exit(1) */
    }

    
    for( int j = 0; j< fbvar.yres; j++)
	for(int i =0; i<fbvar.xres; i++) {
		int offset = (j * fbvar.xres + i) * NO_OF_COLOR;
		lseek(fd, offset, SEEK_SET);
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
		write(fd, color, NO_OF_COLOR);
	}
   
    
    


    close(fd);
    return 0;
}

