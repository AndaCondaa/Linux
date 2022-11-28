#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/types.h>
#include <linux/videodev.h>

#define VIDEODEVFILE "/dev/video0"
#define FBDEVFILE "/dev/fb0"

#define	COLS	640
#define	ROWS	480

int main(int argc, char** argv)
{
	int fd, fbfd, n_max_count = 100;
	unsigned char *buffer, r, g, b;
	unsigned int i, x, y, t, size;
	unsigned short *pfbmap, out_data[COLS*ROWS], pixel;
	
	struct video_capability cap;
	struct video_window win;
	struct video_picture vpic;
	
	fd = open(VIDEODEVFILE, O_RDWR);
	if(fd < 0) {
		perror(VIDEODEVFILE);
		return -1;
	}
	
	fbfd = open(FBDEVFILE, O_RDWR);
	if(fbfd < 0) {
		perror(FBDEVFILE);
		return -1;
	}
	
	pfbmap = (unsigned short *)mmap(0, COLS*ROWS*2, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	if((unsigned)pfbmap == (unsigned)-1) {
		perror("fbdev mmap");
		return -1;
	}
	
	if (ioctl(fd, VIDIOCGCAP, &cap) < 0) {
		perror("VIDIOGCAP");
		perror("(" VIDEODEVFILE " not a video4linux device?)\n");
		close(fd);
		return -1;
	}
	
	if (ioctl(fd, VIDIOCGWIN, &win) < 0) {
		perror("VIDIOCGWIN");
		close(fd);
		return -1;
	}
	
	if (ioctl(fd, VIDIOCGPICT, &vpic) < 0) {
		perror("VIDIOCGPICT");
		close(fd);
		return -1;
	}
	
	if (cap.type & VID_TYPE_MONOCHROME) {
		vpic.depth=8;
		vpic.palette=VIDEO_PALETTE_GREY;	/* 8bit grey */
		if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
			vpic.depth=6;
			if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
				vpic.depth=4;
				if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
					perror("Unable to find a supported capture format.\n");
					close(fd);
					return -1;
				}
			}
		}
	} else {
		vpic.depth=24;
		vpic.palette=VIDEO_PALETTE_RGB24;
		
		if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
			vpic.palette=VIDEO_PALETTE_RGB565;
			vpic.depth=16;
			
			if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
				vpic.palette=VIDEO_PALETTE_RGB555;
				vpic.depth=15;
				
				if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
					vpic.palette=VIDEO_PALETTE_YUV420P;
					vpic.depth=15;
					printf("VIDEO_PALETTE_YUV420P\n");
					
					if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
						perror("Unable to find a supported capture format.\n");
						return -1;
					}
				}
			}
		}
	}
	
	buffer = malloc(win.width * win.height * vpic.depth);
	if (!buffer) {
		perror("Out of memory.\n");
		return -1;
	}
	
	printf("Width : %d, Height : %d\n", win.width, win.height);
	
	for(i=t=b=g=r=0; i < n_max_count; i++) {
		size = read(fd, buffer, win.width * win.height * vpic.depth);		
		for(x = 0; x < win.height; x++) {
			t = x * win.width;
			for(y = 0; y < win.width; y++) {
				r = *(buffer + (t + y)*3 + 2);
				g = *(buffer + (t + y)*3 + 1);
				b = *(buffer + (t + y)*3 + 0);
				pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
				
				out_data[y+t+(COLS-win.width)*x] = pixel;
			}
		}
		
		memcpy(pfbmap, out_data, COLS*ROWS*2);
	};
	
	munmap(pfbmap, win.width*win.height*2);
	free(buffer);
	
	close(fd);
	return 0;
}