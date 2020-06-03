/* drivers/video/backlight/vgg2432a4.c
 *
 * VGG2432A4 (ILI9320) LCD controller driver.
 *
 * Copyright 2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/lcd.h>
#include <linux/module.h>

#include <linux/spi/spi.h>

#include <video/ili9325.h>

struct ili9325_reg {
	unsigned short		address;
	unsigned short		value;
};

struct ili9325;

struct ili9325_client {
	const char	*name;
	int	(*init)(struct ili9325 *ili, struct ili9325_platdata *cfg);

};

struct ili9325_spi {
	struct spi_device	*dev;
	struct spi_message	message;
	struct spi_transfer	xfer[2];

	unsigned char id;
	unsigned char buffer_addr[4];
	unsigned char buffer_data[4];
};

struct ili9325 {
	union {
		struct ili9325_spi	spi;	/* SPI attachged device. */
	} access;				/* Register access method. */

	struct device			*dev;
	struct lcd_device		*lcd;	/* LCD device we created. */
	struct ili9325_client		*client;
	struct ili9325_platdata		*platdata;

	int	 power; /* current power state. */
	int	 initialised;

	unsigned short	display1;
	unsigned short	power1;

	int (*write)(struct ili9325 *ili, unsigned int reg, unsigned int val);
};

static struct ili9325_reg lcd_default1[] = {
	{
		.address = ILI9325_POWER1,
		.value	 = ILI9325_POWER1_AP(0) | ILI9325_POWER1_BT(0),
	}, {
		.address = ILI9325_POWER2,
		.value	 = (ILI9325_POWER2_VC(5) |
			    ILI9325_POWER2_DC0(0) | ILI9325_POWER2_DC1(0)),
	}, {
		.address = ILI9325_POWER3,
		.value	 = ILI9325_POWER3_VRH(0),
	}, {
		.address = ILI9325_POWER4,
		.value	 = ILI9325_POWER4_VREOUT(0),
	}, 
};

static struct ili9325_reg lcd_gamma[] = {
	{
		.address = ILI9325_GAMMA1,
		.value	 = 0x0205,
	}, {
		.address = ILI9325_GAMMA2,
		.value   = 0x0606,
	}, {
		.address = ILI9325_GAMMA3,
		.value	 = 0x0705,
	}, {
		.address = ILI9325_GAMMA4,
		.value	 = 0x0205,
	}, {
		.address = ILI9325_GAMMA5,
		.value	 = 0x000F,
	}, {
		.address = ILI9325_GAMMA6,
		.value	 = 0x0104,
	}, {
		.address = ILI9325_GAMMA7,
		.value	 = 0x0206,
	}, {
		.address = ILI9325_GAMMA8,
		.value	 = 0x0507,
	}, {
		.address = ILI9325_GAMMA9,
		.value	 = 0x0701,
	}, {
		.address = ILI9325_GAMMA10,
		.value	 = 0x1610,
	}
};

static inline int ili9325_write_spi(struct ili9325 *ili,
				    unsigned int reg, unsigned int value)
{
	struct ili9325_spi *spi = &ili->access.spi;
	
	unsigned char *addr = spi->buffer_addr;
	unsigned char *data = spi->buffer_data;

	/* spi message consits of:
	 * first byte: ID and operation
	 */
	addr[0] = spi->id | ILI9325_SPI_INDEX | ILI9325_SPI_WRITE;
	addr[1] = reg >> 8;
	addr[2] = reg;

	/* second message is the data to transfer */
	data[0] = spi->id | ILI9325_SPI_DATA  | ILI9325_SPI_WRITE;
 	data[1] = value >> 8;
	data[2] = value;

	return spi_sync(spi->dev, &spi->message);
}

static int ili9325_write(struct ili9325 *ili, unsigned int reg, unsigned int value)
{
	dev_dbg(ili->dev, "write: reg=%02x, val=%04x\n", reg, value);
	return ili->write(ili, reg, value);
}

static int ili9325_write_regs(struct ili9325 *ili,
		       struct ili9325_reg *values, int nr_values)
{
	int index;
	int ret;

	for (index = 0; index < nr_values; index++, values++) {
		ret = ili9325_write(ili, values->address, values->value);
		if (ret != 0)
			return ret;
	}

	return 0;
}

static inline int ili9325_init_chip(struct ili9325 *lcd)
{
	struct ili9325_platdata *cfg = lcd->platdata;
	int ret;

	/* lcd reset */
	cfg->reset(1);
	mdelay(10);

	cfg->reset(0);
	mdelay(10);

	cfg->reset(1);
	mdelay(50);
	
	ret = lcd->client->init(lcd, lcd->platdata);
	if (ret != 0) {
		dev_err(lcd->dev, "failed to initialise display\n");
		return ret;
	}

	lcd->initialised = 1;
	
	dev_info(lcd->dev, "initialise display done!!\n");
	
	return 0;
}

static inline int ili9325_power_on(struct ili9325 *lcd)
{
	if (!lcd->initialised)
		ili9325_init_chip(lcd);

	lcd->display1 |= (ILI9325_DISPLAY1_D(3) | ILI9325_DISPLAY1_BASEE);
	ili9325_write(lcd, ILI9325_DISPLAY1, lcd->display1);

	return 0;
}

static inline int ili9325_power_off(struct ili9325 *lcd)
{
	lcd->display1 &= ~(ILI9325_DISPLAY1_D(3) | ILI9325_DISPLAY1_BASEE);
	ili9325_write(lcd, ILI9325_DISPLAY1, lcd->display1);

	return 0;
}

#define POWER_IS_ON(pwr)	((pwr) <= FB_BLANK_NORMAL)

static int ili9325_power(struct ili9325 *lcd, int power)
{
	int ret = 0;

	dev_info(lcd->dev, "power %d => %d\n", lcd->power, power);

	if (POWER_IS_ON(power) && !POWER_IS_ON(lcd->power))
		ret = ili9325_power_on(lcd);
	else if (!POWER_IS_ON(power) && POWER_IS_ON(lcd->power))
		ret = ili9325_power_off(lcd);

	if (ret == 0)
		lcd->power = power;
	else
		dev_warn(lcd->dev, "failed to set power mode %d\n", power);

	return ret;
}

static inline struct ili9325 *to_our_lcd(struct lcd_device *lcd)
{
	return lcd_get_data(lcd);
}

static int ili9325_set_power(struct lcd_device *ld, int power)
{
	struct ili9325 *lcd = to_our_lcd(ld);

	return ili9325_power(lcd, power);
}

static int ili9325_get_power(struct lcd_device *ld)
{
	struct ili9325 *lcd = to_our_lcd(ld);

	return lcd->power;
}

static int ili9325_lcd_init(struct ili9325 *lcd,
			      struct ili9325_platdata *cfg)
{
	unsigned int addr;
	int ret;
	
	//# Start Initial Sequence (DOTCLK 5MHz)
	ili9325_write(lcd, 0xE5, 0x78F0);
	/* driver output control */
	ili9325_write(lcd, ILI9325_DRIVER, ILI9325_DRIVER_SS);
	/* LCD Line Inversion */
	ili9325_write(lcd, ILI9325_DRIVEWAVE, ILI9325_DRIVEWAVE_BC);
	/* Entry Mode */
	ili9325_write(lcd, ILI9325_ENTRYMODE, cfg->entry_mode);
	/* DISPLAY CONTROL2 */
	ili9325_write(lcd, ILI9325_DISPLAY2, cfg->display2);
	/* DISPLAY CONTROL3 */
	ili9325_write(lcd, ILI9325_DISPLAY3, cfg->display3);
	/* DISPLAY CONTROL4 */
	ili9325_write(lcd, ILI9325_DISPLAY4, cfg->display4);
	/* RGB Display Interface Control1 */
	ili9325_write(lcd, ILI9325_RGB_IF1, cfg->rgb_if1);
	/* Frame Marker Position */
	ili9325_write(lcd, ILI9325_FRAMEMAKER, 0x0);
	ili9325_write(lcd, ILI9325_RGB_IF2, cfg->rgb_if2);
	
	/* power control */
	ret = ili9325_write_regs(lcd, lcd_default1, ARRAY_SIZE(lcd_default1));
	if (ret != 0)
		goto err_default1;
	mdelay(20);
	/* VCOM GND, Internal On */
	ili9325_write(lcd, ILI9325_DISPLAY1, ILI9325_DISPLAY1_D(1));
	
	/* Power Control */
	ili9325_write(lcd, ILI9325_POWER1, 0x1790);
	ili9325_write(lcd, ILI9325_POWER2, 0x0227);
	mdelay(5);

	ili9325_write(lcd, ILI9325_POWER3, 0x008A);
	mdelay(5);

	ili9325_write(lcd, ILI9325_POWER4, 0x1E00);
	ili9325_write(lcd, ILI9325_POWER7, 0x0022);
	ili9325_write(lcd, ILI9325_FRAME_RATE_COLOUR, 0x000C);
	mdelay(5);

	ili9325_write(lcd, ILI9325_GRAM_HORIZ_ADDR, 0x00);
	ili9325_write(lcd, ILI9325_GRAM_VERT_ADD, 0x00);

	ret = ili9325_write_regs(lcd, lcd_gamma, ARRAY_SIZE(lcd_gamma));
	if (ret != 0)
		goto err_default2;

	ili9325_write(lcd, ILI9325_HORIZ_START, 0x0);
	ili9325_write(lcd, ILI9325_HORIZ_END, cfg->hsize - 1);
	ili9325_write(lcd, ILI9325_VERT_START, 0x0);
	ili9325_write(lcd, ILI9325_VERT_END, cfg->vsize - 1);

	ili9325_write(lcd, ILI9325_DRIVER2, ILI9325_DRIVER2_GS |
		      ILI9325_DRIVER2_NL(((cfg->vsize - 240) / 8) + 0x1D));

	ili9325_write(lcd, ILI9325_BASE_IMAGE, 0x1);
	ili9325_write(lcd, ILI9325_VERT_SCROLL, 0x00);

	for (addr = ILI9325_PARTIAL1_POSITION; addr <= ILI9325_PARTIAL2_END;
	     addr++) {
		ili9325_write(lcd, addr, 0x0);
	}
	
	/* PANEL INTERFACE CONTROL1 */
	ili9325_write(lcd, ILI9325_INTERFACE1, 0x10);
	ili9325_write(lcd, ILI9325_INTERFACE2, cfg->interface2);
	ili9325_write(lcd, ILI9325_INTERFACE4, cfg->interface4);
	mdelay(5);
	
	/* DISPLAY ON */
	lcd->display1 = (ILI9325_DISPLAY1_D(3) | ILI9325_DISPLAY1_DTE |
			 ILI9325_DISPLAY1_GON | ILI9325_DISPLAY1_BASEE);
	ili9325_write(lcd, ILI9325_DISPLAY1, lcd->display1);
	
	/* WRITE DATA TO GRAM */
	ili9325_write(lcd, 0x22, 0x0000);
	mdelay(50);
	return 0;

 err_default2:
 err_default1:
 	
 	dev_err(lcd->dev, "failed to write lcd register\n");
	return ret;
}

static struct lcd_ops ili9325_ops = {
	.get_power	= ili9325_get_power,
	.set_power	= ili9325_set_power,
};

static void __devinit ili9325_setup_spi(struct ili9325 *ili,
					struct spi_device *dev)
{
	struct ili9325_spi *spi = &ili->access.spi;

	ili->write = ili9325_write_spi;
	spi->dev = dev;

	/* fill the two messages we are going to use to send the data
	 * with, the first the address followed by the data. The datasheet
	 * says they should be done as two distinct cycles of the SPI CS line.
	 */
	spi->xfer[0].tx_buf = spi->buffer_addr;
	spi->xfer[1].tx_buf = spi->buffer_data;
	spi->xfer[0].len = 3;
	spi->xfer[1].len = 3;
	spi->xfer[0].bits_per_word = 8;
	spi->xfer[1].bits_per_word = 8;
	spi->xfer[0].cs_change = 1;

	spi_message_init(&spi->message);
	spi_message_add_tail(&spi->xfer[0], &spi->message);
	spi_message_add_tail(&spi->xfer[1], &spi->message);
}

static struct ili9325_client lcd_client = {
	.name	= "ILI9325D",
	.init	= ili9325_lcd_init,
};

static int __devinit ili9325_probe(struct spi_device *spi)
{
	struct ili9325_platdata *cfg = spi->dev.platform_data;
	struct device *dev = &spi->dev;
	struct ili9325 *ili;
	struct lcd_device *lcd;
	int ret = 0;
	
	/* verify we where given some information */
	if (cfg == NULL) {
		dev_err(dev, "no platform data supplied\n");
		return -EINVAL;
	}

	if (cfg->hsize <= 0 || cfg->vsize <= 0 || cfg->reset == NULL) {
		dev_err(dev, "invalid platform data supplied\n");
		return -EINVAL;
	}

	/* allocate and initialse our state */
	ili = kzalloc(sizeof(struct ili9325), GFP_KERNEL);
	if (ili == NULL) {
		dev_err(dev, "no memory for device\n");
		return -ENOMEM;
	}

	ili->access.spi.id = ILI9325_SPI_IDCODE | ILI9325_SPI_ID(0);

	ili->dev = dev;
	ili->client = &lcd_client;
	ili->power = FB_BLANK_POWERDOWN;
	ili->platdata = cfg;

	dev_set_drvdata(&spi->dev, ili);

	ili9325_setup_spi(ili, spi);

	lcd = lcd_device_register("ili9325", dev, ili, &ili9325_ops);
	if (IS_ERR(lcd)) {
		dev_err(dev, "failed to register lcd device\n");
		ret = PTR_ERR(lcd);
		goto err_free;
	}

	ili->lcd = lcd;

	dev_info(dev, "initialising %s\n", lcd_client.name);

	ret = ili9325_power(ili, FB_BLANK_UNBLANK);
	if (ret != 0) {
		dev_err(dev, "failed to set lcd power state\n");
		goto err_unregister;
	}

	return 0;

 err_unregister:
	lcd_device_unregister(lcd);

 err_free:
	kfree(ili);

	return ret;
}

static int __devexit ili9325_remove(struct spi_device *spi)
{
	struct ili9325 *ili9325_data = dev_get_drvdata(&spi->dev);
	
	ili9325_power(ili9325_data, FB_BLANK_POWERDOWN);

	lcd_device_unregister(ili9325_data->lcd);
	kfree(ili9325_data);

	return 0;
}

static struct spi_driver ili9325_driver = {
	.driver = {
		.name		= "ili9325",
		.owner		= THIS_MODULE,
	},
	.probe		= ili9325_probe,
	.remove		= __devexit_p(ili9325_remove),
};

/* Device driver initialisation */
static int __init ili9325_init(void)
{
	return spi_register_driver(&ili9325_driver);
}

static void __exit ili9325_exit(void)
{
	spi_unregister_driver(&ili9325_driver);
}

module_init(ili9325_init);
module_exit(ili9325_exit);

MODULE_AUTHOR("UDWORKs");
MODULE_DESCRIPTION("ILI9325D LCD Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:ili9325");
