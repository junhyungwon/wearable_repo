/*
 * File : dev_gpio.c
 *
 * Copyright (C) 2014 UDWORKs
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * this implements a GPIO hardware library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <dev_common.h>
#include <dev_gpio.h>

#define ARCH_NR_GPIOs		(256)
#define SZ_BUF				64

#define GPIO_BASE_PATH 		"/sys/class/gpio"
#define GPIO_PATH_MAX		64

struct gpio_desc_t {
	int fd;         /* file descriptor for /gpio/valude */
	int dir;		/* direction of gpio */
};

static struct gpio_desc_t desc[ARCH_NR_GPIOs];

/*
 * note: file descriptor 0(stdin), 1(stdout), 2(stderr)
 * so we assumed zero fd -> invalid fd
 */
/*************************************************************
 * NAME : int gpio_init(struct gpio_dev_t *gpio_dev, int value)
 ************************************************************/
int gpio_input_init(int num)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	char buf[32 + 1] = {};

	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* export gpio */
	fd = open(GPIO_BASE_PATH "/export", O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	/* direction input */
	gpio->dir = GPIO_INPUT;
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/direction", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	snprintf(buf, sizeof(buf), "in");
	write(fd, buf, strlen(buf));
	close(fd);

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", num);
	fd = open(path, O_RDWR); /* For (rw) O_RDWR: (r)O_RDONLY, (w)O_WRONLY */
	if (fd < 0) {
		dev_err("couldn't open %s (ret = %d)\n", path, fd);
		return -1;
	}
	gpio->fd = fd;

	/* dummy read : flush gpio */
	read(fd, buf, sizeof(buf) - 1);
	lseek(fd, 0, SEEK_SET);

	return 0;
}

/*************************************************************
 * NAME : int gpio_output_init(int num, int default_val)
 ************************************************************/
int gpio_output_init(int num, int default_val)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	char buf[32 + 1] = {};

	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* export gpio */
	fd = open(GPIO_BASE_PATH "/export", O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	/* direction output (default value) */
	gpio->dir = GPIO_OUTPUT;
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/direction", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	if (default_val)
		snprintf(buf, sizeof(buf), "high");
	else
		snprintf(buf, sizeof(buf), "out");

	write(fd, buf, strlen(buf));
	close(fd);

	/* open gpio value (default : O_WRONLY, output value) */
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s (ret = %d)\n", path, fd);
		return fd;
	}
	gpio->fd = fd;

	return 0;
}

/*************************************************************
 * NAME : int gpio_irq_init(int num, int irq_mode)
 ************************************************************/
int gpio_irq_init(int num, int irq_mode)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	char buf[32 + 1] = {};

	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* export gpio */
	fd = open(GPIO_BASE_PATH "/export", O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/direction", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	/* direction input */
	gpio->dir = GPIO_INPUT;
	snprintf(buf, sizeof(buf), "in");
	write(fd, buf, strlen(buf));
	close(fd);

	/* open irq flag */
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/edge", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	if (irq_mode == GPIO_IRQ_RISING)
		strcpy(buf, "rising");
	else if (irq_mode == GPIO_IRQ_FALLING)
		strcpy(buf, "falling");
	else
		strcpy(buf, "both");

	write(fd, buf, strlen(buf) + 1); //# add '\0'
	close(fd);

	/* open gpio value */
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", num);
	fd = open(path, O_RDWR); /* For rw */
	if (fd < 0) {
		dev_err("couldn't open %s (ret = %d)\n", path, fd);
		return fd;
	}
	gpio->fd = fd;

	/* dummy read : flush gpio */
	read(fd, buf, sizeof(buf) - 1);
	lseek(fd, 0, SEEK_SET);

	return 0;
}

/*************************************************************
 * NAME : int gpio_exit(int num)
 ************************************************************/
int gpio_exit(int num)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	if (gpio->fd > 0) {
		close(gpio->fd);
		gpio->fd = -1;
	}

	/* unexport gpio */
	fd = open(GPIO_BASE_PATH "/unexport", O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return fd;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	return 0;
}

/*************************************************************
 * NAME : int gpio_set_value(int num, int value)
 ************************************************************/
int gpio_set_value(int num, int value)
{
	struct gpio_desc_t *gpio;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* sanity check */
	if (gpio->dir == GPIO_INPUT) {
		dev_err("gpio(%d) is not configure output mode!!\n", num);
		return -1;
	}

	if (gpio->fd > 0) {
		lseek(gpio->fd, 0, SEEK_SET);
		if (value)
			write(gpio->fd, "1", 2); //# add '\0'
		else
			write(gpio->fd, "0", 2); //# add '\0'
	} else {
		dev_err("gpio(%d) is required initialization!!\n", num);
		return -1;
	}

	return 0;
}

/*************************************************************
 * NAME : int gpio_get_value(int num, int *val)
 ************************************************************/
int gpio_get_value(int num, int *val)
{
	struct gpio_desc_t *gpio;

	char buf[SZ_BUF + 1] = {};
	int res;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* sanity check */
	if (gpio->dir == GPIO_OUTPUT) {
		dev_err("gpio(%d) is not configure input mode!!\n", num);
		return -1;
	}

	if (gpio->fd > 0) {
		lseek(gpio->fd, 0, SEEK_SET);
		res = read(gpio->fd, buf, SZ_BUF);
		if (res == -1) {
			dev_err("Failed to read gpio[%d]\n", num);
			return -1; //# Failed to reading
		}
		/* return value: gpio + '\n' = 2byte */
		*val = strtol(buf, NULL, 10); //# atoi
	} else {
		*val = 0;
		dev_err("gpio(%d) is required initialization!!\n", num);
		return -1;
	}

	return 0;
}

/*************************************************************
 * NAME : int gpio_set_active_low(int num, int active_low)
 ************************************************************/
int gpio_set_active_low(int num, int active_low)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	int fd;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* set active mode. */
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/active_low", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		dev_err("couldn't open %s\n", path);
		return -1;
	}

	if (active_low)
		write(fd, "1", 2);
	close(fd);

	return 0;
}

/*************************************************************
 * NAME : int gpio_irq_read(int num, int *value, int timeout)
 ************************************************************/
int gpio_irq_read(int num, int *value, int timeout)
{
	struct gpio_desc_t *gpio;

	char buf[SZ_BUF + 1];
	struct pollfd fdset;

	int res, fd;
	int exit = 0;

	if (num >= ARCH_NR_GPIOs) {
		dev_err("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	if ((gpio->dir == GPIO_OUTPUT) || (gpio->fd < 0)) {
		dev_err("gpio(%d) is not configure input mode!!\n", num);
		return -1;
	}

	fd = gpio->fd;
	/* clear event */
	memset((void*)&fdset, 0, sizeof(fdset));

	fdset.fd     = fd;
	fdset.events = POLLPRI;

	while (!exit)
	{
		res = poll(&fdset, 1, timeout);
		if ((res <= 0)) {
			//dev_err("[GPIO] generated poll error!..exit\n");
			return -1;
		}

		/* check return event */
		if (fdset.revents & POLLPRI) {
			/* wait for 1 event */
			lseek(fd, 0, SEEK_SET);
			res = read(fd, buf, SZ_BUF);
			if (res <= 0)
				return -1; //# Failed to reading
			/*
			 * return value: gpio + '\n' = 2byte
			 */
			*value = strtol(buf, NULL, 10); //# atoi
			return 0;
		}
	}

	return (-1);
}
