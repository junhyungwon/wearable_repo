/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <logo_814x.h>

#ifdef CONFIG_NT35510
#include <nt35510.h>
#endif

#ifdef eprintf
#undef eprintf
#define	eprintf(x...) printf("[logo] !err: " x)
#endif

#ifdef CONFIG_IMK
#include <asm/gpio.h>
#include <ili9486_lcd.h>

#define GPIO_N(bank, gpio)		((32*bank) + gpio)

/* 320x480 Half VGA */
#define LCD_WIDTH  	320
#define LCD_HEIGHT 	480
#else
#define LCD_WIDTH  	480
#define LCD_HEIGHT 	800
#endif

#define WIDTH  		720
#define HEIGHT 		480

#define STRIDE 			(3 * WIDTH)
#define FRAME_SIZE 		(STRIDE * HEIGHT)
#define LCD_STRIDE 		(2 * LCD_WIDTH)
#define LCD_FRAME_SIZE 	(LCD_STRIDE * LCD_HEIGHT)

int ti810x_read_bmp_image(uint32_t lcd_flag, uint32_t bmp_source, uint32_t bmp_dest)
{
	unsigned long long bmp_size = 0;
	int cut_width = 0;
	int bmp_width = 0;
	int bmp_height = 0;
	int bmp_stride = 0;
	int bmp_data_offset = 0;
	int bmp_pixel_bits = 0;
	int pad	= 0;
	int image_type = 0;
	int image_start_addr;
	int row_no;
	int row_dest;
	int width,height,stride,frameSize;

	if(lcd_flag == 1)
	{
		width     = LCD_WIDTH;
		height    = LCD_HEIGHT;
		stride    = LCD_STRIDE;
		frameSize = LCD_FRAME_SIZE;
	}
	else
	{
		width     = WIDTH;
		height    = HEIGHT;
		stride    = STRIDE;
		frameSize = FRAME_SIZE;
	}

	if(bmp_read16(bmp_source) == BMP_SIGNATURE)
	{
		/*read size of bmp file*/
		bmp_size = (bmp_read64(bmp_source) >> 16) & 0xFFFFFFFF;
		#ifdef DEBUG_BMP
		printf("\nfile size = %x",bmp_size);
		#endif
		/*read data starting offset*/
		bmp_data_offset = bmp_read16(bmp_source + BMP_DATA_OFFSET);
		#ifdef DEBUG_BMP
		printf("\ndata offset = %x",bmp_data_offset);
		#endif

		/*read bmp width*/
		bmp_width  = bmp_read16(bmp_source + BMP_WIDTH_OFFSET);
		#ifdef DEBUG_BMP
		printf("\ndword width  = %d",bmp_width);
		#endif
		bmp_stride =  bmp_width * 3;

		/*read bmp height*/
		bmp_height = bmp_read16(bmp_source + BMP_HEIGHT_OFFSET );
		if (bmp_height < 0)
		{
			image_type = BMP_IMAGE_TOPDOWN;
			bmp_height *= -1;
		}
		else
		{
			image_type = BMP_IMAGE_BOTTOMUP;
			bmp_height *= 1;
		}
		#ifdef DEBUG_BMP
		printf("\ndword height = %d",bmp_height);
		#endif
		bmp_pixel_bits = bmp_read16(bmp_source + BMP_BITS_PER_PIXEL_OFFSET);
		/* check whether the number of bits used for pixel is 24 or not */
		if (bmp_pixel_bits != 24)
		{
			eprintf("Not able to read image, use 24-bit BMP Image");
			return -1;
		}

		pad = (bmp_size - (bmp_stride * bmp_height) - 54);
		pad /= bmp_height;
		/*
		   set background color
		 */
		memset((void *)(bmp_dest), LOGO_BGCOLOR, frameSize);
		if(bmp_width > width)
		{
			cut_width = bmp_width - width;
			bmp_width = width;
			bmp_stride = width * 3;

		}
		if(bmp_height > height)
		{
			bmp_height = height;
		}

		/* image start offset */
		image_start_addr = bmp_dest + (stride * ((height - bmp_height) >> 1 )) + ((stride - (bmp_stride)) >> 1);

		/* aligning the address with 24 bit sets */
		if ((image_start_addr - bmp_dest) % 3 == 1)
		{
			image_start_addr += 2;
		}
		else if ((image_start_addr - bmp_dest) % 3 == 2)
		{
			image_start_addr += 1;
		}

		/*copy image data row by row to destination offset*/
		if (image_type == BMP_IMAGE_BOTTOMUP)
		{
			for(row_no = (bmp_height-1), row_dest = 0; row_no >= 0; row_no--, row_dest++)
			{
				memcpy((char *) (image_start_addr + (row_dest * (stride))),
				(uint32_t*) ((bmp_source + bmp_data_offset) + (row_no * (bmp_stride + (cut_width * 3)+pad))),
					(bmp_stride));
			}
		}
		else
		{
			for(row_no = 0, row_dest = 0; row_no <= (bmp_height-1); row_no++, row_dest++)
			{
				memcpy((uint32_t*) (image_start_addr + (row_dest * (stride))),
				(uint32_t*) ((bmp_source + bmp_data_offset) + (row_no * (bmp_stride + (cut_width * 3)+pad))),
					(bmp_stride));
			}
		}
		return 0;
	}

	return -1;
}

int ti810x_prcm_init()
{
	if (ti810x_prcm_enable_vps_power_and_clock()<0)
		return -1;

	return 0;
}

static int ti810x_prcm_enable_vps_power_and_clock()
{
	int repeat;
	int ok;
	uint32_t val;

	/* SW_WKUP: Start a software forced wake up transition on the domain. */
	prcm_write32(CM_HDVPSS_CLKSTCTRL, 0x02);

	/* wait for 1000 cycles before checking for power update */
	udelay(10);

	/* Check the power state after the wakeup transistion */
	for (ok=0, repeat=0; repeat<5; repeat++)
	{
		val = prcm_read32(PM_HDVPSS_PWRSTST);
		if (val == 0x37)
		{
			ok = 1;
			break;
		}
		udelay(10);
	}
	if (!ok)
	{
		return -1;
	}

	/* Enable HDVPSS Clocks */
	prcm_write32(CM_HDVPSS_HDVPSS_CLK_CTRL, 0x02);

	/* Enable HDMI Clocks */
	prcm_write32(CM_HDVPSS_HDMI_CLKCTRL, 0x02);

	for (ok=0, repeat=0; repeat<5; repeat++)
	{
		val = prcm_read32(CM_HDVPSS_CLKSTCTRL);
		if ((val & 0x100) == 0x100) {
			ok = 1;
			break;
		}
		udelay(10);
	}
	if (!ok) {
		return -1;
	}

	/* reset HDVPSS and HDMI */
	prcm_write32(RM_HDVPSS_RSTCTRL, 0x04);
	udelay(10);
	prcm_write32(RM_HDVPSS_RSTST, 0x04);
	udelay(10);

	/* release reset from HDVPSS and HDMI */
	prcm_write32(RM_HDVPSS_RSTCTRL, 0x00);
	udelay(10);

	/* wait for SW reset to complete */
	for (ok=0, repeat=0; repeat<5; repeat++)
	{
		val = prcm_read32(RM_HDVPSS_RSTST);
		if ((val & 0x4) == 0x4) {
			ok = 1;
			break;
		}
		udelay(10);
	}
	if (!ok) {
		return -1;
	}

	/* put HDVPSS in ON State */
	val = prcm_read32(PM_HDVPSS_PWRSTCTRL);
	val |= 0x3;
	prcm_write32(PM_HDVPSS_PWRSTCTRL, val);

	/* wait 10 cycles after powering on */
	udelay(10);

	/* check power status */
	for (ok=0, repeat=0; repeat<5; repeat++)
	{
		val = prcm_read32(PM_HDVPSS_PWRSTST);
		if (val==0x37) {
			ok = 1;
			break;
		}
		udelay(10);
	}
    if (!ok) {
		return -1;
    }

	if (CPU_TYPE == CPU_TI810X) {
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E01C);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E000);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E001);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E003);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E021);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E1E1);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x0000E1E3);
		cm_write32(HD_DAC_CTRL_REG_ADDR, 0x000001E3);
		//cm_write32(HD_DAC_CTRL_REG_ADDR, 0x018800DB);
	}

#ifdef COMMON_UD_FEATURE
	cm_write32(HD_CLK_CTRL_REG_ADDR, (3u << 8u)); /* dvo2 dclk invert */
#endif

	return 0;
}

/**
 * Initialize the PLLs
 */
int ti810x_pll_init()
{
	if (ti810x_pll_config_hdvpss()<0)
		return -1;

	return 0;
}

/**
 * Configure PLL for HDVPSS unit
 */
static int ti810x_pll_config_hdvpss()
{
	uint32_t rd_osc_src;
	rd_osc_src = pll_read32(PLL_VIDEO2_PINMUX);
	rd_osc_src &= 0xFFFFFFFE;
	pll_write32(PLL_VIDEO2_PINMUX,rd_osc_src);
	//ti810x_pll_configure(PLL_HDVPSS_BASE,19,800,4,0x00000801);

	return 0;
}

/**
 * Program a PLL unit
 */
static void ti810x_pll_configure(uint32_t baseAddr, uint32_t N, uint32_t M, uint32_t M2, uint32_t clkCtrlValue)
{
	uint32_t m2nval,mn2val,clkctrl,clk_out,ref_clk,clkout_dco = 0;
	uint32_t status;

	m2nval = (M2<<16) | N;
	mn2val =  M;
	ref_clk     = OSC_FREQ / (N+1);
	clkout_dco  = ref_clk * M;
	clk_out     = clkout_dco / M2;

	pll_write32((baseAddr+M2NDIV    ),m2nval);
	pll_write32((baseAddr+MN2DIV    ),mn2val);
	pll_write32((baseAddr+TENABLEDIV),0x1);
	pll_write32((baseAddr+TENABLEDIV),0x0);
	pll_write32((baseAddr+TENABLE   ),0x1);
	pll_write32((baseAddr+TENABLE   ),0x0);

	clkctrl = pll_read32(baseAddr+CLKCTRL);
	clkctrl = (clkctrl & ~(7<<10 | 1<<23)) | clkCtrlValue;
	pll_write32(baseAddr+CLKCTRL,clkctrl);

	do
	{
		status = pll_read32(baseAddr+STATUS);
	} while ((status & 0x00000600) != 0x00000600);
}

/**
 * Initialize HDVPSS unit
 */
int ti810x_vps_init()
{
	int temp = 0;
	/* enable clocks on all units */
	vps_write32(VPS_CLKC_ENABLE,0x01031fff);
	/*setup both HDMI and HDCOMP Clock source*/
	vps_write32(VPS_CLKC_VENC_CLK_SELECT, 0x9010D);
	udelay(10);

	/* enable comp units(HDMI and HDCOMP(exclude TI814X)) */
	temp = 1;
	if (CPU_TYPE == CPU_TI816X)
		temp |= (1 << 8);
	else if (CPU_TYPE == CPU_TI810X)
		temp |= (1 << 16);

	vps_write32(VPS_COMP_STATUS, temp);

	// set background color
	vps_write32(VPS_COMP_BGCOLOR, LOGO_BGCOLOR);

	return 0;
}

int ti810x_set_mode(char cmd[])
{
	ti810x_pll_config_hdmi(20000000);
	ti810x_hdmi_enable(20000000);

#ifdef LCD_DVO2
	/*modeline "720x480" 27.000 720 736 798 858 480 489 495 525 -hsync -vsync*/
	//ti810x_vps_configure_venc(0x6000, 720, 736, 798, 858, 480, 489, 495, 525, 0, 1, 1, cmd);
	ti810x_vps_configure_venc(0x6000, 720, 736, 798, 858, 480, 489, 495, 525, 0, 1, 0,cmd);

	#ifdef CONFIG_IMK
	 /* arg1-> reg_base,     arg2-> display width,
	  * arg3-> H Sync Start (arg4 - arg3),
	  * arg4-> H Sync End : arg4 - arg2 = blank period.
	  * arg5-> Total Width(arg2 + margine),
	  * arg6-> display Height
	  * arg7-> V Sync Start (arg8 - arg7)
	  * arg8-> V Sync End (arg8 - arg7 = sync width)
	  * arg9-> Total Height(arg6 + margine),
	  * arg 10-> enable_invert,
	  * arg 11-> H Sync Invert,
	  * arg 12-> V Sync Invert,
	  * arg 13-> cmd
		*/
//	ti810x_pll_config_dvo2(20000000); /* pixel clock 20MHz > min 15MHz */
	ti810x_pll_config_dvo2(8000000); /* pixel clock 20MHz > min 15MHz */
	//8, 43, 41, 3, 12, 10 - FrontPorch, backporch, synclen
//	ti810x_vps_configure_venc(0xA000, 320, 620, 630, 640, 480, 516, 518, 520, 0, 1, 1, cmd);
	ti810x_vps_configure_venc(0xA000, 320, 330, 340, 360, 480, 482, 484, 490, 0, 1, 1, cmd);
	#else
	ti810x_pll_config_dvo2(30000000); /* pixel clock 30 MHz */
	//8, 43, 41, 3, 12, 10 - FrontPorch, backporch, synclen
	//ti810x_vps_configure_venc(0xA000, 240, 248, 290, 330, 320, 323, 333, 345, 0, 0, 0, cmd);
	ti810x_vps_configure_venc(0xA000, 480, 500, 502, 522, 800, 816, 826, 832, 0, 1, 1, cmd);
	#endif
#else
	pll_write32(PLL_VIDEO2_PINMUX, 0);
	ti810x_vps_configure_venc(0x6000,720, 736, 798, 858, 480, 489, 495, 525, 0, 1, 1,cmd);
	ti810x_vps_configure_venc(0x8000,720, 736, 798, 858, 480, 489, 495, 525, 0, 1, 1,cmd);
#endif

	/* enable all video encoders */
	vps_write32(VPS_CLKC_VENC_ENABLE, 0xF);
	ti810x_pll_hdmi_setwrapper_clk();

	printf("%s done!\n", __func__);

	return 0;
}

/**
 * Configure PLL for HDMI
 */
int ti810x_pll_config_hdmi(uint32_t freq)
{
	uint32_t rd_osc_src;
	pll_config_t config;

	rd_osc_src = pll_read32(PLL_OSC_SRC_CTRL);
	pll_write32(PLL_OSC_SRC_CTRL,(rd_osc_src & 0xfffbffff) | 0x0);

	rd_osc_src = pll_read32(PLL_VIDEO2_PINMUX);
	rd_osc_src &= 0xFFFFFFFE;
	pll_write32(PLL_VIDEO2_PINMUX,rd_osc_src);
	ti810x_pll_get_dividers(freq, 1, &config);

	ti810x_pll_configure(PLL_VIDEO2_BASE, config.n, config.m, config.m2, config.clk_ctrl);

	return 0;
}

/**
 * Configure PLL for DVO2
 */
int ti810x_pll_config_dvo2(uint32_t freq)
{
	uint32_t rd_osc_src;
	pll_config_t config;

	rd_osc_src = pll_read32(PLL_OSC_SRC_CTRL);
	pll_write32(PLL_OSC_SRC_CTRL,(rd_osc_src & 0xfffbffff) | 0x0);

	ti810x_pll_get_dividers(freq, 0, &config);

	ti810x_pll_configure(PLL_VIDEO1_BASE, config.n, config.m, config.m2, config.clk_ctrl);

	return 0;
}

/**
 * Enable HDMI output.
 */
void ti810x_hdmi_enable(int freq)
{
	uint32_t temp, temp1;

	/* wrapper soft reset */
	temp = hdmi_read32(0x0010) ;
	temp1 = ((temp & 0xFFFFFFFE)| 0x1 );
	hdmi_write32(0x0300, temp1);
	temp = 0;
	udelay(10);

	/* configure HDMI PHY */
	/* 48 Mhz Clock input to HDMI ie SDIO clock output from PRCM */
	prcm_write32(0x15B0,0x2);

	/* Power on the phy from wrapper */
	hdmi_write32(0x0040, 0x8);

	while((hdmi_read32(0x0040) & 0x00000003) != 2);

	hdmi_write32(0x0040, 0x4A);
	while((hdmi_read32(0x0040) & 0x000000FF ) != 0x5A);

	hdmi_write32(0x0040, 0x8A);
	while((hdmi_read32(0x0040) & 0xFF) != 0xAA);

	/* Dummy read to PHY base to complete the SCP reset process HDMI_PHY_U_BAS */
	temp = hdmi_read32(0x0300);

	temp = hdmi_read32(0x0300);

	if(freq > 50000000)
	temp1 = ((temp & 0x3FFFFFFF)|(0x1 << 30));//0x40000000);
	else
	temp1 = ((temp & 0x3FFFFFFF)|(0x0 << 30));//0x40000000);
	hdmi_write32(0x0300, temp1);

	temp = hdmi_read32(0x030C) ;
	temp1 = ((temp & 0x000FFFFF)|0x85400000);
	hdmi_write32(0x030C, temp1);

	hdmi_write32(0x0304, 0xF0000000);

	udelay(10);

	/* cec clock divider config */
	temp = hdmi_read32(0x0070) ;
	temp1 = temp | 0x00000218;
	hdmi_write32(0x0070, temp1);

	/* wrapper debounce config */
	temp = hdmi_read32(0x0044) ;
	temp1 = temp | 0x00001414;
	hdmi_write32(0x0044, temp1);

	/* packing mode config */
	temp = hdmi_read32(0x0050) ;
	temp1 = temp | 0x105;
	hdmi_write32(0x0050, temp1);

	/* disable audio */
	hdmi_write32(0x0080, 0x0);

	/* release HDMI IP CORE reset and release power down of core */
	hdmi_write32(0x0414, 0x1);
	hdmi_write32(0x0424, 0x1);

	/* video action  config of hdmi */
	hdmi_write32(0x0524, 0x0);

	/* config input data bus width */
	hdmi_write32(0x0420, 0x7);

	/* configure AVI INFOFRAME */
	hdmi_write32(0x0528, 0x0);  // VID_MODE CONFIG
	hdmi_write32(0x04CC, 0x1);  // DATA ENABLE CNTRL
	hdmi_write32(0x0420, 0x37); // ENABLE VSYNC AND HSYNC
	hdmi_write32(0x04F8, 0x0);  // iadjust config to enable vsync
	hdmi_write32(0x0520, 0x10); // csc is bt709
	// enable change HDMI mode to DVI mode,
	// both DVI and HDMI monitor can works under DVI mode
	hdmi_write32(0x09BC, 0x20); // enable hdmi

	hdmi_write32(0x0608, 0x20); // tmds_ctrl
	hdmi_write32(0x0904, 0x0);  // disable n/cts of actrl
	hdmi_write32(0x0950, 0x0);  // disable audio
	hdmi_write32(0x0414, 0x0);  // keep audio  operation in reset state

	/* configure AVI INFOFRAME */
	hdmi_write32(0x0A00, 0x82);
	hdmi_write32(0x0A04, 0x2);
	hdmi_write32(0x0A08, 0xD);
	hdmi_write32(0x0A10, 0x1);
	hdmi_write32(0x0A14, 0xA0);
	hdmi_write32(0x0A1C, 0x8F);

	hdmi_write32(0x0538 , 0x3);  // DISABLE DEEP COLOR MODE IN DC PACKET
	hdmi_write32(0x09C0 , 0x10);
	hdmi_write32(0x09F8 , 0x3);  // ENABLE AND REPEAT AVI INFOFRAM TRANSMISSON
	hdmi_write32(0x09FC , 0xF);  // ENABLE AND REPEAT GENEERAL PACKET TRANSMISSION
}

/* Change pin mux */
void ti810x_pll_hdmi_setwrapper_clk()
{
        uint32_t rd_osc_src;
        rd_osc_src = pll_read32(PLL_VIDEO2_PINMUX);
        rd_osc_src |= 0x1;
        pll_write32(PLL_VIDEO2_PINMUX,rd_osc_src);
}

static int ti810x_pll_get_dividers(uint32_t req_out_clk, int hdmi, pll_config_t* config)
{
	int32_t ret = -1;
	int32_t n, m, m2;
	float ref_clk, dco_clk, clk_out;
	float best_delta;

	config->n = 0;
	config->m = 0;
	config->m2 = 0;
	config->clk_ctrl = 0;
	best_delta = 1E20;

	if (req_out_clk == 6500000) {
		config->n = 19;
		config->m = 507;
		config->m2 =78;
		config->clk_ctrl = 0x801;
		return 0;
	} else if (req_out_clk == 8000000) {
		config->n = 19;
		config->m = 500;
		config->m2 =62;
		config->clk_ctrl = 0x801;
		return 0;
	}

	if(hdmi)
	{
		config->n = 19;//n;
		config->m = 1485;//m;
		config->m2 =10;//m2;
		config->clk_ctrl = 0x200a1001;
		if(req_out_clk ==74250000 )
		{
			config->n = 19;//n;
			config->m = 742;//m;
			config->m2 =10;//m2;
			config->clk_ctrl = 0x20020801;
		}
		else if(req_out_clk ==65000000 )
		{
			config->n = 19;//n;
			config->m = 650;//m;
			config->m2 =10;//m2;
			config->clk_ctrl = 0x20020801;
		}
		else if(req_out_clk ==54000000 )
		{
			config->n = 19;//n;
			config->m = 540;//m;
			config->m2 =10;//m2;
			config->clk_ctrl = 0x20020801;
		}
		else if(req_out_clk ==27000000 )
		{
			config->n = 19;//n;
			config->m = 540;//m;
			config->m2 =2;//m2;
			config->clk_ctrl = 0x200A0801;//0x20020801;
		}
		else if(req_out_clk == 33000000 )
		{
			config->n = 19;//n;
			config->m = 660;//m;
			config->m2 =2;//m2;
			config->clk_ctrl = 0x200A0801;//0x20020801;
		}
		else if(req_out_clk == 25200000)
		{
			config->n = 19;//n;
			config->m = 1512;//m;
			config->m2 = 60;//m2;
			config->clk_ctrl = 0x200a1001;//0x20020801;
		}


		ref_clk = 20E6f / (config->n+1);
		dco_clk = ref_clk * config->m;
		clk_out = dco_clk / config->m2;
	}
	else
	{
		for (n=19; n<=19 && ret!=0; n++)
		{
			ref_clk = 20E6f / (n+1);
			if (ref_clk < 2.5E6)
			{
				int32_t m_min;
				int32_t m_max;

				if (hdmi)
				{
					m_min = 1000.0E6f / ref_clk;
					m_max = 2000.0E6f / ref_clk;
				}
				else
				{
					m_min =  500.0E6f / ref_clk;
					m_max = 1000.0E6f / ref_clk;
				}

				for (m=m_min; m<m_max && ret!=0; m++)
				{
					dco_clk = ref_clk * m;
					for (m2 = 10; m2 <= 25; m2++)
					{
						float delta;
						clk_out = dco_clk / m2;
						delta = clk_out-req_out_clk;
						if (delta<0)
						{
							delta = -delta;
						}

						if (delta<best_delta)
						{
							config->n = n;
							config->m = m;
							config->m2 = m2;
							if (hdmi)
							{
								config->clk_ctrl = 0x200a1001;
							}
							else
							{
								config->clk_ctrl = 0x00000801;
							}

							best_delta = delta;

							if (delta==0.0)
							{
								ret = 0;
								break;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

/**
 * Configure VENC unit
  */
static void ti810x_vps_configure_venc(uint32_t cfg_reg_base, int hdisp, int hsyncstart, int hsyncend, int htotal, int vdisp, int vsyncstart, int vsyncend, int vtotal, int enable_invert, int hs_invert, int vs_invert,char *cmd)
{
  	int av_start_h = htotal-hsyncstart;
	/*int av_start_v = vtotal-vsyncstart;*/
	int hs_width = hsyncend-hsyncstart;
	int vs_width = vsyncend-vsyncstart;
	int cfg0;
	 /* CFG10: clamp, lines (total num lines), pixels (total num pixels/line) */

	vps_write32(cfg_reg_base+0x28, 0x84000000 | (vtotal<<12) | (htotal));  // 84h=132d for 1080i

	/*CFG10: HSYNC Width, 2nd VBI end, 1st VBI END*/
	vps_write32(cfg_reg_base+0x2C, (hs_width << 24) | (vtotal - vdisp - 3));
	 /* CFG12: hs_width, act_pix, h_blank-1 */

  	vps_write32(cfg_reg_base+0x30, (hs_width<<24) | (hdisp<<12) | (av_start_h-1));

 	 /* CFG15: vout_hs_wd, vout_avdhw, vout_avst_h (same as in 0x6030?) */

  	vps_write32(cfg_reg_base+0x3c, (hs_width<<24) | (hdisp<<12) | (av_start_h));

  	/* CFG16: bp_pk_l (back porch peak), vout_avst_v1 (active video start field 1), vout_hs_st (hsync start) */

  	vps_write32(cfg_reg_base+0x40, ((vtotal - vdisp)<<12));

 	 /* CFG17: bp_pk_h (back porch peak), vout_avst_vw (num active lines), vout_avst_v1 (active video start field 2) */

  	vps_write32(cfg_reg_base+0x44, (vdisp<<12));

  	/* CFG18: vout_vs_wd1, vout_vs_st1 (vsync start), vout_avd_vw2 (vs width field 2) */

  	vps_write32(cfg_reg_base+0x48, (vs_width<<24) | ((vsyncstart - vdisp) << 12));

	vps_write32(cfg_reg_base+0x4C, 0x4000);

  	/* CFG21: osd_avd_hw (number of pixels per line), osd_avst_h */

	if (cfg_reg_base == 0x8000)
  		vps_write32(cfg_reg_base+0x54, (hs_width<<24) | (hdisp<<12) | (av_start_h - 5));
	else
  		vps_write32(cfg_reg_base+0x54, (hs_width<<24) | (hdisp<<12) | (av_start_h - 8));

  	/* CFG22: osd_avst_v1 (first active line), osd_hs_st (HS pos) */

  	vps_write32(cfg_reg_base+0x58, ((vtotal - vdisp) << 12));

  	/* CFG23: osd_avd_vw1 (number of active lines), osd_avst_v2 (first active line in 2nd field) */

  	vps_write32(cfg_reg_base+0x5c, (vdisp<<12));

  	/* CFG24: osd_vs_wd1 (vsync width), osd_vs_st1 (vsync start), osd_avd_vw2 */

  	vps_write32(cfg_reg_base+0x60, (vs_width<<24));

  	/* CFG25: osd_vs_wd2, osd_fid_st1, osd_vs_st2 */

  	vps_write32(cfg_reg_base+0x64, 0x00000000);

	/*for the HDCOMP VGA output, need remove the SOG to support VGA
	for the DM8107, special CSC coefficients are required to swap the color
	*/
	cfg0 = (enable_invert<<25) | (hs_invert<<24) | (vs_invert<<23) \
	    | (3<<16) // video out format: 10 bit, separate syncs
	    | (1<<13) // bypass gamma correction
	    | (1<<5)  // bypass CSC
	    | (1<<4)  // bypass 2x upscale
	    | (1<<0);  // 480p format

	if (cfg_reg_base == 0x8000) {
		if (CPU_TYPE == CPU_TI810X) {
			vps_write32(cfg_reg_base + 0x08, 0x04000400);
			vps_write32(cfg_reg_base + 0x10, 0x04000000);
			vps_write32(cfg_reg_base + 0x1c, 0x00000b00);
			/* enable the CSC due to the color swap issue. */
			//	cfg0 &= (~(1 << 5));
		}
		vps_write32(cfg_reg_base + 0x20, 0x00000b00);
		vps_write32(cfg_reg_base + 0x24, 0x00000b00);
	}
	vps_write32(cfg_reg_base+0x00, cfg0);

	if (strcmp(cmd,"colorbar") == 0)
		vps_write32(cfg_reg_base+0x00, vps_read32(cfg_reg_base+0x00) | 0x40000000 | 1<<15); /* start encoder with color bar bit set*/
	else
		vps_write32(cfg_reg_base+0x00, vps_read32(cfg_reg_base+0x00) | 0x40000000); /* start encoder */
}

/**
 * Load firmware for VPDMA.
 */
static int ti810x_vpdma_load_firmware(uint32_t* desc_buffer)
{
	uint32_t* firmware;
	volatile uint32_t status;

    volatile uint32_t vpdma_firmware_size;
    uint16_t *vpdma_firmware;

	int repeat;
	int result = 0;
	int ok;

	/* allocate physical contiguous memory for firmware buffer */
	firmware = (uint32_t*)desc_buffer;
	if (!firmware) {
		eprintf("unable to allocate memory for VPDMA firmware\n");
		return -1;
	}

    /* default firmware */
    vpdma_firmware      = vpdmaFirmware_1AD;
    vpdma_firmware_size = vpdmaFirmware_1AD_size;

    //#if defined(CONFIG_TI813X) || defined(CONFIG_DM385_CBB)
    if(CPU_TYPE == CPU_TI810X)
    {
        /* if DM385 then load different firmware */
        vpdma_firmware      = vpdmaFirmware_1B7;
        vpdma_firmware_size = vpdmaFirmware_1B7_size;
    }
    //#endif

    #ifdef CONFIG_TI816X
    if(get_cpu_rev() >= PG2_0)
    {
        /* if TI816x and >= PG2.0 then load different firmware */
        vpdma_firmware      = vpdmaFirmware_1B2;
        vpdma_firmware_size = vpdmaFirmware_1B2_size;
    }
    #endif

	/* copy firmware to buffer */
	memcpy(firmware,vpdma_firmware, vpdma_firmware_size);
	/* Reset VPDMA module */
	ti810x_vps_reset_module( VPS_MODULE_VPDMA, 1);
	udelay(1000000);
	ti810x_vps_reset_module( VPS_MODULE_VPDMA, 0);

	/* load firmware */
	vpdma_write32(VPDMA_LIST_ADDR, (uint32_t)firmware);

	/* wait till firmware is loaded */
	for (ok=0, repeat=0; repeat<100 && !ok; repeat++)
	{
		status = vpdma_read32(VPDMA_LIST_ATTR);
		if ((status & 0x80000) == 0x80000) {
			ok = 1;
		}
		else {
			udelay(10);
		}
	}

	if (!ok) {
		eprintf("failed to load VPDMA firmware\n");
		result = -1;
	}

    printf("VPDMA Firmware Version = 0x%08x\n", vpdma_read32(VPDMA_PID));

	return result;
}

void ti810x_vps_reset_module(vps_module_t module, int assert_reset)
{
	uint32_t val = vps_read32(VPS_CLKC_RESET);

	if (assert_reset)
		val |= (1<<module);
	else
		val &= ~(1<<module);

	vps_write32(VPS_CLKC_RESET, val);
}

uint32_t ti810x_dispmgr_create_grpx_conf_descriptor(int grpx_unit, uint32_t* desc_buffer, uint32_t* payload_buffer, uint32_t xres, uint32_t yres)
{
	uint32_t size = 0;
	uint32_t* desc;
	uint32_t* payload;

	/* set up frame configuration descriptor */
	desc = desc_buffer;
	desc[0] = 0x1;   /* frame configuration address */
	desc[1] = 16;    /* data length */
	desc[2] = (uint32_t)payload_buffer;  /* payload location */
	desc[3] = 0
		| (0xB<<27)     /* packet type */
		| (0<<26)       /* indirect command */
		| (1<<24)       /* block type */
		| ((grpx_unit+1)<<16)  /* destination GRPX0..GRPX2 */
		| (1<<0)        /* payload size in 128 bit units */
		;
	size += 16;
	payload = payload_buffer;
	payload[0] = 0;      /* reserved */
	if(grpx_unit == 2)
	{
		payload[1] = 0
		    | (720<<16)  /* frame width */
		    | (480<<0);   /* frame height */
	}
	else
	{
		payload[1] = 0
		    | (xres<<16)  /* frame width */
		    | (yres<<0);   /* frame height */
	}
	payload[2] = 0;      /* reserved */
	payload[3] = 0
		| (0<<30)    /* progressive format */
		| (1<<31)    /* yes soft reset of GRPX unit */
		;

	return size;
}

uint32_t ti810x_dispmgr_setup_layers(uint32_t* desc_buffer,
                                     int stride,
                                     int width,
                                     int height,
                                     uint32_t dispTime,
                                     uint32_t dispFps,
                                     uint32_t paddr)
{
	uint32_t size = 0;
	uint32_t numDesc = 0, i =0;
	uint32_t* desc;
	int chan;
	int x = 0;
	int y = 0;
	uint32_t layer_ctrl;

	desc = desc_buffer ;
	layer_ctrl = 0x0000E400;

	/* add src_viewport origin to surface start address */
	/*This should be made a param for multiple displays*/
	layer_ctrl |= 1<<(2);  /* enable GRPX0 layer */

	chan = 29;

    /* Program the same data descriptor
        BOOT_LOGO_DISPLAY_TIME_IN_SECS x BOOT_LOGO_DISPLAY_FPS
        times.

       Buffer size for descriptors is VPDMA_DESC_BUFFER_SIZE.
        If size needed for descriptors is more than this
        then limit number of descriptors to fit within
        VPDMA_DESC_BUFFER_SIZE
    */
    numDesc = dispTime * dispFps;

    if(numDesc*VPDMA_DATA_DESC_SIZE > VPDMA_DESC_BUFFER_SIZE)
    {
        numDesc = VPDMA_DESC_BUFFER_SIZE/VPDMA_DATA_DESC_SIZE;
    }

    for(i=0; i<numDesc; i++)
    {
        /* set up data transfer descriptor */
        desc[0] = 0
            | (DATA_TYPE<<26)    // RGB-888
            | (0<<25)            // no notification
            | (0<<24)            // field number
            | (0<<23)            // 2-dimensional data
            | (0<<20)            // +1 line skip (even)
            | (0<<16)            // +1 line skip (odd)
            | (stride)           // line stride
            ;
        desc[1] = 0
            | (width<<16)        // line length in pixels
            | (height)           // number of rows
            ;
        desc[2] = paddr;
        desc[3] = 0
                | (0xA<<27)    // packet type
            | (0<<26)      // 1D mode
            | (0<<25)      // inbound direction
            | (chan<<16)   // channel
            | (0<<9)       // priority
            | (chan<<0)    // next channel
            ;
        desc[4] = 0
            | (width<<16)  // region width
            | (height)     // region height
            ;
        desc[5] = 0
            | (x<<16)      // horizontal start
            | (y<<0)       // vertical start
            ;
        desc[6] = 0
            | (1<<7)       // first region
            | (1<<8)       // last region
            ;
        desc[7] = 0; // alpha blending enable
    	desc += 8;
	    size += 32;
    }

	vps_write32(VPS_COMP_HDMI_VOUT1, layer_ctrl);

	return size;
}

uint32_t ti810x_dispmgr_setup_layers_DVO2(uint32_t* desc_buffer,
					int stride, int width, int height,
					uint32_t dispTime, uint32_t dispFps, uint32_t paddr)
{
	uint32_t size = 0;
	uint32_t numDesc = 0, i =0;
	uint32_t* desc;
	int chan;
	int x = 0;
	int y = 0;
	uint32_t layer_ctrl;

	desc = desc_buffer ;
	layer_ctrl = 0x0000E400;

	/* add src_viewport origin to surface start address */
	/*This should be made a param for multiple displays*/
	layer_ctrl |= 1<<(1);  /* enable GRPX1 layer */

	chan = 30;

    /* Program the same data descriptor
        BOOT_LOGO_DISPLAY_TIME_IN_SECS x BOOT_LOGO_DISPLAY_FPS
        times.

       Buffer size for descriptors is VPDMA_DESC_BUFFER_SIZE.
        If size needed for descriptors is more than this
        then limit number of descriptors to fit within
        VPDMA_DESC_BUFFER_SIZE
    */
    numDesc = dispTime * dispFps;

    if(numDesc*VPDMA_DATA_DESC_SIZE > VPDMA_DESC_BUFFER_SIZE)
    {
        numDesc = VPDMA_DESC_BUFFER_SIZE/VPDMA_DATA_DESC_SIZE;
    }

    for(i=0; i<numDesc; i++)
    {
        /* set up data transfer descriptor */
        desc[0] = 0
            | (DATA_TYPE_LCD<<26)	// RGB-565
            | (0<<25)            // no notification
            | (0<<24)            // field number
            | (0<<23)            // 2-dimensional data
            | (0<<20)            // +1 line skip (even)
            | (0<<16)            // +1 line skip (odd)
            | (stride)           // line stride
            ;
        desc[1] = 0
            | (width<<16)        // line length in pixels
            | (height)           // number of rows
            ;
        desc[2] = paddr;
        desc[3] = 0
                | (0xA<<27)    // packet type
            | (0<<26)      // 1D mode
            | (0<<25)      // inbound direction
            | (chan<<16)   // channel
            | (0<<9)       // priority
            | (chan<<0)    // next channel
            ;
        desc[4] = 0
            | (width<<16)  // region width
            | (height)     // region height
            ;
        desc[5] = 0
            | (x<<16)      // horizontal start
            | (y<<0)       // vertical start
            ;
        desc[6] = 0
            | (1<<7)       // first region
            | (1<<8)       // last region
            ;
        desc[7] = 0; // alpha blending enable
    	desc += 8;
	    size += 32;
    }

	vps_write32(VPS_COMP_DVO2, layer_ctrl);

	return size;
}

//***************SD layers******************
uint32_t ti810x_dispmgr_setup_layers_SD(uint32_t* desc_buffer,
						int stride, int width, int height,
						uint32_t dispTime, uint32_t dispFps, uint32_t paddr)
{
	uint32_t size = 0;
	uint32_t numDesc = 0, i =0;
	uint32_t* desc;
	int chan;
	uint32_t layer_ctrl;

	desc = desc_buffer ;
	layer_ctrl = 0x0000E400;

	/* add src_viewport origin to surface start address */
	layer_ctrl |= 0x1;  /* enable GRPX2 to SD in COMP layer */
	chan = 31;

    numDesc = dispTime * dispFps;

	//4096 desc buffer is shared by 2 GRPXs
    if(numDesc*VPDMA_DATA_DESC_SIZE >(VPDMA_DESC_BUFFER_SIZE))
    {
        numDesc = (VPDMA_DESC_BUFFER_SIZE)/VPDMA_DATA_DESC_SIZE;
		printf("Desc Buffer has 2048 frame limitation. Actual bootlogo time is %d", 2048/dispFps);
    }

    for(i=0; i<numDesc; i++)
    {
        /* set up data transfer descriptor */
        desc[0] = 0
            | (DATA_TYPE<<26)    // RGB-888
            | (0<<25)            // no notification
            | (0<<24)            // field number
            | (0<<23)            // 2-dimensional data
            | (0<<20)            // +1 line skip (even)
            | (0<<16)            // +1 line skip (odd)
            | (stride)           // line stride
            ;
        desc[1] = 0
            | (width<<16)        // line length in pixels
            | (height)           // number of rows
            ;
        desc[2] = paddr;
        desc[3] = 0
                | (0xA<<27)    // packet type
            | (0<<26)      // 1D mode
            | (0<<25)      // inbound direction
            | (chan<<16)   // channel
            | (0<<9)       // priority
            | (chan<<0)    // next channel
            ;
        desc[4] = 0
            | (width<<16)  // region width
            | (height)     // region height
            ;
        desc[5] = 0
            | (40<<16)      // horizontal start
            | (0<<0)       // vertical start
            ;
        desc[6] = 0
            | (1<<7)       // first region
            | (1<<8)       // last region
            ;
        desc[7] = 0; // alpha blending enable
    	desc += 8;
	    size += 32;
    }

	vps_write32(VPS_COMP_SD, layer_ctrl);

	return size;
}
//***************SD layers******************


int ti810x_vpdma_send_list(uint32_t* desc_buffer, int desc_len, int list_no)
{
	/* convert descriptor size from byte units to 128-bit units */
	desc_len = (desc_len+15)>>4;
	/* send list */
	vpdma_write32(VPDMA_LIST_ADDR,(uint32_t)desc_buffer);
	vpdma_write32(VPDMA_LIST_ATTR, 0
			| (list_no<<24)  /* list number */
			| (0<<16)        /* list type: normal */
			| (desc_len<<0)  /* list size */
			);

	return 0;
}

int ti810x_dispmgr_wait_for_list_complete(int dispno)
{
        uint32_t status;
	int repeat;
	int ok;
    	for (ok=0, repeat=0; repeat<100 && !ok; repeat++)
	{
		status = vpdma_read32(VPDMA_LIST_ATTR);
		if ((status & 0x80000) == 0x80000)
		{
			ok = 1;
		}
		else
		{
			udelay(10);
		}
	}
    	if(repeat == 100)
		return -1;

	return 0;
}
//configure the VIDEO0_PLL to 54MHz to support SD output
static void ti810x_pll_configure_SD(void)
{
	u_int32_t clkCtrlVal;

	clkCtrlVal = SD_read32(0x481c51A4);
	clkCtrlVal |= 0x1 << 23;
	SD_write32(0x481c51A4,clkCtrlVal);	//VIDEO0PLL_CLKCTRL	///®

	udelay(10000);

    //soft reset pll before change of dividers
	SD_write32(0x481c51A4, SD_read32(0x481c51A4)& 0xfffffffe);

	SD_write32(0x481c51b0,0x140007);	//M2=20,	N=7
	SD_write32(0x481c51b4,0x1B0);		//N2=0,M=432       23a  1b0
	SD_write32(0x481c51b8,0x5000000);	//sigma-delta divider
	SD_write32(0x481c51bc,0x3);			//VIDEO0PLL_BWCTRL
	SD_write32(0x481c51c0,0x0);			//VIDEO0PLL_FRACCTRL
	udelay(30);

    //VIDEO0PLL_TENABLEDIV
	SD_write32(0x481c51Ac,0x1);
	udelay(10);
	SD_write32(0x481c51Ac,0x0);
	udelay(10);
    //VIDEO0PLL_TENABLED
	SD_write32(0x481c51A8,0x1);
	udelay(10);
	SD_write32(0x481c51A8,0x0);

	SD_write32(0x481c51A4,0x8111011);	//VIDEO0PLL_CLKCTRL	///®®®å
	udelay(10000);

	//SD_write32(0x48100118,0xd);		//clkc_venc_ena
    vps_write32(VPS_CLKC_VENC_ENABLE,0xF);

	//SD_write32(0x48105200, 0x3000101);	//COMP Compositor Status
    vps_write32(VPS_COMP_STATUS, 0x3010101);

	//SD_write32(0x48100114,0x9000d);
    vps_write32(VPS_CLKC_VENC_CLK_SELECT,0x9010D);

return;
}


static int ti810x_configure_SD_venc_registers(int colorbar)
{
	SD_write32(0x48105e00, 0x4FFF0000);
	if (colorbar)
		SD_write32(0x48105e04, 0x00000015);//15 color bar, 5 normal mode
	else
		SD_write32(0x48105e04, 0x00000005);//15 color bar, 5 normal mode
	SD_write32(0x48105e0c, 0x020D06B4);
	SD_write32(0x48105e18, 0x00090005);
	SD_write32(0x48105e1c, 0x000C0006);
	SD_write32(0x48105e20, 0x00000008);
	SD_write32(0x48105e24, 0x200C000E);
	SD_write32(0x48105e28, 0x069200F2);
	SD_write32(0x48105e2c, 0x02060024);
	SD_write32(0x48105e30, 0x02070025);
	SD_write32(0x48105e34, 0x00040000);
	SD_write32(0x48105e38, 0x00090005);
	SD_write32(0x48105e3c, 0x000C0006);
	SD_write32(0x48105e40, 0x00000008);
	SD_write32(0x48105e44, 0x200C000E);
	SD_write32(0x48105e48, 0x069200F2);
	SD_write32(0x48105e4c, 0x02060022);
	SD_write32(0x48105e50, 0x02070021);
	SD_write32(0x48105e58, 0x00010000);
	SD_write32(0x48105e80, 0x00000011);
	SD_write32(0x48105e84, 0x00000004);
	SD_write32(0x48105e88, 0x069400F4);
	SD_write32(0x48105e8c, 0x02070022);
	SD_write32(0x48105e90, 0x02070022);
	SD_write32(0x48105e94, 0x00D3008F);
	SD_write32(0x48105e9c, 0x030900BC);
	SD_write32(0x48105ea0, 0x016A0004);
	SD_write32(0x48105ea4, 0x034400BC);
	SD_write32(0x48105ea8, 0x00000010);
	SD_write32(0x48105eac, 0x003C0136);
	SD_write32(0x48105eb0, 0x0000009E);
	SD_write32(0x48105eb4, 0x000000A2);
	SD_write32(0x48105eb8, 0x00BF1F82);
	SD_write32(0x48105ebc, 0x00001FBF);
	SD_write32(0x48105ec4, 0x1FD41F1F);
	SD_write32(0x48105ec8, 0x0000010D);
	SD_write32(0x48105ed0, 0x0037011D);
	SD_write32(0x48105ed4, 0x00000091);
	SD_write32(0x48105ed8, 0x0000009D);
	SD_write32(0x48105edc, 0x00D31F74);
	SD_write32(0x48105ee0, 0x00001FB9);
	SD_write32(0x48105ee8, 0x1FD01F06);
	SD_write32(0x48105eec, 0x00001FB9);
	SD_write32(0x48105ef4, 0x00000FFF);
	SD_write32(0x48105ef8, 0x180007FF);
	SD_write32(0x48105efc, 0x180007FF);
	SD_write32(0x48105f00, 0x00000FFF);
	SD_write32(0x48105f04, 0x00000FFF);
	SD_write32(0x48105f08, 0x00000FFF);
	SD_write32(0x48105f10, 0x00004020);
	SD_write32(0x48105f14, 0x0C000000);
	SD_write32(0x48105f18, 0x00002820);
	SD_write32(0x48105f20, 0x004EEC06);
	SD_write32(0x48105f48, 0x00870000);
	SD_write32(0x48105f4c, 0x00210019);
	SD_write32(0x48105f50, 0x0E000000);
	SD_write32(0x48105f58, 0x004EEC06);

    //DAC  SD_DAC_CTRL
	SD_write32(0x48140670, 0x018800DB);

	return 0;
}

static int do_color_bar(cmd_tbl_t * cmdtp,int flag, int argc, char *argv[])
{
	if(ti810x_prcm_init()  == -1) {
		eprintf("ti810x_prcm_init failed");
		return -1;
	}

	if(ti810x_pll_init()  == -1) {
		eprintf("ti810x_pll_init failed");
		return -1;
	}

	if(ti810x_vps_init() == -1) {
		eprintf("ti810x_vps_init failed");
		return -1;
	}

	if(ti810x_set_mode(argv[0])  == -1) {
		eprintf("ti810x_set_mode failed");
		return -1;
	}

    //set the SD clocks Video0 PLL to fixed 54MHz
    ti810x_pll_configure_SD();

    //configure the SD Venc parameters, and set the work mode as colorbar self-test mode
    ti810x_configure_SD_venc_registers(COLORBAR_MODE);

#ifdef CONFIG_ILI9486_LCD
	ili9846_lcd_init();
	//#--- backlight on
	gpio_set_value(GPIO_N(1, 30), 1);
#endif

	return 0;
}

static int do_boot_logo(cmd_tbl_t * cmdtp,int flag, int argc, char *argv[])
{
    uint32_t *firmware_addr;
    uint32_t *list_addr;
	uint32_t *payload_buffer;

	uint32_t image_source=0, image_dest=0;
	uint32_t lcd_image_source=0, lcd_image_dest=0;
	uint32_t vpdma_desc_dest = 0;
	uint32_t disp_time=0, disp_fps=0;
	uint32_t paddr,lcd_paddr;
	uint32_t size;

	/* check whether the arguments passed are correct  */
	if (argc < 2)
	{
		cmd_usage(cmdtp);
		return 1;
	}
	else if (strcmp(argv[1],"on") == 0)
	{
		if (argc > 2) {
			image_source = simple_strtoul(argv[2], NULL, 16);
			image_dest = DEFAULT_IMAGE_DEST_ADDR;
			vpdma_desc_dest = DEFAULT_VPDMA_DESC_ADDR;
			disp_time = DEFAULT_BOOT_LOGO_DISPLAY_TIME_IN_SECS;
			disp_fps = DEFAULT_BOOT_LOGO_DISPLAY_FPS;
		}
	}
	else if (strcmp(argv[1],"off") == 0)
	{
		#if EN_TV_LOGO
		vpdma_write32(VPDMA_LIST_ATTR, vpdma_read32(VPDMA_LIST_ATTR)
		             | (VPDMA_LIST_NUMBER_SD<<24)  /* list number */
		             | (1<<20)  /* stop bit for list  */
		             );
		#endif
		vpdma_write32(VPDMA_LIST_ATTR, vpdma_read32(VPDMA_LIST_ATTR)
		             | (VPDMA_LIST_NUMBER_DVO2<<24)  /* list number */
		             | (1<<20)  /* stop bit for list  */
		             );
		vpdma_write32(VPDMA_LIST_ATTR, vpdma_read32(VPDMA_LIST_ATTR)
		             | (VPDMA_LIST_NUMBER_HD<<24)  /* list number */
		             | (1<<20)  /* stop bit for list  */
		             );
		return 0;
	}
	else
	{
		cmd_usage(cmdtp);
		return 1;
	}

	/* memory address for loading vpdma firmware and descriptors, payload */
    firmware_addr    = (uint32_t*)(vpdma_desc_dest+VPDMA_DESC_FIRMWARE_OFFSET);
    payload_buffer   = (uint32_t*)(vpdma_desc_dest+VPDMA_DESC_PAYLOAD_OFFSET);
    list_addr        = (uint32_t*)(vpdma_desc_dest+VPDMA_DESC_LIST_OFFSET);

	/* memory address for displaying the image formatted on the hdmi, the offset from the source location is 2 * WIDTH * HEIGHT */
	//image_dest = image_source + 3 * 3 * WIDTH * HEIGHT;

	if(image_source == image_dest) {
		eprintf("<logo_read_address> & <final_bmp address> cannot be same\n");
		return -1;
	}
	if((image_source == 0) || (image_dest == 0)) {
		eprintf("<logo_read_address> or <final_bmp address> is NULL\n");
		return -1;
	}

	#if EN_TV_LOGO
	if(ti810x_read_bmp_image(0, image_source, image_dest)  == -1) {
		eprintf("reading BMP image for tvout failed\r\n");
		return -1;
	}
	#endif

	#ifdef LCD_DVO2
	lcd_image_source = image_source + 0x100000;
	lcd_image_dest   = image_dest + 0x100000;

	memcpy((void *)lcd_image_dest, (void *)lcd_image_source, LCD_FRAME_SIZE);
	#endif // #ifdef LCD_DVO2


	if(ti810x_prcm_init()  == -1) {
		eprintf("ti810x_prcm_init failed");
		return -1;
	}
	if(ti810x_pll_init()  == -1) {
		eprintf("ti810x_pll_init failed");
		return -1;
	}
	if(ti810x_vps_init()  == -1) {
		eprintf("ti810x_vps_init failed");
		return -1;
	}
	if(ti810x_set_mode(argv[0]) == -1) {
		eprintf("ti810x_set_mode failed");
		return -1;
	}

    //set the SD clocks Video0 PLL to fixed 54MHz
    ti810x_pll_configure_SD();

	#if EN_TV_LOGO
    //configure the SD Venc parameters, and set the work mode as normal mode
    ti810x_configure_SD_venc_registers(NORMAL_MODE);
	#endif

	if (ti810x_vpdma_load_firmware(firmware_addr) == -1) {
		eprintf("vpdma_load_firmware failed");
		return -1;
	}

	/* destination address */
	paddr     = image_dest;
	lcd_paddr = lcd_image_dest;

	//# HDMI->VGA Configuration Desc:GRPX0
	size = ti810x_dispmgr_create_grpx_conf_descriptor(0, list_addr, payload_buffer, WIDTH, HEIGHT);
	ti810x_vpdma_send_list(list_addr, size, 0);//last param 0, list number hardcoded for HDMI->VGA

	if(ti810x_dispmgr_wait_for_list_complete(0) == -1) {
		eprintf("wait for list complete FAILED!");
		return -1;
	}

	#ifdef LCD_DVO2
	//# DVO2 Configuration Desc:GRPX1
	size = ti810x_dispmgr_create_grpx_conf_descriptor(1, list_addr+0x20000, payload_buffer+0x10, LCD_WIDTH, LCD_HEIGHT);
	ti810x_vpdma_send_list(list_addr + 0x20000, size, 1);//last param 1, list number hardcoded for DVO2

	if(ti810x_dispmgr_wait_for_list_complete(1) == -1) {
		eprintf("wait for list complete FAILED!");
		return -1;
	}
	#endif //#ifdef LCD_DVO2

	//# SD Configuration Desc:GRPX2
	size = ti810x_dispmgr_create_grpx_conf_descriptor(2, list_addr+0x10000, payload_buffer+0x8, WIDTH, HEIGHT);
	ti810x_vpdma_send_list(list_addr + 0x10000, size, 2);

	if(ti810x_dispmgr_wait_for_list_complete(2) == -1) {
		eprintf("wait for list complete FAILED!");
		return -1;
	}

    // the frame start event must be properly set, or will cause screen scroll issue
    //set the frame start event of GRPX2 to 'channel active'

	vpdma_write32(VPDMA_GRPX0_DATA_CSTAT,0x00000000);// Hardcoded: HDMI(+VGA)->GRPX0
	vpdma_write32(VPDMA_GRPX1_DATA_CSTAT,0x00000400);// Hardcoded: DVO2->GRPX1
	#if EN_TV_LOGO
	vpdma_write32(VPDMA_GRPX2_DATA_CSTAT,0x00000C00);// Hardcoded: SD->GRPX2
	#endif
    //set the frame start event of GRPX1 to 'SD field ID changes'

    //DATA Descriptors for HDMI/VGA
	size = ti810x_dispmgr_setup_layers(list_addr, STRIDE, WIDTH, HEIGHT,
									disp_time, disp_fps, paddr);
	ti810x_vpdma_send_list(list_addr, size, VPDMA_LIST_NUMBER_HD);	//list_no = 4

    //create data transfer descriptor for GRPX1
	//the list_addr has 0x10000 offset, because 2 GRPX share the same GRPX desc buffer = 4096 descs
	#ifdef LCD_DVO2
	size = ti810x_dispmgr_setup_layers_DVO2((list_addr+0x20000), LCD_STRIDE, LCD_WIDTH, LCD_HEIGHT,
									disp_time, disp_fps, lcd_paddr);
	ti810x_vpdma_send_list((list_addr+0x20000), size, VPDMA_LIST_NUMBER_DVO2);	//list_no = 3
	#endif

	#if EN_TV_LOGO
	//DATA Descriptors for SD
	size = ti810x_dispmgr_setup_layers_SD((list_addr+0x10000), STRIDE, WIDTH, HEIGHT,
									disp_time, disp_fps, paddr);
	ti810x_vpdma_send_list((list_addr+0x10000), size, VPDMA_LIST_NUMBER_SD);	//list_no = 5
	#endif

#ifdef CONFIG_NT35510
	/* lcd enable */
	if (lcd_enable())
		return -1;

	lcd_spi_free();
#endif

#ifdef CONFIG_ILI9486_LCD
	ili9846_lcd_init();
	//#--- backlight on
	gpio_set_value(GPIO_N(1, 30), 1);
#endif

	return 0;
}

U_BOOT_CMD(colorbar, 4, 1, do_color_bar, "Displays a color bar using HDVPSS","");
U_BOOT_CMD(logo, 7, 0, do_boot_logo, "Displays a logo using HDVPSS",
"on <logo_read_address> <final_BMP DSS read address> <VPDMA Descriptor address> <Display time in seconds> <Display fps (less than 60)> \n"
"logo off");
