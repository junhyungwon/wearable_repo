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
// A "ServerMediaSubsession" subclass for on-demand unicast streaming
// of MPEG-4 video from a WIS GO7007 capture device.
// Implementation

#include "WISH264VideoServerMediaSubsession.hh"
#include <H264VideoRTPSink.hh>
#include <H264VideoStreamFramer.hh>

WISH264VideoServerMediaSubsession* WISH264VideoServerMediaSubsession
::createNew(UsageEnvironment& env, NETRAInput& mpeg4Input, unsigned estimatedBitrate) {
  return new WISH264VideoServerMediaSubsession(env, mpeg4Input, estimatedBitrate);
}

WISH264VideoServerMediaSubsession
::WISH264VideoServerMediaSubsession(UsageEnvironment& env, NETRAInput& mpeg4Input,
				     unsigned estimatedBitrate)
  : WISServerMediaSubsession(env, mpeg4Input, estimatedBitrate),
	fAuxSDPLine(NULL), fDoneFlag(0), fDummyRTPSink(NULL) {
}

WISH264VideoServerMediaSubsession::~WISH264VideoServerMediaSubsession() {
	delete[] fAuxSDPLine;
}

static void afterPlayingDummy(void* clientData) {
  WISH264VideoServerMediaSubsession* subsess
    = (WISH264VideoServerMediaSubsession*)clientData;
  subsess->afterPlayingDummy1();
}

void WISH264VideoServerMediaSubsession::afterPlayingDummy1() {
	// Unschedule any pending 'checking' task:
	envir().taskScheduler().unscheduleDelayedTask(nextTask());
	// Signal the event loop that we're done:
	setDoneFlag();
}


static void checkForAuxSDPLine(void* clientData) {
  WISH264VideoServerMediaSubsession* subsess
    = (WISH264VideoServerMediaSubsession*)clientData;
  subsess->checkForAuxSDPLine1();
}

void WISH264VideoServerMediaSubsession::checkForAuxSDPLine1() {

	#if 0
	nextTask() = NULL;

	char const* das1;
	if (fAuxSDPLine != NULL) {
		// Signal the event loop that we're done:
		setDoneFlag();
	}
  else if (fDummyRTPSink != NULL && (das1 = fDummyRTPSink->auxSDPLine()) != NULL) {
	  fAuxSDPLine = strDup(das1);
	  fDummyRTPSink = NULL;

	  // Signal the event loop that we're done:
	  setDoneFlag();
  } else //if (!fDoneFlag) 
	{
    // try again after a brief delay:
    int uSecsToDelay = 100000; // 100 ms
    nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
			      (TaskFunc*)checkForAuxSDPLine, this);
  }
	#else
	{
		/* code */
		if (fDummyRTPSink != NULL && fDummyRTPSink->auxSDPLine()) {
			fDoneFlag = 0xff;
		}
		else
		{
			int uSecsToDelay = (1000.0/20)*1000; // 33 ms
			nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay,
																															 (TaskFunc *)checkForAuxSDPLine, this);
		}
	}
	#endif
	
}

char const *WISH264VideoServerMediaSubsession ::getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource)
{
	if (fAuxSDPLine != NULL)
		return fAuxSDPLine; // it's already been set up (for a previous client)

#if 0
	// Note: For MPEG-4 video buffer, the 'config' information isn't known
	// until we start reading the Buffer.  This means that "rtpSink"s
	// "auxSDPLine()" will be NULL initially, and we need to start reading
	// data from our buffer until this changes.
	fDummyRTPSink = rtpSink;

	// Start reading the buffer:
	fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);

	// Check whether the sink's 'auxSDPLine()' is ready:
	checkForAuxSDPLine(this);

	fDoneFlag = 0;
	envir().taskScheduler().doEventLoop(&fDoneFlag);
#else
	{
		fDummyRTPSink = rtpSink;

		// Start reading the buffer:
		fDummyRTPSink->startPlaying(*inputSource, 0, 0);

		// Check whether the sink's 'auxSDPLine()' is ready:
		checkForAuxSDPLine(this);

		fDoneFlag = 0;
		envir().taskScheduler().doEventLoop(&fDoneFlag);

		fAuxSDPLine = strdup(fDummyRTPSink->auxSDPLine());

		fDummyRTPSink->stopPlaying();
	}
#endif
	

	return fAuxSDPLine;
}

FramedSource* WISH264VideoServerMediaSubsession
::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {
  estBitrate = fEstimatedKbps;

  // Create a framer for the Video Elementary Stream:
  FramedSource *inputSource = fWISInput.videoSource();
  if (inputSource == NULL) return NULL;
  //printf("inputSource->videoType%d\n", inputSource->fileSize());

  // Create a framer for the Video Elementary Stream:
  return H264VideoStreamFramer::createNew(envir(), inputSource);
}

RTPSink* WISH264VideoServerMediaSubsession
::createNewRTPSink(Groupsock* rtpGroupsock,
		   unsigned char rtpPayloadTypeIfDynamic,
		   FramedSource* /*inputSource*/) {
  setVideoRTPSinkBufferSize();
  return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
