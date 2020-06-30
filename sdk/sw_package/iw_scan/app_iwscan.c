/*
 * File : app_iwlist.c
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
#include <math.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <app_main.h>
#include <app_iwscan.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define FXN_ISCAN_THR_PRI		(FXN_THR_PRI_DEFAULT + 1)
#define CMD_IWLIST				"/sbin/iwlist wlan0 scanning"
#define NET_IF_NAME				"wlan0"
#define LINE_BUF_SIZE			512

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj *sObj;

} app_iwscan_data_t;

static app_iwscan_data_t app_iwscan_data;
static app_iwscan_data_t *app_iwscan_pdata = &app_iwscan_data;

static char lbuf[LINE_BUF_SIZE] = {0,};
static iwscan_list_t *piwscan_list;
//static unsigned char *shm_buf;

static int app_iscan_run(void)
{
	FILE *f;

	char mac[18];
	char ssid[LINE_BUF_SIZE];

	int wep, inCell, valid;
	int master, index, level;
	int r = 0;
	int i;

	app_shm->shm_buf[0] = 0;	//# scan flag reset

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

	index = 0; 	inCell = 0; valid = 0;
	level = -1; wep = -1; master = -1;

	memset(lbuf, 0, sizeof(lbuf));

	f = popen(CMD_IWLIST, "r");
	if (f != NULL)
	{
		while (fgets(lbuf, sizeof(lbuf), f) != NULL)
		{
			char *start;
			int length;

			length = strlen(lbuf);
			/* find wlan0 */
			if ((start = strstr(lbuf, NET_IF_NAME)) != NULL) {
				valid = 1;
				continue; /* goto next line for parser */
			}

			if (!valid) {
				/* if don't display --> wlan0 Scan completed */
				continue; /* goto next line */
			}

			/* find Cell strings */
			if ((start = strstr(lbuf, "Cell ")) != NULL) {
				//# find Address: xx:xx:xx:xx:xx:xx
			    int rem;

			    rem  = (lbuf + length) - (start + strlen("Cell XX - Address: xx:xx:xx:xx:xx:xx"));
			    if (rem < 0) {
					//# invalid input
					eprintf("can't find Cell XX line.!!\n");
					continue; /* goto next line */
				}

				strncpy(mac, start + strlen("Cell XX - Address: "), 17);
				mac[17] = '\0';
				inCell = 1;

			} else if (inCell && (start = strstr(lbuf, "ESSID:\""))) {
				start += 7; /* --> ESSID:" */
				for (i = 0; i < (LINE_BUF_SIZE - 1); i++) {
					if (start[i] == '\0' || start[i] == '"') {
						break;
					}
					/* fill ssid */
					ssid[i] = start[i];
				}
				ssid[i] = '\0';

				/* check ascii */
				if (ssid[0] == '\0' || ssid[0] > '\x7F') {
//					printf("founded cell ssid %s no ascii!!\n", ssid);
					/* invalid (no ascii) */
					ssid[0] = '\0';
				}
			} else if (inCell && (start = strstr(lbuf, "Mode:"))) {
				if (strstr(start, "Master")) {
					master = 1;
				} else {
					master = 0; /* ad-hoc or station */
				}

			} else if (inCell && (start = strstr(lbuf, "Signal level="))) {
				char numstr[4];

				//# want to see if there are at least two more places
				if ((lbuf + length) - (start + 3) < 1) {
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

			} else if (inCell && (start = strstr(lbuf, "Encryption key:"))) {
				start += 15; /* Encryption key: */
				if (strstr(start, "on")) {
					wep = 1;
				} else {
					wep = 0;
				}
			}

			if (ssid[0] != '\0' && level != -1 && wep != -1 && master != -1 && mac[0] != '\0')
			{
				#if 0
				printf("founded cell[%d] mac address %s\n", (index+1), mac);
				printf("founded cell[%d] ssid %s\n", (index+1), ssid);
				printf("founded cell[%d] signal level %d\n", (index+1), level);
				printf("founded cell[%d] mode %s\n", (index+1), master?"master":"station");
				printf("founded cell[%d] encrypt wpa %s\n", (index+1), wep?"on":"open");
				#endif

				/* save current incell information */
				piwscan_list->info[index].level = level;
				piwscan_list->info[index].en_key = wep;
				strcpy(piwscan_list->info[index].ssid, ssid);
				index++;

				/* initialize parser helper */
				memset(lbuf, 0, sizeof(lbuf));
				mac[0] = '\0'; ssid[0] = '\0';

				inCell = 0; level = -1;
				wep = -1; master = -1;
			} /* if (ssid[0] != '\0' && level != -1 && wep != -1 && master != -1 && mac[0] != '\0') */

		} /* while (fgets(lbuf, sizeof(lbuf), f) != NULL) */

		/* scan done!! */
		piwscan_list->num = index;

	} else {
		dprintf("could not open %s\n", CMD_IWLIST);
		r = -1;
	}

	app_shm->shm_buf[0] = WIFI_REQUEST_DONE;	//# set flag
	printf(" [app_iscan] %s done...\n", __func__);
	return r;
}

static void *app_iscan_main(void *prm)
{
	app_thr_obj *tObj = (app_thr_obj *)prm;
	int exit = 0, r;

	char cmd;

	printf(" [task] %s start\n", __func__);

	tObj->active = 1;
	while (!exit)
	{
		cmd = THR_event_wait(tObj);
		if (cmd == APP_CMD_STOP)
			break;

		if (cmd == APP_CMD_NOTY)
		{
			if (app_cfg->ste.bit.udev) {
				app_iscan_run();
			}
		}
	}
	tObj->active = 0;

	printf(" [app ] %s exit\n", __func__);

	return NULL;
}

/*****************************************************************************
* @brief    initialize for poll Wi-Fi AP.
* @section  [desc]
*****************************************************************************/
int app_iscan_init(void)
{
	struct sched_param schedprm;
	app_thr_obj *tObj;

	pthread_attr_t tattr;
	pthread_mutexattr_t muattr;
  	pthread_condattr_t cond_attr;

	THR_semHndl *pSem;

	int r = -1;

	piwscan_list = app_shm->shm_buf+1;	//# shm_buf[0] is list_up flag.

	//# Start wifi scan proc.
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
	 	/* set schedule FIFO (first input first output) */
	 	pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);

	  	/* set schedule priority */
	  	schedprm.sched_priority = FXN_ISCAN_THR_PRI;
	  	pthread_attr_setschedparam(&tattr, &schedprm);

		r = pthread_create(&tObj->thr.hndl, &tattr,
						app_iscan_main, (void *)tObj);
		pthread_attr_destroy(&tattr);

		if (!r) {
			//# resgister thread obj
			app_iwscan_pdata->sObj = tObj;
		} else {
			eprintf("Couldn't Create thread [ret = %d]!\n", r);
			r = -1;
		}
	} else {
		eprintf("Failed to alloc app iscan obj!!\n");
	}

	if (r < 0)
		r = FXN_ERR_IWSCAN_INIT;

	return r;
}

/*****************************************************************************
* @brief    deinitialize for poll Wi-Fi AP.
* @section  [desc]
*****************************************************************************/
int app_iscan_exit(void)
{
	app_thr_obj *tObj;
	THR_semHndl *pSem;
  	void *rVal;
	int r = -1;

  	if (app_iwscan_pdata == NULL)
  		return r;

	/* delete iscan object */
	tObj = (app_thr_obj *)app_iwscan_pdata->sObj;
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

void app_iscan_start(void)
{
	app_thr_obj *tObj;

  	if (app_iwscan_pdata == NULL)
  		return;

	tObj = (app_thr_obj *)app_iwscan_pdata->sObj;
	if (tObj != NULL)
		THR_event_send(tObj, APP_CMD_NOTY, 0, 0);
}

