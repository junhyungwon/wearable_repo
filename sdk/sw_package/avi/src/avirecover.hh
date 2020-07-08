// AVIEncoder.h: interface for the CAVIEncoder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _AVIRECOVER_HH_
#define _AVIRECOVER_HH_

#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aviencoder.hh"
#include "mmsystem.h"


// Prototype /////////////////////////////////////////////////////////
int AVIrcStart(const char* pszFileName);
char* AVIrcGetErrMsg();


#ifdef __cplusplus
}
#endif


#endif 
