// AVIEncoder.h: interface for the CAVIEncoder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _AVIENCODER_HH_
#define _AVIENCODER_HH_

#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blockbuffer.hh"
#include "mmsystem.h"

#define  WAVE_FORMAT_PCM					0x0001
#define  WAVE_FORMAT_ALAW       			0x0006  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_MULAW      			0x0007  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_IMA_ADPCM  			0x0011  /*  Intel Corporation  */
#define  WAVE_FORMAT_MPEG       			0x0050  /*  Microsoft Corporation  */
#define  WAVE_FORMAT_MPEGLAYER3 			0x0055  /*  ISO/MPEG Layer3 Format Tag */

// AVI (Standard) Data Structure - Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// AVI Header -------------------------------------------

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
} AVIMAINHEADER;

// Media Stream Header ----------------------------------

#define AVIF_HASINDEX						0x00000010
#define AVIF_MUSTUSEINDEX					0x00000020
#define AVIF_ISINTERLEAVED					0x00000100
#define AVIF_TRUSTCKTYPE					0x00000800
#define AVIF_WASCAPTUREFILE					0x00010000
#define AVIF_COPYRIGHTED					0x00020000

#define AVIF_KNOWN_FLAGS					0x00030130

#define Defined_MainAVIHeader_Size			(14*4)

#define AVISF_DISABLED						0x00000001
#define AVISF_VIDEO_PALCHANGES				0x00010000

#define AVISF_KNOWN_FLAGS					0x00010001

#define Defined_AVIStreamHeader_Size_old	(12*4)
#define Defined_AVIStreamHeader_Size		(14*4)


#define AVIIF_KNOWN_FLAGS					0x0fff0171

#define Defined_AVIINDEXENTRY_Size			(4*4)

typedef struct {
	unsigned long fccType;
	unsigned long fccHandler;
	unsigned long dwFlags;
	unsigned long dwPriority;
	unsigned long dwInitialFrames;
	unsigned long dwScale;
	unsigned long dwRate;
	unsigned long dwStart;
	unsigned long dwLength;
	unsigned long dwSuggestedBufferSize;
	unsigned long dwQuality;
	unsigned long dwSampleSize;
	unsigned short rcFrame[4];
} AVISTREAMHEADER;


#define ACM_MPEG_LAYER1				(0x0001)
#define ACM_MPEG_LAYER2				(0x0002)
#define ACM_MPEG_LAYER3				(0x0004)
#define ACM_MPEG_STEREO				(0x0001)
#define ACM_MPEG_JOINTSTEREO		(0x0002)
#define ACM_MPEG_DUALCHANNEL		(0x0004)
#define ACM_MPEG_SINGLECHANNEL		(0x0008)
#define ACM_MPEG_PRIVATEBIT			(0x0001)
#define ACM_MPEG_COPYRIGHT			(0x0002)
#define ACM_MPEG_ORIGINALHOME		(0x0004)
#define ACM_MPEG_PROTECTIONBIT		(0x0008)
#define ACM_MPEG_ID_MPEG1			(0x0010)

#define MPEGLAYER3_ID_UNKNOWN			0
#define MPEGLAYER3_ID_MPEG				1
#define MPEGLAYER3_ID_CONSTANTFRAMESIZE	2

#define MPEGLAYER3_FLAG_PADDING_ISO	0x00000000
#define MPEGLAYER3_FLAG_PADDING_ON	0x00000001
#define MPEGLAYER3_FLAG_PADDING_OFF	0x00000002

#define MPEGLAYER3_WFX_EXTRA_BYTES   12


// Chunk type --------------------------------------------------
typedef DWORD FOURCC;
typedef WORD TWOCC;

#ifndef mmioFOURCC
    #define mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) |	\
	( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )
#endif

/* Macro to make a TWOCC out of two characters */
#ifndef aviTWOCC
#define aviTWOCC(ch0, ch1) ((WORD)(BYTE)(ch0) | ((WORD)(BYTE)(ch1) << 8))
#endif


/* form types, list types, and chunk types */
#define formtypeENCRYPT					mmioFOURCC('L', 'I', 'N', 'K')
#define formtypeRIFF					mmioFOURCC('R', 'I', 'F', 'F')
#define formtypeLIST					mmioFOURCC('L', 'I', 'S', 'T')
#define formtypeAVI             		mmioFOURCC('A', 'V', 'I', ' ')
#define listtypeAVIHEADER       		mmioFOURCC('h', 'd', 'r', 'l')
#define listtypeODMLExtendedAVIHeader	mmioFOURCC('o', 'd', 'm', 'l')
#define ckidAVIMAINHDR          		mmioFOURCC('a', 'v', 'i', 'h')
#define ckidODMLExtendedAVIHeader		mmioFOURCC('d', 'm', 'l', 'h')
#define listtypeSTREAMHEADER			mmioFOURCC('s', 't', 'r', 'l')
#define ckidSTREAMHEADER        		mmioFOURCC('s', 't', 'r', 'h')
#define ckidSTREAMFORMAT        		mmioFOURCC('s', 't', 'r', 'f')
#define ckidSTREAMHANDLERDATA   		mmioFOURCC('s', 't', 'r', 'd')
#define ckidSTREAMNAME					mmioFOURCC('s', 't', 'r', 'n')
#define ckidVideoPropHeader				mmioFOURCC('v', 'p', 'r', 'p')

#define listtypeAVIMOVIE        		mmioFOURCC('m', 'o', 'v', 'i')
#define listtypeAVIRECORD       		mmioFOURCC('r', 'e', 'c', ' ')

#define ckidAVINEWINDEX         		mmioFOURCC('i', 'd', 'x', '1')

/*
** Stream types for the <fccType> field of the stream header.
*/
#define streamtypeVIDEO         		mmioFOURCC('v', 'i', 'd', 's')
#define streamtypeAUDIO         		mmioFOURCC('a', 'u', 'd', 's')
#define streamtypeMIDI					mmioFOURCC('m', 'i', 'd', 's')
#define streamtypeTEXT          		mmioFOURCC('t', 'x', 't', 's')

/* Basic chunk types */
#define cktypeDIBbits           		aviTWOCC('d', 'b')
#define cktypeDIBcompressed     		aviTWOCC('d', 'c')
#define cktypePALchange         		aviTWOCC('p', 'c')
#define cktypeWAVEbytes         		aviTWOCC('w', 'b')
#define cktypeTEXT						aviTWOCC('t', 'x')

/* Chunk id to use for extra chunks for padding. */
#define ckidAVIPADDING          		mmioFOURCC('J', 'U', 'N', 'K')


#define AVIIF_LIST          			0x00000001L // chunk is a 'LIST'
#define AVIIF_KEYFRAME      			0x00000010L // this frame is a key frame.

#define AVIIF_NOTIME	    			0x00000100L // this frame doesn't take any time
#define AVIIF_COMPUSE       			0x0FFF0000L // these bits are for compressor use


typedef struct
{
	FOURCC	ckID;
	DWORD	ckSize;
} CK;

typedef struct
{
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckCodec;
} CK_RIFF;

typedef struct
{
	FOURCC	ckID;
	DWORD	ckSize;
	DWORD	ckType;
} CK_LIST;



// AVI (Standard) Data Structure - End >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// Costom Data Structure - Start <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef void AVISTREAMFORMAT;

// Header Block
typedef struct 
{
	// RIFF tag
	CK_RIFF	ck_riff;
	// LIST tag
	CK_LIST ck_hdrl;
	// AVI header
	CK		ck_avih;
	AVIMAINHEADER avih;
} AVIHEADBLOCK;

typedef struct {
	unsigned long dwTotalFrames;
} ODMLExtendedAVIHeader;

typedef struct 
{
	CK_LIST ck_odml;
	CK		ck_dmlh;
	ODMLExtendedAVIHeader	odml;
} EAVIHEADBLOCK;

typedef struct {
	// LIST tag
	CK_LIST ck_strl;
	// Stream header tag
	CK	ck_strh;
	AVISTREAMHEADER AviStrHeader;
	// Stream format tag
	CK	ck_strf;
	AVISTREAMFORMAT* pAviStrFormat;
} AVISTREAMBLOCK;

typedef struct 
{
//	CK		ck_junk;
	CK_LIST	ck_movi;
	CK		ck_idx1;
} AVITAILBLOCK;

struct _INDEX{
	struct _INDEX *next;
	AVIINDEXENTRY data;
};

typedef struct _INDEX INDEX;

typedef struct {
	INDEX *first;
	INDEX *last;
	int size;
} INDEX_ROOT;


// Internal Information
typedef struct {
	FILE* pFile;
	unsigned long movi_size;
	unsigned long idx_cnt;
	long nOffset;
	unsigned long nIndexPos;
	unsigned long nHeadWritten;
	AVIHEADBLOCK Head;
//	EAVIHEADBLOCK EHead;
	AVITAILBLOCK Tail;
	INDEX_ROOT	IndexRoot;
} AVIFILE; 

typedef struct {
	AVIFILE* pAVIFile;
	unsigned long nTotalFrames;
	AVISTREAMBLOCK* pStreamBlock;
	CK	ck_data;
} AVISTREAM;

#define BK_AVIERR_OK		1
#define BK_AVIERR_ERROR		0

#define MOVI_POS_OFFSET		2048

// Costom Data Structure - End >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// Prototype /////////////////////////////////////////////////////////

int AVIFileOpen(AVIFILE** ppAVIFile, const char* pszFileName, int m_bEncrypt);
int AVIFileCreateStream(AVIFILE* pAVIFile, AVISTREAM** ppavi, AVISTREAMHEADER* psi);
int AVIFileCreateStreamSub(AVIFILE* pAVIFile, AVISTREAM** ppavi, AVISTREAMHEADER* psi);
int AVIStreamSetFormat(AVISTREAM* pavi, long lPos, void* lpFormat, long cbFormat);

int AVIStreamWrite(AVISTREAM* pavi, long lStart, long nFrameCnt, void* lpBuffer, long nFrameSize, DWORD dwFlags);

int AVIStreamWriteBuf(AVISTREAM* pavi, long lStart, long nFrameCnt, void* lpBuffer, long nFrameSize, DWORD dwFlags, CBlockBuffer* pBuffer);
int AVIStreamWriteBufFlush(AVISTREAM* pavi, CBlockBuffer* pBuffer);

int AVIFileMainHeaderWrite(AVIFILE* pAVIFile);
int AVIFileStreamHeaderWrite(AVISTREAM* pavi);
int AVIFileMoviHeaderWrite(AVIFILE*pAVIFile);

int AVIStreamClose(AVISTREAM* pavi);
int AVIFileClose(AVIFILE* pAVIFile);

#ifdef __cplusplus
}
#endif


#endif 
