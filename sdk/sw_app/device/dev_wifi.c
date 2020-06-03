/*
 * File : dev_wifi.c
 *
 * Copyright (C) 2013 UDWORKs
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
 * this implements a Wi-Fi hardware library for UCX.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sched.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dev_common.h"
#include "dev_wifi.h"

struct rtl_usb_list_t {
	int d_vid;
	int d_pid;
};

static struct rtl_usb_list_t rtl_list[RTL_USB_MAX_NUM] = {
	{
		.d_vid = RTL_8188E_VID,
		.d_pid = RTL_8188E_PID,
	},
	{
		.d_vid = RTL_8188C_VID,
		.d_pid = RTL_8188C_PID,
	},
	{
		.d_vid = RTL_8192C_VID,
		.d_pid = RTL_8192C_PID,
	},
	{
		.d_vid = RTL_8821A_VID,
		.d_pid = RTL_8821A_PID,
	},
	{
		.d_vid = RTL_8812A_VID,
		.d_pid = RTL_8812A_PID,
	}
};

#if 0
static int check_config(const char *file, const char *basename)
{
	int sfd, dfd;
    char buf[2048];

    struct stat sb;
    int nread, ret;

    ret = access(file, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
        if ((ret != 0) &&
            (chmod(file, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) != 0)) {
            dev_err("Cannot set RW to \"%s\": %s", file, strerror(errno));
            return -1;
        }

        /* return if filesize is at least 10 bytes */
        if (stat(file, &sb) == 0 && sb.st_size > 10) {
            return 0;
        }
    } else if (errno != ENOENT) {
        dev_err("Cannot access \"%s\": %s", file, strerror(errno));
        return -1;
    }

    sfd = open(basename, O_RDONLY);
    if (sfd < 0) {
        dev_err("Cannot open \"%s\": %s", basename, strerror(errno));
        return -1;
    }

    dfd = open(file, O_CREAT|O_RDWR, 0660);
    if (dfd < 0) {
        close(sfd);
        dev_err("Cannot create \"%s\": %s", file, strerror(errno));
        return -1;
    }

    while ((nread = read(sfd, buf, sizeof(buf))) != 0) {
        if (nread < 0) {
            dev_err("Error reading \"%s\": %s", basename, strerror(errno));
            close(sfd); close(dfd);
            unlink(file);
            return -1;
        }

        write(dfd, buf, nread);
    }

    close(dfd); close(sfd);

    /* chmod is needed because open() didn't set permisions properly */
    if (chmod(file, 0660) < 0) {
        dev_err("Error changing permissions of %s to 0660: %s",
             file, strerror(errno));
        unlink(file);
        return -1;
    }

    return 0;
}
#endif

static int get_mac_id(char *mac)
{
	struct ifreq ifreq;
	char temp[32] = {}; /* initialize */
	char hwaddr[32] = {};    //# HW address-mac
	char *ifce = "wlan0";
	int ret, fd;

	if (mac == NULL)
		return -1;

	memset(&ifreq, 0, sizeof(struct ifreq));
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		dev_err("Socket creation failed, this is a fatal error\n");
		return -1;
	}
	/* I want to get an IPv4 IP address */
	ifreq.ifr_hwaddr.sa_family = AF_INET;

	memset(hwaddr, 0, 32);
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCGIFHWADDR, &ifreq);
	if (ret >= 0)
		memcpy(hwaddr, ifreq.ifr_hwaddr.sa_data, 8);

	close(fd);

	snprintf(temp, sizeof(temp), "%02X%02X%02X%02X%02X%02X",
				hwaddr[0], hwaddr[1], hwaddr[2],
				hwaddr[3], hwaddr[4], hwaddr[5]);
	strncpy(mac, temp, strlen(temp));

	return 0;
}

static int create_config(char *usr_id, char *usr_pw, int ch, int is_5g, int stealth)
{
	int ret = 0;
	FILE *fp;

	char ssid_buf[32];
	char pass_buf[12];

	/* Before starting the daemon, make sure its config file exists */
	ret = access(HOSTAPD_CONFIG, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
      	unlink(HOSTAPD_CONFIG);
    }

	fp = fopen(HOSTAPD_CONFIG, "wb");
	if (fp == NULL) {
		dev_err("couldn't create %s config file\n", HOSTAPD_CONFIG);
		return -1;
	}

	/* update ssid */
	memset(ssid_buf, 0, 32);
	if (strcmp(usr_id, "") == 0) {
		/* set default MAC id */
		ret = get_mac_id(ssid_buf);
		if (ret < 0) {
			/* Failed to init driver */
			fclose(fp);
			return -1;
		}
		/* copy to system config buffer */
		strncpy(usr_id, ssid_buf, 32);
	} else {
		strncpy(ssid_buf, usr_id, 32);
	}

	/* update password */
	memset(pass_buf, 0, 12);
	if (strcmp(usr_pw, "") == 0) {
		/* set default password */
		strncpy(pass_buf, "1234567890", 12);
	} else {
		strncpy(pass_buf, usr_pw, strlen(usr_pw));
	}

	/* write hostapd config */
	fprintf(fp, "##### hostapd configuration file ########################\n");
	fprintf(fp, "interface=wlan0\n");
	fprintf(fp, "ctrl_interface=/var/run/hostapd\n");
	fprintf(fp, "ssid=%s\n", ssid_buf);
	fprintf(fp, "channel=%d\n", ch);
	fprintf(fp, "wpa=2\n");
	fprintf(fp, "wpa_passphrase=%s\n", pass_buf);
	fprintf(fp, "\n");
	fprintf(fp, "##### Wi-Fi Protected Setup (WPS) #######################\n");
	fprintf(fp, "eap_server=0\n");
	fprintf(fp, "wps_state=2\n");
	fprintf(fp, "uuid=12345678-9abc-def0-1234-56789abcdef0\n");
	fprintf(fp, "device_name=RTL8192CU\n");
	fprintf(fp, "manufacturer=Realtek\n");
	fprintf(fp, "model_name=RTW_SOFTAP\n");
	fprintf(fp, "model_number=WLAN_CU\n");
	fprintf(fp, "serial_number=12345\n");
	fprintf(fp, "device_type=6-0050F204-1\n");
	fprintf(fp, "os_version=01020300\n");
	fprintf(fp, "config_methods=label display push_button keypad\n");
	fprintf(fp, "##### default configuration #############################\n");
	fprintf(fp, "driver=rtl871xdrv\n");
	fprintf(fp, "beacon_int=100\n");

	if (is_5g) {
		fprintf(fp, "hw_mode=a\n");
	} else {
		fprintf(fp, "hw_mode=g\n");
	}
	fprintf(fp, "ieee80211n=1\n"); //# 1->802.11n on 0->802.11n off
	fprintf(fp, "wme_enabled=1\n");
	fprintf(fp, "ht_capab=[SHORT-GI-20][SHORT-GI-40][HT40+]\n");
	fprintf(fp, "wpa_key_mgmt=WPA-PSK\n");
	fprintf(fp, "wpa_pairwise=CCMP\n");
	fprintf(fp, "max_num_sta=2\n");
	fprintf(fp, "wpa_group_rekey=86400\n");

	fflush(fp);
	fclose(fp);

	/* chmod is needed because open() didn't set permisions properly */
	chmod(HOSTAPD_CONFIG, 0660);

	return 0;
}

/****************************************************
 * NAME : int dev_wifi_get_module_state(const char *name)
 *
 * Desc   : to check load state of driver module
 ****************************************************/
int dev_wifi_get_module_state(const char *name)
{
    FILE *proc;

    char line[256] = {};

	if (name == NULL)
		return 0;

    if ((proc = fopen("/proc/modules", "r")) == NULL) {
        dev_err("Could not open /proc/modules!!\n");
        return 0;
    }

    while ((fgets(line, sizeof(line), proc)) != NULL) {
        if (strncmp(line, name, strlen(name)) == 0) {
            fclose(proc);
            return 1;
        }
    }
    fclose(proc);

    return 0;
}

/****************************************************
 * NAME : int dev_wifi_is_exist(void)
 ****************************************************/
int dev_wifi_is_exist(int *dst_vid, int *dst_pid)
{
	struct rtl_usb_list_t *pusb;
	int i, ret;

	*dst_vid = 0; *dst_pid = 0;

	pusb = rtl_list;
	for (i = 0; i < ARRAY_SIZE(rtl_list); i++, pusb++) {
		ret = dev_usb_is_exist(pusb->d_vid, pusb->d_pid);
		if (ret) {
			dev_dbg("founded device [%x, %x]\n", pusb->d_vid, pusb->d_pid);
			*dst_vid = pusb->d_vid;  *dst_pid = pusb->d_pid;
			return 0;
		}
	}

	return (-1); //# error
}

/****************************************************
 * NAME : int dev_wifi_link_up(int enable)
 *
 * Desc   : link up wi-fi hardware.
 *
 * INPUT  :
 *   PARAMETERS:
 *         int enable
 *             nonzero : activate wi-fi device.
 *             0       : disable wi-fi device.
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_ifconfig(int enable)
{
	struct ifreq ifreq;
	int skfd, r = 0;

	memset(&ifreq, 0, sizeof(struct ifreq));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (skfd < 0) {
  		dev_err("Socket creation failed, this is a fatal error\n");
  		return -1;
  	}

  	/* I want to get an IPv4 IP address */
  	ifreq.ifr_addr.sa_family = AF_INET;
  	strncpy(ifreq.ifr_name, WIFI_IFACE_NAME, sizeof(ifreq.ifr_name));
  	r = ioctl(skfd, SIOCGIFFLAGS, &ifreq);
  	if (r < 0) {
  		dev_err("dev %s failed to SIOCGIFFLAGS ioctl\n", WIFI_IFACE_NAME);
  		close(skfd);
  		return -1;
  	}

  	if (enable)
  		ifreq.ifr_flags |= IFF_UP;
  	else
  		ifreq.ifr_flags &= ~IFF_UP;

  	r = ioctl(skfd, SIOCSIFFLAGS, &ifreq);
  	if (r < 0) {
  		dev_err("dev %s failed to SIOCSIFFLAGS ioctl\n", WIFI_IFACE_NAME);
  		r = -1;
  	}

	close(skfd);
	return r;
}

/****************************************************
 * NAME : int dev_wifi_set_ip(const char *ipaddr)
 *
 * Desc   : set ip address to wi-fi hardware.
 *
 * INPUT  :
 *   PARAMETERS:
 *         const char *ipaddr : ip address (ascii value)
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_set_ip(const char *ipaddr)
{
	struct ifreq ifreq;
	struct sockaddr_in socket_in;

	int skfd, r = 0;

	memset(&ifreq, 0, sizeof(struct ifreq));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (skfd < 0) {
  		dev_err("Socket creation failed, this is a fatal error\n");
  		return -1;
  	}

	strncpy(ifreq.ifr_name, WIFI_IFACE_NAME, sizeof(ifreq.ifr_name));

  	/* I want to get an IPv4 IP address */
  	ifreq.ifr_addr.sa_family = AF_INET;
  	r = ioctl(skfd, SIOCGIFFLAGS, &ifreq);
  	if (r < 0)
  		goto err_socket;

  	socket_in.sin_family = AF_INET;
	//socket_in.sin_addr.s_addr = inet_addr(WIFI_DEFAULT_IP);
	socket_in.sin_addr.s_addr = inet_addr(ipaddr);

	memcpy(&ifreq.ifr_addr, &socket_in, sizeof(struct sockaddr));

	r = ioctl(skfd, SIOCSIFADDR, &ifreq);
	if (r < 0)
		goto err_socket;

  	ifreq.ifr_flags |= IFF_UP;
  	r = ioctl(skfd, SIOCSIFFLAGS, &ifreq);
  	if (r < 0)
  		goto err_socket;

err_socket:
	if (r)
		dev_err("dev %s failed to ioctl\n", WIFI_IFACE_NAME);

	close(skfd);
	return r;
}

/****************************************************
 * NAME : int dev_wifi_update_ssid(const char *ssid)
 *
 * Desc   : set unique ssid for wi-fi AP.
 *
 * INPUT  :
 *   PARAMETERS:
 *         const char *ssid : ssid strings
 *					(ascii value, max 32-character)
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_update_ssid(const char *ssid)
{
	struct stat sb;
	char *pbuf, *sptr;

    int srcfd, destfd;
    int nread;

	const char *config_file = HOSTAPD_CONFIG;

    if (stat(config_file, &sb) != 0)
        return -1;

    pbuf = malloc(sb.st_size+32);
    if (!pbuf)
        return 0;

    srcfd = open(config_file, O_RDONLY);
    if (srcfd < 0) {
        dev_err("Cannot open \"%s\": %s", config_file, strerror(errno));
        free(pbuf);
        return 0;
    }

    nread = read(srcfd, pbuf, sb.st_size);
    close(srcfd);
    if (nread < 0) {
        dev_err("Cannot read \"%s\": %s", config_file, strerror(errno));
        free(pbuf);
        return 0;
    }

    if ((sptr = strstr(pbuf, "ssid=")))
	{
        char *iptr = sptr + strlen("ssid=");
        int ilen = 0;
        int mlen = strlen(ssid);

        if (strncmp(ssid, iptr, mlen) != 0) {
            while (((ilen + (iptr - pbuf)) < nread) && (iptr[ilen] != '\n'))
                ilen++;

            mlen = ((ilen >= mlen) ? ilen : mlen) + 1;
            memmove(iptr + mlen, iptr + ilen + 1, nread - (iptr + ilen + 1 - pbuf));
            memset(iptr, '\n', mlen);
            memcpy(iptr, ssid, strlen(ssid));
            destfd = open(config_file, O_RDWR, 0660);
            if (destfd < 0) {
                dev_err("Cannot update \"%s\": %s", config_file, strerror(errno));
                free(pbuf);
                return -1;
            }

            write(destfd, pbuf, nread + mlen - ilen -1);
            close(destfd);
        }
    }

	if (pbuf)
    	free(pbuf);

    return 0;
}

/****************************************************
 * NAME : int dev_wifi_update_passwd(const char *passwd)
 *
 * Desc   : set unique password for wi-fi AP.
 *
 * INPUT  :
 *   PARAMETERS:
 *         const char *passwd : password strings
 *					(ascii value, min: 10-character)
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_update_passwd(const char *passwd)
{
	struct stat sb;
	char *pbuf, *sptr;

    int srcfd, destfd;
    int nread;

	char param[256];

	const char *config_file = HOSTAPD_CONFIG;

    if (stat(config_file, &sb) != 0)
        return -1;

    pbuf = malloc(sb.st_size);
    if (!pbuf)
        return 0;

    srcfd = open(config_file, O_RDONLY);
    if (srcfd < 0) {
        dev_err("Cannot open \"%s\": %s", config_file, strerror(errno));
        free(pbuf);
        return 0;
    }

    nread = read(srcfd, pbuf, sb.st_size);
    close(srcfd);
    if (nread < 0) {
        dev_err("Cannot read \"%s\": %s", config_file, strerror(errno));
        free(pbuf);
        return 0;
    }

	memset(param, 0, sizeof(param));
	strcpy(param, passwd);

    if ((sptr = strstr(pbuf, "wpa_passphrase=")))
    {
        char *iptr = sptr + strlen("wpa_passphrase=");
        int ilen = 0;
        int mlen = strlen(passwd);

        if (strncmp(param, iptr, mlen) != 0) {
            while (((ilen + (iptr - pbuf)) < nread) && (iptr[ilen] != '\n'))
                ilen++;

            mlen = ((ilen >= mlen) ? ilen : mlen) + 1;
            memmove(iptr + mlen, iptr + ilen + 1, nread - (iptr + ilen + 1 - pbuf));
            memset(iptr, '\n', mlen);
            memcpy(iptr, param, strlen(param));

            destfd = open(config_file, O_RDWR, 0660);
            if (destfd < 0) {
                dev_err("Cannot update \"%s\": %s", config_file, strerror(errno));
                free(pbuf);
                return -1;
            }

            write(destfd, pbuf, nread + mlen - ilen -1);
            close(destfd);
        }
    }

	if (pbuf)
    	free(pbuf);

    return 0;
}

/****************************************************
 * NAME : int dev_wifi_load(const char *module_path, const char *module_name)
 *
 * Desc   : insmod the Wi-Fi driver.
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
void dev_wifi_load(const char *module_path)
{
	char buf[256];
	FILE *f = NULL;

	/* load driver */
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "/sbin/insmod %s", module_path);

	f = popen(buf, "r");
	if (f != NULL)
		pclose(f);
}

/****************************************************
 * NAME : int dev_wifi_unload(const char *module_name)
 *
 * Desc   : rmmod the Wi-Fi driver.
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
void dev_wifi_unload(const char *module_name)
{
	char buf[256] = {};

	/* unload driver */
	snprintf(buf, sizeof(buf), "/sbin/rmmod %s", module_name);
  	dev_execlp(buf);
}

/****************************************************
 * NAME : int dev_wifi_on(const char *ssid, const char *passwd, int channel, int is_5g)
 *
 * Desc   : activate the Wi-Fi Soft AP mode.
 *
 * INPUT  :
 *   PARAMETERS:
 *       const char *ssid   : unique ssid.
 *       const char *passwd : unique password.
 *		 int channel		: select 1 ~ 11 channel (for future use)
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_on(const char *ssid, const char *passwd, int channel, int is_5g, int stealth)
{
	char buff[256] = {};
	int ret;

	/* Before starting the daemon, make sure its config file exists */
	ret = create_config((char *)ssid, (char *)passwd, channel, is_5g, stealth);
	if (ret < 0) {
		/* Failed to wi-fi start */
		return ret;
	}

	memset(buff, 0, sizeof(buff));
	snprintf(buff, sizeof(buff), "/usr/sbin/hostapd %s -B -P %s", HOSTAPD_CONFIG,
					HOSTAPD_PID_PATH);
	dev_execlp(buff);
	sleep(1); //#TODO

#if 0
	/* network setting and link_up */
	if (dev_wifi_set_ip(WIFI_DEFAULT_IP) != 0) {
		dev_err("Failed to default IP setting!!\n");
		return -1;
	}
#else
	dev_execlp("/sbin/ifconfig wlan0 192.168.0.1 up");
#endif

	/* dhcpd start */
	memset(buff, 0, sizeof(buff));
	snprintf(buff, sizeof(buff), "/usr/sbin/udhcpd /etc/udhcpd.conf");
	dev_execlp(buff);

	return 0;
}

/****************************************************
 * NAME : int dev_wifi_off(void)
 *
 * Desc   : stop the Wi-Fi Soft AP mode.
 *
 * INPUT  :
 *   PARAMETERS: none.
 *
 * OUTPUT :
 *   RETURN : Error Code
 *       Type  : int
 *       Values:
 *           0        success.
 *           -1       failure.
 ****************************************************/
int dev_wifi_off(void)
{
	int pid = 0;

	FILE *pid_file;

    pid_file = fopen(HOSTAPD_PID_PATH, "r");
    if (pid_file == NULL) {
    	dev_err("couldn't open %s\n", HOSTAPD_PID_PATH);
    	return -1;
    }

    fscanf(pid_file, "%d", &pid);
    fclose(pid_file);
    unlink((const char *)HOSTAPD_PID_PATH);

    kill(pid, SIGKILL);
    waitpid(pid, NULL, 0);

	/* wifi-link down */
//	dev_wifi_ifconfig(WIFI_LINK_DOWN);
  	dev_execlp("/sbin/ifconfig wlan0 down");

  	/* dhcpd stop */
  	dev_execlp("killall udhcpd");

	return 0;
}
