/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_avi.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_AVI_H_
#define _APP_AVI_H_

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define LIBAVI_ERROR	(-1)
#define LIBAVI_SUCCESS	(0)

#define MAX_VID_CHANN	4		//# must be same as MAX_VID_CH

typedef enum {
    DATA_TYPE_VIDEO,
    DATA_TYPE_AUDIO,
    DATA_TYPE_META,
    DATA_TYPE_MAX
} enumDVR_DATA_TYPE;

typedef enum {
	NSDVR_PICTURE_TYPE_I,
	NSDVR_PICTURE_TYPE_P,
	NSDVR_PICTURE_TYPE_B,
} enumNSDVR_PICTURE_TYPE;

typedef enum {
    ENCODING_UNKNOWN=0,

    /* Video */
    ENCODING_MPEG4=1,
    ENCODING_JPEG,
    ENCODING_H264,

    /* Audio */
    ENCODING_PCM=32,
    ENCODING_ULAW,
    ENCODING_ALAW,
    ENCODING_ADPCM,
    ENCODING_G721,
    ENCODING_G723,
    ENCODING_MP3,

    MAX_ENCODING_TYPE
} enumDVR_ENCODING_TYPE;

typedef struct tag_AVI_SYSTEM_PARAM {
	//# System information
	int nVidCh;
	int bEnAudio;
	int bEnMeta;

	// Video information
	int uVideoType;
	int nVidWi[MAX_VID_CHANN];
	int nVidHe[MAX_VID_CHANN];
	double fFrameRate;

	// Audio information
	int nAudioType;			//Audio encoding type
	int nAudioChannel;		//1: momo, 2: stereo
	int nSamplesPerSec;		//8KHz, 16KHz, ...
	int nAudioBitRate;		//96Kbps, 128Kbps,...
	int nAudioFrameSize;
	int nAudioBitPerSample;	//8bit, 16bit,...
} AVI_SYSTEM_PARAM;

typedef struct tag_AVI_FRAME_PARAM {
	char *buf;
	int size;

	int channel;
	int data_type;		//0: video_main, 1: video_sub, 2: audio
	int iskey_frame;
	int time_sec;
	int time_usec;
} AVI_FRAME_PARAM;

/*----------------------------------------------------------------------------
 AVI file format - for playback
-----------------------------------------------------------------------------*/
typedef int				BOOL;
typedef long			LONG;
typedef unsigned long 	DWORD;
typedef unsigned short 	WORD;
typedef unsigned char 	BYTE;

typedef DWORD	FOURCC;
typedef WORD	TWOCC;

#define mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |	\
	( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )

#define aviTWOCC(ch0, ch1) ((WORD)(BYTE)(ch0) | ((WORD)(BYTE)(ch1) << 8))

/* form types, list types, and chunk types */
#define formtypeRIFF			mmioFOURCC('R', 'I', 'F', 'F')
#define formtypeLIST			mmioFOURCC('L', 'I', 'S', 'T')
#define formtypeAVI				mmioFOURCC('A', 'V', 'I', ' ')
#define listtypeAVIHEADER		mmioFOURCC('h', 'd', 'r', 'l')
#define ckidAVIMAINHDR			mmioFOURCC('a', 'v', 'i', 'h')
#define listtypeSTREAMHEADER	mmioFOURCC('s', 't', 'r', 'l')
#define ckidSTREAMHEADER		mmioFOURCC('s', 't', 'r', 'h')
#define ckidSTREAMFORMAT		mmioFOURCC('s', 't', 'r', 'f')

#define listtypeAVIMOVIE		mmioFOURCC('m', 'o', 'v', 'i')
#define listtypeAVIRECORD		mmioFOURCC('r', 'e', 'c', ' ')

#define ckidAVINEWINDEX			mmioFOURCC('i', 'd', 'x', '1')

/* Basic chunk types */
#define cktypeDIBbits			aviTWOCC('d', 'b')
#define cktypeDIBcompressed		aviTWOCC('d', 'c')
#define cktypePALchange			aviTWOCC('p', 'c')
#define cktypeWAVEbytes			aviTWOCC('w', 'b')
#define cktypeTEXT				aviTWOCC('t', 'x')

//# avi index dwFlags
#define AVIIF_LIST				0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME			0x00000010L // this frame is a key frame.
#define AVIIF_NOTIME			0x00000100L // this frame doesn't take any time
#define AVIIF_COMPUSE			0x0FFF0000L // these bits are for compressor use

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
} CK;

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckCodec;
} CK_RIFF;

typedef struct {
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckType;
} CK_LIST;

typedef struct {
	unsigned long dwMicroSecPerFrame;
	unsigned long dwMaxBytesPerSec;
	unsigned long dwReserved1;
	unsigned long dwFlags;
	unsigned long dwTotalFrames;
	unsigned long dwInitialFrames;
	unsigned long dwStreams;
	unsigned long dwSuggestedBufferSize;
	unsigned long dwWidth;
	unsigned long dwHeight;
	unsigned long dwScale;
	unsigned long dwRate;
	unsigned long dwStart;
	unsigned long dwLength;
} AVIMainHeader;

//# Header Block
typedef struct {
	CK_RIFF		ck_riff;	// RIFF tag
	CK_LIST		ck_hdrl;	// LIST tag
	CK			ck_avih;	// AVI header
	AVIMainHeader avih;
} AVIHeadBlock;

typedef struct {
	CK_LIST	ck_movi;
	CK		ck_idx1;
} AVITailBlock;

typedef struct {
    DWORD		ckid;
    DWORD		dwFlags;
    DWORD		dwChunkOffset;		// Position of chunk
    DWORD		dwChunkLength;		// Length of chunk
} AVIINDEXENTRY;

//# AVI File Information
typedef struct {
	AVIHeadBlock Head;
	AVITailBlock Tail;

	unsigned long movi_pos;
	unsigned long idx1_pos;
	unsigned long strm_pos;

	AVIINDEXENTRY *idx_entry;
	unsigned long idx_cnt;

	FILE *pFile;
} AVIFile;

/*----------------------------------------------------------------------------
 Jpeg file format
-----------------------------------------------------------------------------*/
typedef struct {
	char mark[2];
	char reserved[5];
	char he[2];
	char wi[2];
} JpegFile;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void* LIBAVI_createAvi(char* filePullPath, AVI_SYSTEM_PARAM* pAviPrm);
int LIBAVI_write_frame(void* handlAvi, AVI_FRAME_PARAM* pframePrm);
int LIBAVI_closeAvi(void* handlAvi);

int LIBAVI_recoverFile(char* filePullPath);
char* LIBAVI_recoverGetErrMsg();

#endif	/* _APP_AVI_H_ */
