/*******************************************************************************
 *                                                                             *
 *  Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com/     *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
    \ingroup LINK_API
    \defgroup VIDBITSTREAM_LINK_API Video Bitstream data structure definition

    This file defines the data structure representing an encoded video frame's
    bitstream object

    @{
*/

/**
    \file vidbitstream.h
    \brief Definition of video bitstream Link API
*/

#ifndef _VIDBITSTREAM_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _VIDBITSTREAM_H_

/**
 * @brief  Buffer alignment needed for IVA-HD codecs
 */

#ifdef TI_8107_BUILD
#define  IVACODEC_VDMA_BUFFER_ALIGNMENT                                    (128)
#else
#define  IVACODEC_VDMA_BUFFER_ALIGNMENT                                    (32)
#endif
/** \brief  Maximum number of bitstream buf in a Bitstream_BufList */
#define VIDBITSTREAM_MAX_BITSTREAM_BUFS            (256)

/**
 *  \brief Bit stream Buffer 
 *   Defines the Bit stream Buffer data structure used to exchange 
 *   video Bitstream meta data between links
 */
typedef struct Bitstream_Buf {
    UInt32 reserved[2];
    /**< First two 32 bit entries are reserved to allow use as Que element */
    void *addr;
    /**< Buffer Pointer */
    UInt32 bufSize;
    /**< Size of the buffer */
    UInt32 fillLength;
    /**< Filled lengh from start offset */
    UInt32 startOffset;
    /**< Start offset */
    UInt32 mvDataOffset;
    /**< Actual offset to mv data bistream in buffer, in bytes */
    UInt32 mvDataFilledSize;
    /**< Actual size of mv data bistream in buffer, in bytes */
    UInt32 channelNum;
    /**< Channel number */
    UInt32 codingType;
    /**< Coding type */
    void *appData;
    /**< Additional application parameter per buffer */

    UInt32 timeStamp;
    /**< Original Capture time stamp */

    UInt32 temporalId;
    /**< SVC TemporalId */

    UInt32 upperTimeStamp;
    /**< Original Capture time stamp:Upper 32 bit value*/
    UInt32 lowerTimeStamp;
    /**< Original Capture time stamp: Lower 32 bit value*/
    UInt32 encodeTimeStamp;
    /**< Encode complete time stamp */

    UInt32 isKeyFrame;
    /**< Flag indicating whether is currentFrame is key frame */
    UInt32 allocPoolID;
    /**< Pool frame from which buf was originally alloced */
    UInt32 phyAddr;
    /**< Physical address of the buffer */
    UInt32 frameWidth;
    /**< Width of the encoded frame */
    UInt32 frameHeight;
    /**< Height of the encoded frame */
    UInt32 doNotDisplay;
    /**< Flag indicating frame should not be displayed
     *   This is useful when display should start from a
     *   particular frame.
     *   This is temporary until Avsync suuports seek functionality*/
     UInt32 bottomFieldBitBufSize;
     /**< Size of the bottom field Bitstream. Filled by field Merged
          interlaced encoders     */
} Bitstream_Buf;

/**
 *  \brief Bit stream Buffer List 
 *   Defines the Bit stream Buffer List structure used to exchange 
 *   multiple Bitstream Buffers between links
 */
typedef struct
{
    Bitstream_Buf       *bufs[VIDBITSTREAM_MAX_BITSTREAM_BUFS];
    /**< Array of Bitstream_Buf pointers that are to given or received from the
         codec. */

    UInt32              numBufs;
    /**< Number of frames that are given or received from the codec
       i.e number of valid pointers in the array containing Bitstream_Buf
       pointers. */

    void                *appData;
    /**< Additional application parameter per buffer list */

} Bitstream_BufList;



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _VIDBITSTREAM_H_*/

/** @}*/

