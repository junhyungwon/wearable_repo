#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9ad29e1f, "module_layout" },
	{ 0xbd84d0f2, "kmalloc_caches" },
	{ 0x9b388444, "get_zeroed_page" },
	{ 0xb1765bd2, "mem_map" },
	{ 0xa90c928a, "param_ops_int" },
	{ 0xd8f795ca, "del_timer" },
	{ 0xb72c7acf, "dev_set_drvdata" },
	{ 0xbbd45ed4, "hrtimer_forward" },
	{ 0xc2ab79e2, "snd_pcm_period_elapsed" },
	{ 0x7e815704, "snd_card_create" },
	{ 0x4a5361cf, "hrtimer_cancel" },
	{ 0xf7802486, "__aeabi_uidivmod" },
	{ 0x677bb305, "param_ops_bool" },
	{ 0x74c86cc0, "init_timer_key" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xd8435e6c, "snd_pcm_hw_constraint_integer" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x629464e2, "snd_pcm_suspend_all" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xacc1ebd1, "param_ops_charp" },
	{ 0xea147363, "printk" },
	{ 0x3573c114, "snd_pcm_set_ops" },
	{ 0xabd345b9, "snd_ctl_boolean_stereo_info" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0x7298d15d, "snd_pcm_lib_free_pages" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0x2bfff013, "platform_device_unregister" },
	{ 0x8cfc414e, "add_timer" },
	{ 0xba540b82, "platform_driver_register" },
	{ 0xcdce1bd5, "snd_pcm_lib_ioctl" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0x9af54a79, "snd_pcm_lib_malloc_pages" },
	{ 0x59e5070d, "__do_div64" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xf55c8c02, "kmem_cache_alloc" },
	{ 0x46ff8a8e, "snd_ctl_new1" },
	{ 0x72becf51, "hrtimer_start" },
	{ 0x4302d0eb, "free_pages" },
	{ 0xb9e52429, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0xd1329880, "param_array_ops" },
	{ 0xc9cbfbd7, "hrtimer_init" },
	{ 0x1e7d937e, "snd_pcm_hw_constraint_minmax" },
	{ 0x28ff0a1f, "platform_device_register_resndata" },
	{ 0x8ec46d07, "snd_pcm_lib_preallocate_pages_for_all" },
	{ 0xdaac631d, "snd_card_free" },
	{ 0x27c09d20, "snd_card_register" },
	{ 0xc94bc3f5, "snd_pcm_new" },
	{ 0x17084926, "snd_ctl_add" },
	{ 0x1f1e12ae, "platform_driver_unregister" },
	{ 0x9b0ca13d, "dev_get_drvdata" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

