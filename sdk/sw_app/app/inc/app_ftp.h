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
#define FTP_DBG

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
 AVI file format
-----------------------------------------------------------------------------*/
typedef int				BOOL;
typedef long			LONG;
typedef unsigned long 	DWORD;
typedef unsigned short 	WORD;
typedef unsigned char 	BYTE;

typedef DWORD	FOURCC;
typedef WORD	TWOCC;

#define mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |	\
	( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )

#define aviTWOCC(ch0, ch1) ((WORD)(BYTE)(ch0) | ((WORD)(BYTE)(ch1) << 8))

/* form types, list types, and chunk types */
#define formtypeRIFF					mmioFOURCC('R', 'I', 'F', 'F')
#define formtypeLIST					mmioFOURCC('L', 'I', 'S', 'T')
#define formtypeAVI             		mmioFOURCC('A', 'V', 'I', ' ')
#define listtypeAVIHEADER       		mmioFOURCC('h', 'd', 'r', 'l')
#define ckidAVIMAINHDR          		mmioFOURCC('a', 'v', 'i', 'h')
#define listtypeSTREAMHEADER			mmioFOURCC('s', 't', 'r', 'l')
#define ckidSTREAMHEADER        		mmioFOURCC('s', 't', 'r', 'h')
#define ckidSTREAMFORMAT        		mmioFOURCC('s', 't', 'r', 'f')

#define listtypeAVIMOVIE        		mmioFOURCC('m', 'o', 'v', 'i')
#define listtypeAVIRECORD       		mmioFOURCC('r', 'e', 'c', ' ')

#define ckidAVINEWINDEX         		mmioFOURCC('i', 'd', 'x', '1')

/* Basic chunk types */
#define cktypeDIBbits           		aviTWOCC('d', 'b')
#define cktypeDIBcompressed     		aviTWOCC('d', 'c')
#define cktypePALchange         		aviTWOCC('p', 'c')
#define cktypeWAVEbytes         		aviTWOCC('w', 'b')
#define cktypeTEXT						aviTWOCC('t', 'x')

//# avi index dwFlags
#define AVIIF_LIST          			0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME      			0x00000010L // this frame is a key frame.
#define AVIIF_NOTIME	    			0x00000100L // this frame doesn't take any time
#define AVIIF_COMPUSE       			0x0FFF0000L // these bits are for compressor use

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
} CK;

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckCodec;
} CK_RIFF;

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckType;
} CK_LIST;

typedef struct {
	unsigned long dwMicroSecPerFrame;
	unsigned long dwMaxBytesPerSec;
	unsigned long dwReserved1;
	unsigned long dwFlags;
	unsigned long dwTotalFrames;
	unsigned long dwInitialFrames;
	unsigned long dwStreams;
	unsigned long dwSuggestedBufferSize;
	unsigned long dwWidth;
	unsigned long dwHeight;
	unsigned long dwScale;
	unsigned long dwRate;
	unsigned long dwStart;
	unsigned long dwLength;
} AVIMainHeader;

//# Header Block
typedef struct {
	CK_RIFF		ck_riff;	// RIFF tag
	CK_LIST		ck_hdrl;	// LIST tag
	CK			ck_avih;	// AVI header
	AVIMainHeader avih;
} AVIHeadBlock;

typedef struct {
	CK_LIST	ck_movi;
	CK		ck_idx1;
} AVITailBlock;

typedef struct {
	DWORD       ckid;
	DWORD       dwFlags;
	DWORD       dwChunkOffset;      // Position of chunk
	DWORD       dwChunkLength;      // Length of chunk
} AVIINDEXENTRY;

//# AVI File Information
typedef struct {
	AVIHeadBlock Head;
	AVITailBlock Tail;

	unsigned long movi_pos;
	unsigned long idx1_pos;
	unsigned long strm_pos;

	AVIINDEXENTRY *idx_entry;
	unsigned long idx_cnt;

	FILE *pFile;
} AVIFile;



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

/*
#define CLIENT_CERTF  "/mmc/server.crt"
#define CLIENT_KEYF   "/mmc/server.crt"
#define CLIENT_CA_CERTF  "/mmc/server.crt"
*/

#define CLIENT_CERTF "/mmc/log/my_pub_key.pem"
#define CLIENT_KEYF "/mmc/log/my_priv_key.pem"
#define CLIENT_CA_CERTF "/mmc/log/my_cert.pem"


/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_ftp_init(void);
void app_ftp_exit(void);

int app_get_ftp_state(void);
void app_ftp_state_reset(void);

int avi_decrypt_fs(char *src, char *dst) ;

#endif	/* _APP_FTP_H_ */
