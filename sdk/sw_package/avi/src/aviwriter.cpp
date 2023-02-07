// AVIWriter.cpp: implementation of the CAVIWriter class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "aviwriter.hh"
#include "mmsystem.h"

CAVIWriter::CAVIWriter()
{
	int i;

	m_bFileOpen = FALSE; m_pfile = NULL; m_aps = NULL; m_mps = NULL;
	for(i=0; i<MAX_VID_CH; i++) {
		m_vps[i] = NULL;
	}
}

CAVIWriter::~CAVIWriter()
{
	//# do nothing
}

CAVIWriter* CAVIWriter::CreateNew()
{
	return new CAVIWriter();
}

BOOL CAVIWriter::Open(const char* pszFilePath, SYSTEM_PARAM* pSystemParam)
{
	int i;
	if(m_bFileOpen || m_pfile || m_aps || m_mps)
		return PROC_ERROR;

	m_nVidChannel	= pSystemParam->nVidChannel;
	m_bEnableMeta	= pSystemParam->bEnableMeta;
	m_bEncrypt      = pSystemParam->bEncrypt;
	m_bEnableAudio	= pSystemParam->bEnableAudio;
	m_nAudioType 	= pSystemParam->nAudioType;

	BITMAPINFOHEADER 	strfVideo[MAX_VID_CH];
	MPEGAUDIOWAVEFORMAT strfAudio;

	unsigned long uVideoFORCC = 0;
	switch(pSystemParam->uVideoType)
	{
		case IME6400_SD_MPEG4:
			uVideoFORCC = mmioFOURCC('d','i','v','x');
			break;
		/*
		case IME6400_SD_MPEG2:
			uVideoFORCC = mmioFOURCC('m','p','2','v');
			break;
		case IME6400_SD_MPEG1:
			uVideoFORCC = mmioFOURCC('m','p','e','g');
			break;
		*/
		case IME6400_SD_H264:
			uVideoFORCC = mmioFOURCC('H','2','6','4');
			break;
	}

	for(i=0; i<m_nVidChannel; i++)
	{
		m_bFirstVideo[i] = TRUE;

		//# Create a video stream hdr MAIN
		memset(&m_strvhdr[i], 0, sizeof(m_strvhdr[i]));
		m_strvhdr[i].fccType				= streamtypeVIDEO;
		m_strvhdr[i].fccHandler				= uVideoFORCC;
		m_strvhdr[i].dwScale				= 100;
		m_strvhdr[i].dwRate					= (DWORD)(pSystemParam->fFrameRate*m_strvhdr[i].dwScale);   // fps * 100 time scale
		m_strvhdr[i].dwLength				= 0;
		m_strvhdr[i].dwQuality				= -1;
		m_strvhdr[i].dwSuggestedBufferSize	= 0;
		m_strvhdr[i].rcFrame[0]				= 0;
		m_strvhdr[i].rcFrame[1]				= 0;
		m_strvhdr[i].rcFrame[2]				= pSystemParam->nVidWidth[i];
		m_strvhdr[i].rcFrame[3]				= pSystemParam->nVidHeight[i];

		// Make a video format ///////////////////////////////////
		memset(&strfVideo[i], 0, sizeof(BITMAPINFOHEADER));
		strfVideo[i].biSize					= sizeof(BITMAPINFOHEADER);
		strfVideo[i].biWidth				= pSystemParam->nVidWidth[i];
		strfVideo[i].biHeight				= pSystemParam->nVidHeight[i];
		strfVideo[i].biPlanes				= 1;
		strfVideo[i].biBitCount				= 24;
		strfVideo[i].biCompression			= uVideoFORCC;
		strfVideo[i].biSizeImage			= strfVideo[i].biWidth*strfVideo[i].biHeight*3;
		strfVideo[i].biXPelsPerMeter		= 0;
		strfVideo[i].biYPelsPerMeter		= 0;
		strfVideo[i].biClrUsed				= 0;
		strfVideo[i].biClrImportant			= 0;
	}

	if(m_bEnableAudio)
	{
		// Create a audio stream hdr

		// Make a audio stream hdr
		memset(&m_strahdr, 0, sizeof(m_strahdr));
		m_strahdr.fccType				= streamtypeAUDIO;	// stream type
		m_strahdr.fccHandler			= 1;

		if(IME6400_AM_ADPCM == m_nAudioType) {
			m_strahdr.dwScale			= pSystemParam->nAudioFrameSize;
		} else {
			m_strahdr.dwScale			= 1;
		}

		m_strahdr.dwRate = pSystemParam->nAudioBitRate;
		m_strahdr.dwLength				= 0;
		m_strahdr.dwQuality				= -1;
		m_strahdr.dwSuggestedBufferSize = pSystemParam->nAudioFrameSize;

		if(IME6400_AM_ADPCM == m_nAudioType) {
			m_strahdr.dwSampleSize		= pSystemParam->nAudioFrameSize;
		} else {
			m_strahdr.dwSampleSize		= 1;
		}

		// Make a audio format
		if(IME6400_AM_MPEG1L2 == m_nAudioType)
		{
			memset(&strfAudio, 0, sizeof(MPEGAUDIOWAVEFORMAT));
			strfAudio.wfx.wFormatTag 		= WAVE_FORMAT_MPEG;
			strfAudio.wfx.nChannels 		= pSystemParam->nAudioChannel;
			strfAudio.wfx.nSamplesPerSec 	= pSystemParam->nSamplesPerSec;
			strfAudio.wfx.wBitsPerSample 	= 16;
			strfAudio.wfx.nBlockAlign 		= 1;					//(strfAudio.wfx.wBitsPerSample*strfAudio.wfx.nChannels)/8;
			strfAudio.wfx.nAvgBytesPerSec 	= pSystemParam->nAudioBitRate/8;	//strfAudio.wfx.nBlockAlign*strfAudio.wfx.nSamplesPerSec/8;
			strfAudio.wfx.cbSize 			= sizeof(MPEGAUDIOWAVEFORMAT) - sizeof(WAVEFORMATEX);

			strfAudio.fwHeadLayer 			= ACM_MPEG_LAYER2;
			strfAudio.dwHeadBitrate 		= pSystemParam->nAudioBitRate;
			strfAudio.fwHeadMode 			= ACM_MPEG_STEREO;
			strfAudio.fwHeadModeExt 		= 0;
			strfAudio.wHeadEmphasis 		= 0;
			strfAudio.fwHeadFlags 			= 0;
			strfAudio.dwPTSLow 				= 0;
			strfAudio.dwPTSHigh 			= 0;
		}
		else if(IME6400_AM_MULAW == m_nAudioType)
		{
			memset(&strfAudio, 0, sizeof(WAVEFORMATEX));
			strfAudio.wfx.wFormatTag 		= WAVE_FORMAT_MULAW;//
			strfAudio.wfx.nChannels 		= pSystemParam->nAudioChannel;
			strfAudio.wfx.nSamplesPerSec 	= pSystemParam->nSamplesPerSec;
			strfAudio.wfx.wBitsPerSample 	= 8;//pSystemParam->nAudioBitPerSample;
			strfAudio.wfx.nBlockAlign 		= (strfAudio.wfx.wBitsPerSample*strfAudio.wfx.nChannels)/8;
			strfAudio.wfx.nAvgBytesPerSec 	= strfAudio.wfx.nSamplesPerSec * strfAudio.wfx.nBlockAlign;//8;
			strfAudio.wfx.cbSize 			= 0;

			m_strahdr.dwSampleSize			= strfAudio.wfx.nBlockAlign;
		}
		else if(IME6400_AM_ADPCM == m_nAudioType)
		{
			memset(&strfAudio, 0, sizeof(WAVEFORMATEX)+2);
			strfAudio.wfx.wFormatTag 		= WAVE_FORMAT_IMA_ADPCM;
			strfAudio.wfx.nChannels 		= pSystemParam->nAudioChannel;
			strfAudio.wfx.nSamplesPerSec 	= pSystemParam->nSamplesPerSec;
			strfAudio.wfx.wBitsPerSample 	= 4;
			strfAudio.wfx.nBlockAlign 		= 512;//strfAudio.wfx.wBitsPerSample==4 ? 512 : 1024;//pSystemParam->nAudioFrameSize;//
			strfAudio.wfx.nAvgBytesPerSec 	= (strfAudio.wfx.nSamplesPerSec*strfAudio.wfx.nChannels*strfAudio.wfx.wBitsPerSample)/8;
			strfAudio.wfx.cbSize 			= 2;
			strfAudio.fwHeadLayer 			= (unsigned short)(pSystemParam->nAudioFrameSize*pSystemParam->nSamplesPerSec/strfAudio.wfx.nAvgBytesPerSec);
		}
	}

	if(m_bEnableMeta)
	{
		memset(&m_strmhdr, 0, sizeof(m_strmhdr));
		m_strmhdr.fccType               = streamtypeTEXT;// stream type
		m_strmhdr.fccHandler            = 0;
		m_strmhdr.dwScale               = 100;
		m_strmhdr.dwRate                = 0;   // fps * 100 time scale
		m_strmhdr.dwLength				= 0;
		m_strmhdr.dwQuality				= -1;
		m_strmhdr.dwSuggestedBufferSize = 0;
		m_strmhdr.rcFrame[0] 			= 0;
		m_strmhdr.rcFrame[1] 			= 0;
		m_strmhdr.rcFrame[2] 			= 0;
		m_strmhdr.rcFrame[3] 			= 0;
	}

	// Open a file
	DBG2_LOG("AVIFileOpen m_bEncrypt = %d\n",m_bEncrypt);
	int hr;
	hr = AVIFileOpen(&m_pfile,			// returned file pointer
		       pszFilePath, m_bEncrypt);			// file name, Encrypt main header

	if (hr != PROC_SUCCESS)
	{
		DBG2_LOG("AVIFileOpen error : 0x%x\n", hr);
		Close();
		return PROC_ERROR;
	}

	for(i=0; i<m_nVidChannel; i++)
	{
		//# Create the video stream;
		if(i==0) {
			hr = AVIFileCreateStream(m_pfile,	// file pointer
			         &m_vps[i],		    		// returned stream pointer
					 &m_strvhdr[i]);	    	// stream header
		}
		else {
			hr = AVIFileCreateStreamSub(m_pfile,
					&m_vps[i],
					&m_strvhdr[i]);
		}

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIFileCreateStream CH%d error : 0x%x\n", i, hr);
			Close();
			return PROC_ERROR;
		}

		//# Set the video format
		hr = AVIStreamSetFormat(m_vps[i],
			0,
			&strfVideo[i],				// stream format
			sizeof(strfVideo[i]) );		// format size

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIStreamSetFormat CH%d error : 0x%x\n", i, hr);
			Close();
			return PROC_ERROR;
		}
	}

	if(m_bEnableAudio)
	{
		// Create the audio stream;
		hr = AVIFileCreateStream(m_pfile,	// file pointer
			&m_aps,		    				// returned stream pointer
			&m_strahdr);	    			// stream header

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIFileCreateStream error : 0x%x\n", hr);
			Close();
			return PROC_ERROR;
		}

		// Set the audio format
		if(IME6400_AM_MPEG1L2 == m_nAudioType)
		{
			hr = AVIStreamSetFormat(m_aps,
				0,
				&strfAudio,						// stream format
				sizeof(MPEGAUDIOWAVEFORMAT) );	// format size
		}
		else if(IME6400_AM_MULAW == m_nAudioType)
		{
			hr = AVIStreamSetFormat(m_aps,
				0,
				&strfAudio,						// stream format
				sizeof(WAVEFORMATEX)-2 ); 		// format size
		}
		else if(IME6400_AM_ADPCM == m_nAudioType)
		{
			hr = AVIStreamSetFormat(m_aps,
				0,
				&strfAudio,						// stream format
				sizeof(WAVEFORMATEX)+2 ); 		// format size
		}

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIStreamSetFormat error : 0x%x\n", hr);
			Close();
			return PROC_ERROR;
		}
	}

	if(m_bEnableMeta)
	{
		// Create the meta stream;
		hr = AVIFileCreateStream(m_pfile,	// file pointer
			&m_mps,		    // returned stream pointer
			&m_strmhdr);	    // stream header

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIFileCreateStream error : 0x%x\n", hr);
			Close();
			return PROC_ERROR;
		}

		hr = AVIStreamSetFormat(m_mps,
			0,
			NULL,	    // stream format
			0 ); // format size

		if (hr != PROC_SUCCESS)
		{
			DBG2_LOG("AVIStreamSetFormat error : 0x%x\n", hr);
			Close();
			return PROC_ERROR;
		}

	}

	//// write header init info ////
	AVIFileMainHeaderWrite(m_pfile);

	for(i = 0; i < MAX_VID_CH; i++)	{
		if (m_vps[i]) {
			AVIFileStreamHeaderWrite(m_vps[i]);
		}
	}

	if(m_aps) {
		AVIFileStreamHeaderWrite(m_aps);
	}
	if(m_mps) {
		AVIFileStreamHeaderWrite(m_mps);
	}

	AVIFileMoviHeaderWrite(m_pfile);

	//m_BlockBuffer.Alloc(5*1024*1024);

	m_bFileOpen = TRUE;

	return PROC_SUCCESS;
}

BOOL CAVIWriter::WriteVideoData(BYTE* pbyData, int nFrameSize, BOOL isKeyFrame, int nFrameCnt, int ch)
{
	if(!m_bFileOpen) {
		DBG2_LOG("File is not opened!!\n");
		return PROC_ERROR;
	}

	int hr;
	DWORD dwKeyFrame;
	BYTE* pbyData_strip;
	int nFrameSize_strip;

	dwKeyFrame = (isKeyFrame) ? AVIIF_KEYFRAME : 0;

	if(isKeyFrame)
	{
		if(m_bFirstVideo[ch])
		{
			m_bFirstVideo[ch] = FALSE;
			pbyData_strip = pbyData;
			nFrameSize_strip = nFrameSize;
		}
		else
		{
			pbyData_strip = pbyData;
			nFrameSize_strip = nFrameSize;

		}
	}
	else
	{
		pbyData_strip = pbyData;
		nFrameSize_strip = nFrameSize;
	}

	if(NULL == m_vps[ch]) {
		DBG2_LOG("AVI writing error : m_vps[%d] is NULL\n", ch);
		return PROC_ERROR;
	}

	hr = AVIStreamWriteBuf(m_vps[ch],	// stream pointer
		0,								// time of this frame
		nFrameCnt,						// number to write
		pbyData_strip,					// pointer to data
		nFrameSize_strip,				// size of this frame
		dwKeyFrame,						// flags....
		&m_BlockBuffer);

	if (hr != PROC_SUCCESS)
	{
		DBG2_LOG("AVI writing error : Video[%d] 0x%x\n", ch, hr);
		//CloseFile();
		return PROC_ERROR;
	}

	return PROC_SUCCESS;
}

BOOL CAVIWriter::WriteAudioData(BYTE* pbyData, int nFrameSize, int nFrameCnt)
{
//	TRACE("WriteAudioData %d, %d", nFrameSize, nFrameCnt);
	if(!m_bFileOpen) {
		DBG2_LOG("File is not opened!!\n");
		return PROC_ERROR;
	}
	if(!m_bEnableAudio) {
		return PROC_ERROR;
	}

	int hr;

	if(NULL == m_aps) {
		DBG2_LOG("AVI writing error : m_aps is NULL\n");
		return PROC_ERROR;
	}

	hr = AVIStreamWriteBuf(m_aps,	// stream pointer
		1,							// time of this frame
		nFrameCnt,					// number to write
		pbyData,					// pointer to data
		nFrameSize,					// size of this frame
		1,							// key frame flag
		&m_BlockBuffer);

	if (hr != PROC_SUCCESS)
	{
		DBG2_LOG("AVI writing error : Audio 0x%x\n", hr);
		//CloseFile();
		return PROC_ERROR;
	}

	return PROC_SUCCESS;
}

BOOL CAVIWriter::WriteMetaData(BYTE* pbyData, int nFrameSize, int nFrameCnt)
{
//	TRACE("WriteMetaData %d, %d", nFrameSize, nFrameCnt);
	if(!m_bFileOpen) {
		DBG2_LOG("File is not opened!!\n");
		return PROC_ERROR;
	}
	if(!m_bEnableMeta) {
		return PROC_ERROR;
	}

	int hr;

	if(NULL == m_mps){
		DBG2_LOG("AVI writing error : m_mps is NULL\n");
		return PROC_ERROR;
	}

	hr = AVIStreamWriteBuf(m_mps,	// stream pointer
		1,							// time of this frame
		nFrameCnt,					// number to write
		pbyData,					// pointer to data
		nFrameSize,					// size of this frame
		1,							// key frame flag
		&m_BlockBuffer);

	if (hr != PROC_SUCCESS)
	{
		DBG2_LOG("AVI writing error : meta 0x%x\n", hr);
		//CloseFile();
		return PROC_ERROR;
	}

	return PROC_SUCCESS;
}

void CAVIWriter::Close()
{
	int i;
	if(!m_bFileOpen)
		return;

	for(i = 0; i < MAX_VID_CH; i++)
	{
		if (m_vps[i])
		{
			//AVIStreamWriteBufFlush(m_vps[i], &m_BlockBuffer);
			AVIStreamClose(m_vps[i]);
			m_vps[i] = NULL;
		}
	}

	if (m_aps) {
		AVIStreamClose(m_aps);
	}

	if (m_mps) {
		AVIStreamClose(m_mps);
	}

	if (m_pfile) {
		AVIFileClose(m_pfile);
	}

	m_pfile = NULL;
	m_aps = NULL;
	m_mps = NULL;

	//m_BlockBuffer.Free();

	m_bFileOpen = FALSE;
}

BOOL CAVIWriter::IsRecord()
{
	return m_bFileOpen;
}
