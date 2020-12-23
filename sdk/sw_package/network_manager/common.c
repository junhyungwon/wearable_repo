/*
 * File : common.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>	//# waitpid
#include <sys/socket.h>
#include <sys/vfs.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "netmgr_ipc_cmd_defs.h"
#include "main.h"
#include "event_hub.h"
#include "common.h"

/* Wi-Fi Module Path */
#define RTL_8188E_PATH			"/lib/modules/8188eu.ko"
#define RTL_8188C_PATH			"/lib/modules/8192cu.ko"
#define RTL_8821A_PATH			"/lib/modules/8821au.ko"
#define RTL_8812A_PATH			"/lib/modules/8812au.ko"

/* Wi-Fi Module name */
#define RTL_8188E_NAME			"8188eu"
#define RTL_8188C_NAME			"8192cu"
#define RTL_8821A_NAME			"8821au"
#define RTL_8812A_NAME			"8812au"

#define PROC_MODULE_FNAME		"/proc/modules"

#define ARCH_NR_GPIOs		(256)
#define SZ_BUF				64

#define GPIO_BASE_PATH 		"/sys/class/gpio"
#define GPIO_PATH_MAX		64

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

struct gpio_desc_t {
	int fd;         /* file descriptor for /gpio/valude */
	int dir;		/* direction of gpio */
};

typedef struct {
	int d_vid;
	int d_pid;
	
} usb_dev_id_t;

static usb_dev_id_t usb_wlan_list[NETMGR_USB_MAX_NUM] = {
	{ .d_vid = RTL_8188E_VID, .d_pid = RTL_8188E_PID, },
	{ .d_vid = RTL_8188C_VID, .d_pid = RTL_8188C_PID, },
	{ .d_vid = RTL_8192C_VID, .d_pid = RTL_8192C_PID, },
	{ .d_vid = RTL_8821A_VID, .d_pid = RTL_8821A_PID, },
	{ .d_vid = RTL_8812A_VID, .d_pid = RTL_8812A_PID, }
};

static struct gpio_desc_t desc[ARCH_NR_GPIOs];

//################################################################################################################################
/* numeric: & 0x8000: default instead of *,
 *          & 0x4000: host instead of net,
 *          & 0x0fff: don't resolve
 */
static char *__INET_rresolve(struct sockaddr_in *s_in, int numeric,
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
    	eprintf("rresolve: unsupported address family %d!", s_in->sin_family);
    	errno = EAFNOSUPPORT;
    	return NULL;
  	}

  	ad = s_in->sin_addr.s_addr;
//  	dprintf("rresolve: %08x, mask %08x, num %08x\n", (unsigned)ad, netmask, numeric);
  	if (ad == INADDR_ANY) {
    	if ((numeric & 0x0FFF) == 0) {
      		if (numeric & 0x8000) {
				return strdup("default");
			}
      		return strdup("*");
    	}
  	}

  	if (numeric & 0x0FFF) {
		return strdup(inet_ntoa(s_in->sin_addr));
	}
	return 0;
}

static void __set_flags(char *flagstr, int flags)
{
	int i;

	*flagstr++ = 'U';
	for (i = 0; (*flagstr = flagchars[i]) != 0; i++) {
		if (flags & flagvals[i]) {
			++flagstr;
		}
	}
}

static int __net_if_get_gate(const char *ifce, char *gate)
{
	char devname[64], flags[16];
	char *sdest, *sgw;

	unsigned long dst, gw, m;
	int flgs, ref, use, metric, mtu, win, ir;

	struct sockaddr_in s_addr;
	struct in_addr mask;
	FILE *fp;
	
	if (!gate) {
		eprintf("invalid argument(NULL)\n");
		return -1;
	}
	
	fp = fopen("/proc/net/route", "r");
	if (!fp) {
		return -1;
	}

	/* Skip the first line. */
	if (fscanf(fp, "%*[^\n]\n") < 0) {
		/* Empty or missing line, or read error. */
		fclose(fp);
      	return -1;
	}

	while (1) 
	{
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
      		return -1;
		}
		
		/* Skip interfaces that are down. */
		if (!(flgs & RTF_UP) || strcmp(devname, ifce)) {
			continue;
		}

		__set_flags(flags, (flgs & IPV4_MASK));

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
		sdest = __INET_rresolve(&s_addr, (0x0FFF | 0x8000), m);
		s_addr.sin_addr.s_addr = gw;
		if (sdest) {
			free(sdest);
			sdest = NULL;
		}

		/* Host instead of net */
		sgw = NULL;
		sgw = __INET_rresolve(&s_addr, (0x0FFF | 0x4000), m);
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

static int __set_net_if_gate(const char *ifce, const char *gate)
{
	char cmd[128];
	char tmp[16];
	int ret, metric;
	FILE *f = NULL;

	if (gate == NULL) {
		eprintf("invalid argument(NULL)\n");
		return -1;
	}

	memset(cmd, 0, sizeof(cmd));
	memset(tmp, 0, sizeof(tmp));

	ret = __net_if_get_gate(ifce, tmp);
	if (ret < 0)
		return -1;

	if (strcmp((char*)tmp, "0.0.0.0")) {
		snprintf((char*)cmd, sizeof(cmd), "/sbin/route del default gw %s dev %s", tmp, ifce);
		f = popen(cmd, "r");
		if (f != NULL)
			pclose(f);
	}
	
	metric = 0;
	if (strcmp("eth0", ifce)==0) {
		metric = 10;
		snprintf((char*)cmd, sizeof(cmd), "/sbin/route add default gw %s dev %s metric %d", gate, ifce, metric);
	} else {
		snprintf((char*)cmd, sizeof(cmd), "/sbin/route add default gw %s dev %s metric %d", gate, ifce, metric);
	}
	f = popen(cmd, "r");
	if (f != NULL)
		pclose(f);
		
	return 0;
}

static int __set_static_ip(const char *ifce, const char *ip_str, const char *mask_str, const char *gw_str)
{
	int ret = 0, fd;

	struct ifreq ifreq;
  	struct sockaddr_in sin;

  	if (!strcmp(ip_str, "0.0.0.0")) {
  		eprintf("invalid argument(NULL)\n");
  		return -1;
  	}

	memset(&ifreq, 0, sizeof(struct ifreq));
	/* I want to get an IPv4 IP address */
	ifreq.ifr_hwaddr.sa_family = AF_INET;
	
  	fd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (fd < 0) {
  		eprintf("Socket creation failed, this is a fatal error\n");
  		return -1;
  	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip_str);
	memcpy(&ifreq.ifr_addr, &sin, sizeof(struct sockaddr));
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCSIFADDR, &ifreq);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	sin.sin_addr.s_addr = inet_addr(mask_str);
	memcpy(&ifreq.ifr_addr, &sin, sizeof(struct sockaddr));
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(fd, SIOCSIFNETMASK, &ifreq);
	if (ret < 0) {
		eprintf("netmask: Invalid argument\n");
		close(fd);
		return -1;
	}

	ret = __set_net_if_gate(ifce, gw_str);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	close(fd);

	return ret;
}

static int __wlan_get_module(const char *name)
{
    FILE *proc;
    char line[256] = {};

	if (name == NULL)
		return 0;

    if ((proc = fopen(PROC_MODULE_FNAME, "r")) == NULL) {
        eprintf("Could not open /proc/modules!!\n");
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

static void __wlan_insmod(const char *module_path)
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

int netmgr_net_link_detect(const char *ifce)
{
	struct ifreq ifreq;
	int skfd, r;

	memset(&ifreq, 0, sizeof(struct ifreq));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
  	if (skfd < 0) {
  		eprintf("Socket creation failed, this is a fatal error!\n");
  		return 0;
  	}

  	/* I want to get an IPv4 IP address */
  	ifreq.ifr_addr.sa_family = AF_INET;
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	r = ioctl(skfd, SIOCGIFFLAGS, &ifreq);
	close(skfd);

	if ((r < 0) || !(ifreq.ifr_flags & IFF_UP))
		return 0;

	return 1;
}

int netmgr_net_link_up(const char *ifce)
{
	char cmd[128];
	FILE *f;
	
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/sbin/ifconfig %s up", ifce);
	f = popen(cmd, "r");
	if (f != NULL)
		pclose(f);

	return 0;
}

void netmgr_net_link_down(const char *ifce)
{
	char cmd[128];
	FILE *f;
	
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "/sbin/ifconfig %s 0.0.0.0 down", ifce);
	f = popen(cmd, "r");
	if (f != NULL)
		pclose(f);
}

int netmgr_get_net_info(const char *ifce, char *hw_buf, char *ip_buf, char *mask, char *gw)
{
	struct ifreq ifreq;
	int ret, skfd;
	
	memset(&ifreq, 0, sizeof(struct ifreq));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		eprintf("Socket creation failed, this is a fatal error\n");
		return -1;
	}

	/* I want to get an IPv4 IP address */
	ifreq.ifr_hwaddr.sa_family = AF_INET;
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	
	/* Get HWAddress --> MAC */
	if (hw_buf != NULL) {
		memset(hw_buf, 0, 32);
		ret = ioctl(skfd, SIOCGIFHWADDR, &ifreq);
		if (ret >= 0) {
			//# conversion ntoa
			snprintf(hw_buf, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
				ifreq.ifr_hwaddr.sa_data[0], ifreq.ifr_hwaddr.sa_data[1],
				ifreq.ifr_hwaddr.sa_data[2], ifreq.ifr_hwaddr.sa_data[3],
				ifreq.ifr_hwaddr.sa_data[4], ifreq.ifr_hwaddr.sa_data[5]);
			//fprintf(stderr, "get rndis mac = %s\n", hw_buf);
		}
	}
	
	memset(ip_buf, 0, 16); //# net string 16
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(skfd, SIOCGIFADDR, &ifreq);
	if (ret >= 0) {
		sprintf(ip_buf, "%s", inet_ntoa(((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr));
	} else
		strcpy(ip_buf, "0.0.0.0");
	
	memset(mask, 0, 16);
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(skfd, SIOCGIFNETMASK, &ifreq);
	if (ret >= 0) {
		sprintf(mask, "%s",	inet_ntoa(((struct sockaddr_in *)&ifreq.ifr_addr)->sin_addr));
	} else
		strcpy(mask, "255.255.255.0");
	
	close(skfd);

	return __net_if_get_gate(ifce, gw);
}

/*****************************************************************************
 * * @section  DESC Description
 * *   - desc
 * cradle ipaddress eth0, usbtoEthernet cable eth1, wifi client ->wlan0
 * *******************************************************************************/
int netmgr_set_ip_static(const char *ifname, const char *ip, const char *net_mask, const char *gateway) 
{
    int state;
	char tmp_hw[32+1];
	char tmp_ip[NETMGR_NET_STR_MAX_SZ+1];
    char tmp_mask[NETMGR_NET_STR_MAX_SZ+1];
    char tmp_gw[NETMGR_NET_STR_MAX_SZ+1];
	
	state = netmgr_net_link_detect(ifname);
	if (!state) {
		/* Link up */
		netmgr_net_link_up(ifname);
		sleep(1); /* wait 1sec */
	}
	
	netmgr_get_net_info(ifname, &tmp_hw[0], &tmp_ip[0], &tmp_mask[0], &tmp_gw[0]);
	if ((strcmp(tmp_ip, "0.0.0.0") == 0) || (strcmp(tmp_ip, ip) != 0)) 
	{
		strcpy(tmp_ip, ip);
	}

	if ((strcmp(tmp_mask, "0.0.0.0") == 0) || (strcmp(tmp_mask, net_mask) != 0)) 
	{
		strcpy(tmp_mask, net_mask);
	}

	if ((strcmp(tmp_gw, "0.0.0.0") == 0) || (strcmp(tmp_gw, gateway) != 0)) 
	{
		strcpy(tmp_gw, gateway);
	}

	__set_static_ip(ifname, tmp_ip, tmp_mask, tmp_gw);
	sleep(1);
	
    return 0;
}

//	#define UDHCPC_PID_PATH			"/var/run/udhcpc.wlan0.pid"
int netmgr_set_ip_dhcp(const char *ifname) 
{
    FILE *f = NULL;
    char command[128]={0,};
	char path[128]={0,};
	int state;
	
	state = netmgr_net_link_detect(ifname);
	if (!state) {
		/* Link up */
		netmgr_net_link_up(ifname);
		sleep(1); /* wait 1sec */
	}
	
	/* make pid path */
	snprintf(path, sizeof(path), "/var/run/udhcpc.%s.pid", ifname);
	/*
	 * -A: tryagain after 3sec(def 20sec). wait if lease is not obtained
	 * -T: timeout 1sec. (default 3)
	 * -t: retry. (default 3)
	 * -b: background.
	 * -p: create pid file
	 * -n; error exit. if lease cannot be immediately negotiated.
	 */
	memset(command, 0, sizeof(command));
	snprintf(command, sizeof(command), "/sbin/udhcpc -i %s -A 3 -T 1 -t 5 -n -b -p %s", ifname, path);
	f = popen(command, "w");
	if (f != NULL) {
		pclose(f);
	}
	
	return 0;
}

int netmgr_udhcpc_is_run(const char *ifname)
{
	struct stat st;
	char buf[256]={0,};
	char path[128]={0,};
	FILE *f;
	int r = 0, pid;
	
	/* get udhcpc */
	memset(&st, 0, sizeof(st));
	snprintf(path, sizeof(path), "/var/run/udhcpc.%s.pid", ifname);
	f = fopen(path, "r");
	if (f == NULL) {
		/*  terminated or process done!! */
		return 0;
	}

	fscanf(f, "%d", &pid); //# get pid.
	fclose(f);

	snprintf(buf, sizeof(buf), "/proc/%d/cmdline", pid);
	r = stat(buf, &st);
	if (r == -1 && errno == ENOENT) {
		/* process not exist */
		r = 0;
	} else
		/* process exist */
		r = 1;
	
	return r;
}

void netmgr_udhcpc_stop(const char *ifname)
{
	char path[128]={0,};
	int pid = 0;
	FILE *f;
	
	snprintf(path, sizeof(path), "/var/run/udhcpc.%s.pid", ifname);
	f = fopen(path, "r");
    if (f != NULL) {
	    fscanf(f, "%d", &pid);
	    fclose(f);
	    unlink((const char *)path);

	    kill(pid, SIGKILL);
	    waitpid(pid, NULL, 0);
	} else
		eprintf("couldn't stop udhcpc(%s)\n", path);
}

/*
 * 이 함수는 ifconfig ethX up을 해야 값을 읽을 수 있다.
 */
int netmgr_is_netdev_active(const char *ifname)
{
	FILE *fp = NULL;
    char buf[32] = {0, };
    int status=0;
    unsigned char val;

    snprintf(buf, sizeof(buf), "/sys/class/net/%s/carrier", ifname);
    
	fp = fopen(buf, "r") ;
    if (fp != NULL) {   
        fread(&val, 1, 1, fp);
        if (val == '1') {
            status = 1 ; // connect
        } else { 
            status = 0 ; // disconnect
        } 
        fclose(fp);
    }
	
    return status;
}

/*
 * set shared memory response
 */

int netmgr_set_shm_ip_info(int dev, const char *ip, const char *mask, const char *gw)
{
	netmgr_shm_response_info_t *info;
	char *databuf;
	
	//# Memory Offset을 더할 때 바이트 단위로 더하기 위해서 임시 포인터 사용.
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_RESPONSE_INFO_OFFSET);
	info = (netmgr_shm_response_info_t *)databuf;
	
	/* memory clear */
	memset(databuf, 0, NETMGR_SHM_RESPONSE_INFO_SZ);
	strcpy(info->ip_address, ip);
	strcpy(info->mask_address, mask);
	strcpy(info->gw_address, gw);
	
	info->device = dev;
	
	return 0;
}

//################################################################################################################################

/*
 * note: file descriptor 0(stdin), 1(stdout), 2(stderr)
 * so we assumed zero fd -> invalid fd
 */
/*************************************************************
 * NAME : int gpio_init(struct gpio_dev_t *gpio_dev, int value)
 ************************************************************/
int gpio_input_init(int num)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	char buf[32 + 1] = {};

	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		eprintf("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* export gpio */
	fd = open(GPIO_BASE_PATH "/export", O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s\n", path);
		return -1;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	/* direction input */
	gpio->dir = GPIO_INPUT;
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/direction", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s\n", path);
		return -1;
	}

	snprintf(buf, sizeof(buf), "in");
	write(fd, buf, strlen(buf));
	close(fd);

	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", num);
	fd = open(path, O_RDWR); /* For (rw) O_RDWR: (r)O_RDONLY, (w)O_WRONLY */
	if (fd < 0) {
		eprintf("couldn't open %s (ret = %d)\n", path, fd);
		return -1;
	}
	gpio->fd = fd;

	/* dummy read : flush gpio */
	read(fd, buf, sizeof(buf) - 1);
	lseek(fd, 0, SEEK_SET);

	return 0;
}

/*************************************************************
 * NAME : int gpio_output_init(int num, int default_val)
 ************************************************************/
int gpio_output_init(int num, int default_val)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	char buf[32 + 1] = {};

	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		eprintf("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* export gpio */
	fd = open(GPIO_BASE_PATH "/export", O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s\n", path);
		return -1;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	/* direction output (default value) */
	gpio->dir = GPIO_OUTPUT;
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH  "/gpio%d/direction", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s\n", path);
		return -1;
	}

	if (default_val)
		snprintf(buf, sizeof(buf), "high");
	else
		snprintf(buf, sizeof(buf), "out");

	write(fd, buf, strlen(buf));
	close(fd);

	/* open gpio value (default : O_WRONLY, output value) */
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", num);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s (ret = %d)\n", path, fd);
		return fd;
	}
	gpio->fd = fd;

	return 0;
}

/*************************************************************
 * NAME : int gpio_exit(int num)
 ************************************************************/
int gpio_exit(int num)
{
	struct gpio_desc_t *gpio;

	char path[GPIO_PATH_MAX + 1];
	int fd, len;

	if (num >= ARCH_NR_GPIOs) {
		eprintf("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	if (gpio->fd > 0) {
		close(gpio->fd);
		gpio->fd = -1;
	}

	/* unexport gpio */
	fd = open(GPIO_BASE_PATH "/unexport", O_WRONLY);
	if (fd < 0) {
		eprintf("couldn't open %s\n", path);
		return fd;
	}

	len = snprintf(path, sizeof(path), "%d", num);
	write(fd, path, len);
	close(fd);

	return 0;
}

/*************************************************************
 * NAME : int gpio_set_value(int num, int value)
 ************************************************************/
int gpio_set_value(int num, int value)
{
	struct gpio_desc_t *gpio;

	if (num >= ARCH_NR_GPIOs) {
		eprintf("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* sanity check */
	if (gpio->dir == GPIO_INPUT) {
		eprintf("gpio(%d) is not configure output mode!!\n", num);
		return -1;
	}

	if (gpio->fd > 0) {
		lseek(gpio->fd, 0, SEEK_SET);
		if (value)
			write(gpio->fd, "1", 2); //# add '\0'
		else
			write(gpio->fd, "0", 2); //# add '\0'
	} else {
		eprintf("gpio(%d) is required initialization!!\n", num);
		return -1;
	}

	return 0;
}

/*************************************************************
 * NAME : int gpio_get_value(int num, int *val)
 ************************************************************/
int gpio_get_value(int num, int *val)
{
	struct gpio_desc_t *gpio;

	char buf[SZ_BUF + 1] = {};
	int res;

	if (num >= ARCH_NR_GPIOs) {
		eprintf("Not supported gpio number(%d)\n", num);
		return -1;
	}
	gpio = &desc[num];

	/* sanity check */
	if (gpio->dir == GPIO_OUTPUT) {
		eprintf("gpio(%d) is not configure input mode!!\n", num);
		return -1;
	}

	if (gpio->fd > 0) {
		lseek(gpio->fd, 0, SEEK_SET);
		res = read(gpio->fd, buf, SZ_BUF);
		if (res == -1) {
			eprintf("Failed to read gpio[%d]\n", num);
			return -1; //# Failed to reading
		}
		/* return value: gpio + '\n' = 2byte */
		*val = strtol(buf, NULL, 10); //# atoi
	} else {
		*val = 0;
		eprintf("gpio(%d) is required initialization!!\n", num);
		return -1;
	}

	return 0;
}

/*****************************************************************************
* @brief Wi-Fi ??? ???? ??? ??? ?? ??? ???? ???
*****************************************************************************/
int netmgr_usb_is_exist(int usb_v, int usb_p)
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

	snprintf(cmd, sizeof(cmd), USB_LS_CMD_STR);
	lsusbfs = popen(cmd, "r");
	if (lsusbfs == NULL) {
		eprintf("couldn't access %s\n", cmd);
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

/*****************************************************************************
 * * @section  DESC Description
 * *   - desc
 * cradle ipaddress eth0, usbtoEthernet cable eth1, wifi client ->wlan0
 * *******************************************************************************/
int netmgr_wlan_is_exist(int *dst_vid, int *dst_pid)
{
	usb_dev_id_t *pusb = usb_wlan_list;
	int i, ret;

	*dst_vid = 0; *dst_pid = 0;
	for (i = 0; i < ARRAY_SIZE(usb_wlan_list); i++, pusb++) 
	{
		ret = netmgr_usb_is_exist(pusb->d_vid, pusb->d_pid);
		if (ret) {
			dprintf("founded device [%x, %x]\n", pusb->d_vid, pusb->d_pid);
			*dst_vid = pusb->d_vid;  *dst_pid = pusb->d_pid;
			return 0;
		}
	}

	return (-1); //# error
}

int netmgr_wlan_load_kermod(int vid, int pid)
{
	char path[128] = {0,};
	char name[128] = {0,};
	
	/* kernel module 파일명을 확인 */
	if ((vid == RTL_8188E_VID) && (pid == RTL_8188E_PID)) {
		strcpy(path, RTL_8188E_PATH);
		strcpy(name, RTL_8188E_NAME);
	} else if ((vid == RTL_8188C_VID) && (pid == RTL_8188C_PID)) {
		strcpy(path, RTL_8188C_PATH);
		strcpy(name, RTL_8188C_NAME);
	} else if ((vid == RTL_8192C_VID) && (pid == RTL_8192C_PID)) {
		strcpy(path, RTL_8188C_PATH);
		strcpy(name, RTL_8188C_NAME);
	} else if ((vid == RTL_8821A_VID) && (pid == RTL_8821A_PID)) {
		strcpy(path, RTL_8821A_PATH);
		strcpy(name, RTL_8821A_NAME);
	} else {
		/* invalid wifi usb module */
		eprintf("Not Supported WiFi USB Module!!(%x, %x)\n", vid, pid);
		return -1;
	}
				
	/* 이미 모듈이 loading된 상태이면 insmod 수행 안 함 */
	if (__wlan_get_module(name) == 0) {
		__wlan_insmod(path);
	} else {
		/* unload driver TODO */
		//snprintf(buf, sizeof(buf), "/sbin/rmmod %s", module_name);
	}
	
	return 0;
}

int netmgr_wlan_wait_mod_active(void)
{
	int ret;
	
	ret = access("/sys/class/net/wlan0/operstate", R_OK);
	if ((ret == 0) || (errno == EACCES)) {
		return 0;
	} 
	return -1;
}

//#----------------------------------------------------------------------------------------------
//# ------------------------- String Function ---------------------------------------------------
static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

int hex2byte(const char *hex)
{
	int a, b;
	a = hex2num(*hex++);
	if (a < 0)
		return -1;
	b = hex2num(*hex++);
	if (b < 0)
		return -1;
	return (a << 4) | b;
}

int hexstr2bin(const char *hex, unsigned char *buf, size_t len)
{
	size_t i;
	int a;
	const char *ipos = hex;
	unsigned char *opos = buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

/* Convert C from hexadecimal character to integer.  */
int hextobin(unsigned char c)
{
	switch (c)
	{
		default: return c - '0';
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
	}
}

/* referenced from echo.c */
int utf8_unescape(const char *dst, char *src)
{
	char *data_buf = dst;
	char const *s = src;
	unsigned char c;
	int xfred = 0;
	
	if ((data_buf == NULL) || (s == NULL)) {
		return -1;
	}
	
	while ((c = *s++))
	{
		if (c == '\\' && *s)
		{
			switch (c = *s++) 
			{
				case 'x':
				{
					unsigned char ch = *s;
					
					if (!isxdigit(ch)) {
						/* not an escape */
						data_buf[xfred] = '\\';
						xfred++;
					}
					s++;
					c = hextobin(ch);
					ch = *s;
					if (isxdigit(ch)) {
						s++;
						c = c * 16 + hextobin(ch);
					} 
					
				}
				break;
				case '0':
					c = 0;
					if (!('0' <= *s && *s <= '7'))
						break;
					c = *s++;
				/* Fall through.  */
				case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					c -= '0';
					if ('0' <= *s && *s <= '7')
						c = c * 8 + (*s++ - '0');
					if ('0' <= *s && *s <= '7')
						c = c * 8 + (*s++ - '0');
				break;
				case '\\': 
				break;
				default:  
					//putchar('\\');
					data_buf[xfred] = '\\';
					xfred++; 
				break;
			}
		}
		//putchar (c);
		data_buf[xfred] = c;
		xfred++;	
	}
}
//#----------------------------------------------------------------------------------------------------------