/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

/**
    \file multich_ipcbits.h
    \brief Ipcbits helped functions
*/


#ifndef MULTICH_IPCBITS_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define MULTICH_IPCBITS_H_

Void MultiCh_ipcBitsInitCreateParams_BitsInHLOS(IpcBitsInLinkHLOS_CreateParams *cp);
Void MultiCh_ipcBitsInitCreateParams_BitsOutHLOS(IpcBitsOutLinkHLOS_CreateParams *cp,
                                                 System_LinkQueInfo *inQueInfo);
void MultiCh_ipcBitsInitCreateParams_SimcopBitsInHLOS(IpcBitsInLinkHLOS_CreateParams *cp);

Void MultiCh_ipcBitsInitCreateParams_BitsInRTOS(IpcBitsInLinkRTOS_CreateParams *cp,
                                                Bool notifyNextLink);
Void MultiCh_ipcBitsInitCreateParams_BitsOutRTOS(IpcBitsOutLinkRTOS_CreateParams *cp,
                                                Bool notifyPrevLink);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MULTICH_IPCBITS_H_ */
