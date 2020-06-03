/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#ifndef _PWM_H_
#define _PWM_H_

/* register offsets with the write pending bit encoded */
#define	WPSHIFT			16
#define	WP_NONE			0	/* no write pending bit */
#define	WP_TCLR			(1 << 0)
#define	WP_TCRR			(1 << 1)
#define	WP_TLDR			(1 << 2)
#define	WP_TTGR			(1 << 3)
#define	WP_TMAR			(1 << 4)
#define	WP_TPIR			(1 << 5)
#define	WP_TNIR			(1 << 6)
#define	WP_TCVR			(1 << 7)
#define	WP_TOCR			(1 << 8)
#define	WP_TOWR			(1 << 9)

#define PWM_ID_REG					 (0x00 | (WP_NONE << WPSHIFT))
#define PWM_OCP_CFG_REG				 (0x10 | (WP_NONE << WPSHIFT))
#define PWM_STAT_REG				 (0x28 | (WP_NONE << WPSHIFT))
#define PWM_INT_EN_REG				 (0x2c | (WP_NONE << WPSHIFT))
#define PWM_WAKEUP_EN_REG			 (0x34 | (WP_NONE << WPSHIFT))
#define PWM_CTRL_REG				 (0x38 | (WP_TCLR << WPSHIFT))
#define PWM_COUNTER_REG				 (0x3c | (WP_TCRR << WPSHIFT))
#define PWM_LOAD_REG				 (0x40 | (WP_TLDR << WPSHIFT))
#define PWM_TRIGGER_REG				 (0x44 | (WP_TTGR << WPSHIFT))
#define PWM_WRITE_PEND_REG			 (0x48 | (WP_NONE << WPSHIFT))
#define PWM_MATCH_REG			 	(0x4c | (WP_TMAR << WPSHIFT))
#define PWM_CAPTURE_REG		 		(0x50 | (WP_NONE << WPSHIFT))
#define PWM_IF_CTRL_REG		 		(0x54 | (WP_NONE << WPSHIFT))
#define	PWM_CAPTURE2_REG		 	(0x58 | (WP_NONE << WPSHIFT))
#define PWM_IRQ_EOI_REG		 		(0x20 | (WP_NONE << WPSHIFT))
#define PWM_IRQSTATUS_RAW_REG	 	(0x24 | (WP_NONE << WPSHIFT))
#define PWM_IRQSTATUS_CLR_REG	 	(0x30 | (WP_NONE << WPSHIFT))

int pwm_enable(void);
int pwm_init(int id);
int pwm_config(int duty_ns, int period_ns);
void pwm_disable(void);

#endif /* _PWM_H_ */
