/*
LD_PRELOAD=/usr/lib/libv4l/v4l1compat.so ./grab_mmap
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include <liboil/liboil.h>

#define VIDEODEVFILE 	"/dev/video0"
#define FBDEVFILE 	"/dev/fb0"

#define	WIDTH	640
#define	HEIGHT	480

//#define USE_MMAP
//#define USE_LIBOIL

int main(int argc, char** argv)
{
	int fd, fbfd, n_max_count = 10;
	unsigned char *buffer;
	unsigned char r, g, b;
	unsigned int i, x, y, t, size;
	unsigned short *pfbmap, out_data[WIDTH*HEIGHT*2], pixel;
#ifdef USE_MMAP
	struct video_mbuf vmbuf;
	struct video_mmap vmm;
#endif
	
	struct video_buffer vbuf;
	struct video_capability cap;
	struct video_window vwin;
	struct video_picture vpic;
	struct video_channel chan;
	struct video_audio audio;

#ifdef USE_LIBOIL
	unsigned char *tmp;
	oil_init();
#endif

	fbfd = open(FBDEVFILE, O_RDWR);
	if(fbfd < 0) {
		perror(FBDEVFILE);
		return -1;
	}

	pfbmap = (unsigned short *)mmap(NULL, WIDTH*HEIGHT*2, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
	if((unsigned)pfbmap == (unsigned)-1) {
		perror("fbdev mmap");
		return -1;
	}
	
	//fd = open(VIDEODEVFILE, O_RDWR);
	fd = open(VIDEODEVFILE, O_RDONLY);
	if(fd < 0) {
		perror(VIDEODEVFILE);
		return -1;
	}
	
	if (ioctl(fd, VIDIOCGCAP, &cap) < 0) {
		perror("VIDIOGCAP");
		perror("("VIDEODEVFILE " not a video4linux device?)\n");
		close(fd);
		return -1;
	}

	printf("%s - %d(%d/%d)\n", cap.name, cap.type, cap.maxwidth, cap.maxheight);
	printf("channel : %d\n", cap.channels);

	if(ioctl(fd, VIDIOCGAUDIO, &audio) < 0) {
		perror("VIDIOCGAUDIO");
		//return -1;
	}

	chan.channel = 0;
	if(ioctl(fd, VIDIOCGCHAN, &chan) < 0) {
		perror("VIDIOCGCHAN");
		close(fd);
		return -1;
	}

	chan.flags = 0;
	chan.type = VIDEO_TYPE_CAMERA;
	chan.norm = VIDEO_MODE_NTSC;
	if(ioctl(fd, VIDIOCGCHAN, &chan) < 0) {
		perror("VIDIOCGCHAN");
		close(fd);
		return -1;
	}
	
	vwin.width = WIDTH;
	vwin.height = HEIGHT;
	vwin.x = vwin.y = vwin.chromakey = vwin.flags = 0;
	if (ioctl(fd, VIDIOCSWIN, &vwin) < 0) {
		perror("VIDIOCSWIN");
		close(fd);
		return -1;
	}
	
	if (ioctl(fd, VIDIOCGWIN, &vwin) < 0) {
		perror("VIDIOCGWIN");
		close(fd);
		return -1;
	}
	printf("Width : %d, Height : %d\n", vwin.width, vwin.height);

	vpic.brightness = vpic.contrast = vpic.hue = vpic.colour = vpic.whiteness = 32767;
	
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
		printf("VIDEO_PALETTE_RGB24\n");
		
		if(ioctl(fd, VIDIOCSPICT, &vpic) < 0) {
			vpic.palette=VIDEO_PALETTE_RGB565;
			vpic.depth=16;
			printf("VIDEO_PALETTE_RGB565\n");
			
			if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
				vpic.palette=VIDEO_PALETTE_RGB555;
				vpic.depth=15;
				printf("VIDEO_PALETTE_RGB555\n");
				
				if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
					vpic.palette=VIDEO_PALETTE_YUV420P;
					vpic.depth=15;
					printf("VIDEO_PALETTE_YUV420P\n");
					
					if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
						vpic.palette=VIDEO_PALETTE_YUYV;
						vpic.depth=16;
						printf("VIDEO_PALETTE_YUYV\n");
					
						if(ioctl(fd, VIDIOCSPICT, &vpic)==-1) {
							perror("Unable to find a supported capture format.\n");
							return -1;
						}
					}
				}
			}
		}
	}

	if(ioctl(fd, VIDIOCGPICT, &vpic) < 0) {
		perror("VIDIOCGPICT");
		close(fd);
		return -1;
	}
	printf("depth : %d, palette : %d\n", vpic.depth, vpic.palette);

#ifdef USE_MMAP
/*
	vbuf.base = buffer; 
	vbuf.width = WIDTH;
	vbuf.height = HEIGHT;
	vbuf.depth = vpic.depth;
	if(ioctl(fd, VIDIOCSFBUF, &vbuf) < 0) {
		perror("ioctl(VIDIOCSFBUF)");
		return -1;
	}
*/

	memset(&vmbuf, 0, sizeof(vmbuf));
	vmbuf.size = vwin.width * vwin.height * vpic.depth / 8;
	vmbuf.frames = 0;
	//vmbuf.offsets = 0;
	
	/* mmap 정보의 취득 */
	if(ioctl(fd, VIDIOCGMBUF, &vmbuf) == -1) {
		perror("ioctl(VIDIOCGMBUF)");
		close(fd);
		return -1;
	}
	printf("mmap size : %d\n", vmbuf.size);
/*
	if(!vmbuf.size)
		size = vwin.width * vwin.height * vpic.depth / 8;
	else 
		size = vmbuf.size;
*/	
	/* mmap */
	buffer = (unsigned char*)mmap(0, vmbuf.size, PROT_READ, MAP_SHARED, fd, 0);
	//buffer = (unsigned char*)mmap(NULL, vmbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(buffer == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
#else
#ifdef USE_LIBOIL
	tmp = malloc(vwin.width*vwin.height*vpic.depth/8);
#endif
	buffer = malloc(vwin.width*vwin.height*vpic.depth/8);
	if (!buffer) {
		perror("Out of memory.\n");
		return -1;
	}
#endif
	
	for(i=t=b=g=r=0; i < n_max_count; i++) {
#ifdef USE_MMAP
		/* frame #0 에 capture 개시 지시 */
		vmm.frame = 0;
		vmm.width = WIDTH;
		vmm.height = HEIGHT;
		vmm.format = vpic.palette; //VIDEO_PALETTE_YUYV; //VIDEO_PALETTE_RGB24;
		if(ioctl(fd, VIDIOCMCAPTURE, &vmm) == -1)
		{
			perror("ioctl(VIDIOCMCAPTURE)");
			return -1;
		}
#else
		size = read(fd, buffer, vwin.width*vwin.height*vpic.depth/8);		
#endif
#ifdef USE_MMAP 	
		if(ioctl(fd, VIDIOCSYNC, &vmm) == -1) {
			perror("ioctl(VIDIOCSYNC)");
			return -1;
		}
#endif
#ifdef USE_LIBOIL
//		oil_yuyv2ayuv((uint32_t*)tmp, (uint32_t*)buffer, vwin.width*vwin.height);
//		oil_ayuv2argb_u8((uint8_t*)buffer, (uint8_t*)tmp, vwin.width*vwin.height);
#endif
#if 1
		for(x = 0; x < vwin.height; x++) {
			t = x * vwin.width;
			for(y = 0; y < vwin.width; y++) {
				r = *(buffer+(t+y)*3 + 2);
				g = *(buffer+(t+y)*3 + 1);
				b = *(buffer+(t+y)*3 + 0);
				pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);
				
				out_data[y+t+(WIDTH-vwin.width)*x] = pixel;
			}
		}
		
		memcpy(pfbmap, out_data, WIDTH*HEIGHT*2);
#endif
	};
	
#ifdef USE_MMAP
	munmap(buffer, vmbuf.size);
#else
	free(buffer);
#endif
	munmap(pfbmap, vwin.width*vwin.height*2);
	
	close(fd);
	return 0;
}
