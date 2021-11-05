//
//  BCAWaveSink.h
//
//  Created by BKKIM on 2021/10/26
//

#ifndef __BCA_Wave_Sink__
#define __BCA_Wave_Sink__

#ifndef _FILE_SINK_HH
#include "FileSink.hh"
#endif

#include <list>
#include <thread>

////////////////////////////////////////////////////////////////////////////////
// FileSink를 뺄까 싶었지만, 혹시 나중에 Audio 녹화가 필요하지 않을까 싶어 그대로 둠
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// app_fitt.out 와 Wave 통신하기 위함..
#define BCPLAY_MSG_KEY (0x185EA)
#define BCPLAY_CMD_AUD_DATA (0x627) /* delivery audio data */
#define BCA_MAX_SIZE 640 // 아 사이즈는 NEXX의 app_snd.c에서 사용하는 것과 동일하게 했다..맞는건가...
#include <sys/msg.h>
#include <time.h>
typedef struct
{
  long mtext;
  int cmd;
  int len;
  unsigned char buf[BCA_MAX_SIZE];
} bcplay_to_main_msg_t;

using namespace std;
typedef struct _tag_bca_q_entry
{
  short len;
  unsigned char buf[BCA_MAX_SIZE];
} BCA_QENT;


class BCAWaveSink: public FileSink {
public:
    static BCAWaveSink* createNew(UsageEnvironment& env, char const* fileName,
                                        unsigned bufferSize = 10000,
                                        Boolean oneFilePerFrame = False);
    // (See "FileSink.hh" for a description of these parameters.)

    /** Returns true if the thread was successfully started, false if there was an error starting the thread */
    bool StartInternalThread();
    void WaitForInternalThreadToExit();

    bool bRunning;
    list<BCA_QENT> q;
    static int msgqid;

    thread _t1;

protected:
    BCAWaveSink(UsageEnvironment& env, FILE* fid, unsigned bufferSize,
                      char const* perFrameFileNamePrefix);
    // called only by createNew()
    virtual ~BCAWaveSink();

    
protected: // redefined virtual functions:

    virtual void afterGettingFrame(unsigned frameSize,
                                   unsigned numTruncatedBytes,
                                   struct timeval presentationTime);
    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void addData(const unsigned char*, unsigned int, timeval);

private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();
    void InternalThreadEntry();

protected:
    Boolean fHaveWrittenHeader;
};

#endif /* defined(__BCA_Wave_Sink__) */