/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h> //# mmap
#include <sys/time.h>
#include <sys/stat.h>
#include <cmem.h>
#include <unistd.h>

#include "main.h"
#include "avi.h"
#include "av_rec_ipc_cmd_defs.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SD_MOUNT_PATH   				"/mmc"
#define REC_DIR							"DCIM"
#define PRE_REC_SEC                     15      // max 15 secs.

typedef struct {
	app_thr_obj eObj;		//# event rec thread
	FILE *fevt;				//# for event file
	
	int init;		/* task init */
	int qid;
	
	int en_pre;     //# todo
	int fr;  		//# frame rate..
	unsigned int rec_min;
	
    int rec_first;
    int old_min;
	
	int en_snd; 			//# sound enable
	int snd_ch;				//# sound channel
	int snd_rate;			//# sampling rate
	int snd_btime;			//# buffer size

	char deviceId[32];
	char fname[256];
	
	#if defined(NEXX360B) || defined(NEXX360W) 
	int ch2_keyframe;
	int ch3_keyframe;
	int ch4_keyframe;
	#endif

} app_rec_ctrl_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_rec_cfg_t cfg_obj;
app_rec_cfg_t *app_cfg = &cfg_obj;

static app_rec_ctrl_t rec_ctrl;
static app_rec_ctrl_t *irec = &rec_ctrl;

//# gmem
static g_mem_info_t	*imem;

extern int cmem_fd;	//# cmem lib
/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static void app_msleep(unsigned int msecs)
{
	struct timespec delayTime, elaspedTime;

	delayTime.tv_sec  = msecs/1000;
	delayTime.tv_nsec = (msecs%1000)*1000000;

	nanosleep(&delayTime, &elaspedTime);
}

/*----------------------------------------------------------------------------
 message send/recv function
-----------------------------------------------------------------------------*/
static int send_msg(int cmd, int du, char *name)
{
	to_main_msg_t msg;
	
	msg.type = REC_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	if(cmd == AV_CMD_REC_FLIST) {
		msg.du = du;
		strncpy(msg.fname, name, 128);
	}
	return Msg_Send(irec->qid, (void *)&msg, sizeof(to_main_msg_t));
}

static int recv_msg(void)
{
	to_rec_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(irec->qid, REC_MSG_TYPE_TO_REC, (void *)&msg, sizeof(to_rec_msg_t)) < 0) {
		return -1;
	}
	
	irec->en_snd    = msg.en_snd; 	  //# sound enable
	irec->en_pre    = msg.en_pre;     //# todo
	irec->fr        = msg.fr;  		  //# frame rate..
	irec->rec_min   = msg.stime;	  //# save time
	
	if (msg.cmd == AV_CMD_REC_START) {
		irec->snd_ch   = msg.snd_ch;
		irec->snd_rate = msg.snd_rate;		//# sampling rate
		irec->snd_btime = msg.snd_btime;	//# buffer size
	}
	memcpy(irec->deviceId, msg.deviceId, 32);
	
	return msg.cmd;
}

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
	int idx, max_cnt;
	stream_info_t *ifr;

	if (imem->max_num == 0 && imem->cur_num < 1) {
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
//----------------------------------------------------------------------------------------------------
//####################################################################################################

/*****************************************************************************
* @brief    event record function
* @section  [desc]
*****************************************************************************/
static int evt_file_open(stream_info_t *ifr)
{
	struct stat sb;
	struct tm ts, *gmtm;
	char buf_time[64];
	char filename[128];
    int minute_change=0;
	
	gmtm = gmtime((const time_t *)&ifr->t_sec);

//    localtime_r((const time_t *)&ifr->t_sec, &ts);
    memcpy(&ts, gmtm, sizeof(struct tm));
	
    if (!irec->rec_first)
    {
		if ((ts.tm_min%irec->rec_min) == 0) {
            minute_change = 1;
        }
    }
	
    if (irec->rec_first) {
        minute_change = 1;
        irec->rec_first = 0;
    }
	
	if ((irec->old_min != ts.tm_min && minute_change))
	{
		irec->old_min = ts.tm_min;

        minute_change = 0;
		
		if (irec->fevt != NULL) 
		{
			unsigned long sz;
			
        	avi_file_close(irec->fevt, irec->fname);	//# close current file
			/* calculate file size */
			lstat(irec->fname, &sb);
			sz = (sb.st_size / KB); /* Byte->KB */
			send_msg(AV_CMD_REC_FLIST, sz, irec->fname);
		}	
		//# get current date & time
//        localtime_r((const time_t *)&ifr->t_sec, &ts);
		strftime(buf_time, sizeof(buf_time), "%Y%2m%2d_%2H%2M%2S", &ts);
		sprintf(filename, "%s/%s/R_%s_%s_1ch.avi", SD_MOUNT_PATH, REC_DIR, buf_time, 
				irec->deviceId);
		
        memset(irec->fname, 0, sizeof(irec->fname));
		sprintf(irec->fname, "%s", filename);
		
		irec->fevt = avi_file_open(filename, ifr, irec->en_snd, 
						irec->snd_ch, irec->snd_rate, irec->snd_btime);	//# open new file
		if (irec->fevt == NULL) {
			eprintf("new file open (%s)\n", filename);
			return EFAIL;
		}
		
		fprintf(stdout, "new filename %s\n", filename);
/*
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
	struct stat sb;
	unsigned long sz;
	
	if (irec->fevt) {
		avi_file_close(irec->fevt, irec->fname);
		/* calculate file size */
		lstat(irec->fname, &sb);
		sz = (sb.st_size / KB); /* Byte->KB */
		send_msg(AV_CMD_REC_FLIST, sz, irec->fname);
		irec->fevt = NULL;
	}
}

static int evt_file_write(stream_info_t *ifr)
{
	int ret = OSA_SOK;

	ret = avi_file_write(irec->fevt, ifr);

	return ret;
}
//#########################################################################################################################

static void init_rec_cfg(void)
{
	irec->old_min   = -1;
    irec->rec_first = 1;
    irec->fevt      = NULL;
}

/*****************************************************************************
* @brief    event record thread function
* @section  [prm] active channel
*****************************************************************************/
static void *THR_rec_evt(void *prm)
{
	app_thr_obj *tObj = &irec->eObj;
	int cmd, exit=0, ret=0;
	int i, frame_num, read_idx=0;
	int done;
	stream_info_t *ifr;
	
	aprintf("enter...\n");
	
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		} 
		//else if (cmd == APP_CMD_STOP || ( app_set->rec_info.overwrite==OFF && app_cfg->ste.b.disk_full)) {
		else if (cmd == APP_CMD_STOP) {
			continue;
		}
		
		#if 0
		( app_set->rec_info.overwrite==OFF && app_cfg->ste.b.disk_full))
		if(_get_disk_kb_info(ifile, &sz_info) != EFAIL && app_set->rec_info.overwrite==OFF)
		{
            if(sz_info.free < (1024*MB)/KB)
			{
                continue ;
			}
        }
		#endif
		
        if (irec->en_pre)
		    read_idx = search_frame(PRE_REC_SEC);
        else
		    read_idx = search_frame(0);
		
		if (read_idx < 0) {
			eprintf("can't read frame! plz check capture dev.\n");
			continue;
		}

		ifr = &imem->ifr[read_idx];
        if (ifr->ch == 0  && ifr->d_type == DATA_TYPE_VIDEO && ifr->is_key) {
		    ret = evt_file_open(&imem->ifr[read_idx]);
			if (ret < 0) {
				//irec->rec_err = 1;
				continue;
			}
		}

		done = 0;
		while (!done)
		{
			if (tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP) {
				break;
			}
			
			#if 0
			if(_get_disk_kb_info(ifile, &sz_info) != EFAIL && app_set->rec_info.overwrite==OFF)
		    {
                if(sz_info.free < (1024*MB)/KB)
			    {
                    break ;
			    }
            }
			#endif
			
			frame_num = get_valid_frame(read_idx);
			if (frame_num < 10) {
				app_msleep(10);
				continue;
			}
			
			for (i = 0; i < frame_num; i++)
			{
				ifr = &imem->ifr[read_idx];
				if ((ifr->d_type==DATA_TYPE_VIDEO) && (ifr->ch==0) && ifr->is_key) {
					ret = evt_file_open(ifr);
					if (ret < 0) {
						eprintf("couldn't open avi!\n");
						//irec->rec_err = 1;
						//app_cfg->ste.b.mmc_err = 1;
					}
				}
				
				if (irec->fevt != NULL) 
				{
					#if defined(NEXXONE)
					/* ch=1 스트리밍 채널이 gmem에 기록되어 있으므로 avi 저장 시 이를 막아야 함 */
					if (ifr->ch < 1) {
						ret = evt_file_write(ifr);
						if (ret < 0) {
							eprintf("avi write failed!\n");
						}
					}
					#else
					if (ifr->ch == 1) {
					    if (ifr->is_key)
					        irec->ch2_keyframe = 1 ;
                    }
                    if (ifr->ch == 2) {
					    if (ifr->is_key)
					        irec->ch3_keyframe = 1 ;
                    }
                    if (ifr->ch == 3) {
					    if (ifr->is_key)
					        irec->ch4_keyframe = 1 ;
					}
					if (ifr->ch < 4) {
				        if (ifr->ch == 1 && !irec->ch2_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue;
                        }
				        if (ifr->ch == 2 && !irec->ch3_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue;
                        }
				        if (ifr->ch == 3 && !irec->ch4_keyframe)
					    {
				            app_msleep(10);
				            read_idx = idx_increase(read_idx);
						    continue;
                        }
				
				        ret = evt_file_write(ifr);
				        if (ret < 0) {
							eprintf("avi write failed!\n");
				        }
				    }
					#endif
				}
				read_idx = idx_increase(read_idx);
				if (tObj->cmd == APP_CMD_EXIT || tObj->cmd == APP_CMD_STOP) {
					break;
				}
			}
		}
		
		//# record done
		evt_file_close();
		sync();
		
		/* will free the page cache */
		//system("echo 1 > /proc/sys/vm/drop_caches");
		
		dprintf("record done!\n");
	}

	evt_file_close();		//# when APP_CMD_EXIT
	tObj->active = 0;

	aprintf("exit\n");

	return NULL;
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
static void app_main(void)
{
	app_thr_obj *tObj = &irec->eObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	
	if (thread_create(tObj, THR_rec_evt, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread!\n");
		return;
	}
	
	//# communication main process ---------------
	irec->qid = Msg_Init(REC_MSG_KEY);
	send_msg(AV_CMD_REC_READY, 0, 0);
	
	while (!exit)
	{
		cmd = recv_msg();
		if (cmd < 0) {
			dprintf("invalid cmd %d\n", cmd);
			continue;
		}
		
		dprintf("[rec process] receive cmd 0x%x\n", cmd);
		
		switch(cmd) {
		case AV_CMD_REC_START:
			init_rec_cfg();
			event_send(tObj, APP_CMD_START, 0, 0);
			break;
		case AV_CMD_REC_STOP:
			event_send(tObj, APP_CMD_STOP, 0, 0);
			break;
		case AV_CMD_REC_EXIT:
			/* recording process 종료 */
			event_send(tObj, APP_CMD_STOP, 0, 0);
			exit = 1;
			break;
		}
	}
	
	/* delete message send thread */
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);
	
	Msg_Kill(irec->qid);
	
	aprintf("exit...\n");
}

/*****************************************************************************
* @brief    system log write
* @section  [desc]
*****************************************************************************/
void log_write(char *msg)
{
	char tmpMsg[256] = {0,};
	
	memset(tmpMsg, 0, sizeof(tmpMsg));
	snprintf(tmpMsg, sizeof(tmpMsg), "[av_rec] %s", msg);
	syslog(LOG_ERR, "%s\n", tmpMsg);
}

/*****************************************************************************
* @brief    main function
* @section  [desc]
*****************************************************************************/
int main(int argc, char **argv)
{
	unsigned int phy_addr;
	unsigned int gmem_addr;
	int rc = 0;

//	printf(" [rec process] start...\n");
	
	/* get gmem address */
	sscanf(argv[1], "%x", &phy_addr);
	
	if (CMEM_init() < 0) {
		eprintf("CMEM init error\n");
		return -1;
	}
	
	gmem_addr = (unsigned int)mmap(0,	// Preferred start address
				G_MEM_SIZE,				// Length to be mapped
				PROT_WRITE | PROT_READ,	// Read and write access
				MAP_SHARED,				// Shared memory
				cmem_fd,				// File descriptor
				phy_addr);				// The byte offset from fd

	imem = (g_mem_info_t *)gmem_addr;
	if(imem->sync != G_MEM_SYNC) {
		eprintf("share memory sync err\n");
		CMEM_exit();
		return 0;
	}
	
	avi_file_init((unsigned int)gmem_addr);
		
	//#--- main --------------
	app_main();
	//#-----------------------

	CMEM_exit();
	printf(" [rec process] process exit!\n");
	
	return 0;
}
