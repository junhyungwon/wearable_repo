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


#define FRAMEQ_NAME	"FrameQ"

#define FRAMEQBUFMGR_NAME	"FrameQBufMgr"

/* shared region created out of APPLICATION section memory */
#define SHAREDREG_0_ENTRYID               0
#define SHAREDREG_1_ENTRYID               1

#define APP_FRAMEQ_FORCE_NOTIFCATION_MSG   0x1234
#define ITER_COUNT                        200u
#define APPNOTIFY_EVENT_NO                11u

/* Configure FrameQBufMgr with two pools containing frames*/
#define APP_NUM_FREE_FRMPOOL            1u

/* Frame buffers in a frame */
#define APP_NUM_FRAMEBUFS_IN_FRAME      1

/* Configure FrameQ with two queues for a reader */
#define APP_NUM_FRMQREADERS             1u
/* Configure FrameQ with two queues for a reader  same as APP_NUM_FREE_FRMPOOL*/
#define APP_NUM_FRMQUEUES_FOR_READER    APP_NUM_FREE_FRMPOOL

#endif
