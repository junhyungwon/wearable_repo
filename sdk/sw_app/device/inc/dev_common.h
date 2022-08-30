/*
 * dev_common.h
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
 * Common Hardware library interface definitions.
 */

#ifndef _DEV_COMMON_H__
#define _DEV_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <termios.h>	/* For struct termios */
#include <time.h> //# nanosleep()

//#define DBG_DEV

/* Debugging */
#define dev_err(fmt, arg...)  fprintf(stderr, "%s:%d: " fmt, __FILE__, __LINE__, ## arg)

#ifdef DBG_DEV
#define dev_dbg(fmt, arg...)  fprintf(stdout, "%s:%d: " fmt, __FILE__, __LINE__, ## arg)
#else
#define dev_dbg(fmt, arg...)  do { } while (0)
#endif

#define INPUT_PROC_PATH	 	"/proc/bus/input/devices"

#define BITS_PER_LONG 		(sizeof(long) * 8)
#define BIT_WORD(x)			((x) / BITS_PER_LONG)
#define BIT_MASK(x)  		(1UL << ((x) % BITS_PER_LONG))

#define NBITS(x) 			((((x)-1) / BITS_PER_LONG) + 1)

/*
 * test_bit - Determine whether a bit is set
 * nr : bit number to test
 * addr : Address to start counting from
 */
#define TEST_BIT(b, a)		((a[BIT_WORD(b)]>>(b&(BITS_PER_LONG-1)))&1UL)
#define ARRAY_SIZE(x) 		(sizeof(x) / sizeof(x[0]))

#define DEV_NET_NAME_ETH			"eth0"
#define DEV_NET_NAME_ETH1			"eth1"
#define DEV_NET_NAME_WLAN			"wlan0"
#define SIZE_NET_STR			16

#define NETSTATUS_UP			1
#define NETSTATUS_DOWN			0

#define DEV_NET_TYPE_STATIC 	0
#define DEV_NET_TYPE_DHCP       1

typedef struct {
	char type;					//# 0: static, 1: dhcp
	char state;					//# 0: link down, 1: link up
	char ip[SIZE_NET_STR];		//# ip address
	char mask[SIZE_NET_STR];	//# netmask
	char gate[SIZE_NET_STR];	//# gateway
	char hwaddr[32];            //# HW address-mac

} dev_net_info_t;

#ifndef RTF_UP
/* Keep this in sync with /usr/src/linux/include/linux/route.h */
#define RTF_UP          0x0001	/* route usable                 */
#define RTF_GATEWAY     0x0002	/* destination is a gateway     */
#define RTF_HOST        0x0004	/* host entry (net otherwise)   */
#define RTF_REINSTATE   0x0008	/* reinstate route after tmout  */
#define RTF_DYNAMIC     0x0010	/* created dyn. (by redirect)   */
#define RTF_MODIFIED    0x0020	/* modified dyn. (by redirect)  */
#define RTF_MTU         0x0040	/* specific MTU for this route  */

#ifndef RTF_MSS
#define RTF_MSS         RTF_MTU	/* Compatibility :-(            */
#endif

#define RTF_WINDOW      0x0080	/* per route window clamping    */
#define RTF_IRTT        0x0100	/* Initial round trip time      */
#define RTF_REJECT      0x0200	/* Reject route                 */
#endif //# #ifndef RTF_UP

#define IPV4_MASK (RTF_GATEWAY|RTF_HOST|RTF_REINSTATE|RTF_DYNAMIC|RTF_MODIFIED)

static const unsigned flagvals[] = { /* Must agree with flagchars[]. */
	RTF_GATEWAY,
	RTF_HOST,
	RTF_REINSTATE,
	RTF_DYNAMIC,
	RTF_MODIFIED,
};

/* Must agree with flagvals[]. */
static const char flagchars[] =
  "GHRDM"
;

int dev_input_get_bus_num(const char *dev_name);
void dev_wait_for_msecs(unsigned int msecs);
int dev_get_board_info(void);
int dev_execlp(char *arg);
int dev_net_link_detect(const char *ifce);
int dev_net_link_up(const char *ifce);
int dev_net_link_down(const char *ifce);
int dev_get_net_info(const char *ifce, dev_net_info_t *inet);
int dev_set_net_info(const char *ifce, dev_net_info_t *inet);
int dev_set_net_if_gate(const char *ifce, char *gate);
int dev_usb_is_exist(int usb_v, int usb_p);
int dev_rtc_set_time(struct tm set_tm);
speed_t dev_tty_get_baudrate(int rate);

int dev_board_serial_init(void);
int dev_board_serial_write(const char *data, int length);
int dev_board_serial_read(char *data, int length);

int dev_board_uid_init(void);
int dev_board_uid_read(char *data, int length);
int dev_board_uid_write(const char *data, int length);

int dev_board_cert_file_store(const char *fname);
int dev_board_cert_file_load(const char *fname);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif //# _DEV_COMMON_H__
