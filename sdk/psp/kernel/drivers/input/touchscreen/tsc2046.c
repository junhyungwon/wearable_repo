/*
 * TSC2046 Touchscreen driver
 *
 * Author: Kevin Hilman, MontaVista Software, Inc. <source@xxxxxxxxxx>
 *
 * Communication details from original TI driver
 * Copyright (C) 2004-2005 Texas Instruments, Inc.
 *
 * Structure based heavily on TSC2301 driver
 * Copyright (C) 2005-2006 Nokia Corporation
 *
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/gpio.h>

#include <linux/input/tsc2046.h>

/* TSC2046 commands */
#define START_BYTE		0x8000
#define X_CMDWD			0xd300
#define Y_CMDWD			0x9300
#define Z1_CMDWD		0xb300
#define Z2_CMDWD		0xc300

#define TSC2046_TS_SCAN_TIME	1
#define MAX_12BIT		((1 << 12) - 1)

#define MIN_ABS_X    528
#define MAX_ABS_X    3760
#define MIN_ABS_Y    624
#define MAX_ABS_Y    3760

/* The TSC2046 operates at a maximum speed of 2MHz */
#define TSC2046_HZ	2000000

#define CONV_DATA(d1, d2) \
  (((d1 & 0x7f) << 5) | ((d2 >> 11) & 0x1f))

/* Convenience macros for pointercal calibration calculation */
#define PA ts->pointercal[0]
#define PB ts->pointercal[1]
#define PC ts->pointercal[2]
#define PD ts->pointercal[3]
#define PE ts->pointercal[4]
#define PF ts->pointercal[5]
#define PS ts->pointercal[6]

struct tsc2046_ts {
	struct input_dev	*idev;
	char			phys[32];
	struct timer_list	timer;
	spinlock_t		lock;

	struct spi_transfer	read_xfer[3];
	struct spi_message	read_msg;
	struct spi_message	enable_msg;
	u16                     data[5];

	u16			x;
	u16			y;
	u16			p;
	u16			old_x;
	u16			old_y;
	u16			old_pressure;
	int			sample_cnt;

	int			ignore_last : 1;
	u16			x_plate_ohm;
	int			max_pressure;
	int			touch_pressure;
	int			pressure_limit;

	int			pointercal[7];
	u16			x_res;
	u16			y_res;

	u16			irq_enabled:1;
	u16			pen_down:1;
	u16			disabled:1;
	u16			pending:1;
	u16			use_pointercal:1;

	s16			dav_gpio;
	int			(*get_pendown_state)(void);
	int			irq;
	bool		swap_xy;
};

struct tsc2046 {
	struct spi_device	*spi;
	struct tsc2046_ts	*ts;
};

static const u16 tsc2046_ts_cmd_data[] = {
	START_BYTE, X_CMDWD, Y_CMDWD, Z1_CMDWD, Z2_CMDWD,
};

static int tsc2046_device_suspended(struct device *dev)
{
	struct tsc2046 *tsc = dev_get_drvdata(dev);
	return dev->power.power_state.event != PM_EVENT_ON || tsc->ts->disabled;
}

static void tsc2046_update_pen_state(struct tsc2046 *tsc, int x, int y, int pressure)
{
	struct tsc2046_ts *ts = tsc->ts;

	if (x <= ts->old_x - 4 || x >= ts->old_x + 4 ||
	    y <= ts->old_y - 4 || y >= ts->old_y + 4 ||
	    pressure <= ts->old_pressure - 4 || pressure >= ts->old_pressure + 4)
	{
		if (pressure) {
			if (!ts->use_pointercal || (ts->use_pointercal &&
			    x >= 0 &&
			    x <= ts->x_res - 1 &&
			    y >= 0 &&
			    y <= ts->y_res - 1))
			{
				input_report_abs(ts->idev, ABS_X, x);
				input_report_abs(ts->idev, ABS_Y, y);
				input_report_abs(ts->idev, ABS_PRESSURE, 1);
				input_report_key(ts->idev, BTN_TOUCH, 1);
				ts->pen_down = 1;
			} else {
				input_report_abs(ts->idev, ABS_PRESSURE, 0);
				input_report_key(ts->idev, BTN_TOUCH, 0);
				ts->pen_down = 0;
			}
		} else {
			input_report_abs(ts->idev, ABS_PRESSURE, 0);
			input_report_key(ts->idev, BTN_TOUCH, 0);
			ts->pen_down = 0;
		}

		ts->old_x = x;
		ts->old_y = y;
		ts->old_pressure = pressure;

		input_sync(ts->idev);

#ifdef VERBOSE
		dev_dbg(&tsc->spi->dev, "x %4d y %4d p %4d\n", x, y, pressure);
#endif
	}
}

/*
 * This procedure is called by the SPI framework after the coordinates
 * have been read from TSC2046
 */
static void tsc2046_ts_rx(void *arg)
{
	struct tsc2046 *tsc = arg;
	struct tsc2046_ts *ts = tsc->ts;
	unsigned int x, y, z1, z2, pressure;
	int xN, yN;

	x  = CONV_DATA(ts->data[2], ts->data[3]);
	y  = CONV_DATA(ts->data[1], ts->data[2]);
	z1 = CONV_DATA(ts->data[3], ts->data[4]);
	z2 = CONV_DATA(ts->data[4], 0);
	
	if (ts->swap_xy)
		swap(x, y);
			
	if (z1) {
		pressure = ts->x_plate_ohm * x;
		pressure /= 4096;
		pressure *= z2 - z1;
		pressure /= z1;
	} else
		pressure = 0;

	if (ts->use_pointercal) {
		/* Transform pen state according to calibration parameters.  This
		 * algorithm is the same used by tslib and Qt to do 7-value
		 * screen transforms.  See:
		 *
		 * http://doc.trolltech.com/4.3/qwscalibratedmousehandler.html#transform
		 */
		xN = ((PA * x) + (PB * y) + PC) / PS;
		yN = ((PD * x) + (PE * y) + PF) / PS;

		x = (xN < 0) ? 0 : (xN > ts->x_res ? ts->x_res - 1 : (unsigned int)xN);
		y = (yN < 0) ? 0 : (yN > ts->y_res ? ts->y_res - 1 : (unsigned int)yN);
	}

	/* If pressure value is above a preset limit (pen is barely
	 * touching the screen) we can't trust the coordinate values.
	 */
	if (pressure < ts->pressure_limit && x < MAX_12BIT && y < MAX_12BIT) {
		ts->pressure_limit = ts->max_pressure;
		if (ts->ignore_last) {
			if (ts->sample_cnt)
				tsc2046_update_pen_state(tsc, ts->x, ts->y, ts->p);
			ts->x = x;
			ts->y = y;
			ts->p = pressure;
		} else
			tsc2046_update_pen_state(tsc, x, y, pressure);
		ts->sample_cnt++;
	}

	mod_timer(&ts->timer,
		  jiffies + msecs_to_jiffies(TSC2046_TS_SCAN_TIME));
}

static int tsc2046_is_pen_down(struct tsc2046_ts *ts)
{
	return ts->pen_down;
}

static int tsc2046_get_pendown_state(struct tsc2046_ts *ts)
{
	if (ts->get_pendown_state)
		return ts->get_pendown_state();

	return !gpio_get_value(ts->dav_gpio);
}

/*
 * Timer is called every TSC2046_TS_SCAN_TIME when the pen is down
 */
static void tsc2046_ts_timer(unsigned long arg)
{
	struct tsc2046 *tsc = (void *) arg;
	struct tsc2046_ts *ts = tsc->ts;
	unsigned long flags;
	int ndav;
	int r;

	spin_lock_irqsave(&ts->lock, flags);
	ndav = tsc2046_get_pendown_state(ts);
	if (!ndav || tsc2046_device_suspended(&tsc->spi->dev)) {
		/* Pen has been lifted */
		if (!tsc2046_device_suspended(&tsc->spi->dev) &&
		    !ts->irq_enabled)
		{
			ts->irq_enabled = 1;
			enable_irq(ts->irq);
		}
		tsc2046_update_pen_state(tsc, 0, 0, 0);
		ts->pending = 0;
		spin_unlock_irqrestore(&ts->lock, flags);

	} else {
		ts->pen_down = 1;
		spin_unlock_irqrestore(&ts->lock, flags);

		r = spi_async(tsc->spi, &ts->read_msg);
		if (r)
			dev_err(&tsc->spi->dev, "ts: spi_async() failed");
	}
}

/*
 * This interrupt is called when pen is down and first coordinates are
 * available. That is indicated by a falling edge on DEV line.  IRQ is
 * disabled here because while the pen is down the coordinates are
 * read by a timer.
 */
static irqreturn_t tsc2046_ts_irq_handler(int irq, void *dev_id)
{
	struct tsc2046 *tsc = dev_id;
	struct tsc2046_ts *ts = tsc->ts;
	unsigned long flags;

	spin_lock_irqsave(&ts->lock, flags);
	if (ts->irq_enabled) {
		ts->irq_enabled = 0;
		disable_irq_nosync(ts->irq);
		ts->pending = 1;
		ts->pressure_limit = ts->touch_pressure;
		ts->sample_cnt = 0;
		mod_timer(&ts->timer,
			  jiffies + msecs_to_jiffies(TSC2046_TS_SCAN_TIME));
	}
	spin_unlock_irqrestore(&ts->lock, flags);

	return IRQ_HANDLED;
}

/* Must be called with ts->lock held */
static void tsc2046_ts_disable(struct tsc2046 *tsc)
{
	struct tsc2046_ts *ts = tsc->ts;

	if (ts->disabled)
		return;

	ts->disabled = 1;
	if (!ts->pending) {
		ts->irq_enabled = 0;
		disable_irq(ts->irq);
	} else {
		while (ts->pending) {
			spin_unlock_irq(&ts->lock);
			msleep(1);
			spin_lock_irq(&ts->lock);
		}
	}
}

static void tsc2046_ts_enable(struct tsc2046 *tsc)
{
	struct tsc2046_ts *ts = tsc->ts;

	if (!ts->disabled)
		return;

	ts->disabled = 0;
	ts->irq_enabled = 1;
	enable_irq(ts->irq);
}

#ifdef CONFIG_PM
int tsc2046_ts_suspend(struct tsc2046 *tsc)
{
	struct tsc2046_ts *ts = tsc->ts;

	spin_lock_irq(&ts->lock);
	tsc2046_ts_disable(tsc);
	spin_unlock_irq(&ts->lock);

	return 0;
}

void tsc2046_ts_resume(struct tsc2046 *tsc)
{
	struct tsc2046_ts *ts = tsc->ts;

	spin_lock_irq(&ts->lock);
	tsc2046_ts_enable(tsc);
	spin_unlock_irq(&ts->lock);
}
#endif

static void tsc2046_ts_setup_spi_xfer(struct tsc2046 *tsc)
{
	struct tsc2046_ts *ts = tsc->ts;
	struct spi_message *m = &ts->read_msg;
	struct spi_transfer *x = &ts->read_xfer[1];

	spi_message_init(m);

	/* read and write data in one transaction */
	x->tx_buf = &tsc2046_ts_cmd_data;
	x->rx_buf = &ts->data;
	x->len    = 10;
	spi_message_add_tail(x, m);

	/* send another START_BYTE to (re)enable pen interrupts */
	x++;
	x->tx_buf = &tsc2046_ts_cmd_data[0];
	x->rx_buf = &ts->data;
	x->len = 2;
	spi_message_add_tail(x, m);

	m->complete = tsc2046_ts_rx;
	m->context = tsc;
}

/* Attribute functions */

static ssize_t tsc2046_ts_use_pointercal_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;

	return sprintf(buf, "%u\n", ts->use_pointercal);
}

static ssize_t tsc2046_ts_use_pointercal_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;
	char *endp;
	int i;

	i = simple_strtoul(buf, &endp, 10);
	spin_lock_irq(&ts->lock);

	if (i)
		ts->use_pointercal = 1;
	else
		ts->use_pointercal = 0;

	spin_unlock_irq(&ts->lock);

	return count;
}

static DEVICE_ATTR(use_pointercal, 0644, tsc2046_ts_use_pointercal_show, tsc2046_ts_use_pointercal_store);

static ssize_t tsc2046_ts_pointercal_show(struct device *dev,
		   struct device_attribute *attr, char *buf)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;

	return sprintf(buf, "%d %d %d %d %d %d %d\n", PA, PB, PC, PD, PE, PF, PS);
}

static ssize_t tsc2046_ts_pointercal_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;
	char *endp;
	int i;
	unsigned int index = 0, offset = 0;

	spin_lock_irq(&ts->lock);

	for (index = 0; index < 7; index++) {
		/* Grab the numbers */
		i = simple_strtol(buf + offset, &endp, 10);
		ts->pointercal[index] = i;

		/* Skip the space */
		offset = endp - buf + 1;
	}

	spin_unlock_irq(&ts->lock);

	return count;
}

static DEVICE_ATTR(pointercal, 0644, tsc2046_ts_pointercal_show, tsc2046_ts_pointercal_store);

static ssize_t tsc2046_ts_screen_res_show(struct device *dev,
		   struct device_attribute *attr, char *buf)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;

	return sprintf(buf, "%u %u\n", ts->x_res, ts->y_res);
}

static ssize_t tsc2046_ts_screen_res_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;
	char *endp;
	unsigned int i, offset = 0;

	spin_lock_irq(&ts->lock);

	/* Grab the x res */
	i = simple_strtoul(buf, &endp, 10);
	ts->x_res = i;

	/* Skip the space */
	offset = endp - buf + 1;

	/* Grab the y res */
	i = simple_strtoul(buf + offset, &endp, 10);
	ts->y_res = i;

	/* Update the device parameters */
	input_set_abs_params(ts->idev, ABS_X, 0, ts->x_res - 1, 0, 0);
	input_set_abs_params(ts->idev, ABS_Y, 0, ts->y_res - 1, 0, 0);

	spin_unlock_irq(&ts->lock);

	return count;
}

static DEVICE_ATTR(screen_res, 0644, tsc2046_ts_screen_res_show, tsc2046_ts_screen_res_store);

static ssize_t tsc2046_ts_pen_down_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	struct tsc2046 *tsc = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", tsc2046_is_pen_down(tsc->ts));
}

static DEVICE_ATTR(pen_down, S_IRUGO, tsc2046_ts_pen_down_show, NULL);

static ssize_t tsc2046_ts_disable_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;

	return sprintf(buf, "%u\n", ts->disabled);
}

static ssize_t tsc2046_ts_disable_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct tsc2046		*tsc = dev_get_drvdata(dev);
	struct tsc2046_ts	*ts = tsc->ts;
	char *endp;
	int i;

	i = simple_strtoul(buf, &endp, 10);
	spin_lock_irq(&ts->lock);

	if (i)
		tsc2046_ts_disable(tsc);
	else
		tsc2046_ts_enable(tsc);

	spin_unlock_irq(&ts->lock);

	return count;
}

static DEVICE_ATTR(disable_ts, 0664, tsc2046_ts_disable_show,
		   tsc2046_ts_disable_store);

/* Init functions */
static int __devinit tsc2046_probe(struct spi_device *spi)
{
	struct tsc2046			*tsc;
	struct tsc2046_ts		*ts;
	struct tsc2046_platform_data	*pdata = spi->dev.platform_data;
	struct input_dev		*idev;
	int r = -ENODEV;

	dev_dbg(&spi->dev, "%s\n", __FUNCTION__);
	
	if (!pdata) {
		dev_err(&spi->dev, "no platform data?\n");
		return -ENODEV;
	}

	tsc = kzalloc(sizeof(*tsc), GFP_KERNEL);
	if (tsc == NULL)
		return -ENOMEM;

	dev_set_drvdata(&spi->dev, tsc);
	tsc->spi = spi;
	spi->dev.power.power_state = PMSG_ON;

	spi->mode = SPI_MODE_1;
	spi->bits_per_word = 16;

	/* The max speed might've been defined by the board-specific
	 * struct */
	if (!spi->max_speed_hz)
		spi->max_speed_hz = TSC2046_HZ;
	spi_setup(spi);

	if (spi->irq < 0) {
		dev_err(&tsc->spi->dev, "need DAV IRQ/GPIO");
		return -EINVAL;
	}

	if (!pdata->get_pendown_state && !gpio_is_valid(pdata->gpio_pendown)) {
		dev_err(&spi->dev, "no get_pendown_state nor gpio_pendown?\n");
		return -EINVAL;
	}

	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (ts == NULL)
		return -ENOMEM;
	tsc->ts = ts;

	if (pdata->get_pendown_state)
		ts->get_pendown_state = pdata->get_pendown_state;
	else {
		ts->dav_gpio = pdata->gpio_pendown;
		r = gpio_request(ts->dav_gpio, dev_name(&tsc->spi->dev));
		if (r < 0) {
			dev_err(&tsc->spi->dev, "unable to get DAV GPIO");
			goto err1;
		}
		gpio_direction_input(ts->dav_gpio);
	}
	
	ts->swap_xy = pdata->swap_xy;
	ts->irq = spi->irq;

	init_timer(&ts->timer);
	ts->timer.data = (unsigned long) tsc;
	ts->timer.function = tsc2046_ts_timer;

	spin_lock_init(&ts->lock);

	ts->x_plate_ohm	= pdata->ts_x_plate_ohm ? : 280;
	ts->max_pressure = pdata->ts_max_pressure ? : MAX_12BIT;
	ts->touch_pressure = pdata->ts_touch_pressure ? : ts->max_pressure;
	ts->ignore_last	= pdata->ts_ignore_last;

	idev = input_allocate_device();
	if (idev == NULL) {
		r = -ENOMEM;
		goto err2;
	}
	idev->name = "TSC2046 touchscreen";
	snprintf(ts->phys, sizeof(ts->phys),
		 "%s/input-ts", dev_name(&tsc->spi->dev));
	idev->phys = ts->phys;

	idev->evbit[0] = BIT(EV_ABS) | BIT(EV_KEY);
	idev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y) | BIT(ABS_PRESSURE);
	ts->idev = idev;

	tsc2046_ts_setup_spi_xfer(tsc);

	/* X and Y res default to 240x320 */
	ts->x_res = pdata->ts_x_res ? : 240;
	ts->y_res = pdata->ts_y_res ? : 320;

	input_set_abs_params(idev, ABS_X, 0, ts->x_res - 1, 0, 0);
	input_set_abs_params(idev, ABS_Y, 0, ts->y_res - 1, 0, 0);
	input_set_abs_params(idev, ABS_PRESSURE, 0, 1, 0, 0);

	set_bit(BTN_TOUCH, idev->keybit);

	r = request_irq(ts->irq, tsc2046_ts_irq_handler,
			IRQF_SAMPLE_RANDOM | IRQF_TRIGGER_FALLING,
			"tsc2046-ts", tsc);
	if (r < 0) {
		dev_err(&tsc->spi->dev, "unable to get DAV IRQ");
		goto err3;
	}
	set_irq_wake(ts->irq, 1);
	ts->irq_enabled = 1;

	/* Create the various driver attributes */
	if (device_create_file(&tsc->spi->dev, &dev_attr_pen_down) < 0)
		goto err4;
	if (device_create_file(&tsc->spi->dev, &dev_attr_disable_ts) < 0)
		goto err5;
	if (device_create_file(&tsc->spi->dev, &dev_attr_use_pointercal) < 0)
		goto err6;
	if (device_create_file(&tsc->spi->dev, &dev_attr_pointercal) < 0)
		goto err7;
	if (device_create_file(&tsc->spi->dev, &dev_attr_screen_res) < 0)
		goto err8;

	r = input_register_device(idev);
	if (r < 0) {
		dev_err(&tsc->spi->dev, "can't register touchscreen device\n");
		goto err9;
	}

	/* kick off a transaction to enable pen interrupts */
	spi_async(tsc->spi, &ts->read_msg);

	return 0;
err9:
	device_remove_file(&tsc->spi->dev, &dev_attr_screen_res);
err8:
	device_remove_file(&tsc->spi->dev, &dev_attr_pointercal);
err7:
	device_remove_file(&tsc->spi->dev, &dev_attr_use_pointercal);
err6:
	device_remove_file(&tsc->spi->dev, &dev_attr_disable_ts);
err5:
	device_remove_file(&tsc->spi->dev, &dev_attr_pen_down);
err4:
	free_irq(ts->irq, tsc);
err3:
	input_free_device(idev);
err2:
	if (ts->dav_gpio)
		gpio_free(ts->dav_gpio);
err1:
	kfree(ts);
	return r;
}

static int __devexit tsc2046_remove(struct spi_device *spi)
{
	struct tsc2046 *tsc = dev_get_drvdata(&spi->dev);
	struct tsc2046_ts *ts = tsc->ts;
	unsigned long flags;

	dev_dbg(&tsc->spi->dev, "%s\n", __FUNCTION__);

	spin_lock_irqsave(&ts->lock, flags);
	tsc2046_ts_disable(tsc);
	spin_unlock_irqrestore(&ts->lock, flags);

	device_remove_file(&tsc->spi->dev, &dev_attr_disable_ts);
	device_remove_file(&tsc->spi->dev, &dev_attr_pen_down);
	device_remove_file(&tsc->spi->dev, &dev_attr_use_pointercal);
	device_remove_file(&tsc->spi->dev, &dev_attr_pointercal);
	device_remove_file(&tsc->spi->dev, &dev_attr_screen_res);

	free_irq(ts->irq, tsc);
	input_unregister_device(ts->idev);

	if (ts->dav_gpio)
		gpio_free(ts->dav_gpio);

	kfree(ts);
	kfree(tsc);

	return 0;
}

static struct spi_driver tsc2046_driver = {
	.driver = {
	   .name = "tsc2046",
	   .bus = &spi_bus_type,
	   .owner = THIS_MODULE,
	},
	.probe = tsc2046_probe,
	.remove = __devexit_p(tsc2046_remove),
};

static int __init tsc2046_init(void)
{
	printk(KERN_INFO "TSC2046 driver initializing\n");

	return spi_register_driver(&tsc2046_driver);
}

static void __exit tsc2046_exit(void)
{
	spi_unregister_driver(&tsc2046_driver);
}

module_init(tsc2046_init);
module_exit(tsc2046_exit);

MODULE_AUTHOR("Kevin Hilman <khilman@xxxxxxxxxx>");
MODULE_LICENSE("GPL");

