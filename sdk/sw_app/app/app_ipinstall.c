/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_ipinstall.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *    - 2019.10.29 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
   Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <err.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/stat.h>

#include "dev_common.h"
#include "app_comm.h"
#include "app_dev.h"
#include "app_ipinstall.h"
#include "app_set.h"
#include "app_ctrl.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define IPINSTALL_DEBUG 
#define RECVBUFFSIZE 1024

#ifdef IPINSTALL_DEBUG
#define DEBUG_PRI(msg, args...) printf("[IPINSTALL] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DEBUG_PRI(msg, args...) ((void)0)
#endif

typedef struct {
    app_thr_obj ipinsObj ;
    int rsock ;
    int ssock ;
    OSA_MutexHndl       mutex;
} app_ipins_t ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_ipins_t t_ipins;
static app_ipins_t *ipins=&t_ipins;

static char ReadBuffer[RECVBUFFSIZE] ;

static struct sockaddr_in serv_addr, send_addr;
static int addrlen = sizeof(struct sockaddr_in) ;

int create_recvsock()
{
    int recv_sock = IPSOCK_ERROR;

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0) ;
    if (recv_sock != IPSOCK_ERROR)
    {
        memset(&serv_addr, 0, sizeof(serv_addr)) ;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
        serv_addr.sin_port = htons(IPINSTALL_PORT) ;
        bzero(&(serv_addr.sin_zero), 8) ;
 
        if(bind(recv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		{

#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("rsocket open error\n") ;
#endif
            recv_sock = IPSOCK_ERROR ;
        }
		else
            DEBUG_PRI("rsocket bind success\n") ;
    }
    return recv_sock ;
}

int create_sendsock()
{
    int send_sock = IPSOCK_ERROR;

	int so_broadcast = 1 ;

    send_sock = socket(PF_INET, SOCK_DGRAM, 0) ;
    if(send_sock != IPSOCK_ERROR)
    {
        memset(&send_addr, 0, sizeof(send_addr)) ;
        send_addr.sin_family = AF_INET;
        send_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR) ;
        send_addr.sin_port = htons(IPINSTALL_PORT + 1) ;
        bzero(&(send_addr.sin_zero), 8) ;

		setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_broadcast, sizeof(so_broadcast)) ;
    }
	else
	{
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("ssocket open error\n") ;
#endif
    }
    return send_sock ;
}

void send_sysinfo(void)
{
    INFO_RES Infores ;
    int sendlen = 0;
    char Macaddr[MAX_CHAR_16 + 2]; 
	char compbuff[32] ;

//    if(change)
//		return ;

	memset(Macaddr, 0x00, MAX_CHAR_16 + 2);
	GetSvrMacAddress(Macaddr) ;

#ifdef IPINSTALL_DEBUG
    DEBUG_PRI("get Macaddress = %s\n",Macaddr) ;
#endif
    memset(&Infores, 0x00, sizeof(INFO_RES));
    Infores.identifier = htons(IDENTIFIER) ;
    Infores.cmd = htons(CMD_INFO_RES) ;
	Infores.length = htons(sizeof(INFO_RES) - 6);
	Infores.eth_type = htons(app_set->net_info.type) ;
	
	/* rupy: st_cradle로 통합관리 */
	if (app_cfg->ste.b.cradle_net_run)
        sprintf(Infores.ipaddr, "%s", app_set->net_info.eth_ipaddr);
	else
        sprintf(Infores.ipaddr, "%s", "NA");

	sprintf(Infores.subnet, "%s", app_set->net_info.eth_netmask) ;
	sprintf(Infores.gateway, "%s", app_set->net_info.eth_gateway) ;
	Infores.wireless_type = htons(app_set->net_info.wtype) ;

    if (app_cfg->ste.b.usbnet_run)    
        sprintf(Infores.w_ipaddr, "%s", app_set->net_info.wlan_ipaddr);
    else
        sprintf(Infores.w_ipaddr, "%s", "NA");

	sprintf(Infores.w_subnet, "%s", app_set->net_info.wlan_netmask) ;
	sprintf(Infores.w_gateway, "%s", app_set->net_info.wlan_gateway) ;
	sprintf(Infores.fw_version, "%s", app_set->sys_info.fw_ver) ;
	sprintf(Infores.macaddr, "%s",  Macaddr);
	sprintf(Infores.deviceid, "%s", app_set->sys_info.deviceId) ;

    strcpy(compbuff, MODEL_NAME);
	if(strcmp(compbuff, "FITT360_Security") == 0)
	{
		sprintf(Infores.uid, "%s", "NA") ;
	}
	else
		sprintf(Infores.uid, "%s",app_set->sys_info.uid) ;

	if(ipins->ssock != IPSOCK_ERROR)
	{
        sendlen = sendto(ipins->ssock, &Infores, sizeof(INFO_RES), 0, (struct sockaddr *)&send_addr, addrlen);
//        sendlen = sendto(ipins->rsock, &Infores, sizeof(INFO_RES), 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

#ifdef IPINSTALL_DEBUG
        DEBUG_PRI("send sysinfo packet sendlen = %d\n",sendlen) ;
	    if(sendlen < 0)
	    {
            DEBUG_PRI("send sysinfo packet errno = %d\n",errno) ;
        }
#endif
	}

}

void change_info(char *data)
{
	CHANGE_INFO *Changeinfo ;
    char Macaddr[MAX_CHAR_16 + 2] ; 
    int ret = 0, eth_type , wireless_type ;;

    Changeinfo = (CHANGE_INFO *)data;

//    change = 1 ;

	GetSvrMacAddress(Macaddr) ;
	TRACE_INFO("Changeinfo->macaddr = %s\n",Changeinfo->macaddr) ;
    TRACE_INFO("Macaddr = %s\n",Macaddr) ;

//	if(!strcmp(Changeinfo->macaddr, Macaddr))
	if(!strncmp(Changeinfo->macaddr, Macaddr, MAX_CHAR_16 + 1))
	{
        eth_type = htons(Changeinfo->eth_type) ; // STATIC/DHCP
        wireless_type = htons(Changeinfo->wireless_type);

		if(eth_type == NET_TYPE_STATIC)
		{	
			if(strcmp(app_set->net_info.eth_ipaddr, Changeinfo->ipaddr))
        	{
				sprintf(app_set->net_info.eth_ipaddr, "%s",Changeinfo->ipaddr) ;
				ret = 1 ;
			}
			if(strcmp(app_set->net_info.eth_netmask, Changeinfo->subnet))
        	{
				sprintf(app_set->net_info.eth_netmask, "%s",Changeinfo->subnet) ;
				ret = 1 ;
			}
			if(strcmp(app_set->net_info.eth_gateway, Changeinfo->gateway))
        	{
				sprintf(app_set->net_info.eth_gateway, "%s",Changeinfo->gateway) ;
				ret = 1 ;
			}
        	if(app_set->net_info.type != eth_type)
			{	
        	    app_set->net_info.type = eth_type ;
				ret = 1 ;
			}
		}
		else
		{	
        	if(app_set->net_info.type != eth_type)
			{	
		        app_set->net_info.type = eth_type ;
				ret = 1 ;
			}
	    }

		if(wireless_type == NET_TYPE_STATIC)
		{	
			if(strcmp(app_set->net_info.wlan_ipaddr, Changeinfo->w_ipaddr))
        	{
				sprintf(app_set->net_info.wlan_ipaddr, "%s",Changeinfo->w_ipaddr) ;
				ret = 1 ;
			}
			if(strcmp(app_set->net_info.wlan_netmask, Changeinfo->w_subnet))
        	{
				sprintf(app_set->net_info.wlan_netmask, "%s",Changeinfo->w_subnet) ;
				ret = 1 ;
			}
			if(strcmp(app_set->net_info.wlan_gateway, Changeinfo->w_gateway))
        	{
				sprintf(app_set->net_info.wlan_gateway, "%s",Changeinfo->w_gateway) ;
				ret = 1 ;
			}
        	if(app_set->net_info.wtype != wireless_type)
			{
        	    app_set->net_info.wtype = wireless_type ;
				ret = 1;
			}
		}
		else
		{	
        	if(app_set->net_info.wtype != wireless_type)
			{
		        app_set->net_info.wtype = wireless_type ;
				ret = 1 ;
			}
	    }

		if(strcmp(app_set->sys_info.deviceId, Changeinfo->deviceid))
		{
			sprintf(app_set->sys_info.deviceId, "%s", Changeinfo->deviceid); 
			ret = 1 ;
		}

		if(strcmp(app_set->sys_info.uid, Changeinfo->uid))
		{
			sprintf(app_set->sys_info.uid, "%s", Changeinfo->uid); 
            dev_board_uid_write(app_set->sys_info.uid, 16) ;

			ret = 1 ;
		}

	    if(ret)
		    ctrl_sys_halt(0); /* reboot */
	}
}

int processdata (char *data)
{
    INFO_REQ *Inforeq ;
    unsigned short cmd ;

	Inforeq = (INFO_REQ *)data ;

    cmd = ntohs(Inforeq->cmd) ;
	switch(cmd)
	{
		case CMD_INFO_REQ :
#ifdef IPINSTALL_DEBUG
    DEBUG_PRI("recv CMD_INFO_REQ \n") ;
#endif
            send_sysinfo() ;
			break ;
			
		case CMD_CHANGE_INFO :
            change_info(data) ;

			break ;

		default :
			break ;
	}

    return 0;
}

/*****************************************************************************
* @brief    sock main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_ipinstall(void *prm)
{
    app_thr_obj *tObj = &ipins->ipinsObj;
    int cmd, retry_cnt = 0, receive_len = 0;
    int exit = 0;

    tObj->active = 1;

    ipins->rsock = create_recvsock();
    ipins->ssock = create_sendsock();

    while (!exit)
    {
	//# wait cmd
		
        cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
	    	break;
		}

        if(ipins->rsock == IPSOCK_ERROR)
        {
            app_msleep(20);

            retry_cnt += 1;
            if(retry_cnt > 250)
            {
                ipins->rsock = create_recvsock();
#ifdef IPINSTALL_DEBUG
                  DEBUG_PRI("ipinstaller socket create..\n") ;
#endif
                retry_cnt = 0;
            }
        }
        else
        {
            receive_len = recvfrom(ipins->rsock, ReadBuffer, RECVBUFFSIZE,0, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen);
			if(receive_len >= sizeof(INFO_REQ))
			{
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("recv_sock = %d receive_len = %d\n",ipins->rsock, receive_len) ;
#endif
				processdata(ReadBuffer) ;
            }
			else if(receive_len < FALSE)
            {
                switch(errno)
                {
                    case EWOULDBLOCK :
#ifdef IPINSTALL_DEBUG
                  DEBUG_PRI("ReadSocketData EWOULDBLOCK..\n") ;
#endif
                    close(ipins->rsock) ;
                    break ;

                    case EPIPE :
#ifdef IPINSTALL_DEBUG
                    DEBUG_PRI("ReadSocketData EPIPE..\n") ;
#endif
                    close(ipins->rsock) ;
                    break ;

                    case ENOTSOCK :
#ifdef IPINSTALL_DEBUG
                    DEBUG_PRI("ReadSocketData ENOTSOCK\n") ;
#endif
                    close(ipins->rsock) ;
                    break ;

                    case ECONNRESET :
#ifdef IPINSTALL_DEBUG
                    DEBUG_PRI("ReadSocketData ECONNRESET\n") ;
#endif
                    close(ipins->rsock) ;
                    break ;

                    case 110 :
#ifdef IPINSTALL_DEBUG
                    DEBUG_PRI("ReadSocketData CONNECTION TIMEOUT\n") ;
#endif
                    LOGD("[main] Tcp Connect Timeout... !! on RECEIVER\n");

                    close(ipins->rsock) ;
                    break ;

                    default :
#ifdef IPINSTALL_DEBUG
                    DEBUG_PRI("ReadSocketData errno = %d\n",errno) ;
#endif
                    close(ipins->rsock) ;
                    ipins->rsock = IPSOCK_ERROR ;
                    break ;
                }
            }
        }

        app_msleep(10) ;
    }

    if(ipins->rsock != IPSOCK_ERROR)
        close(ipins->rsock) ;

    tObj->active = 0;

    return NULL;
}

void IPSOCK_ERROR_proc(int Err)
{
    switch(Err)
    {
            case EWOULDBLOCK :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData EWOULDBLOCK..\n") ;
#endif
            close(ipins->rsock) ;
            break ;

            case EPIPE :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData EPIPE..\n") ;
#endif
            close(ipins->rsock) ;
            break ;

            case ENOTSOCK :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData ENOTSOCK \n") ;
#endif
            close(ipins->rsock) ;
            break ;

            case ECONNRESET :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData ECONNRESET \n") ;
#endif
            close(ipins->rsock) ;
            break ;

            case 110 :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData CONNECTION TIMEOUT \n") ;
#endif
            close(ipins->rsock) ;
            break ;


            default :
#ifdef IPINSTALL_DEBUG
            DEBUG_PRI("SendSocketData errno = %d\n",errno) ;
#endif
            close(ipins->rsock) ;
            break ;
    }
}

/*****************************************************************************
* @brief    ipinstall thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_ipins_init(void)
{
    app_thr_obj *tObj ;

    memset(ipins, 0x0, sizeof(app_ipins_t)) ;

    //# create meta thread ;
    tObj = &ipins->ipinsObj ;
    if(thread_create(tObj, THR_ipinstall, APP_THREAD_PRI, NULL, __FILENAME__) < 0)
    {
        TRACE_ERR("create sock thread\n") ;
        return EFAIL ;
    }
	
	TRACE_INFO("... done!\n");

    return 0;
}

void app_ipins_exit(void)
{
    app_thr_obj *tObj;

    if(!app_cfg->ste.b.sock)
        return ;

    tObj = &ipins->ipinsObj;
    event_send(tObj, APP_CMD_STOP, 0, 0) ;

    while(tObj->active)
        app_msleep(20);

    thread_delete(tObj);
	
	TRACE_INFO("... done!\n");
}
