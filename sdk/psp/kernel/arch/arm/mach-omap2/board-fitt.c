/*
 * Code for FITT.
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#ifdef CONFIG_GPIO_PCA953X
#include <linux/i2c/pca953x.h>
#endif
#include <linux/regulator/machine.h>
#include <linux/input.h>
#include <linux/input/kxcj9.h>
#include <linux/pwm_backlight.h>
#include <sound/tlv320aic3x.h>
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/irqs.h>
#include <plat/board.h>
#include <plat/common.h>
#include <plat/asp.h>
#include <plat/usb.h>
#include <plat/mmc.h>
#include <plat/gpmc.h>
#include <plat/nand.h>

#ifdef CONFIG_HAVE_PWM
#include <plat/pwm.h>
#endif

#include "board-flash.h"
#include "clock.h"
#include "mux.h"
#include "hsmmc.h"
#include "control.h"

#define BOARD_REV_OFFSET		(TI81XX_GPIO0_BASE+0x138)
#define PWR_HOLD				GIO(2, 26)	//# power hold

#define GPIO_I2C_BUS_BASE 		5

#ifdef CONFIG_OMAP_MUX
static struct omap_board_mux board_mux[] __initdata = {
	{ .reg_offset = OMAP_MUX_TERMINATOR },
};
#else
#define board_mux     NULL
#endif

u32 board_rev(void)
{
	void __iomem *reg =
			TI81XX_L4_SLOW_IO_ADDRESS(BOARD_REV_OFFSET);

	/* get gpio bank0_(1,2,3,4) */
	return ((__raw_readl(reg)>>1)&0x0f);
}
EXPORT_SYMBOL(board_rev);

static struct aic3x_pdata aic3x_pdata = {
	.gpio_reset = 0, /* < 0 if not used */
	.setup = NULL,
	.micbias_vg = AIC3X_MICBIAS_2_5V, /* enum->1 */
};

static struct i2c_board_info __initdata i2c0_boardinfo[2];

#ifdef CONFIG_GPIO_PCA953X
/* GPIO expander platform data */
static struct pca953x_platform_data pca953x_pdata = {
	.gpio_base	= 192, /* cpu gpio is 191 */
	.invert		= 0,
	.irq_base   = -1,
	.context    = NULL,
	.setup		= NULL,
	.teardown	= NULL,
};

static struct i2c_board_info __initdata gpio_i2c_bus_boardinfo[] = {
	{
		/* tca6424a */
		I2C_BOARD_INFO("tca6424", 0x22),
		.platform_data = &pca953x_pdata,
		.irq = 0,
	},
};
#endif

static void __init fitt_i2c_init(void)
{
	strcpy(i2c0_boardinfo[0].type, "tlv320aic3x");
	i2c0_boardinfo[0].addr = 0x18;
	i2c0_boardinfo[0].platform_data = &aic3x_pdata;

	/*
	 * RTC of new board is intersil 1208.
	 */
	if (board_rev() >= 9) {
		strcpy(i2c0_boardinfo[1].type, "isl1208");
		i2c0_boardinfo[1].addr = 0x6F;
		i2c0_boardinfo[1].platform_data = NULL;
	} else {
		strcpy(i2c0_boardinfo[1].type, "mcp7941x");
		i2c0_boardinfo[1].addr = 0x6F;
		i2c0_boardinfo[1].platform_data = NULL;
	}

	/* There are 4 instances of I2C in TI814X but currently only one
	 * instance is being used on the TI8148 EVM
	 */
	omap_register_i2c_bus(1, 400, i2c0_boardinfo, 2);

#ifdef CONFIG_GPIO_PCA953X
	/* gpio i2c bus device register */
	i2c_register_board_info(GPIO_I2C_BUS_BASE, gpio_i2c_bus_boardinfo,
					ARRAY_SIZE(gpio_i2c_bus_boardinfo));
#endif
}

/* NAND flash information */
static struct mtd_partition fitt_nand_partitions[] = {
/* All the partition sizes are listed in terms of NAND block size */
	{
		.name           = "U-Boot-min",
		.offset         = 0,                   /* 0x000000000000-0x000000020000 */
		.size           = SZ_128K,
		.mask_flags	    = MTD_WRITEABLE,       /* force read-only */
	},
	{
		.name           = "U-Boot",
		.offset         = MTDPART_OFS_APPEND,  /* 0x000000020000-0x0000000c0000 */
		.size           = 5 * SZ_128K,
		.mask_flags	    = MTD_WRITEABLE,       /* force read-only */
	},
	{
		.name           = "U-Boot Env",
		.offset         = MTDPART_OFS_APPEND,   /* 0x0000000c0000-0x000000120000 */
		.size           = 3 * SZ_128K,
		.mask_flags		= 0,
	},
	{
		.name           = "boot logo",
		.offset         = MTDPART_OFS_APPEND,   /* 0x000000120000-0x000000220000 */
		.size           = 8 * SZ_128K,
		.mask_flags		= MTD_WRITEABLE,		/* force read-only */
	},
	{
		.name           = "Kernel",
		.offset         = MTDPART_OFS_APPEND,   /* 0x000000220000-0x000000520000 */
		.size           = 24 * SZ_128K,         /* 3072KB */
		.mask_flags		= 0,
	},
	{
		.name           = "File System",
		.offset         = MTDPART_OFS_APPEND,   /* 0x000000520000-0x000004C00000 */
		.size           = 567 * SZ_128K,        /* 70.875MB (0x46E0000)*/
		.mask_flags		= 0,
	},
	{
		.name           = "Data",
		.offset         = MTDPART_OFS_APPEND,   /* 0x000004C00000-0x000007c00000 */
		.size           = 384 * SZ_128K,        /* 48MB (0x3000000)*/
		.mask_flags		= 0,
	},
	{
		.name           = "Reserved",
		.offset         = MTDPART_OFS_APPEND,   /* 0x000007c00000-0x000008000000 */
		.size           = MTDPART_SIZ_FULL,     /* 4MB */
		.mask_flags		= 0,
	},
};

static struct omap_musb_board_data musb_board_data = {
	.interface_type		= MUSB_INTERFACE_ULPI,
#ifdef CONFIG_USB_MUSB_OTG
	.mode           = MUSB_OTG,
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
	.mode           = MUSB_HOST,
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
	.mode           = MUSB_PERIPHERAL,
#endif
	.power		= 500,
	.instances	= 1,
};

static void __init fitt_init_irq(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
	omap_init_irq();
	gpmc_init();
}

#ifdef CONFIG_SND_SOC_TLV320AIC3X
static u8 aic3x_iis_serializer_direction[] = {
	TX_MODE,	RX_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,	INACTIVE_MODE,
};

/*
 * The McASP version 2 supports a hardware FIFO of 256bytes
 * each Tx/Rx which provides additional data buffering and allows
 * tolerance to variations in DMA controller response times.
 */
static struct snd_platform_data aic3x_snd_data = {
	.tx_dma_offset	= 0x46400000, /* MCA[1] */
	.rx_dma_offset	= 0x46400000,
	.op_mode	= DAVINCI_MCASP_IIS_MODE,
	.num_serializer = ARRAY_SIZE(aic3x_iis_serializer_direction),
	.tdm_slots	= 2, /* number of channels */
	.serial_dir	= aic3x_iis_serializer_direction,
	.asp_chan_q	= EVENTQ_0, /* don't use Q2 */
	.version	= MCASP_VERSION_2,
	.txnumevt = 1, //# 32 (1 word 4byte)
	.rxnumevt = 1, //# 32 (1 word 4byte)

	/* McASP21_AHCLKX out to feed CODEC CLK*/
	.clk_input_pin	= MCASP_AHCLKX_OUT,
};

static struct resource ti81xx_mcasp1_resource[] = {
    {
        .name = "mcasp1",
        .start = TI81XX_ASP1_BASE,
        .end = TI81XX_ASP1_BASE + (SZ_1K * 12) - 1,
        .flags = IORESOURCE_MEM,
    },
    /* TX event */
    {
        .start = TI81XX_DMA_MCASP1_AXEVT,
        .end = TI81XX_DMA_MCASP1_AXEVT,
        .flags = IORESOURCE_DMA,
    },
    /* RX event */
    {
        .start = TI81XX_DMA_MCASP1_AREVT,
        .end = TI81XX_DMA_MCASP1_AREVT,
        .flags = IORESOURCE_DMA,
    },
};

static struct platform_device mcasp_aic_dev = {
    .name = "davinci-mcasp",
    .id = 1,
    .dev ={
		.platform_data = &aic3x_snd_data,
	},
    .num_resources = ARRAY_SIZE(ti81xx_mcasp1_resource),
    .resource = ti81xx_mcasp1_resource,
};
#endif /* #ifdef CONFIG_SND_SOC_TLV320AIC3X */

static struct omap2_hsmmc_info mmc[] = {
	{
		.mmc		= 1,
		.caps		= MMC_CAP_4_BIT_DATA,
		.gpio_cd	= GIO(1, 7),
		.gpio_wp	= -EINVAL,
		.ocr_mask	= MMC_VDD_33_34,
	},
	{}	/* Terminator */
};

/* For PWM-Beeper */
static struct omap2_pwm_platform_config fitt_pwm_cfg[] = {
	{
		.timer_id = 7,
		.polarity = 1,
	}
};

static struct platform_pwm_backlight_data fitt_backlight_data = {
	.pwm_id         = 0,
	.max_brightness = 100,
	.dft_brightness = 0,    	/* default 50 */
	.pwm_period_ns  = 366166L,  /* 2731Hz */ //500000L,   /* 2000Hz */
};

static struct platform_device backlight_dev = {
	.name = "pwm-backlight",
	.dev  = {
		.platform_data = &fitt_backlight_data,
	},
	.id   = -1,
};

#ifdef CONFIG_I2C_GPIO
/* GPIO I2C Bus */
static struct i2c_gpio_platform_data i2c_gpio_bus_data = {
	.scl_pin 			= GIO(3, 12), /* GIO(2, 5) */
	.sda_pin 			= GIO(3, 13), /* GIO(1, 4) */
	.sda_is_open_drain 	= 0,
	.scl_is_open_drain 	= 0,
	.udelay 			= 10,
};

static struct platform_device i2c_gpio_bus_dev = {
	.name = "i2c-gpio",
	.dev  = {
		.platform_data = &i2c_gpio_bus_data,
	},
	.id   = GPIO_I2C_BUS_BASE, /* cpu i2c is 4 instance */
};
#endif

#ifdef CONFIG_LEDS_GPIO_PLATFORM
/* gpio led */
static struct gpio_led fitt_leds[] = {
	{
		.name				= "led0", /* LED_RF_G */
		.default_trigger 	= NULL,
		.gpio 				= 192,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led1", /* LED_RF_R */
		.default_trigger 	= NULL,
		.gpio 				= 193,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led2", /* LED_GPS_G */
		.default_trigger 	= NULL,
		.gpio 				= 194,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led3", /* LED_GPS_R */
		.default_trigger 	= NULL,
		.gpio 				= 195,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led4", /* LED_BAT_G1 */
		.default_trigger 	= NULL,
		.gpio 				= 196,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led5", /* LED_BAT_G2 */
		.default_trigger 	= NULL,
		.gpio 				= 197,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led6", /* LED_BAT_G3 */
		.default_trigger 	= NULL,
		.gpio 				= 198,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led7", /* LED_BAT_G4 */
		.default_trigger 	= NULL,
		.gpio 				= 199,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led8", /* LED_CAM1 */
		.default_trigger 	= NULL,
		.gpio 				= 200,
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led9", /* LED_CAM2 */
		.default_trigger 	= NULL,
		.gpio 				= 201,
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led10", /* LED_CAM3 */
		.default_trigger 	= NULL,
		.gpio 				= 202,
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led11", /* LED_CAM4 */
		.default_trigger 	= NULL,
		.gpio 				= 203,
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led12", /* LED_SD_G */
		.default_trigger 	= NULL,
		.gpio 				= 208,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led13", /* LED_SD_R */
		.default_trigger 	= NULL,
		.gpio 				= 209,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led14", /* LED_SD_G1 */
		.default_trigger 	= NULL,
		.gpio 				= 210,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led15", /* LED_SD_G2 */
		.default_trigger 	= NULL,
		.gpio 				= 211,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led16", /* LED_SD_G3 */
		.default_trigger 	= NULL,
		.gpio 				= 212,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led17", /* LED_REC */
		.default_trigger 	= NULL,
		.gpio 				= 215,
		.active_low 		= 1,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led18", /* BACKUP_STATE */
		.default_trigger 	= NULL,
		.gpio 				= GIO(2, 6),
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
	{
		.name				= "led19", /* ETH_STATUS */
		.default_trigger 	= NULL,
		.gpio 				= GIO(1, 8),
		.active_low 		= 0,
		.default_state		= LEDS_GPIO_DEFSTATE_OFF,
	},
};

static struct gpio_led_platform_data fitt_gpio_leds_pdata = {
	.num_leds	= ARRAY_SIZE(fitt_leds),
	.leds		= fitt_leds,
};

static struct platform_device fitt_gpio_leds_dev = {
	.name = "leds-gpio",
	.id   = -1,
	.dev = {
		.platform_data 	= &fitt_gpio_leds_pdata,
	},
};

#endif

static void __init fitt_mux_init(void)
{
	omap_mux_init_signal("xref_clk1.mcasp1_ahclkx", 0);	/* MCA[1]_ahclkx */
	omap_mux_init_signal("gpmc_ben1.timer7_mux3", 0);
}

static struct platform_device *fitt_plat_devices[] __initdata = {
#ifdef CONFIG_SND_SOC_TLV320AIC3X
	&mcasp_aic_dev,
#endif
#ifdef CONFIG_I2C_GPIO
	&i2c_gpio_bus_dev,
#endif
#ifdef CONFIG_LEDS_GPIO_PLATFORM
	&fitt_gpio_leds_dev,
#endif
};

extern struct platform_device **omap_pwm_devices;

static void __init fitt_init(void)
{
	ti814x_mux_init(board_mux);
	omap_serial_init();
	omap2_hsmmc_init(mmc);

	fitt_mux_init();
	fitt_i2c_init();

	/* nand initialisation */
	board_nand_init(fitt_nand_partitions,
				ARRAY_SIZE(fitt_nand_partitions), 0, 0);

	/* initialize usb */
	usb_musb_init(&musb_board_data);
	platform_add_devices(fitt_plat_devices, ARRAY_SIZE(fitt_plat_devices));

	omap_register_pwm_config(fitt_pwm_cfg, ARRAY_SIZE(fitt_pwm_cfg));
	backlight_dev.dev.parent = &omap_pwm_devices[0]->dev;
	platform_device_register(&backlight_dev);

	regulator_use_dummy_regulator();
	printk(KERN_INFO "FITT Board Ver 0x%X\n", board_rev());
}

static int __init fitt_gpio_init(void)
{
	gpio_request(PWR_HOLD, "pwr_hold");
	gpio_direction_input(PWR_HOLD);		//# for control by mcu

	return 0;
}
/* GPIO setup should be as subsys_initcall() as gpio driver
 * is registered in arch_initcall() */
subsys_initcall(fitt_gpio_init);

static void __init fitt_map_io(void)
{
	omap2_set_globals_ti816x();
	ti81xx_map_common_io();
}

MACHINE_START(DM385EVM, "fitt")
	/* Maintainer: Texas Instruments */
	.boot_params	= 0x80000100,
	.map_io			= fitt_map_io,
	.reserve		= ti81xx_reserve,
	.init_irq		= fitt_init_irq,
	.init_machine	= fitt_init,
	.timer			= &omap_timer,
MACHINE_END
