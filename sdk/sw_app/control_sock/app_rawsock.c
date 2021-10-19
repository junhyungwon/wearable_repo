/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_rawsock.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *    - 2013.09.23 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
   Defines referenced header files
-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/resource.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <time.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <pthread.h>
//#include <global.h>

#include <app_message.h>
#include <app_msghandler.h>
#include <app_rawsock.h>
//#include <udbasket.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

extern SYSTEM_INFO SystemInfo ;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

char ReadBuffer[BUFFSIZE] ;
char m_TempBuf[BUFFSIZE] ;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/

int NewDescriptor(int m_RootOneSock)
{
    int m_NewDesc, i = 0, m_Returnvalue = 0 ;
    socklen_t m_SizeSocketPeer = 0;
    struct sockaddr_in m_SocketPeer;

    m_SizeSocketPeer = sizeof(m_SocketPeer);

    if ((m_NewDesc  = accept(m_RootOneSock, (struct sockaddr *)&m_SocketPeer, &m_SizeSocketPeer)) == INVALID_SOCKET)
    {
#ifdef NETWORK_DEBUG
        DEBUG_PRI("\nNew Client Connecting Fail\n");
#endif
        return ERRNO;
    }

    for(i = 1; i < MAXUSER; i++)
    {
        if(SystemInfo.Channel[i] == 0) 
	    break;
    }

    if(i >= (MAXUSER-1))
    {
#ifdef NETWORK_DEBUG
        DEBUG_PRI("i >= MAXUSER -1\n") ;
#endif
        close(m_NewDesc);
        m_Returnvalue = FALSE ;
    } 

    SystemInfo.Channel[i] = m_NewDesc;
    m_Returnvalue = i ;

    return m_Returnvalue ;

}

int MainSocketListen()
{
    int reuse, m_ListenSock = 0 ;
    int keepalive = 1, keepcnt = 1, keepidle = 1, keepintvl = 1 ;

    int recvBufferSize = 0 ;
    int sendBufferSize = 0 ;
    struct linger stLinger ;
    struct timeval tv ;

    struct sockaddr_in m_SocketAddress;

    tv.tv_sec = 5;
    tv.tv_usec = 0 ;

    stLinger.l_onoff = 1 ;
    stLinger.l_linger = 0 ;

    if ((m_ListenSock = socket(PF_INET, SOCK_STREAM,0)) < 0)
    {
#ifdef NETWORK_DEBUG
        DEBUG_PRI("\ndon't create socket.\n");
#endif
        exit(1);
    }

    reuse = TRUE ;

    recvBufferSize = BUFFSIZE ;
    sendBufferSize = MAXBUFF*2 ;
    setsockopt (m_ListenSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) ;
    setsockopt (m_ListenSock, SOL_SOCKET, SO_RCVBUF, &recvBufferSize, sizeof(recvBufferSize)) ;
    setsockopt (m_ListenSock, SOL_SOCKET, SO_SNDBUF, &sendBufferSize, sizeof(sendBufferSize)) ;
    setsockopt (m_ListenSock, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof (keepalive)) ;
    setsockopt (m_ListenSock, SOL_TCP,    TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) ;
    setsockopt (m_ListenSock, SOL_TCP,    TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) ;
    setsockopt (m_ListenSock, SOL_TCP,    TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) ;

    m_SocketAddress.sin_family = AF_INET;
    m_SocketAddress.sin_port = htons(SERVER_PORT);
    m_SocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(m_SocketAddress.sin_zero), 8);

    if (bind(m_ListenSock,(struct sockaddr *) &m_SocketAddress, sizeof(m_SocketAddress)) < 0)
    {
#ifdef NETWORK_DEBUG
        DEBUG_PRI("\nBind Error! incorrect IP.\n\n");
#endif
        close(m_ListenSock);
        exit(1);
    }

    listen(m_ListenSock, 5) ;

    return m_ListenSock ;

}

void ProcessSocket(int m_ListenSock )
{
    fd_set m_read_set,m_write_set,m_spc_set,m_NULLSET;
    struct timeval m_NULLTIME, tv_read ;
    int i, m_NewDes;

    int m_RetrunFailed;

    gettimeofday (&tv_read, NULL) ;
    m_NULLTIME.tv_sec = 0;
    m_NULLTIME.tv_usec = 7500L;

    FD_ZERO(&m_NULLSET);

    m_NewDes = 0;

    FD_ZERO(&m_read_set);
    FD_ZERO(&m_write_set);
    FD_ZERO(&m_spc_set);
    FD_SET(m_ListenSock, &m_read_set);

    for (i=1;i < MAXUSER;i++)
    {
        if(SystemInfo.Channel[i] > 0)
        {
            FD_SET(SystemInfo.Channel[i],&m_read_set);
            FD_SET(SystemInfo.Channel[i],&m_write_set);
            FD_SET(SystemInfo.Channel[i],&m_spc_set);
        }
    }

    if (select(FD_SETSIZE, &m_read_set, &m_write_set, &m_spc_set, &m_NULLTIME) < 0)
    {
        return ;
    }

    if (FD_ISSET(m_ListenSock, &m_read_set))
    {
        m_NewDes = NewDescriptor(m_ListenSock);
#ifdef NETWORK_DEBUG
        DEBUG_PRI("Logon NewDescriptor = %d\n", m_NewDes) ;
#endif
    }

    for (i = 1; i < MAXUSER; i++)
    {
        if((SystemInfo.Channel[i] > 0) && (m_NewDes != i))
        {
            if (FD_ISSET(SystemInfo.Channel[i],&m_spc_set))
            {
                FD_CLR(SystemInfo.Channel[i],&m_read_set);
                FD_CLR(SystemInfo.Channel[i],&m_write_set);
                CloseNowChannel(i);
            }
        }
    }

    for (i = 1;i < MAXUSER;i++)
    {
        if((SystemInfo.Channel[i]>0) && (m_NewDes != i))
        {
            if (FD_ISSET(SystemInfo.Channel[i],&m_read_set))
            {
                if ((m_RetrunFailed = ReadSocketData(i)) < 0)
                {
                    CloseNowChannel(i);
                }
            }
        }
    }
}

void Sock_Init(void)
{
    int i = 0 ;

    for(i = 1; i < MAXUSER ; i++)
    {
        if(SystemInfo.Channel[i] != 0)
        {
            close(SystemInfo.Channel[i]) ;
            SystemInfo.Channel[i] = 0 ;
            SystemInfo.Qposition[i] = 0 ;
			SystemInfo.gps_req[i] = 0 ;
			SystemInfo.event_req[i] = 0 ;
            memset(SystemInfo.QBUFF[i],0,BUFFSIZE) ;
        }
    }
}

void CloseAllChannel(void)
{
    int i = 0 ;

    for(i = 1; i < MAXUSER ; i++)
    {
        if(SystemInfo.Channel[i] != 0)
        {
            close(SystemInfo.Channel[i]) ;
            SystemInfo.Channel[i] = 0 ;
            SystemInfo.Qposition[i] = 0 ;
			SystemInfo.gps_req[i] = 0 ;
			SystemInfo.event_req[i] = 0 ;
            memset(SystemInfo.QBUFF[i],0,BUFFSIZE) ;
        }
    }
}

void CloseNowChannel(int m_NowChno )
{
    if(SystemInfo.Channel[m_NowChno] != 0)
    {
        close (SystemInfo.Channel[m_NowChno]);
        SystemInfo.Channel[m_NowChno] = 0 ;
        SystemInfo.Qposition[m_NowChno] = 0 ;
		SystemInfo.gps_req[m_NowChno] = 0 ;
		SystemInfo.event_req[m_NowChno] = 0 ;
        memset(SystemInfo.QBUFF[m_NowChno],0, BUFFSIZE) ;
    }
}

int ReadSocketData(int m_NowReadChannel) 
{
    int m_ReadBuffSize = 0, m_Returnvalue = 0 ;

    m_ReadBuffSize = recv(SystemInfo.Channel[m_NowReadChannel], ReadBuffer, BUFFSIZE,0);
#ifdef NETWORK_DEBUG
    DEBUG_PRI("logon socket recv......size = %d\n",m_ReadBuffSize) ;
#endif

    if(m_ReadBuffSize < FALSE)
    {
	switch(errno)
        {
            case EWOULDBLOCK :
#ifdef NETWORK_DEBUG
                 DEBUG_PRI("ReadSocketData EWOULDBLOCK..\n") ;
#endif
                 m_Returnvalue = FALSE ; // FALSE == 0
                 break ;

            case EPIPE :
#ifdef NETWORK_DEBUG
                 DEBUG_PRI("ReadSocketData EPIPE..\n") ;
#endif
                 m_Returnvalue = ERRNO ; // ERRNO == -1
                 break ;

            case ENOTSOCK :
#ifdef NETWORK_DEBUG
                 DEBUG_PRI("ReadSocketData ENOTSOCK\n") ;
#endif
                 m_Returnvalue = ERRNO ;
                 break ;

            case ECONNRESET :
#ifdef NETWORK_DEBUG
                 DEBUG_PRI("ReadSocketData ECONNRESET\n") ;
#endif
                 m_Returnvalue = ERRNO ;
                 break ;

            default :
#ifdef NETWORK_DEBUG
                 DEBUG_PRI("ReadSocketData errno = %d\n",errno) ;
#endif
                 m_Returnvalue = ERRNO ;
                 break ;
        }
        return m_Returnvalue ;

    }
    else
    {
	if(m_ReadBuffSize == FALSE)
        {
            m_Returnvalue = ERRNO ;
        }
        if(m_ReadBuffSize > FALSE)
        {
            m_Returnvalue = ReadSocketQue(m_NowReadChannel, m_ReadBuffSize) ;
            if(m_Returnvalue < FALSE)
            {
                m_Returnvalue = ERRNO ;
            }
            else
            {
                memset(ReadBuffer, 0, BUFFSIZE) ;
                m_Returnvalue = FALSE ;
            }
        }
    }
    return m_Returnvalue ;
}

int ReadSocketQue(int m_NowChannel, int m_BuffSize)
{
    int m_Returnvalue = 0 ;

    if(m_BuffSize < FALSE)
    {
        m_Returnvalue = ERRNO ;
    }
    if(m_BuffSize == FALSE)
    {
        m_Returnvalue = FALSE ;
    }
    else
    {
	#ifdef NETWORK_DEBUG
        	DEBUG_PRI("ReadSocketQue m_BuffSize = %d\n",m_BuffSize) ;
	#endif

	if(SystemInfo.Qposition[m_NowChannel] >= BUFFSIZE)
        {
            SystemInfo.Qposition[m_NowChannel] = 0;
            memset(SystemInfo.QBUFF[m_NowChannel], 0, BUFFSIZE);
            m_Returnvalue = FALSE ;
        }
        if(SystemInfo.Qposition[m_NowChannel] + m_BuffSize >= BUFFSIZE)
        {
            SystemInfo.Qposition[m_NowChannel] = 0;
            memset(SystemInfo.QBUFF[m_NowChannel], 0, BUFFSIZE );
            m_Returnvalue = FALSE ;
        }
        else
        {
            memcpy(SystemInfo.QBUFF[m_NowChannel] + SystemInfo.Qposition[m_NowChannel], ReadBuffer,m_BuffSize);
            SystemInfo.Qposition[m_NowChannel] += m_BuffSize;

            if((m_BuffSize < 2) || (SystemInfo.Qposition[m_NowChannel] < 2)) return 0;

            m_Returnvalue = GetQue(m_NowChannel);
        }
    }
    return m_Returnvalue ;
}


int GetQue(int m_NowChno)
{
    int m_PhaseLen = 0, m_PhasePos = 0, phasecmd = 0, m_Returnvalue = 0;

    if (SystemInfo.Channel[m_NowChno] <= 0)
    {
        m_Returnvalue = FALSE ;
       
    }
    m_PhaseLen = ntohs(((SystemInfo.QBUFF[m_NowChno][m_PhasePos+5] << 8) & 0xff00)
               | ((SystemInfo.QBUFF[m_NowChno][m_PhasePos + 4]) & 0xff));

    phasecmd = ntohs(((SystemInfo.QBUFF[m_NowChno][m_PhasePos+3] << 8) & 0xff00)
               | ((SystemInfo.QBUFF[m_NowChno][m_PhasePos + 2]) & 0xff));

    if((m_PhaseLen <= 0) || (m_PhaseLen > BUFFSIZE -10))
    {
        SystemInfo.Qposition[m_NowChno] = 0;
        memset(SystemInfo.QBUFF[m_NowChno], 0, BUFFSIZE );
#ifdef NETWORK_DEBUG
        DEBUG_PRI("m_PhaseLen == 0 \n") ;
#endif
        m_Returnvalue = FALSE ;
    }

    if(m_PhaseLen > SystemInfo.Qposition[m_NowChno])
    {
        m_Returnvalue = FALSE ;
    }

    set_message (gMessageManager, m_NowChno, SystemInfo.QBUFF[m_NowChno]+2, m_PhaseLen - 2) ;

    memset(SystemInfo.QBUFF[m_NowChno],0,m_PhaseLen );
    m_PhasePos = m_PhaseLen ;

    SystemInfo.Qposition[m_NowChno] = SystemInfo.Qposition[m_NowChno] - m_PhasePos;

    if(m_PhasePos + SystemInfo.Qposition[m_NowChno] >= BUFFSIZE )
    {
        SystemInfo.Qposition[m_NowChno] = 0;
        memset(SystemInfo.QBUFF[m_NowChno], 0, BUFFSIZE );
        m_Returnvalue = FALSE ;
    }

    if(SystemInfo.Qposition[m_NowChno] <= FALSE)
    {
        SystemInfo.Qposition[m_NowChno] = 0;
        memset(SystemInfo.QBUFF[m_NowChno], 0, BUFFSIZE );
        m_Returnvalue = FALSE ;
    }
    else
    {
        memcpy(m_TempBuf, SystemInfo.QBUFF[m_NowChno] + m_PhasePos, SystemInfo.Qposition[m_NowChno]);
        memcpy(SystemInfo.QBUFF[m_NowChno],m_TempBuf, SystemInfo.Qposition[m_NowChno]);
    }

    m_PhasePos = 0;

    return m_Returnvalue ;
}


