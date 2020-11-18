/*
 * File : poll_wlan.c
 *
 * Copyright (C) 2020 LF
 *
 */
/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define __USE_GNU
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>

#include "netmgr_ipc_cmd_defs.h"
#include "poll_wlan.h"
#include "common.h"
#include "event_hub.h"
#include "main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define NETLINK_POLL_TIMEOUT	100//300
#define NETLINK_GROUP_KERNEL	1
#define NETLINK_USB_PATH		"/sys/bus/usb/devices"

typedef struct {
	int sid;   /* busnum << 8 | devnum */
	int vid;   /* vendor id */
	int pid;   /* product id */

} netlink_session_t;

typedef struct {
	app_thr_obj pObj; /* device detect */
	
	netlink_session_t usb_ss[NETMGR_USB_MAX_NUM];
	int fd;  /* netlink file descriptor */
	
} netmgr_poll_wlan_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static netmgr_poll_wlan_t t_proc;
static netmgr_poll_wlan_t *iwlan = &t_proc;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

//## ------------------------------- NETLINK USB PARSER -------------------------------------------------------
static const char *netlink_message_parse (const char *buffer, size_t len, const char *key)
{
	size_t keylen = strlen(key);
	size_t offset;

	for (offset = 0 ; offset < len && '\0' != buffer[offset]; 
			offset += strlen(buffer + offset) + 1) 
	{
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
	FILE *f = NULL;
	char filename[1024 + 1];
	char buf[256 + 1] = {0,};
	char *r;
	
	int value;

	snprintf(filename, sizeof(filename), "%s/%s/%s", NETLINK_USB_PATH, devname, attr);
	f = fopen(filename, "r");
	if (f != NULL) 
	{ 
		r = fgets(buf, 255, f);
		fclose(f);
		sscanf(buf, "%x", &value);
		
		if (value < 0) {
			dprintf("%s contains a negative value\n", filename);
			return -1;
		}	
	} else {
		dprintf("open %s failed\n", filename);
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
		return -1;
	}

	/* 0-> hub, 1-> usb port 1, 2-> usb port 2*/
	*busnum = (unsigned char)(strtoul(tmp, NULL, 10) & 0xff);
	if (*busnum < 0 || *busnum > 2) {
		/* invalid bus number (olny supported bus: 0, 1, 2)*/
		return -1;
	}

	tmp = netlink_message_parse(buffer, len, "DEVNUM");
	if (NULL == tmp) {
		return -1;
	}
	*devaddr = (unsigned char)(strtoul(tmp, NULL, 10) & 0xff);

	tmp = netlink_message_parse(buffer, len, "DEVPATH");
	if (NULL == tmp) {
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
	netlink_session_t *pusb_ss;

	unsigned long session_id = 0;
	unsigned char busnum;
	unsigned char devaddr;
	int i, r;

	r = netlink_usb_get_attribute(buffer, len, &sys_name, &busnum, &devaddr);
	if (r < 0) {
		return -1;
	}
	session_id = (busnum << 8 | devaddr);
	
	for (i = 0; i < NETMGR_USB_MAX_NUM; i++) {
		pusb_ss = &iwlan->usb_ss[i];
		if (pusb_ss->sid == session_id) {
			break;
		}
	}

	if (i >= NETMGR_USB_MAX_NUM) {
		eprintf("Not found usb session id (det = %lx)!!\n", session_id);
		return -1;
	}

	pusb_ss = &iwlan->usb_ss[i];
	pusb_ss->sid = 0; //# clear session.

	*v = pusb_ss->vid; 
	*p = pusb_ss->pid;

	return 0;
}

/*****************************************************************************
* @brief static int netlink_usb_attach_parse(char *buffer, size_t len, int *v, int *p)
*****************************************************************************/
static int netlink_usb_attach_parse(char *buffer, size_t len, int *v, int *p)
{
	const char *sys_name = NULL;
	netlink_session_t *pusb_ss;

	unsigned long session_id = 0;
	unsigned char busnum;
	unsigned char devaddr;

	int r = 0, i;
	int vendor, product;

	r = netlink_usb_get_attribute(buffer, len, &sys_name, &busnum, &devaddr);
	if (r < 0) {
		return -1;
	}

	session_id = (busnum << 8 | devaddr);
	/* open idVendor sysfs */
	vendor = netlink_read_sysfs_attr(sys_name, "idVendor");
	if (vendor < 0) {
		return -1;
	}

	/* open idProduct sysfs */
	product = netlink_read_sysfs_attr(sys_name, "idProduct");
	if (product < 0) {
		eprintf("Failed to netlink_read_sysfs_attr(idProduct)\n");
		return -1;
	}

	for (i = 0; i < NETMGR_USB_MAX_NUM; i++) {
		pusb_ss = &iwlan->usb_ss[i];
		if (pusb_ss->sid == session_id) {
			eprintf("already founded usb devices.(%x:%x)\n", vendor, product);
			return -1;
		}
	}

	if (vendor == RTL_8188E_VID && product == RTL_8188E_PID) {
		pusb_ss = &iwlan->usb_ss[0];
	} else if (vendor == RTL_8188C_VID && product == RTL_8188C_PID) {
		pusb_ss = &iwlan->usb_ss[1];
	} else if (vendor == RTL_8192C_VID && product == RTL_8192C_PID) {
		pusb_ss = &iwlan->usb_ss[2];
	} else if (vendor == RTL_8821A_VID && product == RTL_8821A_PID) {
		pusb_ss = &iwlan->usb_ss[3];
	} else if (vendor == RTL_8812A_VID && product == RTL_8812A_PID) {
		pusb_ss = &iwlan->usb_ss[4];
	} else
		/* not supported device. */
		return -1;

	pusb_ss->sid = session_id;
	pusb_ss->vid = vendor;
	pusb_ss->pid = product;

	*v = vendor; *p = product;

	return 0;
}

static int __is_connected_wlan(void)
{
	struct stat sb;
	char path[1024 + 1];
	
	int d_vid, d_pid, value;
	int r = -1;
	int busnum = 1; /* fixed */
	FILE *f = NULL;
	
	r = netmgr_wlan_is_exist(&d_vid, &d_pid);
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
			iwlan->usb_ss[0].sid = (busnum << 8 | value);
			iwlan->usb_ss[0].vid = RTL_8188E_VID;
			iwlan->usb_ss[0].pid = RTL_8188E_PID;
		} else if((d_vid == RTL_8188C_VID) && (d_pid == RTL_8188C_PID)) {
			iwlan->usb_ss[1].sid = (busnum << 8 | value);
			iwlan->usb_ss[1].vid = RTL_8188C_VID;
			iwlan->usb_ss[1].pid = RTL_8188C_PID;
		} else if((d_vid == RTL_8192C_VID) && (d_pid == RTL_8192C_PID)) {
			iwlan->usb_ss[2].sid = (busnum << 8 | value);
			iwlan->usb_ss[2].vid = RTL_8192C_VID;
			iwlan->usb_ss[2].pid = RTL_8192C_PID;
		} else if((d_vid == RTL_8821A_VID) && (d_pid == RTL_8821A_PID)) {
			iwlan->usb_ss[3].sid = (busnum << 8 | value);
			iwlan->usb_ss[3].vid = RTL_8821A_VID;
			iwlan->usb_ss[3].pid = RTL_8821A_PID;
		} else if((d_vid == RTL_8812A_VID) && (d_pid == RTL_8812A_PID)) {
			iwlan->usb_ss[4].sid = (busnum << 8 | value);
			iwlan->usb_ss[4].vid = RTL_8812A_VID;
			iwlan->usb_ss[4].pid = RTL_8812A_PID;
		}
		
		/* USB을 연결하고 부팅하면 netlink에서 검색을 못한다. 수동으로 검색하는 루틴에서 
			* 이를 확인 한 후 vid, pid를 저장함.
			*/
		app_cfg->wlan_vid = d_vid;
		app_cfg->wlan_pid = d_pid;
		return 0;
	} 
	
	return -1;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wlan_poll(void *prm)
{
	app_thr_obj *tObj = &iwlan->pObj;
	int exit = 0, cmd, flags;
	
	struct pollfd pfd;
	struct sockaddr_nl nls;
	char msg[1024] = {0,};

	struct iovec iov = {.iov_base = msg, .iov_len = sizeof(msg)};
	struct msghdr meh = {.msg_iov = &iov, .msg_iovlen = 1,
		.msg_name=&nls, .msg_namelen = sizeof(nls)};

	size_t len;
	int ret, detached, sub;
	int v = 0, p = 0;
	
	tObj->active = 1;
	
	sub = 0; detached = 0;
	// Initialize kernel uevent
	memset(&nls, 0, sizeof(nls));
	nls.nl_family = AF_NETLINK;
	// if the destination is in kernel, always 0
	nls.nl_pid = 0; // self pid
	// from kernel (-1:0xffffffff, all received groups)
	nls.nl_groups = NETLINK_GROUP_KERNEL; //# bit mask group..

	// Open hotplug event netlink socket
	iwlan->fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if (iwlan->fd < 0) {
		eprintf("netlink scan thread exit!!\n");
		return NULL;
	}

	flags = fcntl(iwlan->fd, F_GETFD);
	if (flags < 0) {
		eprintf("fcntl %x\n", flags);
		close(iwlan->fd);
		return NULL;
	}
	
	/* ?? ?????? open? ??? ? */
	if (!(flags & FD_CLOEXEC)) {
		fcntl(iwlan->fd, F_SETFD, flags | FD_CLOEXEC);
	}

	flags = fcntl(iwlan->fd, F_GETFL);
	if (flags < 0) {
		close(iwlan->fd);
		return NULL;
	}

	if (!(flags & O_NONBLOCK)) {
		fcntl(iwlan->fd, F_SETFL, flags | O_NONBLOCK);
	}

	// Listen to netlink socket (non-block and close execution)
	ret = bind(iwlan->fd, (void *)&nls, sizeof(struct sockaddr_nl));
	if (ret < 0) {
		eprintf("scan thread exit (ret %d)!!\n", ret);
		close(iwlan->fd);
		return NULL;
	}

	pfd.events = POLLIN;
	pfd.revents = 0;
	pfd.fd = iwlan->fd;
	
	/* 동글을 연결하고 시스템이 켜지는 경우 netlink로 이벤트 전달이 안됨 */
	if (!app_cfg->ste.bit.wlan) 
	{
		ret = __is_connected_wlan();
		if (!ret) {
			app_cfg->ste.bit.wlan = 1; 
			netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 1);
		} 
	}
	
	while (!exit)
	{
		cmd = tObj->cmd;
        if (cmd == APP_CMD_EXIT) {
		    break;
		}
		
		//# wait USB Wi-Fi event
		ret = poll(&pfd, 1, NETLINK_POLL_TIMEOUT); //# timeout 100ms
		if (ret > 0)
		{
			if (pfd.revents & POLLIN) 
			{
				memset(msg, 0, sizeof(msg));

				//len = recv(inlink->fd, buffer, sizeof(buffer), MSG_DONTWAIT);
				len = recvmsg(iwlan->fd, &meh, 0);
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
										app_cfg->wlan_vid = -1; /* device가 제거된 상태 */
										app_cfg->wlan_pid = -1;
										app_cfg->ste.bit.wlan = 0;
										netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 0);
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
										/* 현재 연결되 USB 장치의 VID와 PID를 저장 */
										app_cfg->wlan_vid = v;
										app_cfg->wlan_pid = p;
										app_cfg->ste.bit.wlan = 1;
										netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 1);
									}
								}
							}
						}
					}
				} //# if (len > 32)
			} //# if (pfd.revents & POLLIN)
			delay_msecs(50);
		} else {
			/* 간헐적으로 부팅 시 인식 안되는 경우가 있어서 poll 형태로 감시 */
			if (!app_cfg->ste.bit.wlan) 
			{
				ret = __is_connected_wlan();
				if (!ret) {
					app_cfg->ste.bit.wlan = 1; 
					netmgr_event_hub_dev_status(NETMGR_DEV_TYPE_WIFI, 1);
				} 
			}
		}
	} 
	
	tObj->active = 0;
	
	return NULL;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) start!
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_wlan_init(void)
{
	app_thr_obj *tObj;
	
	tObj = &iwlan->pObj;
	if (thread_create(tObj, THR_wlan_poll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr usb wifi poll thread\n");
		return EFAIL;
    }
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_poll_wlan_exit(void)
{
	app_thr_obj *tObj;

	/* delete usb scan object */
    tObj = &iwlan->pObj;
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	return 0;
}
