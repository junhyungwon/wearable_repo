/*
 * gpio.h
 */
#ifndef __DEV_GPIO_H__
#define __DEV_GPIO_H__

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
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

#define NETMGR_USB_MAX_NUM		5

#define RTL_8188E_VID			0x0bda
#define RTL_8188E_PID			0x8179

#define RTL_8188C_VID			0x0bda
#define RTL_8188C_PID			0x8176

#define RTL_8192C_VID			0x0bda
#define RTL_8192C_PID			0x8178

#define RTL_8821A_VID			0x0bda
#define RTL_8821A_PID			0xA811

#define RTL_8812A_VID			0x2357 //# TP-link archer T4U
#define RTL_8812A_PID			0x010d

#define USBETHER_OPER_PATH	  	"/sys/class/net/eth1/operstate"
#define USB_LS_CMD_STR			"/usr/bin/lsusb"

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
void netmgr_net_link_down(const char *ifce);
int netmgr_net_link_up(const char *ifce);
int netmgr_net_link_detect(const char *ifce);
int netmgr_is_netdev_active(const char *ifname);

int netmgr_set_ip_static(const char *ifname, const char *ip, const char *net_mask, const char *gateway);
int netmgr_set_ip_dhcp(const char *ifname);

int netmgr_udhcpc_is_run(const char *ifname);
void netmgr_udhcpc_stop(const char *ifname);
int netmgr_set_shm_ip_info(int dev, const char *ip, const char *mask, const char *gw);

int netmgr_usb_is_exist(int usb_v, int usb_p);
int netmgr_wlan_is_exist(int *dst_vid, int *dst_pid);
int netmgr_wlan_load_kermod(int vid, int pid);
int netmgr_wlan_wait_mod_active(void);

#endif /* __DEV_GPIO_H__ */
