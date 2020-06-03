/*
 * dev_wifi.h
 *
 * Copyright (C) 2013 UDWORKs.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Wi-Fi Hardware library interface definitions.
 */

#ifndef _DEV_WIFI_H_
#define _DEV_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * 0-> RTL 8188E usb
 * 1-> RTL 8188C usb
 * 2-> RTL 8192C usb
 * 3-> RTL 8821A usb
 * 4-> RTL 8812A usb
 */
#define RTL_USB_MAX_NUM			(5)

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

#define WIFI_IFACE_NAME				"wlan0"
#define WIFI_DEFAULT_IP				"192.168.0.1"

#define WIFI_LINK_UP				1
#define WIFI_LINK_DOWN				0

#define HOSTAPD_PID_PATH			"/var/run/hostapd.pid"
#define HOSTAPD_CONFIG				"/etc/hostapd.conf"

int dev_wifi_is_exist(int *dst_vid, int *dst_pid);
int dev_wifi_get_module_state(const char *name);
void dev_wifi_load(const char *module_path);
void dev_wifi_unload(const char *module_name);
int dev_wifi_on(const char *ssid, const char *passwd, int channel, int is_5g, int stealth);
int dev_wifi_off(void);
int dev_wifi_update_ssid(const char *ssid);
int dev_wifi_update_passwd(const char *passwd);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif //# _DEV_WIFI_H_

