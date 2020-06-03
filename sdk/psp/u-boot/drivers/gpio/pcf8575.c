/*
 * PCF8575 I2C GPIO EXPANDER DRIVER
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
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

/*
 * Driver for TI pcf-8575 16 bit I2C gpio expander. Based on
 * gpio-pcf857x Linux 4.0 kernel driver and pca953x driver in u-boot
 */

#include <common.h>
#include <asm/gpio.h>
#include <pcf8575.h>

DECLARE_GLOBAL_DATA_PTR;

/*-----------------------------------------------------------------------
 * Definitions
 */
#define PCF8575_SDA_PIN		36 //# GP1[4]
#define PCF8575_SCL_PIN		69 //# GP2[5]

#define RETRIES		0
#define I2C_ACK		0		/* PD_SDA level to ack a byte */
#define I2C_NOACK	1		/* PD_SDA level to noack a byte */

#ifdef DEBUG_I2C
#define PRINTD(fmt,args...)	do {	\
	if (gd->have_console)		\
		printf (fmt ,##args);	\
	} while (0)
#else
#define PRINTD(fmt,args...)
#endif

enum {
	PCF8575_CMD_INFO,
	PCF8575_CMD_DEVICE,
	PCF8575_CMD_OUTPUT,
	PCF8575_CMD_INPUT,
};

struct pcf8575_chip {
	uint8_t addr;
/* current direction of the pcf lines */
	unsigned int out;
};

/* NOTE:  these chips have strange "quasi-bidirectional" I/O pins.
 * We can't actually know whether a pin is configured (a) as output
 * and driving the signal low, or (b) as input and reporting a low
 * value ... without knowing the last value written since the chip
 * came out of reset (if any).  We can't read the latched output.
 * In short, the only reliable solution for setting up pin direction
 * is to do it explicitly.
 *
 * Using "out" avoids that trouble. It flags the status of the pins at
 * boot.
 *
 * Each struct stores address of an instance of pcf
 * and state(direction) of each gpio line for that instance.
 */
static struct pcf8575_chip pcf8575_chips[] = {
	{.addr = CONFIG_PCF8575_CHIP_ADDR},
};

//#------------------------------------ gpio i2c ------------------------------------------
static void i2c_gpio_set_sda(int state)
{
	gpio_direction_output(PCF8575_SDA_PIN, state);
}

static void i2c_gpio_set_scl(int state)
{
	gpio_direction_output(PCF8575_SCL_PIN, state);
}

static int i2c_gpio_get_sda(void)
{
	gpio_direction_input(PCF8575_SDA_PIN);

	return (gpio_get_value(PCF8575_SDA_PIN) ? 1 : 0);
}

static void i2c_gpio_set_sda_tristate(void)
{
	gpio_direction_input(PCF8575_SDA_PIN);
}

/*-----------------------------------------------------------------------
 * START: High -> Low on SDA while SCL is High
 */
static void i2c_gpio_send_start(void)
{
	udelay(3);
	i2c_gpio_set_sda(1);
	udelay(3);
	i2c_gpio_set_scl(1);
	udelay(3);
	i2c_gpio_set_sda(0);
	udelay(3);
}

/*-----------------------------------------------------------------------
 * STOP: Low -> High on SDA while SCL is High
 */
static void i2c_gpio_send_stop(void)
{
	i2c_gpio_set_scl(0);
	udelay(3);
	i2c_gpio_set_sda(0);
	udelay(3);
	i2c_gpio_set_scl(1);
	udelay(3);
	i2c_gpio_set_sda(1);
	udelay(3);
}

/*-----------------------------------------------------------------------
 * Send a reset sequence consisting of 9 clocks with the data signal high
 * to clock any confused device back into an idle state.  Also send a
 * <stop> at the end of the sequence for belts & suspenders.
 */
static void i2c_gpio_send_reset(void)
{
	int j;

	i2c_gpio_set_scl(1);
	i2c_gpio_set_sda(1);

	for (j = 0; j < 9; j++) {
		i2c_gpio_set_scl(0);
		udelay(3); /* 1/4 I2C clock duration */
		udelay(3); /* 1/4 I2C clock duration */
		i2c_gpio_set_scl(1);
		udelay(3);
		udelay(3);
	}
	i2c_gpio_send_stop();
}

/*-----------------------------------------------------------------------
 * ack should be I2C_ACK or I2C_NOACK
 */
static void i2c_gpio_send_ack(int ack)
{
	i2c_gpio_set_scl(0);
	udelay(3);
	i2c_gpio_set_sda(ack);
	udelay(3);
	i2c_gpio_set_scl(1);
	udelay(3);
	udelay(3);
	i2c_gpio_set_scl(0);
	udelay(3);
}

/*-----------------------------------------------------------------------
 * Send 8 bits and look for an acknowledgement.
 */
static int write_byte(uchar data)
{
	int j;
	int nack;

	for (j = 0; j < 8; j++) {
		i2c_gpio_set_scl(0);
		udelay(3);
		i2c_gpio_set_sda(data & 0x80);
		udelay(3);
		i2c_gpio_set_scl(1);
		udelay(3);
		udelay(3);

		data <<= 1;
	}

	/*
	 * Look for an <ACK>(negative logic) and return it.
	 */
	i2c_gpio_set_scl(0);
	udelay(3);
	i2c_gpio_set_sda(1);
	i2c_gpio_set_sda_tristate();
	udelay(3);
	i2c_gpio_set_scl(1);
	udelay(3);
	udelay(3);
	nack = i2c_gpio_get_sda();
	i2c_gpio_set_scl(0);
	udelay(3);

	return(nack);	/* not a nack is an ack */
}

/*-----------------------------------------------------------------------
 * if ack == I2C_ACK, ACK the byte so can continue reading, else
 * send I2C_NOACK to end the read.
 */
static uchar read_byte(int ack)
{
	int data;
	int j;

	/*
	 * Read 8 bits, MSB first.
	 */
	i2c_gpio_set_sda_tristate();
	i2c_gpio_set_sda(1);
	data = 0;
	for(j = 0; j < 8; j++) {
		i2c_gpio_set_scl(0);
		udelay(3);
		i2c_gpio_set_scl(1);
		udelay(3);
		data <<= 1;
		data |= i2c_gpio_get_sda();
		udelay(3);
	}
	i2c_gpio_send_ack(ack);

	return(data);
}

/*-----------------------------------------------------------------------
 * Read bytes
 */
static int i2c_gpio_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int shift;

	PRINTD("i2c_gpio_read: chip %02X addr %02X alen %d buffer %p len %d\n",
		chip, addr, alen, buffer, len);

	/*
	 * Do the addressing portion of a write cycle to set the
	 * chip's address pointer.  If the address length is zero,
	 * don't do the normal write cycle to set the address pointer,
	 * there is no address pointer in this chip.
	 */
	i2c_gpio_send_start();
	if (alen > 0) {
		if (write_byte(chip << 1)) {	/* write cycle */
			i2c_gpio_send_stop();
			PRINTD("i2c_read, no chip responded %02X\n", chip);
			return(1);
		}

		shift = (alen-1) * 8;
		while (alen-- > 0) {
			if (write_byte(addr >> shift)) {
				PRINTD("i2c_read, address not <ACK>ed\n");
				return(1);
			}
			shift -= 8;
		}

		/* Some I2C chips need a stop/start sequence here,
		 * other chips don't work with a full stop and need
		 * only a start.  Default behaviour is to send the
		 * stop/start sequence.
		 */
#if 0 //# for repeated start.
		send_start();
#else
		i2c_gpio_send_stop();
		i2c_gpio_send_start();
#endif
	}
	/*
	 * Send the chip address again, this time for a read cycle.
	 * Then read the data.  On the last byte, we do a NACK instead
	 * of an ACK(len == 0) to terminate the read.
	 */
	write_byte((chip << 1) | 1);	/* read cycle */
	while(len-- > 0) {
		*buffer++ = read_byte(len == 0);
	}
	i2c_gpio_send_stop();
	return(0);
}

/*-----------------------------------------------------------------------
 * Write bytes
 */
static int i2c_gpio_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int shift, failures = 0;

	PRINTD("i2c_write: chip %02X addr %02X alen %d buffer %p len %d\n",
		chip, addr, alen, buffer, len);

	i2c_gpio_send_start();
	if (write_byte(chip << 1)) {	/* write cycle */
		i2c_gpio_send_stop();
		PRINTD("i2c_write, no chip responded %02X\n", chip);
		return(1);
	}

	shift = (alen-1) * 8;
	while (alen-- > 0) {
		if (write_byte(addr >> shift)) {
			PRINTD("i2c_write, address not <ACK>ed\n");
			return(1);
		}
		shift -= 8;
	}

	while (len-- > 0) {
		if (write_byte(*buffer++)) {
			failures++;
		}
	}

	i2c_gpio_send_stop();
	return(failures);
}
//#----------------------------------- end of gpio i2c

static struct pcf8575_chip *pcf8575_chip_get(uint8_t addr)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcf8575_chips); i++)
		if (pcf8575_chips[i].addr == addr)
			return &pcf8575_chips[i];

	return 0;
}

/* Read/Write to 16-bit I/O expander */
static int pcf8575_i2c_write(uint8_t addr, unsigned word)
{
	unsigned word_be = ((word & 0xff) << 8) |
			   ((word & 0xff00) >> 8);
	uint8_t buf = 0;
	int status;

	status = i2c_gpio_write(addr, word_be, 2, &buf, 1);

	return (status < 0) ? status : 0;
}

static int pcf8575_i2c_read(uint8_t addr)
{
	u8 buf[2];
	int status;

	status = i2c_gpio_read(addr, 0, 1, buf, 2);
	if (status < 0)
		return status;

	return (buf[1] << 8) | buf[0];
}

void pcf8575_init(void)
{
	i2c_gpio_send_reset();
}

int pcf8575_input(uint8_t addr, unsigned offset)
{
	struct pcf8575_chip *chip = pcf8575_chip_get(addr);
	int status;

	chip->out |= (1 << offset);
	status = pcf8575_i2c_write(addr, chip->out);

	return status;
}

int pcf8575_get_val(uint8_t addr, unsigned offset)
{
	int value;

	value = pcf8575_i2c_read(addr);
	return (value < 0) ? 0 : (value & (1 << offset));
}

int pcf8575_output(uint8_t addr, unsigned offset, int value)
{
	struct pcf8575_chip *chip = pcf8575_chip_get(addr);
	unsigned bit = 1 << offset;
	int             status;

	if (value)
		chip->out |= bit;
	else
		chip->out &= ~bit;

	status = pcf8575_i2c_write(addr, chip->out);

	return status;
}

/*
 * Display pcf857x information
 */
int pcf8575_info(uint8_t addr)
{
	int i;
	uint data;
	struct pcf8575_chip *chip = pcf8575_chip_get(addr);
	int nr_gpio = 16;
	int msb = nr_gpio - 1;

	printf("pcf857x@ 0x%x (%d pins):\n\n", addr, nr_gpio);
	printf("gpio pins: ");
	for (i = msb; i >= 0; i--)
		printf("%x", i);
	printf("\n");
	for (i = 11 + nr_gpio; i > 0; i--)
		printf("-");
	printf("\n");

	data = chip->out;
	printf("dir:      ");
	for (i = msb; i >= 0; i--)
		printf("%c", data & (1 << i) ? 'i' : 'o');
	printf("\n");

	data = pcf8575_i2c_read(addr);
	if (data < 0)
		return -1;
	printf("input:     ");
	for (i = msb; i >= 0; i--)
		printf("%c", data & (1 << i) ? '1' : '0');
	printf("\n");

	return 0;
}

cmd_tbl_t cmd_pcf8575[] = {
	U_BOOT_CMD_MKENT(device, 3, 0, (void *)PCF8575_CMD_DEVICE, "", ""),
	U_BOOT_CMD_MKENT(output, 4, 0, (void *)PCF8575_CMD_OUTPUT, "", ""),
	U_BOOT_CMD_MKENT(input, 3, 0, (void *)PCF8575_CMD_INPUT, "", ""),
	U_BOOT_CMD_MKENT(info, 2, 0, (void *)PCF8575_CMD_INFO, "", ""),
};

int do_pcf8575(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static uint8_t chip = CONFIG_PCF8575_CHIP_ADDR;
	int val;
	ulong ul_arg2 = 0;
	ulong ul_arg3 = 0;
	cmd_tbl_t *c;

	c = find_cmd_tbl(argv[1], cmd_pcf8575, ARRAY_SIZE(cmd_pcf8575));

	/* All commands but "device" require 'maxargs' arguments */
	if (!c || !((argc == (c->maxargs)) ||
		    (((int)c->cmd == PCF8575_CMD_DEVICE) &&
		    (argc == (c->maxargs - 1))))) {
		cmd_usage(cmdtp);
		return 1;
	}

	/* arg2 used as chip number or pin number */
	if (argc > 2)
		ul_arg2 = simple_strtoul(argv[2], NULL, 16);

	/* arg3 used as pin or invert value */
	if (argc > 3)
		ul_arg3 = simple_strtoul(argv[3], NULL, 16) & 0x1;

	switch ((int)c->cmd) {
	case PCF8575_CMD_INFO:
		return pcf8575_info(chip);

	case PCF8575_CMD_DEVICE:
		if (argc == 3)
			chip = (uint8_t)ul_arg2;
		printf("Current device address: 0x%x\n", chip);
		return 0;

	case PCF8575_CMD_INPUT:
		pcf8575_input(chip, ul_arg2);
		val = pcf8575_get_val(chip, ul_arg2);

		printf("chip 0x%02x, pin 0x%lx = %d\n", chip, ul_arg2,
			       val);
		return val;

	case PCF8575_CMD_OUTPUT:
		pcf8575_output(chip, ul_arg2, ul_arg3);
		return 0;
	}

	return 1;
}

U_BOOT_CMD(
	pcf8575,	5,	1,	do_pcf8575,
	"pcf8575 gpio access",
	"device [dev]\n"
	"	- show or set current device address\n"
	"pcf8575 info\n"
	"	- display info for current chip\n"
	"pcf8575 output pin 0|1\n"
	"	- set pin as output and drive low or high\n"
	"pcf8575 input pin\n"
	"	- set pin as input and read value"
);
