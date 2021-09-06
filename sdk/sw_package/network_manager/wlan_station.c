/*
 * File : wlan_station.c
 *
 * Copyright (C) 2020 LF
 *
 */
/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "netmgr_ipc_cmd_defs.h"
#include "wlan_station.h"
#include "main.h"
#include "common.h"
#include "event_hub.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MAX_ESSID_LEN			32
#define CMD_IWLIST				"/sbin/iwlist wlan0 scanning"
#define CMD_IWLIST_BUFSZ		512
#define MAX_AP_SCAN_LIST		64

#define TIME_CLI_CYCLE			200		//# msec
#define TIME_CONN_STAT_SEND    	5000   //# 5sec
#define CNT_CONN_STAT_SEND      (TIME_CONN_STAT_SEND/TIME_CLI_CYCLE)

#define TIME_CLI_ACTIVE    		10000   //# 10sec
#define CNT_CLI_ACTIVE        	(TIME_CLI_ACTIVE/TIME_CLI_CYCLE)

#define TIME_CLI_AUTH    		20000   //# 20sec
#define CNT_CLI_AUTH        	(TIME_CLI_AUTH/TIME_CLI_CYCLE)

#define TIME_CLI_DHCP    		10000   //# 10sec
#define CNT_CLI_DHCP        	(TIME_CLI_DHCP/TIME_CLI_CYCLE)

#define TIME_CLI_GET_ESSID 		5000   //# 5sec
#define CNT_CLI_GET_ESSID       (TIME_CLI_GET_ESSID/TIME_CLI_CYCLE)

#define IWGETID_CMD				"/sbin/iwgetid wlan0" //#"/sbin/iwgetid wlan0 -r"
#define IWCONFIG_CMD			"/sbin/iwconfig wlan0"

#define SUPPLICANT_CONFIG		"/etc/wpa_supplicant.conf"
#define SUPPLICANT_PID_PATH     "/var/run/wpa_supplicant.pid"

#define CONNECTED_AP_LIST		"/media/nand/aplist.lst"

#define __STAGE_CLI_MOD_WAIT		(0x1)
#define __STAGE_CLI_CHECK_ESSID		(0x2)
#define __STAGE_CLI_AUTH_START		(0x3)
#define __STAGE_CLI_WAIT_AUTH		(0x4)
#define __STAGE_CLI_SET_IP			(0x5)
#define __STAGE_CLI_WAIT_DHCP		(0x6)
#define __STAGE_CLI_DHCP_VERIFY		(0x7)
#define __STAGE_CLI_DHCP_NOTY		(0x8)
#define __STAGE_CLI_CONNECT_STATS	(0x9)
#define __STAGE_CLI_ERROR_STOP		(0xA)

typedef struct {
	char ssid[NETMGR_WLAN_SSID_MAX_SZ+1];      //32
	char passwd[NETMGR_WLAN_PASSWD_MAX_SZ+1];  //64
	
	int en_key;
	
} __attribute__((packed)) iw_item_t;

typedef struct {
	app_thr_obj cObj; /* wlan client mode */
	
	char ip[NETMGR_NET_STR_MAX_SZ+1];
    char mask[NETMGR_NET_STR_MAX_SZ+1];
    char gw[NETMGR_NET_STR_MAX_SZ+1];
	
	int dhcp;
	int stage;
	int cli_timer;
	
	iw_item_t item;
	
} netmgr_wlan_cli_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static const char *cli_dev_name = "wlan0";

static netmgr_wlan_cli_t t_cli;
static netmgr_wlan_cli_t *i_cli = &t_cli;

//# 사용자 요청 AP 목록
static iw_item_t userList[NETMGR_WLAN_CONN_MAX_CNT];

//# 검색된 AP 목록
static iw_item_t scanList[MAX_AP_SCAN_LIST];

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int __cli_get_phrase(const char *ssid, const char *password, char *output)
{
	FILE *f = NULL;
	size_t sz;
	
	char cmd[256] = {0,};
	char lbuf[256] = {0,};
	int flag = 0;
	int hexmode = 0;

	if (output == NULL)
		return -1;
	
	sz = strlen(password);
	if (sz >= 64)	
		hexmode = 1;
	
	if (!hexmode) {
		snprintf(cmd, sizeof(cmd), "/usr/sbin/wpa_passphrase \'%s\' \'%s\'", ssid, password);
		f = popen(cmd, "r");
		if (f != NULL) {
			char *start;
			char *ptr, *s_ptr;
			/* output format is
			* network={
			*     ssid="olleh"
			*     #psk="info00788"
			*     psk=3cb4f8415cb139e587529bcbed8a996bf4669ade3646293d4b21dea463b94188
			* }
			*/
			memset(lbuf, 0, sizeof(lbuf));
			while (fgets(lbuf, sizeof(lbuf), f) != NULL) {
				if ((start = strstr(lbuf, "#psk=")) != NULL) {
					flag = 1;
					continue; /* goto next line */
				}
				if (!flag)
					continue; /* not valid */

				/* find strings */
				if ((start = strstr(lbuf, "psk=")) != NULL) {
					ptr = strtok_r(start, "psk=", &s_ptr);
					if (ptr != NULL) {
						strcpy(output, ptr);
						pclose(f);
						return 0;
					}
				}
			}
		}
		if (f != NULL) {
			pclose(f);
		}
		return -1;
	} 
	/* if (!hexmode) */
	else 
		memcpy(output, password, sz);

	return 0;
}

/*
 * # Plaintext connection (no WPA, no IEEE 802.1X)
 * network={
 *	ssid="AndroidHotspot0877"
 *	key_mgmt=NONE
 * }
 *
 * # Shared WEP key connection (no WPA, no IEEE 802.1X)
 * network={
 *	ssid="static-wep-test"
 *	key_mgmt=NONE
 *	wep_key0="abcde"
 *	wep_key1=0102030405
 *	wep_key2="1234567890123"
 *	wep_tx_keyidx=0
 *	priority=5
 * }
 *
 * # Only WPA-PSK is used. Any valid cipher combination is accepted.
 * #network={
 *	ssid="UD-IP1"
 *	proto=WPA RSN
 *	key_mgmt=WPA-PSK
 *	pairwise=CCMP TKIP
 *	group=CCMP TKIP
 *	psk="4f4f2e563a"
 *	priority=2
 * }
 */
static int __create_supplicant_conf(const char *ssid, const char *password, int wep)
{
	int ret = 0;
	char phrase[256];
	
	FILE *config;

	if (ssid == NULL)
		return -1;

	memset(phrase, 0, sizeof(phrase));
	if (wep) {
		if (password == NULL) {
			eprintf("invalid password!!\n");
			return -1;
		}
    	/* get passphrase */
		ret = __cli_get_phrase(ssid, password, phrase);
		if (ret) {
			eprintf("Failed to get passphrase!!\n");
			return -1;
		}
    }

	/* Before starting the daemon, make sure its config file exists */
	ret = access(SUPPLICANT_CONFIG, R_OK|W_OK);
    if ((ret == 0) || (errno == EACCES)) {
      	unlink(SUPPLICANT_CONFIG);
    }

	config = fopen(SUPPLICANT_CONFIG, "wb");
	if (config == NULL) {
		eprintf("couldn't create %s file!!\n", SUPPLICANT_CONFIG);
		return -1;
	}

	/* write wpa_supplicant config */
	fprintf(config, "##### Example wpa_supplicant configuration file #########################\n");
	fprintf(config, "ctrl_interface=/var/run/wpa_supplicant\n");
	fprintf(config, "#ap_scan=1\n");
	fprintf(config, "network={\n");
	/* update ssid */
	fprintf(config, "\tssid=\"%s\"\n", ssid);

	if (wep) {
        fprintf(config, "\tscan_ssid=1\n");
		fprintf(config, "\tproto=WPA RSN\n");
		/*
		 * # key_mgmt: list of accepted authenticated key management protocols
         *   WPA-PSK = WPA pre-shared key (this requires 'psk' field)
		 *   NONE = WPA is not used
		 *   OPEN = Open System authentication (required for WPA/WPA2)
		 *   SHARED = Shared Key authentication (requires static WEP keys)
		 *   FT-PSK ==>802.11r fast roaming
		 */
		fprintf(config, "\tkey_mgmt=WPA-PSK\n");
		/*
		 * # pairwise: list of accepted pairwise (unicast) ciphers for WPA
         *   CCMP = AES in Counter mode with CBC-MAC [RFC 3610, IEEE 802.11i/D7.0]
         *   TKIP = Temporal Key Integrity Protocol [IEEE 802.11i/D7.0]
         *   NONE = Use only Group Keys (deprecated, should not be included if APs support
         *          pairwise keys)
         *  # If not set, this defaults to: CCMP TKIP
		 */
		fprintf(config, "\tpairwise=CCMP TKIP\n");
		/* # group: list of accepted group (broadcast/multicast) ciphers for WPA
         *    CCMP = AES in Counter mode with CBC-MAC [RFC 3610, IEEE 802.11i/D7.0]
         *    TKIP = Temporal Key Integrity Protocol [IEEE 802.11i/D7.0]
         *    WEP104 = WEP (Wired Equivalent Privacy) with 104-bit key
         *    WEP40 = WEP (Wired Equivalent Privacy) with 40-bit key [IEEE 802.11]
         *  If not set, this defaults to: CCMP TKIP WEP104 WEP40
         */
		fprintf(config, "\tgroup=CCMP TKIP WEP104 WEP40\n");
		/* # psk: WPA preshared key; 256-bit pre-shared key
         *     The key used in WPA-PSK mode can be entered either as 64 hex-digits, i.e.,
         *     32 bytes or as an ASCII passphrase (in which case, the real PSK will be
         *     generated using the passphrase and SSID). ASCII passphrase must be between
         *     8 and 63 characters (inclusive).
         * # Note: Separate tool, wpa_passphrase, can be used to generate 256-bit keys
         *      from ASCII passphrase. This process uses lot of CPU and wpa_supplicant
         *      startup and reconfiguration time can be optimized by generating the PSK only
         *      only when the passphrase or SSID has actually changed.
         */
         fprintf(config, "\tpsk=%s\n", phrase);
         fprintf(config, "\tpriority=2\n");

		 fprintf(config, "\tbgscan=\"simple:5:-60:6300\"\n") ;
	} else {
		/*
		 * # Plaintext connection (no WPA, no IEEE 802.1X)
         * network={
	     *       ssid="AndroidHotspot0877"
	     *        key_mgmt=NONE
         *   }
         */
         fprintf(config, "\tkey_mgmt=NONE\n");
	}

	fprintf(config, "}\n");
	fflush(config);
	fclose(config);
	sync(); //# /etc/wpa_supplicant.conf
		
	return 0;
}

static int __cli_start(const char *ssid, const char *password, int encrypt)
{
	char buf[256] = {0,};
	FILE *f;
	int r = 0;

	/* Before starting the daemon, make sure its config file exists */
	r = __create_supplicant_conf((char *)ssid, (char *)password, encrypt);
	if (r < 0) {
		/* Failed to wi-fi start */
		return r;
	}
	
	/* chmod is needed because open() didn't set permisions properly */
	chmod(SUPPLICANT_CONFIG, 0660);
	snprintf(buf, sizeof(buf), "/usr/sbin/wpa_supplicant -Dwext "
					"-iwlan0 -c %s -P %s -B", SUPPLICANT_CONFIG, SUPPLICANT_PID_PATH);
	f = popen(buf, "w");
	if (f == NULL)
		return -1;

	pclose(f);

//	THR_waitmsecs(500);
//	sleep(1); //# wait for connection.

	return 0;
}

static void __cli_stop(const char *ifce)
{
	FILE *f = NULL;
	char buf[256]={0,};
	int pid = 0;

	/* kill wpa_supplicant daemon */
    f = fopen(SUPPLICANT_PID_PATH, "r");
    if (f != NULL) {
	    fscanf(f, "%d", &pid);
	    fclose(f);
	    unlink((const char *)SUPPLICANT_PID_PATH);

	    kill(pid, SIGKILL);
	    waitpid(pid, NULL, 0);
	} 
	else {
		/* wpa_supplicant가 비정상적으로 실행되면 pid 파일이 생성 안됨 
		 * 직접 kill 해야 함.
		 */
		pid_t num_pid;
		
		f = popen("/bin/pidof wpa_supplicant", "r");
		if (f != NULL) {
			/* */
			fgets(buf, sizeof(buf), f);
			num_pid = strtoul(buf, NULL, 10);
			pclose(f);
			if (num_pid > 0) {
				//dprintf("wpa_supplicant pid is %d\n", num_pid);
				kill(num_pid, SIGKILL);
    			waitpid(num_pid, NULL, 0);
			} else
				eprintf("couldn't stop wpa_supplicant\n"); 
		}
	}
	
	/* stop udhcpc */
	if (netmgr_udhcpc_is_run(ifce))
		netmgr_udhcpc_stop(ifce);
#if 0
	/* kill udhcpc daemon */
//	kill -9 $(cat $WPA_PID)
//	ifconfig $WPA_IFACE 0.0.0.0 down
	netmgr_net_link_down("wlan0");
#endif
}

static int __cli_get_auth_status(const char *essid)
{
	FILE *f = NULL;
	char lbuf[128 + 1];
	char id[MAX_ESSID_LEN + 1];

	int r = 0, i;

	/*
	 * iwgetid
	 * wlan0 ESSID:"UD-DMZ"
	 */
	f = popen(IWGETID_CMD, "r");
	if (f == NULL) {
		eprintf("Not supported %s\n", IWGETID_CMD);
		return r;
	}

	memset(lbuf, 0, sizeof(lbuf));
	while (fgets(lbuf, sizeof(lbuf), f) != NULL)
	{
		char data_buf[128]={0,};
		char *s;

		/* find ESSID: */
		if ((s = strstr(lbuf, "ESSID:\"")) != NULL)
		{
			s += 7; /* --> ESSID:" */
			if (utf8_unescape(data_buf, s) < 0) {
				id[0] = '\0';
			} 
			else {
				for (i = 0; i < MAX_ESSID_LEN; i++) {
					/* To fine next token: '"' */
					if (data_buf[i] == '\0' || data_buf[i] == '"') {
						break;
					}
					id[i] = data_buf[i];
				}
				id[i] = '\0';
				
				//dprintf("iwgetid retured essid-> %s, required-> %s\n", id, essid);
				if (strcmp(id, essid) == 0) {
					/* connection ok */
					r = 1;
					break;
				}
			}
		}
	}

	pclose(f);
	return r;
}

/*
 * 연결된 AP의 신호 레벨을 확인할 때 필요함.
 */
static int __cli_get_link_status(const char *essid, int *level)
{
	FILE *f = NULL;

	char lbuf[256 + 1];
	char tbuf[4];
	char id[MAX_ESSID_LEN + 1];

	int valid = 0;
	
	/*
	 * wlan0     IEEE 802.11bg  ESSID:"UD-DMZ"  Nickname:"<WIFI@REALTEK>"
     *     Mode:Managed  Frequency:2.437 GHz  Access Point: 08:10:75:08:B9:74
     *     Bit Rate:54 Mb/s   Sensitivity:0/0
     *     Retry:off   RTS thr:off   Fragment thr:off
     *     Encryption key:****-****-****-****-****-****-****-****   Security mode:open
     *     Power Management:off
     *     Link Quality=96/100  Signal level=58/100  Noise level=0/100
     *     Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
     *     Tx excessive retries:0  Invalid misc:0   Missed beacon:0
     */
	f = popen(IWCONFIG_CMD, "r");
	if (f == NULL) {
		eprintf("Not supported %s\n", IWCONFIG_CMD);
		return 0;
	}

	while (fgets(lbuf, sizeof(lbuf), f) != NULL)
	{
		char data_buf[128]={0,};
		char *s;
		int i, val;

		/* find ESSID: */
		if ((s = strstr(lbuf, "ESSID:\"")) != NULL) {
			s += 7; /* --> ESSID:" */
			if (utf8_unescape(data_buf, s) < 0) {
				id[0] = '\0';	
			} else {
				for (i = 0; i < MAX_ESSID_LEN; i++) {
					/* To fine next token: '"' */
					if (data_buf[i] == '\0' || data_buf[i] == '"') {
						break;
					}
					id[i] = data_buf[i];
				}
				id[i] = '\0';
			}
			//dprintf("founded essid %s\n", data_buf);
			if (strcmp(id, essid) == 0) {
				/* connection ok */
				valid = 1;
				continue;
			}
		}

		if (!valid)
			continue; /* goto next line */

		s = strstr(lbuf, "Signal level=");
		if (s != NULL)
		{
			s += 13; /* Signal level=58/100 */
			for (i = 0; i < 3; i++) {
				if (s[i] == '/')
					tbuf[i] = '\0';
				else
					tbuf[i] = s[i];
			}

			tbuf[3] = '\0';
			val = atoi(tbuf); /* ex) 56/100 */
			if (val > 100)
				/* invalid */
				val = -1;

			pclose(f);
			*level = val;
			//dprintf("%s rssi level = %d\n", essid, *level);
			return 0;
		}
	}
	
	if (!valid) {
		/* invalid level is -1 */
		*level = -1;
		dprintf("ESSID-->%s connection closed!\n", essid);
	}
	pclose(f);
	return -1;
}

/*
 * 검색된 AP 리스트와 사용자 요구 SSID 리스트를 비교해서 일치하는 항복이 
 * 존재하는 지 확인하는 과정. 연결된 항목은 dst에 전달.
 */
static char listbuf[CMD_IWLIST_BUFSZ];
static int __cli_check_essid(iw_item_t *dst)
{
	FILE *f;
	
	/* max 32bytes */
	char ssid[MAX_ESSID_LEN+1]={0,};
	char mac[18];
		
	int wep, inCell, valid;
	int master, cnt, level;
	int i, j;
	
	/* wlan0     Scan completed :
     *           Cell 01 - Address: 00:25:A6:B5:6F:D3
     *              ESSID:"ollehWiFi"
     *              Protocol:IEEE 802.11gn
     *              Mode:Master
     *              Frequency:2.412 GHz (Channel 1)
     *              Encryption key:on
     *              Bit Rates:144 Mb/s
     *              Extra:wpa_ie=dd1a0050f20101000050f20202000050f2040050f20201000050f201
     *              IE: WPA Version 1
     *                  Group Cipher : TKIP
     *                  Pairwise Ciphers (2) : CCMP TKIP
     *                  Authentication Suites (1) : 802.1x
     *              Extra:rsn_ie=30180100000fac020200000fac04000fac020100000fac010000
     *              IE: IEEE 802.11i/WPA2 Version 1
     *                  Group Cipher : TKIP
     *                  Pairwise Ciphers (2) : CCMP TKIP
     *                  Authentication Suites (1) : 802.1x
     *              Quality=0/100  Signal level=56/100
     *               Extra:fm=0003
	 */
	/* initialize parser helper */
	mac[0] = '\0'; ssid[0] = '\0';

	cnt = 0; 	inCell = 0; valid = 0;
	level = -1; wep = -1; master = -1;

	memset(listbuf, 0, sizeof(listbuf));
	
	f = popen(CMD_IWLIST, "r");
	if (f != NULL)
	{
		while (fgets(listbuf, sizeof(listbuf), f) != NULL)
		{
			char *start;
			int length;
			
			length = strlen(listbuf);
			/* find wlan0 */
			if ((start = strstr(listbuf, cli_dev_name)) != NULL) {
				valid = 1;
				continue; /* goto next line for parser */
			}

			if (!valid) {
				/* if don't display --> wlan0 Scan completed */
				continue; /* goto next line */
			}

			/* find Cell strings */
			if ((start = strstr(listbuf, "Cell ")) != NULL) {
				//# find Address: xx:xx:xx:xx:xx:xx
			    int rem;
			    rem  = (listbuf + length) - (start + strlen("Cell XX - Address: xx:xx:xx:xx:xx:xx"));
			    if (rem < 0) {
					//# invalid input
					eprintf("can't find Cell XX line.!!\n");
					continue; /* goto next line */
				}

				strncpy(mac, start + strlen("Cell XX - Address: "), 17);
				mac[17] = '\0';
				inCell = 1;
			} 
			else if (inCell && (start = strstr(listbuf, "ESSID:\""))) 
			{
				char data_buf[128]={0,};
				
				start += 7; /* --> ESSID:" copy to max 32 */
				if (utf8_unescape(data_buf, start) < 0) {
					ssid[0] = '\0';	
				} 
				else {
					//dprintf("converted essid %s\n", data_buf);
					for (i = 0; i < MAX_ESSID_LEN; i++) 
					{
						if ((data_buf[i] == '\0') || (data_buf[i] == '"')) {
							/* null character */
							break;
						}
						/* fill ssid */
						ssid[i] = data_buf[i];
					}
					ssid[i] = '\0';
				}
			} else if (inCell && (start = strstr(listbuf, "Mode:"))) {
				if (strstr(start, "Master")) {
					master = 1;
				} else {
					master = 0; /* ad-hoc or station */
				}
			} else if (inCell && (start = strstr(listbuf, "Signal level="))) {
				char numstr[4];
				//# want to see if there are at least two more places
				if ((listbuf + length) - (start + 3) < 1) {
					eprintf("iwlist gave bad signal level line\n");
					continue; /* goto next line */
				}

				start += 13; /* Signal level= */
				for (i = 0; i < 3; i++) {
					if (start[i] == '/')
						numstr[i] = '\0';
					else
						numstr[i] = start[i];
				}

				numstr[3] = '\0';
				level = atoi(numstr); /* ex) 56/100 */
				if (level > 100)
					/* invalid */
					level = -1;

			} else if (inCell && (start = strstr(listbuf, "Encryption key:"))) {
				start += 15; /* Encryption key: */
				if (strstr(start, "on")) {
					wep = 1;
				} else {
					wep = 0;
				}
			}

			if (ssid[0] != '\0' && level != -1 && wep != -1 && master != -1 && mac[0] != '\0')
			{
				/* save current sincell information */
				scanList[cnt].en_key = wep;
				strcpy(scanList[cnt].ssid, ssid);
				cnt++;

				/* initialize parser helper */
				memset(listbuf, 0, sizeof(listbuf));
				mac[0] = '\0'; ssid[0] = '\0';

				inCell = 0; level = -1; wep = -1; master = -1;
				
				//# next loop..
				delay_msecs(5);
			} /* if (ssid[0] != '\0' && level != -1 && wep != -1 && master != -1 && mac[0] != '\0') */
		} /* while (fgets(listBuf, sizeof(listBuf), f) != NULL) */
		
		/* 사용자 요청 List와 비교해서 연결 */
		for (i = 0; i < NETMGR_WLAN_CONN_MAX_CNT; i++) {
			/* 현재 검색된 AP 목록수 */
			for (j = 0; j < cnt; j++) 
			{
				//dprintf("ssid compare %s %s\n", userList[i].ssid, scanList[j].ssid);
				if (strcmp(userList[i].ssid, scanList[j].ssid) == 0) {
					/* founded valid essid return 0 */
					memcpy(dst->ssid, userList[i].ssid, NETMGR_WLAN_SSID_MAX_SZ);
					memcpy(dst->passwd, userList[i].passwd, NETMGR_WLAN_PASSWD_MAX_SZ);
					dst->en_key = userList[i].en_key;
					dprintf("wifi connect to AP (ssid %s, passwd %s, encypt (%d)\n", dst->ssid, 
							dst->passwd, dst->en_key);
					
					pclose(f);
					return 0;
				}
			}
			delay_msecs(5);
		}
		//dprintf("not matching ap list!!\n");		
	} else {
		dprintf("could not open %s\n", CMD_IWLIST);
	}
	
	if (f != NULL)
		pclose(f);

	return -1;
}

/*****************************************************************************
* @brief    network proc function!
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_wlan_cli_main(void *prm)
{
	app_thr_obj *tObj = &i_cli->cObj;
	
	int exit = 0;
	int quit = 0;
	
	tObj->active = 1;
	dprintf("enter...!\n");
	
	while (!exit)
	{
		int cmd = 0;
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			exit = 1;
			continue;
		}
		
		/* for loop */ 
		quit = 0;
		while (!quit)
		{
			int st, res;
			
			cmd = tObj->cmd;
			if (cmd == APP_CMD_STOP) {
				//dprintf("wlan station stopping!!!!\n");
				__cli_stop(cli_dev_name);
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_INACTIVE);
				quit = 1;
				continue;
			}
			
			st = i_cli->stage;
			switch (st) {
			case __STAGE_CLI_CONNECT_STATS:
				if (i_cli->cli_timer >= CNT_CONN_STAT_SEND) 
				{
					int level=0;
					
					i_cli->cli_timer = 0;
					__cli_get_link_status(i_cli->item.ssid, &level);
					if (level < 0) {
						/* AP와 연결이 끊어진 상태를 나타냄 (AP가 다시 활성화 될 때까지 waiting... */
						__cli_stop(cli_dev_name);
						/* 
						 * 종료 후 SSID가 갱신될 때까지 약간의 시간이 필요함. 바로 검색할 경우 
						 * SSID가 갱신되지 않아서 검색이 된다. 이러한 상황에서는 DHCP 동작을 안 함.
						 */
						sleep(2);
						/* For LED RED */
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ERROR);
						i_cli->stage = __STAGE_CLI_CHECK_ESSID;
					} else {
						netmgr_event_hub_rssi_status(NETMGR_DEV_TYPE_WIFI, level);
						i_cli->stage = __STAGE_CLI_CONNECT_STATS;
					}
				} else {
					i_cli->cli_timer++;
				}
				break;
			
			case __STAGE_CLI_DHCP_NOTY:
				netmgr_set_shm_ip_info(NETMGR_DEV_TYPE_WIFI, i_cli->ip, i_cli->mask, i_cli->gw);
				netmgr_event_hub_dhcp_noty(NETMGR_DEV_TYPE_WIFI);
				i_cli->stage = __STAGE_CLI_CONNECT_STATS;
				break;
			
			case __STAGE_CLI_DHCP_VERIFY:
				res = netmgr_get_net_info(cli_dev_name, NULL, i_cli->ip, i_cli->mask, i_cli->gw);
				if (res < 0) {
					sysprint("udhcpc error\n");
					i_cli->stage = __STAGE_CLI_ERROR_STOP;
				} else {
					if (!strcmp(i_cli->ip, "0.0.0.0")) {
						/* dhcp로부터 IP 할당이 안된 경우 */
						sysprint("couln't get ip from %s!\n", i_cli->item.ssid);
						i_cli->stage = __STAGE_CLI_ERROR_STOP;
					} else {
						sysprint("ip address of %s is %s\n", i_cli->item.ssid, i_cli->ip);
						netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ACTIVE);
						i_cli->stage = __STAGE_CLI_DHCP_NOTY;
					}
				}
				break;
					
			case __STAGE_CLI_WAIT_DHCP:
				//# check done pipe(udhcpc...)
				if (netmgr_udhcpc_is_run(cli_dev_name)) {
					i_cli->cli_timer = 0;
					i_cli->stage = __STAGE_CLI_DHCP_VERIFY;
				} else {
					i_cli->stage = __STAGE_CLI_WAIT_DHCP;
					i_cli->cli_timer++;
					if (i_cli->cli_timer >= CNT_CLI_DHCP) {
						i_cli->stage = __STAGE_CLI_ERROR_STOP;
					} 
				}
				break;
					
			case __STAGE_CLI_SET_IP:
				if (i_cli->dhcp == 0) { //# set static ip
					netmgr_set_ip_static(cli_dev_name, i_cli->ip, i_cli->mask, i_cli->gw);
					netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ACTIVE);
					i_cli->stage = __STAGE_CLI_CONNECT_STATS;
				} else {
					netmgr_set_ip_dhcp(cli_dev_name);
					i_cli->stage = __STAGE_CLI_WAIT_DHCP;
				}
				break;
					
			case __STAGE_CLI_WAIT_AUTH:
				if (__cli_get_auth_status(i_cli->item.ssid)) {
					/* IP 설정 Stage */
					i_cli->stage = __STAGE_CLI_SET_IP;
					i_cli->cli_timer = 0;
				} else {
					i_cli->stage = __STAGE_CLI_WAIT_AUTH;
					i_cli->cli_timer++;
					if (i_cli->cli_timer >= CNT_CLI_AUTH) {
						i_cli->stage = __STAGE_CLI_ERROR_STOP;
					}
				}
				break;
				
			case __STAGE_CLI_AUTH_START:
				/* create wpa_supplicant.conf and execute wpa_supplicant */
				__cli_start(i_cli->item.ssid, i_cli->item.passwd, i_cli->item.en_key);
				i_cli->stage = __STAGE_CLI_WAIT_AUTH;
				i_cli->cli_timer = 0;
				break;
			
			case __STAGE_CLI_CHECK_ESSID:
				/* 4초정도 소요됨 */
				res = __cli_check_essid(&i_cli->item);
				if (res >= 0)
					i_cli->stage = __STAGE_CLI_AUTH_START;
				else
					i_cli->stage = __STAGE_CLI_CHECK_ESSID;
				break;
				
			case __STAGE_CLI_MOD_WAIT:
				res = netmgr_net_link_detect(cli_dev_name);
				if (!res) {
					/* Link up */
					netmgr_net_link_up(cli_dev_name);
					sleep(1); /* wait 1sec */
				}
				i_cli->cli_timer = 0;
				i_cli->stage = __STAGE_CLI_CHECK_ESSID;
				break;
			
			case __STAGE_CLI_ERROR_STOP:
				/* error 상태를 mainapp에 알려줘야 함 */
				quit = 1;
				netmgr_event_hub_link_status(NETMGR_DEV_TYPE_WIFI, NETMGR_DEV_ERROR);
				__cli_stop(cli_dev_name);
				break;
			}
			
			delay_msecs(TIME_CLI_CYCLE);
		}	
	}
	
	tObj->active = 0;
	dprintf("...exit!\n");
	
	return NULL;
}

/*****************************************************************************
* @brief    int netmgr_wlan_station_init(void)
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_wlan_cli_init(void)
{
	app_thr_obj *tObj = &i_cli->cObj;
	
	if (thread_create(tObj, THR_wlan_cli_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create netmgr wlan client thread\n");
		return EFAIL;
    }
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_wlan_cli_exit(void)
{
	app_thr_obj *tObj = &i_cli->cObj;

	/* delete usb scan object */
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_wlan_cli_start(void)
{
	app_thr_obj *tObj = &i_cli->cObj;
	netmgr_iw_supplicant_req_info_t *info;
	char *databuf;
	int i, key=0;
	
	/* shared memory로부터 5개의 ssid / password / channel등의 정보를 읽어온다 */
	databuf = (char *)(app_cfg->shm_buf + NETMGR_SHM_REQUEST_INFO_OFFSET);
	info = (netmgr_iw_supplicant_req_info_t *)databuf;
	
	for (i = 0; i < NETMGR_WLAN_CONN_MAX_CNT; i++) {
		/* null 문자 미리 확인된 상태임 */
		strcpy(userList[i].ssid, info->iw_data[i].ssid);
		key = info->iw_data[i].en_key;
		if (key) {
			/* password copy */
			strcpy(userList[i].passwd, info->iw_data[i].passwd);
		} else { 
			memset(userList[i].passwd, 0, NETMGR_WLAN_PASSWD_MAX_SZ);
		}
		userList[i].en_key = key;
	}
			
	/* set default to idle */
	i_cli->stage = __STAGE_CLI_MOD_WAIT;
	i_cli->cli_timer = 0;
	i_cli->dhcp = info->dhcp;
	
	memset(i_cli->ip, 0, NETMGR_NET_STR_MAX_SZ);
	memset(i_cli->mask, 0, NETMGR_NET_STR_MAX_SZ);
	memset(i_cli->gw, 0, NETMGR_NET_STR_MAX_SZ);
	
	if (i_cli->dhcp == 0) { //# static 
		strcpy(i_cli->ip, info->ip_address);
		strcpy(i_cli->mask, info->mask_address);
		strcpy(i_cli->gw, info->gw_address);
		sysprint("Wi-Fi client ip address set static!\n");
	} else {
		sysprint("Wi-Fi client ip address set dhcp!\n");
	}
	
	/* delete usb scan object */
   	event_send(tObj, APP_CMD_START, 0, 0);
	
	return 0;
}

/*****************************************************************************
* @brief    poll device(usb, usb2eth, rndis etc) exit
* @section  DESC Description: 
*   - desc
*****************************************************************************/
int netmgr_wlan_cli_stop(void)
{
	app_thr_obj *tObj = &i_cli->cObj;
			
	/* delete usb scan object */
   	event_send(tObj, APP_CMD_STOP, 0, 0);
	
	return 0;
}
