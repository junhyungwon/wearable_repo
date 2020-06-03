/** ==================================================================
 *  @file   iss_platformTI814x.c
 *
 *  @path   /ti/psp/platforms/ti814x/src/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/**
 *  \file iss_platformTI814x.c
 *
 *  \brief Implements the TI814x platform specific functions.
 *
 */

/* ==========================================================================
 */
/* Include Files */
/* ==========================================================================
 */

#include <string.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/psp/vps/common/trace.h>
#include <ti/psp/iss/iss.h>
#include <ti/psp/platforms/iss_platform.h>
#include <ti/psp/cslr/soc_TI814x.h>
#include <ti/psp/cslr/cslr_TI814xprcm.h>
#include <ti/psp/cslr/cslr_TI814xpll.h>
#include <ti/psp/devices/iss_sensorDriver.h>
#include <ti/psp/platforms/ti814x/iss_platformTI814x.h>

/* ==========================================================================
 */
/* Macros & Typedefs */
/* ==========================================================================
 */

#define ENABLE_HDVPSS_CLK

/* Enable I2C control to configure ecn/dec */
#define ENABLE_I2C_CLK

#define ISS_VS_BOARD_IO_EXP_I2C_ADDR    (0x21u)

#define ISS_VC_BOARD_A1_IO_EXP_I2C_ADDR (0x27u)
#define ISS_VC_BOARD_A2_IO_EXP_I2C_ADDR (0x21u)

#define ISS_CA_BOARD_A1_IO_EXP_I2C_ADDR (0x21u)

#define ISS_PLATFORM_EVM_I2C_INST_ID    (ISS_DEVICE_I2C_INST_ID_2)

/** \brief PLL Control Module Base Address*/
#define ISS_CONTROL_MODULE_PLL_CTRL_BASE_ADDR   (CSL_TI814x_PLL_BASE)

/** \brief Control Module Device Configuration Base Address */
#define ISS_CTRL_MODULE_DEV_CFG_BASE_ADDR       (CSL_TI814x_CTRL_MODULE_BASE + \
                                                    0x0600u)

/* Default Values for DDR PLL configuration to get clock for I2C and CEC */
#define ISS_DDR_INT_FREQ2               (0x8u)
#define ISS_DDR_FRACT_FREQ2             (0xD99999u)
#define ISS_DDR_MDIV2                   (0x1Eu)
#define ISS_DDR_SYCCLK10_DIV            (0x0u)

/* gpio base addresses */
#define REG32                           *(volatile unsigned int*)

#define ISS_PRCM_CLKTRCTRL_NO_SLEEP     (0u)
#define ISS_PRCM_CLKTRCTRL_SW_SLEEP     (1u)
#define ISS_PRCM_CLKTRCTRL_SW_WKUP      (2u)
#define ISS_PRCM_CLKTRCTRL_HW_AUTO      (3u)

#define ISS_PRCM_MODULE_DISABLE         (0u)
#define ISS_PRCM_MODULE_ENABLE          (2u)

#define ISS_PRCM_MAX_REP_CNT            (100u)

/* ADPLLJ_CLKCRTL_Register Value Configurations ADPLLJ_CLKCRTL_Register SPEC
 * bug bit 19,bit29 -- CLKLDOEN,CLKDCOEN */
#define ADPLLJ_CLKCRTL_HS2              (0x00000801u)
/* HS2 Mode,TINTZ =1 --used by all PLL's except HDMI */
#define ADPLLJ_CLKCRTL_HS1              (0x00001001u)
/* HS1 Mode,TINTZ =1 --used only for HDMI */
#define ADPLLJ_CLKCRTL_CLKDCO           (0x200a0000u)
/* Enable CLKDCOEN,CLKLDOEN,CLKDCOPWDNZ -- used for HDMI,USB */
#define ISS_TI814X_KHz                  (1000u)
#define ISS_TI814X_MHz                  (ISS_TI814X_KHz * ISS_TI814X_KHz)
#define ISS_TI814X_EVM_OSC_FREQ         (20u * ISS_TI814X_MHz)


#ifndef BOARD_AP_IPNC
#define BOARD_AP_IPNC
#endif

#ifdef BOARD_TI_EVM
#undef BOARD_TI_EVM
#endif
/* ==========================================================================
 */
/* Structure Declarations */
/* ==========================================================================
 */

/* \brief structure to keep track of pll configurations for a video mode */
typedef struct {
    UInt32 __n;
    /**< Divider N for the PLL.*/
    UInt32 __m;
    /**< Multiplier M for the PLL.*/
    UInt32 __m2;
    /**< Divider M2 for the PLL.*/
    UInt32 clkCtrlValue;
    /**< For comparison based on the clkOut used */
} Iss_VideoPllCtrl;

/* Structure to track the versions of boards */
typedef struct {
    Iss_PlatformBoardRev vcCard;
    UInt32 vcIoExpAddr;
    Semaphore_Handle ioExpLock;
    Iss_PlatformBoardRev vsCard_notused;                   /* Not used as of
                                                            * now */
    Iss_PlatformBoardRev baseBoard_notused;                /* Not used as of
                                                            * now */
    Iss_PlatformBoardRev caCard;
    UInt32 caIoExpAddr;
} Iss_BoardVersion;

/* ==========================================================================
 */
/* Function Declarations */
/* ==========================================================================
 */

/* ===================================================================
 *  @func     Iss_platformTI814xSetPinMux
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
static Int32 Iss_platformTI814xSetPinMux(void);

#ifdef POWER_OPT_DSS_OFF
/* ===================================================================
 *  @func     Iss_platformTI814xSetIntMux
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
static Int32 Iss_platformTI814xSetIntMux(void);
#endif

/* ==========================================================================
 */
/* Global Variables */
/* ==========================================================================
 */

/**< Variable to track the version of daughter card, we have Aplha 1 and
     Alpha 2, designated as REV A and REV B */
static Iss_BoardVersion gTi814xBoardVer = { ISS_PLATFORM_BOARD_REV_UNKNOWN,
    ISS_VC_BOARD_A1_IO_EXP_I2C_ADDR,
    ISS_PLATFORM_BOARD_REV_UNKNOWN,
    ISS_PLATFORM_BOARD_REV_UNKNOWN
};

/* ==========================================================================
 */
/* Function Definitions */
/* ==========================================================================
 */

/* ===================================================================
 *  @func     Iss_platformTI814xInit
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
Int32 Iss_platformTI814xInit(Iss_PlatformInitParams * initParams)
{
    Int32 status = FVID2_SOK;
    Iss_PlatformBoardId boardId;

	boardId = Iss_platformGetBoardId();

   	Iss_platformTI814xSetPinMux();
    #ifdef POWER_OPT_DSS_OFF
	Iss_platformTI814xSetIntMux();
	#endif

#ifdef ENABLE_HDVPSS_CLK
    /* Initialize Pixel Clock */
    // status |= Iss_platformTI814xConfigHdIsssPll();
    // status |= Iss_platformTI814xEnableHdIsssClk();
#endif

#ifdef ENABLE_I2C_CLK
    // status |= Iss_platformTI814xEnableI2c();
#endif

    return (status);
}

#ifdef POWER_OPT_DSS_OFF

/* ===================================================================
 *  @func     Iss_platformTI814xSetIntMux
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
static Int32 Iss_platformTI814xSetIntMux(void)
{
    volatile unsigned int int_mux;

    /* I2C2 interrupt is routed through I2C1 interrupt through the crossbar.
     * For this, INT_MUX_[#int_number] register in the Chip Control Module
     * needs to be programmed. INT_MUX_[#int_number] registers start from
     * 0xF54 offset and one register is used to program 4 interrupt muxes (6
     * bits for each mux, 2 bits reserved). After reset INT_MUX_[#int_number]
     * defaults to 000000, which maps the interrupt from default mapping to
     * interrupt_[#int_number]. I2C_INT1 is mapped to interrupt line 19 and
     * INTMUX 16 to 19 --> 0x0f64. So read it first, modify the respective
     * bit field and write is back. */
    int_mux = REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0f64);
    /* I2CINT2 value = 4, INT_MUX_19_SHIFT = 24 */
    int_mux |= (4 << 24);
    REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0f64) = int_mux;

    return (FVID2_SOK);
}
#endif

/* ===================================================================
 *  @func     Iss_platformTI814xDeInit
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
Int32 Iss_platformTI814xDeInit(void)
{
    Int32 status = FVID2_SOK;

    return (status);
}

/* Init EVM related sub-systems like I2C instance */
/* ===================================================================
 *  @func     Iss_platformTI814xDeviceInit
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
Int32 Iss_platformTI814xDeviceInit(Iss_PlatformDeviceInitParams * initPrms)
{
    Int32 status = FVID2_SOK;

    Vps_DeviceInitParams deviceInitPrm;

#ifdef POWER_OPT_DSS_OFF
    UInt8 i2cCnt = 0;

    /* TI814x has 4 I2C instances. */
    UInt32 i2cRegs[ISS_DEVICE_I2C_INST_ID_MAX] = { CSL_TI814x_I2C0_BASE,
        CSL_TI814x_I2C1_BASE,
        CSL_TI814x_I2C2_BASE,
        CSL_TI814x_I2C3_BASE
    };
    UInt32 i2cInt[ISS_DEVICE_I2C_INST_ID_MAX] = { CSL_INTC_EVENTID_I2CINT0,
        CSL_INTC_EVENTID_I2CINT1,
        CSL_INTC_EVENTID_I2CINT2,
        CSL_INTC_EVENTID_I2CINT3
    };
    /*
     * External video device subsystem init
     */
    memset(&deviceInitPrm, 0, sizeof(deviceInitPrm));

    /*
     * Initialize I2C instances
     */
    for (i2cCnt = 0; i2cCnt < ISS_DEVICE_I2C_INST_ID_MAX; i2cCnt++)
    {
        deviceInitPrm.i2cRegs[i2cCnt] = (Ptr) (i2cRegs[i2cCnt]);
        deviceInitPrm.i2cIntNum[i2cCnt] = i2cInt[i2cCnt];
        deviceInitPrm.i2cClkKHz[i2cCnt] = VPS_DEVICE_I2C_INST_NOT_USED;
    }
    deviceInitPrm.isI2cInitReq = initPrms->isI2cInitReq;

    /* TI814x uses only I2C[2], so modify the sampling frequency */
    deviceInitPrm.i2cRegs[ISS_PLATFORM_EVM_I2C_INST_ID]
        = (Ptr) CSL_TI814x_I2C2_BASE;
    deviceInitPrm.i2cIntNum[ISS_PLATFORM_EVM_I2C_INST_ID]
        = CSL_INTC_EVENTID_I2CINT2;
    deviceInitPrm.i2cClkKHz[ISS_PLATFORM_EVM_I2C_INST_ID] = 400;
#endif

    status = Iss_deviceInit(&deviceInitPrm);

    return (status);
}

/* De-Init EVM related sub-systems */
/* ===================================================================
 *  @func     Iss_platformTI814xDeviceDeInit
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
Int32 Iss_platformTI814xDeviceDeInit(void)
{
    /*
     * Extern video device de-init
     */
    Iss_deviceDeInit();

    return (FVID2_SOK);
}

/* ===================================================================
 *  @func     Iss_platformTI814xGetI2cInstId
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
UInt32 Iss_platformTI814xGetI2cInstId(void)
{
    return (ISS_PLATFORM_EVM_I2C_INST_ID);
}

/* ===================================================================
 *  @func     Iss_platformTI814xGetSensorI2cAddr
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
UInt8 Iss_platformTI814xGetSensorI2cAddr(UInt32 vidDecId, UInt32 vipInstId)
{
    UInt8 devAddr = 0x0;

	Uint8 devAddrImx122[ISS_CAPT_INST_MAX] = { 0xFF };		/* IMX-122 use SPI interface so it has no meaning */
    GT_assert(GT_DEFAULT_MASK, vipInstId < ISS_CAPT_INST_MAX);
    switch (vidDecId)
    {
		case FVID2_ISS_SENSOR_IMX122_DRV:
			devAddr = devAddrImx122[vipInstId];

        default:
            GT_0trace(GT_DEFAULT_MASK, GT_ERR, "Invalid decoder ID\n");
            break;
    }

    return (devAddr);
}

#define CSL_TI814x_ISS_BASE 0x0                            // to be done
#define ISSHAL_VIP_INST_MAX 0x1                            // to be done
#define ISSHAL_ISS_CLKC_VIP0 0x1                           // to be done
#define ISSHAL_ISS_CLKC_VIP1 0x1                           // to be done

/* ===================================================================
 *  @func     Iss_platformTI814xSetPinMux
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
static Int32 Iss_platformTI814xSetPinMux(void)
{
	#ifdef ISS_DEV_ON
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A6C) = 0x50002;	//# CAM_D8
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A70) = 0x50002;	//# CAM_D9
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A74) = 0x50002;	//# CAM_D10
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A78) = 0x50002;	//# CAM_D11
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A8C) = 0x50002;	//# CAM_D7
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A90) = 0x50002;	//# CAM_D6
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A94) = 0x50002;	//# CAM_D5
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A98) = 0x50002;	//# CAM_D4
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0A9C) = 0x50002;	//# CAM_D3
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AA0) = 0x50002;	//# CAM_D1
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AA4) = 0x50002;	//# CAM_D1
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AA8) = 0x50002;	//# CAM_D0
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AAC) = 0x50002;	//# CAM_HS
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AB0) = 0x50002;	//# CAM_VS
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AB4) = 0x50002;	//# CAM_FLD
	REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0AB8) = 0x50002; 	//# CAM_PCLK
	#endif

	#if 0
    /* I2c2  configuration Function 6*/
    REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0924) = 0x60020;   /* i2c2_scl_mux0 */
    REG32(CSL_TI814x_CTRL_MODULE_BASE + 0x0928) = 0x60020;   /* i2c2_sda_mux0 */
    #endif

    return (FVID2_SOK);
}

/* ===================================================================
 *  @func     Iss_platformTI814xGetCpuRev
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
Iss_PlatformCpuRev Iss_platformTI814xGetCpuRev(void)
{
    UInt32 cpuId, cpuRev;

    Iss_PlatformCpuRev cpuRevEnum;

    /* Read CPU ID */
    cpuId = REG32(ISS_CTRL_MODULE_DEV_CFG_BASE_ADDR + 0x0000u);

    cpuRev = ((cpuId >> 28u) & 0x0Fu);

    switch (cpuRev)
    {
    	#ifdef PLATFORM_UBX
    	case 0x0u:
            cpuRevEnum = ISS_PLATFORM_CPU_REV_1_0;
            break;
        case 0x1u:
            cpuRevEnum = ISS_PLATFORM_CPU_REV_1_1;
            break;
    	#else
        case 0x0u:
            cpuRevEnum = ISS_PLATFORM_CPU_REV_1_0;
            break;
        case 0xCu:  /* Certain intial sample of PG 2.1 has C but the production
                       samples should read out 3 */
        case 0x3u:
            cpuRevEnum = ISS_PLATFORM_CPU_REV_2_1;
            break;
		#endif
        default:
            // cpuRevEnum = ISS_PLATFORM_CPU_REV_UNKNOWN;
            Vps_printf
                (" PLATFORM: UNKNOWN CPU detected, defaulting to ISS_PLATFORM_CPU_REV_2_1\n");
            cpuRevEnum = ISS_PLATFORM_CPU_REV_2_1;
            break;
    }

    return (cpuRevEnum);
}

/* ===================================================================
 *  @func     Iss_platformTI814xGetBaseBoardRev
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
Iss_PlatformBoardRev Iss_platformTI814xGetBaseBoardRev(void)
{
    /* Only one revision of board till date */
    return (ISS_PLATFORM_BOARD_REV_A);
}

/* ===================================================================
 *  @func     Iss_platformTI814xGetDcBoardRev
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
Iss_PlatformBoardRev Iss_platformTI814xGetDcBoardRev(void)
{
    /* Only one revision of board till date */
    return (ISS_PLATFORM_BOARD_REV_A);
}

/* ===================================================================
 *  @func     Iss_platformTI814xSelectVideoDecoder
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
/* ===================================================================
 *  @func     Iss_platformTI814xSelectSensor
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
Int32 Iss_platformTI814xSelectSensor(UInt32 vidDecId, UInt32 vipInstId)
{
    UInt32 i2cInstId = Iss_platformTI814xGetI2cInstId();

    UInt32 i2cDevAddr;

    UInt8 regValue[2];

    Iss_PlatformBoardId boardId;

    Int32 status = FVID2_SOK;

    boardId = Iss_platformGetBoardId();

    //# only one sensor in ubx
    if (boardId == ISS_PLATFORM_BOARD_UBX) {
        return status;
    }

    /* In TI814X, GMII1 and SiL9135A are muxed on the VC board. */
    if (boardId == ISS_PLATFORM_BOARD_VCAM)
    {
        i2cDevAddr = gTi814xBoardVer.vcIoExpAddr;
        regValue[0] = 0xEF;
        regValue[1] = 0xFF;
    }

    status = Iss_deviceRawWrite8(i2cInstId, i2cDevAddr, regValue, 2);
    return status;
}
