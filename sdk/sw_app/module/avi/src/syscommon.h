// syscommon.h: interface for the common define.
//
//////////////////////////////////////////////////////////////////////
#ifndef _SYSCOMMON_H_
#define _SYSCOMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


///////////////////////////////////////////////////////////////////////////////
#define AVI_DEBUG

#ifdef AVI_DEBUG
#define DBG0_LOG(msg, args...) printf("[libavi] - " msg, ##args)
#define DBG1_LOG(msg, args...) printf("[libavi] - %s:" msg, __FUNCTION__, ##args)
#define DBG2_LOG(msg, args...) printf("[libavi] - %s(%d):" msg, __FUNCTION__, __LINE__, ##args)
#define DBG3_LOG(msg, args...) printf("[libavi] - %s(%d):\t%s:" msg, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define DBG0_LOG(msg, args...) ((void)0)
#define DBG1_LOG(msg, args...) ((void)0)
#define DBG2_LOG(msg, args...) ((void)0)
#define DBG3_LOG(msg, args...) ((void)0)
#endif
///////////////////////////////////////////////////////////////////////////////

#define PROC_SUCCESS	(0)
#define PROC_ERROR		(-1)

#define TRUE	(1)
#define FALSE	(0)

typedef int				BOOL;
typedef long			LONG;
typedef unsigned long 	DWORD;
typedef unsigned short 	WORD;
typedef unsigned char 	BYTE;




#endif // !defined(AFX_BLOCKBUFFER_H__0CF63682_769F_4CF8_B3F4_A0D61958E7A0__INCLUDED_)
