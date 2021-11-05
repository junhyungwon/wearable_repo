#pragma once

#include "FileServerMediaSubsession.hh"
#include "MultiFramedRTPSource.hh"

class WaveBackChannelAudioServerMediaSubsession: public FileServerMediaSubsession{

public:
    static WaveBackChannelAudioServerMediaSubsession*
    createNew(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource);
    
protected:
    WaveBackChannelAudioServerMediaSubsession(UsageEnvironment& env,
                                       char const* fileName, Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~WaveBackChannelAudioServerMediaSubsession();
    
protected: // redefined virtual functions

    // Below functions should be redefined,
    // But they won't use in backchannel
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate);
    //"estBitrate" is the stream's estimated bitrate, in kbps
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                    unsigned char rtpPayloadTypeIfDynamic,
                    FramedSource* inputSource);
    
    virtual FileSink* createNewStreamDestination(unsigned clientSessionId,
                                    unsigned& estBitrate);
    // "estBitrate" is the stream's estimated bitrate, in kbps
    virtual RTPSource* createNewRTPSource(Groupsock* rtpGroupsock,
                                          unsigned char rtpPayloadTypeIfDynamic,
                                          FileSink* outputSink);
};

class WaveRTPSource: public MultiFramedRTPSource {
public:
  static WaveRTPSource*
  createNew(UsageEnvironment& env, Groupsock* RTPgs,
	    unsigned char rtpPayloadFormat,
	    unsigned rtpTimestampFrequency);

protected:
  virtual ~WaveRTPSource();

private:
  WaveRTPSource(UsageEnvironment& env, Groupsock* RTPgs,
		     unsigned char rtpPayloadFormat,
		     unsigned rtpTimestampFrequency);
      // called only by createNew()

private:
  virtual char const* MIMEtype() const;
};

