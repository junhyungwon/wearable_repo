/******************************************************************************
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_rec.c
 * @brief	app record thread
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_comm.h"
#include "app_main.h"
#include "app_gmem.h"
#include "app_dev.h"
#include "app_snd.h"
#include "app_avi.h"
#include "app_rec.h"
#include "app_set.h"
#include "app_file.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define REC_SEC_01MIN			1	//# max 1 min.
#define REC_SEC_O5MIN			5	//# max 5 min.

#define PRE_REC_SEC                     15      // max 15 secs.

typedef struct {
	app_thr_obj eObj;		//# event rec thread

	FILE *fevt;				//# for event file
    char fname[MAX_CHAR_128] ;

	int evt_rec;			//# 1:recording, 0:idle
	int rec_err;
    int rec_first ;

    int old_min ;
    int chg_file;
	unsigned int rec_min;

	int ch2_keyframe ;
	int ch3_keyframe ;
	int ch4_keyframe ;

	char rec_root[MAX_CHAR_32];
	AVI_SYSTEM_PARAM avi_info;
} app_rec_t;

static unsigned int grec_time[REC_PERIOD_MAX] = { REC_SEC_01MIN, REC_SEC_O5MIN };

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_rec_t rec_obj;
static app_rec_t *irec=&rec_obj;

static g_mem_info_t	*imem;

disk_size_t sz_info ;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 function for frame index
-----------------------------------------------------------------------------*/
static int idx_increase(int idx)
{
	if(imem->ifr[idx].end) {
		idx = 0;
	} else {
		idx++;
	}

	return idx;
}

static int idx_decrease(int idx)
{
	idx--;
	if(idx < 0) {
		idx = imem->max_num-1;
	}

	return idx;
}

static int get_valid_frame(int idx)
{
	int cur_idx = idx_decrease(imem->cur_num);	//# protect 1 buffers
	int f_num = 0;

	if(cur_idx < 0)
		return 0;

	if(idx < cur_idx) {
		f_num = cur_idx - idx;
	}
	else if(idx > cur_idx) {
		if( (idx-cur_idx) < 5*30) {
			dprintf("WARRING! : idx buffer not enough\n");
			return 0;
		}
		f_num = (imem->max_num - idx) + cur_idx;
	}
	else {
		return 0;
	}

	return f_num;
}

static int search_frame(int sec)
{
	int idx, max_cnt, key_idx=-1;
	stream_info_t *ifr;
	int lock=0, lock_cnt=4;

	if(imem->max_num == 0 && imem->cur_num < 1) {
		return -1;
	}

	idx = idx_decrease(imem->cur_num);	//# protect 1 buffers
	if(imem->max_num == 0) {			//# for first loop
		max_cnt = imem->cur_num;
	} else {
		max_cnt = imem->max_num;
	}

    if(sec == 0)
         return idx ;

	while(max_cnt--)
	{
		ifr = &imem->ifr[idx];
		if((ifr->d_type==DATA_TYPE_VIDEO) && (ifr->is_key) && (ifr->ch == 0)) {
			sec--;
			if(sec==0)	return idx;
		}
		idx = idx_decrease(idx);
	}

	return 0;
}

/*----------------------------------------------------------------------------
 avi file open/close function
-----------------------------------------------------------------------------*/
static FILE *avi_file_open(int ch, char *file_name)
{
    char msg[128] = {0, } ;
	FILE *favi;

	favi = LIBAVI_createAvi(file_name, &irec->avi_info);
	if(favi == NULL) {
        sprintf( msg, " !!! NOR file open failed !!!");
                        app_log_write( MSG_LOG_WRITE, msg );
		eprintf("avi save handle is NULL!\n");
	}

	return favi;
}

static void avi_file_close(FILE *favi, char* fname)
{
	if(favi) {
		LIBAVI_closeAvi(favi);
	}

	favi = NULL ;
    
	app_file_add(fname) ;

}

static int avi_file_write(FILE *favi, stream_info_t *ifr)
{
	AVI_FRAME_PARAM frame;

	if(favi)
	{
		frame.buf	= (char *)(ifr->addr);
		frame.size	= ifr->b_size;

		if(ifr->d_type == DATA_TYPE_VIDEO) {
			frame.data_type		= ifr->d_type;
			frame.channel 		= ifr->ch;
			frame.iskey_frame 	= ifr->is_key;
		}
		else if(ifr->d_type == DATA_TYPE_AUDIO) {
			frame.data_type		= ifr->d_type;
		}
		else if(ifr->d_type == DATA_TYPE_META) {
			frame.data_type		= ifr->d_type;
 		}
		else {
			dprintf("unknown data type\n");
			return 0;
		}

		if(LIBAVI_write_frame(favi, &frame) < 0) {
			return -1;
		}
	}

	return 0;
}

static int avi_file_init(stream_info_t *ifr)
{
	AVI_SYSTEM_PARAM *aviInfo;
	int i;

	aviInfo = &irec->avi_info;
	memset(aviInfo, 0, sizeof(AVI_SYSTEM_PARAM));

	aviInfo->nVidCh		= app_cfg->num_ch;

	if(app_set->rec_info.audio_rec)
		aviInfo->bEnAudio	= TRUE;

	aviInfo->bEnMeta	= TRUE; //# FALSE
	aviInfo->uVideoType	= ENCODING_H264;

	for(i=0; i<aviInfo->nVidCh; i++) {
		aviInfo->nVidWi[i] = ifr->frm_wi;
		aviInfo->nVidHe[i] = ifr->frm_he;
	}
	aviInfo->fFrameRate	= (double)(ifr->frm_rate*1000./1001.);

	if(aviInfo->bEnAudio) {
		aviInfo->nAudioType			= ENCODING_ULAW;
		aviInfo->nAudioChannel		= 1;
		aviInfo->nSamplesPerSec		= SND_PCM_SRATE;
		aviInfo->nAudioBitRate		= SND_PCM_SRATE;
		aviInfo->nAudioBitPerSample	= 16;
		aviInfo->nAudioFrameSize	= 2000;
	}

	return 0;
}

/*****************************************************************************
* @brief    event record function
* @section  [desc]
*****************************************************************************/
static int evt_file_open(stream_info_t *ifr)
{
	struct tm ts, *gmtm;
	char buf_time[64];
	char filename[MAX_CHAR_128];

    int minute_change=0;
    gmtm = gmtime((const time_t *)&ifr->t_sec);

//    localtime_r((const time_t *)&ifr->t_sec, &ts);
    memcpy(&ts, gmtm, sizeof(struct tm)) ;

    if(!irec->rec_first)
    {
#if FREC_TEST    
        if(ts.tm_sec%10 == 0)
#else
		if(ts.tm_min%irec->rec_min == 0)
#endif
        {
            minute_change = 1;
        }
    }
    if(irec->rec_first)
    {
        minute_change = 1;
        irec->rec_first = 0 ;
    }

#if FREC_TEST
	if(minute_change)
#else
	if((irec->old_min != ts.tm_min && minute_change) || irec->chg_file)
#endif
	{
		irec->old_min = ts.tm_min;

        minute_change = 0;
        irec->chg_file  = 0;

        avi_file_close(irec->fevt, irec->fname);					//# close current file

		//# get current date & time
//        localtime_r((const time_t *)&ifr->t_sec, &ts);
		strftime(buf_time, sizeof(buf_time), "%Y%2m%2d_%2H%2M%2S", &ts);
		sprintf(filename, "%s/R_%s_%s_%dch.avi", irec->rec_root, buf_time, app_set->sys_info.deviceId, app_cfg->num_ch);
        sprintf(irec->fname, "%s", filename);

		avi_file_init(ifr);
		irec->fevt = avi_file_open(0, filename);	//# open new file
		if(irec->fevt == NULL) {
			eprintf("new file open (%s)\n", filename);
			return EFAIL;
		}
/*
		aprintf("new filename %s\n", filename);
		if(app_file_add(filename) == EFAIL) {
			eprintf("file management error!! (%s)\n", filename);
			return EFAIL;
		}
*/
		return SOK;
	}

	return 0;
}

static void evt_file_close(void)
{
	if(irec->fevt) {
		avi_file_close(irec->fevt, irec->fname);
		irec->fevt = NULL;
		irec->ch2_keyframe = 0 ;
		irec->ch3_keyframe = 0 ;
		irec->ch4_keyframe = 0 ;
	}
}

static int evt_file_write(stream_info_t *ifr)
{
	int ret = OSA_SOK;

	ret = avi_file_write(irec->fevt, ifr);

	return ret;
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_rec_evt(void *prm)
{
	app_thr_obj *tObj = (app_thr_obj *)prm;
	int cmd, exit=0, ret=0;
	int i, frame_num, read_idx=0;
	int done;
	stream_info_t *ifr;

	aprintf("enter...\n");
	tObj->active = 1;

	while(!exit)
	{
		//# wait cmd
		irec->evt_rec = 0;
		app_leds_rec_ctrl(LED_REC_OFF);

		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			break;
		}
		if(cmd == APP_CMD_STOP || app_cfg->ste.b.mmc_err || ( app_set->rec_info.overwrite==OFF && app_cfg->ste.b.disk_full)) {
			continue;
		}

		if(_get_disk_kb_info(ifile, &sz_info) != EFAIL && app_set->rec_info.overwrite==OFF)
		{
            if(sz_info.free < (1024*MB)/KB)
			{
                continue ;
			}
        }

        if(app_set->rec_info.pre_rec)
		    read_idx = search_frame(PRE_REC_SEC);
        else
		    read_idx = search_frame(0);

		if(read_idx < 0) {
			continue;
		}

		ifr = &imem->ifr[read_idx];
        if(ifr->ch == 0  && ifr->d_type == DATA_TYPE_VIDEO && ifr->is_key)
		    ret = evt_file_open(&imem->ifr[read_idx]);

		if(ret < 0) {
			irec->rec_err = 1;
			app_cfg->ste.b.mmc_err = 1;
			continue;
		}

		irec->evt_rec = 1;
		if(app_cfg->vid_count)
		    app_leds_rec_ctrl(LED_REC_ON);

		done = 0;
		while(!done)
		{
			if(tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP || (app_set->rec_info.overwrite==OFF && app_cfg->ste.b.disk_full) ) {
				break;
			}

		    if(_get_disk_kb_info(ifile, &sz_info) != EFAIL && app_set->rec_info.overwrite==OFF)
		    {
                if(sz_info.free < (1024*MB)/KB)
			    {
                    break ;
			    }
            }
			frame_num = get_valid_frame(read_idx);
			if(frame_num < 10) {
				app_msleep(10);
				continue;
			}

			for(i=0; i<frame_num; i++)
			{
				ifr = &imem->ifr[read_idx];
				if ((ifr->d_type==DATA_TYPE_VIDEO) && (ifr->ch==0) && ifr->is_key) {
					ret = evt_file_open(ifr);
					if (ret < 0) {
						irec->rec_err = 1;
						app_cfg->ste.b.mmc_err = 1;
					}
				}
				if(irec->fevt)
				{
                    if(ifr->ch == 1 )
				    {
					    if(ifr->is_key)
					        irec->ch2_keyframe = 1 ;
                    }
                    if(ifr->ch == 2)
					{
					    if(ifr->is_key)
					        irec->ch3_keyframe = 1 ;
                    }
                    if(ifr->ch == 3)
					{
					    if(ifr->is_key)
					        irec->ch4_keyframe = 1 ;
					}
				
              
                    if (ifr->ch < 1)  // for one eye 
				    {

				        if(ifr->ch == 1 && !irec->ch2_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue ;
                        }
				        if(ifr->ch == 2 && !irec->ch3_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue ;
                        }
				        if(ifr->ch == 3 && !irec->ch4_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue ;
                        }
				
				        ret = evt_file_write(ifr);
				        if (ret < 0) {
					        irec->rec_err = 1;
					        app_cfg->ste.b.mmc_err = 1;
				        }
				    }
				}

				read_idx = idx_increase(read_idx);
				if(tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP) {
					break;
				}
			}
		}

		//# record done
		evt_file_close();
		dprintf("record done!\n");
	}

	evt_file_close();		//# when APP_CMD_EXIT
	app_leds_rec_ctrl(LED_REC_OFF);

	irec->evt_rec = 0;
	tObj->active = 0;

	aprintf("exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    record start/stop function
* @section
*****************************************************************************/
int app_rec_start(void)
{
    int evt_cnt = 0 ;

	if(!app_cfg->en_rec) {
		return EFAIL;
	}

	if(!app_cfg->ste.b.cap || !app_cfg->ste.b.mmc || app_cfg->ste.b.busy || app_cfg->ste.b.mmc_err ) {
		eprintf("can't record cuz %s %s\n",
			app_cfg->ste.b.mmc?"":"no MMC!", app_cfg->ste.b.busy?"system busy":"");
		return EFAIL;
	}
	if (irec->evt_rec) {				//# currently record
		return EFAIL;
	}

	if(!app_set->rec_info.overwrite)
	{
	    if(_get_disk_kb_info(ifile, &sz_info) != EFAIL )
	    {
            if(sz_info.free > (1024*MB)/KB)
		    {
				if(app_cfg->vid_count)
	                dev_buzz_ctrl(100, 1);			//# buzz: rec start
		    }
	    }
	}
	else
	{
		if(app_cfg->vid_count)
	        dev_buzz_ctrl(100, 1);			//# buzz: rec start
	}

    irec->old_min   = -1;
    irec->rec_first = 1;
    irec->chg_file  = 0;
    irec->fevt      = NULL;

    while(1)
    {
		app_msleep(50);
        if(!app_rec_state())
        {
	        event_send(&irec->eObj, APP_REC_START, 0, 0);
            evt_cnt += 1 ;
            if(evt_cnt == 20)
                break ;
        }
        else
            break ;
    }

	return SOK;
}

int app_rec_stop(int buzz)
{
	if(irec->evt_rec)
	{
		if(buzz) {
			if(app_cfg->vid_count)
			    dev_buzz_ctrl(100, 2);	//# buzz: rec stop
		}
		event_send(&irec->eObj, APP_CMD_STOP, 0, 0);
		while(irec->evt_rec) {		//# wait rec stop
			app_msleep(20);
		}

		sync();
		app_msleep(200);			//# wait for safe
	}

	return SOK;
}

int app_rec_state(void)
{
	return irec->evt_rec;
}

/*****************************************************************************
* @brief    app record init/exit function
* @section  [desc]
*****************************************************************************/
int app_rec_init(void)
{
	app_thr_obj *tObj;

	if (!app_cfg->en_rec) {
		return SOK;
	}

	//# static config clear - when Variable declaration
	memset((void *)irec, 0x0, sizeof(app_rec_t));
	sprintf(irec->rec_root, "%s/%s", SD_MOUNT_PATH, REC_DIR);
	
	imem = (g_mem_info_t *)g_mem_get_virtaddr();

	//# set the record period.
	irec->rec_min = grec_time[app_set->rec_info.period_idx];

	//#--- create record thread
	tObj = &irec->eObj;
	if(thread_create(tObj, THR_rec_evt, APP_THREAD_PRI, tObj) < 0) {
		eprintf("create thread\n");
		return EFAIL;
	}

	aprintf("done!\n");

	return SOK;
}

int app_rec_exit(void)
{
	app_thr_obj *tObj;

//	app_fmng_exit();

	//#--- stop thread
	if (irec->evt_rec)
	{
		tObj = &irec->eObj;
		event_send(tObj, APP_CMD_EXIT, 0, 0);
		while(tObj->active) {
			app_msleep(20);
		}
	}
	thread_delete(tObj);

	aprintf("done!\n");

	return SOK;
}

int is_rec_err(void)
{
	int err = ON;

	if (irec != NULL)
		err = irec->rec_err;

	return err;
}

/*****************************************************************************
* @brief    change file function
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_change_file(void)
{
    if(!app_rec_state())
        return;

    irec->chg_file = 1;
}
