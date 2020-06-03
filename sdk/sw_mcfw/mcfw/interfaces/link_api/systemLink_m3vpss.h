/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
    \ingroup LINK_API
    \defgroup VPSSM3_LINK_API VPSS M3 Link API

    @{
*/

/**
    \file systemLink_m3vpss.h
    \brief VPSS M3 Link API
*/

#ifndef _SYSTEM_M3VPSS_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define _SYSTEM_M3VPSS_H_

#include <mcfw/interfaces/link_api/system.h>
#include <mcfw/interfaces/link_api/systemLink_common.h>


/*
    \param NONE
*/
#define SYSTEM_M3VPSS_CMD_RESET_VIDEO_DEVICES       (0xA000)


/*
    \param [OUT] SystemVpss_PlatformInfo *
*/
#define SYSTEM_M3VPSS_CMD_GET_PLATFORM_INFO         (0xA001)


/*
    \param [IN] SystemVpss_DisplayCtrlInitParam *
*/
#define SYSTEM_M3VPSS_CMD_GET_DISPLAYCTRL_INIT      (0xA002)

/*
    \param NONE
*/
#define SYSTEM_M3VPSS_CMD_GET_DISPLAYCTRL_DEINIT    (0xA003)


/*
    \param [IN] SystemVpss_AvSyncSharedObjPtr *
*/
#define SYSTEM_M3VPSS_CMD_GET_AVSYNC_SHAREDOBJ_PTR	(0xA005)


#define SYSTEM_M3VPSS_CMD_CMD_SYNC_DATETIME			(0xA006)
#define SYSTEM_M3VPSS_CMD_GET_DEVICE_STATUS			(0xA007)

typedef struct
{
    UInt32 cpuRev;
    UInt32 boardId;
    UInt32 baseBoardRev;
    UInt32 dcBoardRev;

} SystemVpss_PlatformInfo;



typedef struct {

    UInt32 grpxId;
    Bool   enable;

} SystemVpss_GrpxEnable;

typedef struct {

    Void *pAvSyncSharedObjPtr;

} SystemVpss_AvSyncSharedObjPtr;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/* @} */
