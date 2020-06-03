/* Texas Instruments TMP103 I2C temperature sensor driver
 *
 * based on tmp102.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/device.h>

#define	DRIVER_NAME 		"tmp103"

#define	TMP103_TEMP_REG		0x00
#define	TMP103_CONF_REG		0x01
#define	TMP103_TLOW_REG		0x02
#define	TMP103_THIGH_REG	0x03

#define	TMP103_CONF_M1		0x02

struct tmp103 {
	struct device *hwmon_dev;
	struct mutex lock;
	
	unsigned long last_update;
	int temp;
};

static inline int tmp103_read_reg(struct i2c_client *client, u8 reg)
{
	return i2c_smbus_read_byte_data(client, reg);
}

static inline int tmp103_write_reg(struct i2c_client *client, u8 reg, u16 val)
{
	return i2c_smbus_write_byte_data(client, reg, val);
}

static struct tmp103 *tmp103_update_device(struct i2c_client *client)
{
	struct tmp103 *tmp103 = i2c_get_clientdata(client);

	mutex_lock(&tmp103->lock);
	
	/* continuous mode, 0.25hz delay */
	if (time_after(jiffies, tmp103->last_update + HZ / 4)) {
		char status = (char)tmp103_read_reg(client, TMP103_TEMP_REG);
		tmp103->temp = (int)status;
		tmp103->last_update = jiffies;
	}
	
	mutex_unlock(&tmp103->lock);
	return tmp103;
}

static ssize_t tmp103_show_temp(struct device *dev,
				struct device_attribute *attr,	char *buf)
{
	struct tmp103 *tmp103 = tmp103_update_device(to_i2c_client(dev));

	return sprintf(buf, "%d\n", tmp103->temp);
}

static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, tmp103_show_temp, NULL , 0);

static struct attribute *tmp103_attributes[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	NULL
};

static const struct attribute_group tmp103_attr_group = {
	.attrs = tmp103_attributes,
};

#define TMP103_CONFIG  (TMP103_CONF_M1) /* continuous mode, 0.25hz time */

static int __devinit tmp103_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	struct tmp103 *tmp103;
	int status;

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev, "adapter doesnt support SMBus word "
			"transactions\n");
		return -ENODEV;
	}

	tmp103 = kzalloc(sizeof(*tmp103), GFP_KERNEL);
	if (!tmp103) {
		dev_dbg(&client->dev, "kzalloc failed\n");
		return -ENOMEM;
	}
	i2c_set_clientdata(client, tmp103);

	status = tmp103_write_reg(client, TMP103_CONF_REG, TMP103_CONFIG);
	if (status < 0) {
		dev_err(&client->dev, "error writing config register\n");
		goto fail_free;
	}
	tmp103->last_update = jiffies - HZ;
	mutex_init(&tmp103->lock);

	status = sysfs_create_group(&client->dev.kobj, &tmp103_attr_group);
	if (status) {
		dev_dbg(&client->dev, "could not create sysfs files\n");
		goto fail_free;
	}
	tmp103->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(tmp103->hwmon_dev)) {
		dev_dbg(&client->dev, "unable to register hwmon device\n");
		status = PTR_ERR(tmp103->hwmon_dev);
		goto fail_remove_sysfs;
	}

	return 0;

fail_remove_sysfs:
	sysfs_remove_group(&client->dev.kobj, &tmp103_attr_group);

fail_free:
	kfree(tmp103);

	return status;
}

static int __devexit tmp103_remove(struct i2c_client *client)
{
	struct tmp103 *tmp103 = i2c_get_clientdata(client);

	hwmon_device_unregister(tmp103->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &tmp103_attr_group);
	kfree(tmp103);

	return 0;
}

static const struct i2c_device_id tmp103_id[] = {
	{ "tmp103", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp103_id);

static struct i2c_driver tmp103_driver = {
	.driver = {
		.name	= DRIVER_NAME,
	},
	.probe		= tmp103_probe,
	.remove		= __devexit_p(tmp103_remove),
	.id_table	= tmp103_id,
};

static int __init tmp103_init(void)
{
	return i2c_add_driver(&tmp103_driver);
}
module_init(tmp103_init);

static void __exit tmp103_exit(void)
{
	i2c_del_driver(&tmp103_driver);
}
module_exit(tmp103_exit);

MODULE_AUTHOR("UDWORKs");
MODULE_DESCRIPTION("Texas Instruments TMP103 temperature sensor driver");
MODULE_LICENSE("GPL");
