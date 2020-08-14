// BaseWriter.h: interface for the CBaseWriter class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _BASEWRITER_H_
#define _BASEWRITER_H_

#include <fcntl.h>
#include "syscommon.h"

class CBaseWriter  
{
public:
	CBaseWriter() { m_fileExtra = NULL; };
	virtual ~CBaseWriter() { if(m_fileExtra) { fclose(m_fileExtra); m_fileExtra = NULL; } };

/*
	virtual BOOL Open(int nChannel, CString strPath, int nSystemMode, unsigned long uVideoMode,
		int nWidth, int nHeight, double fFps,
		int nAudioMode, int nAudioChannel, int nAudioSampleRate, int nAudioBitrate, 
		int nAudioFrameSize, int nAudioFrameCnt, void *nThread = 0) = 0;
*/
	virtual BOOL OpenFile(int nChannel, int nSystemMode,
		int nAudioMode, int nAudioChannel, int nAudioBitrate, const char* sDrive) = 0;
	virtual void Close() = 0;
	
	virtual BOOL WriteVideoData(BYTE* pbyData, int nFrameSize, BOOL isKeyFrame, int nFrameCnt = 0) = 0;
	virtual BOOL WriteAudioData(BYTE* pbyData, int nFrameSize, int nFrameCnt = 0) = 0;

public:
	virtual BOOL OpenExtraFile(const char* strPath);
	virtual BOOL WriteExtraData(BYTE* pbyData, int nDataSize);
	virtual void CloseExtraFile();

protected:
	FILE* m_fileExtra;
};

#endif // !defined_BASEWRITER_H_
