#include "WW_H264VideoSource.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define BUFFER_SIZE   PIPE_BUF
#define REV_BUF_SIZE  (1024*1024)

#define STREAM_GET_VOL    0x0001
#define STREAM_NEW_GOP    0x000

//RTSP_EXT_CHANNEL
enum{
	VIDEO_TYPE_H264_CH1	= 0,
	VIDEO_TYPE_H264_CH2,
	VIDEO_TYPE_MAX_NUM 
};
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
};

#define AV_LOCK_MP4_VOL	0
#define AV_UNLOCK_MP4_VOL 1
#define AV_LOCK_MP4		2
#define AV_LOCK_MP4_IFRAME	3
#define AV_UNLOCK_MP4	4
#define AV_GET_MPEG4_SERIAL 5
#define AV_WAIT_NEW_MPEG4_SERIAL 6


static int GetVideoSerial( int vType )
{
	AV_DATA av_data;
	int *cmd = av_field[vType];

	GetAVData(vType, cmd[AV_GET_MPEG4_SERIAL], -1, &av_data );

	return av_data.serial;
}

static int WaitVideoStart( int vType,int sleep_unit )
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
			continue ;
		}
		
		if( av_data.flags != AV_FLAGS_MP4_I_FRAME )
		{
			usleep(sleep_unit);
		}else{
		
			serialnum = av_data.serial ;
			
			break;
		}
		cnt++;
		if( cnt > 10000 )
			break;
	} 

	return serialnum;
}



WW_H264VideoSource::WW_H264VideoSource(UsageEnvironment & env, int vType) : 
FramedSource(env),
m_pToken(0)
{
    m_nStreamFlag = STREAM_GET_VOL;
    m_nVideoType = vType;
    m_nChannel   = vType;
    m_nSerialBook = 0;
    m_nSerialLock = 0;
}

WW_H264VideoSource::~WW_H264VideoSource(void)
{
	envir().taskScheduler().unscheduleDelayedTask(m_pToken);

	printf("[MEDIA SERVER] rtsp connection closedn");
}

unsigned int WW_H264VideoSource::maxFrameSize() const
{
	return 1024*2000;
	//return 0; // 0 means unlimited
}

void WW_H264VideoSource::doGetNextFrame()
{
	// fps
	double delay = 1000.0 / (FRAME_PER_SEC * 2);  // ms
	int to_delay = delay * 1000;  // us

	m_pToken = envir().taskScheduler().scheduleDelayedTask(to_delay, getNextFrame, this);
}

void WW_H264VideoSource::getNextFrame(void * ptr)
{
	((WW_H264VideoSource *)ptr)->GetFrameData();
}

void WW_H264VideoSource::GetFrameData()
{
	if (!isCurrentlyAwaitingData()) return; // we're not ready for the data yet

	gettimeofday(&fPresentationTime, 0);

    fFrameSize = 0;

	static AV_DATA av_data;
	int ret;
 	if (m_nVideoType < VIDEO_TYPE_MAX_NUM)
    {

        int *cmd = av_field[m_nVideoType];
        if (m_nStreamFlag & STREAM_GET_VOL)
        {
			#if 0
            AV_DATA vol_data;
            printf("GetAVData  calll 33333 field value = %d\n", cmd[AV_LOCK_MP4_VOL]);
            if (GetAVData(m_nChannel, cmd[AV_LOCK_MP4_VOL], -1, &vol_data) != RET_SUCCESS)
            {
                printf("Error on Get Vol data\n");
                return;
            }

            printf("vol_data.frameType=%d, vol_data.size:%d\n", vol_data.frameType, vol_data.size);

            memcpy(fTo + offset, vol_data.ptr, vol_data.size);

            offset += vol_data.size;
            GetAVData(m_nChannel, cmd[AV_UNLOCK_MP4_VOL], -1, &vol_data);

            printf("vol_data.frameType=%d, vol_data.size:%d\n", vol_data.frameType, vol_data.size);
			#endif
            printf("WaitVideoStart 00000000000000000000000\n");
            WaitVideoStart(m_nVideoType, 300);

            printf("GetAVData  calll field 66666666 value = %d\n", cmd[AV_LOCK_MP4_IFRAME]);
            ret = GetAVData(m_nChannel, cmd[AV_LOCK_MP4_IFRAME], -1, &av_data);
            m_nSerialBook = av_data.serial;
            m_nStreamFlag &= ~(STREAM_GET_VOL | STREAM_NEW_GOP);
        }
		else if (m_nStreamFlag & STREAM_NEW_GOP) {

            printf("WaitVideoStart 1111111111111111111111111\n") ;
			WaitVideoStart(m_nVideoType, 5000);

            printf("GetAVData  calll field 70707070 value = %d\n",cmd[AV_LOCK_MP4_IFRAME]) ;
			ret = GetAVData(m_nChannel, cmd[AV_LOCK_MP4_IFRAME], -1, &av_data );
			m_nSerialBook = av_data.serial;
			m_nStreamFlag &= ~STREAM_NEW_GOP;
		}
		else {
			ret = GetAVData(m_nChannel, cmd[AV_LOCK_MP4], m_nSerialBook, &av_data );
		}

		//printf("ret = %d\n", ret);

 		if (ret == RET_SUCCESS)
 		{
			static int IscheckKey = 1;
 			if( av_data.flags == AV_FLAGS_MP4_I_FRAME && IscheckKey == 1 )
 			{
 				int serial_now = GetVideoSerial(m_nVideoType);
 				IscheckKey = 0;
 				if( (serial_now - m_nSerialBook) > 30 )
 				{
                    printf("GetAVData  calll 90909090 field value = %d\n",cmd[AV_UNLOCK_MP4]) ;
 					GetAVData(m_nChannel, cmd[AV_UNLOCK_MP4], m_nSerialBook, &av_data);
 					m_nStreamFlag |= STREAM_NEW_GOP;
 					//return;
 				}
			}
			else
			{
 				IscheckKey = 1;
 			}

            //printf("[MEDIA SERVER] GetFrameData len = [%d],fMaxSize = [%d]n",fFrameSize,fMaxSize);

            // fill frame data
            memcpy(fTo, av_data.ptr, av_data.size);

			fFrameSize = av_data.size;
			//printf("av_data.size = %d\n", av_data.size);
            if (fFrameSize > fMaxSize)
            {
				fNumTruncatedBytes = fFrameSize - fMaxSize;
                fFrameSize = fMaxSize;
				printf("fNumTruncatedBytes = %d\n", fNumTruncatedBytes );
            }
            else
            {
                fNumTruncatedBytes = 0;
            }

            if (m_nSerialLock > 0) {
                //printf("GetAVData  calll 77777777 field value = %d\n",cmd[AV_UNLOCK_MP4]) ;
				GetAVData(m_nChannel, cmd[AV_UNLOCK_MP4], m_nSerialLock, &av_data);
			}
			m_nSerialLock = m_nSerialBook;
			
			// Note the timestamp and size:
#if TIME_GET_WAY
			gettimeofday(&fPresentationTime, NULL);
#else
			fPresentationTime.tv_sec = av_data.timestamp/1000;
			fPresentationTime.tv_usec = (av_data.timestamp%1000)*1000;
#endif
			m_nSerialBook++;
		}
		else if (ret < RET_NO_VALID_DATA) 
		{
			handleClosure();
		    return;
		}
		else 
		{
		    m_nStreamFlag |= STREAM_NEW_GOP;
		}

		nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)afterGetting, this);
        //afterGetting(this);
    }
}
