// AVIEncoder.cpp: implementation of the CAVIEncoder class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>

#include <stdlib.h> 
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

#include "avirecover.hh"

#define NAL_HEADER_SIZE	4

static int readHdrCnt = 0;
static BITMAPINFOHEADER* pVidFormat;
static WAVEFORMATEX* pAudFormat;

static char errMsg[50];
char fccType[4];

static char* convert_fcc(unsigned long fcc)
{
	int i;
	unsigned long fcctmp = fcc;

	for(i = 0; i < 4; i++)
	{
		fccType[i] = fcctmp&0x000000FF;
		fcctmp = fcctmp >> 8;
	}

	return fccType;
}

static void print_mainhdr(AVIHEADBLOCK* pMainHdr)
{
	printf("=======================\n");
	printf("     AVIMAINHEADER     \n");
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

static void print_strmhdr(AVISTREAM* pStream)
{
	printf("===========================\n");
	printf("      AVISTREAMHEADER      \n");
	printf("===========================\n");
	convert_fcc(pStream->pStreamBlock->AviStrHeader.fccType);
	printf("fccType		: %c%c%c%c\n", fccType[0],fccType[1],fccType[2],fccType[3]);
	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeVIDEO){
		convert_fcc(pStream->pStreamBlock->AviStrHeader.fccHandler);
		printf("fccHandler	: %c%c%c%c\n", fccType[0],fccType[1],fccType[2],fccType[3]);
	}
	else
		printf("fccHandler	: %ld\n", pStream->pStreamBlock->AviStrHeader.fccHandler);
	printf("Flags		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwFlags);
	printf("Priority	: %ld\n", pStream->pStreamBlock->AviStrHeader.dwPriority);
	printf("InitFrames	: %ld\n", pStream->pStreamBlock->AviStrHeader.dwInitialFrames);
	printf("Scale		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwScale);
	printf("Rate		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwRate);
	printf("Start		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwStart);
	printf("Length		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwLength);
	printf("SuggestBuf	: %ld\n", pStream->pStreamBlock->AviStrHeader.dwSuggestedBufferSize);
	printf("Quality		: %ld\n", pStream->pStreamBlock->AviStrHeader.dwQuality);
	printf("SmapleSize	: %ld\n", pStream->pStreamBlock->AviStrHeader.dwSampleSize);
	printf("rcFrmae[0]	: %d\n", pStream->pStreamBlock->AviStrHeader.rcFrame[0]);
	printf("rcFrame[1]	: %d\n", pStream->pStreamBlock->AviStrHeader.rcFrame[1]);
	printf("rcFrame[2]	: %d\n", pStream->pStreamBlock->AviStrHeader.rcFrame[2]);
	printf("rcFrame[3]	: %d\n", pStream->pStreamBlock->AviStrHeader.rcFrame[3]);
	printf("\n\n");
	
	printf("===========================\n");
	printf("     AVISTREAMFORMAT	   \n");
	printf("===========================\n");
	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeVIDEO)
	{
		printf("Size	:%ld\n", pVidFormat->biSize);
		printf("Width	:%ld\n", pVidFormat->biWidth);
		printf("Height	:%ld\n", pVidFormat->biHeight);
		printf("Plans	:%d\n", pVidFormat->biPlanes);
		printf("bitCnt	:%d\n", pVidFormat->biBitCount);
		printf("Compress:%s\n", convert_fcc(pVidFormat->biCompression));
		printf("SizeBuf	:%ld\n", pVidFormat->biSizeImage);
		printf("XMeter	:%ld\n", pVidFormat->biXPelsPerMeter);
		printf("YMeter	:%ld\n", pVidFormat->biYPelsPerMeter);
		printf("ClrUsed	:%ld\n", pVidFormat->biClrUsed);
		printf("ClrImp	:%ld\n", pVidFormat->biClrImportant);
	}
	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeAUDIO)
	{
		printf("formTag         : %d\n", pAudFormat->wFormatTag);
		printf("Channels        : %d\n", pAudFormat->nChannels);
		printf("SamplePerSec    : %ld\n", pAudFormat->nSamplesPerSec);
		printf("AvgBytesPerSec  : %ld\n", pAudFormat->nAvgBytesPerSec);
		printf("BlockAlign      : %d\n", pAudFormat->nBlockAlign);
		printf("BitsPerSample   : %d\n", pAudFormat->wBitsPerSample);
		printf("cbSize          : %d\n", pAudFormat->cbSize);
	}
	printf("\n\n");
}

static int readStreamHeader(AVIFILE* pAVIFile, AVISTREAM* pStream)
{

	pStream->pStreamBlock = (AVISTREAMBLOCK*) malloc(sizeof(AVISTREAMBLOCK));
	if(NULL == pStream->pStreamBlock)
	{
		DBG2_LOG("Can't Create Stream Block\n");
		goto err_exit;
	}

	// Set AVISTREAMBLOCK //
	pStream->pAVIFile = pAVIFile;

	fread(&pStream->pStreamBlock->ck_strl, sizeof(CK_LIST), 1, pAVIFile->pFile);
	if(pStream->pStreamBlock->ck_strl.ckSize == 0 || pStream->pStreamBlock->ck_strl.ckType != listtypeSTREAMHEADER){
		sprintf(errMsg, "Invalid strl header size[%ld] ckType[0x%x].\n",pStream->pStreamBlock->ck_strl.ckSize,(int)pStream->pStreamBlock->ck_strl.ckType);
		goto err_exit;
	}

	fread(&pStream->pStreamBlock->ck_strh, sizeof(CK), 1, pAVIFile->pFile);
	if(pStream->pStreamBlock->ck_strh.ckID != ckidSTREAMHEADER || pStream->pStreamBlock->ck_strh.ckSize == 0){
		sprintf(errMsg, "Invalid strh header size[%ld] ckID[0x%x].\n",pStream->pStreamBlock->ck_strh.ckSize,(int)pStream->pStreamBlock->ck_strh.ckID);	
		goto err_exit;
	}

	fread(&pStream->pStreamBlock->AviStrHeader, sizeof(AVISTREAMHEADER), 1, pAVIFile->pFile);
	
	fread(&pStream->pStreamBlock->ck_strf, sizeof(CK), 1, pAVIFile->pFile);
	if(pStream->pStreamBlock->ck_strf.ckID != ckidSTREAMFORMAT){
		sprintf(errMsg, "Invalid strf header ckID[0x%x].\n",(int)pStream->pStreamBlock->ck_strf.ckID);		
		goto err_exit;
	}

	if(pStream->pStreamBlock->AviStrHeader.fccType != streamtypeTEXT && pStream->pStreamBlock->ck_strf.ckSize == 0){
		sprintf(errMsg, "Invalid strf header Size[%ld].\n",pStream->pStreamBlock->ck_strf.ckSize);
		goto err_exit;
	}

	if(pStream->pStreamBlock->ck_strf.ckSize != 0)
	{
		if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeVIDEO)
		{
			pVidFormat = (BITMAPINFOHEADER*)malloc(pStream->pStreamBlock->ck_strf.ckSize);
			if(NULL == pVidFormat)
			{
				DBG2_LOG("Can't Create Stream Format\n");
				goto err_exit;
			}

			fread(pVidFormat, pStream->pStreamBlock->ck_strf.ckSize, 1, pAVIFile->pFile);
		}
			
		else if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeAUDIO)
		{
			pAudFormat = (WAVEFORMATEX*)malloc(pStream->pStreamBlock->ck_strf.ckSize);	

			if(NULL == pAudFormat)
			{
				DBG2_LOG("Can't Create Stream Format\n");
				goto err_exit;
			}

			fread(pAudFormat, pStream->pStreamBlock->ck_strf.ckSize, 1, pAVIFile->pFile);
		}
	}

	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeVIDEO)
		pStream->ck_data.ckID = mmioFOURCC('0', '0'+readHdrCnt, 'd', 'c');

	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeAUDIO)
		pStream->ck_data.ckID = mmioFOURCC('0', '0'+readHdrCnt, 'w', 'b');

	if(pStream->pStreamBlock->AviStrHeader.fccType == streamtypeTEXT)
		pStream->ck_data.ckID = mmioFOURCC('0', '0'+readHdrCnt, 't', 'x');

	++readHdrCnt;
	
	return PROC_SUCCESS;


err_exit:
	
	if(pStream->pStreamBlock)
		free(pStream->pStreamBlock);

	if(pVidFormat)
		free(pVidFormat);

	if(pAudFormat)
		free(pAudFormat);

	return PROC_ERROR;
}

static int recoveryStart(AVIFILE* pAVIFile, AVISTREAM* pStream)
{
	int done = 0, isFirst = 1, i;
	int dwFlags = AVIIF_LIST;
	unsigned long seekOffset = 0;
	
	pAVIFile->nOffset = 4;

	DBG2_LOG("recoveryStart() run!! \n");
	while(!done)
	{
		CK ckData;
		if(fread(&ckData, sizeof(CK), 1, pAVIFile->pFile) == 0)
		{
			if(isFirst)
			{
				sprintf(errMsg, "Empty recorded data.\n");
				return PROC_ERROR;
			}
			else
				done = 1;
		}

		if(isFirst) isFirst = 0;

		if(!done)
		{
			AVISTREAM* pavi=NULL;
			int find_strm = 0;
			
			if(ckData.ckSize !=0)
			{
				for(i = 0; i < readHdrCnt; i++)
				{
					pavi = &pStream[i];
					if(ckData.ckID == pavi->ck_data.ckID){
						find_strm = 1;
						break;
					}
				}

//				AVISTREAM* pavi = &pStream[i];

				seekOffset = ckData.ckSize;
				if(pavi->pStreamBlock->AviStrHeader.fccType == streamtypeVIDEO && find_strm)
				{
					unsigned char checkStream[5];
					dwFlags = 0;
//					fseek(pAVIFile->pFile, NAL_HEADER_SIZE, SEEK_CUR);
					if(fread(checkStream, 5, 1, pAVIFile->pFile) == 0)
						break;

					if(checkStream[4] == 0x27)
						dwFlags = AVIIF_KEYFRAME;

					seekOffset = ckData.ckSize-5;
				}

				if(fseek(pAVIFile->pFile, seekOffset, SEEK_CUR) != 0){
					DBG2_LOG("Last read ckSize(0x%x) ckID[0x%x] \n", (int)ckData.ckSize, (int)ckData.ckID);
					done = 1;
					fseek(pAVIFile->pFile, 0, SEEK_SET);
					break;
				}

				// Update AVI header
				// Update SuggestedBuifferSize
				if(pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize < ckData.ckSize)
					pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize = ckData.ckSize;
	
				// Update total length
				++(pavi->pStreamBlock->AviStrHeader.dwLength);

				// Update movi_size
				pAVIFile->movi_size += sizeof(CK)+ckData.ckSize;

				// Make Index Entry
				INDEX* pIndex = (INDEX *) malloc(sizeof(INDEX));
				pIndex->next				= 0;
				pIndex->data.ckid			= pavi->ck_data.ckID;
				pIndex->data.dwFlags		= dwFlags;
				pIndex->data.dwChunkOffset	= pAVIFile->nOffset;

				pAVIFile->nOffset += (sizeof(CK) + ckData.ckSize);
				pIndex->data.dwChunkLength = ckData.ckSize;

				// Add Entry To Linked List
				if (!pAVIFile->IndexRoot.first)
					pAVIFile->IndexRoot.first = pIndex;
				else 
					pAVIFile->IndexRoot.last->next = pIndex;
				
				pAVIFile->IndexRoot.last = pIndex;

				// Update idx_cnt
				++(pAVIFile->idx_cnt);
			}
			else
				done = 1;
		}

		usleep(5);
	}

	DBG2_LOG("recovery Done!! \n");
	
	for(int i = 0; i < readHdrCnt; i++)
	{
		DBG2_LOG("Header info modify [%d]!! \n", i);
		AVISTREAM* pavi = &pStream[i];
		AVISTREAMBLOCK* pAVIStreamBlock = pavi->pStreamBlock;
		
		// Check file offset
		if(fseek(pAVIFile->pFile, pAVIFile->nHeadWritten, SEEK_SET) != 0){
			DBG2_LOG("File seek fail offset[%ld]!! \n",pAVIFile->nHeadWritten);
			return PROC_ERROR;
		}
		pAVIFile->nHeadWritten += ((sizeof(CK_LIST)-4) + pAVIStreamBlock->ck_strl.ckSize);

		if((streamtypeVIDEO == pavi->pStreamBlock->AviStrHeader.fccType))
		{
			pAVIStreamBlock->AviStrHeader.dwSuggestedBufferSize *= 10;

			if(pAVIFile->Head.avih.dwWidth == pAVIStreamBlock->AviStrHeader.rcFrame[2] &&
				pAVIFile->Head.avih.dwHeight == pAVIStreamBlock->AviStrHeader.rcFrame[3])
			{
				pAVIFile->Head.avih.dwSuggestedBufferSize = pAVIStreamBlock->AviStrHeader.dwSuggestedBufferSize;
				pAVIFile->Head.avih.dwTotalFrames = pAVIStreamBlock->AviStrHeader.dwLength;
			}
		}

		// Write LIST(strl) & strh
		if(fwrite(pAVIStreamBlock, 1, sizeof(AVISTREAMBLOCK) - sizeof(void*), pAVIFile->pFile) != sizeof(AVISTREAMBLOCK) - sizeof(void*)){
			DBG2_LOG("File write fail!! \n");
			return PROC_ERROR;
		}
	}

	if(fseek(pAVIFile->pFile, pAVIFile->nHeadWritten, SEEK_SET)){
		DBG2_LOG("File seek fail offset[%ld]!! \n",pAVIFile->nHeadWritten);
		return PROC_ERROR;
	}

	int nTotalSize = pAVIFile->nHeadWritten;

	// Write movi header
	pAVIFile->movi_size = ((pAVIFile->movi_size+3)>>2)<<2;
	pAVIFile->Tail.ck_movi.ckSize	= sizeof(DWORD) + pAVIFile->movi_size;
	if(fwrite(&pAVIFile->Tail.ck_movi, 1, sizeof(CK_LIST), pAVIFile->pFile) != sizeof(CK_LIST)){
		DBG2_LOG("File write fail!! \n");
		return PROC_ERROR;
	}
	nTotalSize += (pAVIFile->Tail.ck_movi.ckSize)+sizeof(CK);

	// Write idx1 header
	if(fseek(pAVIFile->pFile, nTotalSize, SEEK_SET) != 0){
		DBG2_LOG("File seek fail offset[%d]!! \n",nTotalSize);
		return PROC_ERROR;
	}
	pAVIFile->Tail.ck_idx1.ckSize = pAVIFile->idx_cnt * sizeof(AVIINDEXENTRY);
	if(fwrite(&pAVIFile->Tail.ck_idx1, 1, sizeof(CK), pAVIFile->pFile) != sizeof(CK)){
		DBG2_LOG("File write fail!! \n");
		return PROC_ERROR;
	}

	// Write index list
	INDEX *pIndex = pAVIFile->IndexRoot.first;
	int nCnt = 0;
	while(pIndex)
	{
		if(fwrite(&pIndex->data, 1, sizeof(AVIINDEXENTRY), pAVIFile->pFile) != sizeof(AVIINDEXENTRY)){
			DBG2_LOG("File write fail!! \n");
			return PROC_ERROR;
		}
		pIndex = pIndex->next;
		nCnt++;
	}
	
	nTotalSize += (sizeof(CK) + pAVIFile->Tail.ck_idx1.ckSize);

	// Write AVI header
	if(fseek(pAVIFile->pFile, 0, SEEK_SET) != 0){
		DBG2_LOG("File seek fail offset[0]!! \n");
		return PROC_ERROR;
	}
	pAVIFile->Head.ck_riff.ckSize	= nTotalSize - (sizeof(CK_RIFF)-4);
	if(fwrite(&pAVIFile->Head, 1, sizeof(AVIHEADBLOCK), pAVIFile->pFile) != sizeof(AVIHEADBLOCK)){
		DBG2_LOG("File write fail!! \n");
		return PROC_ERROR;
	}

	DBG2_LOG("Header info modified!! \n");

	return PROC_SUCCESS;
}

int AVIrcStart(const char* pszFileName)
{
	int i, status = PROC_SUCCESS;
	AVIFILE* pAVIFile = NULL;
	AVISTREAM* pAVIStream = NULL;

	readHdrCnt = 0;
	pVidFormat = NULL;
	pAudFormat = NULL;

	// Alloc object	
	pAVIFile = (AVIFILE*) malloc(sizeof(AVIFILE));
	memset(pAVIFile, 0, sizeof(AVIFILE));

	// Open file
	pAVIFile->pFile = fopen(pszFileName, "r+");
	if(!pAVIFile->pFile){
		DBG2_LOG("fopen() Fail!! \n");
		status = PROC_ERROR;
		goto err_exit;
	}

	fseek(pAVIFile->pFile, 0, SEEK_SET);
	
	//AVI MAIN HEADER//
	fread(&pAVIFile->Head, sizeof(AVIHEADBLOCK), 1, pAVIFile->pFile);
	if(pAVIFile->Head.ck_riff.ckID != formtypeRIFF || pAVIFile->Head.ck_riff.ckCodec != formtypeAVI ||
		pAVIFile->Head.ck_hdrl.ckType != listtypeAVIHEADER || pAVIFile->Head.ck_avih.ckID != ckidAVIMAINHDR)
	{
		sprintf(errMsg, "Not enough header information.\n");
		status = PROC_ERROR;
		goto err_exit;
	}

	// Set file header offset
	pAVIFile->nHeadWritten	= sizeof(AVIHEADBLOCK);

	print_mainhdr(&pAVIFile->Head);

	//AVI STREAM HEADER//
	if(pAVIFile->Head.avih.dwStreams == 0)
	{
		sprintf(errMsg, "Empty stream data.\n");
		status = PROC_ERROR;
		goto err_exit;
	}

	pAVIStream = (AVISTREAM*)malloc(sizeof(AVISTREAM)*pAVIFile->Head.avih.dwStreams);
	memset(pAVIStream, 0, sizeof(AVISTREAM)*pAVIFile->Head.avih.dwStreams);
	for(i = 0; i < (int)pAVIFile->Head.avih.dwStreams; i++)
	{
		if(readStreamHeader(pAVIFile, &pAVIStream[i]) < 0)
		{
			status = PROC_ERROR;
			goto err_exit;
		}

		print_strmhdr(&pAVIStream[i]);
	}

	//MOVI HEADER//
	fread(&pAVIFile->Tail.ck_movi, sizeof(CK_LIST), 1, pAVIFile->pFile);

	//Recovery Start//
//	if(pAVIFile->Head.ck_riff.ckSize == 0 || pAVIFile->Tail.ck_movi.ckSize == 0)
	{
		pAVIFile->Tail.ck_idx1.ckID		= ckidAVINEWINDEX;
		pAVIFile->Tail.ck_idx1.ckSize	= 0;

		status = recoveryStart(pAVIFile, pAVIStream);
	}	

		
err_exit:

	if(pAVIFile->pFile)
		fclose(pAVIFile->pFile);

	if(pAVIStream){
		
		for(i = 0; i < readHdrCnt; i++)
		{
			AVISTREAM* pavi = &pAVIStream[i];
			if(pavi->pStreamBlock)
				free(pavi->pStreamBlock);	
		}
		free(pAVIStream);
	}
	
	if(pVidFormat)
		free(pVidFormat);

	if(pAudFormat)
		free(pAudFormat);

	INDEX *pIFree;
	INDEX *pIndex = pAVIFile->IndexRoot.first;
	while(pIndex)
	{
		pIFree = pIndex;
		pIndex = pIndex->next;
		free(pIFree);
	}
	
	free(pAVIFile);
	
	return status;
}


char* AVIrcGetErrMsg()
{	
	return errMsg;
}

