/** ==================================================================
 *  @file   issdrv_alg2AApi.c.c
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
#include <stdlib.h>
#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_alg2APriv.h>
#include <ti/psp/iss/core/inc/iss_drv_common.h>
#include <ti/psp/vps/common/vps_config.h>
#include <ti/psp/iss/drivers/alg/2A/inc/issdrv_algTIaewb.h>
#include <ti/psp/devices/iss_sensorDriver.h>
#include <ti/psp/iss/alg/dcc/inc/idcc.h>
#include <ti/psp/iss/drivers/capture/src/issdrv_capturePriv.h>

#include <ti/psp/iss/alg/aewb/hive2a/inc/hive_2a.h>
#include <ti/psp/iss/alg/aewb/ud2a/inc/iss_ud2a.h>
#include <ti/psp/iss/hal/iss/isp/ipipe/inc/ipipe.h>
#include <ti/psp/iss/hal/iss/isp/ipipe/inc/ipipe_reg.h>

extern ti2a_output ti2a_output_params;
extern ti2aControlParams_t gTi2aControlParams;

/* 2A Exec Param from Hive algorithm */
extern EXE_PARM hive_2a_param;
extern int16 hive_rgb_rgb1[12];
extern int16 hive_rgb_rgb2[12];

Iss_2AObj *gpIssAlg2AObj;

/* Stack for 2A task */
static Int32 start_2A = 0;	//# jaeki - 2A hang issue

#pragma DATA_ALIGN(gIssAlg_captTskStack2A, 32)
#pragma DATA_SECTION(gIssAlg_captTskStack2A, ".bss:taskStackSection")

UInt8 gIssAlg_captTskStack2A[ISSALG_CAPT_TSK_STACK_2A];

//#define TWOA_PROFILE

#ifdef TWOA_PROFILE
    extern UInt32 Utils_getCurTimeInMsec();
#endif

extern ipipe_regs_ovly ipipe_reg;
extern isif_regs_ovly isif_reg;
/* H3A ISR call back function */

#define DBG_ITUNING(msg, args...)

/* ===================================================================
 *  @func     IssAlg_capt2AIsrCallBack
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
Int32 IssAlg_capt2AIsrCallBack(Iss_2AObj *pObj)
{
	/* save the full H3A buf address */

	pObj->fullH3ABufAddr = pObj->h3aBufAddr[pObj->curH3aBufIdx];
	/* post the 2A sem */

	Semaphore_post(pObj->sem);



	/* switch the H3A buffer */

    pObj->curH3aBufIdx = pObj->curH3aBufIdx?0:1;

    h3a_config_aewb_op_addr(pObj->h3aBufAddr[pObj->curH3aBufIdx]);

	h3a_config_af_op_addr(pObj->h3aBufAddr[pObj->curH3aBufIdx] + pObj->h3aBufSize);

    return FVID2_SOK;

}



/****************************************************************************************
                                SET FUNCTIONS
****************************************************************************************/

/* Set color */

/* ===================================================================
 *  @func     IssAlg_capt2ASetColor
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
Int32 IssAlg_capt2ASetColor(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;
    Iss_CaptColor *pColorPrm = (Iss_CaptColor*)cmdArgs;

    pObj->saturation = pColorPrm->saturation;
    pObj->contrast   = pColorPrm->contrast;
    pObj->brightness = pColorPrm->brightness;

    return FVID2_SOK;
}

/* Set AEWB Vendor */

/* ===================================================================
 *  @func     IssAlg_capt2ASetAEWBVendor
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
Int32 IssAlg_capt2ASetAEWBVendor(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->aewbVendor = (AEWB_VENDOR)(*((UInt32*)cmdArgs));

    return FVID2_SOK;
}

/* Set AEWB mode */

/* ===================================================================
 *  @func     IssAlg_capt2ASetAEWBMode
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
Int32 IssAlg_capt2ASetAEWBMode(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->aewbMode = *((UInt32*)cmdArgs);
	pObj->aewbModeFlag = 1;

    return FVID2_SOK;
}

/* Set Iris */

/* ===================================================================
 *  @func     IssAlg_capt2ASetIris
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
Int32 IssAlg_capt2ASetIris(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->AutoIris = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set Frame Rate */

/* ===================================================================
 *  @func     IssAlg_capt2ASetFrameRate
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
Int32 IssAlg_capt2ASetFrameRate(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->FrameRate = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set AEWB Priority */

/* ===================================================================
 *  @func     IssAlg_capt2ASetAEWBPri
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
Int32 IssAlg_capt2ASetAEWBPri(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->aewbPriority = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set sharpness */

/* ===================================================================
 *  @func     IssAlg_capt2ASetSharpness
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
Int32 IssAlg_capt2ASetSharpness(Fdrv_Handle handle, Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;
	Int32 sharpness;

	if(pObj->aewbVendor == AEWB_ID_UD)
    {
    	sharpness = *((Int32*)cmdArgs);
    	UD_IssAlg_setSharpness(sharpness);
    }

    return FVID2_SOK;
}


/* ===================================================================
 *  @func     IssAlg_capt2ASetBrightness
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
Int32 IssAlg_capt2ASetBrightness(Fdrv_Handle handle, Ptr cmdArgs)

{
    Iss_2AObj *pObj = gpIssAlg2AObj;
	Int32 brightness;

	if(pObj->aewbVendor == AEWB_ID_UD)
    {
    	brightness = *((Int32*)cmdArgs);
    	UD_IssAlg_setBrightness(brightness);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     IssAlg_capt2ASetContrast
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
Int32 IssAlg_capt2ASetContrast(Fdrv_Handle handle, Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;
	Int32 contrast;

	if(pObj->aewbVendor == AEWB_ID_UD)
    {
    	contrast = *((Int32*)cmdArgs);
    	UD_IssAlg_setContrast(contrast);
    }

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     IssAlg_capt2ASetSaturation
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
Int32 IssAlg_capt2ASetSaturation(Fdrv_Handle handle, Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;
	Int32 saturation;

	if(pObj->aewbVendor == AEWB_ID_UD)
    {
    	saturation = *((Int32*)cmdArgs);
    	UD_IssAlg_setSaturation(saturation);
    }

    return FVID2_SOK;
}
/* Set BLC */

/* ===================================================================
 *  @func     IssAlg_capt2ASetBlc
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
Int32 IssAlg_capt2ASetBlc(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->blc = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set AWB Mode */

/* ===================================================================
 *  @func     IssAlg_capt2ASetAwbMode
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
Int32 IssAlg_capt2ASetAwbMode(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->AWBMode = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set AE mode */

/* ===================================================================
 *  @func     IssAlg_capt2ASetAeMode
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
Int32 IssAlg_capt2ASetAeMode(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->AEMode = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set ENV */

/* ===================================================================
 *  @func     IssAlg_capt2ASetEnv
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
Int32 IssAlg_capt2ASetEnv(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->Env = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set Binning */

/* ===================================================================
 *  @func     IssAlg_capt2ASetBinning
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
Int32 IssAlg_capt2ASetBinning(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    pObj->Binning = *((UInt32*)cmdArgs);

    return FVID2_SOK;
}

/* Set DCC param */

/* ===================================================================
 *  @func     IssAlg_capt2ASetDccPrm
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
Int32 IssAlg_capt2ASetDccPrm(Fdrv_Handle handle,Ptr cmdArgs)
{
    Iss_2AObj *pObj = gpIssAlg2AObj;
    Iss_CaptDccPrm *pDccPrm = (Iss_CaptDccPrm*)cmdArgs;

    pObj->dcc_Default_Param = pDccPrm->dcc_Default_Param;
    pObj->dcc_init_done     = pDccPrm->dcc_init_done;
    pObj->dccSize           = pDccPrm->dccSize;

    return FVID2_SOK;
}

/****************************************************************************************
                                GET FUNCTIONS
****************************************************************************************/

/* Get AEWB vendor */

/* ===================================================================
 *  @func     IssAlg_capt2AGetAEWBVendor
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
AEWB_VENDOR IssAlg_capt2AGetAEWBVendor()
{
    Iss_2AObj *pObj = gpIssAlg2AObj;

    return (pObj->aewbVendor);
}

/****************************************************************************************
                        TI 2A functions
*****************************************************************************************/
void TI_2A_UpdateDynamicParams1(Iss_2AObj *pObj)
{
    static int flickerFreq = 0;

    if (gTi2aControlParams.brightness != pObj->brightness ||
        gTi2aControlParams.contrast != pObj->contrast)
    {
        gTi2aControlParams.brightness = pObj->brightness;
        gTi2aControlParams.contrast = pObj->contrast;
        gTi2aControlParams.update |= TI2A_UPDATE_CNT_BRGT;
    }

    if (gTi2aControlParams.saturation != pObj->saturation)
    {
        gTi2aControlParams.saturation = pObj->saturation;
        gTi2aControlParams.update |= TI2A_UPDATE_SATURATION;
    }

    if (gTi2aControlParams.sharpness != pObj->sharpness)
    {
        gTi2aControlParams.sharpness = pObj->sharpness;
        gTi2aControlParams.update |= TI2A_UPDATE_SHARPNESS;
    }

    if (gTi2aControlParams.day_night == pObj->aewbPriority)
    {
        gTi2aControlParams.day_night = !(pObj->aewbPriority);
        gTi2aControlParams.update |= TI2A_UPDATE_AE_DAY_NIGHT;
    }

    if (gTi2aControlParams.blc != pObj->blc)
    {
        gTi2aControlParams.blc = pObj->blc;
        gTi2aControlParams.update |= TI2A_UPDATE_BLC;
    }

    if (pObj->Env != flickerFreq)
    {
        flickerFreq = pObj->Env;
        switch (flickerFreq)
        {		/* <TBR: Uday>Sel between 50/60Hz flicker */
            case 0:
                gTi2aControlParams.flickerFreq = 60;
                break;
            case 1:
                gTi2aControlParams.flickerFreq = 50;
                break;
            default:
                gTi2aControlParams.flickerFreq = 0;
                break;
        }
        gTi2aControlParams.update |= TI2A_UPDATE_CONTROL_PARAMS_2A;
    }

	if(pObj->aewbModeFlag == 1){
		gTi2aControlParams.aewbType = pObj->aewbMode;
		pObj->aewbModeFlag = 0;
	// 0=off, 1=AE,
	}
    // 2=AWB, 3=AE+AWB

    gTi2aControlParams.wbSceneMode = pObj->AWBMode;
}

/* TI 2A Process Call */

/* ===================================================================
 *  @func     IssAlg_capt2AProcessTI
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
Int32 IssAlg_capt2AProcessTI(Iss_2AObj *pObj)
{
	Int32 status = FVID2_SOK;
	static UInt32 dccFrmCnt = 0;

	TI_2A_UpdateDynamicParams1(pObj);
	status = ALG_aewbRun((Void*)pObj->fullH3ABufAddr);
	ALG_aewbControl();

    if (status == FVID2_SOK)
    {
        // AE write to SENSOR
        status = FVID2_control(pObj->SensorHandle, IOCTL_ISS_SENSOR_UPDATE_EXP_GAIN,
                          &ti2a_output_params, NULL);
        if (status != FVID2_SOK)
        {
            Vps_rprintf("\nAE write to sensor failed!\n");
            return status;
        }

        pObj->AEWBValue1 = ti2a_output_params.sensorExposure/100;  //10 bit
        pObj->AEWBValue2 = (ti2a_output_params.sensorGain / 10);
		pObj->AEWBValue2 *= (ti2a_output_params.ipipe_awb_gain.dGain *40/1024); //20 bit

        // AWB write to ISP
        status = Iss_captControl(pObj->cameraVipHandle, IOCTL_ISS_ALG_2A_UPDATE,
                            &ti2a_output_params, NULL);
        if (status != FVID2_SOK)
        {
            Vps_rprintf("AWB write to ISP failed!\n");
            return status;
        }
    }
    else
    {
        Vps_rprintf("2A ALG run failed!\n");
    }

    /* Added for DCC Parser */

    if (pObj->aewbVendor == AEWB_ID_TI)
    {
        if (((dccFrmCnt % 32) == 0) && (pObj->dcc_init_done == TRUE))
        {
            dcc_parser_input_params_t input_params;
			dcc_parser_output_params_t output_params;

            extern iss_drv_config_t *iss_drv_config;
            static int first_time = 0;
            if(first_time == 0)
            {
              extern volatile int new_dcc_data_available;
              new_dcc_data_available = 1;

              iss_drv_config->isif_cfg.lsc_params = (isif_2dlsc_cfg_t*)gIss_captCommonObj.pIssConfig->ptLsc2D;
              first_time = 1;
            }

			output_params.iss_drv_config   = iss_drv_config;
            input_params.dcc_buf           = (Int8 *)pObj->dcc_Default_Param;
            input_params.dcc_buf_size      = pObj->dccSize;
            input_params.color_temparature = ti2a_output_params.colorTemparaure;
            input_params.exposure_time     = ti2a_output_params.sensorExposure;
            input_params.analog_gain       = (ti2a_output_params.sensorGain / 1000);

            output_params.pVnfParam   = NULL;
            output_params.pGlbceParams   = NULL;

            dcc_update(&input_params, &output_params);

            output_params.iss_drv_config->ipipe_cfg.rgb_yuv_params->offset[0] =
                gTi2aControlParams.brightness - 128;
            output_params.iss_drv_config->ipipe_cfg.rgb_yuv_params->contrast =
                (gTi2aControlParams.contrast >> 3);
            output_params.iss_drv_config->ipipe_cfg.filter_flag |=
                (IPIPE_RGB_TO_YUV_FLAG);

            if( gTi2aControlParams.sharpness > 0 || gTi2aControlParams.sharpness < 256)
            {
                output_params.iss_drv_config->ipipe_cfg.ee_param->enable = 1;
                output_params.iss_drv_config->ipipe_cfg.ee_param->halo_reduction = IPIPE_HALO_REDUCTION_ENABLE;
                output_params.iss_drv_config->ipipe_cfg.ee_param->sel = 1;

                output_params.iss_drv_config->ipipe_cfg.ee_param->gain = gTi2aControlParams.sharpness/2 ;
                output_params.iss_drv_config->ipipe_cfg.ee_param->hpf_low_thr = 100 ;
                output_params.iss_drv_config->ipipe_cfg.ee_param->hpf_high_thr = 30 ;
                output_params.iss_drv_config->ipipe_cfg.ee_param->hpf_gradient_gain =
                         output_params.iss_drv_config->ipipe_cfg.ee_param->gain >> 1 +
                                output_params.iss_drv_config->ipipe_cfg.ee_param->gain ;
                output_params.iss_drv_config->ipipe_cfg.ee_param->hpf_gradient_offset = 24;
                output_params.iss_drv_config->ipipe_cfg.filter_flag |= (IPIPE_EE_FLAG);
            } else {
                output_params.iss_drv_config->ipipe_cfg.ee_param->enable = 0;
                output_params.iss_drv_config->ipipe_cfg.filter_flag |= (IPIPE_EE_FLAG);
            }

            if(gTi2aControlParams.saturation != 128)
            {
              int K = ((gTi2aControlParams.saturation * 1024) / 128);

              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[0] +=
              		((717 * (K - 1024)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[1] +=
              		((601 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[2] +=
              		((117 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[3] +=
              		((601 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[4] +=
              		((717 * (K - 1024)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[5] +=
              		((117 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[6] +=
                	((306 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[7] +=
             		((311 * (1024 - K)) * 256 )/(1024 * 1024);
              output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[8] +=
              		((717 * (K - 1024)) * 256 )/(1024 * 1024);

              output_params.iss_drv_config->ipipe_cfg.filter_flag |=
                    (IPIPE_RGB_RGB_1_FLAG);
            }

            ipipe_config_filters(&output_params.iss_drv_config->ipipe_cfg);
            output_params.iss_drv_config->ipipe_cfg.filter_flag = 0;
            isif_config(&output_params.iss_drv_config->isif_cfg);
            output_params.iss_drv_config->isif_cfg.feature_flag = 0;
        }
    }

    dccFrmCnt++;

    return FVID2_SOK;
}

/* ===================================================================
 *  @func     copy4x32bit
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
static inline void copy4x32bit(Uint32 * pDest, Uint32 * pSource)
{
    *pDest++ = *pSource++;
    *pDest++ = *pSource++;
    *pDest++ = *pSource++;
    *pDest++ = *pSource++;
}

/***************************************************************************************/

/* ===================================================================
 *  @func     IssAlg_capt2AProcessHive
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
Int32 IssAlg_capt2AProcessUd(Iss_2AObj *pObj)
{
	//# common variable
	Int32 status = FVID2_SOK;
	Int32 runWb = 0;

	runWb = UD_IssAlg_capt2AEstimationExposure(pObj);
	UD_IssAlg_capt2AEstimationWhiteBalance(pObj);

	return status;
}

Int32 IssAlg_capt2AProcessHive(Iss_2AObj *pObj)
{
	Int32 status = FVID2_SOK;

	#ifdef IMGS_SONY_IMX122
    status = hv2A_runAlg((unsigned short*)pObj->fullH3ABufAddr, pObj->aewbMode);
    #endif
    #ifdef IMGS_APTINA_AR0330
    status = hv2A_runAlg(pObj);
	#endif
//	Vps_rprintf("hv 2A Algorithm running============================================.\n");

    if(status != FVID2_SOK)
    {
        Vps_rprintf("hv 2A Algorithm running Error.\n");
        return status;
    }

    hv2A_updateParams();

    if (status == FVID2_SOK)
    {
        status = FVID2_control(pObj->SensorHandle,
                   IOCTL_ISS_SENSOR_UPDATE_EXP_GAIN,
                         &ti2a_output_params,NULL);

        if (status != FVID2_SOK)
        {
            Vps_rprintf("\nAE write to sensor failed!\n");
            return FVID2_EFAIL;
        }

        /* AWB control, Write to ISP internal register */
        status = Iss_captControl(pObj->cameraVipHandle,IOCTL_ISS_ALG_2A_UPDATE,
                            &ti2a_output_params,NULL);

        if (status != FVID2_SOK)
        {
            Vps_rprintf("AWB write to ISP failed!\n");
            return FVID2_EFAIL;
        }

        /**
              Update dynamic Parameter
           */
        if(0)
        {

            dcc_parser_output_params_t output_params;

            extern iss_drv_config_t *iss_drv_config;

            output_params.iss_drv_config   = iss_drv_config;

            output_params.iss_drv_config->ipipe_cfg.rgb_yuv_params->offset[0]   = gTi2aControlParams.brightness - 128;

            output_params.iss_drv_config->ipipe_cfg.rgb_yuv_params->contrast    = (gTi2aControlParams.contrast >> 3);

            output_params.iss_drv_config->ipipe_cfg.filter_flag |= (IPIPE_RGB_TO_YUV_FLAG);


            if(gTi2aControlParams.saturation != 128)
            {

                int K = ((gTi2aControlParams.saturation * 1024) / 128);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[0] += ((717 * (K - 1024)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[1] += ((601 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[2] += ((117 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[3] += ((601 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[4] += ((717 * (K - 1024)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[5] += ((117 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[6] += ((306 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[7] += ((311 * (1024 - K)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.rgb_rgb1_params->mul_off[8] += ((717 * (K - 1024)) * 256 )/(1024 * 1024);

                output_params.iss_drv_config->ipipe_cfg.filter_flag |= (IPIPE_RGB_RGB_1_FLAG);

            }

            ipipe_config_filters(&output_params.iss_drv_config->ipipe_cfg);

            output_params.iss_drv_config->ipipe_cfg.filter_flag = 0;

            //isif_config(&output_params.iss_drv_config->isif_cfg);

            //output_params.iss_drv_config->isif_cfg.feature_flag = 0;

        }

    }
    else
    {
        Vps_rprintf("2A ALG run failed!\n");
    }

    return status;

}

/* 2A process function */
/* ===================================================================
 *  @func     IssAlg_capt2AProcess
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
Int32 IssAlg_capt2AProcess(Iss_2AObj *pObj)

{
#ifdef TWOA_PROFILE
    Uint32 start, end;
	start = Utils_getCurTimeInMsec();
#endif

	if(pObj->aewbVendor == AEWB_ID_TI)
	{
	    IssAlg_capt2AProcessTI(pObj);
	}
    else if(pObj->aewbVendor == AEWB_ID_UD)
    {
    	IssAlg_capt2AProcessUd(pObj);
    }

#ifdef TWOA_PROFILE
    end = Utils_getCurTimeInMsec();
	Vps_printf("2A:Process time = %d msec\n",(end - start));
#endif

    return FVID2_SOK;
}

/* 2A task Main function */

/* ===================================================================
 *  @func     IssAlg_captTsk2A
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
Void IssAlg_captTsk2A(UArg arg0,UArg arg1)

{

    Iss_2AObj *pObj = (Iss_2AObj*)arg0;

	static int count = 0;


    /* allocate H3A temp memory */

    pObj->pH3AMem = memalign(64,20 * pObj->aewbNumWinV * pObj->aewbNumWinH);



    /* Create AEWB algorithm instance */

	if(pObj->aewbVendor != AEWB_ID_UD)
	{
    	pObj->pAlgHndl = (Void*)ALG_aewbCreate(pObj->aewbNumWinH, pObj->aewbNumWinV,pObj->aewbNumPix);

    	if(pObj->pAlgHndl == NULL)

    	{

			Vps_rprintf ( " %s:%d: 2A alg instance create failed !!!\n", __FUNCTION__, __LINE__ );

			GT_assert(GT_DEFAULT_MASK,pObj->pAlgHndl != NULL);

		}
	}

#ifdef ISS_CAPT_DEBUG

	Vps_rprintf ( " %s:%d: 2A task Main function Entered !!!\n", __FUNCTION__,	__LINE__ );

#endif



    while(pObj->exitFlag == FALSE)

    {

        /* wait for 2A sem */

        Semaphore_pend(pObj->sem,BIOS_WAIT_FOREVER);



        /* Got new H3A data apply 2A algorithm */
		if ((pObj->exitFlag == FALSE) && (start_2A == 1)) { 	//# jaeki - temp. (for hang issue when exit)
        	IssAlg_capt2AProcess(pObj);
		}

		if(pObj->aewbVendor != AEWB_ID_UD)
		{
			count ++;
			if ((count % 60) == 0)
				pObj->AFValue = ALG_afRun((Void*)(pObj->fullH3ABufAddr+ pObj->h3aBufSize));
		}
	}



	/* Delete the 2A alg instance */

	if(pObj->aewbVendor != AEWB_ID_UD)
		ALG_aewbDelete(pObj->pAlgHndl);


	/* free H3A temp memory */

	free(pObj->pH3AMem);



#ifdef ISS_CAPT_DEBUG

	Vps_rprintf ( " %s:%d: 2A task Main function Exited !!!\n", __FUNCTION__,

					__LINE__ );

#endif

}



/* Set 2A default params */



/* ===================================================================
 *  @func     IssAlg_capt2ASetDefault
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
Int32 IssAlg_capt2ASetDefault(Iss_2AObj *pObj)

{

    pObj->exitFlag   = FALSE;

    pObj->AutoIris   = 0;

    pObj->saturation = 0;

    pObj->sharpness  = 0;

    pObj->brightness = 0;

    pObj->contrast   = 0;

    pObj->blc        = 0;

    pObj->AWBMode    = 0;

    pObj->AEMode     = 0;

    pObj->Env        = 0;

    pObj->Binning    = 0;

    pObj->FrameRate  = 0;



    pObj->dcc_Default_Param = NULL;

    pObj->dcc_init_done     = FALSE;

    pObj->dccSize           = 0;



    return FVID2_SOK;

}



/* 2A Init */



/* ===================================================================
 *  @func     IssAlg_capt2AInit
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
Int32 IssAlg_capt2AInit(Iss_2AObj *pObj)

{

	Task_Params tskParams;

	Semaphore_Params semParams;



	gpIssAlg2AObj = pObj;



	IssAlg_capt2ASetDefault(pObj);

	if (pObj->taskHndl==NULL)
	{

	/* Create 2A task */

	Task_Params_init(&tskParams);



	tskParams.priority  = ISSALG_CAPT_TSK_PRI_2A;

	tskParams.stack     = gIssAlg_captTskStack2A;

	tskParams.stackSize = sizeof(gIssAlg_captTskStack2A);

	tskParams.arg0      = (UArg)pObj;



	pObj->taskHndl = Task_create(IssAlg_captTsk2A,

                                 &tskParams,

                                 NULL);



    if(pObj->taskHndl == NULL)

    {

		Vps_rprintf ( " %s:%d: 2A task create failed !!!\n", __FUNCTION__,

						__LINE__ );

	    GT_assert(GT_DEFAULT_MASK,pObj->taskHndl != NULL);

	}



	/* create 2A sem */

	Semaphore_Params_init(&semParams);

	semParams.mode = Semaphore_Mode_BINARY;



	pObj->sem = Semaphore_create(0u,&semParams,NULL);



	if(pObj->sem == NULL)

	{

		Vps_rprintf ( " %s:%d: 2A task sem create failed !!!\n", __FUNCTION__,

						__LINE__ );

	    GT_assert(GT_DEFAULT_MASK,pObj->sem != NULL);

	}



	/* Allocate memory for the H3A buffers */

	pObj->h3aBufAddr[0] = (UInt32)memalign(64,pObj->h3aBufSize * 2); // AEWB + AF

	pObj->h3aBufAddr[1] = (UInt32)memalign(64,pObj->h3aBufSize * 2); // AEWB + AF

	}

	pObj->curH3aBufIdx  = 0;



	/* H3A module config */

	pObj->ispH3aCfg.ptIssConfig = pObj->pIssConfig;

    pObj->ispH3aCfg.pAewbBuff   = (MSP_PTR)pObj->h3aBufAddr[0];

    pObj->ispH3aCfg.pAfBuff     = (MSP_PTR)(pObj->h3aBufAddr[0] + pObj->h3aBufSize);



    MSP_ISP_config(pObj->pIspHandle,(MSP_INDEXTYPE)MSP_ISP_CFG_H3A,&pObj->ispH3aCfg);

    UD_IssAlg_capt2AInit(pObj);


#ifdef ISS_CAPT_DEBUG

	Vps_rprintf ( " %s:%d: 2A task Init Done !!!\n", __FUNCTION__,

					__LINE__ );

#endif



    return FVID2_SOK;

}



/* 2A De Init */



/* ===================================================================
 *  @func     IssAlg_capt2ADeInit
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
Int32 IssAlg_capt2ADeInit(Iss_2AObj *pObj)

{

	/* Set exit flag */

	pObj->exitFlag = TRUE;



	/* post the 2A semaphore so that 2A task comes out of sem wait */

	Semaphore_post(pObj->sem);



	/* wait for 2A task to exit the loop */

	if(pObj->taskHndl != NULL)
    while(Task_getMode(pObj->taskHndl) != Task_Mode_TERMINATED)
	{

	    /* wait for 100 msec */

	    Task_sleep(100);

	}



	/* Delete the 2A task */

	if(pObj->taskHndl != NULL)

	{

	    Task_delete(&pObj->taskHndl);

	}



    if(pObj->sem != NULL)

    {

	    Semaphore_delete(&pObj->sem);

	}



    /* Stop H3A module */
	if(ISP_SUCCESS != issDrvH3aStop()) {
		Vps_printf("issDrvH3aStop failed\n");
	}


    /* free H3A buffers */

    free((void*)pObj->h3aBufAddr[0]);

    free((void*)pObj->h3aBufAddr[1]);

    pObj->taskHndl = NULL;

#ifdef ISS_CAPT_DEBUG

	Vps_rprintf ( " %s:%d: 2A task DeInit Done !!!\n", __FUNCTION__,

					__LINE__ );

#endif



    return FVID2_SOK;

}

Int32 ALG_afRun(void *h3aDataVirtAddr)
{
	MSP_H3aAfOutVfDisableOverlay *pAfPaxData;
	unsigned short i,j;
	int focus_value;
	Uint8 *curAfAddr;
	Uint32 accAfVal[9] = {0,0,0,0,0,0,0,0,0};
	//float avgAfVal[9];
	int weighti, weightj, fweight;
	int af_pax_vt_cnt, af_pax_hz_cnt;

	af_pax_vt_cnt = 16;
	af_pax_hz_cnt = 6;

	curAfAddr = (Uint8* ) h3aDataVirtAddr;

	for(i=1;i<af_pax_vt_cnt-1; i++) {
		for(j=1;j<af_pax_hz_cnt-1; j++) {

			pAfPaxData = (MSP_H3aAfOutVfDisableOverlay *)curAfAddr;

			weighti = (i > (af_pax_vt_cnt-1 - i))?(af_pax_vt_cnt-1 - i):i;
			weightj = (j > (af_pax_hz_cnt-1 - j))?(af_pax_hz_cnt-1 - j):j;
			fweight = weighti*weightj;

			accAfVal[0] += fweight*pAfPaxData->hfvSum_0;
			accAfVal[1] += fweight*pAfPaxData->hfv1_0;
			accAfVal[2] += fweight*pAfPaxData->hfv2_0;
			accAfVal[3] += fweight*pAfPaxData->hfvSum_1;
			accAfVal[4] += fweight*pAfPaxData->hfv1_1;
			accAfVal[5] += fweight*pAfPaxData->hfv2_1;
			accAfVal[6] += fweight*pAfPaxData->hfvSum_2;
			accAfVal[7] += fweight*pAfPaxData->hfv1_2;
			accAfVal[8] += fweight*pAfPaxData->hfv2_2;

			curAfAddr += sizeof(MSP_H3aAfOutVfDisableOverlay);

		}
	}

/*   for(i=0;i<9;i++){
   	  avgAfVal[i] = (float)accAfVal[i]/(af_pax_vt_cnt*af_pax_hz_cnt);
      //if((i==4)||(i==5)) // 4 & 5 indices for green value in RGB space
      //	OSA_printf("accAfVal[%d]: %d \t avgAfVal[%d]: %f \n", i, accAfVal[i], i, avgAfVal[i]);
   }*/

   focus_value = accAfVal[4];

   return focus_value;
}

Int32 IssAlg_capt2Astart(void)
{
	start_2A = 1;
	return 0;
}

Int32 IssAlg_capt2Astop(void)
{
	start_2A = 0;
	return 0;
}
