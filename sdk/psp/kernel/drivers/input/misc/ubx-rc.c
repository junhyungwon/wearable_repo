/*
 * Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * UBX IR remotes via linux kernel input layer.	 
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/platform_device.h>

#include <linux/input/ubx-rc.h>
#include <media/ir-common.h>
#include <asm/gpio.h>

#define MODULE_NAME		"ubx-rc"

/* On NEC protocol, One has 2.25 ms, and zero has 1.125 ms
   The first pulse (start) has 9 + 4.5 ms */ 
static irqreturn_t ubx_rc_irq_handler(int irq, void *dev_id)
{
	struct card_ir *ir = (struct card_ir *)dev_id;
	enum raw_event_type type = IR_SPACE;
	int val;
	int rc = 0;
	
	/* read gpio value(active_low) */
	val = !gpio_get_value_cansleep(ir->nec_gpio);
	if (val < 0)
		goto err_get_value;
	
	if (val == 1)
		type = IR_PULSE;
	
	rc = ir_raw_event_store_edge(ir->dev, type);
	if (rc < 0)
		goto err_get_value;

	ir_raw_event_handle(ir->dev);

err_get_value:	
	return IRQ_HANDLED;
}

static int __devinit ubx_rc_probe(struct platform_device *pdev)
{
	struct ubx_rc_platform_data *pdata = pdev->dev.platform_data;
	struct card_ir *ir;
	struct input_dev *input_dev;
	
	char *ir_codes = NULL;
	u64 ir_type = IR_TYPE_OTHER;
	
	int err = 0, irq;
	
	if (!pdata) {
		dev_err(&pdev->dev, "no platform data\n");
		return -ENODEV;
	}
	
	ir = kzalloc(sizeof(*ir), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!ir || !input_dev) {
		return -ENOMEM;
	}
	
	ir_codes = RC_MAP_UBX_NEC;
	/* init ir stuff */
	ir->dev = input_dev;
	ir->nec_gpio	 = pdata->gpio;
	ir->raw_decode	 = 1;
	
	/* init input device */
	snprintf(ir->name, sizeof(ir->name), MODULE_NAME);
	snprintf(ir->phys, sizeof(ir->phys), MODULE_NAME "/input2");
	
	ir->props.driver_type = RC_DRIVER_IR_RAW;
	ir->props.allowed_protos = IR_TYPE_NEC;
	
	err = ir_input_init(input_dev, &ir->ir, ir_type);
	if (err < 0)
		goto err_out_free;
	
	platform_set_drvdata(pdev, ir);
	
	input_dev->name = ir->name;
	input_dev->phys = ir->phys;
	
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 1;
	input_dev->dev.parent = &pdev->dev;
	
	//# gpio setup
	err = gpio_request(ir->nec_gpio, "gpio_ir");
	if (err) {
		dev_err(&pdev->dev, "fail %d request\n", ir->nec_gpio);
		goto err_out_free;
	}
	gpio_direction_input(ir->nec_gpio);
	
	irq = gpio_to_irq(ir->nec_gpio);
	err = request_threaded_irq(irq,
				  NULL,
				  ubx_rc_irq_handler,
				  IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
				  MODULE_NAME, 
				  ir);
	if (err) {
		dev_err(&pdev->dev, "Unable to IR-GPIO IRQ\n");
		goto err_out_gpio;
	}
	
	/* all done */
	err = ir_input_register(ir->dev, ir_codes, &ir->props, MODULE_NAME);
	if (err)
		goto err_out_irq;

	return 0;

err_out_irq:
	free_irq(irq, ir);
	
err_out_gpio:
	gpio_free(ir->nec_gpio);
	
err_out_free:
	kfree(ir);
	platform_set_drvdata(pdev, NULL);
	return err;	
}


static int __devexit ubx_rc_remove(struct platform_device *pdev)
{
	struct card_ir *ir = platform_get_drvdata(pdev);
	struct input_dev *input_dev = ir->dev;
	int irq;
	
	irq = gpio_to_irq(ir->nec_gpio);
	free_irq(irq, ir);
	gpio_free(ir->nec_gpio);
	
	ir_input_unregister(input_dev);
	
	return 0;
}

static struct platform_driver ubx_rc_driver = {
	.probe		= ubx_rc_probe,
	.remove		= __devexit_p(ubx_rc_remove),
	.driver		= {
		.name	= MODULE_NAME,
		.owner	= THIS_MODULE,
	}
};

static int __init ubx_rc_init(void)
{
	return platform_driver_register(&ubx_rc_driver);
}
module_init(ubx_rc_init);

static void __exit ubx_rc_exit(void)
{
	platform_driver_unregister(&ubx_rc_driver);
}
module_exit(ubx_rc_exit);

MODULE_AUTHOR("UDWORKs");
MODULE_DESCRIPTION("UBX Remote Controller driver");
MODULE_LICENSE("GPL");

