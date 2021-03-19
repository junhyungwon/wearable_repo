/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_udpsock.c
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
#include "app_main.h"
#include "app_dev.h"
#include "dev_common.h"
#include "app_udpsock.h"
#include "app_ctrl.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define HWTEST_DEBUG   
#define RECVBUFFSIZE 1024

#ifdef HWTEST_DEBUG
#define DEBUG_PRI(msg, args...) printf("[HWTEST] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DEBUG_PRI(msg, args...) ((void)0)
#endif

typedef struct {
    app_thr_obj udpObj ;
    int rsock ;
    int ssock ;
    OSA_MutexHndl       mutex;
} app_udpsock_t ;

static char cMacAddr[18]; // Server's MAC address

char Macaddr[MAX_CHAR_16 + 2]; 
char Send_Buffer[PACKETSIZE*2] ;
char Gpsdata[32] ;
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_udpsock_t t_udpsock;
static app_udpsock_t *udpsock=&t_udpsock;

static char ReadBuffer[RECVBUFFSIZE] ;

static struct sockaddr_in serv_addr, send_addr;
static int addrlen = sizeof(struct sockaddr_in) ;


int GetMacAddress(char *mac_address)
{
    FILE *fd;
	int readlen;
	
	fd = fopen("/sys/class/net/eth0/address","r");
	if(fd == NULL)
        return -1;

    if((readlen = fread(mac_address, sizeof(char), 17, fd)) > 0)
    {
        printf("Mac:%s len:%d\n", mac_address, readlen) ;
    }
	
	fclose(fd);
    return 0;
}

int GetMac(char *mac_address)
{
    int cur_idx = 0 ;
    bzero( (void *)&cMacAddr[0], sizeof(cMacAddr) );
    if ( !GetMacAddress(cMacAddr) )
    {
        char *ptr = strtok(cMacAddr, ":") ;
        while(ptr != NULL)
        {
            sprintf(&mac_address[cur_idx], "%s", ptr) ;
            cur_idx = strlen(mac_address) ;
            ptr = strtok(NULL, ":") ;
        }
        return 0;
    }
    else
    {
        printf( "Fatal error: Failed to get local host's MAC address\n" );
    }
    return -1 ;
}


int create_recvsock()
{
    int recv_sock = HWSOCK_ERROR;

	struct timeval read_timeout ;

	read_timeout.tv_sec = 0 ;
	read_timeout.tv_usec = 100 ;

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0) ;

    setsockopt(recv_sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout)) ;

    if (recv_sock != HWSOCK_ERROR)
    {
        memset(&serv_addr, 0, sizeof(serv_addr)) ;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
        serv_addr.sin_port = htons(HWTEST_PORT) ;
        bzero(&(serv_addr.sin_zero), 8) ;
 
        if(bind(recv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		{

#ifdef HWTEST_DEBUG
            DEBUG_PRI("rsocket open error\n") ;
#endif
            recv_sock = HWSOCK_ERROR ;
        }
		else
            DEBUG_PRI("rsocket bind success\n") ;
    }
    return recv_sock ;
}

int create_sendsock()
{
    int send_sock = HWSOCK_ERROR;

	int so_broadcast = 1 ;

    send_sock = socket(PF_INET, SOCK_DGRAM, 0) ;
    if(send_sock != HWSOCK_ERROR)
    {
        memset(&send_addr, 0, sizeof(send_addr)) ;
        send_addr.sin_family = AF_INET;
        send_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR) ;
        send_addr.sin_port = htons(HWTEST_PORT + 1) ;
        bzero(&(send_addr.sin_zero), 8) ;

		setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void *)&so_broadcast, sizeof(so_broadcast)) ;
    }
	else
	{
#ifdef HWTEST_DEBUG
            DEBUG_PRI("ssocket open error\n") ;
#endif
    }
    return send_sock ;
}

void send_jpeg()
{
	int jpeg_len = 0, result = 0, filecnt = 0, last_size = 0 ;
	int i = 0, stream_size = 0, sendlen = 0 ;
	CAMERA_RES Camerares ;
	FILE *jfd = NULL;
    char *buffer ;

	jfd = fopen("/tmp/fitt360.jpeg","r") ;
	if (jfd != NULL)
	{
		fseek(jfd, 0, SEEK_END) ;
        jpeg_len = ftell(jfd) ;
        fseek(jfd, (off_t)0, SEEK_SET) ;
		buffer = (char *)malloc(sizeof(char) * jpeg_len) ;
		if(buffer != NULL)
			result = fread(buffer, 1, jpeg_len, jfd) ;

		if(result == jpeg_len)
		{

            filecnt = jpeg_len / PACKETSIZE + 1 ; 
            last_size = jpeg_len % PACKETSIZE ;
            stream_size = PACKETSIZE ;
		
			for(i = 0 ; i < filecnt ; i++)
			{
				if(i == filecnt - 1)
					stream_size = last_size ;

			    Camerares.identifier = htons(IDENTIFIER);
			    Camerares.cmd = htons(CMD_CAMERA_RES) ;
			    Camerares.length = htons(sizeof(CAMERA_RES) + stream_size) ;
			    sprintf(Camerares.macaddr, "%s", Macaddr) ;
			    Camerares.framesize = htonl(jpeg_len) ;
				Camerares.total_fragment = htons(filecnt) ;
				Camerares.fragno = htons(i);
                
                memcpy(Send_Buffer, &Camerares, sizeof(CAMERA_RES)) ;
				memcpy(&Send_Buffer[sizeof(CAMERA_RES)], &buffer[i*PACKETSIZE], stream_size) ;
	            if(udpsock->ssock != HWSOCK_ERROR)
	            {
                    sendlen = sendto(udpsock->ssock, Send_Buffer, sizeof(CAMERA_RES) + stream_size, 0, (struct sockaddr *)&send_addr, addrlen);

#ifdef HWTEST_DEBUG
                    DEBUG_PRI("send CAMERA total frag = %d fragno = %d packet sendlen = %d\n",filecnt, i, sendlen) ;
	                if(sendlen < 0)
	                {
                        DEBUG_PRI("send CAMERA packet errno = %d\n",errno) ;
                    }
#endif
	            }
				OSA_waitMsecs(10);
			}
		}

        fclose(jfd);
		free(buffer) ;
    }

}

void send_keyPress(int Dir)
{
	int sendlen = 0;
    BUTTON_RES Buttonres ;

    Buttonres.identifier = htons(IDENTIFIER) ;
	Buttonres.cmd = htons(CMD_BUTTON_RES) ;
	Buttonres.length = htons(sizeof(BUTTON_RES)) ;
	sprintf(Buttonres.macaddr, "%s",Macaddr) ;
	Buttonres.dir = htons(Dir) ;

	if(udpsock->ssock != HWSOCK_ERROR)
	{
        sendlen = sendto(udpsock->ssock, &Buttonres, sizeof(BUTTON_RES), 0, (struct sockaddr *)&send_addr, addrlen);

#ifdef HWTEST_DEBUG
        DEBUG_PRI("send Button packet sendlen = %d\n",sendlen) ;
	    if(sendlen < 0)
	    {
            DEBUG_PRI("send Button packet errno = %d\n",errno) ;
        }
#endif
	}

}

int set_serialno(char *data)
{
	int retval = 0 ;
    SERIAL_SET *Serialset = (SERIAL_SET *)data ; 
    
	retval = dev_board_serial_write(Serialset->serial_no, 16) ;
	return retval;
}

int get_serial(char *data)
{
	int retval = 0 ;

	retval = dev_board_serial_read(data, 16) ;
	return retval;
}	

int set_board_uid(char *data)
{
	int retval = 0 ;
    UID_SET *Uidset = (UID_SET *)data ;

	retval = dev_board_uid_write(Uidset->uid , 16) ;
	return retval ;

}


int get_board_uid(char *data)
{
	int retval = 0 ;

	retval = dev_board_uid_read(data , 16) ;
	return retval ;
}

int ctrl_time_set(int year, int mon, int day, int hour, int min, int sec)
{
    struct tm ts;
    time_t set;
    char buf[64];

    ts.tm_year = year;
    ts.tm_mon  = mon;
    ts.tm_mday = day;

    ts.tm_min  = min;
    ts.tm_sec  = sec;

    set = mktime(&ts);

    stime(&set);
    Vsys_datetime_init();   //# m3 Date/Time init
    OSA_waitMsecs(100);

    if (dev_rtc_set_time(ts) < 0) {
        DEBUG_PRI("Failed to set system time to rtc !!!");
    }

    return 0;
}


void send_sysinfo(char *data)
{
	INFO_REQ *Inforeq ;
    INFO_RES Infores ;

    struct tm ts ;
	time_t tm1 ;

    int sendlen = 0 ;
//    int	year, month, day, hour, min, sec;
	char Micom_ver[8] ;
	char Hw_ver[8];
	char SerialNo[MAX_CHAR_16] ;
	char Uid[MAX_CHAR_32] ;
	char ipaddr[MAX_CHAR_16] ;
	char subnet[MAX_CHAR_16] ;
	char gateway[MAX_CHAR_16] ;

    Inforeq = (INFO_REQ *)data ;
    
	memset(Macaddr, 0x00, MAX_CHAR_16 + 2);
	memset(SerialNo, 0x00, MAX_CHAR_16);
	memset(Uid, 0x00, MAX_CHAR_32);
	
	if(GetMacAddress(Macaddr))
	    sprintf(Macaddr, "%s", "UNKNOWN");  
/*
    year = ntohs(Inforeq->year) - 1900 ;
    month = ntohs(Inforeq->month) - 1;
    day = ntohs(Inforeq->day) ;
    hour = ntohs(Inforeq->hour) ;
    min = ntohs(Inforeq->min) ;
    sec = ntohs(Inforeq->sec) ;

    ctrl_time_set(year, month, day, hour, min, sec) ;
*/
// set time to rtc

    memset(&Infores, 0x00, sizeof(INFO_RES));

	if(get_serial(SerialNo) == -1)
	{
		memset(SerialNo, 0x00, MAX_CHAR_16) ;
    }
	
	if(get_board_uid(Uid) == -1)
	{
		memset(Uid, 0x00, MAX_CHAR_32) ;
	}

	ctrl_get_hw_version(Hw_ver) ;
	ctrl_get_mcu_version(Micom_ver) ;
    ctrl_ether_cfg_read(ETHER_CFG_NAME, ipaddr, subnet, gateway) ;

    Infores.identifier = htons(IDENTIFIER) ;
    Infores.cmd = htons(CMD_INFO_RES) ;
	Infores.length = htons(sizeof(INFO_RES) - 6);

	sprintf(Infores.macaddr, "%s",  Macaddr);   // todo
	sprintf(Infores.serialno, "%s", SerialNo) ;  // todo
    sprintf(Infores.ipaddr, "%s", ipaddr);
    sprintf(Infores.micom, "%s", Micom_ver) ;
    sprintf(Infores.hwver, "%s", Hw_ver) ;
    sprintf(Infores.fwver, "%s", FITT360_SW_VER) ;
 
    Infores.voltage = htons(iapp->voltage) ;
    Infores.rtc_status = htons(iapp->ste.b.rtc);

	time(&tm1) ;
	localtime_r(&tm1, &ts) ;

	Infores.year = htons(ts.tm_year + 1900) ;
	Infores.month = htons(ts.tm_mon + 1 ) ;
	Infores.day = htons(ts.tm_mday) ;
	Infores.hour = htons(ts.tm_hour) ;
	Infores.min = htons(ts.tm_min) ;
	Infores.sec = htons(ts.tm_sec) ;

    Infores.usb_status = htons(iapp->ste.b.usb) ;
    Infores.gps_status = htons(iapp->ste.b.gps_jack) ;
	get_gpsdata(Gpsdata) ;

    sprintf(Infores.gps_data, "%s",Gpsdata ) ; 	
	sprintf(Infores.uid, "%s", Uid) ;
    
	if(udpsock->ssock != HWSOCK_ERROR)
	{
        sendlen = sendto(udpsock->ssock, &Infores, sizeof(INFO_RES), 0, (struct sockaddr *)&send_addr, addrlen);

#ifdef HWTEST_DEBUG
        DEBUG_PRI("send sysinfo packet sendlen = %d\n",sendlen) ;
	    if(sendlen < 0)
	    {
            DEBUG_PRI("send sysinfo packet errno = %d\n",errno) ;
        }
#endif
	}

}

int processdata (char *data)
{
    STRUCT_BASE *Struct_base ;

    unsigned short cmd ;

	Struct_base = (STRUCT_BASE *)data ;

    cmd = ntohs(Struct_base->cmd) ;

	if(cmd != CMD_INFO_REQ)
	{
        if(!GetMacAddress(Macaddr))
		{
            if(strncmp(Struct_base->macaddr, Macaddr, 17)) 
			    return -1 ; 
		}
    }

	switch(cmd)
	{
		case CMD_INFO_REQ :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_INFO_REQ \n") ;
#endif
            send_sysinfo(data) ;
		    break ;
			
		case CMD_LED_REQ :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_LED_REQ \n") ;
#endif
            check_led_operate() ;
		    break ;

		case CMD_MIC_REQ :
			break ;

		case CMD_BUZZER_REQ :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_BUZZER_REQ \n") ;
#endif
   	        app_buzzer(300, 1) ;
			break ;
         
		case CMD_CAMERA_REQ :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_CAMERA_REQ \n") ;
#endif
            send_jpeg() ;
			break ;
         
		case CMD_SERIAL_SET :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_SERIAL_SET \n") ;
#endif
            set_serialno(data) ;
			break ;

		case CMD_UID_SET :
#ifdef HWTEST_DEBUG
    DEBUG_PRI("recv CMD_UID_SET \n") ;
#endif
            set_board_uid(data) ;
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
static void *THR_udpsock(void *prm)
{
    app_thr_obj *tObj = &udpsock->udpObj;
    int cmd, retry_cnt = 0, receive_len = 0;
    int exit = 0;

    tObj->active = 1;

    udpsock->rsock = create_recvsock();
    udpsock->ssock = create_sendsock();

    while (!exit)
    {
	//# wait cmd
		
        cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
	    	break;
		}

        if(udpsock->rsock == HWSOCK_ERROR)
        {
            app_msleep(20);

            retry_cnt += 1;
            if(retry_cnt > 250)
            {
                udpsock->rsock = create_recvsock();
#ifdef HWTEST_DEBUG
                  DEBUG_PRI("udpsock socket create..\n") ;
#endif
                retry_cnt = 0;
            }
        }
        else
        {
			memset(ReadBuffer, 0x00, RECVBUFFSIZE) ;
            receive_len = recvfrom(udpsock->rsock, ReadBuffer, RECVBUFFSIZE,0, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen);
			if(receive_len >= 6)
			{
#ifdef HWTEST_DEBUG
            DEBUG_PRI("recv_sock = %d receive_len = %d\n",udpsock->rsock, receive_len) ;
#endif
				processdata(ReadBuffer) ;
            }
			else if(receive_len < FALSE)
            {
                switch(errno)
                {
                    case EWOULDBLOCK :
#ifdef HWTEST_DEBUG
//                  DEBUG_PRI("ReadSocketData EWOULDBLOCK..\n") ;
#endif
//                    close(udpsock->rsock) ;
                    break ;

                    case EPIPE :
#ifdef HWTEST_DEBUG
                    DEBUG_PRI("ReadSocketData EPIPE..\n") ;
#endif
                    close(udpsock->rsock) ;
                    break ;

                    case ENOTSOCK :
#ifdef HWTEST_DEBUG
                    DEBUG_PRI("ReadSocketData ENOTSOCK\n") ;
#endif
                    close(udpsock->rsock) ;
                    break ;

                    case ECONNRESET :
#ifdef HWTEST_DEBUG
                    DEBUG_PRI("ReadSocketData ECONNRESET\n") ;
#endif
                    close(udpsock->rsock) ;
                    break ;

                    case 110 :
#ifdef HWTEST_DEBUG
                    DEBUG_PRI("ReadSocketData CONNECTION TIMEOUT\n") ;
#endif

                    close(udpsock->rsock) ;
                    break ;


                    default :
#ifdef HWTEST_DEBUG
                    DEBUG_PRI("ReadSocketData errno = %d\n",errno) ;
#endif
                    close(udpsock->rsock) ;
                    udpsock->rsock = HWSOCK_ERROR ;
                    break ;
                }
            }
        }

        app_msleep(10) ;
    }

    if(udpsock->rsock != HWSOCK_ERROR)
        close(udpsock->rsock) ;

    tObj->active = 0;

    return NULL;
}

void HWSOCK_ERROR_proc(int Err)
{
    switch(Err)
    {
            case EWOULDBLOCK :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData EWOULDBLOCK..\n") ;
#endif
            close(udpsock->rsock) ;
            break ;

            case EPIPE :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData EPIPE..\n") ;
#endif
            close(udpsock->rsock) ;
            break ;

            case ENOTSOCK :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData ENOTSOCK \n") ;
#endif
            close(udpsock->rsock) ;
            break ;

            case ECONNRESET :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData ECONNRESET \n") ;
#endif
            close(udpsock->rsock) ;
            break ;

            case 110 :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData CONNECTION TIMEOUT \n") ;
#endif
            close(udpsock->rsock) ;
            break ;


            default :
#ifdef HWTEST_DEBUG
            DEBUG_PRI("SendSocketData errno = %d\n",errno) ;
#endif
            close(udpsock->rsock) ;
            break ;
    }
}

/*****************************************************************************
* @brief    udpsock thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_udpsock_init(void)
{
    app_thr_obj *tObj ;

    memset(udpsock, 0x0, sizeof(app_udpsock_t)) ;

    //# create meta thread ;
    tObj = &udpsock->udpObj ;
    if(thread_create(tObj, THR_udpsock, APP_THREAD_PRI, NULL) < 0)
    {
        eprintf("create sock thread\n") ;
        return EFAIL ;
    }
	
	aprintf("... done!\n");

    return 0;
}

void app_udpsock_exit(void)
{
    app_thr_obj *tObj;

//    if(!app_cfg->ste.b.sock)
//        return ;

    tObj = &udpsock->udpObj;
    event_send(tObj, APP_CMD_STOP, 0, 0) ;

    while(tObj->active)
        app_msleep(20);

    thread_delete(tObj);
	
	aprintf("... done!\n");
}
