/*
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
#ifndef _UBX_LCD_H_
#define _UBX_LCD_H_

#define LCD_REG(x)	(x)

#define INDEX			LCD_REG(0x00)

#define DRIVER			LCD_REG(0x01)
#define DRIVEWAVE		LCD_REG(0x02)
#define ENTRYMODE		LCD_REG(0x03)
#define FORMATCTL		LCD_REG(0x05)
#define DISPLAY1		LCD_REG(0x07)
#define DISPLAY2		LCD_REG(0x08)
#define DISPLAY3		LCD_REG(0x09)
#define DISPLAY4		LCD_REG(0x0A)
#define RGB_IF1			LCD_REG(0x0C)
#define FRAMEMAKER		LCD_REG(0x0D)
#define RGB_IF2			LCD_REG(0x0F)

#define POWER1				LCD_REG(0x10)
#define POWER2				LCD_REG(0x11)
#define POWER3				LCD_REG(0x12)
#define POWER4				LCD_REG(0x13)
#define GRAM_HORIZ_ADDR		LCD_REG(0x20)
#define GRAM_VERT_ADD		LCD_REG(0x21)
#define POWER7				LCD_REG(0x29)
#define FRAME_RATE_COLOUR	LCD_REG(0x2B)

#define GAMMA1			LCD_REG(0x30)
#define GAMMA2			LCD_REG(0x31)
#define GAMMA3			LCD_REG(0x32)
#define GAMMA4			LCD_REG(0x35)
#define GAMMA5			LCD_REG(0x36)
#define GAMMA6			LCD_REG(0x37)
#define GAMMA7			LCD_REG(0x38)
#define GAMMA8			LCD_REG(0x39)
#define GAMMA9			LCD_REG(0x3C)
#define GAMMA10			LCD_REG(0x3D)

#define HORIZ_START		LCD_REG(0x50)
#define HORIZ_END		LCD_REG(0x51)
#define VERT_START		LCD_REG(0x52)
#define VERT_END		LCD_REG(0x53)

#define DRIVER2			LCD_REG(0x60)
#define BASE_IMAGE		LCD_REG(0x61)
#define SPI_CTRL		LCD_REG(0x66)
#define VERT_SCROLL		LCD_REG(0x6a)

#define PARTIAL1_POSITION	LCD_REG(0x80)
#define PARTIAL1_START		LCD_REG(0x81)
#define PARTIAL1_END		LCD_REG(0x82)
#define PARTIAL2_POSITION	LCD_REG(0x83)
#define PARTIAL2_START		LCD_REG(0x84)
#define PARTIAL2_END		LCD_REG(0x85)

#define INTERFACE1			LCD_REG(0x90)
#define INTERFACE2			LCD_REG(0x92)
#define INTERFACE4			LCD_REG(0x95)

#define OSCILATION_OSC		(1 << 0)

#define DRIVER_SS			(1 << 8)
#define DRIVER_SM			(1 << 10)

#define DRIVEWAVE_BC		(1 << 9)

#define ENTRYMODE_AM		(1 << 3)
#define ENTRYMODE_ID(x)		((x) << 4)
#define ENTRYMODE_ORG		(1 << 7)
#define ENTRYMODE_HWM		(1 << 8)
#define ENTRYMODE_BGR		(1 << 12)
#define ENTRYMODE_DFM		(1 << 14)
#define ENTRYMODE_TRI		(1 << 15)

#define RESIZING_RSZ(x)		((x) << 0)
#define RESIZING_RCH(x)		((x) << 4)
#define RESIZING_RCV(x)		((x) << 8)

#define DISPLAY1_D(x)		((x) << 0)
#define DISPLAY1_CL			(1 << 3)
#define DISPLAY1_DTE		(1 << 4)
#define DISPLAY1_GON		(1 << 5)
#define DISPLAY1_BASEE		(1 << 8)
#define DISPLAY1_PTDE(x)	((x) << 12)

#define DISPLAY2_BP(x)		((x) << 0)
#define DISPLAY2_FP(x)		((x) << 8)

#define RGBIF1_RIM_RGB18	(0 << 0)
#define RGBIF1_RIM_RGB16	(1 << 0)
#define RGBIF1_RIM_RGB6		(2 << 0)
#define RGBIF1_CLK_INT		(0 << 4)
#define RGBIF1_CLK_RGBIF	(1 << 4)
#define RGBIF1_CLK_VSYNC	(2 << 4)

#define RGBIF1_RM			(1 << 8)

#define RGBIF1_ENC_FRAMES(x)	(((x) - 1)<< 13)

#define RGBIF2_DPL		(1 << 0)
#define RGBIF2_EPL		(1 << 1)
#define RGBIF2_HSPL		(1 << 3)
#define RGBIF2_VSPL		(1 << 4)

#define POWER1_SLP		(1 << 1)
#define POWER1_DSTB			(1 << 2)
#define POWER1_AP(x)		((x) << 4)
#define POWER1_APE			(1 << 7)
#define POWER1_BT(x)		((x) << 8)
#define POWER1_SAP			(1 << 12)

#define POWER2_VC(x)		((x) << 0)
#define POWER2_DC0(x)		((x) << 4)
#define POWER2_DC1(x)		((x) << 8)

#define POWER3_VRH(x)		((x) << 0)
#define POWER3_PON			(1 << 4)
#define POWER3_VCMR			(1 << 8)

#define POWER4_VREOUT(x)	((x) << 8)

#define DRIVER2_SCNL(x)		((x) << 0)
#define DRIVER2_NL(x)		((x) << 8)
#define DRIVER2_GS			(1 << 15)

#define BASEIMAGE_REV		(1 << 0)
#define BASEIMAGE_VLE		(1 << 1)
#define BASEIMAGE_NDL		(1 << 2)

#define INTERFACE2_NOWI(x)	((x) << 8)

#define INTERFACE4_RTNE(x)	(x)
#define INTERFACE4_DIVE(x)	((x) << 8)

#define ILI9325_SPI_IDCODE			(0x70)
#define ILI9325_SPI_ID(x)			((x) << 2)
#define ILI9325_SPI_DATA			(0x02)
#define ILI9325_SPI_INDEX			(0x00)

struct ili9325 {
	unsigned int bus;
	unsigned int cs;
	unsigned int id;
	struct spi_slave *spi;
};

extern int ili9325_spi_init(struct ili9325 *data);
extern int ili9325_spi_write(struct ili9325 *data, u16 idx, u16 param);

#endif
