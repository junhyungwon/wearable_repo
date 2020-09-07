/* include/video/ili9325.h
 *
 * ILI9325D LCD controller configuration control.
 *
 * Based on ILI9325.h Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#define ILI9325_REG(x)	(x)

#define ILI9325_INDEX			ILI9325_REG(0x00)

#define ILI9325_DRIVER			ILI9325_REG(0x01)
#define ILI9325_DRIVEWAVE		ILI9325_REG(0x02)
#define ILI9325_ENTRYMODE		ILI9325_REG(0x03)
#define ILI9325_FORMATCTL		ILI9325_REG(0x05)
#define ILI9325_DISPLAY1		ILI9325_REG(0x07)
#define ILI9325_DISPLAY2		ILI9325_REG(0x08)
#define ILI9325_DISPLAY3		ILI9325_REG(0x09)
#define ILI9325_DISPLAY4		ILI9325_REG(0x0A)
#define ILI9325_RGB_IF1			ILI9325_REG(0x0C)
#define ILI9325_FRAMEMAKER		ILI9325_REG(0x0D)
#define ILI9325_RGB_IF2			ILI9325_REG(0x0F)

#define ILI9325_POWER1				ILI9325_REG(0x10)
#define ILI9325_POWER2				ILI9325_REG(0x11)
#define ILI9325_POWER3				ILI9325_REG(0x12)
#define ILI9325_POWER4				ILI9325_REG(0x13)
#define ILI9325_GRAM_HORIZ_ADDR		ILI9325_REG(0x20)
#define ILI9325_GRAM_VERT_ADD		ILI9325_REG(0x21)
#define ILI9325_POWER7				ILI9325_REG(0x29)
#define ILI9325_FRAME_RATE_COLOUR	ILI9325_REG(0x2B)

#define ILI9325_GAMMA1			ILI9325_REG(0x30)
#define ILI9325_GAMMA2			ILI9325_REG(0x31)
#define ILI9325_GAMMA3			ILI9325_REG(0x32)
#define ILI9325_GAMMA4			ILI9325_REG(0x35)
#define ILI9325_GAMMA5			ILI9325_REG(0x36)
#define ILI9325_GAMMA6			ILI9325_REG(0x37)
#define ILI9325_GAMMA7			ILI9325_REG(0x38)
#define ILI9325_GAMMA8			ILI9325_REG(0x39)
#define ILI9325_GAMMA9			ILI9325_REG(0x3C)
#define ILI9325_GAMMA10			ILI9325_REG(0x3D)

#define ILI9325_HORIZ_START		ILI9325_REG(0x50)
#define ILI9325_HORIZ_END		ILI9325_REG(0x51)
#define ILI9325_VERT_START		ILI9325_REG(0x52)
#define ILI9325_VERT_END		ILI9325_REG(0x53)

#define ILI9325_DRIVER2			ILI9325_REG(0x60)
#define ILI9325_BASE_IMAGE		ILI9325_REG(0x61)
#define ILI9325_SPI_CTRL		ILI9325_REG(0x66)
#define ILI9325_VERT_SCROLL		ILI9325_REG(0x6a)

#define ILI9325_PARTIAL1_POSITION	ILI9325_REG(0x80)
#define ILI9325_PARTIAL1_START		ILI9325_REG(0x81)
#define ILI9325_PARTIAL1_END		ILI9325_REG(0x82)
#define ILI9325_PARTIAL2_POSITION	ILI9325_REG(0x83)
#define ILI9325_PARTIAL2_START		ILI9325_REG(0x84)
#define ILI9325_PARTIAL2_END		ILI9325_REG(0x85)

#define ILI9325_INTERFACE1			ILI9325_REG(0x90)
#define ILI9325_INTERFACE2			ILI9325_REG(0x92)
#define ILI9325_INTERFACE4			ILI9325_REG(0x95)

/* Register contents definitions. */
#define ILI9325_OSCILATION_OSC		(1 << 0)

#define ILI9325_DRIVER_SS			(1 << 8)
#define ILI9325_DRIVER_SM			(1 << 10)

#define ILI9325_DRIVEWAVE_BC		(1 << 9)

#define ILI9325_ENTRYMODE_AM		(1 << 3)
#define ILI9325_ENTRYMODE_ID(x)		((x) << 4)
#define ILI9325_ENTRYMODE_ORG		(1 << 7)
#define ILI9325_ENTRYMODE_HWM		(1 << 8)
#define ILI9325_ENTRYMODE_BGR		(1 << 12)
#define ILI9325_ENTRYMODE_DFM		(1 << 14)
#define ILI9325_ENTRYMODE_TRI		(1 << 15)

#define ILI9325_RESIZING_RSZ(x)		((x) << 0)
#define ILI9325_RESIZING_RCH(x)		((x) << 4)
#define ILI9325_RESIZING_RCV(x)		((x) << 8)

#define ILI9325_DISPLAY1_D(x)		((x) << 0)
#define ILI9325_DISPLAY1_CL			(1 << 3)
#define ILI9325_DISPLAY1_DTE		(1 << 4)
#define ILI9325_DISPLAY1_GON		(1 << 5)
#define ILI9325_DISPLAY1_BASEE		(1 << 8)
#define ILI9325_DISPLAY1_PTDE(x)	((x) << 12)

#define ILI9325_DISPLAY2_BP(x)		((x) << 0)
#define ILI9325_DISPLAY2_FP(x)		((x) << 8)

#define ILI9325_RGBIF1_RIM_RGB18	(0 << 0)
#define ILI9325_RGBIF1_RIM_RGB16	(1 << 0)
#define ILI9325_RGBIF1_RIM_RGB6		(2 << 0)
#define ILI9325_RGBIF1_CLK_INT		(0 << 4)
#define ILI9325_RGBIF1_CLK_RGBIF	(1 << 4)
#define ILI9325_RGBIF1_CLK_VSYNC	(2 << 4)

#define ILI9325_RGBIF1_RM			(1 << 8)

#define ILI9325_RGBIF1_ENC_FRAMES(x)	(((x) - 1)<< 13)

#define ILI9325_RGBIF2_DPL		(1 << 0)
#define ILI9325_RGBIF2_EPL		(1 << 1)
#define ILI9325_RGBIF2_HSPL		(1 << 3)
#define ILI9325_RGBIF2_VSPL		(1 << 4)

#define ILI9325_POWER1_SLP		(1 << 1)
#define ILI9325_POWER1_DSTB			(1 << 2)
#define ILI9325_POWER1_AP(x)		((x) << 4)
#define ILI9325_POWER1_APE			(1 << 7)
#define ILI9325_POWER1_BT(x)		((x) << 8)
#define ILI9325_POWER1_SAP			(1 << 12)

#define ILI9325_POWER2_VC(x)		((x) << 0)
#define ILI9325_POWER2_DC0(x)		((x) << 4)
#define ILI9325_POWER2_DC1(x)		((x) << 8)

#define ILI9325_POWER3_VRH(x)		((x) << 0)
#define ILI9325_POWER3_PON			(1 << 4)
#define ILI9325_POWER3_VCMR			(1 << 8)

#define ILI9325_POWER4_VREOUT(x)	((x) << 8)

#define ILI9325_DRIVER2_SCNL(x)		((x) << 0)
#define ILI9325_DRIVER2_NL(x)		((x) << 8)
#define ILI9325_DRIVER2_GS			(1 << 15)

#define ILI9325_BASEIMAGE_REV		(1 << 0)
#define ILI9325_BASEIMAGE_VLE		(1 << 1)
#define ILI9325_BASEIMAGE_NDL		(1 << 2)

#define ILI9325_INTERFACE2_NOWI(x)	((x) << 8)

#define ILI9325_INTERFACE4_RTNE(x)	(x)
#define ILI9325_INTERFACE4_DIVE(x)	((x) << 8)

/* SPI interface definitions */
#define ILI9325_SPI_IDCODE			(0x70)
#define ILI9325_SPI_ID(x)			((x) << 2)
#define ILI9325_SPI_READ			(0x01)
#define ILI9325_SPI_WRITE			(0x00)
#define ILI9325_SPI_DATA			(0x02)
#define ILI9325_SPI_INDEX			(0x00)

/* platform data to pass configuration from lcd */
enum ILI9325_suspend {
	ILI9325_SUSPEND_OFF,
	ILI9325_SUSPEND_DEEP,
};

struct ili9325_platdata {
	unsigned short	hsize;
	unsigned short	vsize;

	enum ILI9325_suspend suspend;

	/* set the reset line, 0 = reset asserted, 1 = normal */
	void		(*reset)(unsigned int val);

	unsigned short	entry_mode;
	unsigned short	display2;
	unsigned short	display3;
	unsigned short	display4;
	unsigned short	rgb_if1;
	unsigned short	rgb_if2;
	unsigned short	interface2;
	unsigned short	interface4;
	unsigned short	interface5;
};

