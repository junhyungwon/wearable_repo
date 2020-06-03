/*
 * ASoC driver for TI 81XX UD Video Recoder platform.
 *
 * Based on davinci-evm.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <asm/dma.h>
#include <asm/mach-types.h>

#include <plat/asp.h>
#include <asm/hardware/edma.h>

#include "../codecs/tlv320aic3x.h"

#include "davinci-pcm.h"
#include "davinci-i2s.h"
#include "davinci-mcasp.h"

#define AIC3X_AUDIO_FORMAT (SND_SOC_DAIFMT_DSP_B | \
		SND_SOC_DAIFMT_CBM_CFM | SND_SOC_DAIFMT_IB_NF)

static int mcasp1_aic3x_hw_params(struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int sysclk;
	int ret = 0;

	/* TI811x use McASP2_AUX 20MHz */
	sysclk = 20000000;

	ret = snd_soc_dai_set_fmt(codec_dai, AIC3X_AUDIO_FORMAT);
	if (ret < 0)
		return ret;

	/* set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, AIC3X_AUDIO_FORMAT);
	if (ret < 0)
		return ret;
	/*
	 * int snd_soc_dai_set_sysclk(struct snd_soc_dai *dai, int clk_id,
     * 		unsigned int freq, int dir);
     */
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, sysclk, SND_SOC_CLOCK_OUT);
	if (ret < 0)
		return ret;

	return 0;
}

static struct snd_soc_ops mcasp1_aic3x_soc_ops = {
	.hw_params = mcasp1_aic3x_hw_params,
};

/* davinci-evm machine dapm widgets */
static const struct snd_soc_dapm_widget aic3x_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_LINE("Line Out", NULL),
	SND_SOC_DAPM_MIC("Mic Jack", NULL),
	SND_SOC_DAPM_LINE("Line In", NULL),
};

/*
 * struct snd_soc_dapm_route {
 * 		const char *sink;
 *		const char *control;
 *		const char *source;
 *		Note: currently only supported for links where source is a supply
 *      int (*connected)(struct snd_soc_dapm_widget *source,
 *                         struct snd_soc_dapm_widget *sink);
 *
 */
static const struct snd_soc_dapm_route aic3x_audio_map[] = {
	/* Headphone connected to HPLOUT, HPROUT */
	{"Headphone Jack", NULL, "HPLOUT"},
//	{"Headphone Jack", NULL, "HPLCOM"},
	{"Headphone Jack", NULL, "HPROUT"},

	/* Line Out is connected to LLOUT, RLOUT */
	{"Line Out", NULL, "LLOUT"},
	{"Line Out", NULL, "RLOUT"},

	/* Mic connected to (LINE2L | LINE2R) */
	{"LINE2R", NULL, "Mic Bias"},
	{"LINE2L", NULL, "Mic Bias"},
	{"Mic Bias", NULL, "Mic Jack"},

	/* Line In connected to LINE1L, LINE1R */
	{"LINE1L", NULL, "Line In"},
	{"LINE1R", NULL, "Line In"},
};

static int mcasp1_aic3x_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_codec *codec = rtd->codec;

	/* Add specific widgets */
	snd_soc_dapm_new_controls(codec, aic3x_dapm_widgets,
				ARRAY_SIZE(aic3x_dapm_widgets));

	/* Set up davinci-evm specific audio path audio_map */
	snd_soc_dapm_add_routes(codec, aic3x_audio_map,
				ARRAY_SIZE(aic3x_audio_map));

//	snd_soc_dapm_disable_pin(codec, "HPRCOM");
//	snd_soc_dapm_disable_pin(codec, "HPROUT");

	snd_soc_dapm_enable_pin(codec, "Line In");
	snd_soc_dapm_enable_pin(codec, "Headphone Jack");
	snd_soc_dapm_enable_pin(codec, "Line Out");
	snd_soc_dapm_enable_pin(codec, "Mic Jack");

	snd_soc_dapm_sync(codec);

	return 0;
}

static struct snd_soc_dai_link mcasp1_aic3x_dai[] = {
	{
		.name = "TLV320AIC3X",
		.stream_name = "AIC3X",
		.cpu_dai_name= "davinci-mcasp.1",
		.codec_dai_name = "tlv320aic3x-hifi",
		.codec_name = "tlv320aic3x-codec.1-0018",
		.platform_name = "davinci-pcm-audio",
		.init = mcasp1_aic3x_init,
		.ops = &mcasp1_aic3x_soc_ops,
	}
};

static struct snd_soc_card mcasp1_aic3x_card[] = {
	{
		.name = "aic3x-sound-card",
		.dai_link = mcasp1_aic3x_dai,
		.num_links = ARRAY_SIZE(mcasp1_aic3x_dai),
	}
};

static struct platform_device *soc_mcasp1_pdev;

static int __init ud_soc_init(void)
{
	int r;

	soc_mcasp1_pdev = platform_device_alloc("soc-audio", 0);
	if (soc_mcasp1_pdev == NULL)
		return -ENOMEM;

	platform_set_drvdata(soc_mcasp1_pdev, mcasp1_aic3x_card);
	r = platform_device_add(soc_mcasp1_pdev);
	if (r) {
		printk(KERN_ERR "Can't add mcasp1 soc platform device!!\n");
		platform_device_put(soc_mcasp1_pdev);
		return r;
	}

	return r;
}

static void __exit ud_soc_exit(void)
{
	platform_device_unregister(soc_mcasp1_pdev);
}

module_init(ud_soc_init);
module_exit(ud_soc_exit);

MODULE_AUTHOR("UDWORKs");
MODULE_DESCRIPTION("TI81XX UD ASoC driver");
MODULE_LICENSE("GPL");
