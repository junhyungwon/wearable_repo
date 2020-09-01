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
// An application that streams audio/video captured by a WIS GO7007,
// using a built-in RTSP server.
// main program

#include <signal.h>
#include <BasicUsageEnvironment.hh>
#include <getopt.h>
#include <liveMedia.hh>
#include "Err.hh"
#include "WISJPEGVideoServerMediaSubsession.hh"
#include "WISMPEG4VideoServerMediaSubsession.hh"
#include "WISH264VideoServerMediaSubsession.hh"
#include "WISPCMAudioServerMediaSubsession.hh"
#include <NetraDrvMsg.h>
#include <Netra_interface.h>
#include <app_set.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <GroupsockHelper.hh>
#include "WISJPEGStreamSource.hh"
#include <Msg_Def.h>
#include "WW_H264VideoSource.h"
#include "WW_H264VideoServerMediaSubsession.h"

enum  StreamingMode
{
	STREAMING_UNICAST,
	STREAMING_UNICAST_THROUGH_DARWIN,
	STREAMING_MULTICAST_ASM,
	STREAMING_MULTICAST_SSM
};

enum{
	AUDIO_G711 = 0,
	AUDIO_AAC
};

portNumBits rtspServerPortNum = 8551;

typedef struct
{
        long mtype;
        unsigned short rtspServerPort_H264_PORT_1;
        unsigned short rtspServerPort_H264_PORT_2;
        unsigned short rtspServerPort_MPEG4_PORT_1;
        unsigned short rtspServerPort_MPEG4_PORT_2;
        unsigned short rtspServerPort_MJPEG_PORT;
        int size_Port;
        int RTSP_Enable;

} rtspServerPort_wis_streamer;

int RTSP_server_message_queue_id;
char const* H264StreamName[VIDEO_TYPE_MAX_NUM] = {"all","all"};
char const* MjpegStreamName = "mjpeg";
char const* Mpeg4StreamName = "mpeg4";
char const* streamDescription = "RTSP/RTP stream from NETRA";
char name[6] ;

int MjpegVideoBitrate = 1500000;
int Mpeg4VideoBitrate = 1500000;
int H264VideoBitrate = 100000;
int audioOutputBitrate = 128000;

unsigned audioSamplingFrequency = 16000;
unsigned audioNumChannels = 1;
int audio_enable = 0;
unsigned audioType = AUDIO_G711;
char watchVariable = 0;
char videoType = -1;
char IsAudioAlarm = 0;

int qid = 0, resol = 0;

void sigterm(int dummy)
{
	printf("caught SIGTERM: shutting down\n");
	NetraInterfaceExit();
	exit(1);
}

void sigint(int dummy)
{
	if( IsAudioAlarm == 1 )
	{
	 	printf("Audio Alarm!!\n");
		IsAudioAlarm = 0;
		return;
	}

	printf("caught SIGINT: shutting down\n");
	watchVariable = 1;
	alarm(1);
}

void sig_audio_enable(int dummy)
{
	printf("Audio enabled!\n");
	audio_enable = 1;
	IsAudioAlarm = 1;
	alarm(2);
}

void sig_audio_disable(int dummy)
{
	printf("Audio disabled!\n");
	audio_enable = 0;
	IsAudioAlarm = 1;
	alarm(2);
}

void init_signals(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sigaddset(&sa.sa_mask, SIGALRM);

	sa.sa_flags = 0;
	sa.sa_handler = sigterm;
	sigaction(SIGTERM, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sigint;
	sigaction(SIGINT, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sig_audio_enable;
	sigaction(SIGUSR1, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sig_audio_disable;
	sigaction(SIGUSR2, &sa, NULL);

	sa.sa_flags = 0;
	sa.sa_handler = sigint;
	sigaction( SIGALRM, &sa, NULL );
}

void share_memory_init(int msg_type)
{
    if(NetraDrvInit(msg_type))
        exit(1);
    if (func_get_mem(&qid, &resol, name )) {
  	NetraInterfaceExit();
	NetraDrvExit();
    exit(1);
    }
    printf("share_memory_init resol = %d\n",resol) ;
   
}
#include <fcntl.h>
int GetSampleRate( void )
{
	static int CurrentStatus = -255;
	int fd_proc = -1;
	int ret = -1;
	char StrBuffer[300];
	char TmpBuf[50];
	char *pStr = NULL;
	int samplerate = 0;
	char delima_buf[] = ":\r\n ";

	if( CurrentStatus >= -1 )
	{
		fprintf(stderr,"CurrentStatus is = %d \n", CurrentStatus);
		return CurrentStatus;
	}


	fd_proc = open("/proc/asound/card0/pcm0c/sub0/hw_params", O_RDONLY);
	if( fd_proc < 0 )
	{
		fprintf(stderr,"GetSampleRate open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_proc,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		fprintf(stderr,"read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "rate", sizeof("rate")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			sscanf( pStr,"%d",&samplerate);

			//fprintf(stderr,"samplerate = %d \n", samplerate);

			ret = samplerate;
			goto CHECK_CPU_END;


		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_proc >= 0 )
		close(fd_proc);

	CurrentStatus = ret;

	return ret;
}

extern int GetSprop(void *pBuff, char vType);
char BuffStr[200];
int main(int argc, char** argv) {
  init_signals();
  setpriority(PRIO_PROCESS, 0, 0);
  int IsSilence = 0;
  int OverHTTPEnable = 1;
  int svcEnable = 0, rtsp_port = 0, ret = 0 ;

  char rtsp_username[32] = {0, } ; 
  char rtsp_password[32] = {0, } ; 

  
/*
  if( GetSampleRate() == 16000 )
  {
	audioOutputBitrate = 128000;
	audioSamplingFrequency = 16000;
  }
  else
  {
	audioOutputBitrate = 64000;
	audioSamplingFrequency = 8000;
  }
*/
	audioOutputBitrate = 128000;
	audioSamplingFrequency = 16000;
 
  if(argc < 2)
  {
     rtsp_port = 8551 ;
  }
  else 
  {
     if(argc < 3)
         rtsp_port = atoi(argv[1]) ;
     if(argc > 3 )
     {  
         rtsp_port = atoi(argv[1]) ;
         strcpy(rtsp_username, argv[2] );        
         strcpy(rtsp_password, argv[3] );        
     }
  }

//printf("rtsp_port = %d\n",rtsp_port) ;
//printf("rtsp_username = %s\n",rtsp_username) ;
//printf("rtsp_password = %s\n",rtsp_password) ;

printf("RTSP audio sampling Frequency = %d\n",audioSamplingFrequency) ;
  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
  int msg_type, video_type;
  NETRAInput* MjpegInputDevice = NULL;
  NETRAInput* Mpeg4InputDevice = NULL;

//RTSP_EXT_CHANNEL
  NETRAInput* H264InputDevice[VIDEO_TYPE_MAX_NUM] = {NULL, NULL};//, NULL, NULL, NULL, NULL};
    
  static pid_t child[4] = { -1, -1 , -1, -1};

  StreamingMode streamingMode = STREAMING_UNICAST;
  netAddressBits multicastAddress = 0;//our_inet_addr("224.1.4.6");
  portNumBits videoRTPPortNum = 0;
  portNumBits audioRTPPortNum = 0;
  portNumBits OverHTTPPortNum = 0;

  IsSilence = 0;
  svcEnable = 0;
  audioType = AUDIO_G711;
  streamingMode = STREAMING_UNICAST;

 //////////////////////////////RTSP_EXT_CHANNEL//////////////////////
  child[1] = fork();

  printf("fork child[1] PID:%d\n", child[1]);

  if( child[1] == 0 )
  {
	  // parent, success //
	  msg_type = LIVE_MSG_TYPE;
	  video_type = VIDEO_TYPE_H264_CH1;
	  rtspServerPortNum = rtsp_port;
	  Mpeg4VideoBitrate = 100000;
	  videoRTPPortNum = 6000;
	  audioRTPPortNum = 6002;
	  OverHTTPPortNum = 8300;
	  streamingMode = STREAMING_MULTICAST_SSM;
  }
  if (child[1] != 0)
  {
	  // parent, success //
	  msg_type = LIVE_MSG_TYPE2;
	  video_type = VIDEO_TYPE_H264_CH2;
	  rtspServerPortNum = rtsp_port + 1;
	  Mpeg4VideoBitrate = 100000;
	  videoRTPPortNum = 6004;
	  audioRTPPortNum = 6006;
	  OverHTTPPortNum = 8301;
	  streamingMode = STREAMING_MULTICAST_SSM;
  }

  if (argc > 1)
  {
	  if (strcmp(argv[1], "-m") == 0)
	  {
		  streamingMode = STREAMING_MULTICAST_SSM;
	  }
	  else
	  {
		  streamingMode = STREAMING_UNICAST;
	  }
  }
  else
  {
	  streamingMode = STREAMING_UNICAST;
  }

  videoType = video_type;

  // Objects used for multicast streaming:
  static Groupsock* rtpGroupsockAudio = NULL;
  static Groupsock* rtcpGroupsockAudio = NULL;
  static Groupsock* rtpGroupsockVideo = NULL;
  static Groupsock* rtcpGroupsockVideo = NULL;
  static FramedSource* sourceAudio = NULL;
  static RTPSink* sinkAudio = NULL;
  static RTCPInstance* rtcpAudio = NULL;
  static FramedSource* sourceVideo = NULL;
  static RTPSink* sinkVideo = NULL;
  static RTCPInstance* rtcpVideo = NULL;

  share_memory_init(msg_type);

  if(strcmp(name, "all") != 0)
      strcpy((char *)H264StreamName[0] ,name) ; 

  //init_signals();

  *env << "Initializing...\n";
    OutPacketBuffer::maxSize = 100000; // allow for some possibly large H.264 frames
	setVideoRTPSinkBufferSize();

  // Initialize the WIS input device:
	H264InputDevice[video_type] = NETRAInput::createNew(*env, video_type);
	if (H264InputDevice[video_type] == NULL) {
		err(*env) << "Failed to create MJPEG input device\n";
		exit(1);
	}

  UserAuthenticationDatabase* authDB = NULL;
  ret = get_account_open(rtsp_username, rtsp_password) ;
    
printf("ret = %d...........rtsp_username = %s\n",ret ,rtsp_username) ;
printf("ret = %d...........rtsp_password = %s\n",ret,rtsp_password) ;
  if(!ret)
  {
      authDB = new UserAuthenticationDatabase;
      authDB->addUserRecord(rtsp_username, rtsp_password); // replace these with real strings  
//      authDB->addUserRecord("linkflow", "1"); // replace these with real strings  
  }
   
  // Create the RTSP server:
  RTSPServer* rtspServer = NULL;
  // Normal case: Streaming from a built-in RTSP server:
  
  rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, authDB);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  *env << "...done initializing \n";
  

  if( streamingMode == STREAMING_UNICAST )
  {
	*env << "... STREAMING_UNICAST \n";

#if 1
	ServerMediaSession * sms 
	    = ServerMediaSession::createNew(*env,H264StreamName[video_type],H264StreamName[video_type],streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
			
    if(video_type)
	{
	    sms->addSubsession(WISJPEGVideoServerMediaSubsession::createNew(sms->envir(), *H264InputDevice[video_type], MjpegVideoBitrate));
	}
	else
	{
	    sms->addSubsession(WISH264VideoServerMediaSubsession::createNew(sms->envir(), *H264InputDevice[video_type], H264VideoBitrate));
/*		
        if(resol == 2)
		    OutPacketBuffer::maxSize = 800000; // allow for some possibly large H.264 frames
        if(resol == 1)
		    OutPacketBuffer::maxSize = 400000; // allow for some possibly large H.264 frames
        if(resol == 0)
		    OutPacketBuffer::maxSize = 200000; // allow for some possibly large H.264 frames
*/
	}
	if (IsSilence == 0)
	{
		sms->addSubsession(WISPCMAudioServerMediaSubsession::createNew(sms->envir(), *H264InputDevice[video_type]));
	}

	rtspServer->addServerMediaSession(sms);
/*		
	if(!video_type)
	{

	    struct in_addr dest;
		dest.s_addr = chooseRandomIPv4SSMAddress(*env);
	    const unsigned char ttl = 255;
        const Port rtpPortVideo(videoRTPPortNum);
        const Port rtcpPortVideo(videoRTPPortNum+1);

        rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
        rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
	    setVideoRTPSinkBufferSize();

//	    GetSprop(BuffStr,video_type);
//        sprintf(BuffStr, "%s","J2QAIK2EBUViuKxUcQgKisVxWKjiECSFITk8nyfk/k/J8nm5s00IEkKQnJ5Pk/J/J+T5PNzZphcqAtD2lSAAAH0AAA6mHAAAD0JAAA9CQXvdZQAAAAE=,86,KP4Briw=,5") ;
             
        sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96) ;
//	    sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96 , BuffStr); // hwjun
//        sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96, (u_int8_t const *)"J2QAIK2EBUViuKxUcQgKisVxWKjiECSFITk8nyfk/k/J8nm5s00IEkKQnJ5Pk/J/J+T5PNzZphcqAtD2lSAAAH0AAA6mHAAAD0JAAA9CQXvdZQAAAAE=",86,(u_int8_t const *)"KP4Briw=",5) ;

	}
*/
	char *url = rtspServer->rtspURL(sms);
	if (OverHTTPEnable == 0)
	{
		*env << "Play this stream using the URL:\n\t" << url << "\n";
	}
	else
	{
		if (rtspServer->setUpTunnelingOverHTTP(OverHTTPPortNum))
		{
			*env << "Play this stream using the URL:\n\t" << url << "\n";
			*env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
		}
		else
		{
			*env << "\n(RTSP-over-HTTP tunneling is not available.)\n";
		}
	}
	delete[] url;
#else
// Add live stream
	{
	WW_H264VideoSource * videoSource = 0;

	ServerMediaSession * sms = ServerMediaSession::createNew(*env, "all", "all", "ww live test", 0);
	OutPacketBuffer::maxSize = 200000; // allow for some possibly large H.264 frames
	sms->addSubsession(WW_H264VideoServerMediaSubsession::createNew(sms->envir(), videoSource, video_type));
	rtspServer->addServerMediaSession(sms);

	char * url = rtspServer->rtspURL(sms);
	*env << "using url:" << url << "\n";
	delete[] url;
	}
#endif
  }
  else
  {
	if (streamingMode == STREAMING_MULTICAST_SSM)
	{
	  if (multicastAddress == 0)
		  multicastAddress = chooseRandomIPv4SSMAddress(*env);
	} 
	else if (multicastAddress != 0) 
	{
		streamingMode = STREAMING_MULTICAST_ASM;
	}

	struct in_addr dest; dest.s_addr = multicastAddress;
	const unsigned char ttl = 255;

	// For RTCP:
	const unsigned maxCNAMElen = 100;
	unsigned char CNAME[maxCNAMElen + 1];
	gethostname((char *) CNAME, maxCNAMElen);
	CNAME[maxCNAMElen] = '\0';      // just in case
  	
	ServerMediaSession* sms;
	
    if( video_type == VIDEO_TYPE_H264_CH1)
    {
	    sms = ServerMediaSession::createNew(*env, H264StreamName[video_type], H264StreamName[video_type], streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
	
	    sourceAudio = H264InputDevice[video_type]->audioSource();  
	    sourceVideo = H264VideoStreamFramer::createNew(*env, H264InputDevice[video_type]->videoSource());

	// Create 'groupsocks' for RTP and RTCP:
        const Port rtpPortVideo(videoRTPPortNum);
        const Port rtcpPortVideo(videoRTPPortNum+1);

        rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
        rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
        if (streamingMode == STREAMING_MULTICAST_SSM) 
        {
           rtpGroupsockVideo->multicastSendOnly();
           rtcpGroupsockVideo->multicastSendOnly();
        }
	    setVideoRTPSinkBufferSize();

        char BuffStr[200];
	    extern int GetSprop(void *pBuff, char vType);

        GetSprop(BuffStr,video_type);

	    sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96, BuffStr); // hwjun

//	    sinkVideo = H264VideoRTPSink::createNew(*env, rtpGroupsockVideo,96, 0x42, "h264");	
    }

	if( video_type == VIDEO_TYPE_H264_CH2)
	{	
		sms = ServerMediaSession::createNew(*env, H264StreamName[video_type], H264StreamName[video_type], streamDescription,streamingMode == STREAMING_MULTICAST_SSM);
		
		sourceAudio = H264InputDevice[video_type]->audioSource();
		sourceVideo = WISJPEGStreamSource::createNew(H264InputDevice[video_type]->videoSource());
//		sourceAudio = MjpegInputDevice->audioSource();
//		sourceVideo = WISJPEGStreamSource::createNew(MjpegInputDevice->videoSource());
		// Create 'groupsocks' for RTP and RTCP:
	    const Port rtpPortVideo(videoRTPPortNum);
	    const Port rtcpPortVideo(videoRTPPortNum+1);
	    rtpGroupsockVideo = new Groupsock(*env, dest, rtpPortVideo, ttl);
	    rtcpGroupsockVideo = new Groupsock(*env, dest, rtcpPortVideo, ttl);
	    if (streamingMode == STREAMING_MULTICAST_SSM) {
	      rtpGroupsockVideo->multicastSendOnly();
	      rtcpGroupsockVideo->multicastSendOnly();
	    }
		setVideoRTPSinkBufferSize();
		sinkVideo = JPEGVideoRTPSink::createNew(*env, rtpGroupsockVideo);
    }   
	/* VIDEO Channel initial */
	if(1)
	{

		// Create (and start) a 'RTCP instance' for this RTP sink:
		unsigned totalSessionBandwidthVideo = (Mpeg4VideoBitrate+500)/1000; // in kbps; for RTCP b/w share
		rtcpVideo = RTCPInstance::createNew(*env, rtcpGroupsockVideo,
					totalSessionBandwidthVideo, CNAME,
					sinkVideo, NULL /* we're a server */ ,
					streamingMode == STREAMING_MULTICAST_SSM);
	    // Note: This starts RTCP running automatically
		sms->addSubsession(PassiveServerMediaSubsession::createNew(*sinkVideo, rtcpVideo));
		
		// Start streaming:
		sinkVideo->startPlaying(*sourceVideo, NULL, NULL);
	}		
	/* AUDIO Channel initial */
	if( IsSilence == 0)
	{
		// there's a separate RTP stream for audio
		// Create 'groupsocks' for RTP and RTCP:
		const Port rtpPortAudio(audioRTPPortNum);
		const Port rtcpPortAudio(audioRTPPortNum+1);

		rtpGroupsockAudio = new Groupsock(*env, dest, rtpPortAudio, ttl);
		rtcpGroupsockAudio = new Groupsock(*env, dest, rtcpPortAudio, ttl);

		if (streamingMode == STREAMING_MULTICAST_SSM) 
		{
			rtpGroupsockAudio->multicastSendOnly();
			rtcpGroupsockAudio->multicastSendOnly();
		}
		if( audioSamplingFrequency == 16000 )
		{

			if( audioType == AUDIO_G711)
			{
				sinkAudio = SimpleRTPSink::createNew(*env, rtpGroupsockAudio, 96, audioSamplingFrequency, "audio", "PCMU", 1);
			}
			else
			{
				char const* encoderConfigStr = "1408";// (2<<3)|(8>>1) = 0x14 ; ((8<<7)&0xFF)|(1<<3)=0x08 ;
				sinkAudio = MPEG4GenericRTPSink::createNew(*env, rtpGroupsockAudio,
						       96,
						       audioSamplingFrequency,
						       "audio", "AAC-hbr",
						       encoderConfigStr, audioNumChannels);
			}
		}
		else{
			if(audioType == AUDIO_G711)
			{
				sinkAudio = SimpleRTPSink::createNew(*env, rtpGroupsockAudio, 0, audioSamplingFrequency, "audio", "PCMU", 1);
			}
			else{
				char const* encoderConfigStr =  "1588";// (2<<3)|(11>>1) = 0x15 ; ((11<<7)&0xFF)|(1<<3)=0x88 ;
				sinkAudio = MPEG4GenericRTPSink::createNew(*env, rtpGroupsockAudio,
						       96,
						       audioSamplingFrequency,
						       "audio", "AAC-hbr",
						       encoderConfigStr, audioNumChannels);

			}
		}

		// Create (and start) a 'RTCP instance' for this RTP sink:
		unsigned totalSessionBandwidthAudio = (audioOutputBitrate+500)/1000; // in kbps; for RTCP b/w share
		rtcpAudio = RTCPInstance::createNew(*env, rtcpGroupsockAudio,
					  totalSessionBandwidthAudio, CNAME,
					  sinkAudio, NULL /* we're a server */,
					  streamingMode == STREAMING_MULTICAST_SSM);
		// Note: This starts RTCP running automatically
		sms->addSubsession(PassiveServerMediaSubsession::createNew(*sinkAudio, rtcpAudio));

		// Start streaming:
		sinkAudio->startPlaying(*sourceAudio, NULL, NULL);
    }
	
	rtspServer->addServerMediaSession(sms);
	{
		struct in_addr dest; dest.s_addr = multicastAddress;
		char *url = rtspServer->rtspURL(sms);
		//char *url2 = inet_ntoa(dest);
		*env << "Mulicast Play this stream using the URL:\n\t" << url << "\n";
		//*env << "2 Mulicast addr:\n\t" << url2 << "\n";
		delete[] url;
	}
  }


  // Begin the LIVE555 event loop:
  env->taskScheduler().doEventLoop(&watchVariable); // does not return
  
  
  if( streamingMode!= STREAMING_UNICAST )
  {
	Medium::close(rtcpAudio);
	Medium::close(sinkAudio);
	Medium::close(sourceAudio);
	delete rtpGroupsockAudio;
	delete rtcpGroupsockAudio;
  
	Medium::close(rtcpVideo);
	Medium::close(sinkVideo);
	Medium::close(sourceVideo);
	delete rtpGroupsockVideo;
	delete rtcpGroupsockVideo;

  }

  
  Medium::close(rtspServer); // will also reclaim "sms" and its "ServerMediaSubsession"s
  if( MjpegInputDevice != NULL )
  {
	Medium::close(MjpegInputDevice);
  }
  

  if( H264InputDevice[video_type] != NULL )
  {
	Medium::close(H264InputDevice[video_type]);
  }
   	
  if( Mpeg4InputDevice != NULL )
  {
	Medium::close(Mpeg4InputDevice);
  }
   
  env->reclaim();
   
  delete scheduler;
   
  NetraInterfaceExit();
  NetraDrvExit();
   
  return 0; // only to prevent compiler warning

}
