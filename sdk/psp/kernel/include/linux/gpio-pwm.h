/*
 * include/linux/gpio-pwm.h
 *
 * Copyright (C) 2008 Bill Gatliff < bgat@billgatliff.com>
 *
 * This program is free software; you may redistribute and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef __LINUX_GPIO_PWM_H__
#define __LINUX_GPIO_PWM_H__

enum {
	PWM_CONFIG_DUTY_TICKS 	= BIT(0),
	PWM_CONFIG_PERIOD_TICKS = BIT(1),
	PWM_CONFIG_POLARITY 	= BIT(2),
	PWM_CONFIG_START 		= BIT(3),
	PWM_CONFIG_STOP 		= BIT(4),
	PWM_CONFIG_HANDLER 		= BIT(5),
	PWM_CONFIG_DUTY_NS 		= BIT(6),
	PWM_CONFIG_DUTY_PERCENT = BIT(7),
	PWM_CONFIG_PERIOD_NS 	= BIT(8),
};

struct gpio_pwm_channel;
struct work_struct;

typedef int (*gpio_pwm_handler_t)(struct gpio_pwm_channel *p, void *data);
typedef void (*gpio_pwm_callback_t)(struct gpio_pwm_channel *p);

struct gpio_pwm_channel_config {
	int config_mask;
	unsigned long duty_ticks;
	unsigned long period_ticks;
	int polarity;

	gpio_pwm_handler_t handler;

	unsigned long duty_ns;
	unsigned long period_ns;
	int duty_percent;
};

struct gpio_pwm_device {
	struct list_head list;
	spinlock_t list_lock;

	struct device *dev;
	struct module *owner;
	struct gpio_pwm_channel *channels;

	const char *bus_id;
	int nchan;

	int	 (*request)(struct gpio_pwm_channel *p);
	void (*free)(struct gpio_pwm_channel *p);
	int  (*config)(struct gpio_pwm_channel *p,
	     	       		struct gpio_pwm_channel_config *c);
	int  (*config_nosleep)(struct gpio_pwm_channel *p,
	                    struct gpio_pwm_channel_config *c);
	int  (*synchronize)  (struct gpio_pwm_channel *p,
	                    struct gpio_pwm_channel *to_p);
	int  (*unsynchronize)(struct gpio_pwm_channel *p,
	                    struct gpio_pwm_channel *from_p);
	int  (*set_callback) (struct gpio_pwm_channel *p,
	                    gpio_pwm_callback_t callback);
};

int gpio_pwm_register(struct gpio_pwm_device *pwm);
int gpio_pwm_unregister(struct gpio_pwm_device *pwm);

enum {
	FLAG_REQUESTED = 0,
    FLAG_STOP = 1,
};

struct gpio_pwm_channel {
	struct list_head list;
	struct gpio_pwm_device *pwm;
	const char *requester;
	pid_t pid;

	int chan;
	unsigned long flags;
	unsigned long tick_hz;
	spinlock_t lock;

	struct completion complete;
	gpio_pwm_callback_t callback;

	struct work_struct handler_work;
	gpio_pwm_handler_t handler;
	void *handler_data;

	int active_high;
	unsigned long period_ticks;
	unsigned long duty_ticks;
};

struct gpio_pwm_platform_data {
	int gpio;
};

struct gpio_pwm_channel *gpio_pwm_request(const char *bus_id, int chan,
            const char *requester);

void gpio_pwm_free(struct gpio_pwm_channel *pwm);

int gpio_pwm_config_nosleep(struct gpio_pwm_channel *pwm,
                       struct gpio_pwm_channel_config *c);

int gpio_pwm_config(struct gpio_pwm_channel *pwm,
               struct gpio_pwm_channel_config *c);

unsigned long gpio_pwm_ns_to_ticks(struct gpio_pwm_channel *pwm,
                              unsigned long nsecs);
unsigned long gpio_pwm_ticks_to_ns(struct gpio_pwm_channel *pwm,
                              unsigned long ticks);
int gpio_pwm_set_period_ns(struct gpio_pwm_channel *pwm,
                      unsigned long period_ns);
unsigned long int gpio_pwm_get_period_ns(struct gpio_pwm_channel *pwm);
int gpio_pwm_set_duty_ns(struct gpio_pwm_channel *pwm,
                    unsigned long duty_ns);
int gpio_pwm_set_duty_percent(struct gpio_pwm_channel *pwm,
                         int percent);
unsigned long gpio_pwm_get_duty_ns(struct gpio_pwm_channel *pwm);
int gpio_pwm_set_polarity(struct gpio_pwm_channel *pwm,
                     int active_high);
int gpio_pwm_start(struct gpio_pwm_channel *pwm);
int gpio_pwm_stop(struct gpio_pwm_channel *pwm);
int gpio_pwm_set_handler(struct gpio_pwm_channel *pwm,
                    gpio_pwm_handler_t handler,
                    void *data);
int gpio_pwm_synchronize(struct gpio_pwm_channel *p,
                    struct gpio_pwm_channel *to_p);
int gpio_pwm_unsynchronize(struct gpio_pwm_channel *p,
                      struct gpio_pwm_channel *from_p);

#endif /* __LINUX_GPIO_PWM_H__ */
