/*
 * Copyright (C) 2009 Texas Instruments
 * Added OMAP4 support - Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <asm/io.h>

#include <asm/arch/hardware.h>
#include <asm/arch/cpu.h>
#include <asm/arch/pwm.h>
#include <div64.h>

#ifdef DEBUG
#define PWM_DBG(fmt, arg...) \
	printf("%s (line:%d) :" fmt "\n", __func__, __LINE__, ## arg)
#else
#define PWM_DBG(fmt, arg...) do {} while (0)
#endif

#define	TIMER_CTRL_GPOCFG			(1 << 14)
#define	TIMER_CTRL_CAPTMODE			(1 << 13)
#define	TIMER_CTRL_PT				(1 << 12)
#define	TIMER_CTRL_TCM_LOWTOHIGH	(0x1 << 8)
#define	TIMER_CTRL_TCM_HIGHTOLOW	(0x2 << 8)
#define	TIMER_CTRL_TCM_BOTHEDGES	(0x3 << 8)
#define	TIMER_CTRL_SCPWM			(1 << 7)
#define	TIMER_CTRL_CE				(1 << 6) /* compare enable */
#define	TIMER_CTRL_PRE				(1 << 5) /* prescaler enable */
#define	TIMER_CTRL_PTV_SHIFT		2 /* prescaler value shift */
#define	TIMER_CTRL_POSTED			(1 << 2)
#define	TIMER_CTRL_AR				(1 << 1) /* auto-reload enable */
#define	TIMER_CTRL_ST				(1 << 0) /* start timer */

/* timer interrupt enable bits */
#define TIMER_INT_CAPTURE			(1 << 2)
#define TIMER_INT_OVERFLOW			(1 << 1)
#define TIMER_INT_MATCH				(1 << 0)

/* trigger types */
#define TIMER_TRIGGER_NONE			0x00
#define TIMER_TRIGGER_OVERFLOW		0x01
#define TIMER_TRIGGER_OVERFLOW_AND_COMPARE	0x02

#define DM_TIMER_LOAD_MIN			0xFFFFFFFE

/* macro to read the 32 bit timer */
#define PWM_CLOCK_RATE				32768L //# 32.768KHz
#define PWM_REG(offset)				(CONFIG_SYS_PWM_TIMER_BASE + offset)

static u32 pwm_base[] = {
	0x4802E000, /* TIMER_1 */
	0x48040000, /* TIMER_2 */
	0x48042000, /* TIMER_3 */
	0x48044000, /* TIMER_4 */
	0x48046000, /* TIMER_5 */
	0x48048000, /* TIMER_6 */
	0x4804A000, /* TIMER_7 */
};

static u32 io_base = 0;

static inline u32 pwm_read_reg(u32 offset)
{
	u32 mask = offset >> WPSHIFT;
	
	while (readl(io_base + (PWM_WRITE_PEND_REG & 0xff)) & mask)
	;
	
	return readl(io_base + (offset & 0xff));
}

/*
 * Writes timer registers in posted and non-posted mode. The posted mode bit
 * is encoded in reg. Note that in posted mode the write pending bit must be
 * checked. Otherwise a write on a register which has a pending write will be
 * lost.
 */
static void pwm_write_reg(u32 offset, u32 value)
{
	u32 mask = offset >> WPSHIFT;
	
	while (readl(io_base + (PWM_WRITE_PEND_REG & 0xff)) & mask)
	;
	
	writel(value, io_base + (offset & 0xff));
}

static inline int pwm_calc_value(unsigned long clk_rate, int ns)
{
	const unsigned long nanoseconds_per_second = 1000000000;
	int cycles;
	__u64 c;

	c = (__u64)clk_rate * ns;
	do_div(c, nanoseconds_per_second);
	cycles = c;

	return DM_TIMER_LOAD_MIN - cycles;
}

int pwm_enable(void)
{
	u32 l;
	
	pwm_write_reg(PWM_COUNTER_REG, DM_TIMER_LOAD_MIN);
	
	/* timer start */
	l = pwm_read_reg(PWM_CTRL_REG);
	l |= TIMER_CTRL_ST;
	pwm_write_reg(PWM_CTRL_REG, l);
	
	return 0;
}

void pwm_disable(void)
{
	u32 l;
	
	/* timer stop */
	l = pwm_read_reg(PWM_CTRL_REG);
	if (l & TIMER_CTRL_ST) {
		l &= ~0x1;
		pwm_write_reg(PWM_CTRL_REG, l);
	}
	
	/* Ack possibly pending interrupt */
	pwm_write_reg(PWM_STAT_REG, TIMER_INT_OVERFLOW);
}

int pwm_config(int duty_ns, int period_ns)
{
	unsigned long clk_rate;
	int load_value, match_value;
	
	u32 l;
	
	clk_rate = PWM_CLOCK_RATE;
	
	/* Calculate the appropriate load and match values based on the
	 * specified period and duty cycle. The load value determines the
	 * cycle time and the match value determines the duty cycle.
	 */
	load_value = pwm_calc_value(clk_rate, period_ns);
	match_value = pwm_calc_value(clk_rate, period_ns - duty_ns);
	PWM_DBG("load value: %#08x (%d), match value: %#08x (%d)\n",	
			load_value, load_value, match_value, match_value);

	/* timer stop */
	l = pwm_read_reg(PWM_CTRL_REG);
	if (l & TIMER_CTRL_ST) {
		l &= ~0x1;
		pwm_write_reg(PWM_CTRL_REG, l);
	}
	/* Ack possibly pending interrupt */
	pwm_write_reg(PWM_STAT_REG, TIMER_INT_OVERFLOW);
	
	/* auto reload */
	l = pwm_read_reg(PWM_CTRL_REG);
	l |= TIMER_CTRL_AR;
	pwm_write_reg(PWM_CTRL_REG, l);
	pwm_write_reg(PWM_LOAD_REG, load_value);
	pwm_write_reg(PWM_TRIGGER_REG, 0);
	
	/* set match */
	l = pwm_read_reg(PWM_CTRL_REG);
	l |= TIMER_CTRL_CE;
	pwm_write_reg(PWM_CTRL_REG, l);
	pwm_write_reg(PWM_MATCH_REG, match_value);
	
	/* set pwm */
	l = pwm_read_reg(PWM_CTRL_REG);
	l &= ~(TIMER_CTRL_GPOCFG | TIMER_CTRL_SCPWM |
	       TIMER_CTRL_PT | (0x03 << 10));
	l |= TIMER_CTRL_PT;
	l |= TIMER_TRIGGER_OVERFLOW_AND_COMPARE << 10;
	pwm_write_reg(PWM_CTRL_REG, l);
	
	/* Set the counter to generate an overflow event immediately. */
	pwm_write_reg(PWM_COUNTER_REG, DM_TIMER_LOAD_MIN);
	
	return 0;
}

int pwm_init(int id)
{
	u32 val = 0;
	u32 shift, mask;
	
	mask = 0x07L;
	
	if (id <= 0 || id > (sizeof(pwm_base) / sizeof(u32))) {
		printf("BUG: unable to get timer %d\n", id);
		return -1;
	}
	
	/* set TIMER clk source */
	if (id < 4) 
		shift = id*3;	
	else
		shift = (id*3) + 4;
		
	val = readl(PLL_SUBSYS_BASE + 0x02E0);
	val &= ~(mask << shift); /* select SYSCLK18 */
		
	writel(val, PLL_SUBSYS_BASE + 0x02E0);
	
	io_base = pwm_base[id-1];
	
	return 0;
}
	
	


