/*
********************************************************************************
* HDVICP2.0 Based JPEG Encoder
*
* "HDVICP2.0 Based JPEG Encoder" is software module developed for
* TI's HDVICP2 based SoCs. This module is capable of encoding a raw image
* by compressing it into a JPEG bitstream compliant with ISO/IEC IS 10918-1.
* Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
********************************************************************************
*/
/**
********************************************************************************
* @file      ijpegenc.h
*
* @brief     JPEG specific interface details
*
* @author    Pavan Kumar
*
* @version 0.0 (Dec 2009) : Created the initial version.
*
* @version 0.1 (Jan 2010):Modified for Functionality requirements[Pavan,Vasudev]
*
* @version 0.2 (Feb 2010) : Review & Cleanup [Vasudev]
*
* @version 0.3 (Aug 2010):Debug Trace & error Robustness support added[Vasudev]
*
* @version 0.4 (Dec 2010) : Added support for error recovery and error 
*                           robustness in case of data sync errors[Pavan]
*
* @version 0.5 (Nov 2011) : Added support for Minimum Quality factor 1 and  
*                           Maximum Quality factor 100        [Naidu]
*
* @version 0.6 (Nov 2011) : Create and Dynamic Default structure are exposed to 
*                           Application                       [Naidu]
********************************************************************************
*/

/* -------------------- compilation control switches -------------------------*/
#ifndef JPEGENC_TI_IJPEGVENC_H
#define JPEGENC_TI_IJPEGVENC_H

/**
* Macro for Minimum,maximum & Default Quality Factor
**/
#define IJPEGVENC_MIN_QUALITY_FACTOR      (1)
#define IJPEGVENC_MAX_QUALITY_FACTOR      (100)
#define IJPEGVENC_DEFAULT_QUALITY_FACTOR  (50)

/*******************************************************************************
*                             INCLUDE FILES
*******************************************************************************/
/* -------------------- system and platform files ----------------------------*/
#include <ti/xdais/ialg.h>
#include <ti/xdais/dm/ividenc2.h>
#include <ti/xdais/xdas.h>
/*--------------------- program files ----------------------------------------*/

/*******************************************************************************
*  PUBLIC DECLARATIONS Defined here, used elsewhere
*******************************************************************************/
/*---------------------- data declarations -----------------------------------*/
/**
********************************************************************************
*  @struct     IJPEGVENC_Obj
*
*  @brief       This structure must be the first field of all JPEGVENC
*               instance objects
*
*  @param  fxns : Pointer to Functions which performs all the operations on
*                 IJPEGVENC objects.
********************************************************************************
*/
typedef struct IJPEGVENC_Obj {
  struct IJPEGVENC_Fxns *fxns;
} IJPEGVENC_Obj;

/**
********************************************************************************
*  @struct     IJPEGVENC_Status
*
*  @brief       Status structure defines the parameters that can be changed or
*               read during real-time operation of the alogrithm.
*
*  @param videnc2Status : This structure holds all the status parameters for
*                         IVIDENC2 Objects
********************************************************************************
*/
typedef struct IJPEGVENC_Status {
  /*--------------------------------------------------------------------------*/
  /* Base Class                                                               */
  /*--------------------------------------------------------------------------*/
  IVIDENC2_Status videnc2Status;

  /*--------------------------------------------------------------------------*/
  /*Debug trace Level : Possible values are 0,1,2,3,4                         */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 debugTraceLevel;

  /*--------------------------------------------------------------------------*/
  /* Number of last frames to log the debug info                              */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 lastNFramesToLog;

  /*--------------------------------------------------------------------------*/
  /* base address of the trace buffer in external memory                      */
  /*--------------------------------------------------------------------------*/  
  XDAS_UInt32 * extMemoryDebugTraceAddr;
  
  /*--------------------------------------------------------------------------*/
  /* Size of the trace buffer                                                 */
  /*--------------------------------------------------------------------------*/    
  XDAS_UInt32 extMemoryDebugTraceSize;

  /*--------------------------------------------------------------------------*/
  /* Extended Error Code0 returned by encoder                                 */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 extendedErrorCode0;
  
  /*--------------------------------------------------------------------------*/
  /* Extended Error Code1 returned by encoder                                 */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 extendedErrorCode1;
  
  /*--------------------------------------------------------------------------*/
  /* Extended Error Code2 returned by encoder                                 */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 extendedErrorCode2;
  
  /*--------------------------------------------------------------------------*/
  /* Extended Error Code3 returned by encoder                                 */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32 extendedErrorCode3;  

} IJPEGVENC_Status;

/**
********************************************************************************
*  @struct     IJPEGVENC_Params
*
*  @brief       This structure defines the creation parameters for
*               all JPEGVENC objects
*
*  @param  videnc2Params :Create time Parameter structure
*
*  @param  maxThumbnailHSizeApp0:Max Horizontal resolution for APP0
*                                   thumbnail
*  @param  maxThumbnailVSizeApp0 :Max Vertical resolution for APP0
*                                    thumbnail
*  @param  maxThumbnailHSizeApp1:Max Horizontal resolution for APP1
*                                   thumbnail
*  @param  maxThumbnailVSizeApp1 :Max Vertical resolution for APP1
*                                    thumbnail
*  @param  chm_dc_vlc[12] :Chroma AC codes
********************************************************************************
*/
typedef struct IJPEGVENC_Params {
  /*--------------------------------------------------------------------------*/
  /* Base Class                                                               */
  /*--------------------------------------------------------------------------*/
  IVIDENC2_Params videnc2Params;
  
  /*--------------------------------------------------------------------------*/
  /* Max Horizontal resolution for APP0 thumbnail                             */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16     maxThumbnailHSizeApp0;

  /*--------------------------------------------------------------------------*/
  /* Max Vertical resolution for APP0 thumbnail                               */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16     maxThumbnailVSizeApp0;
  
  /*--------------------------------------------------------------------------*/
  /* Max Horizontal resolution for APP1 thumbnail                             */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16     maxThumbnailHSizeApp1;

  /*--------------------------------------------------------------------------*/
  /* Max Vertical resolution for APP0 thumbnail                               */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16     maxThumbnailVSizeApp1;

  /*--------------------------------------------------------------------------*/
  /*Debug trace Level                                                         */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32   debugTraceLevel;

  /*--------------------------------------------------------------------------*/
  /*History of last N frames                                                  */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt32   lastNFramesToLog;  

} IJPEGVENC_Params;

typedef IVIDENC2_Cmd IJPEGVENC_Cmd;

/**
********************************************************************************
*  @struct     IJPEGVENC_CustomQuantTables
*
*  @brief      JPEG Encoder Custom Quantization Tables - This structure defines
*              the custom quantization tables for both Luma & Chroma that need
*              to be passed to the JPEG Encoder. This will be used in the
*              Quantization Module.The format is as specified in RFC 2035
*              (RTP Payload Format for JPEG-compressed Video) and also in
*              Table K.1 & K.2 of JPEG Spec.
*
*              NOTE: The "IJPEGVENC_DynamicParams" structure defined below
*              contains a pointer to this structure of type
*              "IJPEGVENC_CustomQuantTables" -
*              "IJPEGVENC_CustomQuantTables *quantTable".
*              This field is used to pass custom Quantization tables to the
*              JPEG Encoder.The application just needs to initialize this field
*              appropriately.However, if the application wishes to use the
*              Standard Quantization Tables,then there are 2 ways of informing
*              this to the JPEG Encoder -
*             (a) by using the base class only "IJPEGVENC_DynamicParams" and not
*                 the extended class "IJPEGVENC_DynamicParams", in which case
*                 the size field in the DynamicParams structure should be set to
*                 the size of "IJPEGVENC_DynamicParams".
*             (b) by using the extended class "IJPEGVENC_DynamicParams", but by
*                 setting the field "IJPEGVENC_CustomQuantTables *quantTable" to
*                 NULL.
*
*  @param  lumQuantTab :This defines the quantization table for the luma
*                       component.
*  @param  chmQuantTab :This defines the quantization table for the chroma
*                       component.
********************************************************************************
*/
typedef struct IJPEGVENC_CustomQuantTables
{
  /*--------------------------------------------------------------------------*/
  /* The array "lumQuantTab" defines the quantization table for the luma      */
  /*  component.                                                              */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16 lumQuantTab[64];

  /*--------------------------------------------------------------------------*/
  /* The array "chmQuantTab" defines the quantization table for the chroma    */
  /* component.                                                               */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16 chmQuantTab[64];

} IJPEGVENC_CustomQuantTables;

/**
********************************************************************************
*  @struct     IJPEGVENC_DynamicParams
*
*  @brief       This structure defines the dynamic parameters for
*               all JPEGVENC objects
*
*  @param videnc2DynamicParams: IVIDENC2_Dynamic Params
*  @param restartInterval     : Restart Interval
*  @param qualityFactor       : Quality factor
*  @param quantTable          :User Defined Quant Table

********************************************************************************
*/
typedef struct IJPEGVENC_DynamicParams {
  /*--------------------------------------------------------------------------*/
  /* Base Class                                                               */
  /*--------------------------------------------------------------------------*/
  IVIDENC2_DynamicParams videnc2DynamicParams;
  
  /*--------------------------------------------------------------------------*/
  /* restartInterval : Specifies the number of MCUs between RST markers       */
  /*--------------------------------------------------------------------------*/
  XDAS_Int32             restartInterval;

  /*--------------------------------------------------------------------------*/
  /* qualityFactor : Specifies the quality setting to use for encoding        */
  /*                 Supported values are from 0 (lowest quality)             */
  /*                 to 97 (highest quality)                                  */
  /*--------------------------------------------------------------------------*/
  XDAS_Int32             qualityFactor;

  /*--------------------------------------------------------------------------*/
  /* quantTable : Application should populate the custom quant table in a     */
  /*              structure of type IJPEGVENC_CustomQuantTables and pass the  */
  /*              pointer to it here. Set it to NULL if custom quant tables   */
  /*              are not to be used.                                         */
  /*--------------------------------------------------------------------------*/
  IJPEGVENC_CustomQuantTables *quantTable;
  
} IJPEGVENC_DynamicParams;

/**
********************************************************************************
*  @struct     IJPEGVENC_InArgs
*
*  @brief       This structure defines the runtime input arguments
*               for IJPEGVENC::process
*
*  @param videnc2InArgs              :IVIDENC2_InArgs
*  @param APPN0                      :APP0 thumbnail Buffer
*  @param thumbnailIndexApp0       :JFIF Thumbnail Enable
*  @param APPN1                      :APP1 thumbnail Buffer
*  @param thumbnailIndexApp1       :EXIF Thumbnail Enable
*  @param Comment                    :Comment Data
********************************************************************************
*/

typedef struct IJPEGVENC_InArgs {
  /*--------------------------------------------------------------------------*/
  /* Base Class                                                               */
  /*--------------------------------------------------------------------------*/
  IVIDENC2_InArgs videnc2InArgs;
  
  /*--------------------------------------------------------------------------*/
  /* APPN0 : buffer holding the data for APP-0 Marker                         */
  /*--------------------------------------------------------------------------*/
  XDM2_SingleBufDesc  APPN0;

  /*--------------------------------------------------------------------------*/
  /* thumbnailIndexApp0 :                                                     */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16    thumbnailIndexApp0;

  /*--------------------------------------------------------------------------*/
  /* APPN1 : buffer holding the data for APP-1 Marker                         */
  /*--------------------------------------------------------------------------*/
  XDM2_SingleBufDesc  APPN1;

  /*--------------------------------------------------------------------------*/
  /* Thumbnail_Index_App0 :                                                   */
  /*--------------------------------------------------------------------------*/
  XDAS_UInt16    thumbnailIndexApp1;

  /*--------------------------------------------------------------------------*/
  /* Comment : buffer holding the data for comment Marker                     */
  /*--------------------------------------------------------------------------*/
  XDM2_SingleBufDesc  Comment;

} IJPEGVENC_InArgs;

/**
********************************************************************************
*  @struct     IJPEGVENC_OutArgs
*
*  @brief  This structure defines the runtime output arguments
*          for IJPEGVENC::process
*
*  @param  videnc2OutArgs :This structure contains run time output arguments for
*                          all IVIDENC2 instance objects
********************************************************************************
*/

typedef struct IJPEGVENC_OutArgs {
  IVIDENC2_OutArgs videnc2OutArgs;
} IJPEGVENC_OutArgs;

/**
********************************************************************************
*  @struct     IJPEGVENC_Fxns
*
*  @brief       This structure defines all of the operations on
*               JPEGVENC objects.
*  @param ividenc : Pointer to Functions which performs all the operations on
*                   IVIDENC2 objects.
********************************************************************************
*/
typedef struct IJPEGVENC_Fxns {
  IVIDENC2_Fxns ividenc;
} IJPEGVENC_Fxns;

/**
 * The error codes correspond to the 32-bit extended error parameter passed 
 * through outargs and get sttus. The error have been categorised to the below 
 * 32 groups and the respective bit is set on error occurrence.
 */
typedef enum
{
  IJPEGVENC_ERR_UNSUPPORTED_VIDENC2PARAMS = 0,
  IJPEGVENC_ERR_UNSUPPORTED_VIDENC2DYNAMICPARAMS,            
  IJPEGVENC_ERR_UNSUPPORTED_JPEGENCDYNAMICPARAMS,              
  IJPEGVENC_ERR_IMPROPER_DATASYNC_SETTING,      
  IJPEGVENC_ERR_NOSLICE,     
  IJPEGVENC_ERR_SLICEHDR,            
  IJPEGVENC_ERR_MBDATA,            
  IJPEGVENC_ERR_UNSUPPFEATURE,     
  IJPEGVENC_ERR_STREAM_END = 16,            
  IJPEGVENC_ERR_INVALID_MBOX_MESSAGE,    
  IJPEGVENC_ERR_HDVICP_RESET,
  IJPEGVENC_ERR_HDVICP_WAIT_NOT_CLEAN_EXIT,
  IJPEGVENC_ERR_IRES_RESHANDLE,
  IJPEGVENC_ERR_STANDBY,

  /* Error Codes for Data Sync */
  IJPEGVENC_ERR_INPUT_DATASYNC,
  IJPEGVENC_ERR_OUTPUT_DATASYNC

}IJPEGVENC_ExtendedErrorCodes;

/**
 * The enum corresponds to the 4 32-bit words used to pass the error codes to 
 * the application in the extended parameters of status stucture through the 
 * getstatus command. Each bit is set for an error which falls under one of 
 * the groups in the outargs 32 bvits.
 */
typedef enum
{
  JPEG_DYNAMIC_PARAMS_HANDLE_ERROR = 0,
  JPEG_STATUS_HANDLE_ERROR,
  JPEG_DYNAMIC_PARAMS_SIZE_ERROR,
  JPEG_ENCODE_HEADER_ERROR,
  JPEG_UNSUPPORTED_RESOLUTION,
  JPEG_CAPTURE_WIDTH_ERROR,
  JPEG_GET_DATA_FXN_NULL_POINTER,
  JPEG_GET_BUFFER_FXN_NULL_POINTER,
  JPEG_INVALID_RESTART_INTERVAL_ERROR,
  JPEG_INVALID_QUALITY_FACTOR_ERROR,
  JPEG_INVALID_INPUT_CHROMA_FORMAT_ERROR,
  JPEG_NULL_QUANT_TABLE_POINTER_ERROR,
  JPEG_NULL_INARGS_POINTER_ERROR,
  JPEG_NULL_INARGS_APP_POINTER_ERROR,
  JPEG_INARGS_SIZE_ERROR,
  JPEG_INVALID_INPUT_BYTES_ERROR,
  JPEG_INVALID_INPUT_ID_ERROR,
  JPEG_NULL_INPUT_BUF_DESC_ERROR,
  JPEG_NULL_INPUT_BUFFER_POINTER_ERROR,
  JPEG_INVALID_INPUT_BUFFER_SIZE_ERROR,
  JPEG_INVALID_NUM_OF_INPUT_BUFFERS_ERROR,
  JPEG_INVALID_INPUT_BUFFER_MEMTYPE_ERROR,
  JPEG_INVALID_OUTPUT_BUFFER_MEMTYPE_ERROR,
  JPEG_NULL_OUTARGS_POINTER_ERROR,
  JPEG_INVALID_OUTARGS_SIZE,
  JPEG_NULL_OUTPUT_BUF_DESC_ERROR,
  JPEG_NULL_OUTPUT_BUFFER_POINTER_ERROR,
  JPEG_INVALID_OUTPUT_BUFFER_SIZE_ERROR,
  JPEG_INVALID_NUM_OF_OUTPUT_BUFFERS_ERROR,
  JPEG_INSUFFICIENT_OUTPUT_BUFFER_SIZE_ERROR,
  JPEG_INVALID_JFIF_THUMBNAIL_ENABLE_ERROR,
  JPEG_INVALID_EXIF_THUMBNAIL_ENABLE_ERROR,
  JPEG_INPUT_BUFFER_POINTER_ALIGN_ERROR,
  
  /* Extended Error Codes related to Data Sync */
  JPEG_DATASYNC_GET_ROW_DATA_ERROR, /* from 33rd bit*/
  
  JPEG_DATASYNC_INVALID_RESTART_INTERVAL_ERROR,
  
  JPEG_DATASYNC_BLOCK_POINTER_ERROR,
  JPEG_DATASYNC_BLOCK_SIZE_ERROR,
  JPEG_DATASYNC_INVALID_BLOCKS_ERROR,
  JPEG_DATASYNC_NOT_VALID_COMBINATION_ERROR
}IjpegVENC_ErrorStatus;

/**
******************************************************************************
 *  ======== IJPEGVENC_DynamicParams ========
 *  Default Create parameter values for JPEGVENC instance objects
********************************************************************************
*/
extern const IJPEGVENC_DynamicParams JPEGVENC_TI_DYNAMICPARAMS;
/**
******************************************************************************
 *  ======== IJPEGVENC_Params ========
 *  Default DynamicParams values for JPEGVENC instance objects
********************************************************************************
*/
extern const IJPEGVENC_Params JPEGVENC_TI_PARAMS;

/*******************************************************************************
*  PRIVATE DECLARATIONS Defined here, used only here
*******************************************************************************/
/*---------------------- data declarations -----------------------------------*/

/*---------------------- function prototypes ---------------------------------*/

#endif /*#ifndef JPEGENC_TI_IJPEGVENC_H*/

