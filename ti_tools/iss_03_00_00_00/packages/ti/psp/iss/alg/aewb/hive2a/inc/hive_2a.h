/** ==================================================================
 *  @file  hive_2a.h
 *
 *  @path
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Hive Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/
/*
 *  Copyright 2012 by Hive Incorporated.
 *
 *  All rights reserved. Property of Hive Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== hive_2a.h ========
 */
#ifndef _HIVE_2A_H_
#define _HIVE_2A_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef	unsigned char           BYTE;
typedef	unsigned short          WORD;
typedef	unsigned int            UINT;
typedef	unsigned long long      DWORD;

typedef long long int           s64;
typedef long long unsigned int  u64;

#define MSG_PRINT				Vps_printf

#define HIVE2A_SOK				0
#define HIVE2A_ERR				-1

#define ABS(x)                  ( (x) >= 0  ? (x) : -(x) )
#define MIN(x,y)                ( (x) < (y) ? (x) :  (y) )
#define MAX(x,y)                ( (x) > (y) ? (x) :  (y) )

/* Auto Exposure related definition */
#define AE_MGAMMA_USE
#define AE_DEBLUR_THRS          550		// shutter threshold 	// 678, 610, 550, 482 -> 1/60s, 1/120s, 1/240s, 1/500s
#define AE_DEBLUR_GAIN          100		// shutter gradient		// 64 ~ 256 		  -> 0.5 ~ 2.0

#ifdef IMGS_SONY_IMX122
	#define AE1_HSP				0x0
	#define AE1_HEP				0x3b8
	#define AE1_VSP				0x8
	#define AE1_VEP				0x434
	#define AE_VTW_60			0x465
	#define AE_VTW_50			0x546

	#define AE_SHT_SLICE		0									// Shutter limit (short dir)
	#define AE_GAIN_UNIT_MAX	0x8c								// DGC step Unit
	#define AE_GAIN_UNIT_EA		1									// DGC step Unit EA
	#define AE_GAIN_UNIT_OVR	0									// DGC Over step (under 256)
	#define AE_GAIN_UNIT_OVR2	38
//#define AE_GAIN_TOTAL		(448+64)							// Normalized total
#define AE_GAIN_TOTAL			448							// Normalized total
	#define AE_GAIN_TGT_OFST	0//0x80								// DGC min offset
	#define AE_GAIN_DENORMAL(val)	((val<=448) ? (int)((float)val*0.3125f) : 0x8c+(int)((float)(val-448)*0.59375f))	// denormalization

	#define AE_IRS_OPN_Max 		0x1E0								// IRIS-MPWM Maximum Duty(khkim)
	#define AE_IRS_CLS_Max 		0x300								// "		 Minimum(khkim)

#define	AE_TGT_GAIN			0x20	//0x10
	#define	AE_TGT_OFST			0x20
	#define	AE_CLIP_OFST		0x140

	#define AE_ERR_LMT			0x80
	#define AE_ERR_MGN			0x8

	#define AE_SPEED_GAIN		4 // 2
	#define AE_SPEED_BUNMO		40.f
#endif	//# IMGS_SONY_IMX122

#ifdef IMGS_MICRON_AR0330
	#define AE1_HSP                 0x0
	#define AE1_HEP                 0x3b8
	#define AE1_VSP                 0x8
	#define AE1_VEP                 0x434
	#define AE_VTW_60               1314    //1125//1108    /* Modified by Hennessy, 2012-04-24 */
	#define AE_VTW_50               1330
	#define AE_SHT_SLICE            0       //510 //(1/240)	        // Shutter limit (short dir)
	#define AE_GAIN_UNIT_MAX        32      //Gain Step                 // DGC step Unit
	#define AE_GAIN_UNIT_EA         15      // Means GainMax X6  // DGC step Unit EA
	//#define AE_GAIN_UNIT_OVR        0
	#define AE_GAIN_UNIT_OVR        38
	#define AE_GAIN_TOTAL           64+(AE_GAIN_UNIT_EA*AE_GAIN_UNIT_MAX)   // Normalized total (362agcdgc+64pgc)
	#define AE_GAIN_TGT_OFST        0x80                                    // DGC min offset
	#define AE_GAIN_DENORMAL(val)   ((val<=362) ? (int)((float)val*0.24862f) : 90+(int)((float)(val-362)*0.59375f)) // denormalization
	//	#define AE_GAIN_DENORMAL(val)/* Aptina*/	((val<=AE_GAIN_UNIT_EA*AE_GAIN_UNIT_MAX) ? (int)((float)val*1.0f) : AE_GAIN_UNIT_EA*AE_GAIN_UNIT_MAX+(int)((float)(val-AE_GAIN_UNIT_EA*AE_GAIN_UNIT_MAX)*0.59375f))	// denormalization
	#define AE_IRS_OPN_Max          0x1E0   // IRIS-MPWM Maximum Duty(khkim)
	#define AE_IRS_CLS_Max          0x300   // Minimum
	#define	AE_TGT_GAIN             0xd	     //0x10
	#define	AE_TGT_OFST             0x20
	#define	AE_CLIP_OFST            0x140
	#define AE_ERR_LMT              0x80
	#define AE_ERR_MGN              0x8 //0x10
	#define AE_SPEED_GAIN           4// 2       /* Modified by Hennessy, to speed up AE*/
	#define AE_SPEED_BUNMO          40.f
#endif	//# IMGS_MICRON_AR0330

#define AE_NUM_WIN_H            16
#define AE_NUM_WIN_V            32

#define AE_NUM_SUB_WIN_H        8
#define AE_NUM_SUB_WIN_V        8

enum
{
	AeLONG = 0,
	AeSHORT
};

enum
{
    hv_RED = 0,
    hv_GREEN,
    hv_BLUE
};

enum
{
    hv_AE_AREA_TOTAL = 0,
    hv_AE_AREA_CENTER,
    hv_AE_AREA_SUB_WIN,
    hv_AE_AREA_NO_CLIP,
    hv_AE_AREA_MODE_MAX
};


enum
{
    hv_DIR_UP = 0,
    hv_DIR_DOWN
};

/* Auto White Balance related definition */
#define TestMnl                 0
#define AWB_HSP                 0x1
#define AWB_HW                  0x3a8
#define AWB_VSP                 0xe
#define AWB_VW                  0x420
#define AWB_DB_MAX              (  78 )
#define AWB_DB_MIN              ( -570)
#define AWB_DR_MAX              (  483)
#define AWB_DR_MIN              ( -346)
#define AWB_DBOFFSET            0x6a
#define AWB_DROFFSET            0xe8
#define AWB_DBGAIN              0xe
#define AWB_DRGAIN              0xc
#define AWB_AGC_MAX             0x1e0
#define AWB_AGC_MIN             0x120
#define AWB_AGC_WGT_MAX         0x100
#define AWB_AGC_WGT_MIN         0x40
#define AWB_YSLICE              0x20
#define AWB_YCLIP               0x200
#define AWB_TCNT_LMT            0x1000
#define AWB_MTRX_IIR            0xf8
#define AWB_GAIN_IIR            0xf8

/* Changed for TI ISP */
#define AWB_GAIN_DFLT           0x80
#define AWB_GAIN_MIN            AWB_GAIN_DFLT
#define AWB_GAIN_MAX            AWB_GAIN_DFLT*4
#define AWB_GAIN_STEP_H         ((float)(AWB_GAIN_MAX-AWB_GAIN_DFLT)/10.f)
#define AWB_GAIN_STEP_L         ((float)(AWB_GAIN_DFLT-AWB_GAIN_MIN)/10.f)

/* AWB Slope Following related Variable */
#ifdef IMGS_SONY_IMX122
#define	GGAIN_DEFAULT           128
#define	RGAIN_5100              207//180//204//201//199//191//
#define	BGAIN_5100              260//260//258//250//245//249//215//
#define	RGAIN_3100              152//156//149//139//153//139//150//
#define	BGAIN_3100              382//378//380//355//361//350//310//
#define RGAIN_2000              128//133//119//130//118//127
#define BGAIN_2000              498//490//475//465//449//462//410
#define RGAIN_8000              307//269//258//262//308
#define BGAIN_8000              197//212//211//205//136
#define GAIN_MARGIN             100//30

#define ALPHA_PLUS_OFFSET       50
#define ALPHA_MINUS_OFFSET      50

#define BoJung	2
#endif	//# IMGS_SONY_IMX122

#ifdef IMGS_MICRON_AR0330
#define	GGAIN_DEFAULT           128
#define	RGAIN_5100              191//178
#define	BGAIN_5100              215//222
#define	RGAIN_3100              150//142
#define	BGAIN_3100              310//320
#define RGAIN_2000              127
#define BGAIN_2000              410
#define RGAIN_8000              308
#define BGAIN_8000              136
#define GAIN_MARGIN             30

#define ALPHA_PLUS_OFFSET       50
#define ALPHA_MINUS_OFFSET      50

#define BoJung	2
#endif	//# IMGS_MICRON_AR0330


#define	Slope                    (RGAIN_5100-RGAIN_3100)/(BGAIN_3100-BGAIN_5100)
#define	Slope1                   (RGAIN_8000-RGAIN_5100)/(BGAIN_5100-BGAIN_8000)
#define	Slope2                   (RGAIN_3100-RGAIN_2000)/(BGAIN_2000-BGAIN_3100)

#define	rvsSlope                 (BGAIN_3100-BGAIN_5100)/(RGAIN_5100-RGAIN_3100)
#define	rvsSlope1                (BGAIN_5100-BGAIN_8000)/(RGAIN_8000-RGAIN_5100)
#define	rvsSlope2                (BGAIN_2000-BGAIN_3100)/(RGAIN_3100-RGAIN_2000)

#define	pseudoSlope              (BGAIN_5100*(RGAIN_5100-RGAIN_3100))/(BGAIN_3100-BGAIN_5100)
#define	pseudoSlope1             (BGAIN_5100*(RGAIN_8000-RGAIN_5100))/(BGAIN_5100-BGAIN_8000)
#define	pseudoSlope2             (BGAIN_3100*(RGAIN_3100-RGAIN_2000))/(BGAIN_2000-BGAIN_3100)


#define	modifySlope(Val)         (Val*(BGAIN_3100-BGAIN_5100))/(RGAIN_5100-RGAIN_3100)
#define	modifySlope1(Val)        (Val*(BGAIN_5100-BGAIN_8000))/(RGAIN_8000-RGAIN_5100)
#define	modifySlope2(Val)        (Val*(BGAIN_2000-BGAIN_3100))/(RGAIN_3100-RGAIN_2000)


#define	Alpha                    RGAIN_5100+pseudoSlope// 8000 to 5100
#define	Alpha1                   RGAIN_5100+pseudoSlope1// 5100 to 3100
#define	Alpha2                   RGAIN_3100+pseudoSlope2// 3100 to 2000
#define	AlphaPlusOffSet          Alpha+ALPHA_PLUS_OFFSET
#define	AlphaMinusOffSet         Alpha-ALPHA_MINUS_OFFSET
#define	AlphaPlusOffSet1         Alpha1+ALPHA_PLUS_OFFSET
#define	AlphaMinusOffSet1        Alpha1-ALPHA_MINUS_OFFSET
#define	AlphaPlusOffSet2         Alpha2+ALPHA_PLUS_OFFSET
#define	AlphaMinusOffSet2        Alpha2-ALPHA_MINUS_OFFSET

#define	Function(rVal, bVal)    rVal+((RGAIN_5100-RGAIN_3100)*bVal)/(BGAIN_3100-BGAIN_5100)
#define	Function1(rVal, bVal)    rVal+((RGAIN_8000-RGAIN_5100)*bVal)/(BGAIN_5100-BGAIN_8000)
#define	Function2(rVal, bVal)    rVal+((RGAIN_3100-RGAIN_2000)*bVal)/(BGAIN_2000-BGAIN_3100)


typedef struct _AEWB_Rgb
{
    unsigned short r;
    unsigned short g;
    unsigned short b;
}AEWB_Rgb;

typedef struct _AEWB_DataEntry {
	unsigned short window_data[8][8];
	unsigned short unsat_block_ct[8];
}AEWB_DataEntry;

typedef struct {

    unsigned short  subSampleAcc[4];
    unsigned short  saturatorAcc[4];

}AEWB_OutSumModeOverlay;

/**
\brief  AE/AWB unsaturated block count
*/
typedef struct {

    unsigned short  unsatCount[8];

}AEWB_OutUnsatBlkCntOverlay;


typedef struct {
	short rgb_mul_rr;
	short rgb_mul_gr;
	short rgb_mul_br;
    short rgb_mul_rg;
    short rgb_mul_gg;
    short rgb_mul_bg;
	short rgb_mul_rb;
	short rgb_mul_gb;
	short rgb_mul_bb;
	short rgb_oft_or;
	short rgb_oft_og;
	short rgb_oft_ob;

} RGB2RGB_MATRIX;


typedef struct {
    int color_temp;
    RGB2RGB_MATRIX rgb2rgbparam;
    RGB2RGB_MATRIX rgb2rgb2param;
}RGB2RGB_AWB;


typedef struct {
    unsigned short aeArea;
    unsigned short clipCount;
    unsigned int maxGSum;
    unsigned int zeroCount;
    unsigned int minGSum;
    unsigned int sumTotal[3];
    unsigned int sumCenter[3];
    unsigned int sumSkyLine[3];
    unsigned int sumCenterLine[3];
    unsigned int sumWinSub[AE_NUM_SUB_WIN_H*AE_NUM_SUB_WIN_V][3];
    unsigned int subWin_y[AE_NUM_SUB_WIN_H*AE_NUM_SUB_WIN_V];
    unsigned short saturatedCnt[AE_NUM_WIN_H*AE_NUM_WIN_V];
    unsigned int saturatedCntPerSubWin[AE_NUM_SUB_WIN_H*AE_NUM_SUB_WIN_V];
    unsigned int totalUnsatCnt;
} AEWB_aeData;


typedef struct {
    unsigned short awbValidCnt;
    unsigned short rgbGain[10][5];
    unsigned short rgbGainSave[5];
    unsigned int awbFrameCnt;
    unsigned short rGainStep;
    unsigned short bGainStep;
    unsigned short rGainRemain;
    unsigned short bGainRemain;
    unsigned short rDirection;
    unsigned short bDirection;
    unsigned short rGainUpdate;
    unsigned short bGainUpdate;
} AEWB_awbData;

typedef struct {

  unsigned int exposureTimeInUsec;
  unsigned int apertureLevel;
  unsigned int sensorGain;
  unsigned int ipipeGain;

  unsigned int gainR;
  unsigned int gainGr;
  unsigned int gainGb;
  unsigned int gainB;

  unsigned int colorTemp;
  unsigned int update;

} AEWB_AewbStatus;

#define NUM_STEPS           8
#define NULL                0

#define OUT_DOOR_OFFSET     85
#define SHUTTER_MAX         1245
#define SHUTTER_MIN         1

#define SATURATION_VALUE    980
//#define SATURATION_ADJUST   1024-SATURATION_VALUE//84
#define SATURATION_ADJUST   -480 // 0//84+20

//#define HIVE_2A_DEBUG

#define hvAwbMode
#define hvAwbStep           5//10// 5

#define TRUE                1
#define FALSE               0

#define AEWB_R_ADJUST_STEP  20
#define AEWB_G_ADJUST_STEP  15
#define AEWB_B_ADJUST_STEP  30

#define AEWB_UPDATE_AE		0x0001
#define AEWB_UPDATE_AWB		0x0002
#define AEWB_UPDATE_DGAIN	0x0004
#define AEWB_UPDATE_RGB2RGB	0x0008
#define AEWB_UPDATE_INDOOR		0x80000000
#define AEWB_UPDATE_LOWLIGHT	0x40000000
#define AEWB_UPDATE_C_TEMP		0x20000000

#ifdef IMGS_SONY_IMX122
#define RGB2RGB_GAIN_THRESHOLD		7
#endif
#ifdef IMGS_MICRON_AR0330
#define RGB2RGB_GAIN_THRESHOLD		160
#endif

static void hv2A_initAe(void);
static WORD  hv2A_aeShutter(int aiVal);
static void  hv2A_aeAgc(int aiVal);
static void  hv2A_aeAlg(void);
static int  hv2A_interp1D (int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2);
static void  hv2A_awbCmapSet(BYTE abMapSel, UINT* apnMapTbl);
static void  hv2A_awbAutoLevel(void);
static void hv2A_initAwb(void);
static void  hv2A_awbAlg(void);
static void hv2A_getStatistics (unsigned short * BLOCK_DATA_ADDR, int shift);
static void hv2A_getY(unsigned short * BLOCK_DATA_ADDR, short *y, int shift);
static void hv2A_translateColorSpace(unsigned int *y, unsigned int *cb, unsigned int *cr);
static void hv2A_addWeightOnClippedPixel(unsigned short shift);

/* External interface functions */
int hv2A_initAlg(int winCtVert, int winCtHorz, unsigned short pixCtWin);
void hv2A_deInitAlg(void);
#ifdef IMGS_SONY_IMX122
int hv2A_runAlg(unsigned short *pBuf, unsigned int awbMode);
#endif
#ifdef IMGS_MICRON_AR0330
int hv2A_runAlg(Iss_2AObj * pObj);
#endif
int hv2A_setBrightness(int level);
int hv2A_setContrast(int level);
int hv2A_setSharpness(int level);
int hv2A_setAgc(unsigned char level);
int hv2A_setColor(unsigned char rLevel, unsigned char gLevel, unsigned char bLevel);
int hv2A_getAgc(void);
int hv2A_getShutter(void);
int hv2A_aeErr(void);

#ifdef __cplusplus
}
#endif

#endif

