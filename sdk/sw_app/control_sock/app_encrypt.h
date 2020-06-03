/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /*
 * @file        app_encrypt.h
 * @brief
 * @author      hwjun
 * @section     MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_ENCRYPT_H_
#define _APP_ENCRYPT_H_

/*----------------------------------------------------------------------------
 Defines referenced     header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a     function prototype
-----------------------------------------------------------------------------*/


extern int createKeys(unsigned long int *pPrivateKey, unsigned long int *pPublicKey, unsigned long int *pModNumber);
extern unsigned long int ss_encrypt(unsigned long int publicKey,unsigned long int modNumber,
                                    unsigned char *srcBufPtr,   unsigned long int srcSize,
                                    unsigned char *outBufPtr,   unsigned long int outBufSize);
extern unsigned long int ss_decrypt (unsigned long int privateKey, unsigned long int modNumber,
                                     unsigned char *srcBufPtr,  unsigned long int srcSize,
                                     unsigned char *outBufPtr,  unsigned long int outBufSize);
unsigned short CheckTimer(void) ;

#endif // _APP_ENCRYPT_H
