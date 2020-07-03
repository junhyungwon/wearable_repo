/******************************************************************************
 * FITT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_hotplug.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2013.04.01 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>

#include "dev_wifi.h"
#include "dev_dial.h"

#include "app_comm.h"
#include "app_netdev.h"
#include "app_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//#define NETLINK_USB_DEVS_MAX	(RTL_USB_MAX_NUM + DIAL_USB_MAX_NUM) //# 5 + 3
#define NETLINK_USB_DEVS_MAX	(RTL_USB_MAX_NUM) //# 5
#define NETLINK_POLL_TIMEOUT	300
#define NETLINK_GROUP_KERNEL	1
#define NETLINK_USB_PATH		"/sys/bus/usb/devices"

struct usb_session_t {
	int sid;   /* busnum << 8 | devnum */
	int vid;   /* vendor id */
	int pid;   /* product id */
};

typedef struct {
	app_thr_obj kObj;	    //# kobject netlink object
	struct usb_session_t usb_ss[NETLINK_USB_DEVS_MAX];
	int fd;

} app_netlink_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_netlink_t t_nlink = {.fd = -1,};
static app_netlink_t *inlink = &t_nlink;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static const char *netlink_message_parse (const char *buffer, size_t len, const char *key)
{
	size_t keylen = strlen(key);
	size_t offset;

	for (offset = 0 ; offset < len && '\0' != buffer[offset] ; offset += strlen(buffer + offset) + 1) {
		if (0 == strncmp(buffer + offset, key, keylen) &&
		    '=' == buffer[offset + keylen]) {
			return buffer + offset + keylen + 1;
		}
	}

	return NULL;
}

/*----------------------------------------------------------------------------
 netlink read sysfs
-----------------------------------------------------------------------------*/
static int netlink_read_sysfs_attr(const char *devname, const char *attr)
{
	char filename[1024 + 1];
	char buf[256 + 1] = {0,};
	char *r;

	FILE *f;
	int value;

	snprintf(filename, sizeof(filename), "%s/%s/%s", NETLINK_USB_PATH, devname, attr);
	f = fopen(filename, "r");
	if (f == NULL) {
		dprintf("open %s failed\n", filename);
		return -1;
	}
	r = fgets(buf, 255, f);
	fclose(f);

	sscanf(buf, "%x", &value);
	if (value < 0) {
		dprintf("%s contains a negative value\n", filename);
		return -1;
	}

	return value;
}

/*****************************************************************************
* @brief    parse parts of netlink message (get subsystem)
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netlink_subsystem_parse(char *buffer, size_t len, int *subsystem, int *detached)
{
	const char *tmp;

	*detached = 0;
	*subsystem = -1; /* default unknown device */

	tmp = netlink_message_parse((const char *) buffer, len, "ACTION");
	if (tmp == NULL)
		return -1;

	if (strcmp(tmp, "remove") == 0) {
		*detached = 1;
	} else if (strcmp(tmp, "add") != 0) {
//		dprintf("unknown device action %s\n", tmp);
		return -1;
	}

	/* check that this is a usb or mmc message */
	tmp = netlink_message_parse(buffer, len, "SUBSYSTEM");
	if (tmp == NULL)
		/* ignore message */
		return -1;

	if (strcmp(tmp, "mmc") == 0) {
		*subsystem = 0;
		return 0;
	} else if (strcmp(tmp, "usb") != 0) {
//		dprintf("unknown device subsystem %s\n", tmp);
		return -1;
	}
	*subsystem = 1; /* usb */

	return 0;
}

/*****************************************************************************
* @brief    parse parts of netlink message (for usb)
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netlink_usb_get_attribute(char *buffer, size_t len, const char **sys_name,
			       unsigned char *busnum, unsigned char *devaddr)
{
	const char *tmp;
	int i;

	*sys_name = NULL;
	*busnum   = 0;
	*devaddr  = 0;

	tmp = netlink_message_parse(buffer, len, "BUSNUM");
	if (NULL == tmp) {
//		eprintf("Failed to netlink_message_parse(BUSNUM)\n");
		return -1;
	}

	/* 0-> hub, 1-> usb port 1, 2-> usb port 2*/
	*busnum = (unsigned char)(strtoul(tmp, NULL, 10) & 0xff);
	if (*busnum < 0 || *busnum > 2) {
		/* invalid bus number (olny supported bus: 0, 1, 2)*/
//		eprintf("invalid bus number(%d)\n", *busnum);
		return -1;
	}

	tmp = netlink_message_parse(buffer, len, "DEVNUM");
	if (NULL == tmp) {
//		eprintf("Failed to netlink_message_parse(DEVNUM)\n");
		return -1;
	}
	*devaddr = (unsigned char)(strtoul(tmp, NULL, 10) & 0xff);

	tmp = netlink_message_parse(buffer, len, "DEVPATH");
	if (NULL == tmp) {
//		eprintf("Failed to netlink_message_parse(DEVPATH)\n");
		return -1;
	}

	for (i = strlen(tmp) - 1 ; i ; --i) {
		if ('/' == tmp[i]) {
			*sys_name = tmp + i + 1;
			break;
		}
	}

	return 0;
}


/*****************************************************************************
* @brief    static int netlink_usb_detach_parse(char *buffer, size_t len)
*****************************************************************************/
static int netlink_usb_detach_parse(char *buffer, size_t len, int *v, int *p)
{
	const char *sys_name = NULL;
	struct usb_session_t *pusb_ss;

	unsigned long session_id = 0;

	unsigned char busnum;
	unsigned char devaddr;

	int i, r;

	r = netlink_usb_get_attribute(buffer, len, &sys_name, &busnum, &devaddr);
	if (r < 0) {
		return -1;
	}

	session_id = (busnum << 8 | devaddr);
	for (i = 0; i < NETLINK_USB_DEVS_MAX; i++) {
		pusb_ss = &inlink->usb_ss[i];

//		printf("pusb_ss->sid %x, session_id %x\n", pusb_ss->sid, session_id);
		if (pusb_ss->sid == session_id) {
			break;
		}
	}

	if (i >= NETLINK_USB_DEVS_MAX) {
		printf("Not found usb session id (det = %lx)!!\n", session_id);
		return -1;
	}

	pusb_ss = &inlink->usb_ss[i];
	pusb_ss->sid = 0; //# clear session.

	*v = pusb_ss->vid; *p = pusb_ss->pid;

	return 0;
}

/*****************************************************************************
* @brief static int netlink_usb_attach_parse(char *buffer, size_t len, int *v, int *p)
*****************************************************************************/
static int netlink_usb_attach_parse(char *buffer, size_t len, int *v, int *p)
{
	const char *sys_name = NULL;
	struct usb_session_t *pusb_ss;

	unsigned long session_id = 0;

	unsigned char busnum;
	unsigned char devaddr;

	int r = 0, i;
	int vendor, product;

#if 0  //# for debugging
	i = 0;
	r = len;
	while (r-- >= 0) {
		fputc(buffer[i++], stdout);
		fflush(stdout);
	}
	fprintf(stdout, "\n");
#endif

	r = netlink_usb_get_attribute(buffer, len, &sys_name, &busnum, &devaddr);
	if (r < 0) {
//		printf("Failed to netlink_usb_get_attribute\n");
		return -1;
	}

	session_id = (busnum << 8 | devaddr);
	/* open idVendor sysfs */
	vendor = netlink_read_sysfs_attr(sys_name, "idVendor");
	if (vendor < 0) {
//		printf("Failed to netlink_read_sysfs_attr(idVendor)\n");
		return -1;
	}

	/* open idProduct sysfs */
	product = netlink_read_sysfs_attr(sys_name, "idProduct");
	if (product < 0) {
		eprintf("Failed to netlink_read_sysfs_attr(idProduct)\n");
		return -1;
	}

	for (i = 0; i < NETLINK_USB_DEVS_MAX; i++) {
		pusb_ss = &inlink->usb_ss[i];

		if (pusb_ss->sid == session_id) {
			dprintf("already founded usb devices.(%x:%x)\n", vendor, product);
			return -1;
		}
	}

	if (vendor == RTL_8188E_VID && product == RTL_8188E_PID) {
		pusb_ss = &inlink->usb_ss[0];
	} else if (vendor == RTL_8188C_VID && product == RTL_8188C_PID) {
		pusb_ss = &inlink->usb_ss[1];
	} else if (vendor == RTL_8192C_VID && product == RTL_8192C_PID) {
		pusb_ss = &inlink->usb_ss[2];
	} else if (vendor == RTL_8821A_VID && product == RTL_8821A_PID) {
		pusb_ss = &inlink->usb_ss[3];
	} else if (vendor == RTL_8812A_VID && product == RTL_8812A_PID) {
		pusb_ss = &inlink->usb_ss[4];
	} else
		/* not supported device. */
		return -1;

	pusb_ss->sid = session_id;
	pusb_ss->vid = vendor;
	pusb_ss->pid = product;

	*v = vendor; *p = product;

//	printf("attached usb session id %x\n", session_id);
	return 0;
}

#if 0
/*****************************************************************************
* @brief    parse parts of netlink message (for usb)
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netlink_usb_get_interface(char *buffer, size_t len, int *iclass)
{
	const char *tmp;

	*iclass  = 0;

	tmp = netlink_message_parse(buffer, len, "INTERFACE");
	if (NULL == tmp)
		return -1;

	*iclass = (int)(strtoul(tmp, NULL, 10) & 0xff);

	return 0;
}

/*****************************************************************************
* @brief static int netlink_usb_disk_attach_parse(char *buffer, size_t len)
*****************************************************************************/
static int netlink_usb_disk_attach_parse(char *buffer, size_t len)
{
	int bclass, r;

#if 0  //# for debugging
	{
		int i = 0;

		r = len;
		while (r-- >= 0) {
			fputc(buffer[i++], stdout);
			fflush(stdout);
		}
		fprintf(stdout, "\n");
	}
#endif

	r = netlink_usb_get_interface(buffer, len, &bclass);
	if (r < 0)
		return -1;

	/* To check bInterfaceClass sysfs
	 * 01h -> audio class
	 * 02h -> communication and CDC control
	 * 03h -> HID (Human Interface Device)
	 * 06h -> Image
	 * 07h -> Printer
	 * 08h -> Mass Storage
	 * 09h -> Hub
	 * E0h -> Wireless Controller
	 * FFh -> Vendor Specific
	 */
//	dprintf("Founded usb Base class.(%x)\n", bclass);

	return ((bclass == 0x08) ? 0:-1);
}
#endif

static int netlink_check_wifi(void)
{
	struct stat sb;
	char path[1024 + 1];

	int value;
	int r = -1;
	int busnum = 1; /* fixed */

	FILE *f;

	/*
	 * if wi-fi connected, can't detection netlink socket
	 * so manual connection start.
	 */
	if (!app_cfg->ste.b.wifi_ready)
	{
		int d_vid, d_pid;

		r = dev_wifi_is_exist(&d_vid, &d_pid);
		if (r < 0) {
			return -1;
		}

		memset(path, 0, sizeof(path - 1));
		snprintf(path, sizeof(path), "%s/1-1/devnum", NETLINK_USB_PATH);
		r = stat(path, &sb);
		if (r)
			return -1;

		f = fopen(path, "r");
		if (f != NULL) {
			fscanf(f, "%d", &value);
			fclose(f);

			if ((d_vid == RTL_8188E_VID) && (d_pid == RTL_8188E_PID)) {
				inlink->usb_ss[0].sid = (busnum << 8 | value);
				inlink->usb_ss[0].vid = RTL_8188E_VID;
				inlink->usb_ss[0].pid = RTL_8188E_PID;
			} else if((d_vid == RTL_8188C_VID) && (d_pid == RTL_8188C_PID)) {
				inlink->usb_ss[1].sid = (busnum << 8 | value);
				inlink->usb_ss[1].vid = RTL_8188C_VID;
				inlink->usb_ss[1].pid = RTL_8188C_PID;
			} else if((d_vid == RTL_8192C_VID) && (d_pid == RTL_8192C_PID)) {
				inlink->usb_ss[2].sid = (busnum << 8 | value);
				inlink->usb_ss[2].vid = RTL_8192C_VID;
				inlink->usb_ss[2].pid = RTL_8192C_PID;
			} else if((d_vid == RTL_8821A_VID) && (d_pid == RTL_8821A_PID)) {
				inlink->usb_ss[3].sid = (busnum << 8 | value);
				inlink->usb_ss[3].vid = RTL_8821A_VID;
				inlink->usb_ss[3].pid = RTL_8821A_PID;
			} else if((d_vid == RTL_8812A_VID) && (d_pid == RTL_8812A_PID)) {
				inlink->usb_ss[4].sid = (busnum << 8 | value);
				inlink->usb_ss[4].vid = RTL_8812A_VID;
				inlink->usb_ss[4].pid = RTL_8812A_PID;
			}
		} else {
			return -1;
		}
	} else
		return -1;

	return 0;
}

#if 0
static int netlink_check_dialer(void)
{
	struct stat sb;
	char path[1024 + 1];

	int d_vid, d_pid;
	int r = 0, value;
	int busnum = 1; /* fixed */

	FILE *f;

	r = dev_dial_is_exist(&d_vid, &d_pid);
	if (r < 0) {
		return -1;
	}

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%s/1-1/devnum", NETLINK_USB_PATH);
	r = stat(path, &sb);
	if (r)
		return -1;

	f = fopen(path, "r");
	if (f == NULL)
		return -1;

	fscanf(f, "%d", &value);
	fclose(f);

	if ((d_vid == PIX_DIAL_VID) && (d_pid == PIX_DIAL_PID)) {
		inlink->usb_ss[4].sid = (busnum << 8 | value);
		inlink->usb_ss[4].vid = PIX_DIAL_VID;
		inlink->usb_ss[4].pid = PIX_DIAL_PID;
	} else if ((d_vid == HUC_DIAL_VID) && (d_pid == HUC_DIAL_PID)) {
		inlink->usb_ss[6].sid = (busnum << 8 | value);
		inlink->usb_ss[6].vid = HUC_DIAL_VID;
		inlink->usb_ss[6].pid = HUC_DIAL_PID;
	} else if ((d_vid == HIV_DIAL_VID) && (d_pid == HIV_DIAL_PID)) {
		inlink->usb_ss[7].sid = (busnum << 8 | value);
		inlink->usb_ss[7].vid = HIV_DIAL_VID;
		inlink->usb_ss[7].pid = HIV_DIAL_PID;
	} else
		return -1;

	return 0;
}
#endif

/*****************************************************************************
* @brief    netlink_scan_main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_scan_main(void *prm)
{
	app_thr_obj *tObj = &inlink->kObj;

	struct pollfd pfd;
	struct sockaddr_nl nls;

	char msg[1024] = {0,};

	struct iovec iov = {.iov_base = msg, .iov_len = sizeof(msg)};
	struct msghdr meh = {.msg_iov = &iov, .msg_iovlen = 1,
		.msg_name=&nls, .msg_namelen = sizeof(nls)};

	size_t len;

	int ret, detached;
	int cmd, sub;
	int exit = 0, flags;
	int v = 0, p = 0;

	sub = 0; detached = 0;

	// Initialize kernel uevent
	memset(&nls, 0, sizeof(nls));
	nls.nl_family = AF_NETLINK;
	// if the destination is in kernel, always 0
	nls.nl_pid = 0; // self pid
	// from kernel (-1:0xffffffff, all received groups)
	nls.nl_groups = NETLINK_GROUP_KERNEL; //# bit mask group..

	// Open hotplug event netlink socket
	inlink->fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if (inlink->fd < 0) {
		dprintf("netlink scan thread exit!!\n");
		return NULL;
	}

	flags = fcntl(inlink->fd, F_GETFD);
	if (flags < 0) {
		dprintf("fcntl %x\n", flags);
		close(inlink->fd);
		return NULL;
	}

	if (!(flags & FD_CLOEXEC)) {
		fcntl(inlink->fd, F_SETFD, flags | FD_CLOEXEC);
	}

	flags = fcntl(inlink->fd, F_GETFL);
	if (flags < 0) {
		close(inlink->fd);
		return NULL;
	}

	if (!(flags & O_NONBLOCK)) {
		fcntl(inlink->fd, F_SETFL, flags | O_NONBLOCK);
	}

	// Listen to netlink socket (non-block and close execution)
	ret = bind(inlink->fd, (void *)&nls, sizeof(struct sockaddr_nl));
	if (ret < 0) {
		dprintf("scan thread exit (ret %d)!!\n", ret);
		close(inlink->fd);
		return NULL;
	}

	pfd.events = POLLIN;
	pfd.revents = 0;
	pfd.fd = inlink->fd;

	if (!netlink_check_wifi()) {
		app_cfg->ste.b.wifi_ready = 1;
	}

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP)
            break;

		//# wait event
		ret = poll(&pfd, 1, NETLINK_POLL_TIMEOUT); //# timeout 300ms
		if (ret > 0)
		{
			if (pfd.revents & POLLIN) {
				memset(msg, 0, sizeof(msg));

				//len = recv(inlink->fd, buffer, sizeof(buffer), MSG_DONTWAIT);
				len = recvmsg(inlink->fd, &meh, 0);
				if (len > 32)
				{
					ret = netlink_subsystem_parse(msg, len, &sub, &detached);
					if (!ret)
					{
						#if 0
						printf("founded subsystem: usb, removed: %s\n", detached ? "yes" : "no");
						#endif

						if (sub) {
							if (detached) {
								if (netlink_usb_detach_parse(msg, len, &v, &p) == 0)
								{
									if (((v == RTL_8188E_VID) && (p == RTL_8188E_PID)) ||
										((v == RTL_8188C_VID) && (p == RTL_8188C_PID)) ||
										((v == RTL_8192C_VID) && (p == RTL_8192C_PID)) ||
										((v == RTL_8821A_VID) && (p == RTL_8821A_PID))
									   )
									{
										app_cfg->ste.b.wifi_ready = 0;
									}
								}
							} else {
								if (netlink_usb_attach_parse(msg, len, &v, &p) == 0)
								{
									if (((v == RTL_8188E_VID) && (p == RTL_8188E_PID)) ||
										((v == RTL_8188C_VID) && (p == RTL_8188C_PID)) ||
										((v == RTL_8192C_VID) && (p == RTL_8192C_PID)) ||
										((v == RTL_8821A_VID) && (p == RTL_8821A_PID))
									   )
									{
										app_cfg->ste.b.wifi_ready = 1;
									}
								}
							}
						}
					}
				}
			}
		}

		// for next event : wait
		app_msleep(50);
	}

	if (inlink->fd >= 0) {
		close(inlink->fd);
		inlink->fd = -1;
	}

   	tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    Hot Plug thread init/exit function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_hotplug_init(void)
{
	app_thr_obj *tObj;

	//# create netlink receive thread
	tObj = &inlink->kObj;
	if (thread_create(tObj, THR_scan_main, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	aprintf("... done!\n");

	return 0;
}

void app_hotplug_exit(void)
{
    app_thr_obj *tObj;

	/* delete netlink scan object */
    tObj = &inlink->kObj;
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	while(tObj->active)
		app_msleep(20);

    thread_delete(tObj);

    dprintf("... done!\n");
}
