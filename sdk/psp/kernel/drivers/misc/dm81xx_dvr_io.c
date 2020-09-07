/******************************************************************************
 * TI81XX DVR Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *-----------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *---------------------------------------------------------------------------*/
 /**
 * @file	dm81xx_dvr_io.c
 * @brief
 * @author
 * @section	MODIFY history
 *     - 2011.05.25 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>			//# fops
#include <linux/miscdevice.h>	//# misc_register
#include <linux/uaccess.h>		//# copy_from_user
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/mutex.h>
#include <linux/sched.h>		//# waitqueue
#include <linux/proc_fs.h>		//# kalloc
#include <linux/poll.h>
#include <linux/list.h>
#include <linux/kfifo.h>
#include <linux/bitops.h>

#include <linux/dm81xx_dvr_io.h>

#include <mach/gpio.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static struct dev_dvrio_t dvrio = {0};
static DEFINE_MUTEX(io_mutex);

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
#define eprintk(x...) printk("dvrio-ERR: " x);
//#define dprintk(x...) printk("dvrio: " x);
#define dprintk(x...)

/*----------------------------------------------------------------------------
 Local function
-----------------------------------------------------------------------------*/
static void dvrio_check_io(unsigned long data)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)data;

	dprintk("dvrio_check_io\n");

	wake_up_interruptible(&dev->wait);
}

static irqreturn_t dvrio_isr(int irq, void *dev_id)
{
	dvrio_t *dev_io = (dvrio_t *)dev_id;
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)&dvrio;
	
	gpio_event_t  event;

	event.gpio = dev_io->gpio;
	event.val = !!gpio_get_value(dev_io->gpio) ^ dev_io->active_low;

	do_gettimeofday(&event.time);
	kfifo_in(&dev->irq_fifo, &event, sizeof(event));

	dprintk("dvrio_isr irq 0x%x, gpio %d\n", irq, dev_io->gpio);

	if (dev->debounce_interval) {
		mod_timer(&dev->timer, 
			jiffies + msecs_to_jiffies(dev->debounce_interval));
	} else
		wake_up_interruptible(&dev->wait);

	return IRQ_HANDLED;
}

/*****************************************************************************
* @brief	dvrio function
* @section	DESC Description
*	- desc
*****************************************************************************/
static ssize_t dvrio_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	dvrio_t io;

	if (copy_from_user(&io, (void __user *)buf, sizeof(dvrio_t)))
		return -EINVAL;

	if (io.trigger)
		io.val = !io.val;

	gpio_set_value(io.gpio, io.val);

	return count;
}

static ssize_t dvrio_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)file->private_data;
	gpio_event_t event;

	int i, res = 0;

	if (count < sizeof(gpio_event_t))
		return -EINVAL;

	wait_event_interruptible(dev->wait, 
						(kfifo_len(&dev->irq_fifo) > 0));

	if (mutex_lock_interruptible(&io_mutex))
		return -EINTR;

	for (i = 0; i < (count/sizeof(gpio_event_t)); i++) {
		if (kfifo_out(&dev->irq_fifo, &event, sizeof(gpio_event_t))) {
			if (copy_to_user(buf + (i*sizeof(gpio_event_t)), 
							&event, sizeof(gpio_event_t))) {
				mutex_unlock(&io_mutex);
				return -EFAULT;
			}
			res++;
		} 
		else 
			break;
	}
	mutex_unlock(&io_mutex);

	dprintk("dvrio_read\n");

	return (res*sizeof(gpio_event_t));
}

static long dvrio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)file->private_data;
	struct list_head *pos, *q;
	unsigned long irq_flags;

	dvrio_t io;
	gpio_list_t *io_list = NULL;

	int ret = 0, irq;

	if (copy_from_user(&io, (void __user *)arg, sizeof(dvrio_t))) {
		eprintk("[%s, %d] Failed to get user data\n", __func__, __LINE__);
		return -EFAULT;
	}

	dprintk("dvrio_ioctl cmd(%d), gpio(%d)\n", cmd, io.gpio);

	switch (cmd) {
	case DVRIO_CMD_INIT:
	{
		ret = gpio_request(io.gpio, "dvrio");
		if (ret) {
			eprintk("Failed to request GPIO [%d]\n", io.gpio);
			return ret;
		}

		if (io.dir) {
			/* set only direction */
			gpio_direction_output(io.gpio, io.val);
			return 0;
		}

		/* allocation for gpio linked-list */
		io_list = kzalloc(sizeof(gpio_list_t), GFP_KERNEL);
		if (!io_list) {
			eprintk("%s: couldn't allocate gpio data\n", __func__);
			gpio_free(io.gpio);
			return -ENOMEM;
		}

		INIT_LIST_HEAD(&io_list->list);
		io_list->io = io;
		gpio_direction_input(io.gpio);

		/* requested gpio irq */
		if (io_list->io.trigger)
		{
			irq = gpio_to_irq(io.gpio);
			if (irq < 0) {
				eprintk("%s: Unable to get irq number %d\n", __func__, io.gpio);
				kfree(io_list);
				gpio_free(io.gpio);
				return -EINVAL;
			}
			
			irq_flags = IRQF_SHARED;
			if (test_bit(0, (unsigned long *)&io_list->io.trigger))
				irq_flags |= IRQF_TRIGGER_RISING;
			if (test_bit(1, (unsigned long *)&io_list->io.trigger))
				irq_flags |= IRQF_TRIGGER_FALLING;
				
			ret = request_irq(irq, dvrio_isr, irq_flags,
					"dvr_io", (void *)&io_list->io);
			if (ret < 0) {
				eprintk("%s: gpio request irq failed\n", __func__);
				kfree(io_list);
				gpio_free(io.gpio);
				return ret;
			}
		}
		list_add(&io_list->list, &dev->irq_list.list);
	}
	break;
	case DVRIO_CMD_DEINIT:
	{
		if (io.dir) {
			gpio_free(io.gpio);
			return 0;
		}

		list_for_each_safe(pos, q, &dev->irq_list.list) {
			io_list = list_entry(pos, gpio_list_t, list);
			if (io_list->io.gpio == io.gpio) {
				if (io_list->io.trigger) {
					irq = gpio_to_irq(io_list->io.gpio);
					dprintk("io.gpio %d(%d)\n", io.gpio, irq);
					if (irq > 0)
						free_irq(irq, &io_list->io.gpio);
					else
						eprintk("Unable to get irq number %d\n", io.gpio);
				}
				gpio_free(io_list->io.gpio);
				list_del(pos);
				kfree(io_list);
			}
		}
	}
	break;
	case DVRIO_CMD_CHG_IRQ:
	{
		list_for_each(pos, &dev->irq_list.list) {
			io_list = list_entry(pos, gpio_list_t, list);
			if (io_list->io.gpio == io.gpio) {
				if (io_list->io.trigger) {
					irq = gpio_to_irq(io_list->io.gpio);
					dprintk("io.gpio %d(%d)\n", io.gpio, irq);
					if (irq > 0) {
						free_irq(irq, &io_list->io.gpio);
						io_list->io = io;
						
						irq_flags = IRQF_SHARED;
						if (test_bit(0, (unsigned long *)&io_list->io.trigger))
							irq_flags |= IRQF_TRIGGER_RISING;
						if (test_bit(1, (unsigned long *)&io_list->io.trigger))
							irq_flags |= IRQF_TRIGGER_FALLING;

						ret = request_irq(irq, dvrio_isr, irq_flags,
							"dvr_io", (void *)&io_list->io);
						if (ret < 0) {
							eprintk("gpio request irq failed %d\n", ret);
							return -EFAULT;
						}
					} else
						eprintk("Unable to get irq number %d\n", io.gpio);
				}
			}
		}
	}
	break;
	case DVRIO_CMD_RD:
	{
		io.val = (gpio_get_value(io.gpio) ^ io.active_low);
		if (copy_to_user((void __user *)arg, &io, sizeof(dvrio_t))) {
			return -EFAULT;
		}
	}
	break;
	default:
		eprintk("%s: Invalid ioctl %x\n", __func__, cmd);
		return -EFAULT;
	}

	return 0;
}

static unsigned int dvrio_poll(struct file *file, poll_table *wait)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)file->private_data;
	unsigned int mask = 0;

	poll_wait(file, &dev->wait, wait);

	if (mutex_lock_interruptible(&io_mutex))
		return -EINTR;

	if (kfifo_len(&dev->irq_fifo) > 0)
		mask = POLLIN | POLLRDNORM;

	mutex_unlock(&io_mutex);

	return mask;
}

static int dvrio_open(struct inode *inode, struct file *file)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)&dvrio;
	int ret = 0;

	if (dev->open) {
		printk(KERN_ERR "DVRIO already enabled!!\n");
		return -EBUSY;
	}

	mutex_lock(&io_mutex);
	ret = kfifo_alloc(&dev->irq_fifo, IRQ_FIFO_SIZE, GFP_KERNEL);
	if (ret) {
		printk(KERN_ERR "[%s,%d]: error kfifo_alloc\n", __func__, __LINE__);
		goto out;
	}
	file->private_data = dev;

	/* opening will always block */
	if (file->f_flags & O_NONBLOCK) {
		ret = -EAGAIN;
		goto out;
	}

	/* init device struct */
	dev->irq_cnt = 0;
	dev->debounce_interval = DBOUNCE_TIME;

	setup_timer(&dev->timer, dvrio_check_io, (unsigned long)dev);
	init_waitqueue_head(&dev->wait);

	INIT_LIST_HEAD(&dev->irq_list.list);
	dev->open = 1;	/* only one open per device */

	ret = nonseekable_open(inode, file);
	dprintk("dvrio_open!!\n");

out:
	mutex_unlock(&io_mutex);
	return ret;
}

static int dvrio_release(struct inode *inode, struct file *file)
{
	struct dev_dvrio_t *dev = (struct dev_dvrio_t *)file->private_data;
	struct list_head *pos, *q;
	gpio_list_t *tmp_io;

	int irq = 0;

	list_for_each_safe(pos, q, &dev->irq_list.list) {
		tmp_io = list_entry(pos, gpio_list_t, list);
		if (tmp_io->io.trigger) {
			irq = gpio_to_irq(tmp_io->io.gpio);
			dprintk("io.gpio %d(%d)\n", tmp_io->io.gpio, irq);
			if (irq > 0)
				free_irq(irq, &tmp_io->io.gpio);
			else
				eprintk("Unable to get irq number %d\n", tmp_io->io.gpio);
		}

		gpio_free(tmp_io->io.gpio);
		list_del(pos);
		kfree(tmp_io);
	}
	kfifo_free(&dev->irq_fifo);
	dev->open = 0;		/* only one open per device */

	dprintk("dvrio_release\n");

	return 0;
}

static struct file_operations dvrio_fops = {
	.owner		= THIS_MODULE,
	.llseek 	= no_llseek,
	.open		= dvrio_open,
	.release	= dvrio_release,
	.poll		= dvrio_poll,
	.write		= dvrio_write,
	.read		= dvrio_read,
	.unlocked_ioctl	= dvrio_ioctl,
};

static struct miscdevice dvrio_misc = {
	.fops   = &dvrio_fops,
	.minor  = DVRIO_MINOR,
	.name   = "dvr_io",
};

/*****************************************************************************
* @brief	dvr_netra gpio module init & exit function
* @section	DESC Description
*	- desc
*****************************************************************************/
static int __init dvr_io_init(void)
{
	return misc_register(&dvrio_misc);
}

static void __exit dvr_io_exit(void)
{
	misc_deregister(&dvrio_misc);
}

module_init(dvr_io_init)
module_exit(dvr_io_exit)

MODULE_AUTHOR("UDWorks");
MODULE_DESCRIPTION("DVR_NETRA GPIO Driver");
MODULE_LICENSE("GPL");
