/** 
 *  @file   FrameQApp.h
 *
 *  @brief      Header file for FrameQ application
 *
 *
 */
/* 
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */



#ifndef  _FRAMEQAPP_H_
#define  _FRAMEQAPP_H_

/*!
 *  @brief  FrameQBufMgr instance 1 name
 */
#define  FRAMEQBUFMGR_INST1_NAME  "FrameQBufMgr1"

/*!
 *  @brief  FrameQ instance 1 name
 */
#define  FRAMEQ_INST1_NAME        "FrameQ1"

/*!
 *  @brief  FrameQBufMgr instance 2 name
 */
#define  FRAMEQBUFMGR_INST2_NAME  "FrameQBufMgr2"
/*!
 *  @brief  FrameQ instance 2 name
 */
#define  FRAMEQ_INST2_NAME        "FrameQ2"

/*!
 *  @brief  FrameQBufMgr instance 3 name
 */
#define  FRAMEQBUFMGR_INST3_NAME  "FrameQBufMgr3"
/*!
 *  @brief  FrameQ instance 3 name
 */
#define  FRAMEQ_INST3_NAME        "FrameQ3"

#define ITER_COUNT                        200u
#define APPNOTIFY_EVENT_NO                11u

typedef struct  FrameQAPP_instInfo_tag {
    char*  frmQBufMgrInstName;
    char*  frmQInstName;
}FrameQAPP_instInfo;


#endif

