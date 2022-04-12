//
//  BCAWaveSink.cpp
//
//  Created by albert on 2021/10/25

#include "BCAWaveSink.hh"
#include "OutputFile.hh"

using namespace std;

////////// BKKIM DEBUG //////////
#define DBG(fmt, args...)                                                           \
  {                                                                                 \
    fprintf(stderr, "[BK] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args); \
  }
#define DBG_ENTER             \
  {                           \
    DBG("Enter >>>>>>>>>\n"); \
  }
#define DBG_EXIT             \
  {                           \
    DBG("Exit >>>>>>>>>\n"); \
  }

////////// BCAWaveSink 시작 //////////
int BCAWaveSink::msgqid; // header에서  static으로 선언할 경우, cpp의 static function에서 쓰려면 다시 선언해 줘야함..header에서 선언된 static은 무쓸모인가..

BCAWaveSink ::BCAWaveSink(UsageEnvironment &env, FILE *fid, unsigned bufferSize,
                          char const *perFrameFileNamePrefix)
    : FileSink(env, fid, bufferSize, perFrameFileNamePrefix)
{

  // Send Audio Data to app_fitt360 for playing -------------------------------------
  if ((msgqid = msgget((key_t)BCPLAY_MSG_KEY, IPC_CREAT | 0666)) == -1)
  {
    perror("msgget() Fail : ");
  }
  else
  {
    DBG("msgqid: 0x%X\n", msgqid);

    StartInternalThread();
  }
}

BCAWaveSink::~BCAWaveSink()
{
  bRunning = false;
  //WaitForInternalThreadToExit();

  if(_t1.joinable()) {
    DBG("joinable?")
    _t1.join();
  }
}

void ThreadEntry(void * pArg)
{

  BCAWaveSink* p = (BCAWaveSink*)pArg;

  bcplay_to_main_msg_t msg;
  while (p && p->bRunning)
  {
    if (p->q.size()>1 && p->msgqid > 0)
    {
      msg.mtext = 1;
      msg.cmd = BCPLAY_CMD_AUD_DATA;

      BCA_QENT e = p->q.front();
      if(e.len == BCA_MAX_SIZE) {
        msg.len = e.len;
        memcpy(msg.buf, e.buf, e.len);
        p->q.pop_front();

        int ret = msgsnd(p->msgqid, &msg, sizeof(bcplay_to_main_msg_t) - sizeof(long), IPC_NOWAIT); /* send msg1 */
        if (ret == -1)
        {
          perror("msgsnd:");
          DBG("Error send audio, ret = %d\n", ret);
        }
        
      } // endif
    }
    usleep(50 * 1000); // 70 ms?
  }

#if 0
  if(p && !p->q.empty()){
    // double free!!??
    //p->q.clear();
    DBG("List Clear()\n");
  }
#endif

}

BCAWaveSink *
BCAWaveSink::createNew(UsageEnvironment &env, char const *fileName,
                       unsigned bufferSize, Boolean oneFilePerFrame)
{
  DBG_ENTER;
  do
  {
    FILE *fid;
    char const *perFrameFileNamePrefix;
    if (oneFilePerFrame)
    {
      // Create the fid for each frame
      fid = NULL;
      perFrameFileNamePrefix = fileName;
    }
    else
    {
      // Normal case: create the fid once
      fid = OpenOutputFile(env, fileName);
      if (fid == NULL)
        break;
      perFrameFileNamePrefix = NULL;

    }

    return new BCAWaveSink(env, fid, bufferSize, perFrameFileNamePrefix);
  } while (0);

  return NULL;
}

Boolean BCAWaveSink::continuePlaying()
{
  if (fSource == NULL)
    return False;

  fSource->getNextFrame(fBuffer, fBufferSize,
                        afterGettingFrame, this,
                        onSourceClosure, this);

  return True;
}

void BCAWaveSink::afterGettingFrame(void *clientData, unsigned frameSize,
                                    unsigned numTruncatedBytes,
                                    struct timeval presentationTime,
                                    unsigned durationInMicroseconds)
{
  BCAWaveSink *sink = (BCAWaveSink *)clientData;
  sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime);
}

void BCAWaveSink::addData(unsigned char const *data, unsigned dataSize,
                          struct timeval presentationTime)
{
  if (fPerFrameFileNameBuffer != NULL && fOutFid == NULL)
  {
    // Special case: Open a new file on-the-fly for this frame
    if (presentationTime.tv_usec == fPrevPresentationTime.tv_usec &&
        presentationTime.tv_sec == fPrevPresentationTime.tv_sec)
    {
      // The presentation time is unchanged from the previous frame, so we add a 'counter'
      // suffix to the file name, to distinguish them:
      sprintf(fPerFrameFileNameBuffer, "%s-%lu.%06lu-%u", fPerFrameFileNamePrefix,
              presentationTime.tv_sec, presentationTime.tv_usec, ++fSamePresentationTimeCounter);
    }
    else
    {
      sprintf(fPerFrameFileNameBuffer, "%s-%lu.%06lu", fPerFrameFileNamePrefix,
              presentationTime.tv_sec, presentationTime.tv_usec);
      fPrevPresentationTime = presentationTime; // for next time
      fSamePresentationTimeCounter = 0;         // for next time
    }
    fOutFid = OpenOutputFile(envir(), fPerFrameFileNameBuffer);
  }

  // Write to our file:
#ifdef TEST_LOSS
  static unsigned const framesPerPacket = 10;
  static unsigned const frameCount = 0;
  static Boolean const packetIsLost;
  if ((frameCount++) % framesPerPacket == 0)
  {
    packetIsLost = (our_random() % 10 == 0); // simulate 10% packet loss #####
  }

  if (!packetIsLost)
#endif

  {
    // Back Channel 사용할때, Device player쪽으로 데이터 던짐...Player에서 저장/재생 옵션에 따라 사용...여기선 그냥 던지고....
    // 상속해서 사용해야겠지?...OnDemand, RTSPServer, MediaSession등 수정 많은데 다 상속????
    // FileSink 상속은 훗날 오디오 녹화파일을 위해 그냥 하자..
    time_t now;
    time(&now);
//    DBG("BCA, dataSize:%d, secs:%ld, q.size:%d\n", dataSize, now - presentationTime.tv_sec, q.size());
  }

  if (fOutFid != NULL && data != NULL)
  {
    // fwrite(data, 1, dataSize, fOutFid);

#if 1 // audio save to buffer
    BCA_QENT ent;

    int remainSize = dataSize;
    int offset = 0;

    // 마지막꺼에 채움
    BCA_QENT eb = q.back();
    if(q.begin() != q.end() && eb.len != BCA_MAX_SIZE) {

      list<BCA_QENT>::iterator iter = q.end();
      iter--; // for insert 

      // 넘어온 데이타 크기가 큐에 넣어야할 사이즈(BCA_MAX_SIZE)보다 작을때
      if(BCA_MAX_SIZE - eb.len < dataSize) {
        memcpy(eb.buf + eb.len, data, BCA_MAX_SIZE - eb.len);

        remainSize -= (BCA_MAX_SIZE - eb.len);
        offset = BCA_MAX_SIZE - eb.len;
        eb.len = BCA_MAX_SIZE;

        q.insert(iter, eb);
        q.pop_back(); // remove end
        
      } else {
        memcpy(eb.buf + eb.len, data, dataSize);
        eb.len += dataSize;

        q.insert(iter, eb);
        q.pop_back(); // remove end

        return;
      }
    }

    while (remainSize > 0)
    {
      if (remainSize >= BCA_MAX_SIZE)
      {
        ent.len = BCA_MAX_SIZE;
        memcpy(ent.buf, data+offset, BCA_MAX_SIZE);
        q.push_back(ent);

        offset += BCA_MAX_SIZE;
        remainSize -= BCA_MAX_SIZE;
      }
      else
      {
        BCA_QENT e;
        e.len = remainSize;
        memcpy(e.buf, data+offset, remainSize);
        q.push_back(e);

        break;
      }
    }
#endif
  }
}

void BCAWaveSink::afterGettingFrame(unsigned frameSize,
                                    unsigned numTruncatedBytes,
                                    struct timeval presentationTime)
{
  if (numTruncatedBytes > 0)
  {
    envir() << "FileSink::afterGettingFrame(): The input frame data was too large for our buffer size ("
            << fBufferSize << ").  "
            << numTruncatedBytes << " bytes of trailing data was dropped!  Correct this by increasing the \"bufferSize\" parameter in the \"createNew()\" call to at least "
            << fBufferSize + numTruncatedBytes << "\n";
  }
  addData(fBuffer, frameSize, presentationTime);

  if (fOutFid == NULL || fflush(fOutFid) == EOF)
  {
    // The output file has closed.  Handle this the same way as if the input source had closed:
    if (fSource != NULL)
      fSource->stopGettingFrames();
    onSourceClosure();
    return;
  }

  if (fPerFrameFileNameBuffer != NULL)
  {
    if (fOutFid != NULL)
    {
      fclose(fOutFid);
      fOutFid = NULL;
    }
  }

  // Then try getting the next frame:
  continuePlaying();
}

bool BCAWaveSink::StartInternalThread()
{
  bRunning = true;
  _t1 = thread(ThreadEntry, (void*)this);
  //_t1 = thread(InternalThreadEntry);
  _t1.detach(); // terminate called without an active exception

  return true;
}
