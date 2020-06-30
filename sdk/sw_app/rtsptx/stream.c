/** ===========================================================================
* @file stream.c
*
* @path $(IPNCPATH)\multimedia\encode_stream\stream
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */

#include <stdio.h>
#include <string.h>
#include <osa.h>

#include "stream.h"

typedef struct _OsdTextPrm{
	int nLegnth;
	char strText[16];
} OsdTextPrm;

//extern int get_g_mem_phyaddr(void);
//extern int get_g_mem_size(void);

extern int g_mem_get_phyaddr(void) ;
extern int g_mem_get_size(void) ;

static int Init_Msg_Func(STREAM_PARM *pParm);

static STREAM_PARM stream_parm;
int ext_resol = 0 ;
char ext_name[6] ;

void stream_feature_setup( int nFeature, void *pParm );

/**
 * @brief	Stream initialization
 * @param	"STREAM_PARM *pParm" : stream parameter
 * @param	"STREAM_SET *pSet"
 * @return	0 : success ;
 */
int stream_init(STREAM_SET *pSet)
{
	STREAM_PARM *pParm = &stream_parm;
	int i, ret = 0;

	/* Init memory manager */
	if(MemMng_Init( &(pParm->MemInfo) ) != STREAM_SUCCESS)
	{
		eprintf_rt("Memory manager init fail\n");
		return STREAM_FAILURE;
	}
	if(CacheMng_Init( &(pParm->MemInfo) ) != STREAM_SUCCESS)
	{
		eprintf_rt("Cache manager init fail\n");
		return STREAM_FAILURE;
	}

	GopInit();

//	for(i = 0; i < STREAM_SEM_NUM; i++)
	for(i = 0; i < STREAM_SEM_AUDIO_1; i++)
	{
		pParm->MemMngSemId[i] = Sem_Creat(i);
// hwjun add 

        pParm->MemInfo.video_info[i].width = pSet->stream_wi[i];
        pParm->MemInfo.video_info[i].height = pSet->stream_he[i];

		pParm->ImageWidth[i]  = pSet->stream_wi[i];
		pParm->ImageHeight[i] = pSet->stream_he[i];
	}



    strcpy(ext_name, pSet->stream_name) ;

	for(i = 0; i < GOP_INDEX_NUM; i++)
	{
		pParm->lockNewFrame[i] 	= 0;
		pParm->checkNewFrame[i]	= 0;
		Rendezvous_open(&(pParm->objRv[i]), 2);
	}

	pParm->qid	= Msg_Init(MSG_KEY);

	ret = Init_Msg_Func(pParm);
	if(ret < 0) {
		eprintf_rt("Init_Msg_Func init fail\n");
		return STREAM_FAILURE;
	}

	return STREAM_SUCCESS;
}

/**
 * @brief	Stream write
 * @param	"void *pAddr"
 * @param	"int size"
 * @param	"int frame_type"
 * @param	"int stream_type"
 * @param	"STREAM_PARM *pParm" : stream parameter
 * @return	0 : success ; -1 : fial
 */
#ifdef USE_GMEM
int stream_write(int pAddr, int size, int frame_type, int stream_type, unsigned int timestamp)
#else
int stream_write(void *pAddr, int size, int frame_type, int stream_type, unsigned int timestamp)
#endif
{
	STREAM_PARM *pParm = &stream_parm;
	VIDEO_BLK_INFO 	*pVidInfo = NULL;

	int ret = -1;
	int sem_id = 0;
	int	cnt = 0;

	pVidInfo = &(pParm->MemInfo.video_info[stream_type]);
	sem_id	 = pParm->MemMngSemId[stream_type];

	Sem_lock(sem_id);

	pVidInfo->extrasize = 0;
	pVidInfo->timestamp = timestamp;

	ret = MemMng_Video_Write((void *)pAddr, size, frame_type, pVidInfo);
	if( ret < 0 ) {
		eprintf_rt("Error code: %d at %d\n", ret,__LINE__);
		Sem_unlock(sem_id);
		return STREAM_FAILURE;
	}

	for( cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
	{
		pVidInfo->frame[pVidInfo->cur_frame].ref_serial[cnt] = pParm->MemInfo.video_info[cnt].cur_serial;
	}

	/* post process */
	//if(stream_type == VIDOE_INFO_CH1 || stream_type == VIDOE_INFO_CH2)
	{
		if(pParm->lockNewFrame[stream_type])
		{
			if(frame_type == P_FRAME) {
				ret = LockCurrentGopP(pVidInfo, stream_type);
				if(ret) {
					pParm->lockNewFrame[stream_type] = 0;
				}
			}
			else if(frame_type == I_FRAME) {
				pParm->lockNewFrame[stream_type] = 0;
			}
		}
	}

	if(pParm->checkNewFrame[stream_type])
	{
		Rendezvous_meet(&(pParm->objRv[stream_type]));
		pParm->checkNewFrame[stream_type] = 0;
		Rendezvous_reset(&(pParm->objRv[stream_type]));
	}

	Sem_unlock(sem_id);

	return STREAM_SUCCESS;
}

/**
 * @brief	Send quit command
 * @param	"STREAM_PARM *pParm" : stream parameter
 * @return	none
 */
static void SendQuitCmd(STREAM_PARM *pParm)
{
	MSG_BUF msgbuf;

	memset(&msgbuf,0,sizeof(msgbuf));
	msgbuf.Des = MSG_TYPE_MSG1;
	msgbuf.Src = 0;
	msgbuf.cmd = MSG_CMD_QUIT;
	Msg_Send( pParm->qid , &msgbuf , sizeof(msgbuf) );
}

/**
 * @brief	Stream end
 * @param	"STREAM_PARM *pParm" : stream parameter
 * @return	none
 */
int stream_end(void)
{
	STREAM_PARM *pParm = &stream_parm;
	int cnt = 0;

	SendQuitCmd(pParm);

	pthread_join(pParm->threadControl, (void **) 0);

	fprintf(stderr,"Msg_ThrFxn closing...\n");

	for( cnt = 0; cnt < GOP_INDEX_NUM; cnt++ )
	{
		Rendezvous_force(&(pParm->objRv[cnt]));
		Rendezvous_close(&(pParm->objRv[cnt]));
	}

	for( cnt = 0; cnt < STREAM_SEM_NUM; cnt++ )
	{
		Sem_kill(pParm->MemMngSemId[cnt]);
	}

	for( cnt = 0; cnt < VIDOE_INFO_END-1; cnt++)
	{
		GopCleanup(&pParm->MemInfo.video_info[cnt],cnt);
		GopCleanup(&pParm->MemInfo.video_info[cnt],cnt);
	}

//	GopCleanup(&pParm->MemInfo.video_info[VIDOE_INFO_MP4],GOP_INDEX_MP4);
//	GopCleanup(&pParm->MemInfo.video_info[VIDOE_INFO_MP4],GOP_INDEX_MP4_EXT);

	CacheMng_Release(&(pParm->MemInfo));
	MemMng_release( &(pParm->MemInfo) );

	Msg_Kill( pParm->qid);
	memset(pParm,0,sizeof(STREAM_PARM));

	return STREAM_SUCCESS;
}

/**
 * @brief	Show info
 * @param	"STREAM_PARM *pParm" : stream parameter
 * @param	"int id"
 * @return	none
 */
void ShowInfo(STREAM_PARM *pParm, int id)
{
	dprintf("ID = %d \n",id);
	dprintf("pParm->ImageHeight = %d \n",pParm->ImageHeight[id]);
	dprintf("pParm->ImageWidth = %d \n",pParm->ImageWidth[id]);
	dprintf("pParm->JpgQuality = %d \n",pParm->JpgQuality);
	dprintf("pParm->Mpeg4Quality = %d \n",pParm->Mpeg4Quality[id]);
	dprintf("pParm->qid = %d \n",pParm->qid);
}

/**
 * @brief	Message control thread
 * @param	"void* args" : arguments
 * @return	none
 */


void msg_resol(int resol) 
{
    ext_resol = resol ;
}   

void *Msg_CTRL(void *args)
{
	STREAM_PARM *pParm = (STREAM_PARM *)args;
	int qid = (int)pParm->qid;
	int exit = 0;

	MSG_BUF msgbuf;
	int msg_size = 0;
//	Rendezvous_Handle hRv = &pParm->objRv[0];	//# Not used

	while(exit == 0)
	{
		//# receive message from wis-streamer -----
		msg_size = msgrcv(qid, &msgbuf, sizeof(msgbuf)-sizeof(long), MSG_TYPE_MSG1, 0);
		if( msg_size <= 0 ) {
			dprintf("Receive msg fail \n");
			continue;
		}

        strcpy(msgbuf.name, ext_name) ;
        msgbuf.resol = ext_resol;

		switch( msgbuf.cmd )
		{
			case MSG_CMD_QUIT:
			{
				exit = 1;
				break;
			}
			case MSG_CMD_GET_MEM:
			{
				#ifdef USE_GMEM
//				msgbuf.mem_info.addr = get_g_mem_phyaddr();
				msgbuf.mem_info.addr = g_mem_get_phyaddr();
//				msgbuf.mem_info.size = get_g_mem_size();
				msgbuf.mem_info.size = g_mem_get_size();
				if(msgbuf.mem_info.size > 0)
					msgbuf.ret = 0;
				else
					msgbuf.ret = -1;
				#else
				msgbuf.ret = 0;		//# use shared mem
				#endif
                 
				break;
			}
			case MSG_CMD_GET_CUR_FRAME:			//# get frame serial
			{
				VIDEO_FRAME		*pFrame = NULL;
				VIDEO_BLK_INFO 	*pVidInfo = NULL;
				int				sem_id = 0;

				sem_id	= pParm->MemMngSemId[msgbuf.frame_info.format];
				Sem_lock(sem_id);

				pVidInfo = &pParm->MemInfo.video_info[msgbuf.frame_info.format];
				if(msgbuf.frame_info.format < FMT_AUDIO_CH1)
                {
					pFrame = GetLastI_Frame(pVidInfo);
                }
				else
					pFrame = GetCurrentFrame(pVidInfo);

				if(pFrame == NULL) {
					msgbuf.ret = -1;
				}
				else
				{

					msgbuf.frame_info.serial_no = pFrame->serial;
					msgbuf.frame_info.flags = pFrame->flag;
					msgbuf.ret = 0;
				}

				Sem_unlock(sem_id);
				break;
			}
			case MSG_CMD_GET_AND_LOCK_IFRAME:
			{
				VIDEO_FRAME* pFrame = NULL;
				VIDEO_BLK_INFO *pVidInfo = NULL;
				int				sem_id = 0;

				switch(msgbuf.frame_info.format)
				{
					default:
						sem_id	= pParm->MemMngSemId[msgbuf.frame_info.format];
					break;
				}
				Sem_lock(sem_id);

				if(msgbuf.frame_info.format < FMT_AUDIO_CH1)
				{
					pVidInfo = &pParm->MemInfo.video_info[msgbuf.frame_info.format];
					pFrame = GetLastI_Frame(pVidInfo);
				}

				if(pFrame == NULL)
				{
					msgbuf.ret = -1;
				}
				else
				{
					if(msgbuf.frame_info.format < FMT_AUDIO_CH1)
					{
						msgbuf.frame_info.serial_no = pFrame->serial;
						msgbuf.frame_info.quality 	= pParm->Mpeg4Quality[msgbuf.frame_info.format];
						msgbuf.ret = LockGopBySerial(msgbuf.frame_info.serial_no, pVidInfo,msgbuf.frame_info.format);

						if(msgbuf.ret == GOP_INCOMPLETE){
							msgbuf.ret = GOP_COMPLETE;
							pParm->lockNewFrame[msgbuf.frame_info.format] = 1;
						}
					}

					if(msgbuf.ret == GOP_COMPLETE)
					{
						CACHE_DATA_INFO Cache_info;
						int cnt = 0;
						msgbuf.frame_info.offset = CacheMng_GetCacheInfoBySerial(pFrame->serial, &Cache_info, pVidInfo);
						#ifndef USE_GMEM
						msgbuf.frame_info.offset = Cache_info.start_phy - GetMemMngPhyBaseAddr(&pParm->MemInfo);
						#endif
						msgbuf.frame_info.size = Cache_info.realsize;

						if(msgbuf.frame_info.format < FMT_AUDIO_CH1)
						{
//							    msgbuf.frame_info.width 	= pParm->ImageWidth[msgbuf.frame_info.format];
//							    msgbuf.frame_info.height 	= pParm->ImageHeight[msgbuf.frame_info.format];
                            if(ext_resol == 2)
                            {
							    msgbuf.frame_info.width 	= 1920;
							    msgbuf.frame_info.height 	= 1080;
                            }
                            if(ext_resol == 1)
                            {
							    msgbuf.frame_info.width 	= 1280;
							    msgbuf.frame_info.height 	= 720;
                            }
                            if(ext_resol == 0)
                            {
							    msgbuf.frame_info.width 	= 720;
							    msgbuf.frame_info.height 	= 480;
                            }
						}

						msgbuf.frame_info.flags = Cache_info.flag;
						msgbuf.frame_info.frameType = Cache_info.fram_type;
						msgbuf.frame_info.timestamp = Cache_info.timestamp;
						for (cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
						{
							msgbuf.frame_info.ref_serial[cnt] = Cache_info.ref_serial[cnt];
						}
					}
				}
				Sem_unlock(sem_id);
				break;
			}
			case MSG_CMD_LOCK_FRAME:
			{
				VIDEO_BLK_INFO *pVidInfo = NULL;
				int				sem_id = 0;

				sem_id	= pParm->MemMngSemId[msgbuf.frame_info.format];
				Sem_lock(sem_id);

				pVidInfo = &pParm->MemInfo.video_info[msgbuf.frame_info.format];
				if(msgbuf.frame_info.format >= FMT_AUDIO_CH1)
				{
					msgbuf.frame_info.quality = 0;
					msgbuf.ret = CacheMng_Video_CacheLock(msgbuf.frame_info.serial_no, pVidInfo);
				}
				else
				{
					msgbuf.frame_info.quality = pParm->Mpeg4Quality[msgbuf.frame_info.format];
					msgbuf.ret = LockGopBySerial(msgbuf.frame_info.serial_no, pVidInfo,msgbuf.frame_info.format);

					if(msgbuf.ret == GOP_INCOMPLETE)
					{
						msgbuf.ret = 0;
						pParm->lockNewFrame[msgbuf.frame_info.format] = 1;
					}
				}

				if(msgbuf.ret == 0)
				{
					CACHE_DATA_INFO Cache_info;
					int	cnt =0;

					msgbuf.frame_info.offset = CacheMng_GetCacheInfoBySerial(msgbuf.frame_info.serial_no, &Cache_info, pVidInfo);
					#ifndef USE_GMEM
					msgbuf.frame_info.offset = Cache_info.start_phy - GetMemMngPhyBaseAddr(&pParm->MemInfo);
					#endif
					msgbuf.frame_info.size = Cache_info.realsize;

					if(msgbuf.frame_info.format < FMT_AUDIO_CH1)
					{
						msgbuf.frame_info.width 	= pParm->ImageWidth[msgbuf.frame_info.format];
						msgbuf.frame_info.height 	= pParm->ImageHeight[msgbuf.frame_info.format];
                        if(ext_resol == 2)
                        {
						    msgbuf.frame_info.width 	= 1920;
						    msgbuf.frame_info.height 	= 1080;
                        }
                        if(ext_resol == 1)
                        {
						    msgbuf.frame_info.width 	= 1280;
							msgbuf.frame_info.height 	= 720;
                        }
                        if(ext_resol == 0)
                        {
							msgbuf.frame_info.width 	= 720;
							msgbuf.frame_info.height 	= 480;
                        }
					}
//printf("MSG_CMD_LOCK_FRAME size = %d width = %d height = %d\n",msgbuf.frame_info.size, msgbuf.frame_info.width, msgbuf.frame_info.height) ;
					msgbuf.frame_info.flags 	= Cache_info.flag;
					msgbuf.frame_info.frameType = Cache_info.fram_type;
					msgbuf.frame_info.timestamp = Cache_info.timestamp;
					for (cnt = 0; cnt < VIDOE_INFO_END; cnt++ )
					{
						msgbuf.frame_info.ref_serial[cnt] = Cache_info.ref_serial[cnt];
					}
				}
				Sem_unlock(sem_id);
				break;
			}
			case MSG_CMD_UNLOCK_FRAME:
			{
				VIDEO_BLK_INFO *pVidInfo = NULL;
				int				sem_id = 0,ret;
				switch(msgbuf.frame_info.format)
				{
					default:
						sem_id	= pParm->MemMngSemId[msgbuf.frame_info.format];
					break;
				}
				Sem_lock(sem_id);

				switch(msgbuf.frame_info.format)
				{
					#if 0
					case FMT_AUDIO:
						pVidInfo = &pParm->MemInfo.video_info[AUDIO_INFO_G711];
						if(CacheMng_Video_CacheUnlock(msgbuf.frame_info.serial_no, pVidInfo)== 0){
							/* No return */
						}
						break;
					#endif

					default:
						pVidInfo = &pParm->MemInfo.video_info[msgbuf.frame_info.format];

						if(msgbuf.frame_info.format >= FMT_AUDIO_CH1)
						{
							if(CacheMng_Video_CacheUnlock(msgbuf.frame_info.serial_no, pVidInfo)== 0) {
								//# to do
							}
						}
						else
						{
							ret = UnlockGopBySerial(msgbuf.frame_info.serial_no, pVidInfo,msgbuf.frame_info.format);
							if(ret == GOP_INCOMPLETE) {
								pParm->lockNewFrame[msgbuf.frame_info.format] = 0;
								ret = 0;
							}
						}
						break;
				}
				Sem_unlock(sem_id);
				break;
			}
			case MSG_CMD_GET_VOL:
			{
				#if 0	//# used not
				VIDEO_BLK_INFO *pVidInfo = NULL;

				if(msgbuf.frame_info.format < FMT_AUDIO_CH1) {
					pVidInfo = &pParm->MemInfo.video_info[msgbuf.frame_info.format];
				}

				if(pVidInfo == NULL)
					break;

				msgbuf.frame_info.offset	= (unsigned long)(pVidInfo->extradata) - pParm->MemInfo.start_addr;
				msgbuf.frame_info.size		= pVidInfo->extrasize;
				#endif

				msgbuf.ret = 0;
				break;
			}

#if 0
			case MSG_CMD_SET_DAY_NIGHT:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_AE_MODE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_BACKLIGHT:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_BLC, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_BRIGHTNESS:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_BRIGHTNESS, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_CONTRAST:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_CONTRAST, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_SATURATION:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_SATURATION, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_SHARPNESS:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_SHARPNESS, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_WHITE_BALANCE:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_AWB_MODE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_BITRATE1:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_BIT_RATE1, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_BITRATE2:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_BIT_RATE2, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_JPEG_QUALITY:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_JPG_QUALITY, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_M41_FRAMERATE:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_M41_FRAMERATE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_M42_FRAMERATE:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_M42_FRAMERATE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_JPG_FRAMERATE:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_JPG_FRAMERATE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_TYPE_2A:	/*img2a*/
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = (int)temp;
				stream_feature_setup(STREAM_FEATURE_AEW_TYPE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_TV_SYSTEM:	/*ntsc/pal*/
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = (int)temp;
				stream_feature_setup(STREAM_FEATURE_ENV_50_60HZ, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_BINNING_SKIP:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = (int)temp;
				stream_feature_setup(STREAM_FEATURE_BINNING_SKIP, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_TSTAMP:
			{
				int value;
				unsigned char temp;
				temp = *(unsigned char*)(&msgbuf.mem_info);
				value = temp;
				stream_feature_setup(STREAM_FEATURE_TSTAMPENABLE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_PTZ:
			{
				int value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_PTZ, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_MOTION:
			{
				ApproMotionPrm tMotionPrm;
				memcpy(&tMotionPrm, &msgbuf.mem_info, sizeof(ApproMotionPrm));
				stream_feature_setup(STREAM_FEATURE_MOTION, &tMotionPrm);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_M41_ADV_FEATURE:
			{
				unsigned int  value = (unsigned int )(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_M41_ADV_FEATURE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_M42_ADV_FEATURE:
			{
				unsigned int value = (unsigned int )(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_M42_ADV_FEATURE, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_JPG_ADV_FEATURE:
			{
				unsigned int  value = (unsigned int )(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_JPG_ADV_FEATURE, &value);
				msgbuf.ret = 0;
				break;
			}

			case MSG_CMD_SET_ROICFG:
			{
				unsigned int  value = (unsigned int )(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_ROICFG, &value);
				msgbuf.ret = 0;
				break;
			}

			case MSG_CMD_SET_OSDTEXT_EN:
			{
				int  value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_OSDTEXT_EN, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_HIST_EN:
			{
				int  value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_HIST_EN, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_GBCE_EN:
			{
				int  value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_GBCE_EN, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_EVALMSG:
			{
				int  value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_EVALMSG_EN, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_OSDLOGO_EN:
			{
				int  value = (int)(&msgbuf.mem_info);
				stream_feature_setup(STREAM_FEATURE_OSDLOGO_EN, &value);
				msgbuf.ret = 0;
				break;
			}
			case MSG_CMD_SET_OSDTEXT:
			{
				OsdTextPrm* pPrm = (OsdTextPrm*)&msgbuf.mem_info;
				stream_feature_setup(STREAM_FEATURE_OSDTEXT, pPrm);
				msgbuf.ret = pPrm->nLegnth;
				break;
			}
			case MSG_CMD_SET_FDETECT:
			{
				FACE_PARM* pFDPrm = (FACE_PARM*)&msgbuf.mem_info;
				stream_feature_setup(STREAM_FEATURE_FACE_SETUP, pFDPrm);
				msgbuf.ret = 0;
				break;
			}
#endif
			default:
				break;
		}

		if(msgbuf.Src != 0)
		{
			/* response */
			msgbuf.Des = msgbuf.Src;
			msgbuf.Src = MSG_TYPE_MSG1;
			msgsnd( qid,&msgbuf,sizeof(msgbuf)-sizeof(long),0);
		}
	}

    pthread_exit(NULL);
}

/**
 * @brief	Initialize message function
 * @param	"STREAM_PARM *pParm" : stream param
 * @return	none
 */
static int Init_Msg_Func(STREAM_PARM *pParm)
{
	pthread_attr_t attr;
	struct sched_param schedParam;
	int status = 0;

	/* Initialize the thread attributes */
    if (pthread_attr_init(&attr)) {
        eprintf_rt("Failed to initialize thread attrs\n");
        return STREAM_FAILURE;
    }

	status = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  	status |= pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

	/* Set the capture thread priority */
	schedParam.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
	status |= pthread_attr_setschedparam(&attr, &schedParam);
	if (status) {
		eprintf_rt("Failed to initialize pthread_attr_setschedparam\n");
	    eprintf_rt("Failed to setschedparam thread attrs!!\n");
		return STREAM_FAILURE;
	}
	//dprintf(" Init_Msg_Func \n" );

	pthread_create(&(pParm->threadControl), &attr, Msg_CTRL, (void *)pParm);

	return STREAM_SUCCESS;
}

/**
 * @brief	Setup stream feature
 * @param	"int nFeature" : feature select
 * @param	"STREAM_PARM *pParm" : input value
 * @return	none
 */
void stream_feature_setup( int nFeature, void *pParm )
{
/*
	extern void video_bitRate_setparm( int type, int bitrate );
	extern void jpeg_quality_setparm( int quality );
	extern void SetPtzCmd(int Getcmd);

	if( nFeature < 0|| nFeature >= STREAM_FEATURE_NUM )
		return;

	switch(nFeature)
	{
		case STREAM_FEATURE_BIT_RATE1:
		{
			int input_val = *(int *)pParm;
			video_bitRate_setparm( 0, input_val );

			break;
		}
		case STREAM_FEATURE_BIT_RATE2:
		{
			int input_val = *(int *)pParm;
			video_bitRate_setparm( 1, input_val );

			break;
		}
		case STREAM_FEATURE_JPG_QUALITY:
		{
			int input_val = *(int *)pParm;
			jpeg_quality_setparm(input_val);

			break;
		}
		case STREAM_FEATURE_M41_FRAMERATE:
		{
			int input_val = *(int *)pParm;
			int set_val = 0;
			if( Aew_ext_parameter.day_night == AE_DAY)
			{
				video_frameRate_setparm(FRAME_RATE_MPEG4_1, input_val);
			}else{
				switch(input_val)
				{
					case 0:
						set_val = 0;
					break;
					case 15000:
						set_val = 1;
					break;
					case 5000:
						set_val = 2;
					break;

				}
				if( Aew_ext_parameter.SENSOR_FRM_RATE_SETUP )
					Aew_ext_parameter.SENSOR_FRM_RATE_SETUP(set_val);
				Aew_ext_parameter.sensor_frame_rate = set_val;
			}
			break;
		}
		case STREAM_FEATURE_M42_FRAMERATE:
		{
			int input_val = *(int *)pParm;
			video_frameRate_setparm(FRAME_RATE_MPEG4_2, input_val);
			break;
		}
		case STREAM_FEATURE_JPG_FRAMERATE:
		{
			int input_val = *(int *)pParm;
			video_frameRate_setparm(FRAME_RATE_JPEG, input_val);
			break;
		}
		case STREAM_FEATURE_SHARPNESS:
		{
			int input_val = *(int *)pParm;

			Aew_ext_parameter.sharpness = input_val;

			break;
		}

		case STREAM_FEATURE_CONTRAST:
		{
			int input_val = *(int *)pParm;


			Aew_ext_parameter.contrast = input_val;


			break;
		}
		case STREAM_FEATURE_BRIGHTNESS:
		{
			int input_val = *(int *)pParm;

			Aew_ext_parameter.brightness = input_val;

			break;
		}
		case STREAM_FEATURE_BLC:
		{
			int input_val = *(int *)pParm;
			if( input_val < 128 )
			{
				Aew_ext_parameter.blc = BACKLIGHT_LOW;
			}
			else if( input_val == 128 )
			{
				Aew_ext_parameter.blc = BACKLIGHT_NORMAL;
			}
			else if( input_val > 128 && input_val <= 255)
			{
				Aew_ext_parameter.blc = BACKLIGHT_HIGH;
			}
			else
			{
				Aew_ext_parameter.blc = BACKLIGHT_NORMAL;
			}

			break;
		}

		case STREAM_FEATURE_SATURATION:
		{
			int input_val = *(int *)pParm;

			Aew_ext_parameter.saturation = input_val;

			break;
		}

		case STREAM_FEATURE_AWB_MODE:
		{
			int input_val = *(int *)pParm;
			if( input_val == 0 )
			{
				Aew_ext_parameter.awb_mode = AWB_AUTO;
			}
			else if( input_val == 1 )
			{
				Aew_ext_parameter.awb_mode = INDOOR;
			}
			else if( input_val == 2 )
			{
				Aew_ext_parameter.awb_mode = OUTDOOR;
			}
			else
			{
				Aew_ext_parameter.awb_mode = AWB_AUTO;
			}

			break;
		}

		case STREAM_FEATURE_AE_MODE:
		{
			int input_val = *(int *)pParm;
			if( Aew_ext_parameter.SENSOR_FRM_RATE_SETUP )
				Aew_ext_parameter.SENSOR_FRM_RATE_SETUP(0);

			Aew_ext_parameter.sensor_frame_rate = 0;

			if( input_val == 0 )
			{
				Aew_ext_parameter.day_night = AE_NIGHT;
			}
			else if( input_val == 1 )
			{
				Aew_ext_parameter.day_night = AE_DAY;
			}
			else
			{
				Aew_ext_parameter.day_night = AE_NIGHT;

			}

			break;
		}

		case STREAM_FEATURE_AEW_TYPE:
		{
			int input_val = *(int *)pParm;
			Aew_ext_parameter.aew_enable= (input_val == 0) ? AEW_DISABLE:AEW_ENABLE;
			VIDEO_aewbSetType(input_val);

			break;
		}

		case STREAM_FEATURE_ENV_50_60HZ:
		{
			int input_val = *(int *)pParm;


			if( input_val == 0 )
			{
				Aew_ext_parameter.env_50_60Hz= VIDEO_NTSC;
				SetDispInetrface(0);
			}
			else if( input_val == 1 )
			{
				Aew_ext_parameter.env_50_60Hz = VIDEO_PAL;
				SetDispInetrface(1);
			}
			else
			{
				SetDispInetrface(0);
				Aew_ext_parameter.env_50_60Hz = VIDEO_NTSC;
			}

			break;
		}
		case STREAM_FEATURE_BINNING_SKIP:
		{
			int input_val = *(int *)pParm;
			if( input_val == 0 )
			{
				Aew_ext_parameter.binning_mode= SENSOR_BINNING;
			}
			else if( input_val == 1 )
			{
				Aew_ext_parameter.binning_mode = SENSOR_SKIP;
			}
			else
			{
				Aew_ext_parameter.binning_mode = SENSOR_BINNING;
			}

			break;
		}
		case STREAM_FEATURE_TSTAMPENABLE:
		{
			int input_val = *(int *)pParm;
			SetDrawDateTimeStatus(input_val);
			break;
		}
		case STREAM_FEATURE_PTZ:
		{
			int input_val = *(int *)pParm;
			SetPtzCmd(input_val);
			break;
		}
		case STREAM_FEATURE_MOTION:
		{
			Motion_setparm((ApproMotionPrm*) pParm);
			break;
		}
		case STREAM_FEATURE_M41_ADV_FEATURE:
		{
			unsigned int input_val = *(unsigned int *)pParm;
			VIDEO_streamSetAdvFeature( FRAME_TYPE_MPEG4_1, input_val );
			break;
		}
		case STREAM_FEATURE_M42_ADV_FEATURE:
		{
			unsigned int input_val = *(unsigned int *)pParm;
			VIDEO_streamSetAdvFeature( FRAME_TYPE_MPEG4_2, input_val );
			break;
		}
		case STREAM_FEATURE_JPG_ADV_FEATURE:
		{
			unsigned int input_val = *(unsigned int *)pParm;
			VIDEO_streamSetAdvFeature( FRAME_TYPE_JPEG, input_val );
			break;
		}
		case STREAM_FEATURE_ROICFG:
		{
			unsigned int input_val = *(unsigned int *)pParm;
			SetROICfgEnable(input_val);
			break;
		}
		case STREAM_FEATURE_OSDTEXT_EN:
		{
			int input_val = *(int *)pParm;
			SetOsdTextEnable(input_val);
			break;
		}
		case STREAM_FEATURE_HIST_EN:
		{
			int input_val = *(int *)pParm;
			SetHistogramEnable(input_val);
			break;
		}
		case STREAM_FEATURE_GBCE_EN:
		{
			int input_val = *(int *)pParm;
			SetGBCEValue(input_val);
			break;
		}
		case STREAM_FEATURE_EVALMSG_EN:
		{
			int input_val = *(int *)pParm;
			SetEvalMessage(input_val);
			break;
		}
		case STREAM_FEATURE_OSDLOGO_EN:
		{
			int input_val = *(int *)pParm;
			SetOsdLogoEnable(input_val);
			break;
		}
		case STREAM_FEATURE_OSDTEXT:
		{
			OsdTextPrm* pPrm = (OsdTextPrm*)pParm;
			SetOsdText(pPrm->strText, pPrm->nLegnth);
			break;
		}
		case STREAM_FEATURE_FACE_SETUP:
		{
			FACE_PARM* pPrm = (FACE_PARM *)pParm;
			VIDEO_streamSetFace(pPrm);
			break;
		}
		default:
			dprintf("unknow feature setup num = %d\n", nFeature);
		break;
	}
*/
}
