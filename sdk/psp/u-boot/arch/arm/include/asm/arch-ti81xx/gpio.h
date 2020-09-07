/*
 * Copyright (C) 2011, Texas Instruments, Incorporated
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

#include <asm/omap_gpio.h>

#define TI81XX_GPIO0_BASE       0x48032000
#define TI81XX_GPIO1_BASE       0x4804C000
#define TI81XX_GPIO2_BASE       0x481AC000
#define TI81XX_GPIO3_BASE       0x481AE000

#ifdef CONFIG_GPIO_LED

#define STATUS_LED_OFF		0   /* active low */
#define STATUS_LED_ON		1
#define STATUS_LED_BLINKING	2 

/* led_id_t is unsigned long mask */
typedef unsigned long led_id_t;

extern void __led_toggle (led_id_t mask);
extern void __led_init (led_id_t mask, int state);
extern void __led_set (led_id_t mask, int state);
#endif

#endif
