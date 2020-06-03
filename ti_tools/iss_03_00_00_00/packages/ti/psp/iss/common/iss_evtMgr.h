/** ==================================================================
 *  @file   iss_evtMgr.h                                                  
 *                                                                    
 *  @path   /ti/psp/iss/common/                                                  
 *                                                                    
 *  @desc   This  File contains.                                      
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012                    
 *                                                                    
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/**
 * \file   iss_evtMgr.h
 *
 * \brief  Event Manager Header File.
 *
 * This file contains Data Structures and Functions definitions for
 * Event Manager. It exposes these API and data structures to its clients.
 *
 */

#ifndef _ISS_EVTMGR_H
#define _ISS_EVTMGR_H

/* ========================================================================== 
 */
/* Include Files */
/* ========================================================================== 
 */

#include <ti/psp/iss/iss.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* ========================================================================== 
     */
    /* Macros & Typedefs */
    /* ========================================================================== 
     */

    /* Macro for the default priority. Default priority is the medium one. */
#define IEM_DEFAULT_PRIORITY     (IEM_PRIORITY2)

    /* ISSS M3 IRQ */
#define IEM_IRQNUMBER            (61)

/**
 *  enum Iem_EventGroup
 *  \brief Events are grouped into multiple groups. These
 *  groups are channels interrupts, client interrupts, list complete interrupts,
 *  list notify interrupts, send interrupts sources, error interrupts and other
 *  interrupts. This enum data type defines the event groups. This enum data
 *  type is type casted to unsigned int so it must not have any negative value
 */
    typedef enum {
        IEM_EG_CHANNEL = 0,
    /**< Event Group for Channel Interrupts */
        IEM_EG_CLIENT = 7,
    /**< Event Group for Client Interrupts */
        IEM_EG_LISTCOMPLETE = 9,
    /**< Event group for List complete interrupt */
        IEM_EG_LISTNOTIFY = 10,
    /**< Event group for List notify interrupt */
        IEM_EG_SI = 11,
    /**< Event group for Send interrupt */
        IEM_EG_ERROR = 12,
    /**< Event group for Error interrupt */
        IEM_EG_OTHERS = 13,
    /**< Event group for the other interrupts not included in above interrupts
         like FMD interrupt etc. */
        IEM_EG_MAX = 14
    } Iem_EventGroup;

/**
 *  enum Iem_ErrorEvent
 *  \brief This enum defines the error interrupts from the ISS
 *  sub modules. ISS specs defines many error events but there are not register
 *  bits available in the register so these error events are commented out here.
 *  They will be uncommented when register bits for them will be added in the
 *  ISS main specs. This enum data type defines the event groups. This enum data
 *  type is type casted to unsigned int so it must not have any negative
 *  value.
 */
    typedef enum {
    /**< Error interrupt from DEI indicating framing error between input video
         data and expected framing set */
        IEM_EE_DEI = 0,

    /**< Following three error interrupts from graphics pipelines indicates
         error condition has been found in the setup of GRPX module */
        IEM_EE_GRPX0,
        IEM_EE_GRPX1,
        IEM_EE_GRPX2,

    /**< Error interrupt from COMP module indicating error condition has been
         found in the setup of COMP module */
        IEM_EE_COMP,

    /**< Following two Error interrupt from NF CHR_DS indicating
         error condition has been found in the setup of NF CHR_DS module */
        /* IEM_EE_NF_CHR_DS_LUMA, */
        IEM_EE_NF_CHR_DS_CHROMA,

        /* IEM_EE_VIP0_CHA_OVERFLOW, */
        /* IEM_EE_VIP0_CHB_OVERFLOW, */
        /* IEM_EE_VIP0_CHR_DS0_LUMA, */
        IEM_EE_VIP0_CHR_DS0_CHROMA,
        /* IEM_EE_VIP0_CHR_DS1_LUMA, */
        IEM_EE_VIP0_CHR_DS1_CHROMA,

        /* IEM_EE_VIP1_CHA_OVERFLOW, */
        /* IEM_EE_VIP1_CHB_OVERFLOW, */
        /* IEM_EE_VIP1_CHR_DS0_LUMA, */
        IEM_EE_VIP1_CHR_DS0_CHROMA,
        /* IEM_EE_VIP1_CHR_DS1_LUMA, */
        IEM_EE_VIP1_CHR_DS1_CHROMA,

        IEM_EE_MAX
    } Iem_ErrorEvent;

/**
 *  enum Iem_OtherEvent
 *  \brief There are specific interrupts from the ISS
 *  sub-modules. This enum data type defines the event groups. This enum data
 *  type is type casted to unsigned int so it must not have any negative
 *  value
 */
    typedef enum {
#ifdef TI_816X_BUILD
    /**< Film Mode Detection interrupt from DEI_HQ */
        IEM_OE_FMD_DEIHQ = 0u,
#endif
    /**< Film Mode Detection interrupt from DEI */
        IEM_OE_FMD_DEI = 1u,
    /**< CPROC Histogram status ready interrupt */
        IEM_OE_CPROC,
    /**< VIP0 format change interrupt */
        IEM_OE_VIP0,
    /**< VIP1 format change interrupt */
        IEM_OE_VIP1,
    /**< DVO1 Start of vertical blanking interval interrupt */
        IEM_OE_HDMI_VBI_START,
    /**< DVO1 End of vertical blanking interval interrupt */
        IEM_OE_HDMI_VBI_END,
    /**< DVO1 Start of frame interrupt */
        IEM_OE_HDMI_FRAME_START,
    /**< HDCOMP Start of vertical blanking interval interrupt */
        IEM_OE_HDCOMP_VBI_START,
    /**< HDCOMP End of vertical blanking interval interrupt */
        IEM_OE_HDCOMP_VBI_END,
    /**< HDCOMP Start of frame interrupt */
        IEM_OE_HDCOMP_FRAME_START,
    /**< DVO2 Start of vertical blanking interval interrupt */
        IEM_OE_DVO2_VBI_START,
    /**< DVO2 End of vertical blanking interval interrupt */
        IEM_OE_DVO2_VBI_END,
    /**< DVO2 Start of frame interrupt */
        IEM_OE_DVO2_FRAME_START,
    /**< SDVENC VSYNC interrupt */
        IEM_OE_SDVENC_VSYNC,

        IEM_OE_MAX
    } Iem_OtherEvent;

/**
 *  enum Iem_Priority
 *  \brief enum for specifying the priority of the client. Event
 *  Manager always calls the callback functions of the clients in the order of
 *  priority so that highest priority callback will get executed much earlier.
 *  When client registers itself to the event manager, it also specifies the
 *  priority of itself. event manager puts the client in the appropriate
 *  location in the linked list as per the given priority. This enum data
 *  type defines the event groups. This enum data type is type casted to
 *  unsigned int so it must not have any negative value
 * */
    typedef enum {
    /**< Highest */
        IEM_PRIORITY0 = 0u,
        IEM_PRIORITY1,
        IEM_PRIORITY2,
        IEM_PRIORITY3,
    /**< Lowest */
        IEM_PRIORITY4
    } Iem_Priority;

/**
 *  \brief This typedef defines the ISS Event manager callback function
 *  prototype. Client registers this callback with the event manager and when
 *  event occurs,
 *  event manager calls this callback.
 *
 *  \param  event       This is an array of events occurred when interrupt came.
 *                      Event manager passes this list to the client.
 *  \param  numEvents   This tells number of events occurred.
 *  \param  arg         Client's Private Data
 */
    typedef Void(*Iem_ClientCbFxn) (const UInt32 * event,
                                    UInt32 numEvents, Ptr arg);

    /* ========================================================================== 
     */
    /* Structure Declarations */
    /* ========================================================================== 
     */

    /* None */

    /* ========================================================================== 
     */
    /* Function Declarations */
    /* ========================================================================== 
     */

/**
 *  Iem_init
 *  \brief This function initializes event manager. It creates the semaphore,
 *  initializes pointers and sets the memory to zero. It also register ISR
 *  for the ISS interrupt. It returns pass or
 *  fail based on initialization passed or failed
 *
 *  \param arg            Not used currently. For the future use.
 *  \return               Returns 0 on success else returns error value
 */
    Int Iem_init(Ptr arg);

/**
 *  Iem_deInit
 *  \brief This function de-initializes the event manager.
 *
 *  \param arg            Not used currently. For the future use.
 *  \return               Returns 0 on success else returns error value
 */
    Int Iem_deInit(Ptr arg);

/**
 *  Iem_register
 *  \brief Function to register callback to the Event Manager. Client passes
 *  the set of events of a group with the callback and priority and
 *  this function adds a new node at the appropriate position as per
 *  the priority in the list of nodes. It also stores the list of
 *  events, registers for the events and masks in the the client info
 *  structure. It returns handle of the registered callback.
 *
 *  \param eventGroup       Group of the event passed in the second argument
 *  \param event            List of event for which client requested callback
 *  \param numEvents        Number of events in the second argument
 *  \param priority         Priority of this callback
 *  \param callback         Callback function of the client
 *  \param arg              Private Data of the client
 *
 *  \return                 It returns the handle of the registered callback.
 */
    Void *Iem_register(Iem_EventGroup eventGroup,
                       const UInt32 * event,
                       UInt32 numEvents,
                       Iem_Priority priority,
                       Iem_ClientCbFxn callback, Void * arg);

/**
 *  Iem_unRegister
 *  \brief Function to un-register callback from the event manager. It removes
 *  node from the linked list and frees up memory allocated to the node
 *  and client info structure. It takes handle to the callback and
 *  returns status of the whether callback is removed.
 *
 *  \param handle           Handle to the callback
 *  \return                 Returns 0 on success else returns error value
 */
    Int Iem_unRegister(Void * handle);

#ifdef __cplusplus
}
#endif
#endif                                                     /* End of #ifndef
                                                            * _ISS_EVTMGR_H */
