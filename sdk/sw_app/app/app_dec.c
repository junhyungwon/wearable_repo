/******************************************************************************
 * XBX Board
 * Copyright by Linkflow, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_dec.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *     - 2023.01.25 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app_gmem.h"
#include "avi.h"
#include "app_main.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define PB_IDXBUF_SIZE                (1800*4) //# 1min, 2ch, audio
#define MAX_ENCODE_BYTE	              (5)

#define AVI_VIDEO	(0)
#define AVI_AUDIO	(1)
#define AVI_META    (2)


typedef struct {
	app_thr_obj *pObj;		//# playback thread

	int rate;
	int msec;
	int cur_idx;
	int icnt;
	int num_ch;
	char filepath[MAX_CHAR_128];
	AVIINDEXENTRY idx_buf[PB_IDXBUF_SIZE];
	AVIFile pAvi;

	app_thr_obj *aObj;

	unsigned char *enc_buf;
} app_pbk_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_pbk_t t_pbk;
static app_pbk_t *ipbk=NULL;
#ifdef DEBUG_META
extern app_gui_t *igui;
static app_meta_t gmeta;
#endif

AVIFile *pAvi=NULL;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Filelist function
-----------------------------------------------------------------------------*/
static void print_mainhdr(AVIHeadBlock* pMainHdr)
{
	printf("=======================\n");
	printf("     AVIMainHeader     \n");
	printf("=======================\n");
	printf("msecPerFrame : %ld\n", pMainHdr->avih.dwMicroSecPerFrame);
	printf("maxBytePerSec: %ld\n", pMainHdr->avih.dwMaxBytesPerSec);
	printf("flags        : %x\n", (int)pMainHdr->avih.dwFlags);
	printf("totFrames    : %ld\n", pMainHdr->avih.dwTotalFrames);
	printf("initFrames   : %ld\n", pMainHdr->avih.dwInitialFrames);
	printf("streams      : %ld\n", pMainHdr->avih.dwStreams);
	printf("suggestedBuff: %ld\n", pMainHdr->avih.dwSuggestedBufferSize);
	printf("width        : %ld\n", pMainHdr->avih.dwWidth);
	printf("height       : %ld\n", pMainHdr->avih.dwHeight);
	printf("scale        : %ld\n", pMainHdr->avih.dwScale);
	printf("rate         : %ld\n", pMainHdr->avih.dwRate);
	printf("start        : %ld\n", pMainHdr->avih.dwStart);
	printf("length       : %ld\n", pMainHdr->avih.dwLength);
	printf("\n\n");
}

static void convert_id(unsigned long fccType, int *ch, int *type)
{
	int i;
	char fccIdx[2];
	unsigned long fccTmp = fccType;
	int strmType;

	for(i = 0; i < 2; i++)
	{
		fccIdx[i] = fccTmp&0x000000FF;
		fccTmp = fccTmp >> 8;
	}

	if(fccTmp == cktypeDIBcompressed)
		strmType = AVI_VIDEO;
	else if(fccTmp == cktypeWAVEbytes)
		strmType = AVI_AUDIO;
    else if(fccTmp == cktypeTEXT)
        strmType = AVI_META;
	else
		strmType = -1;

	*ch = atoi(fccIdx);
	*type = strmType;

//	printf("%d, %s\n", atoi(fccIdx), strmType==AVI_VIDEO?"AVI_VIDEO":"AVI_AUDIO");
}

static int get_avi_info(AVIFile *pAvi, char *filepath)
{
	pAvi->pFile = fopen(filepath, "r+");
	if(pAvi->pFile == NULL) {
		return -1;
	}

	//#--- read avi main header
	fseek(pAvi->pFile, 0, SEEK_SET);
	fread(&pAvi->Head, sizeof(AVIHeadBlock), 1, pAvi->pFile);
	//print_mainhdr(&pAvi->Head);

	if(pAvi->Head.ck_riff.ckID != formtypeRIFF || pAvi->Head.ck_riff.ckCodec != formtypeAVI ||
		pAvi->Head.ck_hdrl.ckType != listtypeAVIHEADER || pAvi->Head.ck_avih.ckID != ckidAVIMAINHDR)
	{
        fclose(pAvi->pFile);
		return -1;
	}
	if(pAvi->Head.avih.dwStreams == 0 || pAvi->Head.avih.dwTotalFrames == 0) {
        fclose(pAvi->pFile);
		return -1;
	}

	//# set framerate
	ipbk->rate = pAvi->Head.avih.dwRate;
	ipbk->msec = (pAvi->Head.avih.dwMicroSecPerFrame/1000)-1;	//# process offset 1ms
//	printf("rate %d.%d, msec %d\n", (int)(ipbk->rate/pAvi->Head.avih.dwScale), (int)(ipbk->rate%pAvi->Head.avih.dwScale), ipbk->msec);

	//#--- read movi header
	pAvi->movi_pos = sizeof(CK_RIFF) + sizeof(CK) + pAvi->Head.ck_hdrl.ckSize;
	fseek(pAvi->pFile, pAvi->movi_pos, SEEK_SET);
	fread(&pAvi->Tail.ck_movi, sizeof(CK_LIST), 1, pAvi->pFile);

	if(pAvi->Tail.ck_movi.ckSize == 0 && pAvi->Tail.ck_movi.ckType != listtypeAVIMOVIE) {
		printf("Invalid movi header\n");
        fclose(pAvi->pFile);
		return -1;
	}

	//#--- read idx1 infomation
	pAvi->idx1_pos = sizeof(CK_RIFF) + sizeof(CK) + pAvi->Head.ck_hdrl.ckSize + sizeof(CK) + pAvi->Tail.ck_movi.ckSize;
	fseek(pAvi->pFile, pAvi->idx1_pos, SEEK_SET);
	fread(&pAvi->Tail.ck_idx1, sizeof(CK), 1, pAvi->pFile);

	if(pAvi->Tail.ck_idx1.ckID != ckidAVINEWINDEX)
	{
		printf("Invalid idx data\n");
        fclose(pAvi->pFile);
		return -1;
	}

//	pAvi->idx_entry = (AVIINDEXENTRY *)malloc(pAvi->Tail.ck_idx1.ckSize);
	pAvi->idx_entry = ipbk->idx_buf;
	pAvi->idx_cnt = pAvi->Tail.ck_idx1.ckSize/sizeof(AVIINDEXENTRY);
	
	if(pAvi->idx_cnt >= PB_IDXBUF_SIZE) {
		printf("Invalid idx count[%d]!!!\n", pAvi->idx_cnt);
        fclose(pAvi->pFile);
		return -1;		
	}

	fread(pAvi->idx_entry, pAvi->Tail.ck_idx1.ckSize, 1, pAvi->pFile);
	pAvi->strm_pos = pAvi->movi_pos+sizeof(CK_LIST)+4;		//# ???

	int i, chId, stream_type;
	ipbk->num_ch = 1;
    for(i = 0; i < pAvi->idx_cnt; i++)
    {
        convert_id(pAvi->idx_entry[i].ckid, &chId, &stream_type);
        if(chId == 0 && stream_type == AVI_VIDEO && pAvi->idx_entry[i].dwFlags)
            ipbk->icnt++;

		if(chId == 1 && stream_type == AVI_VIDEO && ipbk->num_ch == 1)
			ipbk->num_ch = 2;
    }
	
//    printf("@@@@@ I-Frm Count[%02d] num_ch[%d]@@@@@@@\n", ipbk->icnt, ipbk->num_ch);

	if(ipbk->icnt <= 0) {
        fclose(pAvi->pFile);
		printf("Invalid I-Frm count\n");
		return -1;
	}
		
	return 0;
}

static AVIINDEXENTRY *get_frame_idx(AVIFile *pAvi)
{
	AVIINDEXENTRY *idx=NULL;

	if(ipbk->cur_idx < pAvi->idx_cnt)
		idx = &pAvi->idx_entry[ipbk->cur_idx++];

	return idx;
}

static int change_data(char *byte)
{
	int data;

	data = *byte++ << 8;
	data |= *byte;

	return data;
}


/*****************************************************************************
* @brief    start decode function 
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_decode_process(char *file_path)
{
	int stream_id, stream_type ;
    char InBuff[6] ;
	unsigned long tvalue ;
	int retval = 0, i = 0, ret;
	long cur_pos ;

	AVIHeadBlock MAviHead ;
	ipbk = &t_pbk;

    AVIFile *pAvi = (AVIFile *)&ipbk->pAvi;
	AVIINDEXENTRY *idx ;

	memset(ipbk, 0, sizeof(app_pbk_t));
	ipbk->icnt = 0;

	//# init ipbk
	sprintf(ipbk->filepath,"%s", file_path);
	ipbk->cur_idx = 0;

    ret = get_avi_info(pAvi, ipbk->filepath);
	if(ret != 0) {
        printf("get_avi_info failed!!");
    }

	while(1)
	{
		idx = get_frame_idx(pAvi) ;
        if(idx != NULL)
		{
			convert_id(idx->ckid, &stream_id, &stream_type) ;
			fseek(pAvi->pFile, pAvi->strm_pos+idx->dwChunkOffset, SEEK_SET);

			if(stream_type == AVI_VIDEO)
			{
        		if(idx->dwFlags == 0x10) // Per Iframe
				{
					fread(InBuff, 1, MAX_ENCODE_BYTE, pAvi->pFile);
					cur_pos = ftell(pAvi->pFile) ;
					fseek(pAvi->pFile, cur_pos - MAX_ENCODE_BYTE, SEEK_SET);
					for(i = 0; i < MAX_ENCODE_BYTE; i++)
					{
						InBuff[i] = ~(InBuff[i]) ;
					}
					fwrite(InBuff, 1, MAX_ENCODE_BYTE, pAvi->pFile) ;
				}
			}
		}
		else
		    break ;

	}
		fclose(pAvi->pFile) ;
		sync() ;
		return 1 ;
}


