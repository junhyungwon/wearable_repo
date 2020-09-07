/*
 * Copyright (C) 2012 Kionix, Inc.
 * Written by Chris Hudson <chudson@kionix.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/input/kxcj9.h>
#include <linux/input-polldev.h>
#include <linux/gpio.h>

#define NAME			"kxcj9"
/* don't use input handler fuzz (for jitter) */
#define G_MAX			0x7FF //# 8000
/* OUTPUT REGISTERS */
#define XOUT_L			0x06
#define WHO_AM_I		0x0F
/* CONTROL REGISTERS */
#define INT_REL			0x1A
#define CTRL_REG1		0x1B
#define INT_CTRL1		0x1E
#define DATA_CTRL		0x21
/* CONTROL REGISTER 1 BITS */
#define PC1_OFF			0x7F
#define PC1_ON			(1 << 7)
/* Data ready funtion enable bit: set during probe if using irq mode */
#define DRDYE			(1 << 5)
/* DATA CONTROL REGISTER BITS */
#define ODR12_5F		0
#define ODR25F			1
#define ODR50F			2
#define ODR100F			3
#define ODR200F			4
#define ODR400F			5
#define ODR800F			6
/* INTERRUPT CONTROL REGISTER 1 BITS */
/* Set these during probe if using irq mode */
#define KXCJ9_IEL		(1 << 3)
#define KXCJ9_IEA		(1 << 4)
#define KXCJ9_IEN		(1 << 5)
/* INPUT_ABS CONSTANTS */
#define FUZZ			3 /* noise filter +/-3 range is ignored */
#define FLAT			3 /* Flat center value (zero) */

/* RESUME STATE INDICES */
#define RES_DATA_CTRL		0
#define RES_CTRL_REG1		1
#define RES_INT_CTRL1		2
#define RESUME_ENTRIES		3

/*
 * The following table lists the maximum appropriate poll interval for each
 * available output data rate.
 */
static const struct {
	unsigned int cutoff;
	u8 mask;
} kxcj9_odr_table[] = {
	{ 3,	ODR800F },
	{ 5,	ODR400F },
	{ 10,	ODR200F },
	{ 20,	ODR100F },
	{ 40,	ODR50F  },
	{ 80,	ODR25F  },
	{ 0,	ODR12_5F},
};

struct kxcj9_data {
	struct i2c_client *client;
	struct kxcj9_platform_data pdata;
	struct input_dev *input_dev;
#ifdef CONFIG_INPUT_KXCJ9_POLLED_MODE
	struct input_polled_dev *poll_dev;
#endif
	unsigned int last_poll_interval;
	bool	enable;
	u8 shift;
	u8 ctrl_reg1;
	u8 data_ctrl;
	u8 int_ctrl;
};

#if 0
static int kxcj9_i2c_read(struct kxcj9_data *cj9, u8 addr, u8 *data, int len)
{
	struct i2c_msg msgs[] = {
		{
			.addr = cj9->client->addr,
			.flags = cj9->client->flags,
			.len = 1,
			.buf = &addr,
		},
		{
			.addr = cj9->client->addr,
			.flags = cj9->client->flags | I2C_M_RD,
			.len = len,
			.buf = data,
		},
	};

	return i2c_transfer(cj9->client->adapter, msgs, 2);
}
#endif

static void kxcj9_report_acceleration_data(struct kxcj9_data *cj9)
{
	/* Data bytes from hardware xL, xH, yL, yH, zL, zH */
 	u8 data[6];
	s16 x, y, z;
	int err;

	err = i2c_smbus_read_i2c_block_data(cj9->client,
			XOUT_L, 6, data);
	if (err != 6) {
		dev_err(&cj9->client->dev, "accelerometer data read failed\n");
		return;
	}

	/* note: lsb 4bit reserved (refer to datasheet) */
	x = ((0xf0 & data[0]) >> 4) | (data[1] << 4);
	y = ((0xf0 & data[2]) >> 4) | (data[3] << 4);
	z = ((0xf0 & data[4]) >> 4) | (data[5] << 4);

	/* sign extension */
	x = (s16) (x << 4) >> 4;
	y = (s16) (y << 4) >> 4;
	z = (s16) (z << 4) >> 4;

	input_report_abs(cj9->input_dev, ABS_X, cj9->pdata.negate_x ? -x : x);
	input_report_abs(cj9->input_dev, ABS_Y, cj9->pdata.negate_y ? -y : y);
	input_report_abs(cj9->input_dev, ABS_Z, cj9->pdata.negate_z ? -z : z);
	input_sync(cj9->input_dev);
}

static irqreturn_t kxcj9_isr(int irq, void *dev)
{
	struct kxcj9_data *cj9 = dev;
	int err;

	/* data ready is the only possible interrupt type */
	kxcj9_report_acceleration_data(cj9);

	err = i2c_smbus_read_byte_data(cj9->client, INT_REL);
	if (err < 0)
		dev_err(&cj9->client->dev,
			"error clearing interrupt status: %d\n", err);

	return IRQ_HANDLED;
}

static int kxcj9_update_g_range(struct kxcj9_data *cj9, u8 new_g_range)
{
	switch (new_g_range) {
	case KXCJ9_G_2G:
		cj9->shift = 4;
		break;
	case KXCJ9_G_4G:
		cj9->shift = 3;
		break;
	case KXCJ9_G_8G:
		cj9->shift = 2;
		break;
	default:
		return -EINVAL;
	}

	cj9->ctrl_reg1 &= 0xe7;
	cj9->ctrl_reg1 |= new_g_range;

	return 0;
}

static int kxcj9_update_odr(struct kxcj9_data *cj9, unsigned int poll_interval)
{
	int err;
	int i;

	/* Use the lowest ODR that can support the requested poll interval */
	for (i = 0; i < ARRAY_SIZE(kxcj9_odr_table); i++) {
		cj9->data_ctrl = kxcj9_odr_table[i].mask;
		if (poll_interval < kxcj9_odr_table[i].cutoff)
			break;
	}

	/* note change the value of this register, pc1 must set to "0" */
	err = i2c_smbus_write_byte_data(cj9->client, CTRL_REG1, 0);
	if (err < 0)
		return err;

	err = i2c_smbus_write_byte_data(cj9->client, DATA_CTRL, cj9->data_ctrl);
	if (err < 0)
		return err;

	err = i2c_smbus_write_byte_data(cj9->client, CTRL_REG1, cj9->ctrl_reg1);
	if (err < 0)
		return err;

	return 0;
}

static int kxcj9_device_power_on(struct kxcj9_data *cj9)
{
	if (cj9->pdata.power_on)
		return cj9->pdata.power_on();

	return 0;
}

static void kxcj9_device_power_off(struct kxcj9_data *cj9)
{
	int err;

	cj9->ctrl_reg1 &= PC1_OFF;
	err = i2c_smbus_write_byte_data(cj9->client, CTRL_REG1, cj9->ctrl_reg1);
	if (err < 0)
		dev_err(&cj9->client->dev, "soft power off failed\n");

	if (cj9->pdata.power_off)
		cj9->pdata.power_off();
}

static int kxcj9_enable(struct kxcj9_data *cj9)
{
	int err;

	err = kxcj9_device_power_on(cj9);
	if (err < 0)
		return err;

	/* ensure that PC1 is cleared before updating control registers */
	err = i2c_smbus_write_byte_data(cj9->client, CTRL_REG1, 0);
	if (err < 0)
		return err;

	/* only write INT_CTRL_REG1 if in irq mode */
	if (cj9->client->irq) {
		err = i2c_smbus_write_byte_data(cj9->client,
						INT_CTRL1, cj9->int_ctrl);
		if (err < 0)
			return err;
	}

	err = kxcj9_update_g_range(cj9, cj9->pdata.g_range);
	if (err < 0)
		return err;

	/* turn on outputs */
	cj9->ctrl_reg1 |= PC1_ON;
	err = i2c_smbus_write_byte_data(cj9->client, CTRL_REG1, cj9->ctrl_reg1);
	if (err < 0)
		return err;

	err = kxcj9_update_odr(cj9, cj9->last_poll_interval);
	if (err < 0)
		return err;

	/* clear initial interrupt if in irq mode */
	if (cj9->client->irq) {
		err = i2c_smbus_read_byte_data(cj9->client, INT_REL);
		if (err < 0) {
			dev_err(&cj9->client->dev,
				"error clearing interrupt: %d\n", err);
			goto fail;
		}
	}

	return 0;

fail:
	kxcj9_device_power_off(cj9);
	return err;
}

static void kxcj9_disable(struct kxcj9_data *cj9)
{
	kxcj9_device_power_off(cj9);
}

static void __devinit kxcj9_init_input_device(struct kxcj9_data *cj9,
					      struct input_dev *input_dev)
{
	__set_bit(EV_ABS, input_dev->evbit);
	input_set_abs_params(input_dev, ABS_X, -G_MAX, G_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, -G_MAX, G_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_Z, -G_MAX, G_MAX, 0, 0);

	input_dev->name = "kxcj9_accel";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = &cj9->client->dev;
}

static int __devinit kxcj9_setup_input_device(struct kxcj9_data *cj9)
{
	struct input_dev *input_dev;
	int err;

	input_dev = input_allocate_device();
	if (!input_dev) {
		dev_err(&cj9->client->dev, "input device allocate failed\n");
		return -ENOMEM;
	}

	cj9->input_dev = input_dev;

	input_set_drvdata(input_dev, cj9);

	kxcj9_init_input_device(cj9, input_dev);

	err = input_register_device(cj9->input_dev);
	if (err) {
		dev_err(&cj9->client->dev,
			"unable to register input polled device %s: %d\n",
			cj9->input_dev->name, err);
		input_free_device(cj9->input_dev);
		return err;
	}

	return 0;
}

static ssize_t kxcj9_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);

	return snprintf(buf, 4, "%d\n", cj9->enable);
}

static ssize_t kxcj9_enable_store(struct device *dev, struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);
	struct input_dev *input_dev = cj9->input_dev;
	char *after;
	unsigned long data = simple_strtoul(buf, &after, 10);
	size_t size = after - buf;
	ssize_t ret = -EINVAL;

	if (isspace(*after))
		size++;

	if (size == count) {
		ret = size;
		mutex_lock(&input_dev->mutex);

		if (data == 0) {
			disable_irq(client->irq);
			kxcj9_disable(cj9);
			cj9->enable = false;
		} else if (data == 1) {
			if (!kxcj9_enable(cj9)) {
				enable_irq(client->irq);
				cj9->enable = true;
			}
		} else {
			dev_err(&cj9->client->dev,
				"Invalid value of input, input=%ld\n", data);
		}

		mutex_unlock(&input_dev->mutex);
	}

	return ret;
}

static DEVICE_ATTR(enable, S_IRUGO|S_IWUSR|S_IWGRP,
			kxcj9_enable_show, kxcj9_enable_store);

/*
 * When IRQ mode is selected, we need to provide an interface to allow the user
 * to change the output data rate of the part.  For consistency, we are using
 * the set_poll method, which accepts a poll interval in milliseconds, and then
 * calls update_odr() while passing this value as an argument.  In IRQ mode, the
 * data outputs will not be read AT the requested poll interval, rather, the
 * lowest ODR that can support the requested interval.  The client application
 * will be responsible for retrieving data from the input node at the desired
 * interval.
 */

/* Returns currently selected poll interval (in ms) */
static ssize_t kxcj9_get_poll_delay(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);

	return sprintf(buf, "%d\n", cj9->last_poll_interval);
}

/* Allow users to select a new poll interval (in ms) */
static ssize_t kxcj9_set_poll_delay(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);
	struct input_dev *input_dev = cj9->input_dev;
	char *after;
	unsigned int interval = simple_strtoul(buf, &after, 10);
	size_t size = after - buf;
	ssize_t ret = -EINVAL;

	if (isspace(*after))
		size++;

	if (size == count) {
		ret = size;
		/* Lock the device to prevent races with open/close (and itself) */
		mutex_lock(&input_dev->mutex);

		if (cj9->enable)
			disable_irq(client->irq);
		/*
		 * Set current interval to the greater of the minimum interval or
		 * the requested interval
		 */
		cj9->last_poll_interval = max(interval, cj9->pdata.min_interval);

		if (cj9->enable) {
			kxcj9_update_odr(cj9, cj9->last_poll_interval);
			enable_irq(client->irq);
		}
		mutex_unlock(&input_dev->mutex);
	}

	return ret;
}
static DEVICE_ATTR(poll_delay, S_IRUGO|S_IWUSR|S_IWGRP,
		kxcj9_get_poll_delay, kxcj9_set_poll_delay);

static struct attribute *kxcj9_attributes[] = {
	&dev_attr_enable.attr,
	&dev_attr_poll_delay.attr,
	NULL
};

static struct attribute_group kxcj9_attribute_group = {
	.attrs = kxcj9_attributes
};

#ifdef CONFIG_INPUT_KXCJ9_POLLED_MODE
static void kxcj9_poll(struct input_polled_dev *dev)
{
	struct kxcj9_data *cj9 = dev->private;
	unsigned int poll_interval = dev->poll_interval;

	kxcj9_report_acceleration_data(cj9);

	if (poll_interval != cj9->last_poll_interval) {
		kxcj9_update_odr(cj9, poll_interval);
		cj9->last_poll_interval = poll_interval;
	}
}

static void kxcj9_polled_input_open(struct input_polled_dev *dev)
{
	struct kxcj9_data *cj9 = dev->private;

	kxcj9_enable(cj9);
}

static void kxcj9_polled_input_close(struct input_polled_dev *dev)
{
	struct kxcj9_data *cj9 = dev->private;

	kxcj9_disable(cj9);
}

static int __devinit kxcj9_setup_polled_device(struct kxcj9_data *cj9)
{
	int err;
	struct input_polled_dev *poll_dev;
	poll_dev = input_allocate_polled_device();

	if (!poll_dev) {
		dev_err(&cj9->client->dev,
			"Failed to allocate polled device\n");
		return -ENOMEM;
	}

	cj9->poll_dev = poll_dev;
	cj9->input_dev = poll_dev->input;

	poll_dev->private = cj9;
	poll_dev->poll = kxcj9_poll;
	poll_dev->open = kxcj9_polled_input_open;
	poll_dev->close = kxcj9_polled_input_close;

	kxcj9_init_input_device(cj9, poll_dev->input);

	err = input_register_polled_device(poll_dev);
	if (err) {
		dev_err(&cj9->client->dev,
			"Unable to register polled device, err=%d\n", err);
		input_free_polled_device(poll_dev);
		return err;
	}

	return 0;
}

static void __devexit kxcj9_teardown_polled_device(struct kxcj9_data *cj9)
{
	input_unregister_polled_device(cj9->poll_dev);
	input_free_polled_device(cj9->poll_dev);
}

#else

static inline int kxcj9_setup_polled_device(struct kxcj9_data *cj9)
{
	return -ENOSYS;
}

static inline void kxcj9_teardown_polled_device(struct kxcj9_data *cj9)
{
}

#endif

static int __devinit kxcj9_verify(struct kxcj9_data *cj9)
{
	int retval;

	retval = i2c_smbus_read_byte_data(cj9->client, WHO_AM_I);
	if (retval < 0) {
		dev_err(&cj9->client->dev, "read err int source\n");
		goto out;
	}
	/* kxcj9 is 0xa */
	retval = (retval != 0x0a) ? -EIO : 0;

out:
	return retval;
}

static int __devinit kxcj9_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	const struct kxcj9_platform_data *pdata = client->dev.platform_data;
	struct kxcj9_data *cj9;
	int err;

	if (!i2c_check_functionality(client->adapter,
				I2C_FUNC_I2C | I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "client is not i2c capable\n");
		return -ENXIO;
	}

	if (!pdata) {
		dev_err(&client->dev, "platform data is NULL; exiting\n");
		return -EINVAL;
	}

	cj9 = kzalloc(sizeof(*cj9), GFP_KERNEL);
	if (!cj9) {
		dev_err(&client->dev,
			"failed to allocate memory for module data\n");
		return -ENOMEM;
	}

	cj9->client = client;
	cj9->pdata = *pdata;

	if (pdata->init) {
		err = pdata->init();
		if (err < 0)
			goto err_free_mem;
	}

	err = kxcj9_device_power_on(cj9);
	if (err < 0) {
		dev_err(&client->dev, "power on failed! err=%d\n", err);
		goto err_pdata_exit;
	}

	err = kxcj9_verify(cj9);
	if (err < 0) {
		dev_err(&client->dev, "device not recognized\n");
		goto err_power_off;
	}

	i2c_set_clientdata(client, cj9);

	cj9->ctrl_reg1 = cj9->pdata.res_12bit | cj9->pdata.g_range;
	cj9->last_poll_interval = cj9->pdata.init_interval;

	if (client->irq > 0) {
		/* If in irq mode, populate INT_CTRL_REG1 and enable DRDY. */
		cj9->int_ctrl |= KXCJ9_IEN | KXCJ9_IEA | KXCJ9_IEL;
		cj9->ctrl_reg1 |= DRDYE;

		err = kxcj9_setup_input_device(cj9);
		if (err)
			goto err_power_off;

		err = request_threaded_irq(client->irq, NULL, kxcj9_isr,
					   IRQF_TRIGGER_RISING | IRQF_ONESHOT,
					   "kxcj9-irq", cj9);
		if (err) {
			dev_err(&client->dev, "request irq failed: %d\n", err);
			goto err_destroy_input;
		}

		disable_irq(cj9->client->irq);

		err = sysfs_create_group(&client->dev.kobj, &kxcj9_attribute_group);
		if (err) {
			dev_err(&client->dev, "sysfs create failed: %d\n", err);
			goto err_free_irq;
		}

	} else {
		err = kxcj9_setup_polled_device(cj9);
		if (err)
			goto err_power_off;
	}

	dev_info(&client->dev, "%s: kxcj9_probe OK.\n", __func__);

	return 0;

err_free_irq:
	if (client->irq > 0)
		free_irq(client->irq, cj9);
err_destroy_input:
	input_unregister_device(cj9->input_dev);
err_power_off:
	kxcj9_device_power_off(cj9);
err_pdata_exit:
	if (cj9->pdata.exit)
		cj9->pdata.exit();
err_free_mem:
	kfree(cj9);

	dev_err(&client->dev, "%s: kxcj9_probe err=%d\n", __func__, err);
	return err;
}

static int __devexit kxcj9_remove(struct i2c_client *client)
{
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);

	if (client->irq) {
		sysfs_remove_group(&client->dev.kobj, &kxcj9_attribute_group);
		free_irq(client->irq, cj9);
		input_unregister_device(cj9->input_dev);
	} else {
		kxcj9_teardown_polled_device(cj9);
	}

	kxcj9_device_power_off(cj9);

	if (cj9->pdata.exit)
		cj9->pdata.exit();

	kfree(cj9);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int kxcj9_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);
	struct input_dev *input_dev = cj9->input_dev;

	mutex_lock(&input_dev->mutex);

	if (input_dev->users)
		kxcj9_disable(cj9);

	mutex_unlock(&input_dev->mutex);
	return 0;
}

static int kxcj9_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct kxcj9_data *cj9 = i2c_get_clientdata(client);
	struct input_dev *input_dev = cj9->input_dev;
	int retval = 0;

	mutex_lock(&input_dev->mutex);

	if (input_dev->users)
		kxcj9_enable(cj9);

	mutex_unlock(&input_dev->mutex);
	return retval;
}
#endif

static SIMPLE_DEV_PM_OPS(kxcj9_pm_ops, kxcj9_suspend, kxcj9_resume);

static const struct i2c_device_id kxcj9_id[] = {
	{ NAME, 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, kxcj9_id);

static struct i2c_driver kxcj9_driver = {
	.driver = {
		.name	= NAME,
		.owner	= THIS_MODULE,
		.pm	= &kxcj9_pm_ops,
	},
	.probe		= kxcj9_probe,
	.remove		= __devexit_p(kxcj9_remove),
	.id_table	= kxcj9_id,
};

static int __init kxcj9_init(void)
{
	return i2c_add_driver(&kxcj9_driver);
}
module_init(kxcj9_init);

static void __exit kxcj9_exit(void)
{
	i2c_del_driver(&kxcj9_driver);
}
module_exit(kxcj9_exit);

MODULE_DESCRIPTION("KXCJ9 accelerometer driver");
MODULE_AUTHOR("Chris Hudson <chudson@kionix.com>");
MODULE_LICENSE("GPL");
