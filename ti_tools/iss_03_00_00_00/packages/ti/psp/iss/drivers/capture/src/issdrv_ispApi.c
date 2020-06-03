/** ==================================================================

 *  @file   issdrv_ispApi.c

 *

 *  @path   /ti/psp/iss/drivers/capture/src/

 *

 *  @desc   This  File contains.

 * ===================================================================

 *  Copyright (c) Texas Instruments Inc 2011, 2012

 *

 *  Use of this software is controlled by the terms and conditions found

 *  in the license agreement under which this software has been supplied

 * ===================================================================*/

#include <xdc/std.h>
#include <ti/psp/iss/drivers/capture/src/issdrv_ispPriv.h>
#include <ti/psp/iss/hal/iss/isp/isif/inc/isif.h>
#include <ti/psp/iss/hal/iss/isp/rsz/inc/rsz.h>
#include <ti/psp/iss/hal/iss/isp/ipipe/inc/ipipe.h>
#include <ti/psp/iss/core/inc/iss_drv_common.h>
#include <ti/psp/iss/core/inc/iss_drv.h>

extern isif_regs_ovly isif_reg;
extern ipipe_regs_ovly ipipe_reg;
extern rsz_regs_ovly rsz_reg;
extern rsz_A_regs_ovly rszA_reg;
extern rsz_B_regs_ovly rszB_reg;
extern ipipeif_regs_ovly ipipeif_reg;

Iss_PpCommonObj gIss_ispCommonObj;

/* ISS Post Process Write reg */
Void Iss_ispWriteReg(volatile UInt32 *pRegAddr,UInt32 value,UInt32 bitPosn,UInt32 numBits)
{
    UInt32 i,mask = 0;

	for(i = 0;i < numBits;i ++)
	{
	    mask |= (1 << (bitPosn + i));
	}
	mask = ~mask;

	*pRegAddr &= mask;
	*pRegAddr |= (value << bitPosn);
}

/* ISS Post Process Callback */
Int32 Iss_ispDrvCallBack()
{
    Int32 instId;
	Iss_PpObj *pObj;

	for (instId = 0; instId < ISS_ISP_INST_MAX; instId++)
	{
        pObj = (Iss_PpObj *)&gIss_ispCommonObj.ispObj[instId];
	    if(pObj->cbPrm.fdmCbFxn != NULL)
	    {
	        if(pObj->usrCbFlag == 0)
			{
			    pObj->cbPrm.fdmCbFxn(pObj->cbPrm.fdmData,pObj);
			}
			else
            {
			    pObj->usrCbFlag = 0;
                Semaphore_post(pObj->sem420);
            }
	    }
	}

	return FVID2_SOK;
}

/* ISS Post Process Lock create */
Int32 Iss_ispLockCreate(Iss_PpObj *pObj)
{
    Int32 retVal = FVID2_SOK;
    Semaphore_Params semParams;

    /*
     * create locking semaphore
     */
    Semaphore_Params_init(&semParams);

    semParams.mode = Semaphore_Mode_BINARY;
    pObj->lock = Semaphore_create(1u, &semParams, NULL);

    if (pObj->lock == NULL)
    {
        Vps_printf(" ISP:%s:%d: Semaphore_create() failed !!!\n",
                   __FUNCTION__, __LINE__);
        retVal = FVID2_EALLOC;
    }

    return retVal;
}

/* ISS Post Process lock */
Int32 Iss_ispLock(Iss_PpObj * pObj)
{
    /*
     * take semaphore for locking
     */
    Semaphore_pend(pObj->lock, BIOS_WAIT_FOREVER);

    return FVID2_SOK;
}

/* ISS Post Process unlock */
Int32 Iss_ispUnlock(Iss_PpObj * pObj)
{
    /*
     * release semaphore for un-locking
     */
    Semaphore_post(pObj->lock);

    return FVID2_SOK;
}

/* ISS Post Process Lock delete */
Int32 Iss_ispLockDelete(Iss_PpObj *pObj)
{
    /*
     * delete semaphore
     */
    Semaphore_delete(&pObj->lock);

    return FVID2_SOK;
}

/* ISS Post Process Create */
Fdrv_Handle Iss_ispCreate(UInt32 drvId,
                         UInt32 instanceId,
                         Iss_PpCreateParams *pCreateArgs,
                         Iss_PpCreateStatus *pCreateStatus,
                         const FVID2_CbParams *pCbPrm)
{
	Iss_PpObj *pObj;
	Semaphore_Params semParams;

    if (instanceId == ISS_ISP_INST_ALL)
    {
        /*
         * gloabl handle open requested, no action required,
         * just return a special handle ID
         */
        return (Fdrv_Handle)ISS_ISP_INST_ALL;
    }

    if (pCreateStatus == NULL)
	{
        return NULL;
	}

    if ((instanceId >= ISS_ISP_INST_MAX) || (pCreateArgs == NULL) || (drvId != FVID2_ISS_M2M_ISP_DRV))
    {
        /*
         * invalid parameters - return NULL
         */
        pCreateStatus->retVal = FVID2_EBADARGS;
        return NULL;
    }

	pObj = &gIss_ispCommonObj.ispObj[instanceId];

    /* Semaphore for the YUV420 -> YUV420 resizing */
	Semaphore_Params_init(&semParams);

    semParams.mode = Semaphore_Mode_BINARY;
    pObj->sem420 = Semaphore_create(0u, &semParams, NULL);

    /*
     * lock driver instance
     */
    Iss_ispLock(pObj);

    /*
     * check if object is already opended
     */

    if (pObj->state != ISS_ISP_STATE_IDLE)
    {
        pCreateStatus->retVal = FVID2_EDEVICE_INUSE;
        Iss_ispUnlock(pObj);
        return NULL;
    }

	memcpy(&pObj->createArgs, pCreateArgs, sizeof(pObj->createArgs));

	pObj->instanceId = instanceId;

    if (pCbPrm != NULL)
	{
        memcpy(&pObj->cbPrm, pCbPrm, sizeof(pObj->cbPrm));
	}

	{
		// IPIPEIF register setting
		Iss_ispWriteReg(&ipipeif_reg->CFG1,1,0,1);									// ONESHOT -> One shot mode
		Iss_ispWriteReg(&ipipeif_reg->CFG1,1,10,1);									// CLKSEL -> Selects the pixel clock from the fractional clock divider

		Iss_ispWriteReg(&ipipeif_reg->CLKDIV,(pObj->createArgs.clkDivN - 1),0,8);	// CLKDIV -> N = CLKDIV[7:0] + 1
		Iss_ispWriteReg(&ipipeif_reg->CLKDIV,(pObj->createArgs.clkDivM - 1),8,8);	// CLKDIV -> M = CLKDIV[15:8] + 1

		// IPIPE register setting
		Iss_ispWriteReg(&ipipe_reg->SRC_MODE,1,0,1);							 	// OST -> One shot
        Iss_ispWriteReg(&ipipe_reg->SRC_MODE,1,1,1);								// WRT -> Enable
		Iss_ispWriteReg(&ipipe_reg->SRC_COL,3,0,2);									// EE -> B
		Iss_ispWriteReg(&ipipe_reg->SRC_COL,2,2,2);									// EO -> Gb
		Iss_ispWriteReg(&ipipe_reg->SRC_COL,1,4,2);									// OE -> Gr
		Iss_ispWriteReg(&ipipe_reg->SRC_COL,0,6,2);									// OO -> R

		// RSZ register setting
		Iss_ispWriteReg(&rsz_reg->SRC_MODE,1,0,1);									// OST -> One shot
		Iss_ispWriteReg(&rsz_reg->SRC_MODE,1,1,1);									// WRT -> Enable
		Iss_ispWriteReg(&rszA_reg->RZA_MODE,1,0,1);									// MODE -> One shot
		Iss_ispWriteReg(&rszB_reg->RZB_MODE,1,0,1);									// MODE -> One shot

		rsz_clock_enable(RESIZER_A,RSZ_CLK_ENABLE);
		rsz_clock_enable(RESIZER_B,RSZ_CLK_ENABLE);
		isp_enable_interrupt(ISP_RSZ_INT_DMA);
	}

    pObj->state = ISS_ISP_STATE_CREATED;

    /*
     * unlock driver instance
     */
    Iss_ispUnlock(pObj);

	return pObj;
}

/* ISS Post Process Process Frames */
Int32 Iss_ispProcessFrames(FVID2_Handle handle,
                           FVID2_ProcessList *procList)
{
    Int32 retVal = FVID2_SOK;
	Iss_PpObj *pObj = (Iss_PpObj *)handle;
	Ptr inAddr,inAddrC;
	Ptr outAddrAY,outAddrAC,outAddrBY,outAddrBC;

    if (handle == NULL || procList == NULL)
	{
	    retVal = FVID2_EBADARGS;
		return retVal;
	}

	pObj->usrCbFlag = 0;

	if(pObj->numStream == 0)
	{
	    Iss_ispDrvCallBack();
	    return retVal;
	}

	{
	    // set the input address
		inAddr  = procList->inFrameList[0]->frames[0]->addr[0][0];
		inAddrC = procList->inFrameList[0]->frames[0]->addr[0][1];

		if(pObj->numStream == 2)
		{
		    outAddrAY = procList->outFrameList[0]->frames[0]->addr[0][0];
			outAddrAC = procList->outFrameList[0]->frames[0]->addr[0][1];
			outAddrBY = procList->outFrameList[0]->frames[1]->addr[0][0];
			outAddrBC = procList->outFrameList[0]->frames[1]->addr[0][1];
		}
		else if(pObj->numStream == 1)
		     {
			     outAddrAY = procList->outFrameList[0]->frames[0]->addr[0][0];
				 outAddrAC = procList->outFrameList[0]->frames[0]->addr[0][1];
			 }
			 else
			 {
			     Vps_printf(" ISP:%s:%d: Invalid numstream !!!\n",
					        __FUNCTION__, __LINE__);
		         retVal = FVID2_EBADARGS;
				 return retVal;
			 }

		Iss_ispWriteReg(&ipipeif_reg->ADDRU,(((UInt32)inAddr >> 21) & 0x07FF),0,11);
		Iss_ispWriteReg(&ipipeif_reg->ADDRL,(((UInt32)inAddr >> 5) & 0xFFFF),0,16);

		switch(pObj->inDataFmt)
		{
		    default:
			case FVID2_DF_BAYER_RAW:
			case FVID2_DF_YUV422I_VYUY:
			{
				switch(pObj->numStream)
				{
				    case 1:
					{
						switch(pObj->outDataFmt0)
						{
						    case FVID2_DF_YUV422I_VYUY:
							{
						        retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV422_RAW_RGB_OP,
															 outAddrAY,
															 pObj->pitch0);
							    break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
							case FVID2_DF_YUV420SP_VU:
							{
								retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV420_Y_OP,
															 outAddrAY,
															 pObj->pitch0);

								retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV420_C_OP,
															 outAddrAC,
															 pObj->pitch0);
								break;
							}
						}

					    break;
					}

                    case 2:
					{
						switch(pObj->outDataFmt0)
						{
						    case FVID2_DF_YUV422I_VYUY:
							{
								retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV422_RAW_RGB_OP,
															 outAddrAY,
															 pObj->pitch0);
							    break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
							case FVID2_DF_YUV420SP_VU:
							{
								retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV420_Y_OP,
															 outAddrAY,
															 pObj->pitch0);

								retVal = issSetRszOutAddress(RESIZER_A,
															 RSZ_YUV420_C_OP,
															 outAddrAC,
															 pObj->pitch0);
								break;
							}
						}

						switch(pObj->outDataFmt1)
						{
						    case FVID2_DF_YUV422I_VYUY:
							{
								retVal = issSetRszOutAddress(RESIZER_B,
															 RSZ_YUV422_RAW_RGB_OP,
															 outAddrBY,
															 pObj->pitch1);
							    break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
							case FVID2_DF_YUV420SP_VU:
							{
								retVal = issSetRszOutAddress(RESIZER_B,
												             RSZ_YUV420_Y_OP,
															 outAddrBY,
															 pObj->pitch1);

								retVal = issSetRszOutAddress(RESIZER_B,
															 RSZ_YUV420_C_OP,
															 outAddrBC,
															 pObj->pitch1);
								break;
							}
						}

						break;
					}
				}

				break;
			}

			case FVID2_DF_YUV420SP_UV:
			case FVID2_DF_YUV420SP_VU:
			{
				switch(pObj->numStream)
				{
				    case 1:
                    {
					    // Y resizing
						Iss_ispWriteReg(&rszA_reg->RZA_420,1,0,1);							// YEN -> Y output enable and 422to420 conversion enabled
						Iss_ispWriteReg(&rszA_reg->RZA_420,0,1,1);							// CEN -> C output disable

						retVal = issSetRszOutAddress(RESIZER_A,
													 RSZ_YUV420_Y_OP,
													 outAddrAY,
													 pObj->pitch0);

						// Wait for IPIPE and IPIPEIF to be free
						while(ipipeif_reg->IPIPEIF_ENABLE == 1);
						while(ipipe_reg->SRC_EN == 1);

						// Trigger RSZ and IPIPEIF
						Iss_ispWriteReg(&rsz_reg->SRC_EN,1,0,1);

						Iss_ispWriteReg(&rszA_reg->RZA_EN,1,0,1);
						Iss_ispWriteReg(&rszB_reg->RZB_EN,0,0,1);

						Iss_ispWriteReg(&ipipe_reg->SRC_EN,1,0,1);
						Iss_ispWriteReg(&ipipeif_reg->IPIPEIF_ENABLE,1,0,1);

						pObj->usrCbFlag = 1;
						Semaphore_pend(pObj->sem420,BIOS_WAIT_FOREVER);

						Iss_ispWriteReg(&ipipeif_reg->ADDRU,(((UInt32)inAddrC >> 21) & 0x07FF),0,11);
						Iss_ispWriteReg(&ipipeif_reg->ADDRL,(((UInt32)inAddrC >> 5) & 0xFFFF),0,16);

						// UV resizing
						Iss_ispWriteReg(&rszA_reg->RZA_420,0,0,1);							// YEN -> Y output disable
						Iss_ispWriteReg(&rszA_reg->RZA_420,1,1,1);							// CEN -> C output enable and 422to420 conversion enabled

						rszASetOutConfig(pObj->inWidth - 2,
										 (pObj->inHeight >> 1) - 2,
										 pObj->outWidth0,
										 pObj->outHeight0 >> 1);

						retVal = issSetRszOutAddress(RESIZER_A,
													 RSZ_YUV420_C_OP,
													 outAddrAC,
													 pObj->pitch0);

					    break;
					}

                    case 2:
					{
						// Y resizing
						Iss_ispWriteReg(&rszA_reg->RZA_420,1,0,1);							// YEN -> Y output enable and 422to420 conversion enabled
						Iss_ispWriteReg(&rszA_reg->RZA_420,0,1,1);							// CEN -> C output disable

						retVal = issSetRszOutAddress(RESIZER_A,
													 RSZ_YUV420_Y_OP,
													 outAddrAY,
													 pObj->pitch0);

						retVal = issSetRszOutAddress(RESIZER_B,
													 RSZ_YUV420_Y_OP,
													 outAddrBY,
													 pObj->pitch1);

						pObj->usrCbFlag = 1;

						// Wait for IPIPE and IPIPEIF to be free
						while(ipipeif_reg->IPIPEIF_ENABLE == 1);
						while(ipipe_reg->SRC_EN == 1);

						// Trigger RSZ and IPIPEIF
						Iss_ispWriteReg(&rsz_reg->SRC_EN,1,0,1);

						Iss_ispWriteReg(&rszA_reg->RZA_EN,1,0,1);
						Iss_ispWriteReg(&rszB_reg->RZB_EN,1,0,1);

						Iss_ispWriteReg(&ipipe_reg->SRC_EN,1,0,1);
						Iss_ispWriteReg(&ipipeif_reg->IPIPEIF_ENABLE,1,0,1);

						// Wait for Resizer completion
						Semaphore_pend(pObj->sem420,BIOS_WAIT_FOREVER);

						Iss_ispWriteReg(&ipipeif_reg->ADDRU,(((UInt32)inAddrC >> 21) & 0x07FF),0,11);
						Iss_ispWriteReg(&ipipeif_reg->ADDRL,(((UInt32)inAddrC >> 5) & 0xFFFF),0,16);

						// UV resizing
						Iss_ispWriteReg(&rszA_reg->RZA_420,0,0,1);							// YEN -> Y output disable
						Iss_ispWriteReg(&rszA_reg->RZA_420,1,1,1);							// CEN -> C output enable and 422to420 conversion enabled

						rszASetOutConfig(pObj->inWidth - 2,
										 (pObj->inHeight >> 1) - 2,
										 pObj->outWidth0,
										 pObj->outHeight0 >> 1);

					   rszBSetOutConfig(pObj->inWidth - 2,
										(pObj->inHeight >> 1) - 2,
										pObj->outWidth1,
										pObj->outHeight1 >> 1);

						retVal = issSetRszOutAddress(RESIZER_A,
													 RSZ_YUV420_C_OP,
													 outAddrAC,
													 pObj->pitch0);

						retVal = issSetRszOutAddress(RESIZER_B,
													 RSZ_YUV420_C_OP,
													 outAddrBC,
													 pObj->pitch1);

						break;
					}
				}

				break;
			}
		} // switch(pObj->inDataFmt)

		// Wait for IPIPE and IPIPEIF to be free
		while(ipipeif_reg->IPIPEIF_ENABLE == 1);
		while(ipipe_reg->SRC_EN == 1);

		// Trigger RSZ and IPIPEIF
		Iss_ispWriteReg(&rsz_reg->SRC_EN,1,0,1);

		if(pObj->numStream == 2)
		{
		    Iss_ispWriteReg(&rszA_reg->RZA_EN,1,0,1);
			Iss_ispWriteReg(&rszB_reg->RZB_EN,1,0,1);
		}
		else
		{
		    Iss_ispWriteReg(&rszA_reg->RZA_EN,1,0,1);
			Iss_ispWriteReg(&rszB_reg->RZB_EN,0,0,1);
		}

		Iss_ispWriteReg(&ipipe_reg->SRC_EN,1,0,1);
		Iss_ispWriteReg(&ipipeif_reg->IPIPEIF_ENABLE,1,0,1);
	}

	return retVal;
}

/* ISS Post Process Control */
Int32 Iss_ispControl(Fdrv_Handle handle, UInt32 cmd,
                    Ptr cmdArgs, Ptr cmdStatusArgs)
{
    Int32 status = FVID2_SOK;
	Iss_PpObj *pObj = (Iss_PpObj *)handle;
	Iss_PpRszConfig *pRszCfg = (Iss_PpRszConfig*)cmdArgs;

	switch(cmd)
	{
	    case IOCTL_ISS_ISP_RSZ_CONFIG:
			{
				pObj->inDataFmt   = pRszCfg->inDataFmt;
				pObj->outDataFmt0 = pRszCfg->outDataFmt0;
				pObj->outDataFmt1 = pRszCfg->outDataFmt1;
                pObj->numStream   = (pRszCfg->numStream >= 2)?2:1;
				pObj->inWidth     = pRszCfg->inWidth;
				pObj->inHeight    = pRszCfg->inHeight;
				pObj->outWidth0   = pRszCfg->outWidth0;
				pObj->outHeight0  = pRszCfg->outHeight0;
				pObj->pitch0      = pRszCfg->pitch0;
				pObj->outWidth1   = pRszCfg->outWidth1;
				pObj->outHeight1  = pRszCfg->outHeight1;
				pObj->pitch1      = pRszCfg->pitch1;

				if(pObj->numStream == 0)
				{
				    break;
				}

				Iss_ispWriteReg(&ipipeif_reg->PPLN,((pRszCfg->inWidth + 16) - 1),0,13);
				Iss_ispWriteReg(&ipipeif_reg->LPFR,((pRszCfg->inHeight + 16) - 1),0,13);
				Iss_ispWriteReg(&ipipeif_reg->HNUM,(pRszCfg->inWidth - 1),0,13);
				Iss_ispWriteReg(&ipipeif_reg->VNUM,(pRszCfg->inHeight - 1),0,13);
				Iss_ispWriteReg(&ipipeif_reg->ADOFS,(pRszCfg->inPitch >> 5),0,12);

                // IPIPE size setting
				Iss_ispWriteReg(&ipipe_reg->SRC_HPS,0,0,16);
				Iss_ispWriteReg(&ipipe_reg->SRC_HSZ,(pRszCfg->inWidth - 1),0,13);
 				Iss_ispWriteReg(&ipipe_reg->SRC_VPS,0,0,16);
				Iss_ispWriteReg(&ipipe_reg->SRC_VSZ,(pRszCfg->inHeight - 1),0,13);

				switch(pRszCfg->inDataFmt)
				{
				    default:
				    case FVID2_DF_BAYER_RAW:
					{
					    /* RAW input YUV output */

                        // IPIPEIF reg
						Iss_ispWriteReg(&ipipeif_reg->CFG1,1,2,2);						// INPSRC2 -> SDRAM_RAW
						Iss_ispWriteReg(&ipipeif_reg->CFG1,0,8,2);						// UNPACK -> 16 bits / pixel

						// RSZ reg
						Iss_ispWriteReg(&rsz_reg->SRC_FMT0,0,0,1);						// SEL -> IPIPE
						Iss_ispWriteReg(&rsz_reg->SRC_FMT1,0,0,1);						// RAW -> Flipping preserves YCbCr format
						Iss_ispWriteReg(&rsz_reg->SRC_FMT1,0,1,1);						// IN420 -> YUV422

                        switch(pRszCfg->outDataFmt0)
                        {
                            case FVID2_DF_YUV422I_VYUY:
							{
								// RSZA reg
								Iss_ispWriteReg(&rszA_reg->RZA_420,0,0,1);				// YEN -> Y output disable
								Iss_ispWriteReg(&rszA_reg->RZA_420,0,1,1);				// CEN -> C output disable
                                break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
                            case FVID2_DF_YUV420SP_VU:
							{
								// RSZA reg
								Iss_ispWriteReg(&rszA_reg->RZA_420,1,0,1);				// YEN -> Y output enable and 422to420 conversion enabled
								Iss_ispWriteReg(&rszA_reg->RZA_420,1,1,1);				// CEN -> C output enable and 422to420 conversion enabled
                                break;
							}
						}

                        switch(pRszCfg->outDataFmt1)
						{
                            case FVID2_DF_YUV422I_VYUY:
							{
								// RSZB reg
								Iss_ispWriteReg(&rszB_reg->RZB_420,0,0,1);				// YEN -> Y output disable
								Iss_ispWriteReg(&rszB_reg->RZB_420,0,1,1);				// CEN -> C output disable
                                break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
                            case FVID2_DF_YUV420SP_VU:
							{
								// RSZB reg
								Iss_ispWriteReg(&rszB_reg->RZB_420,1,0,1);				// YEN -> Y output enable and 422to420 conversion enabled
								Iss_ispWriteReg(&rszB_reg->RZB_420,1,1,1);				// CEN -> C output enable and 422to420 conversion enabled
                                break;
							}
						}

					    break;
					}

					case FVID2_DF_YUV422I_VYUY:
					{
					    /* YUV 422 input and YUV output */

                        // IPIPEIF reg
						Iss_ispWriteReg(&ipipeif_reg->CFG1,3,2,2);						// INPSRC2 -> SDRAM_YUV
						Iss_ispWriteReg(&ipipeif_reg->CFG1,0,8,2);		    			// UNPACK -> 16 bits / pixel

				        // RSZ reg
						Iss_ispWriteReg(&rsz_reg->SRC_FMT0,1,0,1);						// SEL -> IPIPEIF
                        Iss_ispWriteReg(&rsz_reg->SRC_FMT1,0,0,1);		    			// RAW -> Flipping preserves YCbCr format
						Iss_ispWriteReg(&rsz_reg->SRC_FMT1,0,1,1);						// IN420 -> YUV422

						switch(pRszCfg->outDataFmt0)
						{
						    case FVID2_DF_YUV422I_VYUY:
							{
								// RSZA reg
								Iss_ispWriteReg(&rszA_reg->RZA_420,0,0,1);				// YEN -> Y output disable
								Iss_ispWriteReg(&rszA_reg->RZA_420,0,1,1);				// CEN -> C output disable
							    break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
                            case FVID2_DF_YUV420SP_VU:
							{
								// RSZA reg
								Iss_ispWriteReg(&rszA_reg->RZA_420,1,0,1);				// YEN -> Y output enable and 422to420 conversion enabled
								Iss_ispWriteReg(&rszA_reg->RZA_420,1,1,1);				// CEN -> C output enable and 422to420 conversion enabled
							    break;
							}
						}

						switch(pRszCfg->outDataFmt1)
						{
						    case FVID2_DF_YUV422I_VYUY:
							{
								// RSZB reg
								Iss_ispWriteReg(&rszB_reg->RZB_420,0,0,1);				// YEN -> Y output disable
								Iss_ispWriteReg(&rszB_reg->RZB_420,0,1,1);				// CEN -> C output disable
							    break;
							}

							default:
							case FVID2_DF_YUV420SP_UV:
                            case FVID2_DF_YUV420SP_VU:
							{
								// RSZB reg
								Iss_ispWriteReg(&rszB_reg->RZB_420,1,0,1);				// YEN -> Y output enable and 422to420 conversion enabled
								Iss_ispWriteReg(&rszB_reg->RZB_420,1,1,1);				// CEN -> C output enable and 422to420 conversion enabled
							    break;
							}
						}

					    break;
					}

					case FVID2_DF_YUV420SP_UV:
					case FVID2_DF_YUV420SP_VU:
					{
					    /* YUV 420 input and YUV 420 output */

					    // IPIPEIF reg
                        Iss_ispWriteReg(&ipipeif_reg->CFG1,3,2,2); 						// INPSRC2 -> SDRAM_YUV
					    Iss_ispWriteReg(&ipipeif_reg->CFG1,1,8,2); 						// UNPACK -> 8 bits / pixel

				        // RSZ reg
					    Iss_ispWriteReg(&rsz_reg->SRC_FMT0,1,0,1);						// SEL -> IPIPEIF
						Iss_ispWriteReg(&rsz_reg->SRC_FMT1,0,0,1);						// RAW -> Flipping preserves YCbCr format
						Iss_ispWriteReg(&rsz_reg->SRC_FMT1,1,1,1);      				// IN420 -> YUV4:2:0 is input

					    break;
					}
				}

				if(pObj->numStream == 2)
				{
					rszASetOutConfig(pRszCfg->inWidth - 2,
									 pRszCfg->inHeight - 4,
									 pRszCfg->outWidth0,
									 pRszCfg->outHeight0);

					rszBSetOutConfig(pRszCfg->inWidth - 2,
									 pRszCfg->inHeight - 4,
									 pRszCfg->outWidth1,
									 pRszCfg->outHeight1);
				}
				else
				{
					rszASetOutConfig(pRszCfg->inWidth - 2,
									 pRszCfg->inHeight - 4,
									 pRszCfg->outWidth0,
									 pRszCfg->outHeight0);

					/* Set the resolution params even if RSZ B is going to be disabled */
					rszBSetOutConfig(pRszCfg->inWidth - 2,
									 pRszCfg->inHeight - 4,
									 pRszCfg->outWidth0,
									 pRszCfg->outHeight0);
				}
			}
			break;

		default:
			break;
	}

	return status;
}

/* ISS Post Process Delete */
Int32 Iss_ispDelete(Fdrv_Handle handle, Ptr reserved)
{
    Int32 status = FVID2_SOK;
    Iss_PpObj *pObj = (Iss_PpObj *)handle;

    if (handle == (Fdrv_Handle) ISS_ISP_INST_ALL)
	{
        return FVID2_SOK;
	}

    GT_assert(GT_DEFAULT_MASK, pObj != NULL);

    /*
     * lock driver instance
     */
    Iss_ispLock(pObj);

    if (pObj->state == ISS_ISP_STATE_STOPPED || pObj->state == ISS_ISP_STATE_CREATED)
    {
	    // Stop the rsz and ipipeif
		rsz_reg->SRC_EN = 0;
		ipipeif_reg->IPIPEIF_ENABLE = 0;
		pObj->state = ISS_ISP_STATE_IDLE;
	}

    /*
     * unlock driver instance
     */
    Iss_ispUnlock(pObj);

	Semaphore_delete(&pObj->sem420);

	return status;
}

/* ISS Post Process Init */
Int32 Iss_ispInit()
{
    Int32 status = FVID2_SOK, instId;

	memset(&gIss_ispCommonObj, 0, sizeof(gIss_ispCommonObj));

    for (instId = 0; instId < ISS_ISP_INST_MAX; instId++)
	{
        status |= Iss_ispLockCreate(&gIss_ispCommonObj.ispObj[instId]);
	}

    if (status != FVID2_SOK)
	{
        return status;
	}

    if (status == FVID2_SOK)
    {
        /*

         * register driver to FVID2 layer

         */
        gIss_ispCommonObj.fvidDrvOps.create             = (FVID2_DrvCreate)Iss_ispCreate;
        gIss_ispCommonObj.fvidDrvOps.delete             = Iss_ispDelete;
        gIss_ispCommonObj.fvidDrvOps.control            = Iss_ispControl;
        gIss_ispCommonObj.fvidDrvOps.queue              = NULL;
        gIss_ispCommonObj.fvidDrvOps.dequeue            = NULL;
        gIss_ispCommonObj.fvidDrvOps.processFrames      = Iss_ispProcessFrames;
        gIss_ispCommonObj.fvidDrvOps.getProcessedFrames = NULL;
        gIss_ispCommonObj.fvidDrvOps.drvId              = FVID2_ISS_M2M_ISP_DRV;

        status = FVID2_registerDriver(&gIss_ispCommonObj.fvidDrvOps);

        if (status != FVID2_SOK)
        {
			Vps_printf(" ISP:%s:%d: FVID2_registerDriver() failed !!!\n",
					   __FUNCTION__, __LINE__);
        }
    }

    return status;
}

/* ISS Post Process Deinit */
Int32 Iss_ispDeInit()
{
    Int32 instId;

    /*
     * unregister from FVID2 layer
     */
    FVID2_unRegisterDriver(&gIss_ispCommonObj.fvidDrvOps);

    /*
     * delete driver locks
     */
    for (instId = 0; instId < ISS_ISP_INST_MAX; instId++)
	{
        Iss_ispLockDelete(&gIss_ispCommonObj.ispObj[instId]);
	}

    return FVID2_SOK;
}
