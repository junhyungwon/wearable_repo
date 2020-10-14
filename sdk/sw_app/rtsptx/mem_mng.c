/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    mem_mng.c
 * @brief
 * @author
 * @section MODIFY history
 *     - 2014.04.30 : Modified by phoong for use gmem
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <linux/errno.h>
#include <stdlib.h>
#include <string.h>

#include <stream.h>
#include "mem_mng.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

//static MEM_LAYOUT mem_layout_setting[MEM_LAYOUT_NUM];
//static VIDEO_BLK_INFO Video_info_default[VIDEO_INFO_END];

extern CACHE_MNG Cache_mng_default[VIDEO_INFO_END];

/**
 * @brief	Reset video frame
 * @param	"VIDEO_FRAME *pframe" : video frame
 * @return	none
 */
void Video_Frame_reset( VIDEO_FRAME *pframe)
{
	int cnt = 0;

	pframe->fram_type	= EMPTY_FRAME;
	pframe->serial 		= -1;
	pframe->blkindex	= -1;
	pframe->blks		= -1;
	pframe->realsize	= -1;
	pframe->flag		= 0;
	pframe->timestamp	= 0;
	for( cnt = 0; cnt < VIDEO_INFO_END; cnt++ )
	{
		pframe->ref_serial[cnt] = -1;
	}
}

/**
 * @brief	Video memory allocate
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
static int Video_Mem_Malloc( MEM_MNG_INFO *pInfo)
{
	int	cnt = 0;
	int cnt2 = 0;

	for( cnt = 0; cnt < pInfo->video_info_nums ; cnt++ )
	{
		pInfo->video_info[cnt].start 		= pInfo->start_addr + pInfo->offset;
		pInfo->video_info[cnt].start_phy 	= pInfo->start_phyAddr + pInfo->offset;
		pInfo->video_info[cnt].frame_num	= pInfo->video_info[cnt].blk_num;
		pInfo->video_info[cnt].frame		= calloc(pInfo->video_info[cnt].blk_num, sizeof(VIDEO_FRAME) );

		for( cnt2 = 0; cnt2 < pInfo->video_info[cnt].blk_num ; cnt2++ )
		{
			Video_Frame_reset( &(pInfo->video_info[cnt].frame[cnt2]) );
		}

		pInfo->freesize -= pInfo->video_info[cnt].size;
		pInfo->offset	+= pInfo->video_info[cnt].size;

		if( pInfo->video_info[cnt].extrasize > 0 )
		{
			pInfo->video_info[cnt].extradata = (void *)(pInfo->start_addr + pInfo->offset);
			pInfo->freesize -= pInfo->video_info[cnt].extrasize;
			pInfo->offset	+= pInfo->video_info[cnt].extrasize;
		}

		if( pInfo->freesize < 0 || pInfo->video_info[cnt].frame == NULL)
		{
			eprintf_rt("pInfo->freesize = %d\n", pInfo->freesize);
			eprintf_rt("Video_Mem_Malloc FAIL\n");
				return -1;
		}
	}

	return 0;
}

/**
 * @brief	Free video memory
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int Video_Mem_Free( MEM_MNG_INFO *pInfo )
{
	int	cnt = 0;

	if(pInfo->video_info == NULL) {
		return -1;
	}

	for( cnt = 0; cnt < pInfo->video_info_nums ; cnt++ )
	{
		if( pInfo->video_info[cnt].frame )
			free(pInfo->video_info[cnt].frame);
	}

	return 0;
}

/**
 * @brief	Memory manager initialization
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Init( MEM_MNG_INFO *pInfo )
{
	int cnt;

	memset( pInfo, 0 , sizeof(MEM_MNG_INFO) );
	pInfo->video_info = calloc(VIDEO_INFO_END, sizeof(VIDEO_BLK_INFO));

	for(cnt = 0; cnt < VIDEO_INFO_END; cnt++)
	{
		if(cnt >= AUDIO_INFO_G711_CH1)
		{
			pInfo->video_info[cnt].video_type 	= AUDIO_G711;
			pInfo->video_info[cnt].size			= AUDIO_MEM_SIZE;
			pInfo->video_info[cnt].extradata	= NULL;
			pInfo->video_info[cnt].extrasize	= 0;
			pInfo->video_info[cnt].IsCache		= 0;
			pInfo->video_info[cnt].blk_sz		= AUDIO_BLK_SIZE;
			pInfo->video_info[cnt].blk_num		= (AUDIO_MEM_SIZE/AUDIO_BLK_SIZE);
			pInfo->video_info[cnt].blk_free		= (AUDIO_MEM_SIZE/AUDIO_BLK_SIZE);
			pInfo->video_info[cnt].frame_num	= 0;
			pInfo->video_info[cnt].cur_frame	= -1;
			pInfo->video_info[cnt].cur_serial 	= -1;
			pInfo->video_info[cnt].cur_blk		= -1;
			pInfo->video_info[cnt].timestamp	= -1;
			pInfo->video_info[cnt].frame		= NULL;
			pInfo->video_info[cnt].gop.last_Start 			= -1;
			pInfo->video_info[cnt].gop.last_Start_serial 	= -1;
			pInfo->video_info[cnt].gop.last_End 			= -1;
			pInfo->video_info[cnt].gop.last_End_serial 	= -1;
			pInfo->video_info[cnt].gop.lastest_I 			= -1;
			pInfo->video_info[cnt].gop.lastest_I_serial 	= -1;

			Cache_mng_default[cnt].video_type 	= AUDIO_G711;
			Cache_mng_default[cnt].start_addr	= 0;
			Cache_mng_default[cnt].start_phy	= 0;
			Cache_mng_default[cnt].size			= AUDIO_CACHE_SIZE;
			Cache_mng_default[cnt].blk_sz		= AUDIO_CACHE_BLK_SIZE;
			Cache_mng_default[cnt].blk_num		= (AUDIO_CACHE_SIZE/AUDIO_CACHE_BLK_SIZE);
			Cache_mng_default[cnt].cache_num	= (AUDIO_CACHE_SIZE/AUDIO_CACHE_BLK_SIZE);
			Cache_mng_default[cnt].cache 		= NULL;
			Cache_mng_default[cnt].blk			= NULL;
		}
		else
		{
            if(cnt == 0)	//# ch0
            {
                pInfo->video_info[cnt].video_type 	= VIDEO_MP4;
                pInfo->video_info[cnt].size			= MP4_MEM_SIZE*2;
                pInfo->video_info[cnt].extradata	= NULL;
                pInfo->video_info[cnt].extrasize	= MP4_2_EXTRA_SIZE;
                pInfo->video_info[cnt].IsCache		= 0;
                pInfo->video_info[cnt].blk_sz		= MP4_BLK_SIZE*2;
                pInfo->video_info[cnt].blk_num		= ((MP4_MEM_SIZE*2)/(MP4_BLK_SIZE*2));
                pInfo->video_info[cnt].blk_free		= ((MP4_MEM_SIZE*2)/(MP4_BLK_SIZE*2));
                pInfo->video_info[cnt].frame_num	= 0;
                pInfo->video_info[cnt].cur_frame	= -1;
                pInfo->video_info[cnt].cur_serial 	= -1;
                pInfo->video_info[cnt].cur_blk		= -1;
                pInfo->video_info[cnt].timestamp	= -1;
                pInfo->video_info[cnt].frame		= NULL;
                pInfo->video_info[cnt].gop.last_Start 			= -1;
                pInfo->video_info[cnt].gop.last_Start_serial 	= -1;
                pInfo->video_info[cnt].gop.last_End 			= -1;
                pInfo->video_info[cnt].gop.last_End_serial 	= -1;
                pInfo->video_info[cnt].gop.lastest_I 			= -1;
                pInfo->video_info[cnt].gop.lastest_I_serial 	= -1;

                Cache_mng_default[cnt].video_type	= VIDEO_MP4;
                Cache_mng_default[cnt].start_addr	= 0;
                Cache_mng_default[cnt].start_phy	= 0;
                Cache_mng_default[cnt].size			= MP4_CACHE_SIZE*2;
                Cache_mng_default[cnt].blk_sz		= MP4_CACHE_BLK_SIZE*2;
                Cache_mng_default[cnt].blk_num		= ((MP4_CACHE_SIZE*2)/(MP4_CACHE_BLK_SIZE*2));
                Cache_mng_default[cnt].cache_num	= ((MP4_CACHE_SIZE*2)/(MP4_CACHE_BLK_SIZE*2));
                Cache_mng_default[cnt].cache 		= NULL;
                Cache_mng_default[cnt].blk 			= NULL;
            }
            else			//# ch1
            {

                pInfo->video_info[cnt].video_type 	= VIDEO_MP4;
                pInfo->video_info[cnt].size			= MP4_MEM_SIZE / 4;
                pInfo->video_info[cnt].extradata	= NULL;
                pInfo->video_info[cnt].extrasize	= MP4_2_EXTRA_SIZE;
                pInfo->video_info[cnt].IsCache		= 0;
                pInfo->video_info[cnt].blk_sz		= MP4_BLK_SIZE;
                pInfo->video_info[cnt].blk_num		= (MP4_MEM_SIZE/MP4_BLK_SIZE);
                pInfo->video_info[cnt].blk_free		= (MP4_MEM_SIZE/MP4_BLK_SIZE);
                pInfo->video_info[cnt].frame_num	= 0;
                pInfo->video_info[cnt].cur_frame	= -1;
                pInfo->video_info[cnt].cur_serial 	= -1;
                pInfo->video_info[cnt].cur_blk		= -1;
                pInfo->video_info[cnt].timestamp	= -1;
                pInfo->video_info[cnt].frame		= NULL;
                pInfo->video_info[cnt].gop.last_Start 			= -1;
                pInfo->video_info[cnt].gop.last_Start_serial 	= -1;
                pInfo->video_info[cnt].gop.last_End 			= -1;
                pInfo->video_info[cnt].gop.last_End_serial 	= -1;
                pInfo->video_info[cnt].gop.lastest_I 			= -1;
                pInfo->video_info[cnt].gop.lastest_I_serial 	= -1;

                Cache_mng_default[cnt].video_type	= VIDEO_MP4;
                Cache_mng_default[cnt].start_addr	= 0;
                Cache_mng_default[cnt].start_phy	= 0;
                Cache_mng_default[cnt].size			= MP4_CACHE_SIZE / 4;
                Cache_mng_default[cnt].blk_sz		= MP4_CACHE_BLK_SIZE;
                Cache_mng_default[cnt].blk_num		= (MP4_CACHE_SIZE/MP4_CACHE_BLK_SIZE);
                Cache_mng_default[cnt].cache_num	= (MP4_CACHE_SIZE/MP4_CACHE_BLK_SIZE);
                Cache_mng_default[cnt].cache 		= NULL;
                Cache_mng_default[cnt].blk 			= NULL;
            }
		}

		pInfo->totalsize += pInfo->video_info[cnt].size;
		pInfo->totalsize += pInfo->video_info[cnt].extrasize;
		pInfo->totalsize += Cache_mng_default[cnt].size;
	}

	#ifndef USE_GMEM

	int shmem_id;
	
	pInfo->shmem_key	= SHMEM_KEY;
	shmem_id = shmget((key_t)pInfo->shmem_key, pInfo->totalsize, 0666|IPC_CREAT);
	if(shmem_id == -1) {
		eprintf_rt(" MEM_INIT : shared memory fail shmem_id[%d] \n",shmem_id);
		goto MEM_INIT_FAIL;
	}

	pInfo->start_addr = (unsigned long)shmat(shmem_id, 0, 0);

	if( pInfo->start_addr == 0 ){
		shmctl(shmem_id, IPC_RMID, 0);
		goto MEM_INIT_FAIL;
	}

	dprintf("done... totalsize %d\n", pInfo->totalsize);
	#endif

	pInfo->freesize			= pInfo->totalsize;
	pInfo->offset			= 0;
	pInfo->video_info_nums	= VIDEO_INFO_END;

	if(Video_Mem_Malloc(pInfo) < 0) {
		goto MEM_INIT_FAIL;
    }

	return 0;

MEM_INIT_FAIL:
	eprintf_rt(" MEM_INIT_FAIL \n");
	return -1;
}

/**
 * @brief	Release memory manager
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_release( MEM_MNG_INFO *pInfo )
{
	Video_Mem_Free(pInfo);

	if(pInfo->video_info)
	{
		free(pInfo->video_info);
		pInfo->video_info = NULL;
	}

	return 0;
}

/**
 * @brief	Free video frame
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @param	"int frame_id" : frame ID
 * @return	0 : success ; -1 : fail
 */
int MemMng_VidFrame_Free( VIDEO_BLK_INFO *pVidInfo, int frame_id)
{
	int blkStart = -1;

	if( frame_id >= pVidInfo->frame_num || frame_id < 0 )
	{
		eprintf_rt("invalidate frame_id\n");
		return -1;
	}

	if( pVidInfo->frame[frame_id].fram_type == EMPTY_FRAME ) {
		return 0;
	}

	if( pVidInfo->frame[frame_id].blks > 0 )
	{
		pVidInfo->blk_free += pVidInfo->frame[frame_id].blks;
		blkStart = pVidInfo->frame[frame_id].blkindex;
	}

	if( pVidInfo->gop.last_Start > 0 )
	{
		if( pVidInfo->gop.last_Start == frame_id )
		{
			pVidInfo->gop.last_Start		= -1;
			pVidInfo->gop.last_Start_serial	= -1;
			pVidInfo->gop.last_End			= -1;
			pVidInfo->gop.last_End_serial	= -1;
		}
	}

	if( pVidInfo->gop.lastest_I > 0 )
	{
		if( pVidInfo->gop.lastest_I == frame_id )
		{
			pVidInfo->gop.lastest_I = -1;
			pVidInfo->gop.last_End_serial = -1;

		}
	}

	Video_Frame_reset( &(pVidInfo->frame[frame_id]) );

	return blkStart;
}

/**
 * @brief	Set flag to video frame
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	none
 */
void MemMng_VidFrame_SetFlag(VIDEO_BLK_INFO *pVidInfo)
{
	int last_frame = 0;
	int frame_type = pVidInfo->frame[pVidInfo->cur_frame].fram_type;

	if(  frame_type == I_FRAME && pVidInfo->video_type == VIDEO_MP4 )
	{
		pVidInfo->frame[pVidInfo->cur_frame].flag = 0x0001;

		/* check if the first frame */
		if( ( pVidInfo->cur_frame - 1 ) > 0 )
		{
			last_frame = pVidInfo->cur_frame - 1;
		}else{
			last_frame = pVidInfo->frame_num - 1;
		}
		if( pVidInfo->frame[last_frame].fram_type == P_FRAME ||
			pVidInfo->frame[last_frame].fram_type == B_FRAME 	)
		{
			pVidInfo->frame[last_frame].flag = 0x0002;
		}

	}
	else
	{
		pVidInfo->frame[pVidInfo->cur_frame].flag = 0;
	}

}

void printheader(char *pData, int size)
{
	int cnt = 0;

	dprintf("printheader = %d\n", size);
	for(cnt=0; cnt<size; cnt++)
	{
		dprintf(" 0x%X ",*pData++);
		if( cnt!=0 && cnt%4 == 0 )
			printf("\n");
	}
}

/**
 * @brief	Insert video frame
 * @param	"void *pData"
 * @param	"int size"
 * @param	"int blks"
 * @param	"int frame_type"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
static int MemMng_VidFrame_Insert( void *pData, int size, int blks,
							int frame_type, VIDEO_BLK_INFO *pVidInfo)
{
	if( pVidInfo == NULL ||
		frame_type >= END_FRAME_TYPE || blks <= 0 )
	{
		return -1;
	}
	if( frame_type != DUMMY_FRAME )
	{
		pVidInfo->cur_serial++;
	}
	pVidInfo->cur_blk	= ( pVidInfo->cur_blk+1 ) % pVidInfo->blk_num;
	pVidInfo->cur_frame = ( pVidInfo->cur_frame+1 )% pVidInfo->frame_num;

	pVidInfo->frame[pVidInfo->cur_frame].serial		= pVidInfo->cur_serial;
	pVidInfo->frame[pVidInfo->cur_frame].fram_type	= frame_type;
	pVidInfo->frame[pVidInfo->cur_frame].blkindex	= pVidInfo->cur_blk;
	pVidInfo->frame[pVidInfo->cur_frame].blks		= blks;
	pVidInfo->frame[pVidInfo->cur_frame].realsize	= size;
	pVidInfo->frame[pVidInfo->cur_frame].timestamp	= pVidInfo->timestamp;
	pVidInfo->frame[pVidInfo->cur_frame].offset		= (unsigned int)pData;

	if( frame_type != DUMMY_FRAME )
	{
		#ifndef USE_GMEM
		unsigned long WriteAddr = pVidInfo->start + pVidInfo->cur_blk*pVidInfo->blk_sz;
		if( !WriteAddr || pData == NULL ) {
			eprintf_rt("MemMng_VidFrame_Insert fail!!\n");
			return -1;
		}

		memcpy( (void *)WriteAddr, pData, size );
		#endif
	}

	if( frame_type == I_FRAME && pVidInfo->video_type == VIDEO_MP4)
	{
		if( pVidInfo->gop.last_Start < 0 )
		{
			pVidInfo->gop.last_Start =
					pVidInfo->gop.lastest_I =
									pVidInfo->cur_frame;

			pVidInfo->gop.last_Start_serial =
					pVidInfo->gop.lastest_I_serial =
									pVidInfo->cur_serial;

			pVidInfo->gop.last_End =
					pVidInfo->gop.last_End_serial = -1;

		}
		else
		{
			pVidInfo->gop.last_Start = pVidInfo->gop.lastest_I;
			pVidInfo->gop.last_Start_serial = pVidInfo->gop.lastest_I_serial;
			if( (pVidInfo->cur_frame - 1) > 0 )
			{
				pVidInfo->gop.last_End = pVidInfo->cur_frame - 1;
			}else{
				pVidInfo->gop.last_End = pVidInfo->frame_num - 1;
			}
			pVidInfo->gop.last_End_serial = pVidInfo->cur_serial - 1;

			pVidInfo->gop.lastest_I = pVidInfo->cur_frame;
			pVidInfo->gop.lastest_I_serial = pVidInfo->cur_serial;

		}
	}

	if(pVidInfo->cur_serial == 0 && frame_type != DUMMY_FRAME)
	{
		#ifndef USE_GMEM
		if(pVidInfo->extradata != NULL && pVidInfo->extrasize != 0)
		{
			memcpy(pVidInfo->extradata, pData,pVidInfo->extrasize);
			//printheader(pData,pVidInfo->extrasize);
		}
		#endif
	}

	MemMng_VidFrame_SetFlag(pVidInfo);

	/* Update  global info */
	pVidInfo->cur_blk = (pVidInfo->cur_blk + blks - 1) % pVidInfo->blk_num;
	pVidInfo->blk_free -= blks;

	return 0;
}

/**
 * @brief	Video write
 * @param	"void *pData"
 * @param	"int size"
 * @param	"int frame_type"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Video_Write( void *pData, int size, int frame_type, VIDEO_BLK_INFO *pVidInfo)
{
	int blk_use = (size + pVidInfo->blk_sz-1) / pVidInfo->blk_sz;
	int IsContiguousBlk = 0;
	int	free_cnt = 0;

	if( blk_use > pVidInfo->blk_num || size == 0 ) {
		goto MEM_MNG_WRITE_FAIL;
	}

	do {

		if( blk_use > pVidInfo->blk_free )
		{
			int nextframe = pVidInfo->cur_frame + 1;
			int freeframe = (nextframe + free_cnt)%pVidInfo->frame_num;
			/* Not enough free blk */
			if( MemMng_VidFrame_Free(  pVidInfo,  freeframe) < 0 )
			{
				goto MEM_MNG_WRITE_FAIL;
			}else{
				free_cnt++;
				continue;
			}
		}
		else
		{
			int nextblk = (pVidInfo->cur_blk + 1)%pVidInfo->blk_num;
			if( (nextblk+ blk_use ) > pVidInfo->blk_num )
			{
				/* No Contiguous Blk */
				int dummy_blk_cnt = pVidInfo->blk_num - nextblk;
				if( MemMng_VidFrame_Insert( NULL, 0, dummy_blk_cnt, DUMMY_FRAME, pVidInfo) < 0 )
				{
					goto MEM_MNG_WRITE_FAIL;
				}else{
					free_cnt = 0;
					continue;
				}

			}
			else
			{
				IsContiguousBlk = 1;
			}

		}

	} while( !IsContiguousBlk );

	if( MemMng_VidFrame_Insert( pData, size, blk_use, frame_type, pVidInfo) < 0 ) {
		goto MEM_MNG_WRITE_FAIL;
	}

	return 0;

MEM_MNG_WRITE_FAIL:
	eprintf_rt("MEM_MNG_WRITE_FAIL \n");
	return -1;
}


/**
 * @brief	Video read frame
 * @param	"void *pDest" : destination address
 * @param	"int *pSize" : size
 * @param	"int *pFrm_type" : frame type
 * @param	"int bufflimit" : buffer limit
 * @param	"VIDEO_FRAME *pFrame"
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	0 : success ; -1 : fail
 */
int MemMng_Video_ReadFrame( 	void *pDest, int *pSize, int *pFrm_type , int bufflimit,
								VIDEO_FRAME *pFrame, VIDEO_BLK_INFO *pVidInfo )
{
	if( pDest == NULL || pVidInfo == NULL || pFrame == NULL )
	{
		eprintf_rt("MemMng_Video_ReadFrame: invalidate pointer\n");
		return -1;
	}

	if( pFrame->fram_type == DUMMY_FRAME || pFrame->fram_type == EMPTY_FRAME )
	{
		eprintf_rt("MemMng_Video_ReadFrame: invalidate frame_type\n");
		return -1;
	}

	if( pFrame->realsize > bufflimit )
	{
		eprintf_rt("MemMng_Video_ReadFrame: Over bufflimit\n");
		return -1;
	}

	void *pSrc = (void *)(pVidInfo->start +
							pFrame->blkindex*pVidInfo->blk_sz);

	*pSize 		= pFrame->realsize;
	*pFrm_type 	= pFrame->fram_type;

	memcpy( pDest, pSrc, pFrame->realsize );


	return 0;
}

/**
 * @brief	Get video frame by serial
 * @param	"int serial" : serial no.
 * @param	"VIDEO_BLK_INFO *pVidInfo"
 * @return	frame address ; NULL
 */
VIDEO_FRAME * MemMng_GetFrameBySerial( int serial , VIDEO_BLK_INFO *pVidInfo)
{
	int cnt = 0;

	if( serial < 0 || pVidInfo == NULL || serial > pVidInfo->cur_serial) {
		//eprintf_rt("fail \n");
		return NULL;
	}

	if( serial == pVidInfo->frame[pVidInfo->cur_frame].serial ) {
		return &(pVidInfo->frame[pVidInfo->cur_frame]);
	}

	for( cnt = 0; cnt < pVidInfo->frame_num ; cnt++ )
	{
		if( serial == pVidInfo->frame[cnt].serial )
		{
			if( pVidInfo->frame[cnt].fram_type == DUMMY_FRAME ||
				pVidInfo->frame[cnt].fram_type == EMPTY_FRAME )
			{
				//eprintf_rt("MemMng_GetFrameBySerial: invalidate frame_type\n");
				//return NULL;
				continue;
			}
			return &(pVidInfo->frame[cnt]);
		}
	}

	dprintf("MemMng_GetFrameBySerial: Search no frame\n");

	return NULL;
}

/**
 * @brief	Get physical base address
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	physical base address
 */
unsigned long GetMemMngPhyBaseAddr(MEM_MNG_INFO *pInfo)
{
	if(pInfo == NULL)
		return 0;
	return pInfo->start_phyAddr;
}

/**
 * @brief	Get shared memory base address
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	shared memory base address
 */
unsigned long GetMemMngShmemKey(MEM_MNG_INFO *pInfo)
{
	if(pInfo == NULL)
		return 0;
	return pInfo->shmem_key;
}

/**
 * @brief	Get total memory size
 * @param	"MEM_MNG_INFO *pInfo"
 * @return	totalsize
 */
unsigned long GetMemMngTotalMemSize(MEM_MNG_INFO *pInfo)
{
	if(pInfo == NULL)
		return 0;
	return pInfo->totalsize;
}

/**
 * @brief	Get current serial number
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current serial number
 */
int GetCurrentSerialNo(VIDEO_BLK_INFO *video_info)
{
	return video_info->cur_serial;
}

/**
 * @brief	Get current frame
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current frame
 */
VIDEO_FRAME * GetCurrentFrame(VIDEO_BLK_INFO *video_info)
{
	if(video_info == NULL)
		return NULL;
	if(video_info->cur_frame < 0)
		return NULL;
	return &(video_info->frame[video_info->cur_frame]);
}

/**
 * @brief	Get current frame
 * @param	"VIDEO_BLK_INFO *video_info"
 * @return	current frame
 */
VIDEO_FRAME * GetLastI_Frame(VIDEO_BLK_INFO *video_info)
{
	if(video_info == NULL)
		return NULL;
	if(video_info->gop.lastest_I < 0)
		return NULL;
	return &(video_info->frame[video_info->gop.lastest_I]);
}
