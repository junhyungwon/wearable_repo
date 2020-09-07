/*
 * (C) Copyright 2006-2008
 * Texas Instruments, <www.ti.com>
 * Richard Woodruff <r-woodruff2@ti.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _CLOCKS_TI813X_H_
#define _CLOCKS_TI813X_H_

/*
 * UDWORKS Inc.
 * DM385 Default Clock Configuration.
 * ARM - 400, HDVICP2-297, HDVPSS-100, ISS-300
 * MEDIA_CTLR(M3)-100, L3/L4-200 DDR-400
 * Core 1.2V (OPP_CONFIG=120)
 *
 * DM8107 Default Clock Configuration.
 * ARM - 600, HDVICP2-297, HDVPSS-200, ISS-300
 * MEDIA_CTLR(M3)-100, L3/L4-200 DDR-533
 * Core 1.35V (OPP_CONFIG=120)
 */
#define OSC_0_FREQ	20

#define DCO_HS2_MIN	500
#define DCO_HS2_MAX	1000
#define DCO_HS1_MIN	1000
#define DCO_HS1_MAX	2000

#define SELFREQDCO_HS2			0x00000801
#define SELFREQDCO_HS1			0x00001001
#define ADPLLJ_CLKCRTL_CLKDCO   0x201a0000

/* Put the pll config values over here */
//# Main PLL (ARM)
#define MODENA_N		0x1

/* CLKINP = OSC Clock = 20MHz
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[40/(1+1)]*20*(1/1)
 *        = 400MHz.
 */
#define MODENA_M		40
#define MODENA_M2		1
#define MODENA_CLKCTRL	0x1

//# L3 PLL
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[800/(19+1)]*20*(1/4)
 */
#define L3_N		19
#define L3_M		800  /* L3 Clock to 200Mhz(800) */
#define L3_M2		4
#define L3_CLKCTRL	0x801

//# DDR PLL
#define DDR_N		19

/* For 400 MHz */
#if defined(CONFIG_DM385_DDR3_400)
#define DDR_M		800
#endif /* #if defined(CONFIG_DM385_DDR3_400) */

/* For 533 MHz */
#if defined(CONFIG_DM385_DDR3_533)
#define DDR_M		1066
#endif /* #if defined(CONFIG_DM385_DDR3_533) */

#define DDR_M2			2
#define DDR_CLKCTRL		0x801

//# DSP PLL
#define DSP_N			19
#define DSP_M			500
#define DSP_M2			1
#define DSP_CLKCTRL		0x801

//# DSS PLL
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[800/(19+1)]*20*(1/4)
 *        = 200MHz (mcfw reconfigure)
 */
#define DSS_N			19
#define DSS_M			800
#define DSS_M2			4
#define DSS_CLKCTRL		0x801

//# IVA PLL
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[640/(19+1)]*20*(1/2)
 */
#define IVA_N			19
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[594/(19+1)]*20*(1/2)
 *        = 297MHz
 */
#ifdef CONFIG_UCX
#	define IVA_M			640
#else
#	define IVA_M			594
#endif

#define IVA_M2			2
#define IVA_CLKCTRL		0x801

//# ISS PLL
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[600/(19+1)]*20*(1/2)
 *        = 300Mhz
 */
#define ISS_N			19
#define ISS_M			600
#define ISS_M2			2
#define ISS_CLKCTRL		0x801

//# Audio PLL (McASP)
/* CLKINP = OSC Clock = 20MHz
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[500/(19+1)]*20*(1/2)
 *        = 125MHz.
 */
#define AUDIO_N			19
#define AUDIO_M			500
#define AUDIO_M2		2
#define AUDIO_CLKCTRL	0x801

//# USB PLL
/* CLKINP = OSC Clock = 20MHz
 * DCOCLK = not connected
 * CLKOUT = [M/(N+1)]*CLKINP*(1/M2)=[960/(19+1)]*20*(1/5)
 *        = 192MHz
 */
#define USB_N			19
#define USB_M			960
#define USB_M2			5
#define USB_CLKCTRL		0x200a0801

#define HDMI_N			19
#define HDMI_M			1485
#define HDMI_M2			10
#define HDMI_CLKCTRL	0x290a1001

#define VIDEO0_N		19
#define VIDEO0_M		540
#define VIDEO0_M2		10
#define VIDEO0_CLKCTRL	0x09000801

#define VIDEO1_N		19
#define VIDEO1_M		1485
#define VIDEO1_M2		10
#define VIDEO1_CLKCTRL	0x290a1001

#endif	/* endif _CLOCKS_TI813X_H_ */
