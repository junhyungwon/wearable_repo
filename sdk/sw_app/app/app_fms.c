/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_fms.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *    - 2018.05.28 : First Created
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
#include "dev_disk.h"

#include "app_fms.h"
#include "app_dev.h"

#include "app_set.h"
#include "app_queue.h"
#include "app_comm.h"

typedef struct {
    app_thr_obj sockObj ;
    app_thr_obj metaObj ;
    app_thr_obj jpegObj ;
    int sockfd ;
    OSA_MutexHndl       mutex;
} app_fms_t ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_fms_t t_fms;
static app_fms_t *ifms=&t_fms;

char ReadBuffer[RECVBUFFSIZE] ;

que_mbx_t *meta_que ;
que_mbx_t *jpeg_que ;

META_STR sMeta_data ;
JPEG_STR sJpeg_data ;

Node *Jpeg_Popped ;
LinkedQueue *Jpeg_Queue ;

HTTP_RES Http_res ;
#define TCP_SESSION
#define N_META 1
#define E_META 2

void Init_Var(void)
{
    memset(ReadBuffer, 0, RECVBUFFSIZE) ;
/*
    if(-1 == access(DIR_JPEG_NAND, 0))
    {
        mkdir(DIR_JPEG_NAND, 0775);
        chmod(DIR_JPEG_NAND, 0775);
    }

    if(-1 == access(DIR_META_NAND, 0))
    {
        mkdir(DIR_META_NAND, 0775);
        chmod(DIR_META_NAND, 0775);
    }
*/
    memset(Http_res.SESSIONID, 0, 64) ;
    memset(Http_res.EVENTNAME, 0, 64) ;
    Http_res.Metatype = FALSE ;
    Http_res.SESSIONFLAG = FALSE ;
    Http_res.ResMeta = FALSE ;
    Http_res.ResJpeg = FALSE ;

}

static int que_create(void)
{
    int i;

    LQ_CreateQueue(&Jpeg_Queue);

    meta_que = malloc(sizeof(que_mbx_t));
    if(meta_que == NULL) {
        return -1;
    }
    OSA_queCreate(&meta_que->bufQFreeBufs, MAX_QUE_BUF);
    OSA_queCreate(&meta_que->bufQFullBufs, MAX_QUE_BUF);

    for(i=0; i<MAX_QUE_BUF; i++)
    {
        meta_que->frm[i].buf = malloc(META_REC_TOTAL);
        if(meta_que->frm[i].buf == NULL) {
            return -1;
        }
        OSA_quePut(&meta_que->bufQFreeBufs, (int)&meta_que->frm[i], OSA_TIMEOUT_NONE);
    }

#if 0

    jpeg_que = malloc(sizeof(que_mbx_t));
    if(jpeg_que == NULL) {
        return -1;
    }

    OSA_queCreate(&jpeg_que->bufQFreeBufs, MAX_QUE_BUF);
    OSA_queCreate(&jpeg_que->bufQFullBufs, MAX_QUE_BUF);

    for(i=0; i<MAX_QUE_BUF; i++)
    {
        jpeg_que->frm[i].buf = malloc(QUE_BUF_SIZE/4);
        if(jpeg_que->frm[i].buf == NULL) {
            return -1;
        }
        OSA_quePut(&jpeg_que->bufQFreeBufs, (int)&jpeg_que->frm[i], OSA_TIMEOUT_NONE);
    }
#endif

    printf(" [app] %s done!\n", __func__);

    return 0;
}

static int que_delete(void)
{
    int i ;

    if(meta_que)
    {
        OSA_queDelete(&meta_que->bufQFreeBufs);
        OSA_queDelete(&meta_que->bufQFullBufs);

        for(i = 0; i < MAX_QUE_BUF; i++)
        {
            if(meta_que->frm[i].buf != NULL)
                free(meta_que->frm[i].buf);
        }
        if(meta_que)
            free(meta_que) ;
    }

#if 0
    if(jpeg_que)
    {
        OSA_queDelete(&jpeg_que->bufQFreeBufs);
        OSA_queDelete(&jpeg_que->bufQFullBufs);

        for(i = 0; i < MAX_QUE_BUF; i++)
        {
            if(jpeg_que->frm[i].buf != NULL)
                free(jpeg_que->frm[i].buf);
        }
        if(jpeg_que)
            free(jpeg_que) ;
    }
    LQ_DestroyQueue(&Jpeg_Queue);
#endif

    printf(" [app] %s done!\n", __func__);

    return 0 ;
}

int Sock_create()
{
//    FILE *fd = NULL, *fp = NULL ;

//    char pbuffer[AUTH_SIZE] ;
    struct sockaddr_in serv_addr ;
    struct hostent *host;

    struct timeval tv ;
    tv.tv_sec = 5;
    tv.tv_usec = 0 ;
    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;
    int sock = SOCK_ERROR ;
    int sendBufferSize = 65535 ;

    if(app_set->srv_info.port)
    {
        host = gethostbyname (app_set->srv_info.ipaddr);

        if(host == NULL)
        {
#ifdef FMS_DEBUG
            DEBUG_PRI("gethostbyname error = %s\n",hstrerror(h_errno)) ;
#endif
        }
        else
        {
#ifdef FMS_DEBUG
//            DEBUG_PRI("ipaddress = %s\n",inet_ntoa(*((struct in_addr *)host->h_addr_list[0]))) ;
#endif
            sock = socket(PF_INET, SOCK_STREAM, 0) ;
            if(sock != SOCK_ERROR)
            {

                setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) ;
                setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) ;
                setsockopt (sock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
                setsockopt (sock, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
                setsockopt (sock, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
                setsockopt (sock, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

                setsockopt (sock, SOL_SOCKET, SO_SNDBUF, &sendBufferSize, sizeof(sendBufferSize)) ;

                memset(&serv_addr, 0, sizeof(serv_addr)) ;
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_addr.s_addr = inet_addr(app_set->srv_info.ipaddr) ;
                serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
                serv_addr.sin_port = htons(app_set->srv_info.port) ;
                bzero(&(serv_addr.sin_zero), 8) ;

                if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
                {
//                    app_log_write(MSG_LOG_WRITE, "Manager Server_Connect ... FAIL. !!");
#ifdef FMS_DEBUG
//            DEBUG_PRI("fms tcp connect error!\n") ;
#endif
                    close(sock) ;
                    sock = SOCK_ERROR ;
                }
                else
                {
                    app_log_write(MSG_LOG_WRITE, "Manager Server_Connect ... SUCCESS. !!");
#ifdef FMS_DEBUG
//            DEBUG_PRI("fms TCP Connect Ok..\n") ;
#endif
                }
            }
            else
            {
#ifdef FMS_DEBUG
                DEBUG_PRI("socket open error\n") ;
#endif
//                app_log_write(MSG_LOG_WRITE, "Manager Socket open Error... !!");
                sock = SOCK_ERROR ;
            }
        }
    }
    return sock ;
}

void Set_MetaResponse(int value)
{
    OSA_mutexLock(&ifms->mutex);
    Http_res.ResMeta = TRUE ;
    if(value == E_META)
        Http_res.Metatype = META_HALF ;
    OSA_mutexUnlock(&ifms->mutex);
}

int Get_MetaResponse(void)
{
    int retval ;
    OSA_mutexLock(&ifms->mutex);
    retval = Http_res.ResMeta ;
    Http_res.ResMeta = FALSE ;
    OSA_mutexUnlock(&ifms->mutex);
    return retval ;
}

int Parse_HTTP_Header(char *buffer)
{
    char Res_buffer[2048] ;

#ifdef FMS_DEBUG
    DEBUG_PRI("HTTP_RESPONSE\n") ;
    DEBUG_PRI("%s\n",buffer) ;
#endif
    memcpy(Res_buffer, buffer, strlen(buffer)) ;

    if(strstr(Res_buffer, "200"))
    {
#ifdef FMS_DEBUG
    DEBUG_PRI("MetaE dat.. response OK\n") ;
#endif
        Set_MetaResponse(E_META) ;
    }

    if(strstr(Res_buffer,"-1") != NULL)
    {
#ifdef FMS_DEBUG
    DEBUG_PRI("response FAIL\n") ;
#endif
        Http_res.Metatype = FALSE ;
        Http_res.SESSIONFLAG = FALSE ;
        Http_res.ResMeta = FALSE ;
        Http_res.ResJpeg = FALSE ;

    }

/*
    char *pointer = NULL ;
    if(!(Http_res.SESSIONFLAG))
    {
        if(strstr(Res_buffer, "JSESSIONID"))
        {
            pointer = strtok(buffer,"=; ") ;
            for(;;)
            {
                if(pointer == NULL)
                    break ;
                else
                {
//                  printf("strtok : %s\n",pointer) ;
                    f(!strcmp(pointer,"JSESSIONID"))
                    {
                        pointer = strtok(NULL,"=; ") ;
                        strcpy(Http_res.SESSIONID, pointer) ;
#ifdef FMS_DEBUG
            DEBUG_PRI("SESSIONID = %s\n",Http_res.SESSIONID) ;
#endif
                        Http_res.SESSIONFLAG = TRUE ;
                    }

                    pointer = strtok(NULL,"=; ") ;
                }
            }
        }
    }

    if(strstr(Res_buffer,"max=11") != NULL)
    {
        app_set->srv_info.reconn = READY ;
    }
    if(strstr(Res_buffer,"1_IMAGE") != NULL)
    {
#ifdef FMS_DEBUG
        DEBUG_PRI("jpg.. response OK\n") ;
#endif
        Set_JpegResponse(TRUE) ;
        if(app_set->srv_info.reconn == READY)
            app_set->srv_info.reconn = TRUE ;
    }
    if(strstr(Res_buffer,"1_METAE") != NULL)
    {
#ifdef FMS_DEBUG
    DEBUG_PRI("MetaE dat.. response OK\n") ;
#endif
        Set_MetaResponse(E_META) ;
        if(app_set->srv_info.reconn == READY)
            app_set->srv_info.reconn = TRUE ;
    }
    if(strstr(Res_buffer,"1_METAN") != NULL)
    {
#ifdef FMS_DEBUG
    DEBUG_PRI("MetaN dat.. response OK\n") ;
#endif
        Set_MetaResponse(N_META) ;
        if(app_set->srv_info.reconn == READY)
            app_set->srv_info.reconn = TRUE ;
    }

    if(strstr(Res_buffer,"-1") != NULL)
    {
#ifdef FMS_DEBUG
    DEBUG_PRI("response FAIL\n") ;
#endif
        Http_res.Metatype = FALSE ;
        Http_res.SESSIONFLAG = FALSE ;
        Http_res.ResMeta = FALSE ;
        Http_res.ResJpeg = FALSE ;

    }
*/
    return 0;
}

/*****************************************************************************
* @brief    sock main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_sock(void *prm)
{
    app_thr_obj *tObj = &ifms->sockObj;
    int cmd, retry_cnt = 0, receive_len = 0, E_wouldcnt = 0 ;
    int exit = 0;

    aprintf("enter...\n");
    tObj->active = 1;

    ifms->sockfd = Sock_create() ;

    while (!exit)
    {
	//# wait cmd
        cmd = tObj->cmd;
		if (cmd == APP_CMD_STOP) {
	    	break;
		}

        if(ifms->sockfd == SOCK_ERROR)
        {
            app_msleep(20);
            retry_cnt += 1;
            if(retry_cnt > 250)
            {
                ifms->sockfd = Sock_create();
                retry_cnt = 0;
            }
        }
        else
        {
            receive_len = recv(ifms->sockfd, ReadBuffer, RECVBUFFSIZE,0);
            if(receive_len < FALSE)
            {
                switch(errno)
                {
                    case EWOULDBLOCK :
#ifdef FMS_DEBUG
//                  DEBUG_PRI("ReadSocketData EWOULDBLOCK..\n") ;
#endif
                    E_wouldcnt += 1 ;
                    if(E_wouldcnt == 25)
                    {
                        DEBUG_PRI("ReadSocketData EWOULDBLOCK.. E_wouldcnt = %d\n",E_wouldcnt) ;
                        E_wouldcnt = 0 ;
                        if(ifms->sockfd != SOCK_ERROR)
                            close(ifms->sockfd) ;
                    }
                    break ;

                    case EPIPE :
#ifdef FMS_DEBUG
                    DEBUG_PRI("ReadSocketData EPIPE..\n") ;
#endif
                    close(ifms->sockfd) ;
                    break ;

                    case ENOTSOCK :
#ifdef FMS_DEBUG
                    DEBUG_PRI("ReadSocketData ENOTSOCK\n") ;
#endif
                    close(ifms->sockfd) ;
                    break ;

                    case ECONNRESET :
#ifdef FMS_DEBUG
                    DEBUG_PRI("ReadSocketData ECONNRESET\n") ;
#endif
                    close(ifms->sockfd) ;
                    break ;

                    case 110 :
#ifdef FMS_DEBUG
                    DEBUG_PRI("ReadSocketData CONNECTION TIMEOUT\n") ;
#endif
                    app_log_write(MSG_LOG_WRITE, "Tcp Connect Timeout... !! on RECEIVER");

                    close(ifms->sockfd) ;
                    break ;


                    default :
#ifdef FMS_DEBUG
                    DEBUG_PRI("ReadSocketData errno = %d\n",errno) ;
#endif
                    close(ifms->sockfd) ;
                    ifms->sockfd = SOCK_ERROR ;
                    break ;
                }
            }
            else if(receive_len != 0)
            {
                E_wouldcnt = 0 ;
                Parse_HTTP_Header(ReadBuffer) ;
            }
            else if(receive_len == 0)
            {
#ifdef FMS_DEBUG
                DEBUG_PRI("receive len = %d....................\n",receive_len) ;
#endif
                close(ifms->sockfd) ;
                ifms->sockfd = SOCK_ERROR ;

            }
            app_msleep(30) ;
            receive_len = 0 ;
        }
    }

    if(ifms->sockfd != SOCK_ERROR)
        close(ifms->sockfd) ;

    tObj->active = 0;

    aprintf("...exit\n");

    return NULL;
}

void Meta_send( char *data, int data_size, int evt)
{
    int ret = OSA_EFAIL ;

    frame_info_t *ifrm ;

    if(app_cfg->ste.b.sock)
    {
        ret = OSA_queGet(&meta_que->bufQFreeBufs, (int *)(&ifrm), OSA_TIMEOUT_NONE);
        if(ret == OSA_SOK)
        {
            memcpy(ifrm->buf, data, data_size) ;
            ifrm->size = data_size ;
            ifrm->type = evt ;

            OSA_quePut(&meta_que->bufQFullBufs, (int)ifrm, OSA_TIMEOUT_NONE);
//            OSA_semSignal(&b_obj->gMetaSockObj.sem);
        }
    }
}

void sock_error_proc(int Err)
{
    switch(Err)
    {
            case EWOULDBLOCK :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData EWOULDBLOCK..\n") ;
#endif
            close(ifms->sockfd) ;
            break ;

            case EPIPE :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData EPIPE..\n") ;
#endif
            close(ifms->sockfd) ;
            break ;

            case ENOTSOCK :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData ENOTSOCK \n") ;
#endif
            close(ifms->sockfd) ;
            break ;

            case ECONNRESET :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData ECONNRESET \n") ;
#endif
            close(ifms->sockfd) ;
            break ;

            case 110 :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData CONNECTION TIMEOUT \n") ;
#endif
            close(ifms->sockfd) ;
            break ;


            default :
#ifdef FMS_DEBUG
            DEBUG_PRI("SendSocketData errno = %d\n",errno) ;
#endif
            close(ifms->sockfd) ;
            break ;
    }
}

int Post_Metaupload_req(char *buf, int size, int type)
{
    int i, sendlen = 0, total_len, Total_frag, Last_frag, Spacket_size;
    char Mauthval[HTTP_RAWMETA_SIZE] ;

#ifdef FMS_DEBUG
    DEBUG_PRI("Mata data type = %d size = %d\n", type, size) ;
#endif
    errno = 0 ;
    memset(Mauthval, 0, HTTP_RAWMETA_SIZE) ;

    sprintf(Mauthval,"POST http://%s:%d/v1/devices/:%s/events/ HTTP/1.1\r\n",app_set->srv_info.ipaddr, app_set->srv_info.port, app_set->sys_info.deviceId) ;
    sprintf(Mauthval,"%sHost: %s:%d\r\n",Mauthval, app_set->srv_info.ipaddr, app_set->srv_info.port) ;
    sprintf(Mauthval,"%sConnection: keep-alive\r\n",Mauthval) ;
    sprintf(Mauthval,"%sAccept-Encoding: gzip,deflate\r\n",Mauthval) ;
    sprintf(Mauthval,"%sAccept-Language: ko,en-us;q=0.8,en;q=0.6\r\n",Mauthval) ;
    sprintf(Mauthval,"%sUser-Agent: Fitt360\r\n",Mauthval) ;
    sprintf(Mauthval,"%sAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",Mauthval) ;
    sprintf(Mauthval,"%sContent-Type: application/json \r\n\r\n",Mauthval)  ;
    sprintf(Mauthval,"%sContent-Length: %d\r\n",Mauthval, strlen(Mauthval) + size) ;

    if(buf != NULL && HTTP_RAWMETA_SIZE > size)
        sprintf(Mauthval,"%s%s\r\n",Mauthval, buf) ;
    else
        return FALSE ;

    total_len = strlen(Mauthval) ;
    Total_frag = ((total_len)/PACKETSIZE) + 1 ;
    Last_frag = (total_len) % PACKETSIZE ;

    for(i = 0; i < Total_frag; i++)
    {
        if(i != Total_frag -1)
        {
            Spacket_size = PACKETSIZE ;
        }
        else
        {
            Spacket_size = Last_frag ;
        }

        if(ifms->sockfd != SOCK_ERROR)
            sendlen = send(ifms->sockfd, (void *)&Mauthval[PACKETSIZE*i], Spacket_size , 0) ;

        if(sendlen < 0)
        {
            sock_error_proc(errno) ;
            sendlen = FALSE ;
            break ;
        }
        app_msleep(10) ;
    }

    if(sendlen > 0)
    {
        if(type)
        {
//            sprintf(Http_res.EVENTNAME,"%s\n",Mname) ;
#ifdef FMS_DEBUG
            DEBUG_PRI("HTTP Event meta upload size = %d\n",size) ;
#endif
        }
        else
        {
#ifdef FMS_DEBUG
            DEBUG_PRI("HTTP Normal meta upload size = %d\n", size) ;
#endif
        }
        return TRUE ;
    }
    else
    {
        return FALSE ;
    }

}

int meta_http_send(char *rdata, int length, int type)
{
    int sendlen = 0;
    char data[length] ;

    memcpy(data, rdata, length) ;

    if(ifms->sockfd != SOCK_ERROR)
    {
/*
		char MetaName[HTTP_MNAME_SIZE], timemethod[20], timestr[22] ;
		char *pointer = NULL ;
		int len = 0;

        memcpy(timestr, &data[strlen(app_set->srv_info.deviceId)+1], 21) ;
        timestr[21] = '\0' ;

        pointer = strtok(timestr,"-:.") ;

        for(;;)
        {
            if(pointer == NULL)
                break ;
            else
            {
                strcpy(&timemethod[len], pointer) ;

                len = strlen(timemethod) ;

                pointer = strtok(NULL,"-:. ") ;
            }
        }
        if(type) // event
        {
            sprintf(MetaName, "%s%s-%s.dat",E_META_NAME, app_set->srv_info.deviceId, timemethod) ;
        }
        else
        {
            sprintf(MetaName, "%s%s-%s.dat",META_NAME, app_set->srv_info.deviceId, timemethod) ;
        }
*/
        sendlen = Post_Metaupload_req(data, length, type) ;

    }

    return sendlen ;
}

/*****************************************************************************
* @brief    meta main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_fms_meta(void *prm)
{
    app_thr_obj *tObj = &ifms->metaObj;
    int cmd;
    int exit = 0, ret = OSA_EFAIL,LMCNT = 0 , sendlen = 0 ;

    frame_info_t *ifrm ;

    aprintf("enter...\n");
    tObj->active = 1;

    while (!exit)
    {
        //# wait cmd
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP) {
            break;
        }
        ret = OSA_queGet(&meta_que->bufQFullBufs, (Int32 *)(&ifrm), OSA_TIMEOUT_NONE);

        if(ret == OSA_SOK)
        {
            if(ifms->sockfd != SOCK_ERROR)
            {
                sendlen = meta_http_send((char*)ifrm->buf, ifrm->size, ifrm->type) ;
                app_msleep(SENDING_CYCLE) ;

                if(sendlen > 0)
                {
                    while(!Get_MetaResponse())
                    {
                        app_msleep(50) ;
                        LMCNT++ ;
                        if(LMCNT >= 10)
                            break ;
                    }
                    if(LMCNT == 10)
                    {
                        if(ifms->sockfd != SOCK_ERROR)
                        {
                            LMCNT = 0 ;
                        }
                    }
                }
                if(ifms->sockfd)
                    close(ifms->sockfd) ;

            }
            OSA_quePut(&meta_que->bufQFreeBufs, (int)ifrm, OSA_TIMEOUT_NONE);
        }
        app_msleep(20) ;
    }

    tObj->active = 0;
    aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief    jpeg main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_jpeg(void *prm)
{
    app_thr_obj *tObj = &ifms->jpegObj;
    int cmd;
    int exit = 0;

    aprintf("enter...\n");
    tObj->active = 1;

    while (!exit)
    {
        //# wait cmd
        cmd = event_wait(tObj);
        if (cmd == APP_CMD_STOP) {
            break;
        }
    }

    tObj->active = 0;
    aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief    fms thread start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_fms_init(void)
{
    app_thr_obj *tObj ;

    if(app_cfg->ste.b.sock)
        return FALSE ;

    if(app_set->srv_info.ON_OFF)
    {
        Init_Var() ;
        que_create() ;
        memset(ifms, 0x0, sizeof(app_fms_t)) ;

        //# create meta thread ;
        tObj = &ifms->sockObj ;
        if(thread_create(tObj, THR_sock, APP_THREAD_PRI, NULL) < 0)
        {
            eprintf("create sock thread\n") ;
            return EFAIL ;
        }

        tObj = &ifms->metaObj ;
        if(thread_create(tObj, THR_fms_meta, APP_THREAD_PRI, NULL) < 0)
        {
            eprintf("create meta thread\n") ;
            return EFAIL ;
        }

        tObj = &ifms->jpegObj ;
        if(thread_create(tObj, THR_jpeg, APP_THREAD_PRI, NULL) < 0)
        {
            eprintf("create jpeg thread\n") ;
            return EFAIL ;
        }

        app_cfg->ste.b.sock = 1 ;
    }

    aprintf(".done!\n") ;
    return 0;
}

void app_fms_exit(void)
{
    app_thr_obj *tObj;

    if(!app_cfg->ste.b.sock)
        return ;

    if(app_set->srv_info.ON_OFF)
    {
        tObj = &ifms->metaObj;
        event_send(tObj, APP_CMD_STOP, 0, 0) ;
        while(tObj->active)
            app_msleep(20);

        thread_delete(tObj);

        tObj = &ifms->jpegObj;
        event_send(tObj, APP_CMD_STOP, 0, 0) ;
        while(tObj->active)
            app_msleep(20);

        thread_delete(tObj);

        tObj = &ifms->sockObj;
        event_send(tObj, APP_CMD_STOP, 0, 0) ;

        while(tObj->active)
            app_msleep(20);

        thread_delete(tObj);
        que_delete();

        app_cfg->ste.b.sock = 0 ;
    }
    aprintf(".done!\n") ;
}

void app_event_set()
{
    int  i;

    for(i=0; i< MAX_CH_NUM; i++)
    {
        app_cfg->evtjpg |= (ON<<i);
//        app_cfg->evt_send |= (ON<<i);
    }
    app_cfg->evt_type = TRUE ; // shock
}
