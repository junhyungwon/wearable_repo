/******************************************************************************
 * FITT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_netdev.c
 * @brief
 * @author  phoong
 * @section MODIFY history
 *     - 2018.07.02 : First Created
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
#include <sys/stat.h>

#define __USE_GNU
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "dev_common.h"
#include "app_comm.h"
#include "app_wcli.h"
#include "app_netdev.h"
#include "app_main.h"
#include "app_version.h"
#include "app_dev.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define NETDEV_CYCLE_TIME      200

#define RNDIS_OPER_PATH		  	"/sys/class/net/usb0/operstate"
#define RNDIS_MODULE_DIR	 	"/sys/module/rndis_host"
#define WIFI_OPER_PATH		  	"/sys/class/net/wlan0/operstate"
#define USBETHER_OPER_PATH	  	"/sys/class/net/eth1/operstate"
#define UDHCPC_PID_PATH		 	"/tmp/udhcpc.pid"

#define RNDIS_DEV_NAME_USB	  	1
#define RNDIS_DEV_NAME_ETH	  	2

#define RNDIS_STAGE_NOT_READY   	(-1)
#define RNDIS_STAGE_IDLE     		(0x00)
#define RNDIS_STAGE_WAIT_DEV_ACTIVE	(0x01)
#define RNDIS_STAGE_WAIT_DHCPC		(0x02)
#define RNDIS_STAGE_RUN				(0x03)

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    app_thr_obj rndisObj;	//# rndis thread
    app_thr_obj wifiObj;	//# wi-fi thread
    app_thr_obj usbethObj ; //# usbtoethernet thread

	int rndis_stage;
} app_netdev_t;
/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static app_netdev_t t_net_dev;
static app_netdev_t *inet_dev=&t_net_dev;

/*****************************************************************************
 * @brief    Get rndis mac address
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static int app_netdev_get_rndis_mac_addr(const char *ifce, char *buf)
{
    struct ifreq ifreq;
	int skfd, ret = -1;

	if (buf == NULL)
		return ret;

	memset(buf, 0, 32);
	memset(&ifreq, 0, sizeof(struct ifreq));
	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) 
		return ret;

	ifreq.ifr_hwaddr.sa_family = AF_INET;
	strncpy(ifreq.ifr_name, ifce, sizeof(ifreq.ifr_name));
	ret = ioctl(skfd, SIOCGIFHWADDR, &ifreq);
	if (ret >= 0) {
		//# conversion ntoa
		snprintf(buf, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
            ifreq.ifr_hwaddr.sa_data[0], ifreq.ifr_hwaddr.sa_data[1],
            ifreq.ifr_hwaddr.sa_data[2], ifreq.ifr_hwaddr.sa_data[3],
            ifreq.ifr_hwaddr.sa_data[4], ifreq.ifr_hwaddr.sa_data[5]);
		ret = 0;
		fprintf(stderr, "get rndis mac = %s\n", buf);
	} else {
		ret = -1;	
	}
	close(skfd);

	return ret;
}

/*****************************************************************************
 * @brief    Get rndis module driver state
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static int app_netdev_get_rndis_state(void)
{
    struct stat sb;
    int ret = -1;

	if (stat(RNDIS_MODULE_DIR, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		ret = 0;
	}

	return ret;
}

/*****************************************************************************
 * @brief    checking rndis device mode
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static int app_netdev_wait_for_rndis(void)
{
    FILE *f = NULL;
    char tmp_buf[32];
	int ret = -1, r;

	memset(tmp_buf, 0, sizeof(tmp_buf));
    if (0 == access(RNDIS_OPER_PATH, R_OK)) {
	    f = fopen(RNDIS_OPER_PATH, "r");
	    if (f != NULL) {
			fclose(f);
        }
		//# get hardware address
		r = app_netdev_get_rndis_mac_addr("usb0", tmp_buf);
		if (!r) {
			//# detected rndis usb0 interface.
			ret = RNDIS_DEV_NAME_USB;
			if (strncmp(tmp_buf, "00:00:00:00:00:00", 17) == 0) {
				//# set mac address temporarily
				f = popen("/sbin/ifconfig usb0 hw ether 00:11:22:33:44:55", "r");
				if (f != NULL) {
					pclose(f); 
				}
			} else { 
				dprintf("get tmp_buf mac = %s\n", tmp_buf);
			}
		}
	} 
	//# S10+ device recognized as eth1 
	else if (0 == access(USBETHER_OPER_PATH, R_OK)) { 
		f = fopen(USBETHER_OPER_PATH, "r");
		if (f != NULL) {
	        fclose(f);
        }
		//# get hardware address
		r = app_netdev_get_rndis_mac_addr("eth1", tmp_buf);
		if (!r) {
			//# detected rndis eth1 interface.
			ret = RNDIS_DEV_NAME_ETH;
			if (strncmp(tmp_buf, "00:00:00:00:00:00", 17) == 0) {
				//# set mac address temporarily
				f = popen("/sbin/ifconfig eth1 hw ether 00:11:22:33:44:55", "r");
				if (f != NULL) {
					pclose(f); 
				}
			} else {
				dprintf("get tmp_buf mac = %s\n", tmp_buf);
			}
			
		}
	}

	return ret;
}

static int app_netdev_wait_for_wifi(void)
{
    FILE *f = NULL;
    int ret = -1;

    if (0 == access(WIFI_OPER_PATH, R_OK)) {
	    f = fopen(WIFI_OPER_PATH, "r");
	    if (f != NULL) {
	        ret = 0;
	    }
    	fclose(f);
	}

	return ret;
}


static int app_netdev_wait_for_usbethernet(void)
{
    FILE *f = NULL;
    int ret = -1;

    if (0 == access(USBETHER_OPER_PATH, R_OK)) {
	    f = fopen(USBETHER_OPER_PATH, "r");
	    if (f != NULL) {
	        ret = 0;
	    }
    	fclose(f);
	}

	return ret;
}

/*****************************************************************************
 * @brief    rndis main function
 * @section  DESC Description
 *   - desc
*****************************************************************************/
static void *THR_rndis(void *prm)
{
    app_thr_obj *tObj = &inet_dev->rndisObj;
    int cmd, exit=0;
	int type=0;
	char tmp_buf[128];
	dev_net_info_t  inet;

	FILE *f = NULL;
    aprintf("enter...\n");

	tObj->active = 1;
	inet_dev->rndis_stage = RNDIS_STAGE_NOT_READY;

    while(!exit)
    {
		int stage;

        //# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

		stage = inet_dev->rndis_stage;
		switch (stage) {
		case RNDIS_STAGE_RUN:
			if (app_netdev_get_rndis_state()) 
			{
				if (app_cfg->ste.b.dial_run) {
					f = popen("/usr/bin/killall udhcpc", "w");
					if (f != NULL) 
					{
						pclose(f); 
					    // this means dial_run==0 && wifi_run==0 && usbethernet == 0
						if (!app_cfg->ste.b.wifi_run && !app_cfg->ste.b.eth1_run) {
							app_leds_rf_ctrl(LED_RF_OFF);
						}
						app_cfg->ste.b.dial_run = 0; // Here set, It's OK, 
					}
				}
				app_cfg->ste.b.dial_ready = 0;
				inet_dev->rndis_stage = RNDIS_STAGE_IDLE;
			} else {
				inet_dev->rndis_stage = RNDIS_STAGE_RUN; 
			}
			
			break;
		case RNDIS_STAGE_WAIT_DHCPC:
			if (app_cfg->ste.b.dial_ready) {
				//# check done pipe(udhcpc...)
				if (0 == access(UDHCPC_PID_PATH, F_OK)) {
					app_cfg->ste.b.dial_run = 1;
					app_leds_rf_ctrl(LED_RF_OK);
					inet_dev->rndis_stage = RNDIS_STAGE_RUN;

					memset(tmp_buf, 0, sizeof(tmp_buf));
					dev_get_net_info(((type == RNDIS_DEV_NAME_USB) ? "usb0": "eth1"), &inet);
					sprintf(tmp_buf, "[APP] --- rndis allocated ipaddress %s", inet.ip);
                	app_log_write(MSG_LOG_WRITE, tmp_buf);

				} else {
					inet_dev->rndis_stage = RNDIS_STAGE_WAIT_DHCPC;
				}
			}
			break;	
		case RNDIS_STAGE_WAIT_DEV_ACTIVE:
			if (app_cfg->ste.b.dial_ready) {
				/* usb lte dongle is detected, wait for usb0 ready */
				type = app_netdev_wait_for_rndis();
				if (type > 0) {
					memset(tmp_buf, 0, sizeof(tmp_buf));
					snprintf(tmp_buf, sizeof(tmp_buf)-1, "/sbin/udhcpc -t 5 -n -i %s -p "UDHCPC_PID_PATH, 
											(type == RNDIS_DEV_NAME_USB) ? "usb0": "eth1");
					f = popen(tmp_buf, "w");
					if (f != NULL) {
						app_leds_rf_ctrl(LED_RF_OFF);
						pclose(f); 
					}
					inet_dev->rndis_stage = RNDIS_STAGE_WAIT_DHCPC;
				} 
				else {
					app_cfg->ste.b.dial_ready = 0;
					inet_dev->rndis_stage = RNDIS_STAGE_NOT_READY;
				}
			}	
			break;
		case RNDIS_STAGE_IDLE:
		default:
			if (!app_netdev_get_rndis_state()) {
				app_cfg->ste.b.dial_ready = 1;
				inet_dev->rndis_stage = RNDIS_STAGE_WAIT_DEV_ACTIVE;
			} else {
				if (app_cfg->ste.b.dial_run) {
					f = popen("/usr/bin/killall udhcpc", "w");
					if (f != NULL)
					{
						pclose(f); 
					    // this means dial_run==0 && wifi_run==0 && usbethernet == 0
						if (!app_cfg->ste.b.wifi_run && !app_cfg->ste.b.eth1_run) {
							app_leds_rf_ctrl(LED_RF_OFF);
						}
						app_cfg->ste.b.dial_run = 0; // Here set, It's OK, 
					}
				}
				app_cfg->ste.b.dial_ready = 0;
				inet_dev->rndis_stage = RNDIS_STAGE_IDLE;
			}
			break;
		}

		app_msleep(NETDEV_CYCLE_TIME);
	}

    tObj->active = 0;
	aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
 * @brief    monitor RTL wifi main function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
static void *THR_wifi(void *prm)
{
	app_thr_obj *tObj = &inet_dev->wifiObj;

	int cmd;
	int exit = 0;

    while(!app_cfg->ste.b.cap)
	{
		app_msleep(NETDEV_CYCLE_TIME);
	}
    aprintf("enter...\n");
	tObj->active = 1;

	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

		if (app_cfg->ste.b.wifi_ready) 
		{
			/* usb Wi-Fi dongle detected, wait for wlan0 ready */
			if (!app_cfg->ste.b.wifi_run) 
			{
				if (!app_netdev_wait_for_wifi()) 
				{
					/* wait for wi-fi module loading in iwscan */
               	 	app_cfg->ste.b.wifi_run = 1;
					if(app_cfg->ste.b.wifi)
					    app_leds_rf_ctrl(LED_RF_OK);
					
					if(strcmp(MODEL_NAME, "NEXX360") == 0)
					{	
					    if(app_cfg->wmode.wifi_mode)
					    {
			                app_wconn_check_state(WCONN_STE_START);
                        }
					    else
					    {
                            set_ap_value();
					        ctrl_wifi_set(ON) ;  // AP MODE
					        app_leds_rf_ctrl(LED_RF_OK);
						    app_cfg->wmode.wifi_mode = 1 ;
						}
					}
					else
					    app_wconn_check_state(WCONN_STE_START);
				}
				else
				{	
					if(!app_cfg->ste.b.wifi)
			            app_leds_rf_ctrl(LED_RF_OFF);
				}
			}
			else
			{
				if(app_cfg->ste.b.wifi)
				    app_leds_rf_ctrl(LED_RF_OK);
			}
		} 
		else 
		{
		    if(strcmp(MODEL_NAME, "NEXX360") == 0)  // NEXX360 ONLY
			{
			    if(app_cfg->wmode.wifi_mode) // AP MODE
			    {
			        app_leds_rf_ctrl(LED_RF_OFF);
				    app_cfg->wmode.wifi_mode = 0 ;
               	    app_cfg->ste.b.wifi_run = 0;
                }
			}
			/* remove process -> wcli */
#if USE_WIRELESS
            if(!app_cfg->ste.b.dial_run && !app_cfg->ste.b.eth1_run) 
			{	// this means dial_run==0 && wifi_run==0 && usbeth1 == 0
				app_leds_rf_ctrl(LED_RF_OFF);
			}
#endif
//            app_cfg->ste.b.wifi_run = 0 ;

		}

		app_msleep(NETDEV_CYCLE_TIME);
	}

    tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
 * @brief    monitor USB TO ETHERNET main function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
static void *THR_usbeth(void *prm)
{
	app_thr_obj *tObj = &inet_dev->usbethObj;

	int cmd;
	int exit = 0, i = 0;

    aprintf("enter...\n");
	tObj->active = 1;

	while (!exit)
	{
		//# wait cmd
		cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }

		if (!app_netdev_wait_for_usbethernet() && !app_cfg->ste.b.dial_ready)  // For LTE dongles recognized as eth1
        {
				/* wait for usb2ethernet run */
            if(!app_cfg->ste.b.eth1_run)
            {
                util_set_net_info(DEV_NET_NAME_ETH1); //# "eth1"       
    
                for(i = 0; i < 5; i++)
                { 
                    if(dev_ste_ethernet(1))                         
        	            app_cfg->ste.b.eth1_run = 1;
                    else
        	            app_cfg->ste.b.eth1_run = 0;

                    app_msleep(NETDEV_CYCLE_TIME) ;
                }
            }
            else
            {
                if(dev_ste_ethernet(1))
                {
                    app_cfg->ste.b.eth1_run = 1;
			        app_leds_rf_ctrl(LED_RF_OK);
                }
                else
                {
                    app_cfg->ste.b.eth1_run = 0;
			        app_leds_rf_ctrl(LED_RF_OFF);
                } 
            } 
		}
		else {

#if USE_WIRELESS

            if(!app_cfg->ste.b.wifi_run && !app_cfg->ste.b.dial_run)
            {
			    app_leds_rf_ctrl(LED_RF_OFF);
            }
#endif
        	app_cfg->ste.b.eth1_run = 0;
		}

		app_msleep(NETDEV_CYCLE_TIME);
	}

    tObj->active = 0;
	aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    network device thread init/exit function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_netdev_init(void)
{
	app_thr_obj *tObj;

#if USE_WIRELESS
	 //#--- rndis
    tObj = &inet_dev->rndisObj;
    if (thread_create(tObj, THR_rndis, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create rndis thread\n");
		return EFAIL;
    }
	tObj = &inet_dev->wifiObj;
	if (thread_create(tObj, THR_wifi, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create Wi-Fi thread\n");
		return -1;
	}

	tObj = &inet_dev->usbethObj;
	if (thread_create(tObj, THR_usbeth, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create USBETHERNET thread\n");
		return -1;
	}

#endif

	aprintf("... done!\n");

	return 0;
}

void app_netdev_exit(void)
{
    app_thr_obj *tObj;

#if USE_WIRELESS

	//#--- rndis stop
    tObj = &inet_dev->rndisObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);

	thread_delete(tObj);
	//#--- Wi-Fi stop
    tObj = &inet_dev->wifiObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
    	app_msleep(20);

	thread_delete(tObj);

    //#--- USB2ETHERNET stop
    tObj = &inet_dev->usbethObj;
    event_send(tObj, APP_CMD_STOP, 0, 0);
    while(tObj->active)
        app_msleep(20);

    thread_delete(tObj);
#endif

    dprintf("... done!\n");
}
