/******************************************************************************
 * XBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_ftp.h
 * @brief
 * @author	sksung
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_FTP_H_
#define _APP_FTP_H_

#include <app_main.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
//#define FTP_DBG

#define FTP_STATE_NONE		     	0
#define FTP_STATE_CONNECTING		1
#define FTP_STATE_SENDING			2
#define FTP_STATE_SEND_DONE			3

#define FOTA_STATE_NONE			 	0
#define FOTA_STATE_CONNECTING		1
#define FOTA_STATE_RECEIVE_INFO		2
#define FOTA_STATE_RECEIVE_FIRM		3
#define FOTA_STATE_RECEIVE_DONE		4

#define S_OK	1
#define S_ERR	0

#define CREATE malloc
#define KEY_NUMERIC		256
#define KEY_STRING		257
#define KEY_ENCLOSED	258
#define KEY_BOOLEAN		259
#define KEY_NEWLINE		('\n')
#define KEY_EOF			(EOF)
#define KEY_INVALID		260
#define MAXCFG_LENGTH	1024
#define TITLESIZE		64



/* Debugging */
#ifdef FTP_DBG
#define ftp_dbg(fmt, arg...)  fprintf(stdout, "\x1b[31m%s:%d: \x1b[0m " fmt, __FILE__, __LINE__, ## arg)
#else
#define ftp_dbg(fmt, arg...)  do { } while (0)
#endif
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj ftpObj;	//# key thread
	int lsdFtp;
	int dsdFtp;
	SSL *lsslHandle ;
	SSL *dsslHandle ;
    SSL_CTX *lsslContext ;
    SSL_CTX *dsslContext ;
	int img_save;
	int fota_state;
	int ftp_state;
	int file_cnt;
    int retry_cnt ;
	char fota_firmfname[MAX_CHAR_64] ;
    unsigned long  fota_filesize ;
	char path[MAX_CHAR_128];
} app_ftp_t;

//#define CLIENT_CERTF  "/mmc/linkflow.com.crt"
//#define CLIENT_KEYF   "/mmc/linkflow.com.key"
//#define CLIENT_CA_CERTF  "/mmc/linkflow.com.crt"

#define CLIENT_CERTF  "/mmc/server.crt"
#define CLIENT_KEYF   "/mmc/server.key"
#define CLIENT_CA_CERTF  "/mmc/server.crt"

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_ftp_init(void);
void app_ftp_exit(void);

int app_get_ftp_state(void);
void app_ftp_state_reset(void);
#endif	/* _APP_FTP_H_ */
