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

    fbvar.bits_per_pixel = 16;
    ioctl(fd, FBIOPUT_VSCREENINFO, &fbvar);
    offset = (150 + 150 * fbvar.xres) * NO_OF_COLOR;
    lseek(fd, offset, SEEK_SET);
    pixel = makepixel(255, 0, 0);    /* Red */
    write(fd, &pixel, NO_OF_COLOR);
    offset = (200 + 200 * fbvar.xres) * NO_OF_COLOR;
    lseek(fd, offset, SEEK_SET);
    pixel = makepixel(0, 255, 0);   /*  Green */
    write(fd, &pixel, NO_OF_COLOR);

    offset = (250 + 250 * fbvar.xres) * NO_OF_COLOR;
    if(lseek(fd, offset, SEEK_SET) < 0) {
        printf("ERROR fbdev lseek \n");
        return 1;	   	       /* exit(1) */
    }
    pixel = makepixel(0, 0, 255);    /* Blue */ 
    write(fd, &pixel, NO_OF_COLOR);

    close(fd);
    return 0;
}

