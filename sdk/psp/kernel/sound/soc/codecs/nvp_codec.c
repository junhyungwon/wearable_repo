/*
 * ALSA SoC NEXTCHIP Audio Codec
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/initval.h>

static struct snd_soc_codec_driver soc_codec_nvp;

/* Dummy dai driver for HDMI */
static struct snd_soc_dai_driver nvp_dai = {
	.name = "NVP-CODEC",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 16,
		.rates = SNDRV_PCM_RATE_8000
			| SNDRV_PCM_RATE_16000
			| SNDRV_PCM_RATE_32000 ,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 16,
		.rates = SNDRV_PCM_RATE_8000
			| SNDRV_PCM_RATE_16000
			| SNDRV_PCM_RATE_32000 ,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
};

static int nvp_codec_probe(struct platform_device *pdev)
{
	int ret;

	ret = snd_soc_register_codec(&pdev->dev, &soc_codec_nvp,
					&nvp_dai, 1);
	if (ret < 0)
		printk(KERN_INFO "NVP Codec Register Failed.\n");

	return ret;
}

static int nvp_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver nvp_codec_driver = {
	.probe		= nvp_codec_probe,
	.remove		= nvp_codec_remove,
	.driver		= {
		.name	= "nvp-dummy-codec",
		.owner	= THIS_MODULE,
	},
};

static int __init nvp_modinit(void)
{
	return platform_driver_register(&nvp_codec_driver);
}

static void __exit nvp_exit(void)
{
	platform_driver_unregister(&nvp_codec_driver);
}

module_init(nvp_modinit);
module_exit(nvp_exit);

MODULE_AUTHOR("UDWORKS");
MODULE_DESCRIPTION(" NEXTCHIP Dummy codec Interface");
MODULE_LICENSE("GPL");
