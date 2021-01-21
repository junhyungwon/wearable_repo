/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_gmem.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_GMEM_H_
#define _APP_GMEM_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define G_MEM_SIZE			40*1024*1024
#define G_MEM_INFO_SIZE		0x0100000
#define G_MEM_SYNC			0xAAAAAAAA

#define MAX_STREAM_NUM		5000

typedef struct {
	int d_type;		//# video/audio/meta
	char *addr;		//# stream address
	int offset;		//# base offset
	int b_size;		//# stream size

	//# video info
	int ch;
	int is_key;		//# I-frame
	int frm_wi;
	int frm_he;
	int frm_rate;

	//# time info
	int t_sec;
	int t_msec;

	int end;
} stream_info_t;

typedef struct {
	//# for process record
	int sync;
	int process;

	int cmd;
	int param0;
	int param1;

	int max_num;
	int cur_num;
	int read_num;
	int num_ch;

	stream_info_t ifr[MAX_STREAM_NUM];
} g_mem_info_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int g_mem_init(void);
void g_mem_exit(void);
int g_mem_reset(void);

void *g_mem_get_addr(int size, int *idx);
int g_mem_get_virtaddr(void);
int g_mem_get_phyaddr(void);
int g_mem_get_size(void);

#endif	/* _APP_GMEM_H_ */
