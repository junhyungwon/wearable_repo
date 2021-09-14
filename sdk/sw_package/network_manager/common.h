/*
 * common.h
 */
#ifndef __COMMON_H__
#define __COMMON_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define RTL_8821A_VID				0x0bda
#define RTL_8821A_PID				0xA811

#define RTL_88X2B_VID				0x0bda //# comfast CF-812
#define RTL_88X2B_PID				0xB812

/* set gpio direction */
#define GPIO_INPUT				0
#define GPIO_OUTPUT				1

#define GPIO_ACTIVE_HIGH		0
#define GPIO_ACTIVE_LOW			1

#define GPIO_OUTPUT_HIGH		1
#define GPIO_OUTPUT_LOW			0

/* set gpio irq mode */
#define GPIO_IRQ_NONE			0
#define GPIO_IRQ_RISING			1
#define GPIO_IRQ_FALLING		2
#define GPIO_IRQ_BOTH			3

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/

int gpio_input_init(int num);
int gpio_output_init(int num, int default_val);
int gpio_exit(int num);
int gpio_set_value(int num, int value);
int gpio_get_value(int num, int *val);

int netmgr_get_net_info(const char *ifce, char *hw_buf, char *ip_buf, char *mask, char *gw);
void netmgr_net_link_up(const char *ifce);
void netmgr_net_link_down(const char *ifce);

int netmgr_set_ip_static(const char *ifname, const char *ip, const char *net_mask, const char *gateway);
int netmgr_set_shm_ip_info(int dev, const char *ip, const char *mask, const char *gw);
void netmgr_udhcpc_start(const char *ifname);
void netmgr_udhcpc_stop(const char *ifname);
int netmgr_udhcpc_is_run(const char *ifname);

int utf8_unescape(const char *dst, char *src);
int hextobin(unsigned char c);
int hexstr2bin(const char *hex, unsigned char *buf, size_t len);

#endif /* __DEV_GPIO_H__ */
