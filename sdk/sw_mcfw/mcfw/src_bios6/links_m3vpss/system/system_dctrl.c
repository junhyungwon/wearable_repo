/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "system_priv_m3vpss.h"
#include <mcfw/interfaces/common_def/ti_vsys_common_def.h>
#include <mcfw/interfaces/common_def/ti_vdis_common_def.h>

/* Display Controller Configuration */
Vps_DcConfig gSystem_dctrl_display_config = {
	VPS_DC_USERSETTINGS,	/* Use Case */
	/* Edge information */
	{
		{VPS_DC_BP0_INPUT_PATH, VPS_DC_VCOMP_MUX},
		{VPS_DC_VCOMP_MUX, VPS_DC_VCOMP},
		{VPS_DC_CIG_NON_CONSTRAINED_OUTPUT, VPS_DC_HDMI_BLEND},

		{VPS_DC_AUX_INPUT_PATH, VPS_DC_HDCOMP_MUX},
		{VPS_DC_HDCOMP_MUX, VPS_DC_CIG_PIP_INPUT},
		{VPS_DC_CIG_PIP_OUTPUT, VPS_DC_DVO2_BLEND},

		{VPS_DC_BP1_INPUT_PATH, VPS_DC_SDVENC_MUX},
		{VPS_DC_SDVENC_MUX, VPS_DC_SDVENC_BLEND},

		{VPS_DC_GRPX0_INPUT_PATH, VPS_DC_HDMI_BLEND},
		{VPS_DC_GRPX1_INPUT_PATH, VPS_DC_DVO2_BLEND},
		{VPS_DC_GRPX2_INPUT_PATH, VPS_DC_SDVENC_BLEND}
	},
	11u,		//# numEdges
	/* VENC information */
	{
	 	/* Mode information - no effect */
		{
			{VPS_DC_VENC_HDMI, {FVID2_STD_1080P_60}},
			{VPS_DC_VENC_DVO2, {FVID2_STD_WVGA_60}},
			{VPS_DC_VENC_SD,   {FVID2_STD_NTSC}}
		},
		0,		/* Tied VENC bit mask */
		3u		/* Number of VENCs */
	}
};

Int32 System_getClk(UInt32 displayRes)
{
    Int32 clkValue = VSYS_STD_MAX;
    switch(displayRes) {
        case VSYS_STD_1080P_30:
        case VSYS_STD_1080I_60:
            clkValue = 74250u;
        break;
        case VSYS_STD_720P_60:
            clkValue = 74250;
        break;
        case VSYS_STD_1080P_60:
            clkValue = 148500u;
        break;
        case VSYS_STD_XGA_60:
            clkValue = 65000u;
        break;
        case VSYS_STD_SXGA_60:
            clkValue = 108000u;
        break;
        case VSYS_STD_WVGA_60:	//# for lcd
            clkValue = 28000u;
        break;
        default:
            UTILS_assert(0);
        break;
    }
    return(clkValue);
}

/**
 *  System_dispSetPixClk
 *  Configure Pixel Clock.
 */
Int32 System_dispSetPixClk()
{
    Int32 retVal = FVID2_SOK;

    gSystem_objVpss.systemDrvHandle = FVID2_create(FVID2_VPS_VID_SYSTEM_DRV,
                                                   0, NULL, NULL, NULL);
    if (NULL == gSystem_objVpss.systemDrvHandle)
    {
        Vps_printf("%s: Error %d @ line %d\n", __FUNCTION__, retVal, __LINE__);
        return (FVID2_EFAIL);
    }

    gSystem_objVpss.vpllCfg[SYSTEM_VPLL_OUTPUT_VENC_D].outputVenc = VPS_SYSTEM_VPLL_OUTPUT_VENC_D;
    retVal = FVID2_control(gSystem_objVpss.systemDrvHandle,
                           IOCTL_VPS_VID_SYSTEM_SET_VIDEO_PLL,
                           &(gSystem_objVpss.vpllCfg[SYSTEM_VPLL_OUTPUT_VENC_D]), NULL);
    if (FVID2_SOK != retVal)
    {
        Vps_printf("%s: Error %d @ line %d\n", __FUNCTION__, retVal, __LINE__);
        return (retVal);
    }

    FVID2_delete(gSystem_objVpss.systemDrvHandle, NULL);

    return (FVID2_SOK);
}

Int32 System_getVencMode(Int32 resolution)
{
    Int32 vencMode;

    switch(resolution){
        case VSYS_STD_1080P_30:
            vencMode = FVID2_STD_1080P_30;
        break;
        case VSYS_STD_1080I_60:
            vencMode = FVID2_STD_1080I_60;
        break;
        case VSYS_STD_1080P_60:
            vencMode = FVID2_STD_1080P_60;
        break;
        case VSYS_STD_720P_60:
            vencMode = FVID2_STD_720P_60;
        break;
        case VSYS_STD_XGA_60:
            vencMode = FVID2_STD_XGA_60;
        break;
        case VSYS_STD_SXGA_60:
            vencMode = FVID2_STD_SXGA_60;
        break;
        case VSYS_STD_WVGA_60:
            vencMode = FVID2_STD_WVGA_60;
        break;
        case VSYS_STD_NTSC:
            vencMode = FVID2_STD_NTSC;
        break;
        case VSYS_STD_PAL:
            vencMode = FVID2_STD_PAL;
        break;
        default:
            vencMode = FVID2_STD_1080P_60;

		Vps_printf(" %d: SYSTEM: WARNING: Unsupported VENC mode [%d]. Defaulting to 1080p60\n",
                 Utils_getCurTimeInMsec(), resolution);
    }

    return(vencMode);
}

Int32 System_displayCtrlInit(VDIS_PARAMS_S * pPrm)
{
	Int32                   driverRetVal, retVal;
	Vps_DcCreateConfig      dcCreateCfg;
	Vps_CscConfig           dcVcompCscConfig;
	Vps_CscConfig           dcHdcompCscConfig;
	Vps_CscConfig           dcSdCscConfig;
	Vps_DcEdeConfig         dcEdeCfg;
	Vps_DcVencClkSrc        clkSrc;
	Vps_DcConfig            *dctrlTriDisplayConfig = NULL;

	System_displayUnderflowCheck(TRUE);

	dctrlTriDisplayConfig = &(gSystem_dctrl_display_config);
	dctrlTriDisplayConfig->vencInfo.tiedVencs = pPrm->tiedDevicesMask;

	memcpy(&gSystem_objVpss.displayCtrlCfg, dctrlTriDisplayConfig, sizeof(Vps_DcConfig));

	gSystem_objVpss.enableConfigExtVideoEncoder = pPrm->enableConfigExtVideoEncoder;

	gSystem_objVpss.displayCtrlCfg.vencInfo.modeInfo[0].mInfo.standard =
	          System_getVencMode(pPrm->deviceParams[SYSTEM_DC_VENC_HDMI].resolution);
	gSystem_objVpss.displayCtrlCfg.vencInfo.modeInfo[1].mInfo.standard =
	          System_getVencMode(pPrm->deviceParams[SYSTEM_DC_VENC_DVO2].resolution);
	gSystem_objVpss.displayCtrlCfg.vencInfo.modeInfo[2].mInfo.standard =
	          System_getVencMode(pPrm->deviceParams[SYSTEM_DC_VENC_SD].resolution);

	/* Clock VENC_D (DVO1) */
	gSystem_objVpss.vpllCfg[SYSTEM_VPLL_OUTPUT_VENC_D].outputClk =
				System_getClk(pPrm->deviceParams[SYSTEM_DC_VENC_HDMI].resolution);

	/* Configure pixel clock */
	retVal = System_dispSetPixClk();
	UTILS_assert(FVID2_SOK == retVal);

	dcVcompCscConfig.bypass  =
	dcHdcompCscConfig.bypass =
	dcSdCscConfig.bypass     = FALSE;
	dcVcompCscConfig.coeff   =
	dcHdcompCscConfig.coeff  =
	dcSdCscConfig.coeff      = NULL;

	dcVcompCscConfig.mode  =
	dcHdcompCscConfig.mode =
	dcSdCscConfig.mode     = pPrm->deviceParams[SYSTEM_DC_VENC_HDMI].colorSpaceMode; //VPS_CSC_MODE_HDTV_GRAPHICS_Y2R;

	memset(&dcCreateCfg, 0, sizeof(dcCreateCfg));

	if (pPrm->enableEdgeEnhancement) {
	    dcEdeCfg.ltiEnable = TRUE;
	    dcEdeCfg.horzPeaking = TRUE;
	    dcEdeCfg.ctiEnable = TRUE;
	    dcEdeCfg.transAdjustEnable = TRUE;
	    dcEdeCfg.lumaPeaking = TRUE;
	    dcEdeCfg.chromaPeaking = TRUE;
	    dcEdeCfg.minClipLuma = 0;
	    dcEdeCfg.maxClipLuma = 1023;
	    dcEdeCfg.minClipChroma = 0;
	    dcEdeCfg.maxClipChroma = 1023;
	    dcEdeCfg.bypass = FALSE;

	    dcCreateCfg.edeConfig       = &dcEdeCfg;
	}

	dcCreateCfg.vcompCscConfig  = &dcVcompCscConfig;
	dcCreateCfg.hdcompCscConfig = &dcHdcompCscConfig;
	dcCreateCfg.sdCscConfig     = &dcSdCscConfig;

	/* Open and configure display controller */
	gSystem_objVpss.fvidDisplayCtrl = FVID2_create(
	                  FVID2_VPS_DCTRL_DRV,
	                  VPS_DCTRL_INST_0,
	                  &dcCreateCfg,
	                  &driverRetVal,
	                  NULL);
	UTILS_assert(NULL != gSystem_objVpss.fvidDisplayCtrl);

	/* Set output in display controller */
	if (pPrm->deviceParams[SYSTEM_DC_VENC_DVO2].enable) {
	    retVal = FVID2_control(
	            gSystem_objVpss.fvidDisplayCtrl,
	            IOCTL_VPS_DCTRL_SET_VENC_OUTPUT,
	            &pPrm->deviceParams[SYSTEM_DC_VENC_DVO2].outputInfo,
	            NULL);
	    UTILS_assert(retVal == FVID2_SOK);
	}

	if (pPrm->deviceParams[SYSTEM_DC_VENC_HDMI].enable) {
	    retVal = FVID2_control(
	                gSystem_objVpss.fvidDisplayCtrl,
	                IOCTL_VPS_DCTRL_SET_VENC_OUTPUT,
	                &pPrm->deviceParams[SYSTEM_DC_VENC_HDMI].outputInfo,
	                NULL);
	    UTILS_assert(retVal == FVID2_SOK);
	}

	if (pPrm->deviceParams[SYSTEM_DC_VENC_SD].enable) {
	    retVal = FVID2_control(
	                gSystem_objVpss.fvidDisplayCtrl,
	                IOCTL_VPS_DCTRL_SET_VENC_OUTPUT,
	                &pPrm->deviceParams[SYSTEM_DC_VENC_SD].outputInfo,
	                NULL);
	    UTILS_assert(retVal == FVID2_SOK);
	}

	if(!(dctrlTriDisplayConfig->vencInfo.tiedVencs))
	{
	    /* Set the Clock source */
	    clkSrc.venc = VPS_DC_VENC_DVO2;
	    clkSrc.clkSrc = VPS_DC_CLKSRC_VENCA;
	    retVal = FVID2_control(
	                 gSystem_objVpss.fvidDisplayCtrl,
	                 IOCTL_VPS_DCTRL_SET_VENC_CLK_SRC,
	                 &clkSrc,
	                 NULL);
	    UTILS_assert(retVal == FVID2_SOK);

	    /* Set the Clock source for on-chip HDMI */
        clkSrc.venc = VPS_DC_VENC_HDMI;
        clkSrc.clkSrc = VPS_DC_CLKSRC_VENCD;
        retVal = FVID2_control(
                     gSystem_objVpss.fvidDisplayCtrl,
                     IOCTL_VPS_DCTRL_SET_VENC_CLK_SRC,
                     &clkSrc,
                     NULL);
        UTILS_assert(retVal == FVID2_SOK);
	}

	retVal = FVID2_control(
	             gSystem_objVpss.fvidDisplayCtrl,
	             IOCTL_VPS_DCTRL_SET_CONFIG,
	             &gSystem_objVpss.displayCtrlCfg,
	             NULL);
	UTILS_assert(retVal == FVID2_SOK);

	return retVal;
}

Int32 System_displayCtrlDeInit(VDIS_PARAMS_S * pPrm)
{
    Int32 retVal;

    /* Remove and close display controller configuration */
    retVal = FVID2_control(gSystem_objVpss.fvidDisplayCtrl,
                           IOCTL_VPS_DCTRL_CLEAR_CONFIG,
                           &gSystem_objVpss.displayCtrlCfg, NULL);
    UTILS_assert(retVal == FVID2_SOK);

    retVal = FVID2_delete(gSystem_objVpss.fvidDisplayCtrl, NULL);
    UTILS_assert(retVal == FVID2_SOK);

    System_displayUnderflowPrint(FALSE, TRUE);

    return retVal;
}
