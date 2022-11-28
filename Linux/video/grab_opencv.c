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

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define FBDEVFILE "/dev/fb0"

#define	COLS	640
#define	ROWS	480

int main(int argc, char** argv)
{
	int fd, fbfd, n_max_count = 100;
	unsigned char *buffer, r, g, b;
	unsigned int i, x, y, t, size;
	unsigned short *pfbmap, out_data[COLS*ROWS*2], pixel;
	CvCapture* capture;
	CvVideoWriter* writer;
	IplImage *frame;

	capture = cvCaptureFromCAM(0);
	if(capture == 0) {
		perror("OpenCV : open WebCam\n");
		return -1;
	}

	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	
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

	for(i=t=b=g=r=0; i < n_max_count; i++) {
		cvGrabFrame(capture);
		frame = cvRetrieveFrame(capture);
		buffer = (uchar*)frame->imageData;
		for(x = 0; x < frame->height; x++) {
			t = x * frame->width;
			for(y = 0; y < frame->width; y++) {
				r = *(buffer + (t + y)*3 + 2);
				g = *(buffer + (t + y)*3 + 1);
				b = *(buffer + (t + y)*3 + 0);
				pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
				
				out_data[y+t+(COLS-frame->width)*x] = pixel;
			}
		}
		
		memcpy(pfbmap, out_data, COLS*ROWS*2);
	};
	
	munmap(pfbmap, frame->width*frame->height*2);
	
	close(fd);
	return 0;
}
