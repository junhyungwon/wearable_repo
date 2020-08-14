// mmsystem.h: interface for the multimedia informtion.
//
//////////////////////////////////////////////////////////////////////
#ifndef _MMSYSTEM_H_
#define _MMSYSTEM_H_

#include "syscommon.h"

#define MAX_VID_CH		4

enum IME6400_STANDARD	{ IME6400_SD_MPEG4, IME6400_SD_H264};
enum IME6400_AUDIOMODE	{ IME6400_AM_MPEG1L2, IME6400_AM_MULAW, IME6400_AM_ADPCM };

typedef struct tag_SYSTEM_PARAM {
	// System information
	int nVidChannel;
	int bEnableAudio;
	int bEnableMeta;

	// Video information
	int	uVideoType;
	int nVidWidth[MAX_VID_CH];
	int nVidHeight[MAX_VID_CH];
	int n1stWidth;
	int n1stHeight;
	int n2ndWidth;
	int n2ndHeight;
	int n3rdWidth;
	int n3rdHeight;
	double fFrameRate;

	// Audio information
	int nAudioType;			//Audio encoding type
	int nAudioChannel;		//1: momo, 2: stereo
	int nSamplesPerSec;		//8KHz, 16KHz, ...
	int nAudioBitRate;		//96Kbps, 128Kbps,...
	int nAudioFrameSize;
	int nAudioBitPerSample;	//8bit, 16bit,...

} SYSTEM_PARAM;

typedef struct tagWAVEFORMATEX {
    WORD	wFormatTag;			/* format type */
    WORD	nChannels;			/* number of channels (i.e. mono, stereo...) */
    DWORD	nSamplesPerSec;		/* sample rate */
    DWORD	nAvgBytesPerSec;	/* for buffer estimation */
    WORD	nBlockAlign;		/* block size of data */
    WORD	wBitsPerSample;		/* number of bits per sample of mono data */
    WORD	cbSize;				/* the count in bytes of the size of */
								/* extra information (after cbSize) */
} WAVEFORMATEX;


typedef struct tagMPEGAUDIOWAVEFORMAT {
	WAVEFORMATEX  	wfx;
	unsigned short	fwHeadLayer;
	unsigned long	dwHeadBitrate;
	unsigned short	fwHeadMode;
	unsigned short	fwHeadModeExt;
	unsigned short	wHeadEmphasis;
	unsigned short	fwHeadFlags;
	unsigned long	dwPTSLow;
	unsigned long	dwPTSHigh;
} MPEGAUDIOWAVEFORMAT;


typedef struct tagBITMAPINFOHEADER {
    DWORD	biSize;
    LONG	biWidth;
    LONG	biHeight;
    WORD	biPlanes;
    WORD	biBitCount;
    DWORD	biCompression;
    DWORD	biSizeImage;
    LONG	biXPelsPerMeter;
    LONG	biYPelsPerMeter;
    DWORD	biClrUsed;
    DWORD	biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagAVIINDEXENTRY {
    DWORD	ckid;
    DWORD	dwFlags;
    DWORD	dwChunkOffset;		// Position of chunk
    DWORD	dwChunkLength;		// Length of chunk
} AVIINDEXENTRY;

#endif // !defined(AFX_BLOCKBUFFER_H__0CF63682_769F_4CF8_B3F4_A0D61958E7A0__INCLUDED_)
