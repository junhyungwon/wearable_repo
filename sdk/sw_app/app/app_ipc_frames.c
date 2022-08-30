/******************************************************************************
 * DM8147 P2 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	p2_ipc_frames.c
 * @brief
 * @author	phoong
 * @section	MODIFY history
 *     - 2013.06.20	: First	Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <osa.h>
#include <osa_que.h>
#include <osa_mutex.h>
#include <osa_thr.h>
#include <osa_sem.h>

#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/xdm.h>
#include <ti/xdais/dm/ivideo.h>
#include <ti_venc.h>
#include <ti_vdec.h>

#include <mcfw/src_linux/mcfw_api/ti_vsys_priv.h>

#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_ipc_frames.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define __DATA_WRITE__
#define EN_FILE_WRITE			0

#define MAX_IPCFRAMES_BUF		20
#define MAX_PENDING_SEM_COUNT   (10)

#define MMAP_MEM_PAGEALIGN      (4*1024-1)
typedef struct
{
  unsigned int mmap_addr;
  unsigned int mmap_size;

  int    mem_fd;
  volatile unsigned int *mem_virtAddr;
} mmap_ctrl_t;

typedef struct {
	OSA_ThrHndl thrHandleBitsIn;
	OSA_SemHndl bitsInNotifySem;
	volatile Bool exitBitsInThread;
	volatile Bool exitBitsInThreadDone;

	OSA_ThrHndl thrHandleRecord;
	OSA_SemHndl recordNotifySem;
	volatile Bool exitRecordThread;
	volatile Bool exitRecordThreadDone;

	OSA_QueHndl bufQFullBufs;
    OSA_QueHndl bufQFreeBufs;

} IpcFramesCtrlThrdObj;

typedef struct {
	IpcFramesCtrlThrdObj tObj;

	void *que_buf[MAX_IPCFRAMES_BUF];
} IpcFramesCtrl;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
IpcFramesCtrl	gIpcFramesCtrl;
mmap_ctrl_t gMmap;

//#if EN_FILE_WRITE
static int skipcnt = 0;
//#endif

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static FILE *fout=NULL;
static void file_open(void)
{
	char filename[128];

	sprintf(filename, "/mmc/rec_data.y");

	fout = fopen(filename, "wb");
	if(fout == NULL)
		dprintf("file open!\n");
}

static unsigned int vcap_mmap(unsigned int physAddr, unsigned int size)
{
	int mem_offset;

	gMmap.mem_fd = open("/dev/mem",O_RDWR|O_SYNC);
    if(gMmap.mem_fd < 0) {
		dprintf("/dev/mem open failed!\n");
		return -1;
    }

	mem_offset   = physAddr & MMAP_MEM_PAGEALIGN;
	gMmap.mmap_addr = physAddr - mem_offset;
	gMmap.mmap_size = size + mem_offset;

	gMmap.mem_virtAddr = mmap(
			(void *)gMmap.mmap_addr, gMmap.mmap_size,
			PROT_READ|PROT_WRITE|PROT_EXEC,MAP_SHARED,
			gMmap.mem_fd, gMmap.mmap_addr);
	if (gMmap.mem_virtAddr==NULL) {
		dprintf("mmap() failed!\n");
		return -1;
	}

	return (UInt32)((UInt32)gMmap.mem_virtAddr + mem_offset);
}

static void vcap_unmap(void)
{
	if(gMmap.mem_virtAddr)
		munmap((void*)gMmap.mem_virtAddr, gMmap.mmap_size);

	if(gMmap.mem_fd >= 0)
		close(gMmap.mem_fd);
}

/*----------------------------------------------------------------------------
 ipcFrames process function
-----------------------------------------------------------------------------*/
#ifdef __DATA_WRITE__
void *dataProcessFxn(void *prm)
{
	IpcFramesCtrlThrdObj *tObj = (IpcFramesCtrlThrdObj *)prm;
	frame_info_t *frame_que;
	int status;

	dprintf("--- %s\n", __func__);

	#if EN_FILE_WRITE
	file_open();
	#endif

	while (FALSE == tObj->exitRecordThread)
	{
		//# wait notify
		//OSA_semWait(&tObj->recordNotifySem, OSA_TIMEOUT_FOREVER);

		status = OSA_queGet(&tObj->bufQFullBufs, (Int32 *)&frame_que, OSA_TIMEOUT_NONE);
		if(status != OSA_EFAIL)
		{
			skipcnt++;
			#if EN_FILE_WRITE
			if(fout != NULL)
				if(!(skipcnt%5)) {
					fwrite(&frame_que->buf, 1, frame_que->size, fout);
				}
			#else
			if(!(skipcnt%(15*20)))
				printf("%s: addr 0x%p(%dx%d)\n", __func__, &frame_que->buf, frame_que->width, frame_que->height);
			#endif

			//dprintf("frame_que 0x%x\n", frame_que);
			OSA_quePut(&tObj->bufQFreeBufs, (Int32)frame_que, OSA_TIMEOUT_NONE);
		}
		else
		{
			//OSA_waitMsecs(30);
			OSA_waitMsecs(50);
		}
	}
	
	#if EN_FILE_WRITE
	if(fout != NULL)
		fclose(fout);
	#endif
	
	tObj->exitRecordThreadDone = TRUE;
	dprintf("--- %s done!\n", __func__);

	return NULL;
}
#endif

/*----------------------------------------------------------------------------
 ipcFrames process buffer function
-----------------------------------------------------------------------------*/
static void ipcFramesProcessFullBufs(IpcFramesCtrlThrdObj *tObj)
{
	VIDEO_FRAMEBUF_LIST_S bufList;
	VIDEO_FRAMEBUF_S *pBuf;
	frame_info_t *frame_que;
	int i, status;
	int virtAddr, frameSize;

	Vcap_getFullVideoFrames(&bufList, 0);
	
	dprintf("IPC Frames get %d\n", bufList.numFrames);
			
	for (i = 0; i < bufList.numFrames; i++)
	{
		pBuf = &bufList.frames[i];

		frameSize = (pBuf->frameWidth * pBuf->frameHeight)*2; //# YUV4:2:2
		virtAddr = vcap_mmap((unsigned int)(pBuf->phyAddr[0][0]), frameSize);
		if(virtAddr < 0)
			continue;

		#if 0
		dprintf("CH%d (%dx%d) 0x%x(0x%x), %d\n", pBuf->channelNum,
			pBuf->frameWidth, pBuf->frameHeight, virtAddr, pBuf->phyAddr[0][0], frameSize);
		#endif

		#if 1
		status = OSA_queGet(&tObj->bufQFreeBufs, (Int32 *)&frame_que, OSA_TIMEOUT_NONE);
		if(status != OSA_EFAIL)
		{
			frame_que->size = frameSize;
			frame_que->width = pBuf->frameWidth;
			frame_que->height = pBuf->frameHeight;
			memcpy((void *)&frame_que->buf, (void *)virtAddr, frameSize);

			OSA_quePut(&tObj->bufQFullBufs, (Int32)frame_que, OSA_TIMEOUT_NONE);
			//OSA_semSignal(&tObj->recordNotifySem);
		}
		#endif

		vcap_unmap();
	}

	Vcap_putEmptyVideoFrames(&bufList);
}

/*----------------------------------------------------------------------------
 ipcBits receive function
-----------------------------------------------------------------------------*/
static void *ipcFramesRecvFxn(void *prm)
{
	IpcFramesCtrl *ctrl = (IpcFramesCtrl *)prm;
	IpcFramesCtrlThrdObj *tObj = &ctrl->tObj;

	dprintf("--- %s\n", __func__);

	while (FALSE == tObj->exitBitsInThread)
	{
		//# wait notify
		OSA_semWait(&tObj->bitsInNotifySem, OSA_TIMEOUT_FOREVER);

		ipcFramesProcessFullBufs(tObj);
	}

	tObj->exitBitsInThreadDone = TRUE;
	dprintf("--- %s done!\n", __func__);

	return NULL;
}

/*----------------------------------------------------------------------------
 ipcBits thread init/deinit function
-----------------------------------------------------------------------------*/
static void ipcFramesInitThrdObj(IpcFramesCtrlThrdObj *tObj)
{
	int i;

	OSA_queCreate(&tObj->bufQFreeBufs, MAX_IPCFRAMES_BUF);
    OSA_queCreate(&tObj->bufQFullBufs, MAX_IPCFRAMES_BUF);

    for (i = 0; i < MAX_IPCFRAMES_BUF;i++)
    {
        gIpcFramesCtrl.que_buf[i] = malloc(sizeof(frame_info_t));
        if(gIpcFramesCtrl.que_buf[i] == NULL)
        	dprintf("ipcframes buffer alloc\n");
		
		#if 0
        dprintf("que_buf 0x%x\n", (int)gIpcFramesCtrl.que_buf[i]);
		#endif
			
		OSA_quePut(&tObj->bufQFreeBufs, (Int32)gIpcFramesCtrl.que_buf[i], OSA_TIMEOUT_NONE);
    }

	OSA_semCreate(&tObj->bitsInNotifySem, MAX_PENDING_SEM_COUNT, 0);
	tObj->exitBitsInThread = FALSE;
	tObj->exitBitsInThreadDone = FALSE;

	OSA_thrCreate(&tObj->thrHandleBitsIn,
			ipcFramesRecvFxn, APP_THREAD_PRI, FXN_TSK_STACK_SIZE, &gIpcFramesCtrl, NULL);

#ifdef __DATA_WRITE__
	OSA_semCreate(&tObj->recordNotifySem, MAX_PENDING_SEM_COUNT, 0);
	tObj->exitRecordThread = FALSE;
	tObj->exitRecordThreadDone = FALSE;

	OSA_thrCreate(&tObj->thrHandleRecord,
			dataProcessFxn, APP_THREAD_PRI, FXN_TSK_STACK_SIZE, &gIpcFramesCtrl.tObj, NULL);
#endif			
}

static void ipcFramesDeInitThrObj(IpcFramesCtrlThrdObj *tObj)
{
	int i;

#ifdef __DATA_WRITE__
	OSA_thrDelete(&tObj->thrHandleRecord);
	OSA_semDelete(&tObj->recordNotifySem);
#endif

	OSA_thrDelete(&tObj->thrHandleBitsIn);
	OSA_semDelete(&tObj->bitsInNotifySem);

	OSA_queDelete(&tObj->bufQFreeBufs);
	OSA_queDelete(&tObj->bufQFullBufs);

	for (i = 0; i < MAX_IPCFRAMES_BUF;i++)
    {
    	if(gIpcFramesCtrl.que_buf[i] != NULL)
        	free(gIpcFramesCtrl.que_buf[i]);
    }
}

/*****************************************************************************
* @brief	ipcFramesInCbFxn function
* @section	DESC Description
*	- desc : ipcBits callback function
*****************************************************************************/
void ipcFramesInCbFxn(Ptr cbCtx)
{
	IpcFramesCtrl *ctrl;

	OSA_assert(cbCtx = &gIpcFramesCtrl);

	ctrl = cbCtx;
	OSA_semSignal(&ctrl->tObj.bitsInNotifySem);
}

/*****************************************************************************
* @brief	ipcFramesInit function
* @section	DESC Description
*	- desc
*****************************************************************************/
Int32 ipcFramesInit(void)
{
	VCAP_CALLBACK_S vcapCallback;

    vcapCallback.newDataAvailableCb =  ipcFramesInCbFxn;
	Vcap_registerCallback(&vcapCallback, &gIpcFramesCtrl);

	ipcFramesInitThrdObj(&gIpcFramesCtrl.tObj);

	dprintf("--- %s\n", __func__);

	return 0;
}

/*****************************************************************************
* @brief	ipcFramesStop and Exit function
* @section	DESC Description
*	- desc
*****************************************************************************/
void ipcFramesStop(void)
{
#ifdef __DATA_WRITE__	
	gIpcFramesCtrl.tObj.exitRecordThread = TRUE;
	OSA_semSignal(&gIpcFramesCtrl.tObj.recordNotifySem);
	while(gIpcFramesCtrl.tObj.exitRecordThreadDone == FALSE) {
		OSA_waitMsecs(MCFW_MAIN_WAIT_TIME);
	}
#endif	

	gIpcFramesCtrl.tObj.exitBitsInThread = TRUE;
	OSA_semSignal(&gIpcFramesCtrl.tObj.bitsInNotifySem);
	while(gIpcFramesCtrl.tObj.exitBitsInThreadDone == FALSE){
		OSA_waitMsecs(MCFW_MAIN_WAIT_TIME);
	}

	dprintf("--- %s done!\n", __func__);
}

void ipcFramesExit(void)
{
	ipcFramesDeInitThrObj(&gIpcFramesCtrl.tObj);

	dprintf("--- %s done!\n", __func__);
}
