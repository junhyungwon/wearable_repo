/*
 *	adc081.c - lm_sensors driver for adc081c021/adc081c027 10-bit ADC
 *	Based on the ads7828.c

 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.

 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 *	GNU General Public License for more details.

 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/adc081.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define	DRIVER_NAME					"adc081c"

#define ADC_RESOL			256
#define VREF_MV 			3300 /* Internal vref is 3.3V, 3300mV */

/* The ADC081C registers (Pointer Address) */
#define RESULT 				0x00 /* 2 Bytes */
#define ALERT_STATUS		0x01 /* 1 Byte */
#define CONFIG 				0x02 /* 1 Byte */
#define ALERT_LIMIT_LOW		0x03 /* 2 Bytes */
#define ALERT_LIMIT_HIGH	0x04 /* 2 Bytes */
#define ALERT_HYST			0x05 /* 2 Bytes */

#define ALERT_POLARITY_HIGH			BIT(0)
#define ALERT_PIN_ENABLE			BIT(2)
#define ALERT_FLAG_ENABLE			BIT(3)

#define INTR_CLEAR					0x03

static DEFINE_MUTEX(adc_mutex);

/* Each client has this additional data */
struct adc081_data {
	struct i2c_client *client;
	struct device *hwmon_dev;
	struct delayed_work work; /* work queue */
	
	struct mutex update_lock; /* mutex protect updates */
	
	int debounce_ms;	/* debounce timer */	
	int power_limit;	/* alert limit */
	int interval;     /* update interval */	
	
	u32 vref_mv;
	u32 adc_lsb_resol;
	
	u32 R1; /* Voltage Divider R1 */	
	u32 R2; /* Voltage Divider R2 */
	
	unsigned long misc_opened;
	
	struct list_head list; /* member of the data_list */
	
	wait_queue_head_t	misc_wait;
	struct fasync_struct *async_queue; /* queue for the misc device */
};

/* There is no other method to get to one's device data 
 * from the open file-op and
 * for usage in the reboot notifier callback. 
 */
static LIST_HEAD(adc_data_list);
static struct miscdevice adc081c_misc_device;

static void adc081c_work_func(struct work_struct *work)
{
	struct adc081_data *data =
		container_of(work, struct adc081_data, work.work);
	struct i2c_client *client = data->client;	
	
	s32 ret = 0;
	u32 supply = 0, vin;
	u32 adc_data = 0;
	
	ret = i2c_smbus_read_word_swapped(client, RESULT);
	if (ret < 0) {
		dev_err(&client->dev, "error clearing interrupt status: %d\n", ret);
	} else {
		adc_data = ((ret >> 4) & 0xFF);
	 	dev_dbg(&client->dev, "raw value = 0x%x\n", adc_data);
	}
	
	//# v to mv conversion.
	vin  = (adc_data * data->adc_lsb_resol) / 1000;
	supply  = (vin * (data->R1 + data->R2)) / data->R2;
	
	if (supply < data->power_limit) {
		/* disable adc */
		dev_dbg(&client->dev, "supply voltage is %d mV %d mV\n", supply, data->power_limit);
		
		i2c_smbus_write_byte_data(data->client, CONFIG, 0x00);
		wake_up_interruptible(&data->misc_wait);
		kill_fasync(&data->async_queue, SIGIO, POLL_IN);
	} else 
		i2c_smbus_write_byte_data(client, ALERT_STATUS, INTR_CLEAR); //#clear
	
	dev_dbg(&client->dev, "supply voltage is %d mV\n", supply);
}

static irqreturn_t adc081_isr(int irq, void *dev)
{
	struct adc081_data *data = (struct adc081_data *)dev;
	u32 time = data->debounce_ms;
	
	if (!test_bit(0, &data->misc_opened))
		goto out;
		
	schedule_delayed_work(&data->work, msecs_to_jiffies(time));
	
out:		
	return IRQ_HANDLED;
}

static int adc081c_misc_open(struct inode *inode, struct file *file)
{
	struct adc081_data *pos, *data = NULL;
	unsigned short min;
	
	list_for_each_entry(pos, &adc_data_list, list) {
		if (adc081c_misc_device.minor == iminor(inode)) {
			data = pos;
			break;
		}
	}
	
	if (test_and_set_bit(0, &data->misc_opened))
		return -EBUSY; /* already open */

	/* Store pointer to data into filp's private data */
	file->private_data = data;
	/* Initialize adc  */
	min = (data->power_limit * data->R2 * ADC_RESOL)
			/((data->R1 + data->R2) * VREF_MV);
			
	i2c_smbus_write_word_swapped(data->client, ALERT_LIMIT_LOW, min << 2);
	i2c_smbus_write_byte_data(data->client, CONFIG, 
						ALERT_PIN_ENABLE | data->interval);

	return nonseekable_open(inode, file);
}

static int adc081c_misc_release(struct inode *inode, struct file *filp)
{
	struct adc081_data *data = filp->private_data;

	i2c_smbus_write_byte_data(data->client, CONFIG, 0x00);
	clear_bit(0, &data->misc_opened);

	return 0;
}

static ssize_t adc081c_misc_read(struct file *file, char __user *buf,
				size_t count, loff_t *pos)
{
	struct adc081_data *data = file->private_data;
	struct i2c_client *client = data->client;
	unsigned short adc_data;
	ssize_t retval = 1;
	int rv = 0;
	u32 supply = 0, vin;
	
	/* need to unsigned long */
	if (count < 1)
		return -EINVAL;
		
	rv = i2c_smbus_read_word_swapped(client, RESULT);
	if (rv < 0) {
		dev_err(&data->client->dev, "error clearing interrupt status: %d\n", rv);
		return -EINVAL;
	} 
	
	if (mutex_lock_interruptible(&data->update_lock)) 
		return -ERESTARTSYS;

	adc_data = ((rv >> 4) & 0xff); 
	dev_dbg(&client->dev, "raw value = 0x%x\n", adc_data);
	
	//# v to mv conversion.
	vin  = (adc_data * data->adc_lsb_resol) / 1000;
	supply  = (vin * (data->R1 + data->R2)) / data->R2;
	
	dev_dbg(&client->dev, "input voltage is %d mV\n", supply);
	
	if (copy_to_user(buf, &supply, sizeof(supply))) 
		retval = -EFAULT;
	
	mutex_unlock(&data->update_lock);
	
	return retval;
}

static long adc081c_misc_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	struct adc081_data *data = file->private_data;
	int val, ret = 0;

	mutex_lock(&adc_mutex);
	switch (cmd) {
	case ADCIOC_GETDEBOUNCE:
		val = data->debounce_ms;
		ret = put_user(val, (int __user *)arg);
		break;

	case ADCIOC_GETLEVEL:
		val = data->power_limit;
		ret = put_user(val, (int __user *)arg);
		break;

	case ADCIOC_GETINTERVAL:
		val = data->interval;
		ret = put_user(val, (int __user *)arg);
		break;

	case ADCIOC_SETDEBOUNCE:
		if (get_user(val, (int __user *)arg)) {
			ret = -EFAULT;
			break;
		}
		data->debounce_ms = val;
		break;

	case ADCIOC_SETLEVEL:
		if (get_user(val, (int __user *)arg)) {
			ret = -EFAULT;
			break;
		}
		data->power_limit = val;
		break;
	
	case ADCIOC_SETINTERVAL:
		if (get_user(val, (int __user *)arg)) {
			ret = -EFAULT;
			break;
		}
		data->interval = val;
		break;
	
	default:
		ret = -ENOTTY;
	}
	mutex_unlock(&adc_mutex);
	return ret;
}

static int adc081c_misc_fasync(int fd, struct file *file, int on)
{
	struct adc081_data *data = file->private_data;
	
	return fasync_helper(fd, file, on, &data->async_queue);
}

static unsigned int adc081c_misc_poll(struct file *file, poll_table *wait)
{
	struct adc081_data *data = file->private_data;
	
	poll_wait(file, &data->misc_wait, wait);
	
	return (POLLIN | POLLRDNORM);
}

static const struct file_operations adc081c_misc_fops = {
	.owner   = THIS_MODULE,
	.llseek  = no_llseek,
	.read    = adc081c_misc_read,
	.open    = adc081c_misc_open,
	.release = adc081c_misc_release,
	.fasync  = adc081c_misc_fasync,
	.poll    = adc081c_misc_poll,
	.unlocked_ioctl = adc081c_misc_ioctl,
};

static struct miscdevice adc081c_misc_device = {
	.minor   = MISC_DYNAMIC_MINOR,
	.name    = "adc081",
	.fops    = &adc081c_misc_fops,
};

static int adc081c_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct i2c_adapter *adapter = client->adapter;
	const struct adc081_platform_data *pdata = client->dev.platform_data;
	
	struct adc081_data *data;
	int err = 0;
	
	/* Check we have a valid client */
	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C | 
			I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_READ_WORD_DATA)) {
		dev_err(dev, "%s:client is not i2c capable\n", DRIVER_NAME);
		return -ENODEV;
	}
	
	if (!pdata) {
		dev_err(dev, "%s:platform data is NULL\n", DRIVER_NAME);
		return -EINVAL;
	}
	
	if (pdata->init) {
		err = pdata->init();
		if (err < 0)
			return err;
	}
	
	data = kzalloc(sizeof(struct adc081_data), GFP_KERNEL);
	if (!data) {
		dev_err(dev, "failed to allocate memory for %s\n", DRIVER_NAME);
		return -ENOMEM;
	}
	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);
	init_waitqueue_head(&data->misc_wait);
	
	data->vref_mv 	= VREF_MV;
	data->interval 	= pdata->update_interval;
	data->power_limit 	= pdata->power_limit;
	data->R1  = pdata->div_r1;
	data->R2  = pdata->div_r2;
	
	/* Calculate the LSB resolution (in mv) */
	data->adc_lsb_resol = (data->vref_mv * 1000) / ADC_RESOL;
	data->debounce_ms  = 200; //default 200ms
	
	INIT_DELAYED_WORK(&data->work, adc081c_work_func);	
	data->client = client;
	
	if (client->irq) {
		err = request_threaded_irq(client->irq, NULL, 
					   adc081_isr, IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
		   			   "adc081-irq", data);
		if (err) {
			dev_err(dev, "request irq failed: %d\n", err);
			goto err_free_mem;
		}
	}
	
	data->hwmon_dev = hwmon_device_register(dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_irq;
	}
	
	err = misc_register(&adc081c_misc_device);
	if (err) {		
		dev_err(dev, DRIVER_NAME "misc_register failed\n");
		goto exit_irq;
	}
	list_add(&data->list, &adc_data_list);
		
	dev_info(&client->dev, "Registered %s\n", DRIVER_NAME);
	
	return 0;

exit_irq:
	if (client->irq)
		free_irq(client->irq, data);

err_free_mem:
	kfree(data);
	
	return err;
}

static int adc081c_remove(struct i2c_client *client)
{
	struct adc081_data *data = i2c_get_clientdata(client);
	
	misc_deregister(&adc081c_misc_device);
	if (client->irq) {
		free_irq(client->irq, data);
	}
	
	cancel_delayed_work_sync(&data->work);
	list_del(&data->list);
	hwmon_device_unregister(data->hwmon_dev);
	kfree(data);
	
	return 0;
}

static const struct i2c_device_id adc081c_id[] = {
	{ "adc081c", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, adc081c_id);

static struct i2c_driver adc081c_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
	.probe = adc081c_probe,
	.remove = adc081c_remove,
	.id_table = adc081c_id,
};

static int __init adc081c_init(void)
{
	return i2c_add_driver(&adc081c_driver);
}
module_init(adc081c_init);

static void __exit adc081c_exit(void)
{
	i2c_del_driver(&adc081c_driver);
}
module_exit(adc081c_exit);

MODULE_AUTHOR("UDWORKs");
MODULE_DESCRIPTION("ADC081C driver");
MODULE_LICENSE("GPL");
