/****************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_fms.h
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_FMS_H_
#define _APP_FMS_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

#include <osa.h>
#include <osa_mutex.h>
#include <osa_thr.h>
#include <osa_pipe.h>
#include <osa_sem.h>
#include <osa_que.h>
#include <osa_buf.h>
#include "app_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define CMD_METADATA_SND        0x1001
#define CMD_METADATA_RES        0x1002
#define CMD_EVENT_JPEG_SND      0x1011
#define CMD_EVENT_JPEG_RES      0x1012

#define PACKETSIZE              4096

#define SOCKWAIT                10
#define SOCK_THRFXN_TSK_PRI     (3)

#define MAXBUFFSIZE             (1920*1080)/4
#define QUE_BUF_SIZE            (1920*1080)/4
#define MAX_QUE_BUF             4
#define MAX_HEADER_SIZE         300

#define MAX_PENDING_SEM_COUNT   (10)
#define SOCK_ERROR              0

#define HALF                    2
#define TRUE                    1
#define FALSE                   0
#define ERRNO                   -1

#define META_ALL                2
#define META_HALF               1

#define AUTH_SIZE               MAX_CHAR_64

//#define MAXUSER                 5

#define DIR_JPEG_NAND          "/media/nand/jpeg"
#define DIR_META_NAND          "/media/nand/meta"
#define EVT_REC_MMC            "/mmc/data/event/"
#define MAN_REC_MMC            "/mmc/data/manual/"

#define METASET_PATH           "/etc/network/cfg-network"
#define DDNSSET_PATH           "/etc/network/cfg-ddns"

#define HTTP_FNAME_SIZE             64
#define HTTP_RAWMETA_SIZE           512
#define HTTP_NORMAL_REQ             2048
#define HTTP_MNAME_SIZE             128

#define CLIENT_CERTF  "server.crt"
#define CLIENT_KEYF   "server.key"
#define CLIENT_CA_CERTF  "server.crt"

#define  JPEG_NAME    "E-"
#define  META_NAME    "N-"
#define  E_META_NAME  "E-"
#define  AVI_NAME     "E-"

#define SENDING_CYCLE  100
#define READY          2

/*----------------------------------------------------------------------------
 Defines referenced	STRUCTURE
-----------------------------------------------------------------------------*/
typedef struct TAG_META_STR
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned long length ;
    unsigned short frag_length ;
    unsigned short ch ;
    unsigned short fragmentNo ;
    unsigned short fragmentIdx ;
} META_STR;

typedef struct TAG_JPEG_STR
{
    unsigned short identifier ;
    unsigned short cmd ;
    unsigned long length ;
    unsigned short frag_length ;
    unsigned short ch ;
    unsigned short fragmentNo ;
    unsigned short fragmentIdx ;
} JPEG_STR ;

typedef struct {
        int ch ;
        int size ;
        void *buf;
        int type ;  // normal, event
} frame_info_t;

typedef struct {
    int socket;
} connection;

typedef struct {
    OSA_QueHndl bufQFullBufs;
    OSA_QueHndl bufQFreeBufs;
    frame_info_t frm[MAX_QUE_BUF];
} que_mbx_t;

typedef struct TAG_HTTP_RES{
    char SESSIONID[64] ;
    char EVENTNAME[64] ;
    int Metatype ;
    int SESSIONFLAG ;
    int ResMeta ;
    int ResJpeg ;
} HTTP_RES ;

enum Req_Method { GET, HEAD, UNSUPPORTED };
enum Req_Type   { SIMPLE, FULL };

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_fms_init(void);
void app_fms_exit(void);
void app_event_set(void) ;
void Meta_send( char *data, int data_size, int evt);

#endif	/* _APP_FMS_H_ */
