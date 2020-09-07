/*
 * tmp105.c
 *
 * TMP105 temperature sensor driver
 *
 * Copyright (C) 2010 Texas Instruments
 *
 * Author: Shubhrajyoti Datta <a0393217@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/input-polldev.h>
#include <linux/input/tmp105.h>
#include <linux/mutex.h>


#define	DRIVER_NAME					"tmp105"

/* Registers */
#define		TMP105_TEMP_REG		0x00
#define		TMP105_CONF_REG		0x01
#define		TMP105_TLOW_REG		0x02
#define		TMP105_THIGH_REG	0x03

/* Configuration register parameters */
#define		TMP105_CONF_SD		0x01
#define		TMP105_CONF_TM		0x02
#define		TMP105_CONF_POL		0x04
#define		TMP105_CONF_F0		0x08
#define		TMP105_CONF_F1		0x10
#define		TMP105_CONF_R0		0x20
#define		TMP105_CONF_R1		0x40
#define		TMP105_CONF_OS		0x80

#define         MAX_TEMP		128
#define         MIN_TEMP		-55

/* Each client has this additional data */
struct tmp105_data {
	struct i2c_client *client;

	struct input_dev *input_dev;
	struct input_polled_dev *poll_dev;
	
	/* mutex for sysfs operations */
	struct mutex lock;
	s16 temp[3];
	unsigned long last_updated;
	u8 configuration_setting;
};

static const u8 tmp105_reg[] = {
	TMP105_TEMP_REG,
	TMP105_TLOW_REG,
	TMP105_THIGH_REG,
};

static void tmp105_init_client(struct i2c_client *client);

static signed long tmp105_reg_to_mC(s16 val)
{
	signed long temp_mC;
	if (val  & 0x800)
		val = val - 0x1000 ;
	 temp_mC = (val * 64000) / 1024;
	 return temp_mC;
}

static u16 tmp105_C_to_reg(signed long val)
{
	val =  (val * 1024) / 64000;
	if (val < 0)
		val = val + 0x1000;
	return (u16)val;
}

static s16 tmp105_update_device(struct i2c_client *client,
						int  index)
{
	struct tmp105_data *data = i2c_get_clientdata(client);
	u8 tmp[2];

	mutex_lock(&data->lock);

	if (time_after(jiffies, data->last_updated +  HZ/4)) {
		i2c_smbus_read_i2c_block_data(client,
						tmp105_reg[index], 2, tmp);
		data->temp[index] = ((tmp[0] << 4) | ((tmp[1] & 0xF0) >> 4));
		//printk(KERN_INFO "Raw temperature: %u\n", data->temp[index]);
		data->last_updated = jiffies;
	}

	mutex_unlock(&data->lock);
	return data->temp[index] ;
}

static ssize_t show_temp_value(struct device *dev,
			       struct device_attribute *devattr, char *buf)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(devattr);
	struct i2c_client *client = to_i2c_client(dev);
	s16 temperature = tmp105_update_device(client , sda->index);
	signed long temp_in_mC;

	temp_in_mC = tmp105_reg_to_mC(temperature);

	return sprintf(buf, "%ld\n", temp_in_mC);
}

static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp_value, NULL , 0);

static ssize_t tmp105_set_temp(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct sensor_device_attribute *sda = to_sensor_dev_attr(attr);
	struct i2c_client *client = to_i2c_client(dev);
	struct tmp105_data *tmp105 = i2c_get_clientdata(client);
	signed long val;
	int status = 0;
	u16 temp;

	if ((strict_strtol(buf, 10, &val) < 0))
		return -EINVAL;

	SENSORS_LIMIT(val , MIN_TEMP , MAX_TEMP);

	mutex_lock(&tmp105->lock);

	temp = tmp105_C_to_reg(val);
	temp = ((temp & 0xFF0) >> 4) | ((temp & 0xF)<<12);

	status = i2c_smbus_write_word_data(client, tmp105_reg[sda->index],
			temp);

	tmp105->temp[sda->index] = temp;
	mutex_unlock(&tmp105->lock);
	return status ? : count;
}

static SENSOR_DEVICE_ATTR(temp1_min, S_IWUSR | S_IRUGO, show_temp_value,
	tmp105_set_temp, 1);
static SENSOR_DEVICE_ATTR(temp1_max, S_IWUSR | S_IRUGO, show_temp_value,
	tmp105_set_temp, 2);

/* sysfs call */
static ssize_t set_configuration(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	s32 status;
	struct i2c_client *client = to_i2c_client(dev);
	struct tmp105_data *data = i2c_get_clientdata(client);
	data->configuration_setting = simple_strtoul(buf, NULL, 10);
	/* I2C write to the configuration register */
	status = i2c_smbus_write_byte_data(client, TMP105_CONF_REG,
			data->configuration_setting);
	return count;
}

static ssize_t show_configuration(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct tmp105_data *data = i2c_get_clientdata(client);
	u8 tmp;
	i2c_smbus_read_i2c_block_data(client, TMP105_CONF_REG, 1, &tmp);
	data->configuration_setting = tmp;
	return sprintf(buf, "%u\n", data->configuration_setting);
}
static DEVICE_ATTR(configuration, S_IWUSR | S_IRUGO, show_configuration,
		set_configuration);

static struct attribute *tmp105_attributes[] = {
	&dev_attr_configuration.attr,
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp1_min.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	NULL
};

static const struct attribute_group tmp105_attr_group = {
	.attrs = tmp105_attributes,
};

/* Called when we have found a new TMP105. */
static void tmp105_init_client(struct i2c_client *client)
{
	struct tmp105_data *data = i2c_get_clientdata(client);
	s32	status;

	data->last_updated = jiffies - HZ;

	mutex_init(&data->lock);

	mutex_lock(&data->lock);

	status = i2c_smbus_write_byte_data(client, TMP105_CONF_REG, 
			data->configuration_setting);

	mutex_unlock(&data->lock);
}

static void tmp105_poll(struct input_polled_dev *dev)
{
	struct tmp105_data *tmp105 = dev->private;
	struct i2c_client *client = tmp105->client;
	s16 temperature = tmp105_update_device(client , 0);
	signed long temp_in_mC;

	temp_in_mC = tmp105_reg_to_mC(temperature);

	input_event(tmp105->input_dev, EV_MSC, MSC_RAW, temp_in_mC);
	input_sync(tmp105->input_dev);
}

static int tmp105_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct tmp105_data *tmp105;
	struct input_polled_dev *poll_dev;
	const struct tmp105_platform_data *pdata = client->dev.platform_data;
	int err=0;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_dbg(&client->dev, "adapter doesn't support I2C\n");
		return -ENODEV;
	}
	
	if (!pdata) {
		dev_err(&client->dev, "%s:platform data is NULL\n", DRIVER_NAME);
		return -EINVAL;
	}

	tmp105 = kzalloc(sizeof(struct tmp105_data), GFP_KERNEL);
	if (!tmp105) {
		return -ENOMEM;
	}

	tmp105->client = client;

	if (pdata->init) {
		err = pdata->init();
		if (err < 0)
			goto err_free_mem;
	}
	
	i2c_set_clientdata(client, tmp105);

	poll_dev = input_allocate_polled_device();
	if (!poll_dev) {
		dev_err(&client->dev, "Failed to allocate polled device\n");
		goto  err_pdata_exit;
	}

	/* Initialize the TMP105 chip */
	tmp105->configuration_setting = pdata->config;
	tmp105_init_client(client);
	
	tmp105->input_dev = poll_dev->input;
	tmp105->poll_dev = poll_dev;
	
	poll_dev->private = tmp105;
	poll_dev->poll_interval = pdata->poll_interval? pdata->poll_interval:100; 
	poll_dev->poll = tmp105_poll;
	
	tmp105->input_dev->name = "tmp105";
	tmp105->input_dev->id.bustype = BUS_I2C;
	tmp105->input_dev->dev.parent = &client->dev;
	
	tmp105->input_dev->evbit[0]  = BIT_MASK(EV_MSC);
	tmp105->input_dev->mscbit[0] = BIT_MASK(MSC_RAW);
		
	err = input_register_polled_device(poll_dev);
	if (err) {
		dev_err(&client->dev,
				"Unable to register polled device, err=%d\n", err);
		input_free_polled_device(poll_dev);
		goto err_pdata_exit;
	}

	
	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &tmp105_attr_group);
	if (err)
		goto err_destroy_input;
		
	return 0;

err_destroy_input:
	input_unregister_polled_device(poll_dev);
	input_free_polled_device(poll_dev);
	
err_pdata_exit:
	if (pdata->exit)
		pdata->exit();
		
err_free_mem:
	kfree(tmp105);
	
	return err;
}

static int tmp105_remove(struct i2c_client *client)
{
	struct tmp105_data *tmp105 = i2c_get_clientdata(client);

	sysfs_remove_group(&client->dev.kobj, &tmp105_attr_group);
	input_unregister_polled_device(tmp105->poll_dev);
	input_free_polled_device(tmp105->poll_dev);
	
	i2c_set_clientdata(client, NULL);

	kfree(tmp105);

	return 0;
}

static const struct i2c_device_id tmp105_id[] = {
	{ "tmp105", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp105_id);

static struct i2c_driver tmp105_driver = {
	.driver = {
		.name	= "tmp105",
		.owner = THIS_MODULE,
	},
	.probe		= tmp105_probe,
	.remove		= tmp105_remove,
	.id_table	= tmp105_id,
};

static int __init tmp105_init(void)
{
	return i2c_add_driver(&tmp105_driver);
}

static void __exit tmp105_exit(void)
{
	i2c_del_driver(&tmp105_driver);
}
module_init(tmp105_init);
module_exit(tmp105_exit);

MODULE_DESCRIPTION("TMP105 driver");
MODULE_LICENSE("GPL");
