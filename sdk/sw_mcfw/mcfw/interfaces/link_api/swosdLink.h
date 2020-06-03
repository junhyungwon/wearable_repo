/** ==================================================================
 *  @file   swosdLink.h
 *
 *  @path    ipnc_mcfw/mcfw/interfaces/link_api/
 *
 *  @desc   This  File contains Swosd Link API.
 *
 * ===================================================================
 *
 *  Copyright (c) Texas Instruments Inc 2011, 2012
 *
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 *
 * ===================================================================*/

/**
    \ingroup LINK_API
    \defgroup SWOSD_LINK_API Swosd Link API

	SWOSD Link imprints the TI logo,Date/Time,Codec details and User data onto Video streams.
	Any change in the data to be imprinted is checked at an interval of Frames Per Second.

    @{
*/

/**
    \file swosdLink.h
    \brief Swosd Link API
*/

#ifndef _SWOSD_LINK_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _SWOSD_LINK_H_

#include <mcfw/interfaces/link_api/system.h>

#define SWOSDLINK_MAX_STR_LENGTH		(32)

#define SWOSDLINK_CMD_DATETIME		(0x7001)
#define SWOSDLINK_CMD_EN_DATETIME	(0x7002)
#define SWOSDLINK_CMD_GPS			(0x7003)
#define SWOSDLINK_CMD_PARK			(0x7004)
#define SWOSDLINK_CMD_EVENT			(0x7005)
#define SWOSDLINK_CMD_MOTION		(0x7006)
#define SWOSDLINK_CMD_VOLTAGE		(0x7007)
#define SWOSDLINK_CMD_TEMPERATURE	(0x7008)
#define SWOSDLINK_CMD_SPEED			(0x7009)
#define SWOSDLINK_CMD_GSENS			(0x700A)
#define SWOSDLINK_CMD_USERSTR		(0x700B)
#define SWOSDLINK_CMD_ENABLE		(0x700C)

/**
    \brief Swosd link create parameters
*/
typedef struct {
    System_LinkInQueParams inQueParams;
    /**< Input queue information */

    System_LinkOutQueParams outQueParams;
    /**< Output queue information */

    UInt32 frameSync;
    /**< Frame sync ON/OFF flag */

    UInt32 frameSycChId;
    /**< Channel ID for which frame sync is required */

} SwosdLink_CreateParams;

/**
    \brief Swosd GUI parameters
*/
typedef struct {
    UInt32 streamId;

	UInt32 enable;	//# window enable/disable
	UInt32 volt;
	UInt32 temp;
	UInt32 speed;
	Int32 x;
	Int32 y;
	Int32 z;
    UInt8 usrString[32];

} SwosdLink_GuiParams;

/**
    \brief Swosd Date Time params
*/
typedef struct {
    UInt32 year;
    /**< Year */

    UInt32 month;
    /**< Month */

    UInt32 day;
    /**< Day */

    UInt32 hour;
    /**< Hour */

    UInt32 min;
    /**< Minute */

    UInt32 sec;
    /**< Second */

} SwosdLink_dateTime;

/**
    \brief Swosd VA str params
*/
typedef struct {
    UInt32 disFlag;
	/**<display flag */

	UInt8 vaString[SWOSDLINK_MAX_STR_LENGTH];
	/**< VA string */

} SwosdLink_vaStrPrm;

/**
 *  @func     SwosdLink_init
 *
 *  @desc     Function does theSwosd link register and init
 *               - Creates link task
 *               - Registers as a link with the system API
 *
 *  @modif
 *
 *  @inputs   This function takes the following inputs
 *
 *  @outputs
 *
 *  @return   FVID2_SOK on success
 *
 */
Int32 SwosdLink_init();

/**
 *  @func     SwosdLink_deInit
 *
 *  @desc     Function does the Swosd link de-register and de-init
 *
 *             - Deletes link task
 *             - De-registers as a link with the system API
 *
 *  @modif
 *
 *  @inputs
 *
 *  @outputs
 *
 *  @return  FVID2_SOK on success
 *
 */
Int32 SwosdLink_deInit();

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/* @} */
