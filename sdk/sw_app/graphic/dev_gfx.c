/******************************************************************************
 * UCX/UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    dev_gfx.c
 * @brief	device graphic function
 * @author  phoong
 * @section MODIFY history
 *     - 2013.11.15 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/ti81xxfb.h>

#include "draw_gui.h"
#include "dev_gfx.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define	FBDEV0_NAME		"/dev/fb0"
#define	FBDEV1_NAME		"/dev/fb1"
#define	FBDEV2_NAME		"/dev/fb2"

typedef struct {
	int fd;

	Upix *buf;
	int sz;
} gfx_info_t;

typedef struct {
	gfx_info_t x[MAX_GFX];
} app_gfx_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gfx_t gfx_t;
static app_gfx_t *igfx=&gfx_t;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#define eprintf(x...) do { printf(" [gfx err!] %s: ", __func__); printf(x); } while(0)
#ifdef SYS_DEBUG
#define dprintf(x...) do { printf(" [gfx ] %s: ", __func__); printf(x); } while(0)
#else
#define dprintf(x...)
#endif

int gfx_blending(int fd, int percent);

/*----------------------------------------------------------------------------
 graphic scale function
-----------------------------------------------------------------------------*/
static int gfx_scale(int fd, int in_w, int in_h, int out_w, int out_h)
{
	struct ti81xxfb_scparams scparams;
	struct ti81xxfb_region_params regp;
	//int dummy;

	/* Set Scalar Params for resolution conversion */
	scparams.inwidth = in_w;
	scparams.inheight = in_h;
	scparams.outwidth = out_w;
	scparams.outheight = out_h;
	scparams.coeff = NULL;

	if( ioctl(fd, TIFB_SET_SCINFO, &scparams) < 0) {
	    eprintf("TIFB_SET_SCINFO !!!\n");
	    return -1;
	}

	if( ioctl(fd, TIFB_GET_PARAMS, &regp) < 0) {
	    eprintf("TIFB_GET_PARAMS !!!\n");
	     return -1;
	}

	regp.pos_x = 0;
	regp.pos_y = 0;
	regp.scalaren = TI81XXFB_FEATURE_ENABLE;

	if( ioctl(fd, TIFB_SET_PARAMS, &regp) < 0) {
		eprintf("TIFB_SET_PARAMS !!!\n");
		return -1;
	}

	#if 0
	if( ioctl(fd, FBIO_WAITFORVSYNC, &dummy)) {
		eprintf("FBIO_WAITFORVSYNC !!!\n");
		return -1;
	}
	#endif

	return 0;
}

/*----------------------------------------------------------------------------
 get frame buffer info function
-----------------------------------------------------------------------------*/
static int gfx_fb_info(int fd, int wi, int he, int en_print)
{
	struct fb_fix_screeninfo fixinfo;
	struct fb_var_screeninfo varinfo;
	int size;
	int dw, dh, scale=0;

	if( ioctl(fd, FBIOGET_FSCREENINFO, &fixinfo) < 0) {
		eprintf("Error reading fixed information.\n");
		return -1;
	}

	if(en_print)
	{
		usleep(10000);
		dprintf("\n");
        dprintf("Fix Screen Info:\n");
		dprintf("----------------\n");
		dprintf("Line Length - %d\n", fixinfo.line_length);
		dprintf("Physical Address = %lx\n",fixinfo.smem_start);
		dprintf("Buffer Length = %d\n",fixinfo.smem_len);
	}

	if( ioctl(fd, FBIOGET_VSCREENINFO, &varinfo) < 0) {
		eprintf("Error reading variable information.\n");
		return -1;
	}

	if(en_print)
	{
		usleep(10000);
		dprintf("\n");
		dprintf("Var Screen Info:\n");
		dprintf("----------------\n");
		dprintf("Xres - %d\n", varinfo.xres);
		dprintf("Yres - %d\n", varinfo.yres);
		dprintf("Xres Virtual - %d\n", varinfo.xres_virtual);
		dprintf("Yres Virtual - %d\n", varinfo.yres_virtual);
		dprintf("Bits Per Pixel - %d\n", varinfo.bits_per_pixel);
		dprintf("Pixel Clk - %d\n", varinfo.pixclock);
		dprintf("Rotation - %d\n", varinfo.rotate);
	}

	//# device size
	dw = varinfo.xres;
	dh = varinfo.yres;

	if(dw != wi || dh != he) {
		scale = 1;
		varinfo.xres = wi;
	    varinfo.yres = he;
	    varinfo.xres_virtual = wi;
	    varinfo.yres_virtual = he;
	}

	if( ioctl(fd, FBIOPUT_VSCREENINFO, &varinfo) < 0) {
		eprintf("Error writing variable information.\n");
		return -1;
	}

	if( ioctl(fd, FBIOGET_FSCREENINFO, &fixinfo) < 0) {
		eprintf("Error reading fixed information.\n");
		return -1;
	}

	size = varinfo.xres*varinfo.yres*(varinfo.bits_per_pixel/8);
	if(en_print)
		dprintf("buf_sz - %d Bytes\n", size);

	if(scale) {
		if(he == 576) {
			struct ti81xxfb_region_params regp;

			if( ioctl(fd, TIFB_GET_PARAMS, &regp) < 0) {
				eprintf("TIFB_GET_PARAMS !!!\n");
				return -1;
			}

			regp.pos_x = 0;
			regp.pos_y = 0;

			if( ioctl(fd, TIFB_SET_PARAMS, &regp) < 0) {
				eprintf("TIFB_SET_PARAMS !!!\n");
				return -1;
			}
		} else {		
			gfx_scale(fd, wi, he, dw, dh);
		}
	}

	return size;
}

/*----------------------------------------------------------------------------
 set alpha blending function
-----------------------------------------------------------------------------*/
int gfx_blending(int fd, int percent)
{
	struct ti81xxfb_region_params regp;
	unsigned char alpha;

	//# Set Global Alpha Blending
	alpha = (percent*0xff)/100;
	alpha = 0xff - alpha;
	dprintf("alpha = 0x%x\n", alpha);

	if( ioctl(fd, TIFB_GET_PARAMS, &regp) < 0) {
		eprintf("TIFB_GET_PARAMS\n");
		return -1;
	}

	regp.blendtype = TI81XXFB_BLENDING_GLOBAL;
	regp.blendalpha = alpha;
	if( ioctl(fd, TIFB_SET_PARAMS, &regp) < 0) {
		eprintf ("TIFB_SET_PARAMS.\n");
		return -1;
	}

	return 0;
}

/*----------------------------------------------------------------------------
 set transparency color function
-----------------------------------------------------------------------------*/
static int gfx_transparency(int fd, unsigned int key_color)
{
	struct ti81xxfb_region_params regp;

	if( ioctl(fd, TIFB_GET_PARAMS, &regp) < 0) {
		eprintf("TIFB_GET_PARAMS\n");
		return -1;
	}

	if(regp.transcolor == key_color)
		return 0;

	//regp.transen = TI81XXFB_FEATURE_ENABLE;
	regp.transcolor = key_color;		//# RGB24 color
	regp.transtype = 3;					//# VPS_GRPX_TRANS_3BIT_MASK
	if( ioctl(fd, TIFB_SET_PARAMS, &regp) < 0) {
		eprintf ("TIFB_SET_PARAMS.\n");
		return -1;
	}

	return 0;
}

/*****************************************************************************
* @brief    dev graphic init/exit function
* @section  DESC Description
*   - desc
*****************************************************************************/
int dev_gfx_init(int gfxnum, int *addr, int wi, int he)
{
	int fd=0;
	Upix *buf;
	int buf_sz;

	if(gfxnum > (MAX_GFX-1))
		return -1;

	if(gfxnum == GFX2)			//# tv-out
	{
		//memset(&igfx->x[gfxnum], 0, sizeof(gfx_info_t));

		//# Open the display device
		fd = open(FBDEV2_NAME, O_RDWR);
		if (fd <= 0) {
			eprintf("Could not open device [%s] \n", FBDEV2_NAME );
			return -1;
		}

		buf_sz = gfx_fb_info(fd, wi, he, 0);
		if(buf_sz < 0) {
			goto exit1;
		}

		draw_size_set(UI_NORMAL, wi, he);
	}
	else if(gfxnum == GFX1)		//# lcd
	{
		//memset(&igfx->x[gfxnum], 0, sizeof(gfx_info_t));

		//# Open the display device
		fd = open(FBDEV1_NAME, O_RDWR);
		if (fd <= 0) {
			eprintf("Could not open device [%s] \n", FBDEV1_NAME);
			return -1;
		}

		buf_sz = gfx_fb_info(fd, wi, he, 0);
		if(buf_sz < 0) {
			goto exit1;
		}

		draw_size_set(UI_NORMAL, wi, he);
	}
	else if(gfxnum == GFX0)		//# hdmi, same GFX2(720x480)
	{
		//# Open the display device
		fd = open(FBDEV0_NAME, O_RDWR);
		if (fd <= 0) {
			eprintf("Could not open device [%s] \n", FBDEV0_NAME );
			return -1;
		}

		buf_sz = gfx_fb_info(fd, wi, he, 0);
		if(buf_sz < 0) {
			goto exit1;
		}

		draw_size_set(UI_NORMAL, wi, he);
	}
	else
	{
		goto exit1;
	}

	//gfx_blending(fd, 30);
	gfx_transparency(fd, RGB_KEY_24);

	buf = (Upix *)mmap(0, buf_sz, (PROT_READ|PROT_WRITE), MAP_SHARED, fd, 0);
	if (buf == MAP_FAILED) {
		eprintf("GFX%d mmap failed\n", gfxnum);
		goto exit1;
	}

	igfx->x[gfxnum].fd = fd;
	igfx->x[gfxnum].buf = buf;
	igfx->x[gfxnum].sz = buf_sz;

	*addr = (int)buf;

	dprintf("GFX%d... %dx%d done!\n", gfxnum, wi, he);

	return 0;

exit1:
	if(fd > 0) {
		ioctl(fd, TIFB_CLOSE, NULL);
		close(fd);
	}

	return -1;
}

int dev_gfx_exit(int gfxnum)
{
	int fd=igfx->x[gfxnum].fd;

	if(gfxnum > (MAX_GFX-1))
		return -1;

	if(igfx->x[gfxnum].buf) {
		munmap(igfx->x[gfxnum].buf, igfx->x[gfxnum].sz);
		igfx->x[gfxnum].buf = NULL;
	}

	if(fd > 0) {
		ioctl(fd, TIFB_CLOSE, NULL);
		close(fd);
	}

	dprintf("GFX%d... done!\n", gfxnum);

	return 0;
}
