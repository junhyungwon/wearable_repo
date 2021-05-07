/******************************************************************************
 * XBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_ftp.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2017.06.16 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glob.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <fcntl.h>
#include <errno.h>

#include "app_comm.h"
#include "app_ftp.h"
#include "app_dev.h"
#include "app_msg.h"
#include "app_main.h"
#include "app_rec.h"
#include "app_set.h"
#include "dev_common.h"
#include "app_file.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define FTP_CYCLE_TIME		100
#define FTP_TIME_OUT		5	//# 2sec
#define FTP_RETRY_CNT       5
#define MSIZE 8192*8  // buffer size

typedef struct {
	app_thr_obj ftpObj;	//# key thread
	int sdFtp;
	int img_save;
	int ftp_state;
	int file_cnt;
    int retry_cnt ;
	char path[MAX_CHAR_128];
} app_img_ftp_t;

/*----------------------------------------------------------------------------
 Declares variable)s
-----------------------------------------------------------------------------*/
static app_img_ftp_t t_ftp;
static app_img_ftp_t *iftp=&t_ftp;

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int ftpRecvResponse(int sock, char * buf)
{
    socklen_t lon ;
    int valopt = 0 ;
	int i, len;

    getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
    if(valopt)
    {
        return -1 ;
    }

	len = recv(sock, buf, MSIZE, 0);
	if (len == -1) {//receive the data
		perror("recv");
		return -1;
	}

	for (i=(len - 1); i>0; i--) {
		if (buf[i]=='.' || buf[i]=='\r') {
	 		buf[i+1]='\0';
	 		break;
		}
	}
	printf("%d,%s\n", len, buf); //print response to the screen

	return 0;
}

static int ftpNewCmd(int sock, char * buf, char * cmd, char * param)
{
    socklen_t lon ;
    int valopt = 0 ;

	strcpy(buf,cmd);

    if(strcmp(cmd, "CDUP"))
    {
	    if (strlen(param) > 0) {
		    strcat(buf," ");
		    strcat(buf,param);
	    }
    }

	strcat(buf,"\r\n");
	printf("ftpNewCmd *%s", buf); //print the cmd to the screen

    getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
    if(valopt)
    {
        return -1 ;
    }
    else
    {
	    if (send(sock, buf, strlen(buf), 0) == -1) {
		    perror("send");
		    return -1;
	    }
    }  

	//clear the buffer
	return 0;
}

static int ftpConvertAddy(char * buf, char * hostname, int * port)
{
	unsigned int i,t = 0;
	int flag = 0, decCtr = 0;
	int tport1 = 0, tport2 = 0;
	char tmpPort[6];

	//example data in quotes below:
	//"227 Listening on (149,122,52,162,4,20)"
	//4 * 256 + 20 = 1044
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i]=='(') {
			flag = 1;
			i++;
		}

		if (buf[i]==')') {
			hostname[t]='\0';
			break;
		}

		if (flag==1) {
			if (buf[i] != ',') {
				hostname[t]=buf[i];
				t++;
			} else {
				hostname[t]='.';
				t++;
			}
		}
	}

	t = 0;
	for (i=0; i<strlen(hostname); i++) {
		if (hostname[i]=='.')
			decCtr++;

		if (decCtr==4 && hostname[i]!='.') {
			tmpPort[t]=hostname[i];
			t++;

			if (hostname[i+1]=='.') {
				tmpPort[t]='\0';
				tport1=atoi(tmpPort);
				t=0;
			}
		}

		if (decCtr==5 && hostname[i]!='.') {
			tmpPort[t]=hostname[i];
			t++;

			if (hostname[i+1]=='\0') {
				tmpPort[t]='\0';
				tport2 = atoi(tmpPort);
				t=0;
			}
		}
	}

	*port = tport1 * 256 + tport2;
	decCtr=0;

	for (i = 0;i < strlen(hostname);i++) {
		if (hostname[i]=='.') {
			decCtr++;
		}
		if (decCtr > 3)
			hostname[i]='\0';
	}

	ftp_dbg("host [%s,%d]\n", hostname, *port); //print response to the screen

	return 0;
}

static int ftp_transfer_file(int sd, char *filename)
{
	FILE *in;
	char buf[MSIZE];
	size_t len;
    int retval = 0, valopt = 0 ;
    socklen_t lon ;

	in = fopen(filename, "rb");
	if (in == NULL)
		return (-1);


	while ((len = fread(buf, sizeof(char), MSIZE, in)) != 0)
	{
        if(iftp->ftp_state != FTP_STATE_SENDING)
        {
            break ;
        }
        getsockopt(sd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
        if(valopt)
        {
            retval = -1 ;
            break ;
        }  
        else
        {   
		    if (send(sd, buf, len, MSG_NOSIGNAL) == -1)
            {
	 	 	    perror("send");
                retval = -1 ;
                break ;
            }
		}
        OSA_waitMsecs(1) ;
	}
	fclose(in);

	return retval;
}

static int createDataSock(char * host, int port)
{
	int sd, reuse = 1;

	struct sockaddr_in pin;
	struct ifreq interface ;
	struct hostent *hp;
    struct timeval tv ;
    struct linger stLinger ;

    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;

    stLinger.l_onoff = 1 ;
    stLinger.l_linger = 0 ;

	if ((hp = gethostbyname(host)) == 0) {
		perror("gethostbyname");
		return -1;
	}

	memset(&pin, 0, sizeof(pin));
	pin.sin_family = AF_INET;
	pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	pin.sin_port = htons(port);

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    strncpy(interface.ifr_ifrn.ifrn_name, "eth0", IFNAMSIZ);

    setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) ;

    setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) ;

    setsockopt (sd, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

    setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
    setsockopt (sd, IPPROTO_TCP, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
//    setsockopt (sd, SOL_SOCKET, SO_LINGER, &stLinger, sizeof(stLinger)) ;


	if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
		perror("connect");
		close(sd); //close the socket
		return -1;
	}

	return sd ;
}
static int ftpSendFile(int sd, char *buf)
{
	if (ftp_transfer_file(sd, buf) != 0) {
		perror("send");
		close(sd); //close the socket
		return -1;
	}

	close(sd); //close the socket

	return 0;
}

static int ftp_change_dir(int sd, char *dirname, int day_dir)
{
    char buf[MSIZE] ;
 
    if(day_dir > 0)
    {
        if(ftpNewCmd(sd, buf, "CDUP", "") != 0)
            return -1 ;

        if(ftpRecvResponse(sd, buf) != 0)
            return -1;

        if(ftpNewCmd(sd, buf, "CDUP", "") != 0)
            return -1 ;

        if(ftpRecvResponse(sd, buf) != 0)
            return -1;

		return 0 ;
    }

    if(ftpNewCmd(sd, buf, "CWD", dirname) != 0)
        return -1 ;

    if(ftpRecvResponse(sd, buf) != 0)
        return -1;

    if(strncmp(buf, "250", 3) != 0) // do not exist
    {
        if(ftpNewCmd(sd, buf, "MKD", dirname) != 0) 
            return -1 ;

        if(ftpRecvResponse(sd, buf) != 0)
            return -1 ;

        if(strncmp(buf, "257", 3) == 0) // create success
        {
            if(ftpNewCmd(sd, buf, "CWD", dirname) != 0)
                return -1 ;

            if(ftpRecvResponse(sd, buf) != 0)
                return -1 ;

            if(strncmp(buf, "250", 3) == 0) // chdir success
            {
                return 0 ; 
            }
        }        
    }
    else
        return 0 ;
    
    return -1 ;

}

static int ftp_send_file(int sd, char *filename)
{
    char tmpHost[100];
    int tmpPort, data_sock;
    char buf[MSIZE];

	if(ftpNewCmd(sd, buf, "TYPE", "I") != 0)
		return -1 ;
	if(ftpRecvResponse(sd, buf) != 0)
		return -1 ;
    if(strncmp(buf,"200",3) != 0)
		return -1 ;

	if(ftpNewCmd(sd, buf, "PASV", "") != 0)
        return -1 ;

	if(ftpRecvResponse(sd, buf) != 0)
        return -1 ;

	if (strncmp(buf, "227", 3) == 0) 
	{
		ftpConvertAddy(buf, tmpHost, &tmpPort);

        data_sock = createDataSock(tmpHost, tmpPort) ;

		if(data_sock > 0)
		{
/*
		    if(ftpNewCmd(sd,buf,"REST", 0) != 0)
				return -1 ;
	
			if(ftpRecvResponse(sd, buf) != 0)
                return -1 ;

            if(strncmp(buf, "350", 3) == 0) 
	            ftp_dbg("ftpRecvResponse REST success = %s\n", buf);
*/
		    if(ftpNewCmd(sd,buf,"STOR",&filename[strlen(iftp->path) + 1]) != 0)
                return -1 ;

		    if(ftpRecvResponse(sd, buf) != 0)
            {
                return -1 ;
            }

	        ftp_dbg("ftpRecvResponse After STORE = %s\n", buf);
		    if (strncmp(buf, "150", 3) == 0) 
			{ //make sure response is a 150
			    if (ftpSendFile(data_sock, filename) == 0) 
				{
				    if(ftpRecvResponse(sd, buf) == 0)
					{
				        if (strncmp(buf, "226", 3) == 0) 
					    {
					        printf("%s transfer completed.\n", &filename[strlen(iftp->path) + 1]);
					        return 0;
				        }
					}
					else
					{
						return -1 ;
					}
				}	
			}
		}
	}

	return -1;
}

static int ftp_login(int sd, char *username, char *password)
{
    char buf[MSIZE];

    if (ftpNewCmd(sd, buf, "USER", username) == 0) {  //issue the command to login
        if(ftpRecvResponse(sd,buf) == 0) {  //wait for response
            if(strncmp(buf,"331",3) == 0) {  //make sure response is a 331
                if(ftpNewCmd(sd,buf,"PASS",password) == 0) {  //send your password
                    if(ftpRecvResponse(sd,buf) == 0) { //wait for response
                        if(strncmp(buf,"230",3) == 0) { //make sure its a 230
                                        return 0;
                        }
                    }
                }
            }
        }
    }

    return -1;
}

static int ftp_connect (char *hostname, int port)
{
	char buf[MSIZE];
	int sd, res, valopt, reuse = 1;
	fd_set myset;
	struct timeval tv;
	socklen_t lon;

    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;

	struct ifreq interface ;
	struct sockaddr_in pin;
	struct hostent *hp;

	if ((hp = gethostbyname(hostname)) == 0) {
		perror ("gethostbyname");
		return -1;
	}

	memset (&pin, 0, sizeof(pin));
	pin.sin_family 		= AF_INET;
	pin.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
	pin.sin_port 		= htons(port);

	//#--- create socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

#if 0
	//#--- set non-blocking
	arg = fcntl(sd, F_GETFL, NULL);
	arg |= O_NONBLOCK;
	fcntl(sd, F_SETFL, arg);
#endif

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    strncpy(interface.ifr_ifrn.ifrn_name, "eth0", IFNAMSIZ);

    setsockopt (sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) ;
    setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) ;

    setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
    setsockopt (sd, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

    setsockopt (sd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
    setsockopt (sd, IPPROTO_TCP, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ; 

	//# trying to connect with timeout
	res = connect(sd, (struct sockaddr *)&pin, sizeof(pin));
	if(res < 0)
	{
/*		
		if (errno == EINPROGRESS)
		{
			tv.tv_sec = FTP_TIME_OUT;
	        tv.tv_usec = 0;
	        FD_ZERO(&myset);
	        FD_SET(sd, &myset);

	        if(select(sd+1, NULL, &myset, NULL, &tv) > 0)
			{
	           lon = sizeof(int);
	           getsockopt(sd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
	           if (valopt) {
	              ftp_dbg("Error in connection() %d - %s\n", valopt, strerror(valopt));
	           }
	        } else {
	        	ftp_dbg("Timeout or error() %d - %s\n", valopt, strerror(valopt));
        	}
		} else {
        	ftp_dbg("Error connecting %d - %s\n", errno, strerror(errno));
   	 	}
*/
		goto ftp_err;
	}

#if 0
	//# set to blocking mode again...
	arg = fcntl(sd, F_GETFL, NULL);
	arg &= (~O_NONBLOCK);
	fcntl(sd, F_SETFL, arg);
#endif

	if(ftpRecvResponse(sd, buf) == 0) {  //wait for ftp server to start talking
		if(strncmp(buf,"220",3) == 0) {  //make sure it ends with a 220
			return sd;
		}
	}

ftp_err:
	close(sd);
	return -1;
}

static int ftp_close(int sd)
{
	char buf[MSIZE];

	ftpNewCmd(sd, buf, "QUIT", "");
	ftpRecvResponse(sd, buf);

	if (strncmp(buf, "221", 3) == 0) {
		printf("FTP OK.\n");
	}

	close(sd); //close the socket

	iftp->sdFtp = -1 ;
	return 0;
}

static void ftp_send(void)
{
	int i=0;
	int ret = 0,ftp_cycle = 0, retry_cnt = 0;
    char FileName[MAX_CHAR_128] ;
    char temp[10] = {0, } ;

	iftp->ftp_state = FTP_STATE_CONNECTING;

	while(iftp->ftp_state == FTP_STATE_CONNECTING)
	{
		if(!app_cfg->ftp_enable)
		{
			iftp->ftp_state = FTP_STATE_NONE;
			ftp_dbg(" \n[ftp] FTP stop process -- FTP DISABLED!! \n");
			break;
		}

		//#--- ping check for ftp IP
		if(iftp->sdFtp >= 0)
			close(iftp->sdFtp);

        if(!app_cfg->ste.b.cradle_eth_run)  // eth0 off in ftp running
        	break;
 
		iftp->sdFtp = ftp_connect(app_set->ftp_info.ipaddr, app_set->ftp_info.port);
		if(iftp->sdFtp != -1)
		{
            retry_cnt = 0 ;
		 //read result
			while(1)
			{
		        ret = ftp_login(iftp->sdFtp, app_set->ftp_info.id, app_set->ftp_info.pwd);
	            if(ret == 0)
                {
			        iftp->ftp_state = FTP_STATE_SENDING;
                    retry_cnt = 0 ;
					break ;
                }
			    else
                {     
                    if(retry_cnt >= FTP_RETRY_CNT)
                    {
                        iftp->ftp_state = FTP_STATE_SEND_DONE ;
                        retry_cnt = 0 ;
					    break ;
                    }
                    else
                        retry_cnt += 1 ;

			        ftp_dbg("ftp login failure ID:%s, PWD:%s \n",app_set->ftp_info.id,app_set->ftp_info.pwd);
                    app_leds_eth_status_ctrl(LED_FTP_ERROR);
                }
			}
		}
		else 
        {
            if(retry_cnt >= FTP_RETRY_CNT)
            {
                iftp->ftp_state = FTP_STATE_SEND_DONE ;
                retry_cnt = 0 ;
				break ;
            }
            retry_cnt += 1 ;

		    ftp_dbg("ftp connection failure ip:%s port = %d\n", app_set->ftp_info.ipaddr, app_set->ftp_info.port);
            app_leds_eth_status_ctrl(LED_FTP_ERROR);
		}

		app_msleep(FTP_CYCLE_TIME);
	}

	if (iftp->ftp_state == FTP_STATE_SENDING)
	{
        app_leds_backup_state_ctrl(LED_FTP_ON);
        app_leds_eth_status_ctrl(LED_FTP_ON);

		iftp->file_cnt = get_recorded_file_count() ;

		for (i = 0; i < iftp->file_cnt; i++)
        {
            if(get_ftp_send_file(FileName) == 0)   // do not exist file
               continue ;

            if(!app_cfg->ste.b.cradle_eth_run)  // eth0 off in ftp running
                break;

            strncpy(temp, &FileName[12], 8) ;  // create folder per date ex) /deviceID/20190823/

		    if (app_cfg->ftp_enable && iftp->ftp_state == FTP_STATE_SENDING)
		    {
                if(ftp_change_dir(iftp->sdFtp, temp, 0) != 0)
                {
                    iftp->ftp_state = FTP_STATE_NONE ;
                }
				else
				{	
                    if(ftp_change_dir(iftp->sdFtp, app_set->sys_info.deviceId, 0) != 0)
                    {
                        iftp->ftp_state = FTP_STATE_NONE ;
                    }
					else
					{
                        if(ftp_send_file(iftp->sdFtp, FileName) == 0)
                        {
					        ftp_dbg(" \n[ftp] Send image -- %s \n", FileName);
                            delete_ftp_send_file(FileName) ;
			            }   
                        else
                        {
                            restore_ftp_file(FileName) ;
				            ftp_dbg(" \n[ftp] Send Fail image -- %s \n", FileName);
                            break;
						}

                        if(ftp_change_dir(iftp->sdFtp, temp, 1) != 0)
                        {
                            iftp->ftp_state = FTP_STATE_NONE ;
						}

					}	
                }
		    }
            else
            {
                break ;
            }

        } 

		iftp->file_cnt = get_recorded_file_count() ;
        if(iftp->file_cnt == 0)
        {
            app_leds_backup_state_ctrl(LED_FTP_OFF);
            app_leds_eth_status_ctrl(LED_FTP_OFF);
		    iftp->ftp_state = FTP_STATE_SEND_DONE;
            ftp_close(iftp->sdFtp);
        }
        else
        {
            app_leds_eth_status_ctrl(LED_FTP_ERROR);
            app_leds_backup_state_ctrl(LED_FTP_ERROR);
          
            iftp->ftp_state = FTP_STATE_NONE ;
        }   
	}

}

int app_get_ftp_state(void)
{
	return iftp->ftp_state;
}

void app_ftp_state_reset(void)
{
	iftp->ftp_state = FTP_STATE_NONE;
}

/*****************************************************************************
* @brief    ftp main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_ftp(void *prm)
{
	app_thr_obj *tObj = &iftp->ftpObj;
	int cmd, exit = 0 ;
	unsigned long ret = 0;

	aprintf("enter...\n");
	tObj->active = 1;

    iftp->ftp_state = FTP_STATE_NONE;
    iftp->retry_cnt = 0 ;

    app_cfg->ste.b.prerec_state = app_set->rec_info.auto_rec ;

	while (!exit)
	{
		//# wait cmd
        cmd = tObj->cmd ;

		if (cmd == APP_CMD_STOP) {
			break;
		}
		
        if(app_cfg->ste.b.cradle_eth_run)
        {
            if(iftp->ftp_state == FTP_STATE_NONE)
            {
			    iftp->file_cnt = get_recorded_file_count() ;

				if (app_rec_state())  // rec status
                {
                    app_rec_stop(1);
			     	app_cfg->ste.b.prerec_state = 1 ;
                }

                if (iftp->file_cnt > 0)  
                {
                    if(app_cfg->ftp_enable)
                    {
                        app_leds_eth_status_ctrl(LED_FTP_ON);

                        app_cfg->ste.b.ftp_run = 1 ;  // rec key disable

                        ftp_send() ;
                        app_cfg->ste.b.ftp_run = 0 ;
					}
                }
                else  
                {
                    app_cfg->ste.b.ftp_run = 0 ;
                    iftp->ftp_state = FTP_STATE_SEND_DONE ;
                }  
            }
            else
			{	
                app_cfg->ste.b.ftp_run = 0 ;
			}

			/* ftp connection 실패시 auto record 설정 On, 및 현재 record 상태 였으면 이전 상태로 돌리기 위한 작업 */
            if (app_cfg->ste.b.prerec_state && iftp->ftp_state == FTP_STATE_SEND_DONE)
			{      
                app_rec_start() ;  // rec start after ftp send
			    app_cfg->ste.b.prerec_state = 0 ;
            } 
			
        }
        else
        {
			/* cradle에서 분리 되었을 경우 처리*/
//			if(iftp->sdFtp < 0)
			{
                iftp->ftp_state = FTP_STATE_NONE ;
                app_cfg->ste.b.ftp_run = 0 ;
            } 

            if (app_cfg->ste.b.prerec_state && app_cfg->en_rec)
			{
                app_rec_start() ;  // rec start after ftp send
				app_cfg->ste.b.prerec_state = 0 ;
			}
        }
   
        OSA_waitMsecs(1000) ;
	}

    tObj->active = 0;
    aprintf("...exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    ftp thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_ftp_init(void)
{
	app_thr_obj *tObj;

	memset(iftp, 0, sizeof(app_img_ftp_t));

	sprintf(iftp->path , "%s/%s", SD_MOUNT_PATH, REC_DIR);

	if (access(iftp->path, 0) == -1) {
        mkdir(iftp->path, 0775);
        chmod(iftp->path, 0775);
    }

    if(app_set->ftp_info.ON_OFF)
    {
	//# create ftp thread
	    tObj = &iftp->ftpObj;
	    if (thread_create(tObj, THR_ftp, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		    eprintf("create ftp thread\n");
		    return EFAIL;
	    }
    }

	dprintf(".done!\n");

	return 0;
}

void app_ftp_exit(void)
{
    app_thr_obj *tObj;

	tObj = &iftp->ftpObj;
    app_ftp_state_reset() ;

    if(app_set->ftp_info.ON_OFF)
    {
        event_send(tObj, APP_CMD_STOP, 0, 0);
	    while(tObj->active)
		    app_msleep(20);

	    thread_delete(tObj);
    }

    dprintf(".. done!\n");
}
