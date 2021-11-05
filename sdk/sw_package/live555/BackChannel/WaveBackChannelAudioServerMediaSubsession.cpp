//
//  WaveBackChannelAudioServerMediaSubsession.cpp
#include "WaveBackChannelAudioServerMediaSubsession.hh"
//
//  Created by BKKIM on 2021/10/25.
#include "BCAWaveSink.hh"
#include "WaveBackChannelAudioServerMediaSubsession.hh"

WaveBackChannelAudioServerMediaSubsession*
WaveBackChannelAudioServerMediaSubsession::createNew(UsageEnvironment& env,
                                                     char const* fileName,
                                                     Boolean reuseFirstSource) {
    return new WaveBackChannelAudioServerMediaSubsession(env, fileName, reuseFirstSource);
}

WaveBackChannelAudioServerMediaSubsession::WaveBackChannelAudioServerMediaSubsession(UsageEnvironment& env,
                                                char const* fileName, Boolean reuseFirstSource)
: FileServerMediaSubsession(env, fileName, reuseFirstSource){
    
}


WaveBackChannelAudioServerMediaSubsession
::~WaveBackChannelAudioServerMediaSubsession() {
}


FileSink* WaveBackChannelAudioServerMediaSubsession::createNewStreamDestination(unsigned clientSessionId,
                                                                          unsigned& estBitrate)
{
    estBitrate = 8; // kbps, estimate
    return BCAWaveSink::createNew(envir(), fFileName);
}


// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSource* WaveBackChannelAudioServerMediaSubsession::createNewRTPSource(Groupsock* rtpGroupsock,
                                                                             unsigned char rtpPayloadTypeIfDynamic,
                                                                             FileSink* outputSink)
{
	rtpPayloadTypeIfDynamic = 0;
    unsigned fSamplingFrequency = 8000;

    RTPSource* fReadSource = WaveRTPSource::createNew(envir(), rtpGroupsock,
    		rtpPayloadTypeIfDynamic, fSamplingFrequency);

    return fReadSource;
}

// implment virtual function defined in OnDemandServerMediaSubsession.hh
// new virtual functions, defined by all subclasses
FramedSource* WaveBackChannelAudioServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
                                            unsigned& estBitrate)
{ return NULL;}

// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSink* WaveBackChannelAudioServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
                                  unsigned char rtpPayloadTypeIfDynamic,
                                  FramedSource* inputSource)
{ return NULL;}










WaveRTPSource* WaveRTPSource::createNew(UsageEnvironment& env,
			      Groupsock* RTPgs,
			      unsigned char rtpPayloadFormat,
			      unsigned rtpTimestampFrequency) {
  return new WaveRTPSource(env, RTPgs, rtpPayloadFormat,
				rtpTimestampFrequency);
}

WaveRTPSource::WaveRTPSource(UsageEnvironment& env,
				       Groupsock* rtpGS,
				       unsigned char rtpPayloadFormat,
				       unsigned rtpTimestampFrequency)
  : MultiFramedRTPSource(env, rtpGS,
			 rtpPayloadFormat, rtpTimestampFrequency) {
}

WaveRTPSource::~WaveRTPSource() {
}

char const* WaveRTPSource::MIMEtype() const {
  return "audio";
}



