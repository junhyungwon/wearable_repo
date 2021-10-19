/*
 * File : dev_common.c
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
 * this implements a common hardware interface for UBX.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <ctype.h>
#include <wait.h>
#include <errno.h>
#include <linux/rtc.h>
#include <stdint.h>
#include <stdbool.h>
#include <asm/types.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/statfs.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/types.h>
#include <linux/netlink.h>

#define  __user	/* nothing */
#include <mtd/mtd-user.h>

#include "dev_common.h"

#define YEAR	((((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0')) * 10 \
				+ (__DATE__[9] - '0')) * 10 + (__DATE__[10] - '0'))

#define MONTH 	(__DATE__[2] == 'n' ? (__DATE__[1] == 'a' ? 0 : 5) \
			  	:__DATE__[2] == 'b' ? 1 \
			  	:__DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? 2 : 3) \
			  	:__DATE__[2] == 'y' ? 4 \
			  	:__DATE__[2] == 'l' ? 6 \
			  	:__DATE__[2] == 'g' ? 7 \
			  	:__DATE__[2] == 'p' ? 8 \
			  	:__DATE__[2] == 't' ? 9 \
			  	:__DATE__[2] == 'v' ? 10 : 11)

#define DAY		((__DATE__[4] == ' ' ? 0 : __DATE__[4] - '0') * 10 \
				+ (__DATE__[5] - '0'))

#define MTD_MAGIC			"AA55"
#define MTD_PATH			"/dev/mtd7"
#define MTD_SIZE			4194304 //# 4MB, Total 32Blocks
#define MTD_ERASESIZE		131072  
#define MTD_PAGESIZE		2048

/* Maximum MTD device name length */
#define MTD_NAME_MAX 127
/* Maximum MTD device type string length */
#define MTD_TYPE_MAX 64

/* some C lib headers define this for us */
#ifndef MIN	
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
/* glue for linux kernel source */
#define min(a, b) MIN(a, b) 

struct mtd_dev_info {
	int mtd_num;
	int major;
	int minor;
	int type;
	const char type_str[MTD_TYPE_MAX + 1];
	const char name[MTD_NAME_MAX + 1];
	long long size;
	int eb_cnt;
	int eb_size;
	int min_io_size;
	int subpage_size;
	int oob_size;
	int region_cnt;
	unsigned int writable:1;
	unsigned int bb_allowed:1;
};

#define PRETTY_ROW_SIZE 16
#define PRETTY_BUF_LEN 80

static char pagebuf[MTD_PAGESIZE];

/****************************************************
 * NAME : int dev_input_get_bus_num(const char *dev_name)
 ****************************************************/
int dev_input_get_bus_num(const char *dev_name)
{
	FILE *file;

	char buffer[PATH_MAX];
	int find_field = 0;
	char *c;

	file = fopen(INPUT_PROC_PATH, "r");
    if (file == NULL) {
    	dev_err("%s read failed\n", INPUT_PROC_PATH);
       	return -1;
	}

    while (!feof(file)) {
    	memset(buffer, 0, sizeof(buffer));
    	fgets(buffer, sizeof(buffer), file);

    	if (!find_field && strstr(buffer, dev_name) != NULL) {
    		find_field = 1;
    	} else if (find_field) {
    		if (strstr(buffer, "Handlers=") != NULL) {
    			c = strstr(buffer, "event");
    			fclose(file);
    			return atoi(c+strlen("event"));
    		}
    	}
    }
    fclose(file);

    return -1;
}

/****************************************************
 * NAME : void dev_wait_for_msecs(unsigned int msecs)
 ****************************************************/
void dev_wait_for_msecs(unsigned int msecs)
{
  	struct timespec delayTime, elaspedTime;

  	delayTime.tv_sec  = msecs/1000;
  	delayTime.tv_nsec = (msecs%1000)*1000000;

  	nanosleep(&delayTime, &elaspedTime);
}

/****************************************************
 * NAME : int dev_get_board_info(void)
 *
 * Desc : Get Hardware Version Information.
          via /proc/device_version
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *       nonzero : (success)
 *         -1 : failure.
 *
 ****************************************************/
int dev_get_board_info(void)
{
	char buf[256];
	ssize_t nbytes;
	int fd, ret, ver;

    fd = open("/proc/device_version", O_RDONLY);
    if (fd < 0)
    	return -1;

    memset(buf, 0, 256);
    nbytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (nbytes < 0)
        return -1;

	buf[nbytes] = '\0';

	ret = sscanf(buf, "%d", &ver);
	if (ret <= 0)
		return -1;

    return ver;
}

int dev_execlp(char *arg)
{
    int numArg, i, j, k;
    int len, status;

    char exArg[10][64];
	pid_t chId;
	pid_t pid_child;

    if (arg[0] == '\0')
        return 0;

    j = 0; 	k = 0;
	len = strlen(arg);

    for (i = 0; i < len; i++) {
        if (arg[i] == ' ') {
		    exArg[j][k] = '\0';
		    j ++; k = 0;
		} else {
		    exArg[j][k] = arg[i];
		    k ++;
		}
	}

    if (exArg[j][k - 1] == '\n') {
	    exArg[j][k - 1] = '\0';
	} else {
	    exArg[j][k] = '\0';
	}

	numArg = j + 1;

	if (numArg > 10) {
	    dev_dbg("The no of arguments are greater than 10" \
	    		"calling standard system function...\n");
	    return (system(arg));
	}

    chId = fork();
	if (chId == 0) {
	    // child process
	    switch (numArg) {
	    case 1:
	        execlp(exArg[0],exArg[0],NULL);
	        break;
	    case 2:
	        execlp(exArg[0],exArg[0],exArg[1],NULL);
	        break;
	    case 3:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],NULL);
	        break;
	    case 4:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],NULL);
	        break;
	    case 5:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               NULL);
	        break;
	    case 6:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],NULL);
	        break;
	    case 7:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],NULL);
	        break;
	    case 8:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],NULL);
	        break;
	    case 9:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],exArg[8],NULL);
	        break;
	    case 10:
	        execlp(exArg[0],exArg[0],exArg[1],exArg[2],exArg[3],exArg[4],
	               exArg[5],exArg[6],exArg[7],exArg[8],exArg[9],NULL);
	        break;
		}
        dev_err("execlp failed...\n");
	    return -1;
	} else if(chId < 0) {
		dev_err("Failed to create child process\n");
		return -1;
	} else {
		/* parent process */
		/* wait for the completion of the child process */
		/* 3th option WNOHANG->non-block 0->block */
		pid_child = waitpid(chId, &status, 0);
		if (WIFEXITED(status))
			dev_dbg("Chiled exited with the code %d\n", WEXITSTATUS(status));
		else
			dev_err("Child terminated abnormally..\n");
	}

    return 0;
}

/****************************************************
 * NAME : speed_t dev_tty_get_baudrate(int rate)
 ****************************************************/
speed_t dev_tty_get_baudrate(int rate)
{
	switch (rate) {
	case 150: 		return B150;
   	case 300: 		return B300;
   	case 600: 		return B600;
   	case 1200: 		return B1200;
   	case 2400: 		return B2400;
   	case 4800: 		return B4800;
   	case 9600: 		return B9600;
   	case 19200: 	return B19200;
   	case 57600: 	return B57600;
   	case 115200: 	return B115200;
   	case 230400: 	return B230400;
   	case 460800: 	return B460800;
   	case 921600: 	return B921600;

   	/* NOTE! The speed of 38400 is required, if you want to set
	 * an non-standard baudrate. See below!
	 */
	 default: 		return B38400;
	}
}

/****************************************************
 * NAME : dev_net_link_detect(const char *ifce)
 *
 * Desc : detect ifce network
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_net_link_detect(const char *ifce)
{
	struct ifreq ifreq;
	int skfd, r;

	memset(&ifreq, 0, sizeof(struct ifreq));

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (skfd < 0) {
  		dev_err("Socket creation failed, this is a fatal error\n");
  		return NETSTATUS_DOWN;
  	}

  	/* I want to get an IPv4 IP address */
  	ifreq.ifr_addr.sa_family = AF_INET;
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	r = ioctl(skfd, SIOCGIFFLAGS, &ifreq);
	close(skfd);

	if ((r < 0) || !(ifreq.ifr_flags & IFF_UP))
		return NETSTATUS_DOWN;

	return NETSTATUS_UP;
}


/****************************************************
 * NAME : dev_net_link_up(const char *ifce)
 *
 * Desc : link-up ifce network
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_net_link_up(const char *ifce)
{
	struct ifreq ifreq;
	int skfd, r;

	memset(&ifreq, 0, sizeof(struct ifreq));

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (skfd < 0) {
  		dev_err("Socket creation failed, this is a fatal error\n");
  		return -1;
  	}

  	/* I want to get an IPv4 IP address */
  	ifreq.ifr_addr.sa_family = AF_INET;
  	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
  	r = ioctl(skfd, SIOCGIFFLAGS, &ifreq);
  	if (r < 0)
  		return -1;

  	ifreq.ifr_flags |= IFF_UP;
  	r = ioctl(skfd, SIOCSIFFLAGS, &ifreq);
  	if (r < 0)
    {
  		dev_err("dev %s link up failed!!\n", ifce);
    }

	close(skfd);

	return r;
}


/****************************************************
 * NAME : dev_net_link_down(const char *ifce)
 *
 * Desc : link-down ifce network
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_net_link_down(const char *ifce)
{
	char cmd[128];

	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "ifconfig %s 0.0.0.0 down", ifce);

  	return dev_execlp(cmd);
}

/* numeric: & 0x8000: default instead of *,
 *          & 0x4000: host instead of net,
 *          & 0x0fff: don't resolve
 */
static char *INET_rresolve(struct sockaddr_in *s_in, int numeric,
				uint32_t netmask)
{
  	/* addr-to-name cache */
  	struct addr {
    	struct addr *next;
    	struct sockaddr_in addr;
    	int host;
    	char name[1];
  	};
	uint32_t ad;

  	if (s_in->sin_family != AF_INET) {
    	dev_err("rresolve: unsupported address family %d!",
          		s_in->sin_family);
    	errno = EAFNOSUPPORT;
    	return NULL;
  	}

  	ad = s_in->sin_addr.s_addr;

  	dev_dbg("rresolve: %08x, mask %08x, num %08x\n",
  				(unsigned)ad, netmask, numeric);

  	if (ad == INADDR_ANY) {
    	if ((numeric & 0x0FFF) == 0) {
      		if (numeric & 0x8000)
        		return strdup("default");
      		return strdup("*");
    	}
  	}

  	if (numeric & 0x0FFF)
    	return strdup(inet_ntoa(s_in->sin_addr));

	return 0;
}

static void set_flags(char *flagstr, int flags)
{
	int i;

	*flagstr++ = 'U';

	for (i = 0; (*flagstr = flagchars[i]) != 0; i++) {
		if (flags & flagvals[i]) {
			++flagstr;
		}
	}
}

/****************************************************
 * NAME : int net_if_get_gate(const char *ifce, char *gate)
 *
 * Desc : get ifce gateway
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
static int net_if_get_gate(const char *ifce, char *gate)
{
	char devname[64], flags[16];
	char *sdest, *sgw;

	unsigned long dst, gw, m;
	int flgs, ref, use, metric, mtu, win, ir;

	struct sockaddr_in s_addr;
	struct in_addr mask;

	FILE *fp;

	if (!gate) {
		dev_err("LINE %d->invalid argument(NULL)\n", __LINE__);
		return -1;
	}

	fp = fopen("/proc/net/route", "r");
	if (!fp) {
		perror("/proc/net/route");
		return -1;
	}

	/* Skip the first line. */
	if (fscanf(fp, "%*[^\n]\n") < 0) {
		/* Empty or missing line, or read error. */
		fclose(fp);
      	perror("fscanf");
      	return -1;
	}

	while (1) {
		int r;

		r = fscanf(fp, "%63s%lx%lx%X%d%d%d%lx%d%d%d\n",
				devname, &dst, &gw, &flgs, &ref, &use, &metric, &m,
				&mtu, &win, &ir);
		if (r != 11) {
			/* EOF with no (nonspace) chars read. */
			if ((r < 0) && feof(fp)) {
				break;
			}

			strcpy(gate, "0.0.0.0");
			fclose(fp);
      		perror("fscanf");
      		return -1;
		}

		/* Skip interfaces that are down. */
		if (!(flgs & RTF_UP) || strcmp(devname, ifce))
			continue;

		set_flags(flags, (flgs & IPV4_MASK));

		#ifdef RTF_REJECT
		if (flgs & RTF_REJECT) {
			flags[0] = '!';
		}
		#endif

		memset(&s_addr, 0, sizeof(struct sockaddr_in));
		s_addr.sin_family = AF_INET;
		s_addr.sin_addr.s_addr = dst;

		/* 'default' instead of '*' */
		sdest = NULL;
		sdest = INET_rresolve(&s_addr, (0x0FFF | 0x8000), m);
		s_addr.sin_addr.s_addr = gw;
		if (sdest) {
			free(sdest);
			sdest = NULL;
		}

		/* Host instead of net */
		sgw = NULL;
		sgw = INET_rresolve(&s_addr, (0x0FFF | 0x4000), m);
		mask.s_addr = m;
		if (flgs & RTF_GATEWAY) {
			strcpy(gate, sgw);
			free(sgw);
			fclose(fp);
			return 0;
    	}

    	if (sgw)
			free(sgw);
	}

	//# not founded.
	fclose(fp);
	strcpy(gate, "0.0.0.0");

	return 1;
}

/****************************************************
 * NAME : int dev_set_net_if_gate(const char *ifce, char *gate)
 *
 * Desc : set ifce gateway
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_set_net_if_gate(const char *ifce, char *gate)
{
	char cmd[128];
	char tmp[16];
	int ret;

	if (gate == NULL) {
		dev_err("LINE %d->invalid argument(NULL)\n", __LINE__);
		return -1;
	}

	memset(cmd, 0, sizeof(cmd));
	memset(tmp, 0, sizeof(tmp));

	ret = net_if_get_gate(ifce, tmp);
	if (ret < 0)
		return -1;

	if (strcmp((char*)tmp, "0.0.0.0")) {
		snprintf((char*)cmd, sizeof(cmd), "route del default gw %s %s", tmp, ifce);
		ret = dev_execlp((char*)cmd);
	}

	snprintf((char*)cmd, sizeof(cmd), "route add default gw %s %s", gate, ifce);

	return dev_execlp((char*)cmd);
}

/****************************************************
 * NAME : int dev_get_net_info(const char *ifce, dev_net_info_t *inet)
 *
 * Desc : get ifce network information.
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_get_net_info(const char *ifce, dev_net_info_t *inet)
{
	struct ifreq ifreq;
	int ret, fd;

	memset(&ifreq, 0, sizeof(struct ifreq));

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		dev_err("Socket creation failed, this is a fatal error\n");
		return -1;
	}

	/* I want to get an IPv4 IP address */
	ifreq.ifr_hwaddr.sa_family = AF_INET;

	memset(inet->hwaddr, 0, 32);
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCGIFHWADDR, &ifreq);
	if (ret >= 0)
		memcpy(inet->hwaddr, ifreq.ifr_hwaddr.sa_data, 8);

	memset(inet->ip, 0, sizeof(inet->ip));
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCGIFADDR, &ifreq);
	if (ret >= 0) {
		sprintf(inet->ip, "%s",
				inet_ntoa(((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr));
	} else
		strcpy(inet->ip, "0.0.0.0");

	memset(inet->mask, 0, sizeof(inet->mask));
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCGIFNETMASK, &ifreq);
	if (ret >= 0) {
		sprintf(inet->mask, "%s",
				inet_ntoa(((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr));
	}
	else
		strcpy(inet->mask, "255.255.255.0");

	close(fd);

	return net_if_get_gate(ifce, inet->gate);
}

/****************************************************
 * NAME : int dev_set_net_info(const char *ifce, dev_net_info_t *inet)
 *
 * Desc : set ifce network information.
 *
 * INPUT  :
 *   PARAMETERS:
 *
 * OUTPUT :
 *   RETURN :
 *          0 : if success
 *         -1 : failure.
 *
 ****************************************************/
int dev_set_net_info(const char *ifce, dev_net_info_t *inet)
{
	int ret = 0, fd;
	char cmd[32];

	struct ifreq ifreq;
  	struct sockaddr_in sin;

  	if (!strcmp(inet->ip, "0.0.0.0")) {
  		dev_err("LINE %d->invalid argument(NULL)\n", __LINE__);
  		return -1;
  	}

	memset(&ifreq, 0, sizeof(struct ifreq));

  	fd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (fd < 0) {
  		dev_err("Socket creation failed, this is a fatal error\n");
  		return -1;
  	}

  	/* I want to get an IPv4 IP address */
	ifreq.ifr_hwaddr.sa_family = AF_INET;

  	if (inet->type == DEV_NET_TYPE_DHCP) {
  		sprintf(cmd, "udhcpc -n -i %s", ifce);
  		ret = dev_execlp(cmd);

  	} else if (inet->type == DEV_NET_TYPE_STATIC) {
		//memset(&sin, 0, sizeof(struct sockaddr));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr(inet->ip);

		memcpy(&ifreq.ifr_addr, &sin, sizeof(struct sockaddr));
		strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
		ret = ioctl(fd, SIOCSIFADDR, &ifreq);
		if (ret < 0) {
			close(fd);
			return -1;
		}

		sin.sin_addr.s_addr = inet_addr(inet->mask);
		memcpy(&ifreq.ifr_addr, &sin, sizeof(struct sockaddr));
		strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
		ret = ioctl(fd, SIOCSIFNETMASK, &ifreq);
		if (ret < 0) {
			dev_err("netmask: Invalid argument\n");
			close(fd);
			return -1;
		}

		ret = dev_set_net_if_gate(ifce, inet->gate);
		if (ret < 0) {
			close(fd);
			return -1;
		}
	}

	close(fd);

	return ret;
}

/*
 * lsusb. (format)
 * Bus 001 Device 003: ID 1c9e:7801
 */
/****************************************************
 * NAME : int dev_usb_is_exist(int usb_v, int usb_p)
 ****************************************************/
int dev_usb_is_exist(int usb_v, int usb_p)
{
	FILE *lsusbfs;

	char cmd[128] = {0,};
	char buffer[256] = {0,};

	char vendor[5];
	char product[5];

	char *save_ptr;

	vendor[4] = 0;
	snprintf(vendor, sizeof(vendor), "%04x", usb_v);

	product[4] = 0;
	snprintf(product, sizeof(product), "%04x", usb_p);

	snprintf(cmd, sizeof(cmd), "/usr/bin/lsusb");
	lsusbfs = popen(cmd, "r");
	if (lsusbfs == NULL) {
		dev_err("couldn't access %s\n", cmd);
		return 0;
	}

	while (fgets(buffer, 255, lsusbfs) != NULL) {
		char *v, *p;
		/* %*s->discard input */
		memset(cmd, 0, sizeof(cmd));
		sscanf(buffer, "%*s%*s%*s%*s%*s%s", cmd);
		/* splite ":" */
		if (cmd != NULL) {
			v = strtok_r(cmd, ":", &save_ptr);
			p = strtok_r(NULL, ":", &save_ptr);

			if ((strncmp(v, vendor, 4) == 0) &&
				(strncmp(p, product, 4) == 0))
			{
				pclose(lsusbfs);
				return 1; //# founded usb
			}
		}
	}
	pclose(lsusbfs);

	return 0;
}

/******************************************************
 * NAME : int dev_rtc_set_time(struct tm set_tm)
 ******************************************************/
int dev_rtc_set_time(struct tm set_tm)
{
	int fd = -1, ret;

	fd = open("/dev/rtc0", O_RDONLY);
	if (fd < 0) {
		dev_err("Failed to open /dev/rtc0 device.\n");
		return fd;
	}

	ret = ioctl(fd, RTC_SET_TIME, &set_tm);
	if (ret < 0) {
		dev_err("Failed to ioctl (RTC_SET_TIME)!\n");
	}
	close(fd);

	return ret;
}

//########################### MTD #######################################################
static void pretty_dump_to_buffer(const unsigned char *buf, size_t len,
		char *linebuf, size_t linebuflen, bool pagedump, bool ascii,
		unsigned long long prefix)
{
	static const char hex_asc[] = "0123456789abcdef";
	unsigned char ch;
	unsigned int j, lx = 0, ascii_column;

	if (pagedump)
		lx += sprintf(linebuf, "0x%.8llx: ", prefix);
	else
		lx += sprintf(linebuf, "  OOB Data: ");

	if (!len)
		goto nil;
		
	if (len > PRETTY_ROW_SIZE)	/* limit to one line at a time */
		len = PRETTY_ROW_SIZE;

	for (j = 0; (j < len) && (lx + 3) <= linebuflen; j++) {
		ch = buf[j];
		linebuf[lx++] = hex_asc[(ch & 0xf0) >> 4];
		linebuf[lx++] = hex_asc[ch & 0x0f];
		linebuf[lx++] = ' ';
	}
	
	if (j)
		lx--;

	ascii_column = 3 * PRETTY_ROW_SIZE + 14;

	if (!ascii)
		goto nil;

	/* Spacing between hex and ASCII - ensure at least one space */
	lx += sprintf(linebuf + lx, "%*s",
			MAX((int)MIN(linebuflen, ascii_column) - 1 - lx, 1),
			" ");

	linebuf[lx++] = '|';
	for (j = 0; (j < len) && (lx + 2) < linebuflen; j++)
		linebuf[lx++] = (isascii(buf[j]) && isprint(buf[j])) ? buf[j]
			: '.';
	linebuf[lx++] = '|';
nil:
	linebuf[lx++] = '\n';
	linebuf[lx++] = '\0';
}

static int mtd_valid_erase_block(const struct mtd_dev_info *mtd, int eb)
{
	if (eb < 0 || eb >= mtd->eb_cnt) {
		dev_err("bad eraseblock number %d, mtd has %d eraseblocks",
		       eb, mtd->eb_cnt);
		return -1;
	}
	return 0;
}

static int mtd_is_bad(const struct mtd_dev_info *mtd, int fd, int eb)
{
	int ret;
	loff_t seek;

	ret = mtd_valid_erase_block(mtd, eb);
	if (ret)
		return ret;

	if (!mtd->bb_allowed)
		return 0;

	seek = (loff_t)eb * mtd->eb_size;
	ret = ioctl(fd, MEMGETBADBLOCK, &seek);
	if (ret == -1) {
		dev_err("MEMGETBADBLOCK ioctl failed for eraseblock %d\n", eb);
		return -1;
	}
	return ret;
}

static int mtd_read(const struct mtd_dev_info *mtd, int fd, int eb, int offs,
	     void *buf, int len)
{
	int ret, rd = 0;
	off_t seek;

	ret = mtd_valid_erase_block(mtd, eb);
	if (ret)
		return ret;

	if (offs < 0 || offs + len > mtd->eb_size) {
		dev_err("bad offset %d or length %d, eraseblock size is %d\n",
		       offs, len, mtd->eb_size);
		return -1;
	}

	/* Seek to the beginning of the eraseblock */
	seek = (off_t)eb * mtd->eb_size + offs;
	if (lseek(fd, seek, SEEK_SET) != seek) {
		dev_err("cannot seek offset!\n");
		return -1;
	}

	while (rd < len) 
	{
		ret = read(fd, buf, len);
		if (ret < 0) {
			dev_err("cannot read %d bytes from mtd (eraseblock %d, offset %d)\n",
					  len, eb, offs);
			return -1;
		}
		rd += ret;
	}

	return 0;
}

static int mtd_write(const struct mtd_dev_info *mtd, int fd, int eb,
	      int offs, void *data, int len)
{
	int ret;
	off_t seek;

	ret = mtd_valid_erase_block(mtd, eb);
	if (ret)
		return ret;

	if (offs < 0 || offs + len > mtd->eb_size) {
		dev_err("bad offset %d or length %d, mtd eraseblock size is %d",
		       offs, len, mtd->eb_size);
		return -1;
	}
	
	/* Calculate seek address */
	seek = (off_t)eb * mtd->eb_size + offs;
	//fprintf(stderr, "mtd write seek %ld, eb = %d, offs = %ld\n", seek, eb, offs);
	if (data) {
		/* Seek to the beginning of the eraseblock */
		if (lseek(fd, seek, SEEK_SET) != seek) {
			dev_err("cannot seek to offset %ld\n", seek);
			return -1;
		}
		
		ret = write(fd, data, len);
		if (ret != len) {
			dev_err("cannot write %d bytes to mtd (eraseblock %d, offset %d)\n",
					  len, eb, offs);
			return -1;
		}
	}

	return 0;
}

static int mtd_nand_read(char *data, int start, int length)
{
	struct mtd_dev_info mtd;
	
	int fd = -1;
	int ofs, bs, count, end_addr;
	
	int blockstart = 1;
	int firstblock = 1;
	int badblock = 0;
	
	unsigned char readbuf[MTD_PAGESIZE];
	char pretty_buf[PRETTY_BUF_LEN];
	
	/* Open the mtd7 device */
	fd = open(MTD_PATH, O_RDWR);
	if (fd == -1) {
		dev_err("failed to open %s\n", MTD_PATH);
		return -1;
	}
	
	/* set mtd structure */
	mtd.size        = MTD_SIZE;
	/* mtd.eb_size = /sys/class/mtd/mtd7/erasesize = 131072bytes(128KB), 1page */
	mtd.eb_size     = MTD_ERASESIZE;
	/* mtd.min_io_size = /sys/class/mtd/mtd7/writesize = 2048bytes, 1page */
	mtd.min_io_size = MTD_PAGESIZE;
	mtd.eb_cnt = mtd.size / mtd.eb_size;
	mtd.type   = MTD_NANDFLASH;
	mtd.bb_allowed = !!(mtd.type == MTD_NANDFLASH);
	
	/*
	 * start --> 0, 2048, 4096......
	 */
	if (start & (mtd.min_io_size - 1)) {
		fprintf(stderr, "the start address is not page-aligned!\n");
		return -1;
	}
	
	if (length)
		end_addr = start + length;
	if (!length || end_addr > mtd.size)
		end_addr = mtd.size;
		
	bs = mtd.min_io_size;
	memset(readbuf, 0xff, bs);
	count = 0;
	
	for (ofs = start; ofs < end_addr; ofs += bs) 
	{
		/* badblock checking... */
		if (blockstart != (ofs & (~mtd.eb_size + 1)) || firstblock) {
			blockstart = ofs & (~mtd.eb_size + 1);
			firstblock = 0;
			
			badblock = mtd_is_bad(&mtd, fd, ofs / mtd.eb_size);
			if (badblock < 0) {
				dev_err("mtd_is_bad error!\n");
				close(fd);
				return -1;
			}
		}
		
		if (badblock) {
			/* skip bad block, increase end_addr */
			end_addr += mtd.eb_size;
			ofs += mtd.eb_size - bs;
			if (end_addr > mtd.size)
				end_addr = mtd.size;
			continue;
		} else {
			if (mtd_read(&mtd, fd, ofs / mtd.eb_size, ofs % mtd.eb_size, readbuf, bs)) {
				dev_err("mtd_read error!\n");
				close(fd);
				return -1;
			}
			
			/* data copy */
			memcpy(data+count, readbuf, bs);
			count += bs;
		}
		
		if (0) 
		{
			int i;
			
			for (i = 0; i < bs; i += PRETTY_ROW_SIZE) {
				pretty_dump_to_buffer(readbuf + i, PRETTY_ROW_SIZE,
						pretty_buf, PRETTY_BUF_LEN, true, true, ofs + i);
				write(STDOUT_FILENO, pretty_buf, strlen(pretty_buf));
			}
		}
	}
	
	close(fd);
	return 0;
}

static int mtd_nand_write_page(const char *data, int start, int length)
{
	struct mtd_dev_info mtd;
	
	int fd = -1;
	
	int ebsize_aligned;
	int pagelen;
	long blockstart = -1;
	long offs;
	long mtdoffset = 0;
	int ret;
	int	blockalign = 1;
	
	bool baderaseblock = false;
	unsigned char writebuf[MTD_PAGESIZE];
	
	if (length > MTD_PAGESIZE) {
		dev_err("Input size is big!!!\n");
		return -1;
	}
	
	/* Open the mtd7 device */
	fd = open(MTD_PATH, O_RDWR);
	if (fd == -1) {
		dev_err("failed to open %s\n", MTD_PATH);
		return -1;
	}
	
	/* set mtd structure */
	mtd.size        = MTD_SIZE;
	/* mtd.eb_size = /sys/class/mtd/mtd7/erasesize = 131072bytes(128KB), 1page */
	mtd.eb_size     = MTD_ERASESIZE;
	/* mtd.min_io_size = /sys/class/mtd/mtd7/writesize = 2048bytes, 1page */
	mtd.min_io_size = MTD_PAGESIZE;
	mtd.eb_cnt = mtd.size / mtd.eb_size;
	mtd.type   = MTD_NANDFLASH;
	mtd.bb_allowed = !!(mtd.type == MTD_NANDFLASH);
	
	ebsize_aligned = mtd.eb_size * blockalign;
	pagelen = mtd.min_io_size;
	mtdoffset = (long)start;
	
	//# erase write buffer
	memset(writebuf, 0xff, MTD_PAGESIZE);
	//# data copy
	memcpy(writebuf, data, length);
	
	/*
	 * New eraseblock, check for bad block(s)
	 */
	while (blockstart != (mtdoffset & (~ebsize_aligned + 1))) 
	{
		blockstart = mtdoffset & (~ebsize_aligned + 1);
		offs = blockstart;

		baderaseblock = false;

		do {
			ret = mtd_is_bad(&mtd, fd, offs / ebsize_aligned);
			if (ret < 0) {
				dev_err("MTD get bad block failed\n");
				close(fd);
				return -1;
			} else if (ret == 1) {
				baderaseblock = true;
			}

			if (baderaseblock) {
				mtdoffset = blockstart + ebsize_aligned;
				if (mtdoffset > mtd.size) {
					dev_err("too many bad blocks, cannot complete request\n");
					close(fd);
					return -1;
				}
			}

			offs +=  ebsize_aligned / blockalign;
		} while (offs < blockstart + ebsize_aligned);
	}
	
	/* Write out data */
	mtd_write(&mtd, fd, mtdoffset / mtd.eb_size, 
			mtdoffset % mtd.eb_size, writebuf, mtd.min_io_size);
	close(fd);
	
	return 0;
}

/******************************************************
 * NAME : int dev_board_serial_init(void)
 * MTD7 block 0 ~ Block 9
 *      Serial ID 
 ******************************************************/
int dev_board_serial_init(void)
{
	char tmpbuf[16];
	char strbuf[16];
	
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	memset(tmpbuf, 0, 16);
	memset(strbuf, 0, 16);
	
	/* Dump the 1 page contents 1st block, page 0 */
	mtd_nand_read(pagebuf, 0, MTD_PAGESIZE);
	/* 4byte copy and compare AA55 */
	memcpy(tmpbuf, pagebuf, 4);
	snprintf(strbuf, sizeof(strbuf), "%s", tmpbuf); 
	if (strncmp(strbuf, MTD_MAGIC, 4) != 0) 
	{
		dev_err("Not founded serial magic code! clear memory...\n");
		/* erase block /dev/mtd7 block 0 ~ 4 */
		system("/usr/sbin/flash_erase /dev/mtd7 0 5");
		/* wait erase done!! */
		sleep(1);
		
		/* write magic code aa55 */
		memset(pagebuf, 0xff, MTD_PAGESIZE);
		strcpy(pagebuf, MTD_MAGIC);
		strcpy(pagebuf+4, "empty");
		
		mtd_nand_write_page(pagebuf, 0, 16);	
	} 
		
	return 0;
}

int dev_board_serial_read(char *data, int length)
{
	char *tmpbuf;
	char strbuf[16];
	
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	memset(strbuf, 0, 16);
	
	/* Dump the 1 page contents 1st block, page 0 */
	mtd_nand_read(pagebuf, 0, MTD_PAGESIZE);
	
	tmpbuf = pagebuf + 4; //# except magic code AA55
	memcpy(data, tmpbuf, 16);
	
	return 0;
}

int dev_board_serial_write(const char *data, int length)
{
	char *tmpbuf;
	char pretty_buf[PRETTY_BUF_LEN];
	
	if (length > MTD_PAGESIZE) {
		dev_err("invalid length (< 2048)\n");
		return -1;
	}
		
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	
	/* data copy */
	strcpy(pagebuf, MTD_MAGIC);
	
	tmpbuf = pagebuf + 4;
	memcpy(tmpbuf, data, length);
	
	/* erase block /dev/mtd7 block 0 ~ 4 */
	system("/usr/sbin/flash_erase /dev/mtd7 0 5");
	sleep(1);
	
	if (0) 
	{
		int i;
		
		for (i = 0; i < MTD_PAGESIZE; i += PRETTY_ROW_SIZE) {
			pretty_dump_to_buffer((const unsigned char *)tmpbuf + i, PRETTY_ROW_SIZE,
					pretty_buf, PRETTY_BUF_LEN, true, true, i);
			write(STDOUT_FILENO, pretty_buf, strlen(pretty_buf));
		}
	}
		
	mtd_nand_write_page(pagebuf, 0, (length + 4));
		
	return 0;
}

/******************************************************
 * NAME : int dev_board_uid_init(void)
 * MTD7 block 0 ~ Block 9
 *      Serial ID 
 ******************************************************/
int dev_board_uid_init(void)
{
	char tmpbuf[16];
	char strbuf[16];
	
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	memset(tmpbuf, 0, 16);
	memset(strbuf, 0, 16);
	
	/* Dump the 1 page contents page 0xA0000 */
	mtd_nand_read(pagebuf, 0xA0000, MTD_PAGESIZE);
	/* 4byte copy and compare AA55 */
	memcpy(tmpbuf, pagebuf, 4);
	snprintf(strbuf, sizeof(strbuf), "%s", tmpbuf); 
	if (strncmp(strbuf, MTD_MAGIC, 4) != 0) 
	{
		dev_err("Not founded UID magic code! clear memory...\n");
		/* erase block /dev/mtd7 block 5 ~ 10, 1page=2048, 1block 64page */
		system("/usr/sbin/flash_erase /dev/mtd7 0xA0000 5");
		/* wait erase done!! */
		sleep(1);
		
		/* write magic code aa55 */
		memset(pagebuf, 0xff, MTD_PAGESIZE);
		strcpy(pagebuf, MTD_MAGIC);
		strcpy(pagebuf+4, "empty");
		
		mtd_nand_write_page(pagebuf, 0xA0000, 16);	
	} 
		
	return 0;
}

int dev_board_uid_read(char *data, int length)
{
	char *tmpbuf;
	char strbuf[16];
	
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	memset(strbuf, 0, 16);
	
	/* Dump the 1 page contents 1st block, page 0 */
	mtd_nand_read(pagebuf, 0xA0000, MTD_PAGESIZE);
	
	tmpbuf = pagebuf + 4; //# except magic code AA55
	memcpy(data, tmpbuf, 16);
	
	return 0;
}

int dev_board_uid_write(const char *data, int length)
{
	char *tmpbuf;
	char pretty_buf[PRETTY_BUF_LEN];
	
	if (length > MTD_PAGESIZE) {
		dev_err("invalid length (< 2048)\n");
		return -1;
	}
		
	memset(pagebuf, 0xff, MTD_PAGESIZE);
	
	/* data copy */
	strcpy(pagebuf, MTD_MAGIC);
	
	tmpbuf = pagebuf + 4;
	memcpy(tmpbuf, data, length);
	
	/* erase block /dev/mtd7 block 0 ~ 4 */
	system("/usr/sbin/flash_erase /dev/mtd7 0xA0000 5");
	sleep(1);
	
	if (0) 
	{
		int i;
		
		for (i = 0; i < MTD_PAGESIZE; i += PRETTY_ROW_SIZE) {
			pretty_dump_to_buffer((const unsigned char *)tmpbuf + i, PRETTY_ROW_SIZE,
					pretty_buf, PRETTY_BUF_LEN, true, true, i);
			write(STDOUT_FILENO, pretty_buf, strlen(pretty_buf));
		}
	}
		
	mtd_nand_write_page(pagebuf, 0xA0000, (length + 4));
		
	return 0;
}
