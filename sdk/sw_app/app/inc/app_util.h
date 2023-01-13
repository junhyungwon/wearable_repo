/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_util.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_UTIL_H_
#define _APP_UTIL_H_

#include "dev_disk.h"

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#define RFS_NFS                 1

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
    int file_num;
    unsigned long long total_size;
} dinfo_t;

typedef struct {
        unsigned long total;
        unsigned long avail;
        unsigned long used;
} disk_info_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#if 0
#define app_malloc	util_mem_alloc
#define app_mfree	util_mem_free
#define app_memcpy	util_mem_copy
#else
#define app_malloc	malloc
#define app_mfree	free
#define app_memcpy	memcpy
#endif

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void *util_mem_alloc(unsigned int msize);
void util_mem_free(void *ptr);
int util_mem_copy(void *pDes, void *pSrc, int size);
int util_disk_info(disk_info_t *idisk, const char *path);
int util_sys_exec(char *arg);
int menu_get_data(void);
char menu_get_cmd(void);

void __time_trace(int param);
unsigned int util_gen_crc32(unsigned int crc, const void *buf, unsigned int size);
void util_hexdump(char *p, int n);
int hextobin(unsigned char c);
int utf8_unescape(const char *dst, char *src);

int uinput_init(void);
int uinput_emit_key(int type, int code, int val);
int uinput_exit(void);

int Msg_Init(int msgKey);
int Msg_Kill(int qid);
int Msg_Send(int qid, void *pdata, int size);
int Msg_Rsv(int qid, int msg_type, void *pdata , int size);
int Msg_Send_Rsv(int qid, int msg_type, void *pdata , int size);

void urandom_value(char *outdata, int count);   // read /dev/urandom

#endif	/* _APP_UTIL_H_ */
