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
* @file      jpegenc_ti_Test.c
*
* @brief     This is the top level client file that drives the JPEG
*            Encoder Call using XDM v1.0 Interface. This is the Application file
*            which user is allowed to change so as to give various kind of 
*            inputs and check the behaviour of the codec.
*            Algorithm Instance creation happens here , once this is done we 
*            call the Resource Manager to assign the resources , then depending
*            on the chroma format we ask for the memory requirement to 
*            GETBUFINFO and then allocate it for input and output and use it 
*            accordingly. Process call happens after these things happened 
*            successfully.
*
* @author    Pavan Kumar
*
* @version 0.0 (Dec 2009) : Created the initial version.
*
* @version 0.1 (Feb 2010) : Review & Clean up [vasudev]
*
* @version 0.2 (Aug 2010) : Debug Trace Feature Added[Vasudev]
*
* @version 0.3 (Sep 2010) : Added tiler.lib support[Vasudev]
*
* @version 0.4 (Nov 2010) : Increased the size of luma tiler space passed to 
*                           tiler_init API. Also, modified the datatype of
*                           variable holding the return value of tiler_init 
*                           to XDAS_Int32. [Pavan] 
*
* @version 0.5 (Dec 2010) : Added macros for Netra and OMPA4 Simulators[Naidu]
*******************************************************************************
*/
/* -------------------- compilation control switches -------------------------*/

/**
*  Set the VDMA View based on the Host Type
*/
#if defined(HOSTARM968_FPGA)
#define VDMAVIEW_EXTMEM      (0x07FFFFFF)
#elif defined(HOSTCORTEXM3_OMAP4) || defined(HOSTCORTEXM3_OMAP4SIMULATOR)
#define VDMAVIEW_EXTMEM      (0xFFFFFFFF) 
#elif defined(HOSTCORTEXM3_NETRA) || defined(HOSTCORTEXM3_NETRASIMULATOR)
#define VDMAVIEW_EXTMEM      (0xFFFFFFFF) 
#elif defined(HOSTCORTEXM3_GAIA)
#define VDMAVIEW_EXTMEM      (0x1FFFFFFF)
#else
#define VDMAVIEW_EXTMEM      (0x07FFFFFF)
#endif
#define SOURCE_PITCH_TILER8   (16*1024)
#define SOURCE_PITCH_TILER16  (32*1024)

/*******************************************************************************
*                             INCLUDE FILES
*******************************************************************************/
/* -------------------- system and platform files ----------------------------*/
#include <xdc/std.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ti/xdais/dm/ividenc2.h>
#include <ti/xdais/xdas.h>

/*--------------------- program files ----------------------------------------*/
#include <jpegenc_ti_Test.h>
#include <jpegenc_ti_rman_config.h>
#include <jpegenc_ti_config.h>
#include <jpegenc_ti.h>
#include <tilerBuf.h>

XDAS_UInt32 configCount = 0;

/* Global Variable for the Data Sync */
#define NUM_INPUT_BLOCKS (16)

XDAS_UInt8* curr_ddr_addr[NUM_INPUT_BLOCKS];

XDAS_UInt32 blockSizeArray[NUM_INPUT_BLOCKS] = {
  1024,4096,2048,1024,3072,1024,1024,1024,7168,10240,12288,25600,1024,
  51200,0x25000,1024,
};

XDAS_UInt32 blockPerDataSync[10] = {3,2,1,2,5,3,0,0,0,0};
XDAS_UInt32 GblCount = 0;
XDAS_UInt32 AccCount = 0;

/*******************************************************************************
*  PUBLIC DECLARATIONS Defined here, used elsewhere
*******************************************************************************/
/*---------------------- data declarations -----------------------------------*/
extern IRES_HDVICP2_Obj IRES_HDVICP2_handle;
FILE *fin, *fout;
JpegBPEncConfig         gConfig;

/*----------------------------------------------------------------------------*/
/*Array of elements of type sTokenMapping for parsing and holding the tokens  */
/*from the input configuration file.                                          */
/*----------------------------------------------------------------------------*/
static sTokenMapping sTokenMap[MAX_ITEMS_TO_PARSE] =
{
  {"MaxWidth"                      , &gConfig.maxWidth                 , 0 } ,
  {"MaxHeight"                     , &gConfig.maxHeight                , 0 } ,
  {"DataEndianess"                 , &gConfig.dataEndianness           , 0 } ,
  {"InputChromaFormat"             , &gConfig.inputChromaFormat        , 0 } ,
  {"InputContentType"              , &gConfig.inputContentType         , 0 } ,
  {"OperatingMode"                 , &gConfig.operatingMode            , 0 } ,
  {"InputDataMode"                 , &gConfig.inputDataMode            , 0 } ,
  {"OutputDataMode"                , &gConfig.outputDataMode           , 0 } ,
  {"NumInputUnits"                 , &gConfig.numInputDataUnits        , 0 } ,
  {"NumOutputUnits"                , &gConfig.numOutputDataUnits       , 0 } ,
  {"FrameWidth"                    , &gConfig.frameWidth               , 0 } ,
  {"FrameHeight"                   , &gConfig.frameHeight              , 0 } ,
  {"QualityFactor"                ,  &gConfig.qualityFactor            , 0 } ,
  {"CaptureWidth"                  , &gConfig.captureWidth             , 0 } ,
  {"generateHeader"                , &gConfig.generateHeader           , 0 } ,
  {"debugTraceLevel"               , &gConfig.debugTraceLevel          , 0 } ,
  {"lastNFramesToLog"              , &gConfig.lastNFramesToLog         , 0 } ,
  {"tilerEnable"                   , &gConfig.tilerEnable              , 0 } ,
  {"chromaTilerMode"               , &gConfig.chromaTilerMode          , 0 } ,
  {"NumFrames"                     , &gConfig.NumFrames                , 0 } ,  

  /*--------------------------------------------------------------------------*/
  /* ALWAYS the last element in the map                                       */
  /*--------------------------------------------------------------------------*/
  {"\0"                            , NULL                               , 0}
};

/*----------------------------------------------------------------------------*/
/*  Global flag for non-blocking call.                                        */
/*----------------------------------------------------------------------------*/
volatile XDAS_Int32      unServMesgCount;
volatile XDAS_Int32      ServMesgCount;

char    GlobalMemory[MAX_IO_MEMORY];
sTilerParams             tilerParams;
char    flag = 1;
unsigned int initial_size;
FILE* ftestFile;

/*******************************************************************************
*  PRIVATE DECLARATIONS Defined here, used only here
*******************************************************************************/
/*---------------------- data declarations -----------------------------------*/

/*---------------------- function prototypes ---------------------------------*/
/**
********************************************************************************
*  @fn          main(int argc, char *argv[])
*
*  @brief       Sample main program implementation which uses xDM v1.0 JPEG
*               encoder's APIs to create a encoder instance and encode multiple
*               input files. 
*               
*               Uses buffer manager for XDM v1.0 compatible buffer management 
*               and uses utility functions for reading raw data,
*               Seperating CbCr components,
*               Comparing output data with reference data &
*               Dumping raw jpeg encoded data into a file in binary format.
*
*  @param [in]  argc  :  Number of arguments
*
*  @param [in]  argv  :  Arguments
*
*  @return      XDM_EOK     : If successfully decoded all input streams/files
*               XDM_EFAIL   : If encountered error in decoding the given set of
*                             input files.
********************************************************************************
*/
int main(int argc, char *argv[])
{
  /*--------------------------------------------------------------------------*/
  /*  File I/O variables                                                      */
  /*--------------------------------------------------------------------------*/
  FILE         *fConfigFile, *finFile;
  FILE         *fTestVecsFile;
  char         inFile[FILE_NAME_SIZE];
  char         testFile[FILE_NAME_SIZE];
  char         configFile[FILE_NAME_SIZE], testCompliance;
  char         testvecsFile[FILE_NAME_SIZE];
  char         line[FILE_NAME_SIZE];
  char         sVersion[VERSION_SIZE];
  char         *pIoBuffer = GlobalMemory;
  
  /*--------------------------------------------------------------------------*/
  /*  Base Class Structures for status, input and output arguments.           */
  /*--------------------------------------------------------------------------*/
  JPEGVENC_Params            params;
  JPEGVENC_DynamicParams     dynamicParams;
  JPEGVENC_InArgs            inArgs;
  JPEGVENC_OutArgs           outArgs;
  JPEGVENC_Status            status;

  /*--------------------------------------------------------------------------*/
  /*  Declaring Algorithm specific handle                                     */
  /*--------------------------------------------------------------------------*/
  JPEGVENC_Handle            handle;

  /*--------------------------------------------------------------------------*/
  /*  Input/Output Buffer Descriptors                                         */
  /*--------------------------------------------------------------------------*/
  IVIDEO2_BufDesc           inputBufDesc;
  XDM2_BufDesc              outputBufDesc;

  /*--------------------------------------------------------------------------*/
  /*  Other local variables                                                   */
  /*--------------------------------------------------------------------------*/
  XDAS_Int32 retValue, extWidth, extHeight, numFramesEncoded = 0;
  XDAS_Int32 compareTestVal;
  XDAS_Int32 frmNumEncoded = 0;
  XDAS_UInt32 loopCount    = 0;
  XDAS_UInt32 offsetAdd    = 0;
  XDAS_UInt32 count        = 0;
  char  *outputAdd;

  /*--------------------------------------------------------------------------*/
  /*  Get the path of config file                                             */
  /*--------------------------------------------------------------------------*/
  strcpy(testvecsFile, 
  "..\\..\\..\\test\\testvecs\\config\\encoder_testvecs.cfg");

  /*--------------------------------------------------------------------------*/
  /*  Open Test Vectors File                                                  */
  /*--------------------------------------------------------------------------*/
  fTestVecsFile = fopen(testvecsFile,"r");

  /*--------------------------------------------------------------------------*/
  /*  Perform file open error check.                                          */
  /*--------------------------------------------------------------------------*/
  if (!fTestVecsFile)
  {
    printf("Couldn't open Parameter Config file %s.\n",testvecsFile);
    return XDM_EFAIL;
  }

  /*--------------------------------------------------------------------------*/
  /* Read Test Vectors parameter File until it reaches the end of file        */
  /*--------------------------------------------------------------------------*/
  while(!feof(fTestVecsFile))
  {
    AccCount = 0;
    GblCount = 0;
    numFramesEncoded = 0;
    
    /*------------------------------------------------------------------------*/
    /* Read Compliance Checking parameter                                     */
    /* when set, the frame output is compared with the reference output       */
    /*------------------------------------------------------------------------*/
    if(fgets(line,FILE_NAME_SIZE,fTestVecsFile))
    {
      sscanf(line,"%d",&testCompliance);
    }
    else
    {
      break ;
    }
    
    /*------------------------------------------------------------------------*/
    /* Read Parameters file name                                              */
    /*------------------------------------------------------------------------*/
    if(fgets(line,FILE_NAME_SIZE,fTestVecsFile))
    {
      sscanf(line,"%s",configFile);
    }
    else
    {
      break ;
    }
    
    /*------------------------------------------------------------------------*/
    /* Read Input file name                                                   */
    /*------------------------------------------------------------------------*/
    if(fgets(line,FILE_NAME_SIZE,fTestVecsFile))
    {
      sscanf(line,"%s",inFile);
    }
    else
    {
      break ;
    }

    /*------------------------------------------------------------------------*/
    /* Read Output file name                                                  */
    /*------------------------------------------------------------------------*/
    if(fgets(line,TESTVECS_FILENAME_SIZE,fTestVecsFile))
    {
      sscanf(line,"%s",testFile);
    }
    else
    {
      break ;
    }

    /*------------------------------------------------------------------------*/
    /*  Open Test Config File                                                 */
    /*------------------------------------------------------------------------*/
    fConfigFile = fopen(configFile,"r");
    
    /*------------------------------------------------------------------------*/
    /*  Perform file open error check.                                        */
    /*------------------------------------------------------------------------*/
    if (!fConfigFile)
    {
      printf("Couldn't open Parameter Config file %s.\n",configFile);
      return XDM_EFAIL;
    }

    /*------------------------------------------------------------------------*/
    /* Read Test Config Parameter File                                        */
    /*------------------------------------------------------------------------*/
    if(readparamfile(fConfigFile) != 0)
    {
      printf("Unable to read Config Parameter File %s\n", configFile);
      return(XDM_EFAIL);
    }
    
    /*------------------------------------------------------------------------*/
    /* Close Config Parameter File.                                           */
    /*------------------------------------------------------------------------*/
    fclose(fConfigFile);

    fprintf(stdout, "Testing: %s\n", inFile);
    
    /*------------------------------------------------------------------------*/
    /*  Setting the sizes of Base Class Objects                               */
    /*------------------------------------------------------------------------*/
    status.videnc2Status.size       = sizeof(IVIDENC2_Status);
    inArgs.videnc2InArgs.size       = sizeof(IVIDENC2_InArgs);
    outArgs.videnc2OutArgs.size     = sizeof(IVIDENC2_OutArgs);

    /*------------------------------------------------------------------------*/
    /*  Allocate memory for the reconstructed frames required during file     */
    /*  write.Align tbe extended width & height to 16 bit                     */
    /*------------------------------------------------------------------------*/
    extWidth                        = (gConfig.frameWidth + 15) & ~15;
    extHeight                       = (gConfig.frameHeight + 15) & ~15;

    /*------------------------------------------------------------------------*/
    /*  Initialization of static parameters is done by this function call     */
    /*------------------------------------------------------------------------*/
    TestApp_SetInitParams(&params, &gConfig);

    /*------------------------------------------------------------------------*/
    /*  Algorithm create call. This function will allocate memory for the     */
    /*  algorithm handle and for all the buffers (memtabs) required by the    */
    /*  algorithm and initialize them accordingly. The alg handle is          */
    /*  returned.                                                             */
    /*------------------------------------------------------------------------*/
    handle = JPEGVENC_create((JPEGVENC_Fxns *)&JPEGVENC_TI_IJPEGVENC, &params);

    /*------------------------------------------------------------------------*/
    /*  Check for errors in algorithm handle creation                         */
    /*------------------------------------------------------------------------*/
    if(handle == NULL)
    {
      fprintf(stdout, "Creation Failed.\n");
      fprintf(stdout, "Check the Create-time static parameter settings.\n");
      goto DELETE_INSTANCE;
    }
    
    /*------------------------------------------------------------------------*/
    /*  Print success message if handle creation is proper.                   */
    /*------------------------------------------------------------------------*/
    printf("\nAlgorithm Instance Creation Done...\n");
    
    /*------------------------------------------------------------------------*/
    /*  Call to RMAN Assign Resources (to verify the ires-rman functions.     */
    /*------------------------------------------------------------------------*/
    RMAN_AssignResources((IALG_Handle) handle);
    configCount++;

    /*------------------------------------------------------------------------*/
    /*  The following function is used to initialize the dynamic params of    */
    /*  the IVIDENC2 handle.                                                  */
    /*------------------------------------------------------------------------*/
    TestApp_SetDynamicParams(&dynamicParams, &gConfig);

    /*------------------------------------------------------------------------*/
    /* Control call for Get the number of buffers and their Sizes             */
    /*------------------------------------------------------------------------*/
    status.videnc2Status.data.buf = (XDAS_Int8 *)sVersion;
    status.videnc2Status.data.bufSize = VERSION_DATA_BUF_SIZE;

    JPEGVENC_control(handle, XDM_GETVERSION,
    (JPEGVENC_DynamicParams *)&dynamicParams,
    (JPEGVENC_Status *)&status);

    printf("Version = %s\n",sVersion);
    
    /*------------------------------------------------------------------------*/
    /*  XDM_SETPARAMS is called to pass the Dynamic Parameters to the         */
    /*  codec. The Control function shall validate the parameters. If an      */
    /*  error is found then the Control function returns failure.             */
    /*------------------------------------------------------------------------*/
    retValue = JPEGVENC_control
    (
    handle,
    XDM_SETPARAMS,
    (JPEGVENC_DynamicParams *)&dynamicParams,
    (JPEGVENC_Status *)&status
    );

    /*------------------------------------------------------------------------*/
    /*  Check for errors during Control function call to XDM_SETPARAMS.       */
    /*------------------------------------------------------------------------*/
    if(retValue != IVIDENC2_EOK)
    {
      if(status.videnc2Status.extendedError & 
          (XDM_UNSUPPORTEDINPUT))
      {
        fprintf(stdout, "Error: Invalid dynamic parameters. \n");
      }
      
      /*----------------------------------------------------------------------*/
      /*  Query for the default Dynamic parameters from the codec.            */
      /*----------------------------------------------------------------------*/
      retValue = JPEGVENC_control(handle, XDM_GETDYNPARAMSDEFAULT,
      (JPEGVENC_DynamicParams *)&dynamicParams,
      (JPEGVENC_Status *)&status);
      
      /*----------------------------------------------------------------------*/
      /*  Pass the dynamic Parameters to the codec.                           */
      /*----------------------------------------------------------------------*/
      retValue = JPEGVENC_control(handle, XDM_SETPARAMS,
      (JPEGVENC_DynamicParams *)&dynamicParams,
      (JPEGVENC_Status *)&status );
    }
    
    /*------------------------------------------------------------------------*/
    /*  The GETBUFINFO call gives information for number of input and         */
    /*  output buffers. For the first call however,                           */
    /*------------------------------------------------------------------------*/
    JPEGVENC_control(handle, XDM_GETBUFINFO,
    (JPEGVENC_DynamicParams *)&dynamicParams,
    (JPEGVENC_Status *)&status);

    /*------------------------------------------------------------------------*/
    /* Open Input YUV Sequence file.                                          */
    /*------------------------------------------------------------------------*/
    finFile = fopen(inFile, "rb");
    if(finFile == NULL)
    {
      printf("\n Unable to open Input YUV Sequence File %s\n", inFile);
      return(XDM_EFAIL);
    }

    /*------------------------------------------------------------------------*/
    /* Open Output Bitstream file.                                            */
    /*------------------------------------------------------------------------*/
    if(testCompliance)
    {
      ftestFile = fopen(testFile, "rb");
      
      if(ftestFile == NULL)
      {
        printf("\n Unable to open Reference Bitstream File %s\n", testFile);
        return(XDM_EFAIL);
      }
    }
    else
    {
      ftestFile = fopen(testFile, "wb");
      
      if(ftestFile == NULL)
      {
        printf("\n Unable to open Output Bitstream File %s\n", testFile);
        return(XDM_EFAIL);
      }
    }

    /*------------------------------------------------------------------------*/
    /*        initialize the input buffer values &  pointers                  */
    /*------------------------------------------------------------------------*/
    inputBufDesc.chromaFormat = status.videnc2Status.inputChromaFormat;
    inputBufDesc.numPlanes = status.videnc2Status.bufInfo.minNumInBufs;

    inputBufDesc.imageRegion.topLeft.x = 0;
    inputBufDesc.imageRegion.topLeft.y = 0;
    inputBufDesc.imageRegion.bottomRight.x = extWidth;
    inputBufDesc.imageRegion.bottomRight.y = extHeight;

    inputBufDesc.contentType = IVIDEO_PROGRESSIVE;

    offsetAdd = 0;

    /*-----------------------------------------------------------------------*/
    /*  Initialize the TILER params & Get the buffers in TILER space         */
    /*-----------------------------------------------------------------------*/
    testAppTilerInit(status.videnc2Status.bufInfo.minInBufSize);

    /*------------------------------------------------------------------------*/
    /*   Initialize Input buffer descriptors for all the planes used          */
    /*------------------------------------------------------------------------*/
    for(loopCount = 0;loopCount<inputBufDesc.numPlanes;loopCount++)
    {
      /*----------------------------------------------------------------------*/
      /*   Initialize Input buffer pointer                                    */
      /*----------------------------------------------------------------------*/
      inputBufDesc.planeDesc[loopCount].buf         = (XDAS_Int8 *) pIoBuffer + 
      offsetAdd;
      
      /*----------------------------------------------------------------------*/
      /*   Initialize Input buffer memory type                                */
      /*----------------------------------------------------------------------*/
      inputBufDesc.planeDesc[loopCount].memType     = XDM_MEMTYPE_RAW;
      
      /*----------------------------------------------------------------------*/
      /*   Initialize number of bytes of input data present in input buffer   */
      /*----------------------------------------------------------------------*/
      inputBufDesc.planeDesc[loopCount].bufSize.bytes = 
      status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.width *
      status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.height;
      
      /*----------------------------------------------------------------------*/
      /*   Initialize image pitch                                             */
      /*----------------------------------------------------------------------*/
      inputBufDesc.imagePitch[loopCount] = extWidth;
      offsetAdd += inputBufDesc.planeDesc[loopCount].bufSize.bytes;
    }
    
    outputAdd = pIoBuffer + offsetAdd ;

    /*------------------------------------------------------------------------*/
    /*        initialize the output  pointers                                 */
    /*------------------------------------------------------------------------*/
    outputBufDesc.numBufs =status.videnc2Status.bufInfo.minNumOutBufs;

    offsetAdd =0;

    /*------------------------------------------------------------------------*/
    /*   Initialize output buffer descriptors for all the planes used         */
    /*------------------------------------------------------------------------*/
    for(loopCount = 0; loopCount < outputBufDesc.numBufs; loopCount++)
    {
      /*----------------------------------------------------------------------*/
      /*   Initialize Output buffer pointer                                   */
      /*----------------------------------------------------------------------*/
      outputBufDesc.descs[loopCount].buf  = (XDAS_Int8 *)outputAdd + offsetAdd;
      
      /*----------------------------------------------------------------------*/
      /*   Initialize number of bytes of output data present in output buffer */
      /*----------------------------------------------------------------------*/
      outputBufDesc.descs[loopCount].bufSize.bytes = 
              status.videnc2Status.bufInfo.minOutBufSize[loopCount].bytes;
      
      /*----------------------------------------------------------------------*/
      /*   Initialize Output buffer memory type                               */
      /*----------------------------------------------------------------------*/
      outputBufDesc.descs[loopCount].memType       =
      status.videnc2Status.bufInfo.outBufMemoryType[loopCount];
      
      offsetAdd += outputBufDesc.descs[loopCount].bufSize.bytes;
    }

    /*------------------------------------------------------------------------*/
    /* Change here to test the Data Sync                                      */
    /*------------------------------------------------------------------------*/
    if (gConfig.outputDataMode == IVIDEO_SLICEMODE)
    {
      initial_size = 2048;
    }
    else
    {
      initial_size = 2560;
    }
    
    if (gConfig.outputDataMode != IVIDEO_ENTIREFRAME)
    {
      outputBufDesc.descs[0].bufSize.bytes = initial_size;
    }
    
    curr_ddr_addr[0] = (XDAS_UInt8*)outputBufDesc.descs[0].buf + initial_size;
    
    for(count = 1;count < NUM_INPUT_BLOCKS; count++)
    {
      curr_ddr_addr[count] = (XDAS_UInt8*)curr_ddr_addr[count - 1] + 
                             blockSizeArray[count -1];
    }
   
    /*------------------------------------------------------------------------*/
    /*  Initializing Thumbnail Buffer Descriptors in Inargs                   */
    /*------------------------------------------------------------------------*/
    inArgs.APPN0.buf                 = NULL;
    inArgs.APPN0.bufSize.bytes       = 0;
    inArgs.APPN0.memType             = XDM_MEMTYPE_RAW;

    /*------------------------------------------------------------------------*/
    /*  Encode Thumbnail data in APPO Marker                                  */
    /*------------------------------------------------------------------------*/
    inArgs.thumbnailIndexApp0        = 0;
    
    /*------------------------------------------------------------------------*/
    /*  Encode Thumbnail data in APPO Marker                                  */
    /*------------------------------------------------------------------------*/
    inArgs.thumbnailIndexApp1        = 0;

    /*------------------------------------------------------------------------*/
    /*  Do-While Loop for Encode Call                                         */
    /*------------------------------------------------------------------------*/
    do
    {
      /*----------------------------------------------------------------------*/
      /*Initialize the input ID in input arguments to the bufferid of         */
      /*the buffer element returned from getfreebuffer() function.            */
      /*----------------------------------------------------------------------*/
      inArgs.videnc2InArgs.inputID   = 1;

      /*----------------------------------------------------------------------*/
      /*   Initialize Input buffer descriptors for all the planes used        */
      /*----------------------------------------------------------------------*/
      offsetAdd = 0;
      
      for(loopCount = 0; loopCount < inputBufDesc.numPlanes; loopCount++)
      {
        /*--------------------------------------------------------------------*/
        /*   Initialize Input buffer pointer                                  */
        /*--------------------------------------------------------------------*/
        inputBufDesc.planeDesc[loopCount].buf       = (XDAS_Int8 *) pIoBuffer + 
        offsetAdd;
        
        /*--------------------------------------------------------------------*/
        /*   Initialize Input buffer memory type                              */
        /*--------------------------------------------------------------------*/
        inputBufDesc.planeDesc[loopCount].memType   = XDM_MEMTYPE_RAW;
        
        /*--------------------------------------------------------------------*/
        /*   Initialize number of bytes of input data present in input buffer */
        /*--------------------------------------------------------------------*/
        inputBufDesc.planeDesc[loopCount].bufSize.bytes = 
        status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.width *
        status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.height;
        
        /*--------------------------------------------------------------------*/
        /*   Initialize image pitch                                           */
        /*--------------------------------------------------------------------*/
        inputBufDesc.imagePitch[loopCount] = extWidth;
        offsetAdd += inputBufDesc.planeDesc[loopCount].bufSize.bytes;
      }
    
      /*----------------------------------------------------------------------*/
      /*  Read Input YUV Sequence data.                                       */
      /*----------------------------------------------------------------------*/
      if(gConfig.captureWidth != 0)
      {
        TestApp_ReadInputYUVData(&inputBufDesc, gConfig.captureWidth,
        gConfig.frameHeight, extWidth, extHeight, 
        frmNumEncoded, finFile);
      }
      else
      {
        TestApp_ReadInputYUVData(&inputBufDesc, gConfig.frameWidth,
        gConfig.frameHeight, extWidth, extHeight, 
        frmNumEncoded, finFile);
      }

      tilerParams.tilerSpace[0]    = CONTAINER_8BITS;
      tilerParams.tilerSpace[1]    = CONTAINER_16BITS;
      if(gConfig.chromaTilerMode)
      {
        tilerParams.tilerSpace[1]  = CONTAINER_8BITS;
      }

      for(loopCount = 0;loopCount<inputBufDesc.numPlanes;loopCount++)
      {
        if(gConfig.tilerEnable)
        {
          Tiler_DataMove((char *)inputBufDesc.planeDesc[loopCount].buf,
          (char *)(tilerParams.tiledBufferAddr[loopCount]),
          status.videnc2Status.bufInfo.minInBufSize[\
          loopCount].tileMem.width, 
          status.videnc2Status.bufInfo.minInBufSize[\
          loopCount].tileMem.height, 
          tilerParams.tilerSpace[loopCount] );
          
          /*------------------------------------------------------------------*/
          /* Initialize Input buffer pointer                                  */
          /*------------------------------------------------------------------*/
          inputBufDesc.planeDesc[loopCount].buf         = 
          (XDAS_Int8 *)(tilerParams.tiledBufferAddr[loopCount]);
          inputBufDesc.planeDesc[loopCount].memType = 
          status.videnc2Status.bufInfo.inBufMemoryType[loopCount];
          inputBufDesc.planeDesc[loopCount].bufSize.tileMem.width =
          status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.width;
          inputBufDesc.planeDesc[loopCount].bufSize.tileMem.height =
          status.videnc2Status.bufInfo.minInBufSize[loopCount].tileMem.height;
          
          if(gConfig.chromaTilerMode == TRUE && loopCount > 0)
          {
            inputBufDesc.planeDesc[loopCount].memType = XDM_MEMTYPE_TILED8;
          }
        }
      }
      
      /*----------------------------------------------------------------------*/
      /*  Start the encode process for one frame/field by calling the         */
      /*  JPEGVENC_encodeFrame() function.                                    */
      /*----------------------------------------------------------------------*/
      retValue  = JPEGVENC_encodeFrame(handle, &inputBufDesc,
      &outputBufDesc, &inArgs, &outArgs);
      
      /*----------------------------------------------------------------------*/
      /*  Check for failure message in the return value.                      */
      /*----------------------------------------------------------------------*/
      if(retValue != IVIDENC2_EOK)
      {
        /* Frame encoded unsuccessfully */
        fprintf(stdout, "Frame encoding failed.\n");
        fprintf(stdout,"Encoded number of bits = %d\n",
        outArgs.videnc2OutArgs.bytesGenerated * 8);
      }

      fprintf(stdout, "\nFrame #%d Bytes Generated: %d",(numFramesEncoded + 1),
      (outArgs.videnc2OutArgs.bytesGenerated));

      /*----------------------------------------------------------------------*/
      /*  Write the Output Bitstream to a file.                               */
      /*----------------------------------------------------------------------*/
      if(outArgs.videnc2OutArgs.bytesGenerated)
      {
        if(testCompliance)
        {
          /*------------------------------------------------------------------*/
          /* Compare the output with the reference                            */
          /*------------------------------------------------------------------*/
          compareTestVal = TestApp_CompareOutputData(ftestFile, &outputBufDesc, 
          &outArgs);
          
          if(compareTestVal == IVIDENC2_EOK)
          {
            printf(" : Passed\n");
          }
          else
          {
            /*----------------------------------------------------------------*/
            /* Test Compliance Failed... Breaking...                          */
            /* The output is not bit exact with the reference                 */
            /*----------------------------------------------------------------*/
            printf("************Failed*************\n");
            break;
          }
        }
        else
        {
          TestApp_WriteOutputData(&outputBufDesc,
          outArgs.videnc2OutArgs.bytesGenerated,
          ftestFile);

        }
      }

      /*----------------------------------------------------------------------*/
      /*  Increment the FramesEncoded count.                                  */
      /*----------------------------------------------------------------------*/
      numFramesEncoded++;

      /*----------------------------------------------------------------------*/
      /*  Datasync variables for every frame                                  */
      /*----------------------------------------------------------------------*/
      AccCount = 0;
      GblCount = 0;
    } while(numFramesEncoded < gConfig.NumFrames);

    DELETE_INSTANCE:

    /*------------------------------------------------------------------------*/
    /*  Free the buffers allocated for through IRES-RMAN.                     */
    /*------------------------------------------------------------------------*/
    RMAN_FreeResources((IALG_Handle) handle);                                 
    
    /*------------------------------------------------------------------------*/
    /*  Delete the Encoder Instance.                                          */
    /*------------------------------------------------------------------------*/
    JPEGVENC_delete(handle);                                                  
    
    /*------------------------------------------------------------------------*/
    /*  Close all the open file pointers.                                     */
    /*------------------------------------------------------------------------*/
    fclose(finFile);
    fclose(ftestFile);
    fprintf(stdout, "\n\n");

  } /* while !feof(testvecsFile) */

  fclose(fTestVecsFile);
  printf("End of Execution.\n\n");
  return (XDM_EOK);
}

/**
********************************************************************************
*  @fn     GetConfigFileContent (FILE *fname)
*
*  @brief  Reads the configuration file content in a buffer and returns the
*          address of the buffer
*
*  @param[in]  fname : Pointer to the configuration file.
*
*  @return Pointer to memory loaction holding configuration parameters/ 0
*
********************************************************************************
*/
char *GetConfigFileContent (FILE *fname)
{
  XDAS_Int32 FileSize;
  char *bufConfig;

  /*--------------------------------------------------------------------------*/
  /*  Set the File Data Pointer to the End of the file                        */
  /*--------------------------------------------------------------------------*/
  if (0 != fseek (fname, 0, SEEK_END))
  {
    return 0;
  }

  /*--------------------------------------------------------------------------*/
  /*  Read the current Pointer location which will give the file size         */
  /*--------------------------------------------------------------------------*/
  FileSize = ftell (fname);

  /*--------------------------------------------------------------------------*/
  /*  Check if filesize is valid                                              */
  /*--------------------------------------------------------------------------*/
  if (FileSize < 0 || FileSize > MAX_CONFIG_FILE_BUF_SIZE)
  {
    return 0;
  }

  /*--------------------------------------------------------------------------*/
  /*  Set the File Data Pointer to beginning of the file                      */
  /*--------------------------------------------------------------------------*/
  if (0 != fseek (fname, 0, SEEK_SET))
  {
    return 0;
  }

  /*--------------------------------------------------------------------------*/
  /*  Allocate buffer for storing data from config file                       */
  /*--------------------------------------------------------------------------*/
  if ((bufConfig = malloc(FileSize + 1)) == NULL)
  {
    fprintf(stdout, "No Enough memory for Config Buffer");
  }

  /*--------------------------------------------------------------------------*/
  /* Note that ftell() gives us the file size as the file system sees it.     */
  /* The actual file size, as reported by fread() below will be often         */
  /* smaller due to CR/LF to CR conversion and/or control characters after    */
  /* the dos EOF marker in the file.                                          */
  /*--------------------------------------------------------------------------*/
  FileSize = fread (bufConfig, 1, FileSize, fname);
  bufConfig[FileSize] = '\0';

  fclose (fname);

  return bufConfig;
}/* GetConfigFileContent() */

/**
********************************************************************************
*  @fn     ParameterNameToMapIndex (char *s)
*
*  @brief  Returns the index number from sTokenMap[] for a given parameter name.
*
*  @param[in]  s : parameter name string
*
*  @return The index number if the string is a valid parameter name, -1 for
*          error
********************************************************************************
*/
XDAS_Int32 ParameterNameToMapIndex (char *s)
{
  XDAS_Int32 indexNum = 0;

  while (sTokenMap[indexNum].tokenName != NULL)
  {
    if (0==strcmp (sTokenMap[indexNum].tokenName, s))
    return indexNum;
    else
    indexNum++;
  }

  return -1;
}/* ParameterNameToMapIndex() */

/**
********************************************************************************
* @fn    ParseContent (char *buf, XDAS_Int32 bufsize)
*
*@brief  Parses the character array buf and writes global variable input.This is
*        necessary to facilitate the addition of new parameters through the
*        sTokenMap[] mechanism. Need compiler-generated addresses in sTokenMap.
*
*@param[in,out]  buf     : Pointer to the buffer to be parsed
*
*@param[in]      bufsize : size of buffer
*
*@return Pass/Fail
********************************************************************************
*/
XDAS_Int32 ParseContent (char *buf, XDAS_Int32 bufsize)
{
  char *items[MAX_ITEMS_TO_PARSE];
  XDAS_Int32 MapIdx;
  XDAS_Int32 item = 0;
  XDAS_Int32 InString = 0, InItem = 0;
  char *pbuf = buf;
  char *bufend = &buf[bufsize];
  XDAS_Int32 IntContent;
  XDAS_Int32 itemNum;
  FILE *fpErr = stderr;

  /*--------------------------------------------------------------------------*/
  /*                                STAGE ONE                                 */
  /*--------------------------------------------------------------------------*/
  /* Generate an argc/argv-type list in items[], without comments and         */
  /* whitespace. This is context insensitive and could be done most easily    */
  /* with lex(1).                                                             */
  /*--------------------------------------------------------------------------*/
  while (pbuf < bufend)
  {
    switch (*pbuf)
    {
    case 13:  /* ASCII value 13 represents Carriage return                    */
      pbuf++;
      break;
    case '#':       /* Found comment                                          */
      *pbuf = '\0'; /* Replace '#' with '\0' in case of comment               */
      /* immediately following integer or string                */
      /* Skip till EOL or EOF, whichever comes first            */
      while (*pbuf != '\n' && pbuf < bufend)
      pbuf++;
      InString = 0;
      InItem = 0;
      break;
    case '\n':
      InItem = 0;
      InString = 0;
      *pbuf++='\0';
      break;
    case ' ':
    case '\t':             /* Skip whitespace, leave state unchanged          */
      if (InString)
      pbuf++;
      else
      {                    /* Terminate non-strings once whitespace is found  */
        *pbuf++ = '\0';
        InItem = 0;
      }
      break;
    case '"':              /* Begin/End of String                             */
      *pbuf++ = '\0';
      if (!InString)
      {
        items[item++] = pbuf;
        InItem = ~InItem;
      }
      else
      InItem = 0;
      InString = ~InString; /* Toggle                                         */
      break;
    default:
      if (!InItem)
      {
        items[item++] = pbuf;
        InItem = ~InItem;
      }
      pbuf++;
    }
  }

  item--;
  for (itemNum=0; itemNum<item; itemNum+= 3)
  {
    if (0 > (MapIdx = ParameterNameToMapIndex (items[itemNum])))
    {
      fprintf(fpErr, "\nParameter Name '%s' not recognized.. ", 
      items[itemNum]);
      return -1;
    }

    if (strcmp ("=", items[itemNum+1]))
    {
      fprintf(fpErr,
      "\nfile: '=' expected as the second token in each line.");
      return -1;
    }
    if(sTokenMap[MapIdx].type == 1)
    {
      strcpy(sTokenMap[MapIdx].place, items[itemNum+2]);
    }
    else
    {
      sscanf (items[itemNum+2], "%d", &IntContent);
      * ((XDAS_Int32 *) (sTokenMap[MapIdx].place)) = IntContent;
    }
  }

  return 0 ;
}/* ParseContent() */

/**
********************************************************************************
*  @fn     readparamfile(FILE * fname)
*
*  @brief  Top Level function to read the parameter file.
*
*  @param[in]  fname  : Pointer to the configuration file
*
*
*  @return pass/Fail
********************************************************************************
*/
XDAS_Int32 readparamfile(FILE * fname)
{
  char *FileBuffer = NULL ;
  XDAS_Int32 retVal ;

  /*--------------------------------------------------------------------------*/
  /*  read the content in a buffer                                            */
  /*--------------------------------------------------------------------------*/
  FileBuffer = GetConfigFileContent(fname);

  /*--------------------------------------------------------------------------*/
  /*  Parse the content of config file                                        */
  /*--------------------------------------------------------------------------*/
  if(FileBuffer)
  {
    retVal  = ParseContent(FileBuffer,strlen(FileBuffer));
    return retVal;
  }
  else
  {
    return -1;
  }
}/* readparamfile() */

/**
********************************************************************************
*  @fn   TestApp_ReadInput_444P_YUVData(IVIDEO2_BufDesc *inputBuf, 
*                                       short frmWidth, short frmHeight,
*                                       int frameCount, FILE * fin)
* 
*
*  @brief          Read Input YUV444P Data from a File
*
*  @param[in,out]  inputBuf  : Pointer to the Input Buffer
*
*  @param[in]      frmWidth  : Frame Width
*
*  @param[in]      frmHeight : Frame Height
*
*  @param[in]      frameCount: Number of Frames
*
*  @param[in]      fin       : Input File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_ReadInput_444P_YUVData(IVIDEO2_BufDesc *inputBuf, 
short frmWidth, short frmHeight, int frameCount, FILE * fin)
{
  
  unsigned char *dst;
  unsigned int loopCnt, u32ChromaPitch, u32LumaPitch;

  u32LumaPitch = ALIGN_16(frmWidth);
  u32ChromaPitch = ALIGN_16(frmWidth);
  
  /*--------------------------------------------------------------------------*/
  /* Read Y Data                                                              */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[0].buf;
  for(loopCnt = 0;loopCnt<frmHeight;loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32LumaPitch;
  }
  
  /*--------------------------------------------------------------------------*/
  /* Read U Data                                                              */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[1].buf;
  for(loopCnt= 0;loopCnt<(frmHeight);loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32ChromaPitch;
  }
  
  /*--------------------------------------------------------------------------*/
  /* Read V Data                                                              */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[2].buf;
  for(loopCnt= 0;loopCnt<(frmHeight);loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32ChromaPitch;
  }
  return 0;
}

/**
********************************************************************************
*  @fn   TestApp_ReadInput_422IBE_YUVData(IVIDEO2_BufDesc *inputBuf, 
*                                         short frmWidth, short frmHeight,
*                                         int frameCount, FILE * fin)
*
*  @brief  Read Input YUV422 IBE Data from a File
*
*  @param[in,out]  inputBuf  : Pointer to the Input Buffer
*
*  @param[in]      frmWidth  : Frame Width
*
*  @param[in]      frmHeight : Frame Height
*
*  @param[in]      frameCount: Number of Frames
*
*  @param[in]      fin       : Input File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_ReadInput_422IBE_YUVData(IVIDEO2_BufDesc *inputBuf, 
short frmWidth, short frmHeight,int frameCount, FILE * fin)
{

  unsigned char *dst;
  unsigned int loopCnt, u32LumaPitch;
  
  u32LumaPitch = ALIGN_16(frmWidth*2);
  
  /*--------------------------------------------------------------------------*/
  /* Read YUYV data                                                           */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[0].buf;
  for(loopCnt = 0;loopCnt<frmHeight;loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth<<1), fin);
    dst += u32LumaPitch;
  }
  return 0;
}

/**
********************************************************************************
*  @fn   TestApp_ReadInput_400Gry_YUVData(IVIDEO2_BufDesc *inputBuf, 
*                                         short frmWidth, short frmHeight,
*                                         int frameCount, FILE * fin)
*
*  @brief  Read Input Gray Data from a File
*
*  @param[in,out]  inputBuf  : Pointer to the Input Buffer
*
*  @param[in]      frmWidth  : Frame Width
*
*  @param[in]      frmHeight : Frame Height
*
*  @param[in]      frameCount: Number of Frames
*
*  @param[in]      fin       : Input File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_ReadInput_400Gry_YUVData(IVIDEO2_BufDesc *inputBuf, 
short frmWidth, short frmHeight,int frameCount, FILE * fin)
{
  unsigned char *dst;
  unsigned int loopCnt, u32LumaPitch;
  
  u32LumaPitch = ALIGN_16(frmWidth);
  
  /*--------------------------------------------------------------------------*/
  /* Read Y data                                                              */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[0].buf;
  for(loopCnt = 0;loopCnt<frmHeight;loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32LumaPitch;
  }
  return 0;
}

/**
********************************************************************************
*  @fn   TestApp_ReadInput_420SP_YUVData(IVIDEO2_BufDesc *inputBuf, 
*                                        short frmWidth, short frmHeight,
*                                        int frameCount, FILE * fin)
*
*  @brief  Read Input YUV420 Data from a File
*
*  @param[in,out]  inputBuf  : Pointer to the Input Buffer
*
*  @param[in]      frmWidth  : Frame Width
*
*  @param[in]      frmHeight : Frame Height
*
*  @param[in]      frameCount: Number of Frames
*
*  @param[in]      fin       : Input File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_ReadInput_420SP_YUVData(IVIDEO2_BufDesc *inputBuf,
short frmWidth, short frmHeight,int frameCount, FILE * fin)
{
  
  unsigned char *dst;
  unsigned int loopCnt, u32ChromaPitch, u32LumaPitch;
  
  u32LumaPitch = ALIGN_16(frmWidth);
  u32ChromaPitch = ALIGN_16(frmWidth);

  /*--------------------------------------------------------------------------*/
  /* Read Y data                                                              */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[0].buf;
  for(loopCnt = 0;loopCnt<frmHeight;loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32LumaPitch;
  }
  /*--------------------------------------------------------------------------*/
  /* Read UV Interleaved data                                                 */
  /*--------------------------------------------------------------------------*/
  dst = (unsigned char *) inputBuf->planeDesc[1].buf;
  for(loopCnt= 0;loopCnt<(frmHeight>>1);loopCnt++)
  {
    fread(dst, sizeof(unsigned char), (frmWidth), fin);
    dst += u32ChromaPitch;
  }
  return 0;
}

/**
********************************************************************************
*  @fn   TestApp_ReadInputYUVData(IVIDEO2_BufDesc *inputBuf, 
*                                 short frmWidth, short frmHeight,
*                                 short extWidth, short extHeight, 
*                                 int frameCount, FILE * fin)
*
*  @brief  Read Input YUV Data from a File
*
*  @param[in,out]  inputBuf  : Pointer to the Input Buffer
*
*  @param[in]      frmWidth  : Frame Width
*
*  @param[in]      frmHeight : Frame Height
*  
*  @param[in]      extWidth  : Extended Width
*  
*  @param[in]      extHeight : Extended Height
*
*  @param[in]      frameCount: Number of Frames
*
*  @param[in]      fin       : Input File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_ReadInputYUVData(IVIDEO2_BufDesc *inputBuf, 
short frmWidth, short frmHeight, short extWidth, short extHeight, 
int frameCount, FILE * fin)
{
  /*--------------------------------------------------------------------------*/
  /* Call Functions to read YUV Data depending on input Chroma format         */
  /*--------------------------------------------------------------------------*/
  switch(inputBuf->chromaFormat)
  {
  case XDM_YUV_422IBE :   TestApp_ReadInput_422IBE_YUVData(inputBuf, 
    frmWidth, frmHeight, frameCount, fin);
    break;
    
  case XDM_GRAY       :   TestApp_ReadInput_400Gry_YUVData(inputBuf, 
    frmWidth, frmHeight, frameCount, fin);
    break;
    
  case XDM_YUV_420SP  :   TestApp_ReadInput_420SP_YUVData(inputBuf, 
    frmWidth, frmHeight, frameCount, fin);
    break;
    
  case XDM_YUV_444P   :   TestApp_ReadInput_444P_YUVData(inputBuf, 
    frmWidth, frmHeight, frameCount, fin);
    break;
    
    default             :   break;
  }
  return 0;
}


/**
********************************************************************************
*  @fn   TestApp_WriteOutputData(XDM2_BufDesc *outputBuf,
*                                    int bytesGenerated,
*                                      FILE *fout)
*
*  @brief  Write Bitstream Data to a File
*
*  @param[in]      outputBuf      : Pointer to the Output Buffer
*
*  @param[in]      bytesGenerated : Number of Bytes Generated
*
*  @param[in,out]  fout           : Output File Pointer
*
* @return          0
********************************************************************************
*/
short TestApp_WriteOutputData(XDM2_BufDesc *outputBuf,int bytesGenerated, 
FILE *fout)
{
  /*--------------------------------------------------------------------------*/
  /*Get the Pointer to encoded data in external memory                        */
  /*--------------------------------------------------------------------------*/
  char *src = (char*)outputBuf->descs[0].buf;
  
  /*--------------------------------------------------------------------------*/
  /* Write Encoded data into output file                                      */
  /*--------------------------------------------------------------------------*/
  if (gConfig.outputDataMode == IVIDEO_ENTIREFRAME)
  {
    fwrite(src, bytesGenerated, 1, fout);
  }

  return 0;
}

/**
********************************************************************************
*  @fn   TestApp_CompareOutputData(FILE *fRefFile, XDM2_BufDesc *outputBufDesc,
*                                  JPEGVENC_OutArgs *outArgs)
*
*  @brief  Comparing Output Data with Reference File data
*
*  @param[in]  fRefFile        : Reference file pointer
*
*  @param[in]  outputBufDesc   : Pointer to Output Buffer Descriptors
*
*  @param[in]  outArgs         : Pointer to Outargs
*
*  @return  pass/fail
********************************************************************************
*/
XDAS_Int32 TestApp_CompareOutputData(FILE *fRefFile,
XDM2_BufDesc *outputBufDesc, JPEGVENC_OutArgs *outArgs)
{
  XDAS_Int32 bufNum, retVal;
  XDAS_UInt8 *refData;

  retVal = IVIDENC2_EOK;

  refData = malloc(outArgs->videnc2OutArgs.bytesGenerated);
  
  /*--------------------------------------------------------------------------*/
  /* Compare all the output Buffers with the ref File                         */
  /*--------------------------------------------------------------------------*/
  for(bufNum=0; bufNum < outputBufDesc->numBufs ; bufNum++)
  {
    /*------------------------------------------------------------------------*/
    /* Read Reference data from reference file to reference data buffer       */
    /*------------------------------------------------------------------------*/
    fread(refData, 1, outArgs->videnc2OutArgs.bytesGenerated, fRefFile);

    /*------------------------------------------------------------------------*/
    /* Compare reference data & output data generated.                        */
    /* Number of bytes compared is number of bytes generated by encoder       */
    /*------------------------------------------------------------------------*/
    if(memcmp(refData, outputBufDesc->descs[bufNum].buf,
          outArgs->videnc2OutArgs.bytesGenerated))
    {
      retVal = IVIDENC2_EFAIL;
    }
    break ;
  }
  free(refData);
  return retVal;
}

/**
********************************************************************************
*  @fn   TestApp_SetInitParams(JPEGVENC_Params *params,
*                              JpegBPEncConfig *config)
*
*  @brief  Set create time Parameters
*
*  @param[out]  params        : Pointer to Create time Parameters
*
*  @param[in]   config        : Pointer to config Structure
*
*  @return      none
********************************************************************************
*/

XDAS_Void TestApp_SetInitParams(JPEGVENC_Params *params,JpegBPEncConfig *config)
{
  /*--------------------------------------------------------------------------*/
  /*  Set IVIDENC2 parameters                                                 */
  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /*  Size: sizeof(IVIDENC2_Params)                                           */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.size                = sizeof(IJPEGVENC_Params);            
  
  /*--------------------------------------------------------------------------*/
  /*  maxHeight: HD Height (4096)                                             */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.maxHeight           = config->maxHeight; 
  
  /*--------------------------------------------------------------------------*/
  /*  maxWidth: HD Width   (4096)                                             */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.maxWidth            = config->maxWidth;  
  
  /*--------------------------------------------------------------------------*/
  /*  Data Endianness : XDM_BYTE. (Big endian stream)                         */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.dataEndianness      = config->dataEndianness;      
  
  /*--------------------------------------------------------------------------*/
  /*  inputChromaFormat :  Chroma format for the input buffer.                */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.inputChromaFormat   = config->inputChromaFormat;

  /*--------------------------------------------------------------------------*/
  /*  inputContentType :  Progressive or Interlaced format of the buffer      */
  /*                        being encoded.                                    */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.inputContentType    = config->inputContentType;

  /*--------------------------------------------------------------------------*/
  /*  operatingMode :  Video Coding Mode of operation (encode/decode/         */
  /*  transcode/transrate).                                                   */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.operatingMode       = config->operatingMode;

  /*--------------------------------------------------------------------------*/
  /*  inputDataMode :  Video input data mode                                  */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.inputDataMode       = config->inputDataMode;

  /*--------------------------------------------------------------------------*/
  /*  outputDataMode :  Video output data mode                                */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.outputDataMode      = config->outputDataMode;

  /*--------------------------------------------------------------------------*/
  /*  numInputDataUnits :  Number of input rows                               */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.numInputDataUnits   = config->numInputDataUnits;

  /*--------------------------------------------------------------------------*/
  /*  numOutputDataUnits :  Number of output slices                           */
  /*--------------------------------------------------------------------------*/
  params->videnc2Params.numOutputDataUnits  = config->numOutputDataUnits;

  /*--------------------------------------------------------------------------*/
  /*  Set Extended Parameters in IJpegVENC parameters                         */
  /*--------------------------------------------------------------------------*/
  params->maxThumbnailHSizeApp0 = 4096;
  params->maxThumbnailHSizeApp1 = 4096;
  params->maxThumbnailVSizeApp0 = 4096;
  params->maxThumbnailVSizeApp1 = 4096;
  params->debugTraceLevel = config->debugTraceLevel;

  params->lastNFramesToLog = config->lastNFramesToLog;
  return;
}

/**
********************************************************************************
*  @fn   TestApp_SetDynamicParams(JPEGVENC_DynamicParams *dynamicParams,
*                                 JpegBPEncConfig *config)
*
*  @brief  Set Dynamic Parameters
*
*  @param[out]  dynamicParams : Pointer to dynamic Parameters
*
*  @param[in]   config        : Pointer to config Structure
*
*  @return      none
********************************************************************************
*/
XDAS_Void TestApp_SetDynamicParams(JPEGVENC_DynamicParams *dynamicParams,
JpegBPEncConfig *config)
{

  /*--------------------------------------------------------------------------*/
  /*  Set IVIDENC2 Dynamic parameters                                         */
  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /*  Size: sizeof(JPEGVENC_DynamicParams)   incase of extended class         */
  /*  of paramaters.                                                          */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.size  = sizeof(JPEGVENC_DynamicParams);

  /*--------------------------------------------------------------------------*/
  /*  Input Height                                                            */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.inputHeight    = config->frameHeight;

  /*--------------------------------------------------------------------------*/
  /*  Input Width                                                             */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.inputWidth     = config->frameWidth;

  /*--------------------------------------------------------------------------*/
  /*  Capture Width                                                           */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.captureWidth   = config->captureWidth;

  /*--------------------------------------------------------------------------*/
  /*  generateHeader :  Encode Header only                                    */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.generateHeader = config->generateHeader;

  /*--------------------------------------------------------------------------*/
  /*  Set Extended Parameters in IJpegVENC parameters                         */
  /*--------------------------------------------------------------------------*/
  if (gConfig.outputDataMode == IVIDEO_SLICEMODE)
  {
    dynamicParams->restartInterval                     = 7;
  }
  else
  {
    dynamicParams->restartInterval                     = 0;
  }

  /*--------------------------------------------------------------------------*/
  /*  Initalize the Quality factor                                            */
  /*--------------------------------------------------------------------------*/
  dynamicParams->qualityFactor                       = config->qualityFactor;

  /*--------------------------------------------------------------------------*/
  /*  Initalize the Quantization Table                                        */
  /*--------------------------------------------------------------------------*/
  dynamicParams->quantTable                          = NULL;

  /*--------------------------------------------------------------------------*/
  /*    assign dataSync  callBack function pointers to encoder dynamicParams  */
  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  /*    INPUT DATA SYNC Function Pointer                                      */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.getDataFxn =
  (XDM_DataSyncGetFxn)JPEGVENC_TI_UpdateGetDataCallBack;

  dynamicParams->videnc2DynamicParams.getDataHandle = NULL;
  

  /*--------------------------------------------------------------------------*/
  /* OUTPUT DATA SYNC Function Pointer                                        */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.getBufferFxn =
  (XDM_DataSyncGetBufferFxn)JPEGVENC_TI_UpdateGetBufferCallBack;

  dynamicParams->videnc2DynamicParams.getBufferHandle = NULL;
  
  /*--------------------------------------------------------------------------*/
  /* PUTDATA DATA SYNC Function Pointer                                       */
  /*--------------------------------------------------------------------------*/
  dynamicParams->videnc2DynamicParams.putDataFxn =
  (XDM_DataSyncPutFxn)JPEGVENC_TI_PutDataCallBack;

  dynamicParams->videnc2DynamicParams.putDataHandle = NULL;
  
  return;
}

/**
*******************************************************************************
*  @fn       MEMUTILS_getPhysicalAddr(Ptr Addr)
*  @brief      This function will give the physical address on HOST_M3
*
*  @param[in]  Addr : Pointer to the address for which physical address
*                     is required
*
*  @return     Pointer to the physical memory
*******************************************************************************
*/
void *MEMUTILS_getPhysicalAddr(Ptr Addr)
{
  return ((void *)((unsigned int)Addr & VDMAVIEW_EXTMEM));
}

/**
********************************************************************************
*  @fn     JPEGVENC_TI_UpdateGetDataCallBack(  XDM_DataSyncHandle ptr,
*                                     XDM_DataSyncDesc *dataSyncDescrRowLevel)
*  @brief  Append one or more number of input rows into the frame buffer and
*          update the numBlocks equal to the total number of rows has been
*          filled into the frame buffer.
*          codec will do a call back as soon as it completed encoding the
*          number of rows available in frame buffer.
*          Codec assumes atleast one row appended per a call back.

*
*  @param[in]  ptr  : Pointer to the IALG_Handle structure
*
*  @param[in]  dataSyncDescrRowLevel : Pointer to the data sync descriptor
*
*  @return None
********************************************************************************
*/
XDAS_Void JPEGVENC_TI_UpdateGetDataCallBack( XDM_DataSyncHandle ptr,
XDM_DataSyncDesc* dataSyncDescrRowLevel)
{
  volatile int* pNumBlocks =  &dataSyncDescrRowLevel->numBlocks;    
  
  if (gConfig.inputDataMode == IVIDEO_NUMROWS) /* NUM_ROWS */
  {
    /*------------------------------------------------------------------------*/
    /* get the input to frame buffer and update the numBlocks.                */
    /* numBlocks should be total number of input rows available               */
    /* from the start of the frame, codec internally knows the                */
    /* number of rows it has encoded already.                                 */
    /* codec assumes atleast one row appended per a call back.                */
    /*------------------------------------------------------------------------*/
    *pNumBlocks = 3;
  }

  return;
}


/**
********************************************************************************
*  @fn     JPEGVENC_TI_UpdateGetBufferCallBack(  XDM_DataSyncHandle ptr,
*                                  XDM_DataSyncDesc *dataSyncDescrFixedLength )
*                                  
*  @brief  Append one or more number of input rows into the frame buffer and
*          update the numBlocks equal to the total number of rows has been
*          filled into the frame buffer.
*          codec will do a call back as soon as it completed encoding the
*          number of rows available in frame buffer.
*          Codec assumes atleast one row appended per a call back.
*
*  @param[in]  ptr  : Pointer to the IALG_Handle structure
*
*  @param[in]  dataSyncDescrFixedLength : Pointer to the data sync descriptor
*
*  @return None
********************************************************************************
*/
XDAS_Void JPEGVENC_TI_UpdateGetBufferCallBack( XDM_DataSyncHandle ptr,
XDM_DataSyncDesc* dataSyncDescrFixedLength)
{
  if (gConfig.outputDataMode == IVIDEO_FIXEDLENGTH)
  {
    dataSyncDescrFixedLength->scatteredBlocksFlag = TRUE;
    dataSyncDescrFixedLength->varBlockSizesFlag   = TRUE;
    
    /*------------------------------------------------------------------------*/
    /* In IVIDEO_FIXEDLENGTH Mode the stream buffer may contigous or          */
    /* non-contigous Memory depending upon the Scatter Flag                   */
    /*------------------------------------------------------------------------*/
    if(dataSyncDescrFixedLength->scatteredBlocksFlag == TRUE)
    {
      dataSyncDescrFixedLength->numBlocks  = blockPerDataSync[GblCount];
      /* (1 <= numBlocks <= 4) */
      
      dataSyncDescrFixedLength->baseAddr   = 
      (XDAS_Int32*)&curr_ddr_addr[AccCount]; 
      dataSyncDescrFixedLength->blockSizes = 
      (XDAS_Int32*)&blockSizeArray[AccCount];
    }
    else
    {
      dataSyncDescrFixedLength->numBlocks  = 1; 
      dataSyncDescrFixedLength->baseAddr   = (XDAS_Int32*)curr_ddr_addr[0];
      dataSyncDescrFixedLength->blockSizes = (XDAS_Int32*)&blockSizeArray[0];
    }
    
    AccCount += blockPerDataSync[GblCount];
    GblCount++;
  }
  else if(gConfig.outputDataMode == IVIDEO_SLICEMODE)
  {
    dataSyncDescrFixedLength->scatteredBlocksFlag = FALSE;
    dataSyncDescrFixedLength->varBlockSizesFlag   = FALSE;
    
    dataSyncDescrFixedLength->numBlocks  = 1;
    dataSyncDescrFixedLength->blockSizes = (XDAS_Int32*)&blockSizeArray[0];
  }
  
  return;
}
/**
********************************************************************************
*  @fn     JPEGVENC_TI_PutDataCallBack(  XDM_DataSyncHandle ptr,
*                                     XDM_DataSyncDesc *putDataSyncDescr )
*                                      
*  @brief    Call back funtion from IVAHD to Application informing about the 
*            availability of the output stream
*
*
*  @param[in]  ptr  : Pointer to the IALG_Handle structure
*
*  @param[in]  putDataSyncDescr : Pointer to the data sync descriptor
*
*  @return     None
********************************************************************************
*/
XDAS_Void JPEGVENC_TI_PutDataCallBack( XDM_DataSyncHandle ptr,
XDM_DataSyncDesc* putDataSyncDescr)
{
  /*--------------------------------------------------------------------------*/
  /* Write Encoded data into output file                                      */
  /*--------------------------------------------------------------------------*/
  fwrite(putDataSyncDescr->baseAddr, putDataSyncDescr->blockSizes[0], 1, 
  ftestFile);
}

/** 
********************************************************************************
*  @fn     SetPATViewMapBase
*
*  @brief  This function provides the base address, i.e the physical address 
*          in external memory to be used as tiler memory.
*
*  @return None
********************************************************************************
*/
void SetPATViewMapBase()
{
/*--------------------------------------------------------------------------*/
/* The default values set in gel files is 0x8000000 which will              */
/* overlap on the .text mapped in client. Hence, to avoid corruption        */
/* of vital memory we are mapping the tiler base address to a               */
/* different value.                                                         */
/* In below piece of code, programming of Below Registers are done          */
/* DMM_PAT_VIEW_MAP                                                         */
/* DMM_PAT_VIEW_MAP_BASE                                                    */
/* DMM_PAT_IRQSTATUS_RAW                                                    */
/* DMM_PAT_STATUS                                                           */
/* DMM_PAT_AREA                                                             */
/* DMM_PAT_DATA                                                             */
/*--------------------------------------------------------------------------*/
#if defined(HOSTCORTEXM3_OMAP4) || defined(HOSTCORTEXM3_OMAP4SIMULATOR)
  /*------------------------------------------------------------------------*/ 
  /* Tiler is mapped to following physical container addresses:             */
  /* TILE8_PHY_ADDR -  0x9000 0000 -> 0x9800 0000                           */
  /* TILE16_PHY_ADDR - 0x9800 0000 -> 0xA000 0000                           */
  /* TILE32_PHY_ADDR - 0xA000 0000 -> 0xA800 0000                           */
  /* TILEPG_PHY_ADDR - 0xA000 0000 -> 0xA800 0000                           */
  /* Tiler32 & TilePG are overlayed with assumption that Tile32 is not      */ 
  /* used                                                                   */
  /*------------------------------------------------------------------------*/

      *(int *)(0x4E000440) = 0x04040302; 
      *(int *)(0x4E000444) = 0x04040302; 
      *(int *)(0x4E000448) = 0x04040302; 
      *(int *)(0x4E00044C) = 0x04040302; 

      *(int *)(0x4E000460) = 0x80000000; 

      *(int *)(0x4E000480) = 0x00000003; 
      *(int *)(0x4E0004C0) = 0x0000000B; 
      *(int *)(0x4E000504) = 0x3FFF20E0;
      *(int *)(0x4E00050C) = 0x8510F010;
#endif

#if defined(HOSTCORTEXM3_NETRA) || defined(HOSTCORTEXM3_NETRASIMULATOR)

  /*------------------------------------------------------------------------*/
  /* Tiler is mapped to following physical container addresses:             */
  /* TILE8_PHY_ADDR -  0xA000 0000 -> 0xA800 0000                           */
  /* TILE16_PHY_ADDR - 0xA800 0000 -> 0xB000 0000                           */
  /* TILE32_PHY_ADDR - 0xB000 0000 -> 0xB800 0000                           */
  /* TILEPG_PHY_ADDR - 0xB000 0000 -> 0xB800 0000                           */
  /* Tiler32 & TilePG are overlayed with assumption that Tile32 is not      */ 
  /* used                                                                   */
  /*------------------------------------------------------------------------*/
      *(int *)(0x4E000440) = 0x06060504; 
      *(int *)(0x4E000444) = 0x06060504; 
      *(int *)(0x4E000448) = 0x06060504; 
      *(int *)(0x4E00044C) = 0x06060504;

      *(int *)(0x4E000460) = 0xA0000000;

      *(int *)(0x4E000480) = 0x00000003; 
      *(int *)(0x4E0004C0) = 0x0000000B; 
      *(int *)(0x4E000504) = 0x3FFF20E0;
      *(int *)(0x4E00050C) = 0x8510F010;
#endif
}

/** 
********************************************************************************
*  @fn     XDAS_Void testAppTilerInit(XDM2_BufSize *bufSizes)
*
*  @brief  The testAppTilerInit function initializes & chunks the TILER 
*          memory into required number with required 2D block sizes.
*
*  @param[in] bufSizes  : Address of array containing the buffer sizes of the 
*                         numbufs.
*
*  @return None
********************************************************************************
*/
XDAS_Void testAppTilerInit(XDM2_BufSize *bufSizes)
{
  XDAS_Int32    ret_val;
  
  /*--------------------------------------------------------------------------*/
  /*  Get the buffers in TILER space                                          */
  /*--------------------------------------------------------------------------*/
  if(gConfig.tilerEnable)                                                     
  {                                                                           
    /*------------------------------------------------------------------------*/
    /* Get the buffers in TILER space                                         */
    /* Do the allocation for maximum supported resoultion and keep to some    */
    /* aligned boundary. Also this allocation assumes that tiled8 and         */
    /* tiled16 are mapped to same physical space                              */
    /*------------------------------------------------------------------------*/
    sTilerParams *pTilerParams     = &tilerParams;
    
    /*------------------------------------------------------------------------*/
    /* Set the base address of the tiler memory area allocated by the tiler   */
    /* library.                                                               */
    /*------------------------------------------------------------------------*/
    SetPATViewMapBase();
                            
    pTilerParams->tilerSpace[0]    = CONTAINER_8BITS;                         
    pTilerParams->tilerSpace[1]    = CONTAINER_16BITS;                        
    if(gConfig.chromaTilerMode)                                               
    {                                                                         
      pTilerParams->tilerSpace[1]  = CONTAINER_8BITS;                         
    }                                                                         
    
    pTilerParams->imageDim[0]      = bufSizes[0].tileMem.width;               
    pTilerParams->imageDim[1]      = bufSizes[0].tileMem.height;              

    /*------------------------------------------------------------------------*/
    /* Max Luma Size we will be using (4096*4096) is set to  totalSizeLuma    */
    /*------------------------------------------------------------------------*/
    pTilerParams->totalSizeLuma    = 0x6000000;                               
    
    pTilerParams->memoryOffset[0]  = 0;                                       
    pTilerParams->memoryOffset[1]  = pTilerParams->totalSizeLuma;             
    
    ret_val = tiler_init(pTilerParams);                                       
    if(ret_val == -1)                                                         
    {                                                                         
      /*----------------------------------------------------------------------*/
      /* If tiler allocation is not successful then Force both the buffer     */
      /* to be in raw region                                                  */
      /*----------------------------------------------------------------------*/
      gConfig.tilerEnable = 0 ;                                               
    }
  }
}

