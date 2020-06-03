/** ==================================================================
 *  @file   iss_capture.h
 *
 *  @path   /ti/psp/iss/
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/**
 * \defgroup ISS_DRV_FVID2_CAPTURE_API Capture API
 *
 *  This modules define APIs to capture video data using VIP ports in ISS.
 *  This module can be used for single channel capture as well as multi-channel capture.
 *
 *  Capture driver supports the following FVID2 APIs (see also \ref ISS_DRV_FVID2_API)
 *
 - <b> Creating the driver </b> - FVID2_create()
     <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>drvId</td>
        <td>\ref FVID2_ISS_CAPT_VIP_DRV</td>
      </tr>
      <tr>
        <td>instanceId</td>
        <td>
            \ref ISS_CAPT_INST_VIP0_PORTA <br>
            \ref ISS_CAPT_INST_VIP0_PORTB <br>
            \ref ISS_CAPT_INST_VIP1_PORTA <br>
            \ref ISS_CAPT_INST_VIP1_PORTB <br>
            \ref ISS_CAPT_INST_VIP_ALL  <br>
        </td>
      </tr>
      <tr>
        <td>createArgs</td>
        <td>
        Iss_CaptCreateParams *
        </td>
      </tr>
      <tr>
        <td>createStatusArgs</td>
        <td>
        Iss_CaptCreateStatus *
        </td>
      </tr>
      <tr>
        <td>cbParams</td>
        <td>
        FVID2_CbParams *

        When FVID2_CbParams.cbFxn is set, FVID2_CbParams.cbFxn
        gets called based on value of Iss_CaptCreateParams.periodicCallbackEnable

        </td>
      </tr>
    </table>
    \ref FVID2_Handle returned by FVID2_create() is used in subsequent FVID2 APIs

  - <b> Deleting the driver </b> - FVID2_delete()
    <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>deleteArgs</td>
        <td>NOT USED, set to NULL</td>
      </tr>
    </table>

 - <b> Starting the driver </b> - FVID2_start()
    <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>cmdArgs</td>
        <td>NOT USED, set to NULL</td>
      </tr>
    </table>

 - <b> Stopping the driver </b> - FVID2_stop()
    <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>cmdArgs</td>
        <td>NOT USED, set to NULL</td>
      </tr>
    </table>

 - <b> Controlling the driver </b> - FVID2_control() <br>
 See \ref ISS_DRV_FVID2_IOCTL_CAPTURE for the list of IOCTLs supported by the driver

 - <b> Getting captured frames from the driver </b> - FVID2_dequeue()
    <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>frameList</td>
        <td>
        [OUT] FVID2_FrameList.numFrames returns the number of captured frames returned in this FVID2_dequeue() invocation. <br>
        [OUT] FVID2_FrameList.frames[0..FVID2_FrameList.numFrames-1] are the captured FVID2_Frame pointers to the captured frames. <br>
        [OUT] For each FVID2_Frame, FVID2_Frame.perFrameCfg points to Iss_CaptRtParams that was set during FVID2_queue()  <br>
        </td>
      </tr>
      <tr>
        <td>streamId</td>
        <td>
        Value can be from 0 .. Iss_CaptCreateParams.numStream-1
        </td>
      </tr>
      <tr>
        <td>timeout</td>
        <td>
        Must be BIOS_NO_WAIT
        </td>
      </tr>
    </table>

 - <b> Releasing used frames back to the driver </b> - FVID2_queue() <br>
 Also used to queue initial frame buffers to the driver, before calling FVID2_start().
   <table border="1">
      <tr>
        <th>Parameter</th>
        <th>Value</th>
      </tr>
      <tr>
        <td>frameList</td>
        <td>
        [IN] FVID2_FrameList.numFrames sets the number of captured frames given back to driver in this FVID2_queue() invocation. <br>
        [IN] FVID2_FrameList.frames[0..FVID2_FrameList.numFrames-1] are the captured FVID2_Frame pointers that are being returned. <br>
        [IN] For each FVID2_Frame, FVID2_Frame.perFrameCfg points to Iss_CaptRtParams.
              This structure is filled and returned when FVID2_dequeue() is called  <br>
        </td>
      </tr>
      <tr>
        <td>streamId</td>
        <td>
        Value can be from 0 .. Iss_CaptCreateParams.numStream-1 OR \ref ISS_CAPT_STREAM_ID_ANY
        </td>
      </tr>
    </table>

 * @{
*/

/**
 *  \file iss_capture.h
 *
 *  \brief Capture API
*/

#ifndef _ISS_CAPTURE_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _ISS_CAPTURE_H

/* ==========================================================================
 */
/* Include Files */
/* ==========================================================================
 */

#include <ti/psp/iss/iss.h>

#include <ti/psp/iss/hal/iss/isp/common/inc/isp_common.h>
#include <ti/psp/iss/hal/iss/isp/rsz/inc/rsz.h>

#include <ti/psp/iss/core/msp_types.h>
#include <ti/psp/iss/core/isp_msp.h>
/* ==========================================================================
 */
/* Macros & Typedefs */
/* ==========================================================================
 */

/**
    \name Capture Instance ID's

    @{
*/

/** \brief Capture instance - VP  */
#define ISS_CAPT_INST_VP (0u)

/** \brief Capture instance - CSI  */
#define ISS_CAPT_INST_CSI (1u)

/** \brief Capture instance - ALL  */
#define ISS_CAPT_INST_ALL    (0xFFu)

/** \brief ISP Capture instance - ALL  */
#define ISS_ISP_INST_ALL      (0xFFu)

/* @} */

/**
    \name Capture Max Limits

    @{
*/

/** \brief Capture instance - MAX instances  */
#define ISS_CAPT_INST_MAX        (1u)

/** \brief ISP Capture instance - MAX instances  */
#define ISS_ISP_INST_MAX   	     (1u)

/** \brief Maximun channels that can be captured per port */
#define ISS_CAPT_CH_PER_PORT_MAX       (1u)

/** \brief Maximun channels that can be captured in the system */
#define ISS_CAPT_CH_MAX       (ISS_CAPT_CH_PER_PORT_MAX*ISS_CAPT_INST_MAX)

/** \brief Maximum frames that can be queued per channel */
#define ISS_CAPT_FRAME_QUE_LEN_PER_CH_MAX  (32u)

/** \brief Maximum stream's that can be captured per Capture channel

  Stream's corresponds to different types of output that are possible
  for a given capture input source.

  The stream output type is setup by user during FVID2_create()

  Example streams are,

  For a 1080P input source,
  Stream 0: YUV420 non-scaled output
  Stream 1: YUV422 scaled output
  Stream 3: RAW VBI output

  Refer to user guide for more details about what valid stream
  combinations are possible.
*/
#define ISS_CAPT_STREAM_ID_MAX 	(2u)
/** \brief Maximum stream's that can be captured per ISP channel

  Stream's corresponds to different types of output that are possible
  for a given capture input source.

  The stream output type is setup by user during FVID2_create()

  Example streams are,

  For a 1080P input source,
  Stream 0: YUV420 non-scaled output
  Stream 1: YUV422 scaled output
  Stream 3: RAW VBI output

  Refer to user guide for more details about what valid stream
  combinations are possible.
*/
#define ISS_ISP_STREAM_ID_MAX 	(2u)

/* @} */

/** \brief Value to use for 'streamId' when required to do FVID2_queue
 to any stream

 Note, this value cannot be used as 'streamId' with FVID2_dequeue
*/
#define ISS_CAPT_STREAM_ID_ANY (0xFFFFu)

/**
 *  \brief Scalar ID to be used in the set scalar coefficient and set/get
 *  advance scalar params IOCTLS.
 *  Since this driver supports only one scalar per driver instance, the scalar
 *  could be identified by the instance handle. The below macro is used for
 *  initializing the scalar ID to a default value.
 */
#define ISS_CAPT_SCALAR_ID_DEFAULT      (0u)

/* Capture IOCTL's */

/**
  \addtogroup ISS_DRV_FVID2_IOCTL_CAPTURE
  @{
*/

/**
  * \brief Set scaler frame info
  *
  * This IOCTL is valid only when Scaler is enabled during create time
  * This IOCTL can be used for features like Down scaling, Pan, Crop
  *
  * Note, upscaling is not supported for capture path
  *
  * \param cmdArgs       [IN]  Iss_CaptScParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_SC_PARAMS       (ISS_CAPT_IOCTL_BASE + 0x0000u)

/**
  * \brief Set frame skip info
  *
  * This IOCTL can be used to control capture frame drop sequence,
  * Example, this IOCTL can be used to convert 60fps input source to 30fps
  * by dropping frames.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_CaptFrameSkip *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_FRAME_SKIP      (ISS_CAPT_IOCTL_BASE + 0x0001u)

/**
  * \brief Get 2A Alg Update
  *
  * This IOCTL can be used to get status about a specific channel, stream
  *
  * This control can be done independently for each stream, channel.
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_ALG_2A_UPDATE       (ISS_CAPT_IOCTL_BASE + 0x0002u)

/**
 * \brief Reset VP
 *
 * IMPORTANT: Only applicable to \ref ISS_CAPT_INST_VIP_ALL handle
 *
 * This IOCTL executes the VP reset sequence for VP.
 *
 * This will reset both Port A as well as Port B for that VP instance.
 *
 * Make sure no module is being accessed in that VP instance,
 * either in M2M path or capture path when this API is called.
 *
 * \param cmdArgs       [IN]  NULL
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return FVID2_SOK
*/
#define IOCTL_ISS_CAPT_RESET_VP          (ISS_CAPT_IOCTL_BASE + 0x0003u)

/**
 * \brief Reset CSI
 *
 * Same as \ref IOCTL_ISS_CAPT_RESET_CSI except that it operates on CSI
 *
 * \param cmdArgs       [IN]  NULL
 * \param cmdArgsStatus [OUT] NULL
 *
 * \return FVID2_SOK
*/
#define IOCTL_ISS_CAPT_RESET_CSI          (ISS_CAPT_IOCTL_BASE + 0x0004u)

/**
 * \brief Print detailed capture information
 *
 * IMPORTANT: Only applicable to \ref ISS_CAPT_INST_VIP_ALL handle
 *
 * This is meant to be used by driver developer for internal debugging purposes
 * It should not be used by user.
 *
 * \param cmdArgs    [IN] UInt32 Execution time in msecs
 *
 * \return FVID2_SOK
*/
#define IOCTL_ISS_CAPT_PRINT_ADV_STATISTICS (ISS_CAPT_IOCTL_BASE + 0x0005u)

/**
 *  \brief Check Capture overflow status and return the status
 *
 *  \param cmdArgs       [IN]  NULL
 *  \param cmdArgsStatus [OUT] Iss_CaptOverFlowStatus *
 *
 *  \return FVID2_SOK
 */
#define IOCTL_ISS_CAPT_CHECK_OVERFLOW       (ISS_CAPT_IOCTL_BASE + 0x0006u)

/**
 *  \brief If the overflow flag is set, resets Capt
 *
 *  This API does the following
 *  If VP output FIFO is overflowed
 *    - then its stops the driver on PortA, PortB
 *    - reset the VP instance
 *    - starts driver of PortA, PortB
 *
 *  \param cmdArgs       [IN]  const Iss_CaptOverFlowStatus *
 *  \param cmdArgsStatus [OUT] NULL
 *
 *  \return FVID2_SOK
 */
#define IOCTL_ISS_CAPT_RESET_AND_RESTART    (ISS_CAPT_IOCTL_BASE + 0x0007u)

/**
 *  \brief Flush capture driver and dequeue all frames including those that are not captured
 *
 *  This API can be called only when driver is in stopped state.
 *  Driver is in stopped state when,
 *  - FIVD2_stop() is called
 *  - FVID2_create() is called and FVID2_start() is not called.
 *
 *  This will return frames from all streams for a given capture handle.
 *  If this IOCTL is called with global handle then this will return
 *  frames from all capture handles.
 *
 *  Since maximum FVID2_MAX_FVID_FRAME_PTR frames can be returned at a time.
 *  This function should be called in a loop until FVID2_FrameList.numFrames = 0
 *  is returned in order to get back all the frames from the capture driver.
 *
 *  In case capture is in running state this function will return error.
 *
 *  This IOCTL will return capture frames, non-captured frames, as well as all frames
 *  held inside the hardware.
 *
 *  \param cmdArgs       [IN]  NULL
 *  \param cmdArgsStatus [OUT] FVID2_FrameList
 *
 *  \return FVID2_SOK
 */
#define IOCTL_ISS_CAPT_FLUSH                 (ISS_CAPT_IOCTL_BASE + 0x0008u)

/**
 * \brief User provided buffer to be used by driver when dropping data
 *
 * WARNING: Experimental IOCTL for test. Users should NOT call this IOCTL.
 *
 *  \param cmdArgs       [IN]  Void * pointer to data buffer
 *
 *  \return FVID2_SOK
*/
#define IOCTL_ISS_CAPT_DROP_DATA_BUFFER      (ISS_CAPT_IOCTL_BASE + 0x0009u)

/**
  * \brief Set Resolution info
  *
  * This IOCTL can be used to control capture resolution,
  * Example, this IOCTL can be used to change resolutions.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_CaptResolution *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_RESOLUTION        (ISS_CAPT_IOCTL_BASE + 0x000Au)

/**
  * \brief Set FrameRate info
  *
  * This IOCTL can be used to control capture FrameRate,
  * Example, this IOCTL can be used to change FrameRate.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_CaptFrameRate *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_FRAMERATE        (ISS_CAPT_IOCTL_BASE + 0x000Bu)

/**
  * \brief Capture Itt Data
  *
  * This IOCTL can be used to control capture of RAW and YUV Frames,
  * Example, this IOCTL can be used to Capture Raw Data.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_IttParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_ALG_ITT_CONTROL 			(ISS_CAPT_IOCTL_BASE + 0x000Cu)

/**
  * \brief Capture Mirror Mode
  *
  * This IOCTL can be used to set the modes.
  * Example, this IOCTL can be used to rotate 180 degrees.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Int32 mirrorMode
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_MIRROR_MODE 		(ISS_CAPT_IOCTL_BASE + 0x000Du)

/**
  * \brief Capture Set Color
  *
  * This IOCTL can be used to control Brightness, Contrast and Saturation,
  * Example, this IOCTL can be used to control Brightness.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_CaptColor *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_COLOR 			(ISS_CAPT_IOCTL_BASE + 0x000Eu)

/**
  * \brief Capture AEWB Vendor
  *
  * This IOCTL can be used to set the AEWB Vendor,
  * Example, this IOCTL can be used to set the AEWB Vendor.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  AEWB_VENDOR *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_AEWBVENDOR		(ISS_CAPT_IOCTL_BASE + 0x000Fu)

/**
  * \brief Capture AEWB Mode
  *
  * This IOCTL can be used to set the AEWB Mode
  * Example, this IOCTL can be used to set the AEWB Mode.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_AEWBMODE			(ISS_CAPT_IOCTL_BASE + 0x0010u)

/**
  * \brief Capture Set IRIS Data
  *
  * This IOCTL can be used to control Auto IRIS Data,
  * Example, this IOCTL can be used to control Auto IRIS Data.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_IRIS				(ISS_CAPT_IOCTL_BASE + 0x0011u)

/**
  * \brief Capture AEWB  Priority
  *
  * This IOCTL can be used to Set AEWB Priority,
  * Example, this IOCTL can be used to Set AEWB Priority.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_AEWBPRI	        (ISS_CAPT_IOCTL_BASE + 0x0012u)

/**
  * \brief Capture Sharpness
  *
  * This IOCTL can be used to Set Sharpness,
  * Example, this IOCTL can be used to Set Sharpness.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_IttParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_SHARPNESS		(ISS_CAPT_IOCTL_BASE + 0x0013u)

/**
  * \brief Capture BLC
  *
  * This IOCTL can be used to set BLC,
  * Example, this IOCTL can be used to set BLC.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_BLC		        (ISS_CAPT_IOCTL_BASE + 0x0014u)

/**
  * \brief Capture AWB Mode
  *
  * This IOCTL can be used to control AWB Mode,
  * Example, this IOCTL can be used to control AWB Mode.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_AWBMODE		    (ISS_CAPT_IOCTL_BASE + 0x0015u)

/**
  * \brief Capture Itt Data
  *
  * This IOCTL can be used to control AE Mode,
  * Example, this IOCTL can be used to control AE Mode.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_AEMODE		    (ISS_CAPT_IOCTL_BASE + 0x0016u)

/**
  * \brief Capture Env
  *
  * This IOCTL can be used to set Env,
  * Example, this IOCTL can be used to set Env.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_ENV              (ISS_CAPT_IOCTL_BASE + 0x0017u)

/**
  * \brief Capture BINNING
  *
  * This IOCTL can be used to set Binning Value,
  * Example, this IOCTL can be used to set Binning Value.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_BINNING          (ISS_CAPT_IOCTL_BASE + 0x0018u)

/**
  * \brief Capture DCC Params
  *
  * This IOCTL can be used to Set DCC params,
  * Example, this IOCTL can be used to set DCC params.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Ptr *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_DCCPRM          	(ISS_CAPT_IOCTL_BASE + 0x0019u)

/**
  * \brief Capture Histogram Address
  *
  * This IOCTL can be used to Set Histogram Address,
  * Example, this IOCTL can be used to Set Histogram Address.
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_GET_HISTADDR        	(ISS_CAPT_IOCTL_BASE + 0x001Au)

/**
  * \brief Capture Auto Focus
  *
  * This IOCTL can be used to capture Auto Focus Data,
  * Example, this IOCTL can be used to capture Auto Focus Data.
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_GET_AFDATA        	(ISS_CAPT_IOCTL_BASE + 0x001Bu)
#define IOCTL_ISS_CAPT_GET_AEWBDATA        	(ISS_CAPT_IOCTL_BASE + 0x001Cu)


/**
  * \brief Capture Brightness
  *
  * This IOCTL can be used to Set Brightness,
  * Example, this IOCTL can be used to Set Brightness.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_IttParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_BRIGHTNESS		(ISS_CAPT_IOCTL_BASE + 0x001Du)


/**
  * \brief Capture Contrast
  *
  * This IOCTL can be used to Set Contrast,
  * Example, this IOCTL can be used to Set Contrast.
  *
  * This control can be done independently for each stream, channel.
  *
  * \param cmdArgs       [IN]  Iss_IttParams *
  * \param cmdArgsStatus [OUT] NULL
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_CAPT_SET_CONTRAST			(ISS_CAPT_IOCTL_BASE + 0x001Eu)

#define IOCTL_ISS_CAPT_GET_EXPOSURE			(ISS_CAPT_IOCTL_BASE + 0x001Fu)

#define IOCTL_ISS_CAPT_SET_CROP				(ISS_CAPT_IOCTL_BASE + 0x0020u)

#define IOCTL_ISS_CAPT_SET_SATURATION		(ISS_CAPT_IOCTL_BASE + 0x0021u)

//# ISP version
#define IOCTL_ISS_ALG_VERSION				(ISS_CAPT_IOCTL_BASE + 0x0022u)

/* Capture ISP IOCTL */
/**
  * \brief ISP Resizer Config
  *
  * This IOCTL can be used to Configure Resizer for ISP Driver,
  *
  * \return FVID_SOK on success, else failure
  *
*/
#define IOCTL_ISS_ISP_RSZ_CONFIG  			(ISS_M2M_IOCTL_BASE + 0x0001u)

/* @} */

/**
 * \brief Video capture operation mode
*/
typedef enum {

    ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_STREAM = 0,
  /**< Single Stream Video Capture Mode */

    ISS_CAPT_VIDEO_CAPTURE_MODE_DUAL_STREAM,
  /**< Dual Stream Video Capture Mode */

    ISS_CAPT_VIDEO_CAPTURE_MODE_MAX
  /**< Maximum modes */
} Iss_CaptVideoCaptureMode;

/**
 * \brief Video interface mode
*/
typedef enum {

    ISS_CAPT_BAYER_12BIT = 0,
  /**< Embedded sync mode:  8bit - BT656 standard  */

    ISS_CAPT_YUV_8BIT,
  /**< Embedded sync mode:  16bit - BT1120 standard  */

    ISS_CAPT_YUV_16BIT,
  /**< Embedded sync mode:  24bit */

    ISS_CAPT_VIDEO_IF_MODE_MAX
  /**< Maximum modes */
} Iss_CaptVideoIfMode;

/**
 * \brief Video interface mode
*/
typedef enum {

    ISS_CAPT_INMODE_ISIF,
	/**< Capture Continious Mode Capture */
	ISS_CAPT_INMODE_DDR
	/**< Capture One Shot Mode Capture*/

} Iss_CaptMode;

/* ==========================================================================
 */
/* Structure Declarations */
/* ==========================================================================
 */

typedef struct {
    MSP_PROC_ISIF_VALID_ID eFProcIsifValidity;
    MSP_PROC_IPIPE_VALID_ID eFProcIpipeValidity;
    MSP_PROC_IPIPEIF_VALID_ID eFProcIpipeifValidity;
    MSP_PROC_IPIPE_RSZ_VALID_ID eFProcRszValidity;
    MSP_PROC_H3A_VALID_ID eFH3aValidity;
} IspFlagsMirrorT;

typedef struct {
    MSP_PTR pH3aAewbBuff;
    MSP_PTR pH3aAewbBuffNoPad;
    MSP_PTR pH3aAfBuff;
    MSP_PTR p2DLscTableBuff;
    MSP_HANDLE hIspHandle;
    MSP_HANDLE hInit;
    MSP_HANDLE hSdmaHandle;
    MSP_HANDLE hCamHMSP;
    MSP_HANDLE hCamLensHMSP;

    MSP_IspConfigParamsT tConfigParams;
    MSP_IpipeBscCfgT tBscCfg;
    MSP_BOOL bBscFromFD;
    MSP_IpipeHistCfgT tHistCfg;
    MSP_U32 nBscRowBuffSize;
    MSP_U32 nBscColBuffSize;
    MSP_APPCBPARAM_TYPE tIspAppParam;
    IspFlagsMirrorT tIspMirror;
    MSP_BOOL bVidStab;
    MSP_BOOL bAllocateOutBuff;
    MSP_U32 nAlterSeqNum;
    MSP_U32 nBscAlterSeqNum;
    MSP_U32 nARPrvAlterSeqNum;
    MSP_U32 nARCapAlterSeqNum;
    MSP_BOOL bFilledBuffsInvalidate;
    MSP_U32 nCalcStartX;
    MSP_U32 nCalcStartY;
    MSP_U32 nOutStartX;
    MSP_U32 nOutStartY;
    MSP_U32 nRemainIsrEvents;
    MSP_PTR pIssSem;
    MSP_U32 nIssInX;
    MSP_U32 nIssInY;
    MSP_U8 nLscEnable;
    MSP_BOOL nLscTableIsValid;
    MSP_U8 nProcessAreaAlign;
    MSP_HANDLE hAibhHeap;
    MSP_HANDLE hHwHeap;
    MSP_PTR pBscSem;
    MSP_IpipeBoxcarCfgT tBoxcar;

    MSP_U32 nFDOffset_SysState;
    MSP_U32 nFDOffset_SysStateFrm;
    MSP_U32 nFDOffset_ImgPrv;
    MSP_U32 nFDOffset_ImgPrvOmx;
    MSP_U32 nFDOffset_ImgFaceDetect;
    MSP_U32 nFDOffset_ImgRawOmx;
    MSP_U32 nFDOffset_H3A_AeAwb;
    MSP_U32 nFDOffset_H3A_Af;
    MSP_U32 nFDOffset_Bsc;
    MSP_U32 nFDOffset_Histogram;
    MSP_U32 nFDOffset_Boxcar;
    MSP_U32 nFDOffset_MeasurementData;
    MSP_U32 nFDOffset_EmbeddedData;
    MSP_BOOL bUseBTE;

    MSP_PTR p2AObj;
} VideoModuleInstanceT;

/**
 * \brief Capture window information
 *
 * Only valid when Iss_CaptOutInfo.scEnable is or can be TRUE
 *
 * Note, upscaling is not supported for capture path
*/
typedef struct {
    UInt32 inWidth;
  /**< Input source width, MUST be >= actual or expected
   * video source input width
  */

    UInt32 inHeight;
  /**< Input source height, MUST be >= actual or expected
     video source input width

     Height should be field height in case source is interlaced \n
     Height should be frame height in case source is progressive \n
  */

    UInt32 InStartX;
  /**< Input Start Index Width , in pixels */

    UInt32 InStartY;
  /**< Input Start Index Height , in pixels */

    UInt32 outWidth;
  /**< Scaler output width, in pixels */

    UInt32 outHeight;
  /**< Scaler output height, in lines */

    Iss_CropConfig inCropCfg;
  /**< Scalar input crop config
  */

    Iss_ScConfig *scConfig;
  /**< Scaler config, set NULL to setup default parameters */

    Iss_ScCoeffParams *scCoeffConfig;
  /**< Scaler co-eff config, set NULL to setup default co-effs */

} Iss_CaptScParams;

/**
 * \brief Capture output format information
*/
typedef struct {

    UInt32 dataFormat;
  /**< Output Data format, valid options are
      FVID2_DF_YUV422I_YUYV,
      FVID2_DF_YUV420SP_UV,
      FVID2_DF_YUV422SP_UV,
      FVID2_DF_RGB24_888,
      FVID2_DF_RAW_VBI.
      For valid values see #FVID2_DataFormat.

      If FVID2_DF_YUV422SP_UV is used as output format, it must
      be the first output format (output format at the
      index 0 in outStreamInfo in #Iss_CaptCreateParams).
   */

    UInt32 memType;
  /**< Tiled or non-tiled memory selection for output.
       For valid values see #Iss_VpdmaMemoryType.

      For FVID2_DF_YUV422I_YUYV, FVID2_DF_RGB24_888, FVID2_DF_RAW_VBI
      output, always ISS_VPDMA_MT_NONTILEDMEM will be used internally.

      User can control tiled or non-tiled for FVID2_DF_YUV420SP_UV,
      FVID2_DF_YUV422SP_UV
  */

    UInt32 pitch[FVID2_MAX_PLANES];
    /**< Pitch in bytes between two lines.
        Pitch can be specified separately for every plane.
    */

    UInt32 maxOutHeight;
    /**< Set limit on the max possible height of the output frame
        For valid values see #Iss_CaptMaxOutHeight
    */

    UInt32 scEnable;
  /**< TRUE: Use scaler before writing video data to memory,
   * FALSE: Scaler is not used in capture path
   * MUST be FALSE for line-multiplexed, pixel multiplexed modes  */

    UInt32 subFrameModeEnable;
  /**< TRUE: subframe based capture is enabled
      FALSE: subframe based capture is disabled

    Must be FALSE for multi-channel modes
  */

    UInt32 numLinesInSubFrame;
  /**< Number of lines per subframes for this output stream

    MUST be multiple of the stream output size.
    Not valid, ignored for ancillary data stream

    SubFrame callback gets called after every numLinesInSubFrame
    for every output stream, except ancillary data stream

    Ignored when subFrameModeEnable = FALSE
  */

    FVID2_SubFrameCbFxn subFrameCb;
  /**< SubFrame callback, is called for every subframe of this
      output stream

      Ignored when subFrameModeEnable = FALSE
  */

} Iss_CaptOutInfo;

/**
 * \brief Capture driver VS process parameters
*/
typedef struct {
    UInt32 bscBufferCurrV;
	/**< Memory Address where the vertical BSC data for the current frame is present */

    UInt32 bscBufferCurrH;
	/**< Memory Address where the horizontal BSC data for the current frame is present */

    UInt32 bscBufferPrevV;
	/**< Memory Address where the vertical BSC data for the previous frame is present */

    UInt32 bscBufferPrevH;
	/**< Memory Address where the horizontal BSC data for the previous frame is present */

	UInt32 startX;
	/**< X co-ordinate of stabilized frame */

	UInt32 startY;
	/**< Y co-ordinate of stabilized frame */

} Iss_CaptVsProcessPrm;

/**
 * \brief Capture driver create arguments, used when calling FVID2_create()
*/
typedef struct {
    VideoModuleInstanceT *pCompPrivate;

    UInt32 videoCaptureMode;
  /**< Video capture mode. For valid values see #Iss_CaptVideoCaptureMode */

    UInt32 videoIfMode;
  /**< Video interface mode. For valid values see #Iss_CaptVideoIfMode */

    UInt32 inDataFormat;
  /**< Input source color data format, valid values are given below \n
      FVID2_DF_YUV422P, ( 'P' is not relavent for input data format) \n
      FVID2_DF_YUV444P, ( 'P' is not relavent for input data format) \n
      FVID2_DF_RGB24_888.
      For valid values see #FVID2_DataFormat.
  */

    UInt32 periodicCallbackEnable;
  /**< TRUE: User callback passed during FVID2 create is called periodically
             at a fixed duration of about 8msecs
      FALSE: User callback passed during FVID2 create is called only
             if one or more frames are captured in any of the streams,
             channels belonging to this handle
  */

    UInt32 numCh;
  /**< Number of channel for multi-channel modes,

      Must be 1 for ISS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC
  */

    UInt32 vsEnable;
    /**< TRUE/FALSE: Enable/disable VSTAB */

	UInt32 glbcEnable;
    /**< TRUE/FALSE: Enable/disable GLBC */

    UInt32 numStream;
  /**< Number of streams to capture, MUST be <= ISS_CAPT_STREAM_ID_MAX  */
    UInt32 pitch[ISS_CAPT_STREAM_ID_MAX];

    Iss_CaptOutInfo outStreamInfo[ISS_CAPT_STREAM_ID_MAX];
  /**< Output format for each stream */

    Iss_CaptScParams scParams[ISS_CAPT_STREAM_ID_MAX];
  /**< Scaler parameters to use when
        Iss_CaptCreateParams.outStreamInfo[x].scEnable = TRUE

       Parameters are ignored when outStreamInfo[x].scEnable = FALSE
   */

    UInt32 channelNumMap[ISS_CAPT_STREAM_ID_MAX][ISS_CAPT_CH_PER_PORT_MAX];
  /**< Channel Number to assign to each channel and stream of this handle.
       This is used during FVID2_queue(), FVID2_dequeue().
       Channel number must be unique across the whole system.
       Users can use Iss_captMakeChannelNum() to generate a system unique channel number
  */

    FVID2_Handle SensorHandle;

    UInt32 aewbVendor;
  /**< AEWB algorithm vendor ID */

    UInt32 aewbMode;
  /**< AEWB mode */

    UInt32 aewbPriority;
  /**< AEWB priority */

	Iss_CaptMode captureMode;
  /**< Capture Mode */

	Int32 (*vsProcessFunc)(Iss_CaptVsProcessPrm *pVsProcessPrm);
  /**< VS process function pointer */

} Iss_CaptCreateParams;

/**
  \brief Capture driver create status. Returned after calling FVID2_create()
*/
typedef struct {
    Int32 retVal;
  /**< Create status, FVID2_SOK on success, else failure */

} Iss_CaptCreateStatus;

/**
  \brief Capture driver run-time parameters

  - This structure is returned by capture driver when FVID2_dequeue()
    is called by application
  - NOTE:this structure is NOT filled by driver when TILED mode is used
  - This structure is returned as part of FVID2_Frame.perFrameCfg
  - Application should make sure FVID2_Frame.perFrameCfg is set to a valid
    Iss_CaptRtParams pointer when queue-ing the frame back to capture driver
    \code
    Iss_CaptRtParams rtParams;

    pFrame->perFrameCfg = &rtParams;
    \endcode
  - Alternatively, user can pass back the same FVID2_Frame pointer without
    modifying FVID2_Frame.perFrameCfg
*/
typedef struct {

    UInt32 captureOutWidth;
  /**< Capture data width in pixels */

    UInt32 captureOutHeight;
  /**< Capture data height in lines */

    UInt32 captureOutPitch;
  /**< Capture data Pitch in lines */

} Iss_CaptRtParams;

/**
  \brief Capture Channel Get Status IOCTL input arguments
*/
typedef struct {

    UInt32 channelNum;
  /**< [IN] Channel number for which status is requested
  */

    UInt32 frameInterval;
  /**< [IN] Expected interval in units of timer ticks between frames.

    Iss_CaptChStatus.isVideoDetected is FALSE if no frame is captured
    for a duration of 'frameInterval x 2', else
    Iss_CaptChStatus.isVideoDetected is TRUE
  */

} Iss_CaptChGetStatusArgs;

/**
  \brief Capture Channel Get Status IOCTL result
*/
typedef struct {

    UInt32 isVideoDetected;
  /**< [OUT] TRUE: Video detected at this channel,
            FALSE: Video not detected at this channel
    */

    UInt32 outWidth;
  /**< [OUT] Capture data output width in pixels */

    UInt32 outHeight;
  /**< [OUT] Capture data output height in lines */

    UInt32 droppedFrameCount;
  /**< [OUT] Number of frame's dropped by driver due to unavailability
    of buffer from application  */

    UInt32 captureFrameCount;
  /**< [OUT] Number of frame's captured by driver into the buffer
             provided by application
    */

} Iss_CaptChStatus;

/**
  \brief Frame Skip parameters
*/
typedef struct {

    UInt32 channelNum;
  /**< Channel number of which frame skip will be applied
  */

    UInt32 frameSkipMask[ISS_CAPT_STREAM_ID_MAX];
    UInt32 frameSkipMaskHigh[ISS_CAPT_STREAM_ID_MAX];
  /**< Frame Skip Mask (bit0..bit29) bitN = 1 Skip frame,
        bitN = 0 DO NOT skip frame

    Example, 0x0 for no frame skip, 0x2AAAAAAA for skipping alternate frames
  */

} Iss_CaptFrameSkip;

/**
  \brief Itt parameters
*/
typedef struct {

    UInt32 ittCommand;
  /**< ITT Command Used
  */

    UInt32 *isif_dump;
  /**< Raw Memory Buffer address
  */

    UInt32 Raw_Data_Ready;
	/**  TRUE/FALSE: RAW output ready
     */
} Iss_IttParams;

/**
  \brief Resolution parameters
*/
typedef struct {

	UInt32 channelNum;
	/**< Channel number of which frame skip will be applied */

	UInt32 ResolutionWidth;		/**< Resolution Width */
    UInt32 ResolutionHeight;	/**< Resolution Height */

    UInt32 CropStartX;
    UInt32 CropStartY;

} Iss_CaptResolution;

/**
  \brief Resolution parameters
*/
typedef struct {

    UInt32 channelNum;
  /**< Channel number of which frame skip will be applied
  */

    UInt32 FrameRate;
  /**< Resolution Width
  */

} Iss_CaptFrameRate;

/**
  \brief VIP overflow status structure.
*/
typedef struct {
    UInt32 isPortOverFlowed[ISS_CAPT_INST_MAX];
    /**< Flag to indicate whether a port overflowed or not. */
} Iss_CaptOverFlowStatus;

/**
  \brief Color parameters
*/
typedef struct
{

    UInt32 saturation;
  /**< Saturation
  */

    UInt32 contrast;
  /**< Contrast
  */

    UInt32 brightness;
  /**< Brightness
  */

} Iss_CaptColor;

/**
  \brief DCC parameters
*/
typedef struct
{

    UInt8 *dcc_Default_Param;
  /**< DCC prm buffer address
  */

    Int32 dcc_init_done;
  /**< DCC init done flag
  */

    UInt32 dccSize;
  /**< DCC param size
  */

} Iss_CaptDccPrm;

/* ISS Post Pricess structures */

typedef struct {
    Int32 retVal;
  /**< Create status, FVID2_SOK on success, else failure */

} Iss_PpCreateStatus;

typedef struct
{
    UInt32 clkDivM;
  /**< Clk DIV M
  */
	UInt32 clkDivN;
  /**< Clk DIV N
  */

}Iss_PpCreateParams;

typedef struct
{
	FVID2_DataFormat inDataFmt;
	FVID2_DataFormat outDataFmt0;
	FVID2_DataFormat outDataFmt1;
	UInt32 inWidth;
	UInt32 inHeight;
	UInt32 inPitch;
	UInt32 outWidth0;
	UInt32 outHeight0;
	UInt32 pitch0;
	UInt32 outWidth1;
	UInt32 outHeight1;
	UInt32 pitch1;
	UInt32 numStream;

} Iss_PpRszConfig;

typedef struct {
    Int32 agc;
    Int32 shutter;
    Int32 ae_err;
} Iss_ExposureData;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* ==========================================================================
 */
/* Function Declarations */
/* ==========================================================================
 */

/**
 * \brief Get capture instance ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param channelNum [IN] channelNum
 *
 * \return instance ID ( 0 .. ISS_CAPT_INST_MAX-1 )
*/
/* ===================================================================
 *  @func     Iss_captGetInstId
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
static inline UInt32 Iss_captGetInstId(UInt32 channelNum)
{
    return channelNum / (ISS_CAPT_CH_PER_PORT_MAX * ISS_CAPT_STREAM_ID_MAX);
}

/**
 * \brief Get capture stream ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param channelNum [IN] channelNum
 *
 * \return stream ID ( 0 .. ISS_CAPT_STREAM_ID_MAX-1 )
*/
/* ===================================================================
 *  @func     Iss_captGetStreamId
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
static inline UInt32 Iss_captGetStreamId(UInt32 channelNum)
{
    UInt32 value;

    value = channelNum % (ISS_CAPT_CH_PER_PORT_MAX * ISS_CAPT_STREAM_ID_MAX);
    value /= ISS_CAPT_CH_PER_PORT_MAX;

    return value;
}

/**
 * \brief Get capture channel ID from channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
* \param channelNum [IN] channelNum
 *
 * \return channel ID ( 0 .. ISS_CAPT_CH_PER_PORT_MAX-1 )
*/
/* ===================================================================
 *  @func     Iss_captGetChId
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
static inline UInt32 Iss_captGetChId(UInt32 channelNum)
{
    return channelNum % (ISS_CAPT_CH_PER_PORT_MAX);
}

/**
 * \brief Make a system unique channelNum
 *
 * channelNum is value which is a combination of
 *  - instance ID
 *  - stream ID for that instance
 *  - channel ID for that stream, instance
 *
 * For details refer to VIP capture section in User Guide
 *
 * \param instId    [IN] VIP Instance ID
 * \param streamId  [IN] Stream ID
 * \param chId      [IN] Channel ID
 *
 * \return channelNum
*/
/* ===================================================================
 *  @func     Iss_captMakeChannelNum
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
static inline UInt32 Iss_captMakeChannelNum(UInt32 instId, UInt32 streamId,
                                            UInt32 chId)
{
    return instId * ISS_CAPT_CH_PER_PORT_MAX * ISS_CAPT_STREAM_ID_MAX
        + streamId * ISS_CAPT_CH_PER_PORT_MAX + chId;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif                                                     /* #ifndef
                                                            * _ISS_CAPTURE_H */

/* @} */
