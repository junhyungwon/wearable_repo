/*
 * Copyright (C) 2005-2006 WIS Technologies International Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and the associated README documentation file (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
// An interface to the WIS GO7007 capture device.
// Implementation

#include "NETRAInput.hh"
//#include "Options.hh"
#include "Err.hh"
#include "Base64.hh"
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <linux/soundcard.h>

#include <time.h>

extern unsigned audioSamplingFrequency;
extern unsigned audioNumChannels;
extern int audio_enable;
extern void sigterm(int dummy);

#define TIME_GET_WAY 1

#define ENABLE_WRITE_RAW_DATE 0
#define JPEG_DUMP             0


////////// OpenFileSource definition //////////

// A common "FramedSource" subclass, used for reading from an open file:

class OpenFileSource: public FramedSource {
public:
  int  uSecsToDelay;
  int  uSecsToDelayMax;
  int  srcType;
protected:
  OpenFileSource(UsageEnvironment& env, NETRAInput& input);
  virtual ~OpenFileSource();

  virtual int readFromFile() = 0;

private: // redefined virtual functions:
 	virtual unsigned int maxFrameSize() const;
    virtual void doGetNextFrame();

private:
  static void incomingDataHandler(OpenFileSource* source);
  void incomingDataHandler1();

protected:
  NETRAInput& fInput;
  
//  int fFileNo;
};


////////// VideoOpenFileSource definition //////////

class VideoOpenFileSource: public OpenFileSource {
public:
  VideoOpenFileSource(UsageEnvironment& env, NETRAInput& input);
  virtual ~VideoOpenFileSource();

protected: // redefined virtual functions:
  virtual int readFromFile();
  virtual int readFromFile264();
  unsigned int SerialBook;
  unsigned int SerialLock;
  int StreamFlag;
  struct timeval fPresentationTimePre;
  int IsStart;
};

#define STREAM_GET_VOL    0x0001
#define STREAM_NEW_GOP    0x0002

////////// AudioOpenFileSource definition //////////

class AudioOpenFileSource: public OpenFileSource {
public:
  AudioOpenFileSource(UsageEnvironment& env, NETRAInput& input);
  virtual ~AudioOpenFileSource();

protected: // redefined virtual functions:
  virtual int readFromFile();
  int getAudioData();

  unsigned int AudioBook;
  unsigned int AudioLock;
  struct timeval fPresentationTimePre;
  int IsStart;

};

long timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}

void printErr(UsageEnvironment& env, char const* str = NULL) {
  if (str != NULL) err(env) << str;
  env << ": " << strerror(env.getErrno()) << "\n";
}

////////// NETRAInput implementation //////////

NETRAInput* NETRAInput::createNew(UsageEnvironment& env, int vType) {
  return new NETRAInput(env, vType);
}

FramedSource* NETRAInput::videoSource() {

  if (fOurVideoSource == NULL) {
    fOurVideoSource = new VideoOpenFileSource(envir(), *this);
  }

  return fOurVideoSource;
}

FramedSource* NETRAInput::audioSource() {

  if (fOurAudioSource == NULL) {
    fOurAudioSource = new AudioOpenFileSource(envir(), *this);
  }

  return fOurAudioSource;
}

NETRAInput::NETRAInput(UsageEnvironment& env, int vType)
  : Medium(env), videoType(vType), channel(vType), fOurVideoSource(NULL), fOurAudioSource(NULL) {
}

NETRAInput::~NETRAInput() {
 if( fOurVideoSource )
 {
 	delete (VideoOpenFileSource *)fOurVideoSource;
 	fOurVideoSource = NULL;
 }
 
 if( fOurAudioSource )
 {
 	delete (AudioOpenFileSource *)fOurAudioSource;
 	fOurAudioSource = NULL;
 }
 	
}

#include <stdio.h>
#include <stdlib.h>
FILE *pFile = NULL;
void OpenFileHdl(void)
{
	if( pFile == NULL )
	{
		pFile = fopen("test.264", "rb");
		if( pFile == NULL )
		{
			fprintf(stderr,"h264 open file fail!!\n");
		}
	}
}

void CloseFileHdl(void)
{
	if( pFile != NULL )
	{
		fclose(pFile);
		pFile = NULL;
	}
}


char FrameBuff[1024*1024];

int NAL_Search(int offsetNow)
{
	unsigned long testflg = 0;
	int IsFail = 0;

	for(;;)
	{
		fseek(pFile, offsetNow, SEEK_SET);
		if( fread(&testflg, sizeof(testflg), 1, pFile) <=  0 )
		{
			IsFail = -1;
			break;
		}

		//printf("testflg=0x%x \n",(int)testflg );
		
		if( testflg == 0x01000000 )
		{
			break;
		}
		
		
		offsetNow++;
		
	}
	if( IsFail != 0 )
		return IsFail;
		
	return offsetNow;
	
}

void *GetFileFrame(int *pSize,int IsReset)
{
	static int offset = 0;
	int offset1 = 0;
	int offset2 = 0;
	int framesize = 0;

	*pSize = 0;

	if( pFile == NULL )
		return NULL;

	if( IsReset == 1 )
	{
		offset = 0;
		fseek(pFile, 0, SEEK_SET);
	}
	
	if( pFile )
	{
		fseek(pFile, offset, SEEK_SET);
		
		offset1 = NAL_Search(offset);
		offset2 = NAL_Search(offset1+4);
		
		
		framesize = offset2 - offset1;
		printf("framesize=%d\n", framesize);

		/*reset position*/
		fseek(pFile, offset1, SEEK_SET);
		fread(FrameBuff, framesize, 1, pFile);

		offset = offset2;

		*pSize = framesize;
	}

	return FrameBuff;
}



////////// OpenFileSource implementation //////////

OpenFileSource
::OpenFileSource(UsageEnvironment& env, NETRAInput& input)
  : FramedSource(env),
    fInput(input) {
}

OpenFileSource::~OpenFileSource() {
	CloseFileHdl();
}

unsigned int OpenFileSource::maxFrameSize() const
{
	return 2000*1000 ;
//	return 0; // unlimited
}

void OpenFileSource::doGetNextFrame() {
	incomingDataHandler(this);

}

void OpenFileSource
::incomingDataHandler(OpenFileSource* source) {
  source->incomingDataHandler1();
}

void OpenFileSource::incomingDataHandler1() {
	int ret;

	if (!isCurrentlyAwaitingData())	return; // we're not ready for the data yet

	ret = readFromFile();

	if (ret < 0) {
		handleClosure(this);
		fprintf(stderr,"In Grab Image, the source stops being readable!!!!\n");
	}

	else if (ret == 0) 
	{
		if( uSecsToDelay >= uSecsToDelayMax )
		{
			uSecsToDelay = uSecsToDelayMax;
		}else{
			uSecsToDelay *= 2; 
		}
		nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
			      (TaskFunc*)incomingDataHandler, this);

	}
	else {
		nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
	}
}

//RTSP_EXT_CHANNEL
static int av_field[VIDEO_TYPE_MAX_NUM][7] = {
	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},

	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},
	
/*
	{AV_OP_LOCK_MP4_CIF_VOL,
	AV_OP_UNLOCK_MP4_CIF_VOL,
	AV_OP_LOCK_MP4_CIF,
	AV_OP_LOCK_MP4_CIF_IFRAME,
	AV_OP_UNLOCK_MP4_CIF,
	AV_OP_GET_MPEG4_CIF_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL},
	
	{AV_OP_LOCK_MP4_VOL,
	AV_OP_UNLOCK_MP4_VOL,
	AV_OP_LOCK_MP4,
	AV_OP_LOCK_MP4_IFRAME,
	AV_OP_UNLOCK_MP4,
	AV_OP_GET_MPEG4_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_SERIAL},
	
	{AV_OP_LOCK_MP4_CIF_VOL,
	AV_OP_UNLOCK_MP4_CIF_VOL,
	AV_OP_LOCK_MP4_CIF,
	AV_OP_LOCK_MP4_CIF_IFRAME,
	AV_OP_UNLOCK_MP4_CIF,
	AV_OP_GET_MPEG4_CIF_SERIAL,
	AV_OP_WAIT_NEW_MPEG4_CIF_SERIAL},
*/	
};

#define AV_LOCK_MP4_VOL	0
#define AV_UNLOCK_MP4_VOL 1
#define AV_LOCK_MP4		2
#define AV_LOCK_MP4_IFRAME	3
#define AV_UNLOCK_MP4	4
#define AV_GET_MPEG4_SERIAL 5
#define AV_WAIT_NEW_MPEG4_SERIAL 6

////////// VideoOpenFileSource implementation //////////

VideoOpenFileSource
::VideoOpenFileSource(UsageEnvironment& env, NETRAInput& input)
  : OpenFileSource(env, input), SerialBook(0), SerialLock(0), StreamFlag(STREAM_GET_VOL),IsStart(1) {

 uSecsToDelay = 30000;     // 5ms
 uSecsToDelayMax = 50000;
// uSecsToDelay = 10000;     // 5ms
// uSecsToDelayMax = 160000;
 srcType = 0;


}

VideoOpenFileSource::~VideoOpenFileSource() {

  fInput.fOurVideoSource = NULL;

  if ((fInput.videoType <= VIDEO_TYPE_MAX_NUM) && SerialLock > 0)
  {
    printf("GetAVData  calll 14141414 field value = %d\n",av_field[fInput.videoType][AV_UNLOCK_MP4]) ;
    GetAVData(fInput.channel, av_field[fInput.videoType][AV_UNLOCK_MP4], SerialLock, NULL);
  }
  SerialLock = 0;

}

int VideoOpenFileSource::readFromFile264()
{
#if 1
	void *pframe = NULL;
	int framesize = 0;
	int ret = 0;
	int offset = 0;
	OpenFileHdl();

	if (StreamFlag & STREAM_GET_VOL)
	{
		pframe = GetFileFrame(&framesize,1);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;

		StreamFlag &= ~(STREAM_GET_VOL|STREAM_NEW_GOP);
		StreamFlag |= STREAM_NEW_GOP;

	}
	else if (StreamFlag & STREAM_NEW_GOP) {
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
		pframe = GetFileFrame(&framesize,0);
		memcpy(fTo+offset, pframe, framesize);
		offset += framesize;
	}

	if (1) {
		fFrameSize = offset;
		if (fFrameSize > fMaxSize) {
			printf("readFromFile264:Frame Truncated\n");
			printf("fFrameSize = %d\n",fFrameSize);
			printf("fMaxSize = %d\n",fMaxSize);
			printf("fNumTruncatedBytes = %d\n",fFrameSize - fMaxSize);
			fNumTruncatedBytes = fFrameSize - fMaxSize;
			fFrameSize = fMaxSize;
		}
		else {
			fNumTruncatedBytes = 0;
		}
		//memcpy(fTo+offset, av_data.ptr, fFrameSize-offset);


		// Note the timestamp and size:
		gettimeofday(&fPresentationTime, NULL);

		return 1;
	}
	else if (ret == RET_NO_VALID_DATA) {
		return 0;
	}
	else {
		StreamFlag |= STREAM_NEW_GOP;
		return 0;
	}
#else
	return 0;
#endif

}


int GetVolInfo(int vType,void *pBuff,int bufflen)
{
    int ret, Serial ;
	int *cmd = av_field[vType];
	AV_DATA av_data;
/*
    static FILE *FP = NULL ;
    FP = fopen("/mmc/test.264","w");
*/
	if(GetAVData(vType, cmd[AV_LOCK_MP4_VOL], -1, &av_data ) != RET_SUCCESS)
	{
		printf("Error on Get Vol data\n");
		return -1 ;
	}
 	GetAVData(vType, cmd[AV_UNLOCK_MP4_VOL], -1, &av_data);

	GetAVData(vType, cmd[AV_LOCK_MP4_IFRAME], -1, &av_data) ;
	memcpy(pBuff, av_data.ptr, av_data.size) ;
	Serial = av_data.serial ;

    GetAVData(vType, cmd[AV_UNLOCK_MP4], Serial, &av_data);

/*
    fwrite(av_data.ptr, av_data.size, 1, FP) ;
	fflush(FP);
    fclose(FP);
*/
/*
	printf("GetAVData  calll field 13131313 value = %d\n",cmd[AV_LOCK_MP4_VOL]) ;
	if(GetAVData(vType, cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS) {
		printf("Error on Get Vol data\n");
		return 0;
	}

	memcpy(pBuff, vol_data.ptr, vol_data.size);
    printf("GetAVData  calll field 14141414 value = %d\n",cmd[AV_UNLOCK_MP4_VOL]) ;
	GetAVData(vType, cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);
*/
	return av_data.size;
}

int GetSprop(void *pBuff, char vType)
{
	int i ;
	static char tempBuff[512] = {0 ,};
	int ret = 0;
	int cnt = 0;
	int IsSPS = 0;
	int IsPPS = 0;
	int SPS_LEN = 82;
	int PPS_LEN = 5;

	char *pSPS=tempBuff;//0x7
	char *pPPS=tempBuff;//0x8
	char *pSPSEncode=NULL;
	char *pPPSEncode=NULL;

printf("GetSprop... reached......\n") ;

	ret = GetVolInfo(vType, tempBuff,sizeof(tempBuff));

    printf("av_data.size = %d\n",ret) ;

	for(;;)
	{
		if(pSPS[0]==0&&pSPS[1]==0&&pSPS[2]==0&&pSPS[3]==1)
		{
			if( (pSPS[4]& 0x1F) == 7 )
			{
				printf("SPS........................\n");
				IsSPS = 1;
				break;
			}
		}
		pSPS++;
		cnt++;
		if( (cnt+4)>ret )
			break;
	}
	if(IsSPS)
		pSPS += 4;

	cnt = 0;
	for(;;)
	{
		if(pPPS[0]==0&&pPPS[1]==0&&pPPS[2]==0&&pPPS[3]==1)
		{
			if( (pPPS[4]& 0x1F) == 8 )
			{
				printf("PPS........................\n");
				IsPPS = 1;
				break;
			}
		}
		pPPS++;
		cnt++;
		if( (cnt+4)>ret )
			break;
	}

	if(IsPPS)
		pPPS += 4;

	SPS_LEN = (unsigned int)pPPS - (unsigned int)pSPS;

	pSPSEncode = base64Encode((char *)pSPS,SPS_LEN);
	pPPSEncode = base64Encode((char *)pPPS,PPS_LEN);

	sprintf((char *)pBuff,"%s,%d,%s,%d",(char *)pSPSEncode, SPS_LEN, (char *)pPPSEncode, PPS_LEN);
//	sprintf((char *)pBuff,"%s, %s",(char *)pSPSEncode, (char *)pPPSEncode);
//	printf("vType = %d pBuff = %s \n",vType,(char *)pBuff);

	delete[] pPPSEncode;
	delete[] pSPSEncode;

	return 1;
}


static char mpeg4_header[] = {0x00, 0x00, 0x01, 0xb0, 0x01, 0x00, 0x00, 0x01, 0xb5, 0x09};


void printheader(char *pData, int size)
{
	int cnt = 0;
	
	printf("printheader = %d\n",size);
	for(cnt = 0 ;cnt < size; cnt++ )
	{
		
		printf(" 0x%X ",*pData++);
		if( cnt!=0 && cnt%4 == 0 )
		printf("\n");
	}
}
int WaitVideoStart( int vType,int sleep_unit )
{
	AV_DATA av_data;
	int cnt = 0;
	int serialnum = -1;
	int *cmd = av_field[vType];

	while(1)
	{
		av_data.serial = -1;
        printf("GetAVData  calll 11111111 field value = %d\n",cmd[AV_GET_MPEG4_SERIAL]) ;
		GetAVData(vType, cmd[AV_GET_MPEG4_SERIAL], -1, &av_data );

		if( (int)av_data.serial < 0 )
		{
			printf("Stream %d is not avaliable~~~~~~~~\n",vType);

			break;
		}
		
		if( av_data.flags != AV_FLAGS_MP4_I_FRAME )  // 0x00001 First frame of GOP 0x00002 Last frame of GOP
		{
			printf("WaitVideoStart cnt = %d\n",cnt) ;
			usleep(sleep_unit);
		}else{
			printf("Stream Iframe...avaliable~~~~~~~~serial = %d\n",av_data.serial);
			serialnum = av_data.serial ;
			break;
		}
		cnt++;
		if( cnt > 100 )
			break;
	} 

	return serialnum;
}

int GetVideoSerial( int vType )
{
	AV_DATA av_data;
	int *cmd = av_field[vType];

    //printf("GetAVData  calll 222222 field value = %d\n",cmd[AV_GET_MPEG4_SERIAL]) ;
	GetAVData(vType, cmd[AV_GET_MPEG4_SERIAL], -1, &av_data );

	return av_data.serial;
}

#if JPEG_DUMP

int firstjpeg = 0 ;

#endif

int VideoOpenFileSource::readFromFile()
{
	AV_DATA av_data;
	int ret;

    FILE *jfp = NULL ;

	if (fInput.videoType == VIDEO_TYPE_H264_CH2) 
	{
		av_data.serial = -1;
		GetAVData(fInput.channel, AV_OP_GET_MJPEG_SERIAL, -1, &av_data);

//printf("av_data.serial = %d\n",av_data.serial) ;
		if (av_data.serial <= SerialLock)
			return 0;
		if( (int)av_data.serial<0 )
		{
//			printf("read From File Stream %d is not avaliable~~~~~~~~\n",fInput.videoType);
//			sigterm(0);
			return 0;
//          continue ;
		}
        
//printf("wis-streamer readFromeFile AV_OP_LOCK_MJPEG ...........................\n") ;        	

		ret = GetAVData(fInput.channel, AV_OP_LOCK_MJPEG, av_data.serial, &av_data );
		if (ret == RET_SUCCESS) {
			fFrameSize = av_data.size;
			if (fFrameSize > fMaxSize) {
				printf("Frame Truncated\n");
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}
			else {
				fNumTruncatedBytes = 0;
			}
			memcpy(fTo, av_data.ptr, fFrameSize);
#if JPEG_DUMP
            if(!firstjpeg)
            {
                jfp = fopen("/mmc/DCIM/fitt.jpeg","w") ;
                firstjpeg = 1 ;
            }
            if(firstjpeg == 1)
            {
                fwrite((void *)fTo, fFrameSize, 1, jfp) ;
                fclose(jfp) ;
                firstjpeg = 2 ;
                printf("fclose......\n") ;
            }
#endif

			SerialLock = av_data.serial;
			//printf("av_data.serial = %d\n", av_data.serial);
			GetAVData(fInput.channel, AV_OP_UNLOCK_MJPEG, SerialLock, NULL);
			
			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			return 1;
		}
		return 0;
        	
	}
 	if (fInput.videoType == VIDEO_TYPE_H264_CH1) 
 	{
#if 1

#if ENABLE_WRITE_RAW_DATE 
		static FILE *fp = NULL;
		if (fp == NULL)
		{
			char fname[32];
			time_t cur_sec = time(NULL);
			struct tm *ptm = localtime(&cur_sec);
			sprintf(fname, "/mmc/test-%04d%02d%02d%02d%02d%02d.264", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			fp = fopen(fname, "w");
			if (fp == NULL)
			{
				fprintf(stderr, "h264 open file fail!!\n");
				exit(0);
			}
		}
#endif

		int offset = 0;

		int *cmd = av_field[fInput.videoType];

        //printf("readFromFile fInput.videoType = %d\n",fInput.videoType) ;

		if (StreamFlag & STREAM_GET_VOL)
		{
			AV_DATA vol_data;
//			printf("GetAVData  calll 33333 field value = %d\n", cmd[AV_LOCK_MP4_VOL]);
			if (GetAVData(fInput.channel, cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS)
			{
				printf("Error on Get Vol data\n");
				return -1;
			}
			memcpy(fTo + offset, vol_data.ptr, vol_data.size);
#if ENABLE_WRITE_RAW_DATE 
			fwrite(vol_data.ptr, vol_data.size, 1, fp);
#endif

			offset += vol_data.size;
			GetAVData(fInput.channel, cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

//			printf("WaitVideoStart 00000000000000000000000\n");
			WaitVideoStart(fInput.videoType, 100);

			ret = GetAVData(fInput.channel, cmd[AV_LOCK_MP4_IFRAME], -1, &av_data);
			SerialBook = av_data.serial;
//			printf("GetAVData  calll field 66666666 value = %d serial = %d av_data.flags = %d\n", cmd[AV_LOCK_MP4_IFRAME], SerialBook, av_data.flags);

			StreamFlag &= ~(STREAM_GET_VOL | STREAM_NEW_GOP);

	    }
		else if (StreamFlag & STREAM_NEW_GOP) {

//            printf("WaitVideoStart 1111111111111111111111111\n") ;
			WaitVideoStart( fInput.videoType,1000);

//            printf("GetAVData  calll field 70707070 value = %d\n",cmd[AV_LOCK_MP4_IFRAME]) ;
			ret = GetAVData(fInput.channel, cmd[AV_LOCK_MP4_IFRAME], -1, &av_data );
			SerialBook = av_data.serial;
			StreamFlag &= ~STREAM_NEW_GOP;
//			printf("I ret = %d av_data.size = %d serial = %d av_data.flags = %d\n",ret, av_data.size, SerialBook, av_data.flags) ;
		}
		else {
			ret = GetAVData(fInput.channel, cmd[AV_LOCK_MP4], SerialBook, &av_data );
//			printf("P ret = %d av_data.size = %d serial = %d av_data.flags = %d\n",ret, av_data.size, SerialBook, av_data.flags) ;
		}
 		if (ret == RET_SUCCESS)
 		{
			static int IscheckKey = 1;
 			if( av_data.flags == AV_FLAGS_MP4_I_FRAME && IscheckKey == 1 )
 			{
 				int serial_now;
 				serial_now = GetVideoSerial(fInput.videoType);
 				IscheckKey = 0;
//                printf("serial now = %d, SerialBook = %d\n",serial_now, SerialBook);
 				if( (serial_now - SerialBook) > 0 )
 				{
                    printf("GetAVData  calll 90909090 field value = %d\n",cmd[AV_UNLOCK_MP4]) ;
 					GetAVData(fInput.channel, cmd[AV_UNLOCK_MP4], SerialBook, &av_data);
 					StreamFlag |= STREAM_NEW_GOP;
 					return 0;
 				}
			}
 				IscheckKey = 1;
 		}
        	
#if ENABLE_WRITE_RAW_DATE 
			fwrite(av_data.ptr, av_data.size, 1, fp);
			fflush(fp);
			fclose(fp) ;
#endif

		if (ret == RET_SUCCESS) {
#if 0
			fFrameSize = av_data.size + offset;
			if (fFrameSize > fMaxSize) {
				printf("Frame Truncated\n");
				printf("av_data.size = %d\n", av_data.size);
				printf("fFrameSize   = %d\n", fFrameSize);
				printf("fMaxSize     = %d\n", fMaxSize);
			    printf("offset       = %d\n", offset);
				printf("fNumTruncatedBytes = %d\n", fFrameSize - fMaxSize);
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}
			else {
				fNumTruncatedBytes = 0;
			}


			memcpy(fTo+offset, av_data.ptr, fFrameSize-offset);
#else
//			memcpy(fTo, av_data.ptr, av_data.size);
			fFrameSize = av_data.size;
			if (fFrameSize > fMaxSize) {
				printf("av_data.size = %d\n", av_data.size);
				printf("fMaxSize     = %d\n", fMaxSize);
				fNumTruncatedBytes = fFrameSize - fMaxSize;
				fFrameSize = fMaxSize;
			}else {
				fNumTruncatedBytes = 0;
			}

			memcpy(fTo, av_data.ptr, fFrameSize);

#endif
			if (SerialLock > 0) {
//                printf("GetAVData  calll 77777777 field value = %d\n",cmd[AV_UNLOCK_MP4]) ;
				GetAVData(fInput.channel, cmd[AV_UNLOCK_MP4], SerialLock, &av_data);
			}

			SerialLock = SerialBook;
			
			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			SerialBook++;

			return 1;
		}
		else if (ret == RET_NO_VALID_DATA) 
		{
			printf("RET_NO_VALID_DATA... \n") ;
		    return 0;
		}
		else 
		{
		    StreamFlag |= STREAM_NEW_GOP;
		    return 0;
		}
#else
		    return readFromFile264();
#endif
 	}
	else
	{
		return 0;
	}
}


////////// AudioOpenFileSource implementation //////////

AudioOpenFileSource
::AudioOpenFileSource(UsageEnvironment& env, NETRAInput& input)
  : OpenFileSource(env, input), AudioBook(0), AudioLock(0), IsStart(1) {
  uSecsToDelay = 5000;
  uSecsToDelayMax = 125000;
  srcType = 1;
}

AudioOpenFileSource::~AudioOpenFileSource() {
  fInput.fOurAudioSource = NULL;
  if (AudioLock > 0) {
    printf("GetAVData  calll 88888888 field value = %d\n",AV_OP_UNLOCK_ULAW) ;
    GetAVData(fInput.channel, AV_OP_UNLOCK_ULAW, AudioLock, NULL);
    AudioLock = 0;
  }

}

int AudioOpenFileSource::getAudioData() {
	AV_DATA av_data;
	int ret;

	if (AudioBook == 0) {
        printf("GetAVData  calll 9999999 field value = %d\n",AV_OP_GET_ULAW_SERIAL) ;
		GetAVData(fInput.channel, AV_OP_GET_ULAW_SERIAL, -1, &av_data );
		printf("av_data.serial = %d <= audio_lock = %d!!!\n", av_data.serial, AudioLock);
		if (av_data.serial <= AudioLock) {
			printf("av_data.serial = %d <= audio_lock = %d!!!\n", av_data.serial, AudioLock);
			return 0;
		}
		AudioBook = av_data.serial;
	}
    //printf("GetAVData  calll 10101010 field value = %d\n",AV_OP_LOCK_ULAW) ;
	ret = GetAVData(fInput.channel, AV_OP_LOCK_ULAW, AudioBook, &av_data );

	if (ret == RET_SUCCESS) {
		if (AudioLock > 0) {
            printf("GetAVData  calll field 12121212 value = %d\n",AV_OP_UNLOCK_ULAW) ;
			GetAVData(fInput.channel, AV_OP_UNLOCK_ULAW, AudioLock, NULL);
			AudioLock = 0;
		}
		if (av_data.size > fMaxSize)
			av_data.size = fMaxSize;
		memcpy(fTo, av_data.ptr, av_data.size);

		AudioLock = av_data.serial;
		AudioBook = av_data.serial + 1;
#if TIME_GET_WAY
		gettimeofday(&fPresentationTime, NULL);
#else
		fPresentationTime.tv_sec = av_data.timestamp/1000;
		fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
 		return av_data.size;
	}
	else if (ret == RET_NO_VALID_DATA) {
//        printf("AUDIO RET_NO_VALID_DATA...\n") ;
		return 0;
	}
	else {
printf("AudioBook = 0....... \n") ;
		AudioBook = 0;
		dbg("ERROR, ret=%d\n", ret);
		return -1;
	}
}


int AudioOpenFileSource::readFromFile() {
  int timeinc;

  if (!audio_enable)  return 0;

  // Read available audio data:
  int ret = getAudioData();

  if (ret <= 0) return 0;
  if (ret < 0) ret = 0;
  fFrameSize = (unsigned)ret;
  fNumTruncatedBytes = 0;

#if (TIME_GET_WAY)
/* PR#2665 fix from Robin
   * Assuming audio format = AFMT_S16_LE
   * Get the current time
   * Substract the time increment of the audio oss buffer, which is equal to
   * buffer_size / channel_number / sample_rate / sample_size ==> 400+ millisec
   */
 timeinc = fFrameSize * 1000 / audioNumChannels / (audioSamplingFrequency/1000) ;/// 2;
 while (fPresentationTime.tv_usec < timeinc)
 {
   fPresentationTime.tv_sec -= 1;
   timeinc -= 1000000;

 }

 fPresentationTime.tv_usec -= timeinc;

#else
  timeinc = fFrameSize*1000 / audioNumChannels / (audioSamplingFrequency/1000);
  if( IsStart )
  {
  	IsStart = 0;
  	fPresentationTimePre = fPresentationTime;
  	fDurationInMicroseconds = timeinc;
  }else{
	fDurationInMicroseconds = timevaldiff(&fPresentationTimePre, &fPresentationTime )*1000;
	fPresentationTimePre = fPresentationTime;
  }


  if( fDurationInMicroseconds < timeinc)
  {
  	unsigned long msec;
  	//printf("1.fPresentationTime.tv_sec = %d fPresentationTime.tv_usec = %d \n",fPresentationTime.tv_sec,fPresentationTime.tv_usec);
	msec = fPresentationTime.tv_usec;
  	msec += (timeinc - fDurationInMicroseconds);
	fPresentationTime.tv_sec += msec/1000000;
	fPresentationTime.tv_usec = msec%1000000;
	//printf("2.fPresentationTime.tv_sec = %d fPresentationTime.tv_usec = %d \n",fPresentationTime.tv_sec,fPresentationTime.tv_usec);
	fDurationInMicroseconds = timeinc;

	fPresentationTimePre = fPresentationTime;
  }
#endif


  return 1;
}

