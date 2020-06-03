/** ==================================================================
 *  @file   ispLink_drv.c                                                  
 *                                                                    
 *  @path    ipnc_mcfw/mcfw/src_bios6/links_m3vpss/isp/                                                 
 *                                                                    
 *  @desc   This  File contains.                                      
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012                    
 *                                                                    
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

#include "ispLink_priv.h"

/* isp driver callback functions */

/* ===================================================================
 *  @func     IspLink_drvScFvidCb                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
Int32 IspLink_drvScFvidCb(FVID2_Handle handle, Ptr appData, Ptr reserved)
{
    IspLink_Obj *pObj = (IspLink_Obj *)appData;

    Semaphore_post(pObj->drvObj.complete);

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     IspLink_drvFvidErrCb                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
Int32 IspLink_drvFvidErrCb(FVID2_Handle handle, Ptr appData, Ptr errList,
                           Ptr reserved)
{
    return FVID2_SOK;
}

/* Isp FVID2 driver create */

/* ===================================================================
 *  @func     IspLink_drvCreateFvidObj                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
Int32 IspLink_drvCreateFvidObj(IspLink_Obj *pObj)
{
    Semaphore_Params semParams;
    IspLink_drvObj *pDrvObj;
    FVID2_CbParams cbParams;

    pDrvObj = &pObj->drvObj;
    pDrvObj->drvInstId = 0;

    /* create a semaphore for sync */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pDrvObj->complete = Semaphore_create(0u, &semParams, NULL);

    UTILS_assert(pDrvObj->complete != NULL);

	/* Driver create params */ 
	pDrvObj->createParams.clkDivM   = pObj->createArgs.clkDivM; 
	pDrvObj->createParams.clkDivN   = pObj->createArgs.clkDivN; 

    memset(&cbParams, 0, sizeof(cbParams));
    cbParams.cbFxn    = IspLink_drvScFvidCb;
    cbParams.errCbFxn = IspLink_drvFvidErrCb;
    cbParams.errList  = &pDrvObj->errCbProcessList;
    cbParams.appData  = pObj;

    pDrvObj->fvidHandle = FVID2_create(FVID2_ISS_M2M_ISP_DRV,
                                       pDrvObj->drvInstId,
                                       &pDrvObj->createParams,
                                       &pDrvObj->createStatus, 
									   &cbParams);

    UTILS_assert(pDrvObj->fvidHandle != NULL);

    pDrvObj->processList.numInLists      = 1;
    pDrvObj->processList.numOutLists     = 1;
    pDrvObj->processList.inFrameList[0]  = &pDrvObj->inFrameList;
    pDrvObj->processList.outFrameList[0] = &pDrvObj->outFrameList;

#ifdef SYSTEM_DEBUG_ISP
    Vps_printf(" %d: ISP   : FVID2 Create Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}

/* Isp link process frames */

/* ===================================================================
 *  @func     IspLink_drvProcessFrames                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
Int32 IspLink_drvProcessFrames(IspLink_Obj * pObj)
{
    UInt32 frameId,queId;
    Int32 status;
    System_LinkInQueParams *pInQueParams;
    FVID2_Frame *pEmptyFrame;
    FVID2_FrameList frameList;
    IspLink_drvObj *pDrvObj;
	Iss_PpRszConfig rszCfg;
	
#ifdef SYSTEM_DEBUG_ISP_RT					
	UInt32 start,end;
#endif	

    pDrvObj = &pObj->drvObj;
    pInQueParams = &pObj->createArgs.inQueParams;

    System_getLinksFullFrames(pInQueParams->prevLinkId,
                              pInQueParams->prevLinkQueId, &frameList);

    if (frameList.numFrames)
    {
#ifdef SYSTEM_DEBUG_ISP_RT
        Vps_printf(" %d: ISP   : Received and returning %d frames !!!\n",
                   Clock_getTicks(), frameList.numFrames);
#endif

        for (frameId = 0; frameId < frameList.numFrames; frameId++)
        {
            pDrvObj->inFrameList.numFrames  = 0;
            pDrvObj->outFrameList.numFrames = 0;
			
            pDrvObj->inFrameList.frames[pDrvObj->inFrameList.numFrames] = frameList.frames[frameId];
            pDrvObj->inFrameList.numFrames = 1;			
						
			for(queId = 0;queId < pObj->createArgs.numOutQueue;queId += 2)
			{
				if (((pObj->inFrameCnt < 30) && (((pObj->frameSkipMask[queId] >> pObj->inFrameCnt) & 0x1) == 0u)) ||
					((pObj->inFrameCnt >= 30) && (((pObj->frameSkipMaskHigh[queId] >> (pObj->inFrameCnt - 30)) & 0x1) == 0u)))
				{
					/* Get frame buffer from empty queue */
					status = Utils_bufGetEmptyFrame(&pObj->outFrameQue[queId],
													&pEmptyFrame, 
													BIOS_NO_WAIT);

					if ((status != FVID2_SOK) || (pEmptyFrame == NULL))
					{
						/* No empty buffers available from the output queue */
						/* So return the full frames back to previous link */
						System_putLinksEmptyFrames(pInQueParams->prevLinkId,
												   pInQueParams->prevLinkQueId,
												   &frameList);

						return FVID2_SOK;
					}
					
					pEmptyFrame->timeStamp = frameList.frames[frameId]->timeStamp;
					pDrvObj->outFrameList.frames[pDrvObj->outFrameList.numFrames] = pEmptyFrame;
					pDrvObj->outFrameList.numFrames++;	
                }					

				if((queId + 1) < pObj->createArgs.numOutQueue)
				{
					if (((pObj->inFrameCnt < 30) && (((pObj->frameSkipMask[queId + 1] >> pObj->inFrameCnt) & 0x1) == 0u)) ||
						((pObj->inFrameCnt >= 30) && (((pObj->frameSkipMaskHigh[queId + 1] >> (pObj->inFrameCnt - 30)) & 0x1) == 0u)))
					{
						/* Get frame buffer from empty queue */
						status = Utils_bufGetEmptyFrame(&pObj->outFrameQue[queId + 1],
														&pEmptyFrame, 
														BIOS_NO_WAIT);

						if ((status != FVID2_SOK) || (pEmptyFrame == NULL))
						{
							/* No empty buffers available from the output queue */
							/* So return the full frames back to previous link */
							System_putLinksEmptyFrames(pInQueParams->prevLinkId,
													   pInQueParams->prevLinkQueId,
													   &frameList);

							return FVID2_SOK;
						}
						
						pEmptyFrame->timeStamp = frameList.frames[frameId]->timeStamp;
						pDrvObj->outFrameList.frames[pDrvObj->outFrameList.numFrames] = pEmptyFrame;
						pDrvObj->outFrameList.numFrames++;				
					}
				}

				rszCfg.inDataFmt   = (FVID2_DataFormat)pObj->inQueInfo.chInfo[0].dataFormat;
				rszCfg.outDataFmt0 = (FVID2_DataFormat)pObj->createArgs.outQueuePrm[queId].dataFormat;
				rszCfg.outDataFmt1 = (FVID2_DataFormat)pObj->createArgs.outQueuePrm[queId + 1].dataFormat;
				rszCfg.inWidth     = pObj->inQueInfo.chInfo[0].width;
				rszCfg.inHeight    = pObj->inQueInfo.chInfo[0].height;
				rszCfg.inPitch     = pObj->inQueInfo.chInfo[0].pitch[0];
				rszCfg.outWidth0   = pObj->createArgs.outQueuePrm[queId].width;
				rszCfg.outHeight0  = pObj->createArgs.outQueuePrm[queId].height;
				rszCfg.pitch0      = pObj->pitch0[queId];
				rszCfg.outWidth1   = pObj->createArgs.outQueuePrm[queId + 1].width;
				rszCfg.outHeight1  = pObj->createArgs.outQueuePrm[queId + 1].height;
				rszCfg.pitch1      = pObj->pitch0[queId + 1];
				rszCfg.numStream   = pDrvObj->outFrameList.numFrames;

				status = FVID2_control(pDrvObj->fvidHandle,
									   IOCTL_ISS_ISP_RSZ_CONFIG,
							           &rszCfg,
									   NULL);		
				UTILS_assert(status == FVID2_SOK);					   

#ifdef SYSTEM_DEBUG_ISP_RT								
				start = Utils_getCurTimeInMsec(); 
#endif				
				
				/* Process the frames */
				status = FVID2_processFrames(pDrvObj->fvidHandle, &pDrvObj->processList);
				UTILS_assert(status == FVID2_SOK);				
				
				Semaphore_pend(pDrvObj->complete, BIOS_WAIT_FOREVER);

#ifdef SYSTEM_DEBUG_ISP_RT								
				end = Utils_getCurTimeInMsec();
				Vps_printf(" %d: ISP   : Process time = %d msec !!!\n",
						   Clock_getTicks(), (end - start));				
#endif				
				
				if (((pObj->inFrameCnt < 30) && (((pObj->frameSkipMask[queId] >> pObj->inFrameCnt) & 0x1) == 0u)) ||
					((pObj->inFrameCnt >= 30) && (((pObj->frameSkipMaskHigh[queId] >> (pObj->inFrameCnt - 30)) & 0x1) == 0u)))
				{
					/* Put the full buffers */
					status = Utils_bufPutFullFrame(&pObj->outFrameQue[queId], pDrvObj->outFrameList.frames[0]);
					UTILS_assert(status == FVID2_SOK);

					/* send SYSTEM_CMD_NEW_DATA to next link */
					System_sendLinkCmd(pObj->createArgs.outQueInfo[queId].nextLink,
									   SYSTEM_CMD_NEW_DATA);
				}				   

				if((queId + 1) < pObj->createArgs.numOutQueue)
                {
					if (((pObj->inFrameCnt < 30) && (((pObj->frameSkipMask[queId + 1] >> pObj->inFrameCnt) & 0x1) == 0u)) ||
						((pObj->inFrameCnt >= 30) && (((pObj->frameSkipMaskHigh[queId + 1] >> (pObj->inFrameCnt - 30)) & 0x1) == 0u)))
					{ 
						/* Put the full buffers */
						status = Utils_bufPutFullFrame(&pObj->outFrameQue[queId + 1], pDrvObj->outFrameList.frames[1]);
						UTILS_assert(status == FVID2_SOK);

						/* send SYSTEM_CMD_NEW_DATA to next link */
						System_sendLinkCmd(pObj->createArgs.outQueInfo[queId + 1].nextLink,
										   SYSTEM_CMD_NEW_DATA);
					}				   
                } 				

				pDrvObj->outFrameList.numFrames = 0;
			}
			
			pObj->inFrameCnt++;
			pObj->inFrameCnt %= 60;
        }

        /* Put the empty buffers back to previous link */
        System_putLinksEmptyFrames(pInQueParams->prevLinkId,
                                   pInQueParams->prevLinkQueId, &frameList);
    }

    return FVID2_SOK;
}

/* Delete the driver instance */

/* ===================================================================
 *  @func     IspLink_drvDeleteFvidObj                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
Int32 IspLink_drvDeleteFvidObj(IspLink_Obj * pObj)
{
    IspLink_drvObj *pDrvObj;

    pDrvObj = &pObj->drvObj;
    FVID2_delete(pDrvObj->fvidHandle, NULL);
    Semaphore_delete(&pDrvObj->complete);

#ifdef SYSTEM_DEBUG_ISP
    Vps_printf(" %d: ISP   : FVID2 Delete Done !!!\n", Clock_getTicks());
#endif

    return FVID2_SOK;
}
