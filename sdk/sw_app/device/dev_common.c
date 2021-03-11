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

#define SRAM_SZ				64
#define SRAM_PATH			"/sys/devices/platform/omap/omap_i2c.1/i2c-1/1-006f/rtcram"
#define SRAM_MAGIC_CODE		"AA55"

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

/******************************************************
 * NAME : int dev_rtcmem_setdata(const char *data, int len)
 ******************************************************/
int dev_rtcmem_setdata(const char *data, int len)
{
	char buf[SRAM_SZ]={0,};
	int fd;
	
	if ((data == NULL) || (len > (SRAM_SZ-4)))
		return -1;
		
	fd = open(SRAM_PATH, O_RDWR);
	if (fd < 0) {
		dev_err("Failed to open %s\n", SRAM_PATH);
		return -1;
	}
	
	memcpy(buf, data, len);
	lseek(fd, 4, SEEK_SET);
	write(fd, buf, SRAM_SZ-4);
	lseek(fd, 0, SEEK_SET);
	close(fd);
	
	return 0;
}

/******************************************************
 * NAME : int dev_rtcmem_getdata(char *data)
 ******************************************************/
int dev_rtcmem_getdata(char *data, int len)
{
	char buf[SRAM_SZ]={0,};
	int fd;
	
	if ((data == NULL) || (len > (SRAM_SZ-4)))
		return -1;
		
	fd = open(SRAM_PATH, O_RDWR);
	if (fd < 0) {
		dev_err("Failed to open %s\n", SRAM_PATH);
		return -1;
	}
	
	/* magic code location : 4byte */
	lseek(fd, 4, SEEK_SET);
	read(fd, buf, SRAM_SZ-4);
	memcpy(data, buf, len);
	/* set first offset */
	lseek(fd, 0, SEEK_SET);
	close(fd);
	
	return 0;
}

/******************************************************
 * NAME : int dev_rtcmem_initdata(char *data)
 ******************************************************/
int dev_rtcmem_initdata(void)
{
	char buf[SRAM_SZ]={0,};
	int fd;
	
	fd = open(SRAM_PATH, O_RDWR);
	if (fd < 0) {
		dev_err("Failed to open %s\n", SRAM_PATH);
		return -1;
	}
	
	read(fd, buf, SRAM_SZ);
	lseek(fd, 0, SEEK_SET); /* for sysfs */
	
	if (strncmp(buf, SRAM_MAGIC_CODE, 4) != 0) {
		//dev_err("Not founded RTCMEM magic code(%s)! clear memory...\n", buf);
		
		/* clear data */
		memset(buf, 0, SRAM_SZ);
		/* write magic code aa55aa55 */
		strcpy(buf, SRAM_MAGIC_CODE);
		strcpy(buf+4, "empty");
		write(fd, buf, SRAM_SZ);
		lseek(fd, 0, SEEK_SET);
	} 
	close(fd);
	
	return 0;
}
