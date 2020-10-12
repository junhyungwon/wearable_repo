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
#include <sys/stat.h>
#include <unistd.h>

#include <mcfw/src_linux/mcfw_api/ti_vsys_priv.h>
#include "ti_vsys.h"
#include "ti_vcap.h"
#include "ti_venc.h"
#include "ti_vdis.h"
#include "ti_mjpeg.h"

#include "dev_micom.h"

#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_ctrl.h"
#include "app_snd.h"
#include "app_cap.h"
#include "app_dev.h"
#include "app_rtsptx.h"
#include "app_set.h"
#include "app_rec.h"
#include "app_mcu.h"
#include "app_web.h"
#include "app_gps.h"

#define H264_DUMP  0
#define JPEG_DUMP  0

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
typedef struct {
	app_thr_obj vObj;		//# video capture(ipcbits) thread

	g_mem_info_t *imem;
} app_cap_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_cap_t cap_obj;
static app_cap_t *icap=&cap_obj;
static app_gps_meta_t gmeta;

#if H264_DUMP
static int first = 0;
static int cnt = 0;
static FILE *fp = NULL;
#endif

#if JPEG_DUMP
static FILE *jfp = NULL;
#endif

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
void video_status(void)
{
    int i, temp, count, ret, vcount = 0;
	int vstatus[MODEL_CH_NUM] = {0,};

    char msg[128] = {0,};

	/* current maximum video count */
	count = Vcap_get_video_status(MODEL_CH_NUM, &vstatus[0], &temp);
	
#if defined(NEXXONE)
	app_leds_cam_ctrl(vstatus[0]);
	dprintf("cam_0 : %s!\n", vstatus[0]?"video detect":"no video");
    vcount += vstatus[0] ;
#else
	for (i = 0; i < count; i++)
    {
		/* cam led on/off */
		app_leds_cam_ctrl(i, vstatus[i]);
		dprintf("cam_%d : %s!\n", i, vstatus[i]?"video detect":"no video");
        vcount += vstatus[i] ;
	}
#endif
	app_cfg->vid_count = vcount;
    sprintf(msg, " Camera Detected Count: %d", count);
	app_log_write(MSG_LOG_WRITE, msg);

	if (app_cfg->ste.b.cap == 0) {
		app_cfg->ste.b.cap = 1;
		
		if (app_cfg->vid_count > 0 && app_set->rec_info.auto_rec)
			app_rec_start();  //#--- record start
	}
	
    if (app_cfg->vid_count == 0)
    {
		ret = app_rec_state();
		if (ret) {
        	app_rec_stop(1);
			sleep(1); /* wait for file close */
		}
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
	int i, idx, pre_msec = -1;
	Uint64 captime, pre_captime = -1;
	stream_info_t *ifr;
	char *addr;
	int meta_size = 0;

	meta_size = sizeof(app_gps_meta_t);
	Venc_getBitstreamBuffer(&fullBufList, 0);

	for (i=0; i<fullBufList.numBufs; i++)
	{
		/* watchdog clear */
		app_cfg->wd_flags |= WD_ENC;
		
		pFullBuf = &fullBufList.bitsBuf[i];
		captime = (Uint64)((Uint64)pFullBuf->upperTimeStamp<<32|pFullBuf->lowerTimeStamp);

		if(pFullBuf->codecType == (VCODEC_TYPE_E)IVIDEO_H264HP || pFullBuf->codecType == 0)
		{
			addr = g_mem_get_addr(pFullBuf->filledBufSize, &idx);
			if (addr == NULL) {
				continue;
			}

			ifr = &icap->imem->ifr[idx];
			ifr->d_type = CAP_TYPE_VIDEO;
			ifr->ch = pFullBuf->chnId;
			ifr->addr = addr;
			ifr->offset = (int)addr - g_mem_get_virtaddr();
			ifr->b_size = pFullBuf->filledBufSize;
			ifr->frm_wi = pFullBuf->frameWidth;
			ifr->frm_he = pFullBuf->frameHeight;
			ifr->frm_rate = app_cfg->ich[ifr->ch].fr;
			ifr->is_key = (pFullBuf->frameType == VCODEC_FRAME_TYPE_I_FRAME) ? 1:0;
			captime = (Uint64)((Uint64)pFullBuf->upperTimeStamp<<32|pFullBuf->lowerTimeStamp);
			ifr->t_sec = (Uint32)(captime/1000);
			ifr->t_msec = (Uint32)(captime%1000);

			app_memcpy(ifr->addr, (char*)pFullBuf->bufVirtAddr, ifr->b_size);
			
            if(app_cfg->ste.b.rtsptx)
            {
				/* ch == 1 --> streaming */
                if(pFullBuf->codecType == (VCODEC_TYPE_E)IVIDEO_H264HP && ifr->ch == STREAM_CH_NUM)
                {
					captime += 8 ;

//                    printf("ifr->t_sec = %d ifr->t_msec = %ld \n",ifr->t_sec, ifr->t_msec) ;
#if H264_DUMP
                    if(first == 0 && ifr->is_key)
                    {
                        fp = fopen("/mmc/DCIM/fitt_hd.264","w") ;
                        first = 1 ;
                    }
                    if(first == 1)
                    {
                        fwrite((void *)pFullBuf->bufVirtAddr, ifr->b_size, 1, fp) ;
                        cnt += 1 ;
                        if(cnt == 300)
                        {
                            fclose(fp) ;
                            first = 2 ;
                            printf("fclose......\n") ;
                        }
                    }
#endif
					
                    app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size,
                                        ifr->is_key?FTYPE_VID_I:FTYPE_VID_P, STYPE_VID_CH1, captime);
                }
				/* ch == 2 --> JPEG  */
                else if(pFullBuf->codecType == IVIDEO_MJPEG || pFullBuf->codecType == 0 || ifr->ch == JPEG_CH_NUM)
                {
					FILE *jpeg_f = NULL;
//                    printf("Jpeg...... channel = %d pFullBuf->codecType = %d is_key = %d\n",pFullBuf->chnId, pFullBuf->codecType, ifr->is_key) ;          

                    jpeg_f = fopen("/tmp/fitt360.jpeg","w") ;
                    if (jpeg_f != NULL)
                    { 
//                        lockf(fp, F_LOCK, 1L) ;
                        fwrite((void *)pFullBuf->bufVirtAddr, ifr->b_size, 1, jpeg_f);
                        fclose(jpeg_f) ;
                    } 
#if JPEG_DUMP
                    jfp = fopen("/mmc/DCIM/fitt360.jpeg","w") ;

                    if(jfp)
                    { 
                        fwrite((void *)pFullBuf->bufVirtAddr, ifr->b_size, 1, jfp) ;
                        fclose(jfp) ;
				    }
#endif
                    app_rtsptx_write((void *)ifr->addr, ifr->offset, ifr->b_size,
                                        ifr->is_key?FTYPE_VID_I:FTYPE_VID_P, STYPE_VID_CH2, captime);
                } 

            }

			//# META (GPS)
			if (pFullBuf->chnId == 0 && pFullBuf->frameType == VCODEC_FRAME_TYPE_I_FRAME)
			{
                addr = g_mem_get_addr(meta_size, &idx);
                if (addr == NULL) {
					continue;
				}

				memset(&gmeta, 0, meta_size);
                ifr = &icap->imem->ifr[idx];
                memset(ifr, 0, sizeof(stream_info_t));

                ifr->d_type = 2; //DATA_TYPE_META;
                ifr->ch = 0;
                ifr->addr = addr;
				ifr->offset = (int)addr - g_mem_get_virtaddr();
                ifr->b_size = meta_size;

				captime = (Uint64)((Uint64)pFullBuf->upperTimeStamp<<32|pFullBuf->lowerTimeStamp);
                ifr->t_sec = (Uint32)(captime/1000);
                ifr->t_msec = (Uint32)(captime%1000);

                app_gps_get_rmc_data((void *)&gmeta);
                app_memcpy(addr, (char*)&gmeta, ifr->b_size);
			}//# end of META
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
	if(thread_create(&icap->vObj, THR_vid_cap, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return EFAIL;
    }

	return SOK;
}

static void vid_cap_stop(void)
{
	app_thr_obj *tObj;

    //#--- recording stop
    if (app_rec_state()) {
        app_rec_stop(1);
		sleep(1); /* wait for file close */
	}

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

	channels = MODEL_CH_NUM+1; /* 4 camera + 1 stream */

	for (i=0; i < channels; i++)
	{
		params.frameRate 		= app_cfg->ich[i].fr;
		params.inputFrameRate 	= DEFAULT_FPS;
		params.targetBitRate 	= app_cfg->ich[i].br * 1000;

		vencParams->encChannelParams[i].enableAnalyticinfo = 0;
		Venc_params_set(vencParams, i, &params, VENC_ALL);
	}

	if (app_cfg->en_jpg)
	{
		params.frameRate 		= JPEG_FPS;
		params.inputFrameRate 	= DEFAULT_FPS;
		params.targetBitRate 	= (app_cfg->ich[MODEL_CH_NUM].br * 1000)/DEFAULT_FPS;

		vencParams->encChannelParams[channels].enableAnalyticinfo = 0;
		Venc_params_set(vencParams, channels, &params, VENC_ALL);
	}

	return SOK;
}

static void cap_jpeg_quality(int ch, int value)
{
	VENC_CHN_DYNAMIC_PARAM_S params = { 0 };

	params.qpMin 	= 1;
	params.qpMax 	= 100;
	params.qpInit	= value;

	Venc_setDynamicParam(ch, 0, &params, VENC_QPVAL_I);
	//dprintf("ch %d, value %d\n", ch, value);
}

static void cap_enc_late_init(void)
{
	int i, channels=0;

	channels = MODEL_CH_NUM+1;
	
	//#--- set rate control
	for(i=0; i < channels; i++)
    {
		ctrl_vid_rate(i, app_cfg->ich[i].rc, app_cfg->ich[i].br);
//        ctrl_vid_gop_set(i, app_set->ch[i].gop) ; 

        ctrl_vid_gop_set(i, app_set->ch[i].framerate) ; 
	}

    if (app_cfg->en_jpg)
		cap_jpeg_quality(channels, JPEG_QUALITY);
}

static int capt_param_init(VCAP_PARAMS_S *vcapParams)
{
	int idx=0, wi, he, br, channels;
	app_ch_cfg_t *ch_prm;

	channels = MODEL_CH_NUM+1;

	for(idx=0; idx<channels; idx++)
	{
		ch_prm = &app_set->ch[idx];

		if (get_frame_size(ch_prm->resol, &wi, &he) == EFAIL) {
			eprintf("Failed get resolution!!!\n");
			return EFAIL;
		}
        dprintf("channel = %d resolution = %d\n", idx, ch_prm->resol) ;

		app_cfg->ich[idx].wi = wi;
		app_cfg->ich[idx].he = he;
		app_cfg->ich[idx].fr = app_set->ch[idx].framerate ;
		app_cfg->ich[idx].br = (app_set->ch[idx].quality * app_cfg->ich[idx].fr)/DEFAULT_FPS;
		app_cfg->ich[idx].rc = app_set->ch[idx].rate_ctrl ; 
		dprintf(" [app] (CH%d): %dx%d, fr %d, br %d\n", idx, wi, he, app_cfg->ich[idx].fr, app_cfg->ich[idx].br);
        
		if(idx==0) {
			//# set cap param
			vcapParams->width 	= wi;
			vcapParams->height	= he;
		}
	}

	return SOK;
}

/*****************************************************************************
* @brief    capture start/stop function
* @section  [desc]
*****************************************************************************/
int app_cap_start(void)
{
	VSYS_PARAMS_S   vsysParams;
	VCAP_PARAMS_S   vcapParams;
	VENC_PARAMS_S   vencParams;
	VDIS_PARAMS_S   vdisParams;

	if (app_cfg->ste.b.cap) {
		return 0;
	}

	//# register event
	Vsys_registerEventHandler(vsys_event, NULL);

	//# static config clear - when Variable declaration
	memset((void *)icap, 0x0, sizeof(app_cap_t));

	//#--- init params
	Vsys_params_init(&vsysParams);
	Vcap_params_init(&vcapParams);
	Venc_params_init(&vencParams);

	Vdis_params_init(&vdisParams, app_set->ch[MODEL_CH_NUM].resol);  // 0 sd, 1 hd 2 fhd 

	//#--- init component
	vsysParams.enableEncode 	= 1; //# if hardware test -> 0
	vsysParams.enableHDMI		= app_set->ch[MODEL_CH_NUM].resol;
    vsysParams.enableMjpeg      = app_cfg->en_jpg;
	vsysParams.systemUseCase 	= VSYS_USECASE_CAPTURE;
	vsysParams.decoderHD 		= SYSTEM_DEVICE_VID_DEC_NVP2440H_DRV; //SYSTEM_DEVICE_VID_DEC_PH3100K_DRV;

	/* case 1 -> //# 3m~25m
	 * case 2 -> //# 0.5m~20m
	 * case 3 -> //# 3m~30m
	 * case 4 -> //# 0.15m~15m
	 * others -> //# 0.5m~15m
     */
	vsysParams.serdesEQ = 2;

	vsysParams.captMode = CAPT_MODE_720P;
	vsysParams.numChs = MODEL_CH_NUM;

	app_cfg->wd_tot |= WD_ENC; /* Fixed */
	app_cfg->num_ch = vsysParams.numChs;
	if (capt_param_init(&vcapParams) == EFAIL) {
		eprintf("Failed initialize capture parameters!!\n");
		return EFAIL;
	}

	cap_enc_init(&vencParams);

	Vsys_init(&vsysParams);
	Vcap_init(&vcapParams);
	Venc_init(&vencParams);
	Vdis_init(&vdisParams);

	vid_cap_start();

	//#--- start component
#if defined(NEXXONE)	
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

	aprintf("done!\n");

	return SOK;
}

int app_cap_stop(void)
{
	if(!app_cfg->ste.b.cap) {		//# already stop
		return EINVALID;
	}
	app_cfg->ste.b.cap = 0;

	//#--- stop component
	Venc_stop();
	Vdis_stop();
	Vcap_stop();

	vid_cap_stop();
	Vsys_delete();

	//#--- exit component
	Vdis_exit();
	Venc_exit();
	Vcap_exit();
	Vsys_exit();

	app_msleep(500);	//# wait m3 cap_stop done

	aprintf("done!\n");

	return SOK;
}

/*
 * sound captureë¥??„í•œ GMEM ë©”ëª¨ë¦?ì£¼ì†Œ ë°˜í™˜
 */
void *app_cap_get_gmem(void)
{
	return (void *)icap->imem;
}
