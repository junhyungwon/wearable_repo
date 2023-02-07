// AVIEncoder.cpp: implementation of the CAVIEncoder class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

#include "aviencoder.hh"

clock_t start_fwc, end_fwc;

int AVIFileOpen(AVIFILE** ppAVIFile, const char* pszFileName, int encrypt)
{
	// Alloc object
	AVIFILE* pAVIFile;
	pAVIFile = (AVIFILE*) malloc(sizeof(AVIFILE));
	if (pAVIFile == NULL) {
		return PROC_ERROR;
	}
	memset(pAVIFile, 0, sizeof(AVIFILE));

	printf("*************************************************************\n") ;
	DBG2_LOG("encrypt = %d\n",encrypt);
	printf("*************************************************************\n") ;
	// Open file
	#if 1
	pAVIFile->pFile = fopen(pszFileName, "wb");
	if(!pAVIFile->pFile){
		DBG2_LOG("fopen() Fail!!(fname is %s)\n", pszFileName);
		return PROC_ERROR;
	}
	#else
	int fd;

    fd = open(pszFileName, O_WRONLY|O_CREAT|O_TRUNC|O_CLOEXEC);
    if(fd < 0){
        DBG2_LOG("open() Fail!! \n");
        return PROC_ERROR;
    }
    pAVIFile->pFile = fdopen(fd, "wb");
    if(!pAVIFile->pFile){
        DBG2_LOG("fdopen() Fail!! \n");
        return PROC_ERROR;
    }
	#endif

	// Write AVI header
   // TTA Enrypion video data
	if(encrypt)
		pAVIFile->Head.ck_riff.ckID		= mmioFOURCC('L','I','N','K');
	else
		pAVIFile->Head.ck_riff.ckID		= mmioFOURCC('R','I','F','F');

	pAVIFile->Head.ck_riff.ckSize	= 0;
	pAVIFile->Head.ck_riff.ckCodec	= formtypeAVI;
	pAVIFile->Head.ck_hdrl.ckID		= mmioFOURCC('L','I','S','T');
	pAVIFile->Head.ck_hdrl.ckSize	= (sizeof(DWORD) + sizeof(CK) + sizeof(AVIMAINHEADER));
	pAVIFile->Head.ck_hdrl.ckType	= listtypeAVIHEADER;
	pAVIFile->Head.ck_avih.ckID		= ckidAVIMAINHDR;
	pAVIFile->Head.ck_avih.ckSize	= sizeof(AVIMAINHEADER);
#if 0
	pAVIFile->EHead.ck_odml.ckID	= mmioFOURCC('L','I','S','T');
	pAVIFile->EHead.ck_odml.ckSize	= (sizeof(DWORD) + sizeof(CK) + sizeof(ODMLExtendedAVIHeader));
	pAVIFile->EHead.ck_odml.ckType	= listtypeODMLExtendedAVIHeader;
	pAVIFile->EHead.ck_dmlh.ckID	= ckidODMLExtendedAVIHeader;
	pAVIFile->EHead.ck_dmlh.ckSize	= sizeof(ODMLExtendedAVIHeader);
#endif
	pAVIFile->Tail.ck_movi.ckID		= mmioFOURCC('L','I','S','T');
	pAVIFile->Tail.ck_movi.ckSize	= 0;
	pAVIFile->Tail.ck_movi.ckType	= listtypeAVIMOVIE;
#if 0
	pAVIFile->Tail.ck_junk.ckID		= ckidAVIPADDING;
	pAVIFile->Tail.ck_junk.ckSize	= 0;
#endif
	pAVIFile->Tail.ck_idx1.ckID		= ckidAVINEWINDEX;
	pAVIFile->Tail.ck_idx1.ckSize	= 0;

	pAVIFile->movi_size = 0;
	pAVIFile->idx_cnt = 0;
	pAVIFile->nIndexPos = 0;
	pAVIFile->nOffset = 4;


	// Set file header offset
	pAVIFile->nHeadWritten	= sizeof(AVIHEADBLOCK);

	*ppAVIFile = pAVIFile;

	return PROC_SUCCESS;
}


int AVIFileMainHeaderWrite(AVIFILE* pAVIFile)
{
#if 0
		AVIHEADBLOCK copyHdr;
		memcpy(&copyHdr, &pAVIFile->Head, sizeof(AVIHEADBLOCK));
		
		copyHdr.ck_riff.ckID	= ~(pAVIFile->Head.ck_riff.ckID);
		copyHdr.ck_riff.ckSize	= ~(pAVIFile->Head.ck_riff.ckSize);
		copyHdr.ck_riff.ckCodec = ~(pAVIFile->Head.ck_riff.ckCodec);
		copyHdr.ck_hdrl.ckID	= ~(pAVIFile->Head.ck_hdrl.ckID);
		copyHdr.ck_hdrl.ckSize	= ~(pAVIFile->Head.ck_hdrl.ckSize);
		copyHdr.ck_hdrl.ckType	= ~(pAVIFile->Head.ck_hdrl.ckType);
		copyHdr.ck_avih.ckID	= ~(pAVIFile->Head.ck_avih.ckID);
		copyHdr.ck_avih.ckSize	= ~(pAVIFile->Head.ck_avih.ckSize);
		
		copyHdr.avih.dwMicroSecPerFrame 	= ~(pAVIFile->Head.avih.dwMicroSecPerFrame);
		copyHdr.avih.dwMaxBytesPerSec		= ~(pAVIFile->Head.avih.dwMaxBytesPerSec);
		copyHdr.avih.dwFlags				= ~(pAVIFile->Head.avih.dwFlags);
		copyHdr.avih.dwTotalFrames			= ~(pAVIFile->Head.avih.dwTotalFrames);
		copyHdr.avih.dwInitialFrames		= ~(pAVIFile->Head.avih.dwInitialFrames);
		copyHdr.avih.dwStreams				= ~(pAVIFile->Head.avih.dwStreams);
		copyHdr.avih.dwSuggestedBufferSize	= ~(pAVIFile->Head.avih.dwSuggestedBufferSize);
		copyHdr.avih.dwWidth				= ~(pAVIFile->Head.avih.dwWidth);
		copyHdr.avih.dwHeight				= ~(pAVIFile->Head.avih.dwHeight);
		copyHdr.avih.dwRate 				= ~(pAVIFile->Head.avih.dwRate);
		copyHdr.avih.dwScale				= ~(pAVIFile->Head.avih.dwScale);
		
		// write AVI main header
		fwrite(&copyHdr, 1, sizeof(AVIHEADBLOCK), pAVIFile->pFile);
#else
	// write AVI main header
	fwrite(&pAVIFile->Head, 1, sizeof(AVIHEADBLOCK), pAVIFile->pFile);
#endif
	return PROC_SUCCESS;
}

int AVIFileStreamHeaderWrite(AVISTREAM* pavi)
{
	AVISTREAMBLOCK* pAVIStreamBlock = pavi->pStreamBlock;

	// Write LIST(strl) & strh
	fwrite(pAVIStreamBlock, 1, sizeof(AVISTREAMBLOCK) - sizeof(void*), pavi->pAVIFile->pFile);

	// Write strf
	fwrite(pAVIStreamBlock->pAviStrFormat, 1, pAVIStreamBlock->ck_strf.ckSize, pavi->pAVIFile->pFile);

	return PROC_SUCCESS;
}

int AVIFileMoviHeaderWrite(AVIFILE*pAVIFile)
{
	// Write LIST(movi)
	fwrite(&pAVIFile->Tail.ck_movi, 1, sizeof(CK_LIST), pAVIFile->pFile);
	fflush(pAVIFile->pFile);

	return PROC_SUCCESS;
}


int AVIFileCreateStream(AVIFILE* pAVIFile, AVISTREAM** ppavi, AVISTREAMHEADER* psi)
{
	// Alloc object
	AVISTREAM* pavi;
	pavi = (AVISTREAM*) malloc(sizeof(AVISTREAM));
	if(NULL == pavi){
		DBG2_LOG("Can't Create Stream object\n");
		return PROC_ERROR;
	}
	memset(pavi, 0, sizeof(AVISTREAM));

	pavi->pStreamBlock = (AVISTREAMBLOCK*) malloc(sizeof(AVISTREAMBLOCK));
	if(NULL == pavi->pStreamBlock)
	{
		free(pavi);
		DBG2_LOG("Can't Create Stream Block\n");
		return PROC_ERROR;
	}

	// Set AVISTREAMBLOCK //
	pavi->pAVIFile = pAVIFile;

	// Set strh
	pavi->pStreamBlock->ck_strl.ckID 	= mmioFOURCC('L','I','S','T');
	pavi->pStreamBlock->ck_strl.ckSize 	= 0;
	pavi->pStreamBlock->ck_strl.ckType 	= listtypeSTREAMHEADER;
	pavi->pStreamBlock->ck_strh.ckID 	= ckidSTREAMHEADER;
	pavi->pStreamBlock->ck_strh.ckSize 	= sizeof(AVISTREAMHEADER);

	memcpy(&(pavi->pStreamBlock->AviStrHeader), psi, sizeof(AVISTREAMHEADER));

	// Renew information
	if((streamtypeVIDEO == pavi->pStreamBlock->AviStrHeader.fccType))
	{
		pavi->ck_data.ckID = mmioFOURCC('0', '0'+pAVIFile->Head.avih.dwStreams, 'd', 'c');

		pAVIFile->Head.avih.dwMicroSecPerFrame 		= (DWORD)(1000000./pavi->pStreamBlock->AviStrHeader.dwRate*pavi->pStreamBlock->AviStrHeader.dwScale + 0.5);
		pAVIFile->Head.avih.dwMaxBytesPerSec 		= 0;
//		pAVIFile->Head.avih.dwFlags 				= AVIF_HASINDEX | AVIF_TRUSTCKTYPE;
		pAVIFile->Head.avih.dwFlags 				= AVIF_HASINDEX | AVIF_ISINTERLEAVED;
		pAVIFile->Head.avih.dwTotalFrames 			= 0;
		pAVIFile->Head.avih.dwInitialFrames			= 0;
		pAVIFile->Head.avih.dwStreams 				= 0;
		pAVIFile->Head.avih.dwSuggestedBufferSize 	= pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize;
		pAVIFile->Head.avih.dwWidth 				= pavi->pStreamBlock->AviStrHeader.rcFrame[2];
		pAVIFile->Head.avih.dwHeight 				= pavi->pStreamBlock->AviStrHeader.rcFrame[3];
		pAVIFile->Head.avih.dwRate 					= pavi->pStreamBlock->AviStrHeader.dwRate;
		pAVIFile->Head.avih.dwScale 				= pavi->pStreamBlock->AviStrHeader.dwScale;
	}
	else if(streamtypeAUDIO == pavi->pStreamBlock->AviStrHeader.fccType)
	{
		pavi->ck_data.ckID = mmioFOURCC('0', '0'+pAVIFile->Head.avih.dwStreams, 'w', 'b');
	}
	else if(streamtypeTEXT == pavi->pStreamBlock->AviStrHeader.fccType)
	{
		pavi->ck_data.ckID = mmioFOURCC('0', '0'+pAVIFile->Head.avih.dwStreams, 't', 'x');
	}
	else
	{
		free(pavi->pStreamBlock);
		free(pavi);
		DBG2_LOG("Invalid Stream Type: %ld\n", pavi->pStreamBlock->AviStrHeader.fccType);
		return PROC_ERROR;
	}

	++pAVIFile->Head.avih.dwStreams;

	*ppavi = pavi;

	return PROC_SUCCESS;
}

int AVIFileCreateStreamSub(AVIFILE* pAVIFile, AVISTREAM** ppavi, AVISTREAMHEADER* psi)
{
	// Alloc object
	AVISTREAM* pavi;
	pavi = (AVISTREAM*) malloc(sizeof(AVISTREAM));
	if(NULL == pavi){
		DBG2_LOG("Can't Create Stream object\n");
		return PROC_ERROR;
	}
	memset(pavi, 0, sizeof(AVISTREAM));

	pavi->pStreamBlock = (AVISTREAMBLOCK*) malloc(sizeof(AVISTREAMBLOCK));
	if(NULL == pavi->pStreamBlock)
	{
		free(pavi);
		DBG2_LOG("Can't Create Stream Block\n");
		return PROC_ERROR;
	}

	// Set AVISTREAMBLOCK //
	pavi->pAVIFile = pAVIFile;

	// Set strh
	pavi->pStreamBlock->ck_strl.ckID 	= mmioFOURCC('L','I','S','T');
	pavi->pStreamBlock->ck_strl.ckSize 	= 0;
	pavi->pStreamBlock->ck_strl.ckType 	= listtypeSTREAMHEADER;
	pavi->pStreamBlock->ck_strh.ckID 	= ckidSTREAMHEADER;
	pavi->pStreamBlock->ck_strh.ckSize 	= sizeof(AVISTREAMHEADER);

	memcpy(&(pavi->pStreamBlock->AviStrHeader), psi, sizeof(AVISTREAMHEADER));

	// Renew information
	if((streamtypeVIDEO == pavi->pStreamBlock->AviStrHeader.fccType))
	{
		pavi->ck_data.ckID = mmioFOURCC('0', '0'+pAVIFile->Head.avih.dwStreams, 'd', 'c');

	}
	else if(streamtypeAUDIO == pavi->pStreamBlock->AviStrHeader.fccType)
	{
		pavi->ck_data.ckID = mmioFOURCC('0', '0'+pAVIFile->Head.avih.dwStreams, 'w', 'b');
	}
	else
	{
		free(pavi->pStreamBlock);
		free(pavi);
		DBG2_LOG("Invalid Stream Type: %ld\n", pavi->pStreamBlock->AviStrHeader.fccType);
		return PROC_ERROR;
	}

	++pAVIFile->Head.avih.dwStreams;

	// Set file pointer
//	fseek(pAVIFile->pFile, sizeof(CK_LIST) + sizeof(CK) + sizeof(AVISTREAMHEADER), SEEK_CUR);

	*ppavi = pavi;

	return PROC_SUCCESS;
}

int AVIStreamSetFormat(AVISTREAM* pavi, long lPos, void* lpFormat, long cbFormat)
{
	AVISTREAMBLOCK* pAVIStreamBlock = pavi->pStreamBlock;

	pAVIStreamBlock->ck_strf.ckID 	= ckidSTREAMFORMAT;
	if((streamtypeTEXT == pavi->pStreamBlock->AviStrHeader.fccType))
	{
		pAVIStreamBlock->ck_strf.ckSize = 0;
		pAVIStreamBlock->pAviStrFormat = NULL;
	}
	else
	{
		pAVIStreamBlock->pAviStrFormat 	= (AVISTREAMFORMAT*) malloc(cbFormat);
		pAVIStreamBlock->ck_strf.ckSize = cbFormat;
		memcpy(pAVIStreamBlock->pAviStrFormat, lpFormat, cbFormat);
	}

	// Set file pointer to write data
	pAVIStreamBlock->ck_strl.ckSize = sizeof(DWORD) + 2*sizeof(CK) + pAVIStreamBlock->ck_strh.ckSize + pAVIStreamBlock->ck_strf.ckSize;
	pavi->pAVIFile->Head.ck_hdrl.ckSize += ((sizeof(CK_LIST)-4) + pAVIStreamBlock->ck_strl.ckSize);

//	int nHeaderSize = sizeof(CK_RIFF) + ((sizeof(CK_LIST)-4) + pavi->pAVIFile->Head.ck_hdrl.ckSize);

	return PROC_SUCCESS;
}

int AVIStreamWrite(AVISTREAM* pavi, long lStart, long lSamples, void* lpBuffer, long cbBuffer, DWORD dwFlags)
{
	// Write chunk header
	pavi->ck_data.ckSize = cbBuffer;
	fwrite(&pavi->ck_data, 1, sizeof(CK), pavi->pAVIFile->pFile);

	// Write chunk Data
	fwrite(lpBuffer, 1, cbBuffer, pavi->pAVIFile->pFile);


	// Make Index Entry
	INDEX* pIndex = (INDEX *) malloc(sizeof(INDEX));
	pIndex->next 				= 0;
	pIndex->data.ckid 			= pavi->ck_data.ckID;
	pIndex->data.dwFlags 		= dwFlags;
	pIndex->data.dwChunkOffset 	= pavi->pAVIFile->nOffset;

	pavi->pAVIFile->nOffset += (sizeof(CK) + cbBuffer);
	pIndex->data.dwChunkLength = cbBuffer;

	// Add Entry To Linked List
	if (!pavi->pAVIFile->IndexRoot.first)
		pavi->pAVIFile->IndexRoot.first = pIndex;
	else
		pavi->pAVIFile->IndexRoot.last->next = pIndex;
	pavi->pAVIFile->IndexRoot.last = pIndex;


	// Update movi_size
	pavi->pAVIFile->movi_size += (sizeof(CK) + cbBuffer);

	// Update idx_cnt
	++(pavi->pAVIFile->idx_cnt);

	// Update AVI header
		// Update SuggestedBuifferSize
	if(pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize < (unsigned)cbBuffer)
		pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize = cbBuffer;

	// Update total length
	pavi->pStreamBlock->AviStrHeader.dwLength += lSamples;

	return PROC_SUCCESS;
}

int AVIStreamWriteBuf(AVISTREAM* pavi, long lStart, long nFrameCnt, void* lpBuffer, long nFrameSize, DWORD dwFlags, CBlockBuffer* pBuffer)
{
	long nSize = nFrameSize * nFrameCnt;
	long nWriteSize = nSize;
//	char bzero[3] = {0, 0, 0};

	pavi->ck_data.ckSize = nWriteSize;

#if 1

	fwrite(&pavi->ck_data, 1, sizeof(CK), pavi->pAVIFile->pFile);
	fwrite(lpBuffer, 1, nWriteSize, pavi->pAVIFile->pFile);

#else
	if(pBuffer->GetRestSize() < (nWriteSize+sizeof(CK)))
	{
		// Write buffered data
		fwrite(pBuffer->GetData(), 1, pBuffer->GetCurSize(), pavi->pAVIFile->pFile);
		pBuffer->ClearData();
	}

	// Write chunk header
	pBuffer->Write((unsigned char*)&pavi->ck_data, sizeof(CK));
	// Write chunk Data
	pBuffer->Write((unsigned char*)lpBuffer, nSize);

	if(nWriteSize-nSize)
		pBuffer->Write((unsigned char*)bzero, nWriteSize-nSize);
#endif

	// Make Index Entry
	INDEX* pIndex = (INDEX *) malloc(sizeof(INDEX));
	pIndex->next 				= 0;
	pIndex->data.ckid 			= pavi->ck_data.ckID;
	pIndex->data.dwFlags 		= dwFlags;
	pIndex->data.dwChunkOffset 	= pavi->pAVIFile->nOffset;

	pavi->pAVIFile->nOffset += (sizeof(CK) + nWriteSize);
	pIndex->data.dwChunkLength = nWriteSize;

	// Add Entry To Linked List
	if (!pavi->pAVIFile->IndexRoot.first)
		pavi->pAVIFile->IndexRoot.first = pIndex;
	else
		pavi->pAVIFile->IndexRoot.last->next = pIndex;
	pavi->pAVIFile->IndexRoot.last = pIndex;


	// Update movi_size
	pavi->pAVIFile->movi_size += (sizeof(CK) + nWriteSize);

	// Update idx_cnt
	++(pavi->pAVIFile->idx_cnt);

	// Update AVI header
		// Update SuggestedBuifferSize
	if(pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize < (unsigned)nWriteSize)
		pavi->pStreamBlock->AviStrHeader.dwSuggestedBufferSize = nWriteSize;

	// Update total length
	pavi->pStreamBlock->AviStrHeader.dwLength += nFrameCnt;

	return PROC_SUCCESS;
}

int AVIStreamWriteBufFlush(AVISTREAM* pavi, CBlockBuffer* pBuffer)
{
	// Write buffered data
	if(pBuffer->GetCurSize())
		fwrite(pBuffer->GetData(), 1, pBuffer->GetCurSize(), pavi->pAVIFile->pFile);

	pBuffer->ClearData();

	return PROC_SUCCESS;
}


int AVIStreamClose(AVISTREAM* pavi)
{
	if(!pavi)
		return PROC_ERROR;

	fpos_t pos;

	if(0 == pavi->pAVIFile->nIndexPos)
		if( fgetpos(pavi->pAVIFile->pFile, &pos ) == 0 )
			pavi->pAVIFile->nIndexPos = 1;//(unsigned long)pos;

	AVISTREAMBLOCK* pAVIStreamBlock = pavi->pStreamBlock;

	// Check file offset
	fseek(pavi->pAVIFile->pFile, pavi->pAVIFile->nHeadWritten, SEEK_SET);
	pavi->pAVIFile->nHeadWritten += ((sizeof(CK_LIST)-4) + pAVIStreamBlock->ck_strl.ckSize);

	if((streamtypeVIDEO == pavi->pStreamBlock->AviStrHeader.fccType))
	{
		pAVIStreamBlock->AviStrHeader.dwSuggestedBufferSize *= 10;

		if(pavi->pAVIFile->Head.avih.dwWidth == pAVIStreamBlock->AviStrHeader.rcFrame[2] &&
			pavi->pAVIFile->Head.avih.dwHeight == pAVIStreamBlock->AviStrHeader.rcFrame[3])
		{
			pavi->pAVIFile->Head.avih.dwSuggestedBufferSize = pAVIStreamBlock->AviStrHeader.dwSuggestedBufferSize;
			pavi->pAVIFile->Head.avih.dwTotalFrames = pAVIStreamBlock->AviStrHeader.dwLength;
		}
	}
#if 0
	if((streamtypeAUDIO == pavi->pStreamBlock->AviStrHeader.fccType))
	{
		pAVIStreamBlock->AviStrHeader.dwSuggestedBufferSize *= 10;
	}
#endif
	// Write LIST(strl) & strh
	fwrite(pAVIStreamBlock, 1, sizeof(AVISTREAMBLOCK) - sizeof(void*), pavi->pAVIFile->pFile);

	// Write strf
	if(pAVIStreamBlock->ck_strf.ckSize > 0)
		fwrite(pAVIStreamBlock->pAviStrFormat, 1, pAVIStreamBlock->ck_strf.ckSize, pavi->pAVIFile->pFile);

	fflush(pavi->pAVIFile->pFile);

	// Delete a stream header
	if(pavi)
	{
		if(pAVIStreamBlock->pAviStrFormat)
			free(pAVIStreamBlock->pAviStrFormat);
		if(pAVIStreamBlock)
			free(pAVIStreamBlock);
		free(pavi);
	}

	return PROC_SUCCESS;
}


int AVIFileClose(AVIFILE* pAVIFile)
{
	if(!pAVIFile){
		DBG2_LOG("Invalid pAVIFile object!!! \n");
		return PROC_ERROR;
	}

	unsigned long nTotalSize;

	if(!pAVIFile->pFile){
		DBG2_LOG("Invalid pFile pointer!!! \n");
		return PROC_ERROR;
	}
#if 0
	// Check file offset
	fseek(pAVIFile->pFile, pAVIFile->nHeadWritten, SEEK_SET);
	pAVIFile->nHeadWritten += ((sizeof(CK_LIST)-4) + pAVIFile->EHead.ck_odml.ckSize);
	pAVIFile->EHead.odml.dwTotalFrames = pAVIFile->Head.avih.dwTotalFrames;
	fwrite(&pAVIFile->EHead, 1, sizeof(EAVIHEADBLOCK), pAVIFile->pFile);
	pAVIFile->Head.ck_hdrl.ckSize += ((sizeof(CK_LIST)-4) + pAVIFile->EHead.ck_odml.ckSize);


	fseek(pAVIFile->pFile, pAVIFile->nHeadWritten, SEEK_SET);

	// Write JUNK
	int nHeaderSize = pAVIFile->nHeadWritten;
	if(nHeaderSize < MOVI_POS_OFFSET)
	{
		pAVIFile->Tail.ck_junk.ckSize = MOVI_POS_OFFSET - nHeaderSize - sizeof(CK) - sizeof(CK_LIST);
		fwrite(&pAVIFile->Tail.ck_junk, 1, sizeof(CK), pAVIFile->pFile);
		fseek(pAVIFile->pFile, pAVIFile->Tail.ck_junk.ckSize, SEEK_CUR);
		nTotalSize = MOVI_POS_OFFSET;
	}
	else
		nTotalSize = nHeaderSize;
#endif

	fseek(pAVIFile->pFile, pAVIFile->nHeadWritten, SEEK_SET);
	int nHeaderSize = pAVIFile->nHeadWritten;
	nTotalSize = nHeaderSize;

	// Write movi header
	pAVIFile->movi_size = ((pAVIFile->movi_size+3)>>2)<<2;
	pAVIFile->Tail.ck_movi.ckSize	= sizeof(DWORD) + pAVIFile->movi_size;
	fwrite(&pAVIFile->Tail.ck_movi, 1, sizeof(CK_LIST), pAVIFile->pFile);
//	nTotalSize += (pAVIFile->Tail.ck_movi.ckSize-4);
	nTotalSize += (pAVIFile->Tail.ck_movi.ckSize)+sizeof(CK);


	// Write idx1 header
	fseek(pAVIFile->pFile, nTotalSize, SEEK_SET);
	pAVIFile->Tail.ck_idx1.ckSize = pAVIFile->idx_cnt * sizeof(AVIINDEXENTRY);
	fwrite(&pAVIFile->Tail.ck_idx1, 1, sizeof(CK), pAVIFile->pFile);

	// Write index list
	INDEX *pIndex = pAVIFile->IndexRoot.first;
	int nCnt = 0;
	while(pIndex)
	{
		fwrite(&pIndex->data, 1, sizeof(AVIINDEXENTRY), pAVIFile->pFile);
		pIndex = pIndex->next;
		nCnt++;
	}
	nTotalSize += (sizeof(CK) + pAVIFile->Tail.ck_idx1.ckSize);

#if 0
#ifdef _DEBUG
	int nHeight = pAVIFile->Head.avih.dwHeight;
	int nWidth = pAVIFile->Head.avih.dwWidth;
#endif
#endif

	// Write AVI header
	fseek(pAVIFile->pFile, 0, SEEK_SET);
	pAVIFile->Head.ck_riff.ckSize	= nTotalSize - (sizeof(CK_RIFF)-4);
	fwrite(&pAVIFile->Head, 1, sizeof(AVIHEADBLOCK), pAVIFile->pFile);

	// Close AVI file
	#if 0		//# phoong
	if(fsync(pAVIFile->pFile->_fileno))
		printf("%s: fsync\n", __func__);
	#endif
	fclose(pAVIFile->pFile);

	INDEX *pIFree;
	pIndex = pAVIFile->IndexRoot.first;
	while(pIndex)
	{
		pIFree = pIndex;
		pIndex = pIndex->next;
		free(pIFree);
	}

	if(pAVIFile)
		free(pAVIFile);
	pAVIFile = NULL;

	return PROC_SUCCESS;
}
