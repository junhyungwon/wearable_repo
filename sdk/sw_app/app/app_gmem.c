/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_gmem.c
 * @brief	global memory function
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <cmem.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MAX_BUF_OFFSET		500

typedef struct {
	int s_addr;		//# virtual addr
	int p_addr;		//# physical addr
	int c_addr;		//# data addr
	int size;

	int init;		//# initialize

	OSA_MutexHndl mutex;
} app_gmem_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_gmem_t gmem_obj={0,};
static app_gmem_t *igmem=&gmem_obj;

static g_mem_info_t	*imem;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
* @brief    gmem init/exit/reset function
* @section  [desc]
*****************************************************************************/
int g_mem_init(void)
{
    CMEM_AllocParams  prm;

    //# static config clear - when Variable declaration
	//memset((void *)igmem, 0x0, sizeof(app_gmem_t));

    prm.type      = CMEM_HEAP;
    prm.flags     = CMEM_CACHED;//CMEM_NONCACHED;
    prm.alignment = 32;

	if(CMEM_init() < 0){
		TRACE_INFO("CMEM init error\n");
		return EFAIL;
	}

	igmem->s_addr = (int)CMEM_alloc(G_MEM_SIZE, &prm);
	igmem->p_addr = (int)CMEM_getPhys((void *)igmem->s_addr);
	TRACE_INFO("addr: %p(phy: %p), size:%dMB\n", (void *)igmem->s_addr, (void *)igmem->p_addr, (G_MEM_SIZE/MB));

	imem = (g_mem_info_t *)igmem->s_addr;
	igmem->c_addr = igmem->s_addr+G_MEM_INFO_SIZE;

	imem->sync = G_MEM_SYNC;

	OSA_mutexCreate(&igmem->mutex);

	igmem->init = 1;

	return SOK;
}

void g_mem_exit(void)
{
	CMEM_AllocParams  prm;

    prm.type      = CMEM_HEAP;
    prm.flags     = 0;
    prm.alignment = 0;

    CMEM_free((void *)igmem->s_addr, &prm);

    CMEM_exit();

    OSA_mutexDelete(&igmem->mutex);
}

int g_mem_reset(void)
{
	imem->max_num = 0;
	imem->cur_num = -1;
	imem->read_num = 0;

	return (int)igmem->s_addr;
}

/*****************************************************************************
* @brief    gmem api function
* @section  [desc]
*****************************************************************************/
void *g_mem_get_addr(int size, int *idx)
{
	char *addr;

	if(!igmem->init) {
		return NULL;
	}

	OSA_mutexLock(&igmem->mutex);

	#if 0
	if(app_rec_state())		//# protect overflow
	{
		int wr_offset=0;
		if(imem->read_num > imem->cur_num)
			wr_offset = (imem->max_num - imem->read_num) + imem->cur_num;
		else
			wr_offset = imem->cur_num - imem->read_num;

		if(wr_offset > MAX_BUF_OFFSET) {
			TRACE_INFO("--- skip write!\n");
			OSA_mutexUnlock(&igmem->mutex);
			return NULL;
		}
	}
	#endif

	//# align 32bit
	size = (size + 0x1f) & (~0x1f);

	if( (((int)igmem->s_addr+G_MEM_SIZE-(int)igmem->c_addr) < size)
			|| (imem->cur_num > (MAX_STREAM_NUM-2)) ) {
		igmem->c_addr = igmem->s_addr+G_MEM_INFO_SIZE;
		imem->ifr[imem->cur_num].end = 1;
		imem->max_num = imem->cur_num+1;
		imem->cur_num = 0;
	} else {
		imem->cur_num++;
	}

	imem->ifr[imem->cur_num].end = 0;

	addr = (char *)igmem->c_addr;
	igmem->c_addr += size;

	#if 0
	if(imem->cur_num == 0) {
		TRACE_INFO("idx(%3d/%3d), addr(%p, phy:0x%x), size(%d)\n",
				imem->cur_num, imem->max_num, (int)addr, (int)CMEM_getPhys((void *)addr), size);
	}
	#endif

	*idx = imem->cur_num;

	OSA_mutexUnlock(&igmem->mutex);

	return addr;
}

int g_mem_get_virtaddr(void)
{
	return (int)igmem->s_addr;
}

int g_mem_get_phyaddr(void)
{
	return (int)igmem->p_addr;
}

int g_mem_get_size(void)
{
	return G_MEM_SIZE;
}
