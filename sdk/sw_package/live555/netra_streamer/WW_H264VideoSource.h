#ifndef _WW_H264VideoSource_H
#define _WW_H264VideoSource_H

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include "FramedSource.hh"

#include <Netra_interface.h>

#define FRAME_PER_SEC 20

class WW_H264VideoSource : public FramedSource
{
public:
	WW_H264VideoSource(UsageEnvironment & env, int vType);
	~WW_H264VideoSource(void);

public:
	virtual void doGetNextFrame();
	virtual unsigned int maxFrameSize() const;

	static void getNextFrame(void * ptr);
	void GetFrameData();

private:
	void *m_pToken;
    int   m_nVideoType;
    int   m_nStreamFlag;
    int   m_nChannel;
    int   m_nSerialBook;
    int   m_nSerialLock;
};

#endif