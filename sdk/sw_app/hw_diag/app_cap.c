/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_cap.c
 * @brief	app video capture thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <mcfw/src_linux/mcfw_api/ti_vsys_priv.h>
#include "ti_vsys.h"
#include "ti_vcam.h"
#include "ti_vcap.h"
#include "ti_venc.h"
#include "ti_vdis.h"
#include "ti_mjpeg.h"

#include "app_comm.h"
#include "app_gmem.h"
#include "app_main.h"
#include "app_ctrl.h"
#include "app_cap.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define JPEG_DUMP  			1

typedef struct {
	app_thr_obj vObj;		//# video capture(ipcbits) thread

	g_mem_info_t *imem;
} app_cap_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_cap_t cap_obj;
static app_cap_t *icap=&cap_obj;

#if JPEG_DUMP
static FILE *jfp = NULL;
#endif

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
void video_status(void)
{
	int i, temp, count;
	
#if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)	
	int vstatus[1] = {0,};
	count = Vcap_get_video_status(1, &vstatus[0], &temp);
#else
	int vstatus[4] = {0,};
	count = Vcap_get_video_status(4, &vstatus[0], &temp);
#endif
	for (i = 0; i < count; i++)
    {
		dprintf("cam_%d : %s!\n", i, vstatus[i]?"video detect":"no video");
	}	
}

/*----------------------------------------------------------------------------
 m3 event handler function
-----------------------------------------------------------------------------*/
int vsys_event(unsigned int eventId, void *pPrm, void *appData)
{
	if (eventId == VSYS_EVENT_VIDEO_DETECT)
    {
        video_status();
    }

	return 0;
}

/*----------------------------------------------------------------------------
 ipcBits in callback function
-----------------------------------------------------------------------------*/
void ipc_bits_fxn(void)
{
	event_send(&icap->vObj, APP_CMD_NOTY, 0, 0);
}

/*----------------------------------------------------------------------------
 video capture[ipcBits] process function
-----------------------------------------------------------------------------*/
static void proc_vid_cap(void)
{
	VCODEC_BITSBUF_LIST_S fullBufList;
	VCODEC_BITSBUF_S *pFullBuf;
	stream_info_t *ifr;
	int i, idx;
	Uint64 captime;
	char *addr;
	
	Venc_getBitstreamBuffer(&fullBufList, 0);

	for (i=0; i<fullBufList.numBufs; i++)
	{
		pFullBuf = &fullBufList.bitsBuf[i];
		captime = (Uint64)((Uint64)pFullBuf->upperTimeStamp<<32|pFullBuf->lowerTimeStamp);

		if(pFullBuf->codecType == (VCODEC_TYPE_E)IVIDEO_H264HP || pFullBuf->codecType == 0)
		{	
			addr = g_mem_get_addr(pFullBuf->filledBufSize, &idx);
			if (addr == NULL) {
				continue;
			}
			
			ifr = &icap->imem->ifr[idx];
			ifr->d_type = 0; //# CAP_TYPE_VIDEO
			ifr->ch = pFullBuf->chnId;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = pFullBuf->filledBufSize;
			ifr->frm_wi = pFullBuf->frameWidth;
			ifr->frm_he = pFullBuf->frameHeight;
			#if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)
			ifr->frm_rate = 30;
			#else
			ifr->frm_rate = 15;
			#endif
			ifr->is_key = (pFullBuf->frameType == VCODEC_FRAME_TYPE_I_FRAME) ? 1:0;
			captime = (Uint64)((Uint64)pFullBuf->upperTimeStamp<<32|pFullBuf->lowerTimeStamp);
			ifr->t_sec = (Uint32)(captime/1000);
			ifr->t_msec = (Uint32)(captime%1000);

			app_memcpy(ifr->addr, (char*)pFullBuf->bufVirtAddr, ifr->b_size);
			
            if(1)
            {
                #if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)
				if (pFullBuf->codecType == (VCODEC_TYPE_E)IVIDEO_MJPEG || pFullBuf->codecType == 0 || (ifr->ch == 2))
				#else
				if (pFullBuf->codecType == (VCODEC_TYPE_E)IVIDEO_MJPEG || pFullBuf->codecType == 0 || (ifr->ch == 5))
				#endif
                {
					FILE *jpeg_f = NULL;
                    #if 0
					dprintf("Jpeg...... channel = %d pFullBuf->codecType = %d is_key = %d\n", 
								pFullBuf->chnId, pFullBuf->codecType, ((pFullBuf->frameType == VCODEC_FRAME_TYPE_I_FRAME) ? 1:0));          
					#endif
                    jpeg_f = fopen("/tmp/fitt360.jpeg","w") ;
                    if (jpeg_f != NULL)
                    { 
//                        lockf(fp, F_LOCK, 1L) ;
                        fwrite((void *)pFullBuf->bufVirtAddr, pFullBuf->filledBufSize, 1, jpeg_f);
                        fclose(jpeg_f);
                    } 
#if JPEG_DUMP
                    if (iapp->snapshot) {
						iapp->snapshot = 0;
						jfp = fopen("/mmc/DCIM/fitt360.jpeg","w") ;
						if (jfp != NULL) { 
							fwrite((void *)pFullBuf->bufVirtAddr, pFullBuf->filledBufSize, 1, jfp) ;
							fclose(jfp);
							sync();
							dprintf("jpeg snapshot done!!\n");
						}
					}
#endif
                } 

            }
        }
	}

	Venc_releaseBitstreamBuffer(&fullBufList);
}

/*****************************************************************************
* @brief    video capture thread function
* @section  [desc]
*****************************************************************************/
static void *THR_vid_cap(void *prm)
{
	app_thr_obj *tObj = &icap->vObj;
	int cmd, exit=0;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}

		//# process full buffers
		proc_vid_cap();
	}

	tObj->active = 0;
	aprintf("exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    video capture thread start/stop function
* @section  [desc]
*****************************************************************************/
int vid_cap_start(void)
{
	VENC_CALLBACK_S callback;

	icap->imem = (g_mem_info_t *)g_mem_reset();

	//# Register call back with encoder
	callback.newDataAvailableCb = (VENC_CALLBACK_NEW_DATA)ipc_bits_fxn;
	Venc_registerCallback(&callback, NULL);

	//#--- create thread
	if (thread_create(&icap->vObj, THR_vid_cap, APP_THREAD_PRI, NULL, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
    }

	return SOK;
}

static void vid_cap_stop(void)
{
	app_thr_obj *tObj;

	//#--- stop thread
	tObj = &icap->vObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active) {
		app_msleep(10);
	}
	thread_delete(tObj);
}

/*----------------------------------------------------------------------------
 encoder channel init function
-----------------------------------------------------------------------------*/
static int cap_enc_init(VENC_PARAMS_S *vencParams)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };
	int i, channels=0;
	int fr, br;
	
#if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)
	channels = 1+1; /* 1 camera + 1 stream */
	fr       = 30;
	br       = 1000; //# 1Mbps
#else
	channels = 4+1; /* 4 camera + 1 stream */
	fr       = 15;
	br       = 1000; //# 1Mbps
#endif
	for (i = 0; i < channels; i++)
	{
		params.frameRate 		= fr;
		params.inputFrameRate 	= fr;
		params.targetBitRate 	= br * 1000;

		vencParams->encChannelParams[i].enableAnalyticinfo = 0;
		Venc_params_set(vencParams, i, &params, VENC_ALL);
	}
	
	/* JPEG */
	params.frameRate 		= 1;
	params.inputFrameRate 	= fr;
	params.targetBitRate 	= (br * 1000)/fr;

	vencParams->encChannelParams[channels].enableAnalyticinfo = 0;
	Venc_params_set(vencParams, channels, &params, VENC_ALL);

	return SOK;
}

static void cap_jpeg_quality(int ch, int value)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

	params.qpMin 	= 1;
	params.qpMax 	= 100;
	params.qpInit	= value;

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_I);
	dprintf("jpeg ch %d, value %d\n", ch, value);
}

static void cap_enc_late_init(void)
{
	int i, channels=0;
	int fr;
	
#if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)
	channels = 1+1; /* 1 camera + 1 stream */
	fr       = 30;
#else
	channels = 4+1; /* 4 camera + 1 stream */
	fr       = 15;
#endif
	
	//#--- set rate control
	for(i=0; i < channels; i++)
    {
		ctrl_vid_rate(i, RATE_CTRL_VBR);
        ctrl_vid_gop_set(i, fr); 
	}

	cap_jpeg_quality(channels, 80);
}

static int capt_param_init(VCAP_PARAMS_S *vcapParams)
{
	//# set cap param
	vcapParams->width 	= 1280;
	vcapParams->height	= 720;

	return SOK;
}

/*****************************************************************************
* @brief    app capture start/stop function
* @section  [desc]
*****************************************************************************/
int app_cap_start(void)
{
	VSYS_PARAMS_S   vsysParams;
	VCAP_PARAMS_S   vcapParams;
	VENC_PARAMS_S   vencParams;
	VDIS_PARAMS_S   vdisParams;
	
	if(iapp->ste.b.cap)		//# already start
		return SOK;

	dprintf("start ...\n");
	
	//# register event
	Vsys_registerEventHandler(vsys_event, NULL);
	
	//#--- init params
	Vsys_params_init(&vsysParams);
	Vcap_params_init(&vcapParams);
	Venc_params_init(&vencParams);
	
	Vdis_params_init(&vdisParams, 1);  // 0 sd, 1 hd 2 fhd 
	
	//#--- init module
	vsysParams.enableEncode = 1; //# if hardware test -> 0
	vsysParams.enableHDMI	= 1; //# 0 sd, 1 hd 2 fhd
	vsysParams.enableMjpeg	= 1;
	vsysParams.systemUseCase = VSYS_USECASE_CAPTURE;
	vsysParams.decoderHD = SYSTEM_DEVICE_VID_DEC_NVP2440H_DRV;
	
	/* case 1 -> //# 3m~25m
	 * case 2 -> //# 0.5m~20m
	 * case 3 -> //# 3m~30m
	 * case 4 -> //# 0.15m~15m
	 * others -> //# 0.5m~15m
     */
	vsysParams.serdesEQ = 2;
	
	vsysParams.captMode = CAPT_MODE_720P;
#if defined(NEXXONE) || defined(NEXX360H) || #defined(NEXXB_ONE)
	vsysParams.numChs = 1;
#else
	vsysParams.numChs = 4;
#endif	
	
	capt_param_init(&vcapParams);
	cap_enc_init(&vencParams);
	
	Vsys_init(&vsysParams);
	Vcap_init(&vcapParams);
	Venc_init(&vencParams);
	Vdis_init(&vdisParams);
	
	vid_cap_start();
	
	//#--- start link
#if defined(NEXXONE) || defined(NEXX360H) || defined(NEXXB_ONE)
	Vsys_create();
#else	
	Vsys_create(0);
#endif	
	Vsys_datetime_init();	//# m3 Date/Time init

	Vcap_start();
	Vdis_start();
	
	Venc_start();

	cap_enc_late_init();
	ctrl_enc_multislice() ; 

	iapp->ste.b.cap = 1;

	dprintf("... done!\n");

	return SOK;
}

int app_cap_stop(void)
{
	if(!iapp->ste.b.cap)
		return EINVALID;

	dprintf("start ...\n");
	iapp->ste.b.cap = 0;

	//#--- stop link
	Venc_stop();
	Vdis_stop();
	Vcap_stop();
	
	vid_cap_stop();
	Vsys_delete();

	//#--- exit module
	Vdis_exit();
	Venc_exit();
	Vcap_exit();
	Vsys_exit();
	
	app_msleep(500);	//# wait m3 cap_stop done
	
	dprintf("... done!\n");

	return SOK;
}
