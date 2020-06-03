/*
 * File : app_net_manager.c
 *
 * Copyright (C) 2015 UDWORKs
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
#include <sys/types.h>
#include <sys/stat.h>

#include <app_main.h>
#include <app_net_manager.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FXN_NET_MGR_THR_PRI		(FXN_THR_PRI_DEFAULT + 1)

#define IWGETID_CMD				"/sbin/iwgetid wlan0"
#define IWCONFIG_CMD			"/sbin/iwconfig wlan0"

#define SUPPLICANT_CONFIG		"/etc/wpa_supplicant.conf"
#define SUPPLICANT_PID_PATH     "/var/run/wpa_supplicant.pid"
#define UDHCPC_PID_PATH			"/var/run/udhcpc.wlan0.pid"

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj *nObj;

	char essid[32]; /* current net essid */

} app_net_mgr_data_t;

static app_net_mgr_data_t app_net_mgr_data;
static app_net_mgr_data_t *app_net_mgr_pdata = &app_net_mgr_data;

/*****************************************************************************
* @brief    Get AP authentication status.
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_get_auth_status(char *essid)
{
	FILE *f = NULL;
	char lbuf[128 + 1];
	char id[32 + 1];

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
		char *s;

		/* find ESSID: */
		if ((s = strstr(lbuf, "ESSID:")) != NULL)
		{
			s += 7; /* --> ESSID:" */
			for (i = 0; i < strlen(essid); i++) {
				/* To fine next token: '"' */
				id[i] = s[i];
			}
			id[i] = '\0';

			if (strcmp(id, essid) == 0) {
				/* connection ok */
				r = 1;
				break;
			}
		}
	}

	pclose(f);

	return r;
}

/*****************************************************************************
* @brief    Wi-Fi wait for AP connection
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_get_link_status(char *essid, int *level)
{
	FILE *f = NULL;

	char lbuf[256 + 1];
	char tbuf[4];
	char id[32 + 1];

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
		char *s;
		int i, val;

		/* find ESSID: */
		if ((s = strstr(lbuf, "ESSID:")) != NULL) {
			s += 7; /* --> ESSID:" */
			for (i = 0; i < sizeof(lbuf) - 1; i++) {
				/* To fine next token: '"' */
				if (s[i] == '\0' || s[i] == '"') {
					break;
				}
				id[i] = s[i];
			}
			id[i] = '\0';

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

			return 1;
		}
	}

	pclose(f);

	return 0;
}

/*****************************************************************************
* @brief    create wps_passphrase..
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_get_phrase(const char *ssid,
					const char *password, char *output)
{
	FILE *f = NULL;

	char cmd[256] = {0,};
	char lbuf[256] = {0,};

	int flag = 0;

	if (output == NULL)
		return -1;

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

	if (f != NULL)
		pclose(f);

	return -1;
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
static int netmgr_cli_create_conf(const char *ssid,
							const char *password, int wep)
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
		ret = netmgr_cli_get_phrase(ssid, password, phrase);
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

	return 0;
}

#if 0
/*****************************************************************************
* @brief    Wi-Fi to check wpa_supplicant running..
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_is_supplicant_run(void)
{
	struct stat st;
	char buf[256] = {0,};
	FILE *f;

	int r = 0, pid;

	/* get udhcpc */
	memset(buf, 0, sizeof(buf));
	memset(&st, 0, sizeof(st));

	f = fopen(SUPPLICANT_PID_PATH, "r");
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
#endif

/*****************************************************************************
* @brief    Wi-Fi wpa_supplicant start..
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_start(const char *ssid,
					const char *password, int encrypt)
{
	char buf[256] = {0,};
	FILE *f;

	int r = 0;

	/* Before starting the daemon, make sure its config file exists */
	r = netmgr_cli_create_conf((char *)ssid, (char *)password, encrypt);
	if (r < 0) {
		/* Failed to wi-fi start */
		return r;
	}

	sync(); //# /etc/wpa_supplicant.conf
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

/*****************************************************************************
* @brief    Wi-Fi to check udhcpc running..
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_is_udhcpc_run(void)
{
	struct stat st;
	char buf[256] = {0,};
	FILE *f;

	int r = 0, pid;

	/* get udhcpc */
	memset(buf, 0, sizeof(buf));
	memset(&st, 0, sizeof(st));

	f = fopen(UDHCPC_PID_PATH, "r");
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

/*****************************************************************************
* @brief    Wi-Fi get ip address(udhcpc)..
* @section  DESC Description
*   - desc
*****************************************************************************/
static int netmgr_cli_get_ipaddr(void)
{
	char buf[256] = {0,};
	FILE *f;

	int r = 0;

	/* get udhcpc */
	memset(buf, 0, sizeof(buf));
	/*
	 * -A: tryagain after 3sec(def 20sec). wait if lease is not obtained
	 * -T: timeout 1sec. (default 3)
	 * -t: retry. (default 3)
	 * -b: background.
	 * -p: create pid file
	 */
	snprintf(buf, sizeof(buf), "/sbin/udhcpc -A 3 -T 1 -b -p %s -i wlan0", UDHCPC_PID_PATH);
	f = popen(buf, "w");
	if (f != NULL)
		pclose(f);

	return 0;
}

/*****************************************************************************
* @brief    Wi-Fi wpa_supplicant stop..
* @section  DESC Description
*   - desc
*****************************************************************************/
static void netmgr_cli_stop(void)
{
	FILE *f;

	int pid = 0;

	/* kill wpa_supplicant daemon */
    f = fopen(SUPPLICANT_PID_PATH, "r");
    if (f != NULL) {
	    fscanf(f, "%d", &pid);
	    fclose(f);
	    unlink((const char *)SUPPLICANT_PID_PATH);

	    kill(pid, SIGKILL);
	    waitpid(pid, NULL, 0);
	} else
		eprintf("couldn't stop wpa_supplicant\n");

	/* kill udhcpc daemon */
//	kill -9 $(cat $WPA_PID)
//	ifconfig $WPA_IFACE 0.0.0.0 down
	pid = 0;
	f = fopen(UDHCPC_PID_PATH, "r");
    if (f != NULL) {
	    fscanf(f, "%d", &pid);
	    fclose(f);
	    unlink((const char *)UDHCPC_PID_PATH);

	    kill(pid, SIGKILL);
	    waitpid(pid, NULL, 0);
	} else
		eprintf("couldn't stop udhcpc\n");

#if 0
	/* wlan0 link down */
	f = popen("/sbin/ifconfig wlan0 0.0.0.0 down", "r");
	if (f != NULL)
		pclose(f);
#endif
}

/*****************************************************************************
* @brief   wifi network manager thread main function.
*   - desc
*****************************************************************************/
static void *app_net_mgr_main(void *prm)
{
	app_thr_obj *tObj = (app_thr_obj *)prm;

	char cmd;

	int exit = 0, r;

	printf(" [task] %s start\n", __func__);

	tObj->active = 1;
	while (!exit)
	{
		cmd = THR_event_wait(tObj);
		switch(cmd) {
		case APP_CMD_STOP:
			exit = 1;
			break;
		case APP_CLI_START:
		{
			scan_info_t *net_info;

			/*
			 * get connection information from shared memory.
			 * memcpy((iwcli->sbuf + 1), winfo, sizeof(scan_info_t));
			 */
			net_info = (scan_info_t *)(app_shm->shm_buf + 1); //# shm_buf[0] is request done flag.
			dprintf("ssid: %s \t pwd: %s\t encrypt mode %d\n", net_info->ssid,
								net_info->pwd, net_info->en_key);

			r = netmgr_cli_start((const char *)net_info->ssid,
						(const char *)net_info->pwd, (int)net_info->en_key);
			if (!r) {
				/* TODO */
				strcpy(app_cfg->link_essid, net_info->ssid);
			} else {
				memset(app_cfg->link_essid, 0, 32);
				eprintf("Failed to start Wi-Fi Client(err = %d)\n", r);
			}

			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		case APP_CLI_STOP:
		{
			netmgr_cli_stop();

			/* clear essid */
			memset(app_cfg->link_essid, 0, 32);
			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		case APP_CLI_WAIT_FOR_AUTH:
		{
			int retry = 20; //# max 10s
			do {
				r = netmgr_cli_get_auth_status((char *)app_cfg->link_essid);
				if (r > 0)
					break;
				THR_waitmsecs(500);
				retry--;
			} while (retry > 0);

			/* r > 0 connection ok */
			app_shm->shm_buf[1] = r;
			app_cfg->ste.bit.link = r;
			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		case APP_CLI_GET_AUTH_STATUS:
		{
			r = netmgr_cli_get_auth_status((char *)app_cfg->link_essid);
			/* r > 0 connection ok */
			app_shm->shm_buf[1] = r;
			app_cfg->ste.bit.link = r;
			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		case APP_CLI_GET_NET_STATUS:
		{
			int l;

			if (app_cfg->ste.bit.link) {
				r = netmgr_cli_get_link_status((char *)app_cfg->link_essid, &l);
				if (r) {
					/* valid information */
					app_shm->shm_buf[1] = (char)l; //# level
				} else {
					/* invalid information */
					app_shm->shm_buf[1] = -1;
				}
			} else
				app_shm->shm_buf[1] = -1;

			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		case APP_CLI_GET_IP:
		{
			if (app_cfg->ste.bit.link) {
				if (netmgr_cli_is_udhcpc_run() == 0)
					netmgr_cli_get_ipaddr();
				else {
					/* already udhcpc running.. skip */
//					printf("previous udhcpc is running...\n");
				}
			}
			app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
			break;
		}
		default:
			eprintf("unknown event command = %x\n", cmd);
			break;
		}
	}
	tObj->active = 0;

	printf(" [app ] %s exit\n", __func__);

	return NULL;
}

/*****************************************************************************
* @brief    initialize for wi-fi connection manager.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_init(void)
{
	struct sched_param schedprm;
	app_thr_obj *tObj;

	pthread_attr_t tattr;
	pthread_mutexattr_t muattr;
  	pthread_condattr_t cond_attr;

	THR_semHndl *pSem;

	int r = FXN_ERR_NET_MGR_INIT;

	tObj = (app_thr_obj *)malloc(sizeof(app_thr_obj));
	if (tObj != NULL) {
		pSem = &tObj->sem;

		/* mutex and thread create */
		pthread_mutexattr_init(&muattr);
  		pthread_condattr_init(&cond_attr);

  		pthread_mutex_init(&pSem->lock, &muattr);
  		pthread_cond_init(&pSem->cond, &cond_attr);

  		pSem->count = 0;
  		pSem->maxCount = MAX_PENDING_SEM_CNT;

		pthread_condattr_destroy(&cond_attr);
 	 	pthread_mutexattr_destroy(&muattr);

		/* initialize thread attributes structure */
	  	pthread_attr_init(&tattr);
	  	pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	  	pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);

	  	schedprm.sched_priority = FXN_NET_MGR_THR_PRI;
	  	pthread_attr_setschedparam(&tattr, &schedprm);

		r = pthread_create(&tObj->thr.hndl, &tattr,
						app_net_mgr_main, (void *)tObj);
		pthread_attr_destroy(&tattr);

		if (!r) {
			//# resgister thread obj
			app_net_mgr_pdata->nObj = tObj;
		} else {
			eprintf("Couldn't Create thread [ret = %d]!\n", r);
			r = -1;
		}
	} else {
		eprintf("Failed to alloc app net manager obj!!\n");
	}

	if (r < 0)
		r = FXN_ERR_NET_MGR_INIT;

	return r;
}

/*****************************************************************************
* @brief    deinitialize for wi-fi connection manager.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_exit(void)
{
	app_thr_obj *tObj;
	THR_semHndl *pSem;
  	void *rVal;
	int r = -1;

  	if (app_net_mgr_pdata == NULL)
  		return r;

	/* delete usb monitor object */
	tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;
	if (tObj != NULL) {
		THR_event_send(tObj, APP_CMD_STOP, 0, 0);
		while (tObj->active) {
			THR_waitmsecs(20);
		}

		pSem = &tObj->sem;
		pthread_cond_destroy(&pSem->cond);
	  	pthread_mutex_destroy(&pSem->lock);

	  	r |= pthread_cancel(tObj->thr.hndl);
	  	r |= pthread_join(tObj->thr.hndl, &rVal);

		free(tObj);
		tObj = NULL;
	}

	return r;
}

/*****************************************************************************
* @brief     wi-fi connection start.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_start(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_START, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}

/*****************************************************************************
* @brief     wi-fi connection stop.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_stop(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_STOP, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}

/*****************************************************************************
* @brief     Get wlan ip address.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_get_ipaddr(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_GET_IP, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}

/*****************************************************************************
* @brief     wi-fi wait for connection.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_wait_for_auth(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_WAIT_FOR_AUTH, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}

/*****************************************************************************
* @brief     wi-fi get authentication status.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_auth_status(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_GET_AUTH_STATUS, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}

/*****************************************************************************
* @brief     wi-fi get network status.
* @section  [desc]
*****************************************************************************/
int app_net_mgr_cli_net_status(void)
{
	app_thr_obj *tObj = (app_thr_obj *)app_net_mgr_pdata->nObj;

	if (tObj != NULL) {
		THR_event_send(tObj, APP_CLI_GET_NET_STATUS, 0, 0);
	} else
		eprintf("please run app_net_mgr_init!!\n");

	return 0;
}
