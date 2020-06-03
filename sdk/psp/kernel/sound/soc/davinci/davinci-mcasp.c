/*
 * ALSA SoC McASP Audio Layer for TI DAVINCI processor
 *
 * Multi-channel Audio Serial Port Driver
 *
 * Author: Nirmal Pandey <n-pandey@ti.com>,
 *         Suresh Rajashekara <suresh.r@ti.com>
 *         Steve Chen <schen@.mvista.com>
 *
 * Copyright:   (C) 2009 MontaVista Software, Inc., <source@mvista.com>
 * Copyright:   (C) 2009  Texas Instruments, India
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/clk.h>

#include <asm/mach-types.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "davinci-pcm.h"
#include "davinci-mcasp.h"

#define DAVINCI_MCASP_NUM_SERIALIZER	16
#define MCASP_MAX_AFIFO_DEPTH			64

struct davinci_audio_dev {
	struct davinci_pcm_dma_params dma_params[2];
	void __iomem *base;
	int sample_rate;
	struct clk *clk;
	unsigned int codec_fmt;
	u8 clk_active;

	/* McASP specific data */
	int	tdm_slots;
	u8	op_mode;
	u8	num_serializer;
	u8	*serial_dir;
	u8	version;
	u16	bclk_lrclk_ratio;
	int	streams;

	int	sysclk_freq;
	bool	bclk_master;

	/* McASP FIFO related */
	u8	txnumevt;
	u8	rxnumevt;

	bool	dat_port;
	/*
	 * codec clock drive from the AHCLKX out from the SoC
	 * This need the McASP pin direction configurations
	 * as out put and the soc pin-mux.This can be used to
	 * configure the McASP driver PIN_DIR
	 */
	int clk_input_pin;
};

static inline void mcasp_set_bits(void __iomem *reg, u32 val)
{
	__raw_writel(__raw_readl(reg) | val, reg);
}

static inline void mcasp_clr_bits(void __iomem *reg, u32 val)
{
	__raw_writel((__raw_readl(reg) & ~(val)), reg);
}

static inline void mcasp_mod_bits(void __iomem *reg, u32 val, u32 mask)
{
	__raw_writel((__raw_readl(reg) & ~mask) | val, reg);
}

static inline void mcasp_set_reg(void __iomem *reg, u32 val)
{
	__raw_writel(val, reg);
}

static inline u32 mcasp_get_reg(void __iomem *reg)
{
	return (unsigned int)__raw_readl(reg);
}

static inline void mcasp_set_ctl_reg(void __iomem *regs, u32 val)
{
	int i = 0;

	mcasp_set_bits(regs, val);

	/* programming GBLCTL needs to read back from GBLCTL and verfiy */
	/* loop count is to avoid the lock-up */
	for (i = 0; i < 1000; i++) {
		if ((mcasp_get_reg(regs) & val) == val)
			break;
	}

	if (i == 1000 && ((mcasp_get_reg(regs) & val) != val))
		printk(KERN_ERR "GBLCTL write error\n");
}

static bool mcasp_is_synchronous(struct davinci_audio_dev *dev)
{
	u32 rxfmctl = mcasp_get_reg(dev->base + DAVINCI_MCASP_RXFMCTL_REG);
	u32 aclkxctl = mcasp_get_reg(dev->base + DAVINCI_MCASP_ACLKXCTL_REG);

	return !(aclkxctl & TX_ASYNC) && rxfmctl & AFSRE;
}

static void mcasp_start_rx(struct davinci_audio_dev *dev)
{
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXHCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXCLKRST);

	/*
	 * When ASYNC == 0 the transmit and receive sections operate
	 * synchronously from the transmit clock and frame sync. We need to make
	 * sure that the TX signlas are enabled when starting reception.
	 */
	if (mcasp_is_synchronous(dev)) {
		mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);
		mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXCLKRST);
	}

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSERCLR);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXFSRST);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, RXFSRST);

	if (mcasp_is_synchronous(dev))
		mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXFSRST);
}

static void mcasp_start_tx(struct davinci_audio_dev *dev)
{
	u8 offset = 0, i;
	u32 cnt;

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXCLKRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXSERCLR);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);

	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXSMRST);
	mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXFSRST);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);
	for (i = 0; i < dev->num_serializer; i++) {
		if (dev->serial_dir[i] == TX_MODE) {
			offset = i;
			break;
		}
	}

	/* wait for TX ready */
	cnt = 0;
	while (!(mcasp_get_reg(dev->base + DAVINCI_MCASP_XRSRCTL_REG(offset)) &
		 TXSTATE) && (cnt < 100000))
		cnt++;

	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXBUF_REG, 0);
}

static void davinci_mcasp_start(struct davinci_audio_dev *dev, int stream)
{
	dev->streams++;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dev->txnumevt) {	/* enable FIFO */
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
								FIFO_ENABLE);
			mcasp_set_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
								FIFO_ENABLE);
		}
		mcasp_start_tx(dev);
	} else {
		if (dev->rxnumevt) {	/* enable FIFO */
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
								FIFO_ENABLE);
			mcasp_set_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
								FIFO_ENABLE);
		}
		mcasp_start_rx(dev);
	}
}

static void mcasp_stop_rx(struct davinci_audio_dev *dev)
{
	/*
	 * In synchronous mode stop the TX clocks if no other stream is
	 * running
	 */
	if (mcasp_is_synchronous(dev) && !dev->streams)
		mcasp_set_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, 0);

	mcasp_set_reg(dev->base + DAVINCI_MCASP_GBLCTLR_REG, 0);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);
}

static void mcasp_stop_tx(struct davinci_audio_dev *dev)
{
	u32 val = 0;

	/*
	 * In synchronous mode keep TX clocks running if the capture stream is
	 * still running.
	 */
	if (mcasp_is_synchronous(dev) && dev->streams)
		val =  TXHCLKRST | TXCLKRST | TXFSRST;

	mcasp_set_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, val);
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);
}

static void davinci_mcasp_stop(struct davinci_audio_dev *dev, int stream)
{
	dev->streams--;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dev->txnumevt)	/* disable FIFO */
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
								FIFO_ENABLE);
		mcasp_stop_tx(dev);
	} else {
		if (dev->rxnumevt)	/* disable FIFO */
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
								FIFO_ENABLE);
		mcasp_stop_rx(dev);
	}
}

static int davinci_mcasp_set_dai_fmt(struct snd_soc_dai *cpu_dai,
					 unsigned int fmt)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	void __iomem *base = dev->base;

	int ret = 0;
	u32 data_delay;
	bool fs_pol_rising;
	bool inv_fs = false;

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_DSP_A:
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* 1st data bit occur one ACLK cycle after the frame sync */
		data_delay = 1;
		break;
	case SND_SOC_DAIFMT_DSP_B:
	case SND_SOC_DAIFMT_AC97:
		/*
		 * configure a full-word SYNC pulse (LRCLK)
		 * FSRDUR-->DAVINCI_MCASP_RXFMCTL_REG.[4]FRWID: 0->single bit, 1->single word
		 * FSXDUR-->DAVINCI_MCASP_TXFMCTL_REG.[4]FXWID: 0->single bit, 1->single word
		 */
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* No delay after FS */
		data_delay = 0;
		break;
	case SND_SOC_DAIFMT_I2S:
		/*
		 * configure a full-word SYNC pulse (LRCLK)
		 * FSRDUR-->DAVINCI_MCASP_RXFMCTL_REG.[4]FRWID: 0->single bit, 1->single word
		 * FSXDUR-->DAVINCI_MCASP_TXFMCTL_REG.[4]FXWID: 0->single bit, 1->single word
		 */
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* 1st data bit occur one ACLK cycle after the frame sync */
		data_delay = 1; /* TVP5158 */
		/* FS need to be inverted */
		inv_fs = true;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		/* configure a full-word SYNC pulse (LRCLK) */
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXDUR);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRDUR);
		/* No delay after FS */
		data_delay = 0;
		break;
	default:
		ret = -EINVAL;
		goto out;
	}

	mcasp_mod_bits(base + DAVINCI_MCASP_TXFMT_REG, FSXDLY(data_delay),
		       FSXDLY(3));
	mcasp_mod_bits(base + DAVINCI_MCASP_RXFMT_REG, FSRDLY(data_delay),
		       FSRDLY(3));

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		/* codec is clock and frame slave */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG, ACLKX | ACLKR);
		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG, AFSX | AFSR);
		dev->bclk_master = 1;
		break;
	case SND_SOC_DAIFMT_CBM_CFS:
		/* codec is clock master and frame slave */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE);

		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE);

		mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG, ACLKX | ACLKR);
		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG, AFSX | AFSR);
		dev->bclk_master = 1;
		break;
	case SND_SOC_DAIFMT_CBM_CFM:
		/* codec is clock and frame master */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE);
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE); //# external fs

		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRE);
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, AFSRE); //# external fs

		mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG,
			       ACLKX | AHCLKX | AFSX | ACLKR | AHCLKR | AFSR);
		dev->bclk_master = 0;

		/* TI811x AIC_MCLK <-- McASP2_AHCLKX(Pin out) */
		switch (dev->clk_input_pin) {
		case MCASP_AHCLKX_IN:
			mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
			break;
		case MCASP_AHCLKX_OUT:
			mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
			break;
		default:
			ret = -EINVAL;
			goto out;
		}

		break;

	default:
		ret = -EINVAL;
		goto out;
	}

	/*
	 * ACLKX Bit[7]CLKXP:clock-x polarity, 0->rising edge, 1->falling edge.
	 * ACLKR Bit[7]CLKRP:clock-r polarity, 0->falling edge, 1->rising edge.
	 */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_IB_NF:
		/*
		 * ACLKX; rising edge, ACLKR: falling edge
		 */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL); /* rising */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL); /* falling */
		fs_pol_rising = true;
		break;

	case SND_SOC_DAIFMT_NB_IF:
		/*
		 * ACLKX; falling edge, ACLKR: rising edge
		 */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL); /* falling */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL); /* rising */
		fs_pol_rising = false; /* TVP5158 */
		break;

	case SND_SOC_DAIFMT_IB_IF:
		/*
		 * ACLKX; rising edge, ACLKR: falling edge
		 */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL); /* rising */
		mcasp_clr_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL); /* falling */
		fs_pol_rising = false;
		break;

	case SND_SOC_DAIFMT_NB_NF:
		/*
		 * ACLKX; falling edge, ACLKR: rising edge
		 */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXPOL); /* falling */
		mcasp_set_bits(base + DAVINCI_MCASP_ACLKRCTL_REG, ACLKRPOL); /* rising */
		fs_pol_rising = true;
		break;

	default:
		ret = -EINVAL;
		goto out;
	}

	if (inv_fs)
		fs_pol_rising = !fs_pol_rising;

	/*
	 * SND_SOC_DAIFMT_IB_NF-> fs_pol_rising = true;
	 * SND_SOC_DAIFMT_NB_IF-> fs_pol_rising = false;
	 * SND_SOC_DAIFMT_IB_IF-> fs_pol_rising = false;
	 * SND_SOC_DAIFMT_NB_NF-> fs_pol_rising = true;
	 */
	if (fs_pol_rising) {
		/*
		 * AFSXCTL Bit[0]: 0->rising edge. 1->falling edge.
		 */
		mcasp_clr_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL); /* rising edge */
		/*
		 * AFSRCTL Bit[0]: 0->rising edge. 1->falling edge.
		 */
		mcasp_clr_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL); /* rising edge */
	} else {
		mcasp_set_bits(base + DAVINCI_MCASP_TXFMCTL_REG, FSXPOL); /* falling edge */
		mcasp_set_bits(base + DAVINCI_MCASP_RXFMCTL_REG, FSRPOL); /* falling edge */
	}

out:
	return ret;
}

#if 0 //# not supported.
static int __davinci_mcasp_set_clkdiv(struct snd_soc_dai *dai, int div_id,
				      int div, bool explicit)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);
	void __iomem *base = dev->base;

	switch (div_id) {
	case 0:		/* MCLK divider */
		mcasp_mod_bits(base + DAVINCI_MCASP_AHCLKXCTL_REG,
			       AHCLKXDIV(div - 1), AHCLKXDIV_MASK);
		mcasp_mod_bits(base + DAVINCI_MCASP_AHCLKRCTL_REG,
			       AHCLKRDIV(div - 1), AHCLKRDIV_MASK);
		break;

	case 1:		/* BCLK divider */
		mcasp_mod_bits(base + DAVINCI_MCASP_ACLKXCTL_REG,
			       ACLKXDIV(div - 1), ACLKXDIV_MASK);
		mcasp_mod_bits(base + DAVINCI_MCASP_ACLKRCTL_REG,
			       ACLKRDIV(div - 1), ACLKRDIV_MASK);
		break;

	case 2:		/* BCLK/LRCLK ratio */
		dev->bclk_lrclk_ratio = div;
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int davinci_mcasp_set_sysclk(struct snd_soc_dai *dai, int clk_id,
				    unsigned int freq, int dir)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);
	void __iomem *base = dev->base;

	if (dir == SND_SOC_CLOCK_OUT) {
		mcasp_set_bits(base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_set_bits(base + DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_set_bits(base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
	} else {
		mcasp_clr_bits(base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXE);
		mcasp_clr_bits(base + DAVINCI_MCASP_AHCLKRCTL_REG, AHCLKRE);
		mcasp_clr_bits(base + DAVINCI_MCASP_PDIR_REG, AHCLKX);
	}

	dev->sysclk_freq = freq;

	return 0;
}
#endif

static int davinci_config_channel_size(struct davinci_audio_dev *dev,
				       int word_length)
{
	u32 fmt;
	u32 tx_rotate = (word_length / 4) & 0x7;
	u32 mask = (1ULL << word_length) - 1;
	/*
	 * For captured data we should not rotate, inversion and masking is
	 * enough to get the data to the right position:
	 * Format	  data from bus		after reverse (XRBUF)
	 * S16_LE:	|LSB|MSB|xxx|xxx|	|xxx|xxx|MSB|LSB|
	 * S24_3LE:	|LSB|DAT|MSB|xxx|	|xxx|MSB|DAT|LSB|
	 * S24_LE:	|LSB|DAT|MSB|xxx|	|xxx|MSB|DAT|LSB|
	 * S32_LE:	|LSB|DAT|DAT|MSB|	|MSB|DAT|DAT|LSB|
	 */
	u32 rx_rotate = 0;

	/*
	 * if s BCLK-to-LRCLK ratio has been configured via the set_clkdiv()
	 * callback, take it into account here. That allows us to for example
	 * send 32 bits per channel to the codec, while only 16 of them carry
	 * audio payload.
	 * The clock ratio is given for a full period of data (for I2S format
	 * both left and right channels), so it has to be divided by number of
	 * tdm-slots (for I2S - divided by 2).
	 */
	if (dev->bclk_lrclk_ratio) {
		u32 slot_length = dev->bclk_lrclk_ratio / dev->tdm_slots;

		/*
		 * When we have more bclk then it is needed for the data, we
		 * need to use the rotation to move the received samples to have
		 * correct alignment.
		 */
		rx_rotate = (slot_length - word_length) / 4;
		word_length = slot_length;
	}
	/* mapping of the XSSZ bit-field as described in the datasheet */
	fmt = (word_length >> 1) - 1;

	if (dev->op_mode != DAVINCI_MCASP_DIT_MODE) {
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMT_REG, RXSSZ(fmt),
			       RXSSZ(0x0F));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, TXSSZ(fmt),
			       TXSSZ(0x0F));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, TXROT(tx_rotate),
			       TXROT(7));
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMT_REG, RXROT(rx_rotate),
			       RXROT(7));
		mcasp_set_reg(dev->base + DAVINCI_MCASP_RXMASK_REG, mask);
	}

	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXMASK_REG, mask);

	return 0;
}

static int davinci_hw_common_param(struct davinci_audio_dev *dev, int stream,
						int period_words, int channels)
{
	struct davinci_pcm_dma_params *dma_params = &dev->dma_params[stream];
	int i;
	u8 tx_ser = 0;
	u8 rx_ser = 0;
	u8 slots = dev->tdm_slots;
	u8 max_active_serializers = (channels + slots - 1) / slots;
	int active_serializers, numevt, n;

	/* Default configuration */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_PWREMUMGT_REG, MCASP_SOFT);

	/* All PINS as McASP */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_PFUNC_REG, 0x00000000);

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcasp_set_reg(dev->base + DAVINCI_MCASP_TXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_XEVTCTL_REG, TXDATADMADIS);
	} else {
		mcasp_set_reg(dev->base + DAVINCI_MCASP_RXSTAT_REG, 0xFFFFFFFF);
		mcasp_clr_bits(dev->base + DAVINCI_MCASP_REVTCTL_REG, RXDATADMADIS);
	}

	for (i = 0; i < dev->num_serializer; i++) {
		mcasp_set_bits(dev->base + DAVINCI_MCASP_XRSRCTL_REG(i),
					dev->serial_dir[i]);
		if (dev->serial_dir[i] == TX_MODE  &&
					tx_ser < max_active_serializers) {
			mcasp_set_bits(dev->base + DAVINCI_MCASP_PDIR_REG, AXR(i));
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_XRSRCTL_REG(i),
				       DISMOD_LOW, DISMOD_MASK);
			tx_ser++;
		} else if (dev->serial_dir[i] == RX_MODE &&
					rx_ser < max_active_serializers) {
			mcasp_clr_bits(dev->base + DAVINCI_MCASP_PDIR_REG, AXR(i));
			rx_ser++;
		} else {
			mcasp_mod_bits(dev->base + DAVINCI_MCASP_XRSRCTL_REG(i),
				       SRMOD_INACTIVE, SRMOD_MASK);
		}
	}

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		active_serializers = tx_ser;
		numevt = dev->txnumevt;
	} else {
		active_serializers = rx_ser;
		numevt = dev->rxnumevt;
	}

	if (active_serializers < max_active_serializers) {
		printk(KERN_ERR "stream has more channels (%d) than are "
			 "enabled in mcasp (%d)\n", channels,
			 active_serializers * slots);
		return -EINVAL;
	}

	/* AFIFO is not in use */
	if (!numevt) {
		/* Configure the burst size for platform drivers */
		if (active_serializers > 1) {
			/*
			 * If more than one serializers are in use we have one
			 * DMA request to provide data for all serializers.
			 * For example if three serializers are enabled the DMA
			 * need to transfer three words per DMA request.
			 */
			dma_params->fifo_level = active_serializers;
		} else {
			dma_params->fifo_level = 0;
		}
		return 0;
	}

	if (period_words % active_serializers) {
		printk(KERN_ERR "Invalid combination of period words and "
			"active serializers: %d, %d\n", period_words,
			active_serializers);
		return -EINVAL;
	}

	/*
	 * Calculate the optimal AFIFO depth for platform side:
	 * The number of words for numevt need to be in steps of active
	 * serializers.
	 */
	n = numevt % active_serializers;
	if (n)
		numevt += (active_serializers - n);
	while (period_words % numevt && numevt > 0)
		numevt -= active_serializers;
	if (numevt <= 0)
		numevt = active_serializers;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
							active_serializers, NUMDMA_MASK);
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_WFIFOCTL,
							NUMEVT(numevt), NUMEVT_MASK);
	} else {
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
							active_serializers, NUMDMA_MASK);
		mcasp_mod_bits(dev->base + DAVINCI_MCASP_RFIFOCTL,
							NUMEVT(numevt), NUMEVT_MASK);
	}
	/* Configure the burst size for platform drivers */
	if (numevt == 1)
		numevt = 0;

	dma_params->fifo_level = numevt;

	return 0;
}

static int davinci_i2s_hw_param(struct davinci_audio_dev *dev, int stream)
{
	int i, active_slots;
	u32 mask = 0;
	u32 busel = 0;

	if ((dev->tdm_slots < 2) || (dev->tdm_slots > 32)) {
		printk(KERN_ERR "tdm slot %d not supported\n",
			dev->tdm_slots);
		return -EINVAL;
	}

	active_slots = (dev->tdm_slots > 31) ? 32 : dev->tdm_slots;
	for (i = 0; i < active_slots; i++)
		mask |= (1 << i);

	mcasp_clr_bits(dev->base + DAVINCI_MCASP_ACLKXCTL_REG, TX_ASYNC); //# synchronous mode.

	if (!dev->dat_port)
		busel = TXSEL;

	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXTDM_REG, mask);
	mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, busel | TXORD);
	mcasp_mod_bits(dev->base + DAVINCI_MCASP_TXFMCTL_REG,
		       FSXMOD(dev->tdm_slots), FSXMOD(0x1FF));

	mcasp_set_reg(dev->base + DAVINCI_MCASP_RXTDM_REG, mask);
	mcasp_set_bits(dev->base + DAVINCI_MCASP_RXFMT_REG, busel | RXORD);
	mcasp_mod_bits(dev->base + DAVINCI_MCASP_RXFMCTL_REG,
		       FSRMOD(dev->tdm_slots), FSRMOD(0x1FF));

	return 0;
}

/* S/PDIF */
static int davinci_hw_dit_param(struct davinci_audio_dev *dev)
{
	/* Set the TX format : 24 bit right rotation, 32 bit slot, Pad 0
	   and LSB first */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_TXFMT_REG, TXROT(6) | TXSSZ(15));

	/* Set TX frame synch : DIT Mode, 1 bit width, internal, rising edge */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXFMCTL_REG, AFSXE | FSXMOD(0x180));

	/* Set the TX tdm : for all the slots */
	mcasp_set_reg(dev->base + DAVINCI_MCASP_TXTDM_REG, 0xFFFFFFFF);

	/* Set the TX clock controls : div = 1 and internal */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_ACLKXCTL_REG, ACLKXE | TX_ASYNC);

	mcasp_clr_bits(dev->base + DAVINCI_MCASP_XEVTCTL_REG, TXDATADMADIS);

	/* Only 44100 and 48000 are valid, both have the same setting */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG, AHCLKXDIV(3));

	/* Enable the DIT */
	mcasp_set_bits(dev->base + DAVINCI_MCASP_TXDITCTL_REG, DITEN);

	return 0;
}

static int davinci_mcasp_calc_clk_div(struct davinci_audio_dev *dev,
				      unsigned int bclk_freq,
				      int *error_ppm)
{
	int div = dev->sysclk_freq / bclk_freq;
	int rem = dev->sysclk_freq % bclk_freq;

	if (rem != 0) {
		if (div == 0 ||
		    ((dev->sysclk_freq / div) - bclk_freq) >
		    (bclk_freq - (dev->sysclk_freq / (div+1)))) {
			div++;
			rem = rem - bclk_freq;
		}
	}
	if (error_ppm)
		*error_ppm =
			(div*1000000 + (int)div64_s64(1000000LL*rem,
						       (int)bclk_freq))
			/div - 1000000;

	return div;
}

static int davinci_mcasp_hw_params(struct snd_pcm_substream *substream,
					struct snd_pcm_hw_params *params,
					struct snd_soc_dai *cpu_dai)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	struct davinci_pcm_dma_params *dma_params =
					&dev->dma_params[substream->stream];
	int word_length;
	int channels = params_channels(params);
	int period_size = params_period_size(params);
	int ret;

	/*
	 * If mcasp is BCLK master, and a BCLK divider was not provided by
	 * the machine driver, we need to calculate the ratio.
	 */
	#if 0
	if (dev->bclk_master) {
		int slots = dev->tdm_slots;
		int rate = params_rate(params);
		int sbits = 16;
		int ppm, div;

		__davinci_mcasp_set_clkdiv(cpu_dai, 0, 100, 0);

		div = davinci_mcasp_calc_clk_div(dev, rate*sbits*slots,
						 &ppm);
		if (ppm)
			printk("Sample-rate is off by %d PPM\n", ppm);

		__davinci_mcasp_set_clkdiv(cpu_dai, 1, div, 0);
	}
	#endif

	ret = davinci_hw_common_param(dev, substream->stream,
							period_size * channels, channels);
	if (ret)
		return ret;

	if (dev->op_mode == DAVINCI_MCASP_DIT_MODE)
		ret = davinci_hw_dit_param(dev);
	else
		ret = davinci_i2s_hw_param(dev, substream->stream);

	if (ret)
		return ret;

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_U8:
	case SNDRV_PCM_FORMAT_S8:
		dma_params->data_type = 1;
		word_length = 8;
		break;

	case SNDRV_PCM_FORMAT_U16_LE:
	case SNDRV_PCM_FORMAT_S16_LE:
		dma_params->data_type = 2;
		word_length = 16;
		break;

	case SNDRV_PCM_FORMAT_U24_3LE:
	case SNDRV_PCM_FORMAT_S24_3LE:
		dma_params->data_type = 3;
		word_length = 24;
		break;

	case SNDRV_PCM_FORMAT_U24_LE:
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_U32_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
		dma_params->data_type = 4;
		word_length = 32;
		break;

	default:
		printk(KERN_WARNING "davinci-mcasp: unsupported PCM format");
		return -EINVAL;
	}

	if (dev->version == MCASP_VERSION_2 && !dma_params->fifo_level)
		dma_params->acnt = 4;
	else
		dma_params->acnt = dma_params->data_type;

	davinci_config_channel_size(dev, word_length);

	return 0;
}

static int davinci_mcasp_trigger(struct snd_pcm_substream *substream,
				     int cmd, struct snd_soc_dai *cpu_dai)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(cpu_dai);
	int ret = 0;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (!dev->clk_active) {
			clk_enable(dev->clk);
			dev->clk_active = 1;
		}
		davinci_mcasp_start(dev, substream->stream);
		break;

	case SNDRV_PCM_TRIGGER_SUSPEND:
		if (dev->clk_active) {
			davinci_mcasp_stop(dev, substream->stream);
			clk_disable(dev->clk);
			dev->clk_active = 0;
		}

		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		davinci_mcasp_stop(dev, substream->stream);
		break;

	default:
		ret = -EINVAL;
	}

	return ret;
}

static int davinci_mcasp_startup(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *dai)
{
	struct davinci_audio_dev *dev = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_set_dma_data(dai, substream, dev->dma_params);

	return 0;
}

#ifdef CONFIG_MACH_FITT
static inline void davinci_mcasp_ahclkx_enable(struct davinci_audio_dev *dev)
{
	u32 reg;

	mcasp_set_bits(dev->base + DAVINCI_MCASP_PDIR_REG,	(0x1 << 27));
	mcasp_set_bits(dev->base + DAVINCI_MCASP_AHCLKXCTL_REG,	AHCLKXE);

	reg = mcasp_get_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG);
	if ((reg & TXHCLKRST) == 0)
		mcasp_set_ctl_reg(dev->base + DAVINCI_MCASP_GBLCTLX_REG, TXHCLKRST);

}
#endif

static struct snd_soc_dai_ops davinci_mcasp_dai_ops = {
	.startup	= davinci_mcasp_startup,
	.trigger	= davinci_mcasp_trigger,
	.hw_params	= davinci_mcasp_hw_params,
	.set_fmt	= davinci_mcasp_set_dai_fmt,
//	.set_sysclk	= davinci_mcasp_set_sysclk,
};

static struct snd_soc_dai_driver davinci_mcasp_dai[] = {
	{
		.name		= "davinci-mcasp.0",
		.playback	= {
			.channels_min	= 2,
			.channels_max 	= 2,
			.rates 		= DAVINCI_MCASP_RATES,
			.formats 	= SNDRV_PCM_FMTBIT_S8 |
						SNDRV_PCM_FMTBIT_S16_LE |
						SNDRV_PCM_FMTBIT_S32_LE,
		},
		.capture 	= {
			.channels_min 	= 2,
			.channels_max 	= 2,
			.rates 		= DAVINCI_MCASP_RATES,
			.formats	= SNDRV_PCM_FMTBIT_S8 |
						SNDRV_PCM_FMTBIT_S16_LE |
						SNDRV_PCM_FMTBIT_S32_LE,
		},
		.ops 		= &davinci_mcasp_dai_ops,

	},
	{
		"davinci-mcasp.1",
		.playback 	= {
			.channels_min	= 1,
			.channels_max	= 384,
			.rates		= DAVINCI_MCASP_RATES,
			.formats	= SNDRV_PCM_FMTBIT_S16_LE,
		},
		.ops 		= &davinci_mcasp_dai_ops,
	},

};

static int davinci_mcasp_probe(struct platform_device *pdev)
{
	struct davinci_pcm_dma_params *dma_data;
	struct resource *mem, *ioarea, *res;
	struct snd_platform_data *pdata;
	struct davinci_audio_dev *dev;
	int ret = 0;

	dev = kzalloc(sizeof(struct davinci_audio_dev), GFP_KERNEL);
	if (!dev)
		return	-ENOMEM;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem) {
		dev_err(&pdev->dev, "no mem resource?\n");
		ret = -ENODEV;
		goto err_release_data;
	}

	ioarea = request_mem_region(mem->start,
			resource_size(mem), pdev->name);
	if (!ioarea) {
		dev_err(&pdev->dev, "Audio region already claimed\n");
		ret = -EBUSY;
		goto err_release_data;
	}

	pdata = pdev->dev.platform_data;
	dev->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(dev->clk)) {
		dev_err(&pdev->dev, "Failed to get audio clock!!\n");
		ret = -ENODEV;
		goto err_release_region;
	}

	clk_enable(dev->clk);
	dev->clk_active = 1;

	dev->base = ioremap(mem->start, resource_size(mem));
	if (!dev->base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENOMEM;
		goto err_release_clk;
	}

	dev->op_mode = pdata->op_mode;
	dev->tdm_slots = pdata->tdm_slots;
	dev->num_serializer = pdata->num_serializer;
	dev->serial_dir = pdata->serial_dir;
	dev->codec_fmt = pdata->codec_fmt;
	dev->version = pdata->version;
	dev->txnumevt = pdata->txnumevt;
	dev->rxnumevt = pdata->rxnumevt;
	dev->clk_input_pin = pdata->clk_input_pin;

	/* dma_params->dma_addr is pointing to the data port address */
	dev->dat_port = true;

	dma_data = &dev->dma_params[SNDRV_PCM_STREAM_PLAYBACK];
	dma_data->asp_chan_q = pdata->asp_chan_q;
	dma_data->ram_chan_q = pdata->ram_chan_q;
	if (cpu_is_ti81xx())
		dma_data->dma_addr = (dma_addr_t) (pdata->tx_dma_offset);
	else
		dma_data->dma_addr = (dma_addr_t) (pdata->tx_dma_offset
							+ mem->start);

	/* first TX, then RX */
	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENODEV;
		goto err_iounmap;
	}

	dma_data->channel = res->start;

	dma_data = &dev->dma_params[SNDRV_PCM_STREAM_CAPTURE];
	dma_data->asp_chan_q = pdata->asp_chan_q;
	dma_data->ram_chan_q = pdata->ram_chan_q;
	if (cpu_is_ti81xx())
		dma_data->dma_addr = (dma_addr_t) (pdata->rx_dma_offset);
	else
		dma_data->dma_addr = (dma_addr_t) (pdata->rx_dma_offset
							+ mem->start);

	res = platform_get_resource(pdev, IORESOURCE_DMA, 1);
	if (!res) {
		dev_err(&pdev->dev, "no DMA resource\n");
		ret = -ENODEV;
		goto err_iounmap;
	}

	dma_data->channel = res->start;
	dev_set_drvdata(&pdev->dev, dev);

	ret = snd_soc_register_dai(&pdev->dev, &davinci_mcasp_dai[pdata->op_mode]);
	if (ret != 0) {
		dev_err(&pdev->dev, "Failed to register audio dev!!\n");
		goto err_iounmap;
	}

#ifdef CONFIG_MACH_FITT
 	davinci_mcasp_ahclkx_enable(dev);
#endif

	return 0;

err_iounmap:
	iounmap(dev->base);
err_release_clk:
	clk_disable(dev->clk);
	clk_put(dev->clk);
err_release_region:
	release_mem_region(mem->start, resource_size(mem));
err_release_data:
	kfree(dev);

	return ret;
}

static int davinci_mcasp_remove(struct platform_device *pdev)
{
	struct davinci_audio_dev *dev = dev_get_drvdata(&pdev->dev);
	struct resource *mem;

	snd_soc_unregister_dai(&pdev->dev);
	clk_disable(dev->clk);
	clk_put(dev->clk);
	dev->clk = NULL;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(mem->start, resource_size(mem));

	kfree(dev);

	return 0;
}

static struct platform_driver davinci_mcasp_driver = {
	.probe		= davinci_mcasp_probe,
	.remove		= davinci_mcasp_remove,
	.driver		= {
		.name	= "davinci-mcasp",
		.owner	= THIS_MODULE,
	},
};

static int __init davinci_mcasp_init(void)
{
	return platform_driver_register(&davinci_mcasp_driver);
}
module_init(davinci_mcasp_init);

static void __exit davinci_mcasp_exit(void)
{
	platform_driver_unregister(&davinci_mcasp_driver);
}
module_exit(davinci_mcasp_exit);

MODULE_AUTHOR("Steve Chen");
MODULE_DESCRIPTION("TI DAVINCI McASP SoC Interface");
MODULE_LICENSE("GPL");

