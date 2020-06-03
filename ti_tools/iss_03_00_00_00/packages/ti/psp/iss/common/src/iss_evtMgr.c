/** ==================================================================
 *  @file   iss_evtMgr.c                                                  
 *                                                                    
 *  @path   /ti/psp/iss/common/src/                                                  
 *                                                                    
 *  @desc   This  File contains.                                      
 * ===================================================================
 *  Copyright (c) Texas Instruments Inc 2011, 2012                    
 *                                                                    
 *  Use of this software is controlled by the terms and conditions found
 *  in the license agreement under which this software has been supplied
 * ===================================================================*/

/**
 * \file iss_evtMgr.c
 *
 * \brief Event Manager file
 * This file implements Event Manager for ISS
 *
 */

/* ========================================================================== 
 */
/* Include Files */
/* ========================================================================== 
 */

#include <string.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/psp/iss/common/trace.h>
#include <ti/psp/iss/common/iss_utils.h>
// #include <ti/psp/iss/common/src/iss_intcRegOffset.h>
#include <ti/psp/iss/common/iss_evtMgr.h>
// #include <ti/psp/cslr/cslr_hd_vps.h>
#include <ti/sysbios/knl/Clock.h>

/* ========================================================================== 
 */
/* Macros & Typedefs */
/* ========================================================================== 
 */

#define IemTrace 1

#define IEM_MAX_LOG             (200)

/**
 *  \brief Semaphore time out value
 */
#define IEM_SEM_TIMEOUT                     (BIOS_WAIT_FOREVER)

#define IEM_REGSIZE_SHIFT                   (5u)

/**
 *  Maximum number of clients for Event Manager
 */
#define IEM_MAX_CLIENTS                     (50u)
#define IEM_MAX_CLIENT_EVENTS               (10u)

/**
 *  \brief Flags used by clientinfo structures memory pool manager to
 *  indicate availability of pool memory
 */
#define IEM_MEM_FLAG_FREE                   (0xFFFFFFFFu)
#define IEM_MEM_FLAG_ALLOC                  (0x00000000u)

#define regw(reg, val)      ((*(volatile UInt32 *)(reg)) = (val))
#define regr(reg)           (*(volatile UInt32 *)(reg))

/* ========================================================================== 
 */
/* Structure Declarations */
/* ========================================================================== 
 */

/**
 *  struct Iem_EventGroupInfo
 *  \brief Structure to keep track of the information related to a group. One
 *  instance of eventGroupinfo will be used for list of events of single group
 *  stored in a single register. It keeps bit position where event group starts
 *  in the register, difference between bit position of two events and registers
 *  to set event, clear event, enable event, disable event.
 */
typedef struct {
    UInt32 maxEvents;
    /**< Maximum number of events in this event group */
    UInt32 startBit;
    /**< Location of the event group within the register */
    UInt32 bitOffset;
    /**< Defines the offset between two consecutive events within the
         register */
    UInt32 statusReg;
    UInt32 vpdmaStatClrReg;
    UInt32 dssStatClrReg;

    UInt32 vpdmaEnableReg;
    UInt32 dssEnableReg;
    UInt32 vpdmaDisableReg;
    UInt32 dssDisableReg;

    UInt32 numClients;
    UInt32 isDssOnlyIntr;
} Iem_EventGroupInfo;

/**
 * struct Iem_EventManager
 * \brief structure to keep track of information regarding
 * event manager.
 */
typedef struct {
    Iem_EventGroupInfo egInfo[IEM_EG_MAX];
    /**< Pointer to the event group structure */
    VpsUtils_Node *headNode;
    /**< Pointer to the head node of the priority linked list */
    Semaphore_Handle sem;
    /**< Semaphore handle */
    Hwi_Handle hwiHandle;
    /**< Handle to Hardware Interrupt */
    UInt32 numIntr;
    /**< Total number of interrupts occured since last init */

    UInt32 numClient[IEM_MAX_LOG];
    UInt32 timestamp[IEM_MAX_LOG];
    UInt32 dssRegStatBefore[IEM_MAX_LOG];
    UInt32 vpdmaRegStatBefore[IEM_MAX_LOG];
    UInt32 dssRegStatAfter[IEM_MAX_LOG];
    UInt32 vpdmaRegStatAfter[IEM_MAX_LOG];
} Iem_EventManager;

/**
 *  struct Iem_ClientInfo
 *  \brief This structure keeps track of the client information.
 *  It will be filled up when client registers for the event callback to the
 *  event manager. Client can register one or multiple events of a single event
 *  group with a callback. So if any of one or multiple events occurs, event
 *  manager calls the callback function of the client. For each client, this
 *  structure keeps track of register to be checked for the event, register in
 *  which event is to cleared. It also keeps track of eventGroup, priority,
 *  client callback etc.
 *
 *  rawSetReg - Pointer to array containing register addresses in which event
 *               manager will check for the event. Size of this array will be
 *               the number of event for which client has registered the
 *               callback.
 *  rawClearReg - Pointer to array containing register addresses in which event
 *                manager will clear the event. Size of this array will be
 *                the number of event for which client has registered the
 *                callback.
 */
typedef struct {
    UInt32 statusReg[IEM_MAX_CLIENT_EVENTS];
    UInt32 vpdmaStatClrReg[IEM_MAX_CLIENT_EVENTS];
    UInt32 dssStatClrReg[IEM_MAX_CLIENT_EVENTS];
    UInt32 statMask[IEM_MAX_CLIENT_EVENTS];
    UInt32 dssMask[IEM_MAX_CLIENT_EVENTS];
    UInt32 vpdmaMask[IEM_MAX_CLIENT_EVENTS];
    UInt32 isDssOnlyIntr[IEM_MAX_CLIENT_EVENTS];
    /**< Pointer to the array containing the masks using which event in
         the rawSetReg will be checked or will be cleared in the rawClearReg */
    Iem_ClientCbFxn clientCb;
    /**< Client's callback function. This function will be called by the
         event manager whenever client registered event occurs */
    Ptr arg;
    /**< client's private data to be passed to it when callback function is
         called */
    Iem_EventGroup eventGroup;
    /**< This tells the event group into which registered event belongs */
    UInt32 allEvents[IEM_MAX_CLIENT_EVENTS];
    /**< List of events for which client has registered callback */
    UInt32 setEvents[IEM_MAX_CLIENT_EVENTS];
    /**< This array contains the list of events occurred at the time of
         interrupt. It will be passed down to the clients.*/
    UInt32 numEvents;
    /**< Count of events for client has registered callback. */
    Iem_Priority priority;
    /**< Priority of the client */
    Iem_EventManager *parent;
    /**< Pointer to the instance of the event manager to which this client
         is registered. */
    UInt32 numIntr;
    /**< Keeps track of number of interrupts occured for this client */

    UInt32 timestamp[IEM_MAX_LOG];
} Iem_ClientInfo;

/* ========================================================================== 
 */
/* Function Declarations */
/* ========================================================================== 
 */

/* ===================================================================
 *  @func     vemFillClientInfo                                               
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
static Void vemFillClientInfo(Iem_ClientInfo * clientInfo,
                              Iem_EventGroup eventGroup,
                              const UInt32 * event,
                              UInt32 numEvents,
                              Iem_Priority priority,
                              Iem_ClientCbFxn callback, Void * arg);

/* ===================================================================
 *  @func     vemDeleteClientInfo                                               
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
static Void vemDeleteClientInfo(Iem_ClientInfo * clientInfo);

/* ===================================================================
 *  @func     vemCreateClientInfo                                               
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
static Iem_ClientInfo *vemCreateClientInfo(VpsUtils_Node ** node);

/* ===================================================================
 *  @func     vemDeleteNode                                               
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
static Void vemDeleteNode(VpsUtils_Node * emNode);

/* ===================================================================
 *  @func     vemAddNode                                               
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
static Void vemAddNode(VpsUtils_Node * newNode, Iem_Priority priority);

/* ===================================================================
 *  @func     Iem_masterISR                                               
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
static void Iem_masterISR(UArg arg);

#ifdef __DEBUG__
/* ===================================================================
 *  @func     vemPrintList                                               
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
static Void vemPrintList(Iem_EventManager * em);
#endif

/* ========================================================================== 
 */
/* Global Variables */
/* ========================================================================== 
 */

/**
 *  \brief Array group info structure to keep track of group information. One
 *  instance of eventGroupinfo will be used for list of events of single group
 *  stored in a single register.
 */
Iem_EventManager gIem_obj = {
    {
     /* {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL0_INT_STAT, VPDMA_INT_CHANNEL0_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL0_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL0_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL1_INT_STAT, VPDMA_INT_CHANNEL1_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL1_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL1_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL2_INT_STAT, VPDMA_INT_CHANNEL2_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL2_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL2_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL3_INT_STAT, VPDMA_INT_CHANNEL3_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL3_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL3_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL4_INT_STAT, VPDMA_INT_CHANNEL4_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL4_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL4_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL5_INT_STAT, VPDMA_INT_CHANNEL5_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL5_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL5_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE},
      * {ISSHAL_VPDMA_CHANNEL_NUM_CHANNELS, 0, 1,
      * VPDMA_INT_CHANNEL6_INT_STAT, VPDMA_INT_CHANNEL6_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CHANNEL6_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CHANNEL6_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE}, {ISSHAL_VPDMA_MAX_CLIENTS, 0,
      * 1, VPDMA_INT_CLIENT0_INT_STAT, VPDMA_INT_CLIENT0_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CLIENT0_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CLIENT0_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE}, {ISSHAL_VPDMA_MAX_CLIENTS, 0,
      * 1, VPDMA_INT_CLIENT1_INT_STAT, VPDMA_INT_CLIENT1_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG1, VPDMA_INT_CLIENT1_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG1, VPDMA_INT_CLIENT1_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG1, 0u, FALSE}, {ISSHAL_VPDMA_MAX_LIST, 0, 2,
      * VPDMA_INT_LIST0_INT_STAT, VPDMA_INT_LIST0_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG0, 0u, FALSE}, {ISSHAL_VPDMA_MAX_LIST, 1, 2,
      * VPDMA_INT_LIST0_INT_STAT, VPDMA_INT_LIST0_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLE_CLR_REG0, 0, FALSE}, {ISSHAL_VPDMA_MAX_SI_SOURCE, 16, 
      * 1, VPDMA_INT_LIST0_INT_STAT, VPDMA_INT_LIST0_INT_STAT,
      * ISS_INTC_ENABLED_CLR_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLE_SET_REG0, VPDMA_INT_LIST0_INT_MASK,
      * ISS_INTC_ENABLED_CLR_REG0, 0u, FALSE}, {IEM_EE_MAX, 16, 1,
      * ISS_INTC_ENABLED_CLR_REG1, 0u, ISS_INTC_ENABLED_CLR_REG1, 0u,
      * ISS_INTC_ENABLE_SET_REG1, 0u, ISS_INTC_ENABLE_CLR_REG1, 0u, TRUE},
      * {IEM_OE_MAX, 17, 2, ISS_INTC_ENABLED_CLR_REG0, 0u,
      * ISS_INTC_ENABLED_CLR_REG0, 0u, ISS_INTC_ENABLE_SET_REG0, 0u,
      * ISS_INTC_ENABLE_CLR_REG0, 0u, TRUE} */
     },
    NULL,
    NULL,
    NULL
};

/**
 *  \brief Pool of memory for the clientinfo structure and flags for each
 *  instance to indicate whether it is free or allocated
 */
Iem_ClientInfo gIem_clientInfoMemPool[IEM_MAX_CLIENTS];

UInt32 gIem_clientInfoFlag[IEM_MAX_CLIENTS];

/**
 *  \brief Pool of memory for the Node
 */
static VpsUtils_Node IemNode[IEM_MAX_CLIENTS];

/* ========================================================================== 
 */
/* Function Definitions */
/* ========================================================================== 
 */

/**
 *  \brief Initializes the ISS Event Manager. It marks all the client info
 *  instances in the memory as available and register interrupt handler
 */
/* ===================================================================
 *  @func     Iem_init                                               
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
Int32 Iem_init(Ptr arg)
{
    Int32 retVal = 0;

    UInt32 cnt;

    Semaphore_Params semParams;
    Hwi_Params hwiParams = { 0 };

    /* Init node pool and mark flags as free */
    VpsUtils_memset(gIem_clientInfoMemPool,
                    (UInt8) 0, sizeof(gIem_clientInfoMemPool));
    for (cnt = 0u; cnt < IEM_MAX_CLIENTS; cnt++)
    {
        gIem_clientInfoFlag[cnt] = IEM_MEM_FLAG_FREE;
    }

    /* Initialise the semaphore parameters and create Pool Semaphore */
    Semaphore_Params_init(&semParams);
    gIem_obj.sem = Semaphore_create(1u, &semParams, NULL);
    if (NULL == gIem_obj.sem)
    {
        retVal = -1;
    }

    /* Clear out any pending interrupts */
    Hwi_clearInterrupt(IEM_IRQNUMBER);

    /* Clear out pending interrupts at DSS and VPDMA Level and disable all
     * interrupts */
    for (cnt = 0u; cnt < IEM_EG_MAX; cnt++)
    {
        /* Disable Interrupts at the DSS and VPDMA level */
        if (FALSE == gIem_obj.egInfo[cnt].isDssOnlyIntr)
        {
            regw(gIem_obj.egInfo[cnt].vpdmaDisableReg, 0x0u);
            /* Clean the pending interrupts */
            regw(gIem_obj.egInfo[cnt].vpdmaStatClrReg, 0xFFFFFFFFu);
        }

        regw(gIem_obj.egInfo[cnt].dssDisableReg, 0xFFFFFFFFu);

        /* Clean the pending interrupts */
        regw(gIem_obj.egInfo[cnt].dssStatClrReg, 0xFFFFFFFFu);
    }

    gIem_obj.headNode = NULL;
    gIem_obj.numIntr = 0u;
    memset(gIem_obj.timestamp, 0, sizeof(gIem_obj.timestamp));
    memset(gIem_obj.numClient, 0, sizeof(gIem_obj.numClient));
    memset(gIem_obj.dssRegStatBefore, 0, sizeof(gIem_obj.dssRegStatBefore));
    memset(gIem_obj.vpdmaRegStatBefore, 0, sizeof(gIem_obj.vpdmaRegStatBefore));
    memset(gIem_obj.dssRegStatAfter, 0, sizeof(gIem_obj.dssRegStatAfter));
    memset(gIem_obj.vpdmaRegStatAfter, 0, sizeof(gIem_obj.vpdmaRegStatAfter));

    /* ----------------- Register ISR --------------- */
    Hwi_Params_init(&hwiParams);
    gIem_obj.hwiHandle =
        Hwi_create(IEM_IRQNUMBER, Iem_masterISR, &hwiParams, 0);
    if (NULL == gIem_obj.hwiHandle)
    {
        GT_1trace(IemTrace, GT_ERR, "%s: Cannot Register IRQ", __FUNCTION__);
        retVal = -1;
    }

    return retVal;
}

/**
 *  \brief De-Initializes event Manager. It deletes semaphore and un-registers
 *  the IRQ handle and removes all the client info instance from the linked
 *  list.
 */
/* ===================================================================
 *  @func     Iem_deInit                                               
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
Int32 Iem_deInit(Ptr arg)
{
    VpsUtils_Node *tempNode = NULL, *emNode = NULL;

    /* ------------- Un-register ISR ------------- */
    Hwi_delete(&(gIem_obj.hwiHandle));

    Semaphore_delete(&(gIem_obj.sem));

    /* Free Entire Linked List */
    tempNode = gIem_obj.headNode;
    while (NULL != tempNode)
    {
        emNode = tempNode;
        tempNode = tempNode->next;
        vemDeleteClientInfo((Iem_ClientInfo *) emNode->data);
        vemDeleteNode(emNode);
    }
    return (0);
}

/**
 *  \brief Master ISR function. This is interrupt handler registered for the
 *  interrupt. When called, it will search for all events of all
 *  clients to see if it has occurred. If any event is occurred, it will
 *  clear that event and call the callback function of the client.
 */
/* ===================================================================
 *  @func     Iem_masterISR                                               
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
static void Iem_masterISR(UArg arg)
{
    VpsUtils_Node *tempNode = NULL;

    Iem_ClientInfo *clientInfo = NULL;

    Iem_EventManager *em = NULL;

    UInt32 regValue, numEvents;

    UInt32 cnt;

    UInt32 numClients = 0;

    GT_1trace(IemTrace,
              GT_INFO, "%s: At last interrupt occurred!!!!", __FUNCTION__);

    em = (Iem_EventManager *) & gIem_obj;
    tempNode = em->headNode;

    em->timestamp[em->numIntr % IEM_MAX_LOG] = Clock_getTicks();

    // em->dssRegStatBefore[em->numIntr%IEM_MAX_LOG] =
    // regr(ISS_INTC_ENABLED_CLR_REG0);
    // em->vpdmaRegStatBefore[em->numIntr%IEM_MAX_LOG] =
    // regr(VPDMA_INT_LIST0_INT_STAT);

    /* Traverse through the entire list of registered clients */
    while (NULL != tempNode)
    {
        numEvents = 0u;
        clientInfo = (Iem_ClientInfo *) tempNode->data;
        GT_assert(IemTrace, (NULL != clientInfo));
        for (cnt = 0; cnt < clientInfo->numEvents; cnt++)
        {
            /* See if any of registered events has occurred */
            regValue = regr(clientInfo->statusReg[cnt]);
            GT_3trace(IemTrace,
                      GT_INFO,
                      "%s: RAW Reg %x Value %x",
                      __FUNCTION__, clientInfo->statusReg[cnt], regValue);
            if (0u != (regValue & (clientInfo->statMask[cnt])))
            {

                /* Make an array of events which are occurred. */
                clientInfo->setEvents[numEvents++] = clientInfo->allEvents[cnt];
                /* Clear the event */
                GT_3trace(IemTrace,
                          GT_INFO,
                          "%s: statusClearReg %x Mask %x",
                          __FUNCTION__,
                          clientInfo->vpdmaStatClrReg[cnt],
                          clientInfo->vpdmaMask[cnt]);
                GT_3trace(IemTrace,
                          GT_INFO,
                          "%s: secStatusClearReg %x secMask %x",
                          __FUNCTION__,
                          clientInfo->dssStatClrReg[cnt],
                          clientInfo->dssMask[cnt]);

                regw(clientInfo->dssStatClrReg[cnt], clientInfo->dssMask[cnt]);
                if (FALSE == clientInfo->isDssOnlyIntr[cnt])
                {
                    regw(clientInfo->vpdmaStatClrReg[cnt],
                         clientInfo->vpdmaMask[cnt]);
                }
            }
        }

        /* Call callback if any event is occurred. */
        if (0u != numEvents)
        {
            clientInfo->clientCb(clientInfo->setEvents,
                                 numEvents, clientInfo->arg);
            /* Increment total number of interrupts */

            clientInfo->timestamp[clientInfo->numIntr % IEM_MAX_LOG] =
                Clock_getTicks();
            clientInfo->numIntr++;

            numClients++;
        }

        /* Move to the next client */
        tempNode = tempNode->next;
    }

    // em->dssRegStatAfter[em->numIntr%IEM_MAX_LOG] =
    // regr(ISS_INTC_ENABLED_CLR_REG0);
    // em->vpdmaRegStatAfter[em->numIntr%IEM_MAX_LOG] =
    // regr(VPDMA_INT_LIST0_INT_STAT);

    // regw(ISS_INTC_EOI_ADDR, ISS_INTC_NUM);

    em->numClient[em->numIntr % IEM_MAX_LOG] = numClients;

    /* Increment total number of interrupts */
    em->numIntr++;

    return;
}

/**
 *  \brief This function is used to register call back function for a specific
 *  event. It takes group number and event number within the group as the
 *  argument and returns handle.
 */
/* ===================================================================
 *  @func     Iem_register                                               
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
Void *Iem_register(Iem_EventGroup eventGroup,
                   const UInt32 * event,
                   UInt32 numEvents,
                   Iem_Priority priority, Iem_ClientCbFxn callback, Void * arg)
{
    UInt32 cnt;

    VpsUtils_Node *newNode = NULL;

    Iem_ClientInfo *clientInfo = NULL;

    Iem_EventGroupInfo *egInfo = gIem_obj.egInfo;

    /* Check for the error conditions */
    GT_assert(IemTrace, (NULL != event));
    GT_assert(IemTrace, (0u != numEvents));
    GT_assert(IemTrace, (NULL != callback));
    GT_assert(IemTrace, (numEvents <= egInfo[eventGroup].maxEvents));

    for (cnt = 0u; cnt < numEvents; cnt++)
    {
        GT_assert(IemTrace, (event[cnt] < egInfo[eventGroup].maxEvents));
    }

    Semaphore_pend(gIem_obj.sem, (UInt32) IEM_SEM_TIMEOUT);

    /* Allocate memory for the client info structure */
    clientInfo = vemCreateClientInfo(&newNode);

    if ((NULL != clientInfo) && (NULL != newNode))
    {
        newNode->data = (Void *) clientInfo;

        clientInfo->parent = &(gIem_obj);
        clientInfo->numIntr = 0u;

        /* Fill up the details about events in the client info structure */
        vemFillClientInfo(clientInfo,
                          eventGroup,
                          event, numEvents, priority, callback, arg);

        /* Add the node in the linked list as per the priority */
        vemAddNode(newNode, priority);
    }

    Semaphore_post(gIem_obj.sem);

#ifdef __DEBUG__
    vemPrintList(clientInfo->parent);
#endif

    return ((Void *) newNode);
}

/**
 *  Int Em_unRegister(Void *handle)
 *  \brief  Function to un-register callback from the event manager. It removes
 *          node from the linked list and frees up memory allocated to the node
 *          and client info structure. It takes handle to the callback and
 *          returns status of the whether callback is removed.
 *          must be valid handle to the callback already registered
 *
 *  \param  handle      Handle to the callback
 *
 *  \return             Indicates whether callback is removed or not.
 */
/* ===================================================================
 *  @func     Iem_unRegister                                               
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
Int Iem_unRegister(Void * handle)
{
    VpsUtils_Node *tempNode = NULL, *emNode = NULL;

    Iem_ClientInfo *clientInfo = NULL;

    UInt32 cnt, regValue;

    Int index = 0, found = 0;

    Iem_EventGroupInfo *egInfo = NULL;

    Iem_EventManager *em = NULL;

    /* Error condition Checking */
    GT_assert(IemTrace, (NULL != handle));

    tempNode = (VpsUtils_Node *) handle;

    em = ((Iem_ClientInfo *) (tempNode->data))->parent;

    egInfo = em->egInfo;
    GT_assert(IemTrace, (NULL != egInfo));
    /* Check to see if the node given is in the linked list */
    emNode = em->headNode;
    GT_assert(IemTrace, (NULL != emNode));
    found = 0;
    while (NULL != emNode)
    {
        if (tempNode == emNode)
        {
            found = 1;
            break;
        }
        emNode = emNode->next;
    }
    GT_assert(IemTrace, (0 != found));

    clientInfo = (Iem_ClientInfo *) tempNode->data;
    GT_assert(IemTrace, (NULL != clientInfo));
    Semaphore_pend(em->sem, (UInt32) IEM_SEM_TIMEOUT);

    /* Delete the Node */
    vemDeleteNode(tempNode);

    /* Disable events in the ISS registers */
    for (cnt = 0u; cnt < clientInfo->numEvents; cnt++)
    {
        if ((IEM_EG_CHANNEL == clientInfo->eventGroup) ||
            (IEM_EG_CLIENT == clientInfo->eventGroup))
        {
            index = clientInfo->eventGroup + (clientInfo->allEvents[cnt] >> 5u);
        }
        else
        {
            index = (Int) clientInfo->eventGroup;
        }

        egInfo[index].numClients--;
        /* Disable the event at the VPDMA */
        if (FALSE == egInfo[index].isDssOnlyIntr)
        {
            regValue = regr(egInfo[index].vpdmaDisableReg);
            regValue &= (~clientInfo->vpdmaMask[cnt]);
            regw(egInfo[index].vpdmaDisableReg, regValue);

            /* Clear the any pending event at the VPDMA */
            /* regValue = regr(egInfo[index].vpdmaStatClrReg); regValue &=
             * (~clientInfo->vpdmaMask[cnt]); */
            regw(egInfo[index].vpdmaStatClrReg, clientInfo->vpdmaMask[cnt]);
        }

        if ((IEM_EG_LISTCOMPLETE == clientInfo->eventGroup) ||
            (IEM_EG_LISTNOTIFY == clientInfo->eventGroup))
        {
            regw(egInfo[index].dssDisableReg, clientInfo->dssMask[cnt]);

            /* Clear the any pending event at the DSS. For the ListComplete
             * and ListNotify interrupts, there are no groups in dss
             * register, so it can be cleared here */
            regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);
        }
        else if (IEM_EG_SI == clientInfo->eventGroup)
        {
            /* Disable At the DSS only if there are no clients registered */
            if (0 == egInfo[index].numClients)
            {
                regw(egInfo[index].dssDisableReg, clientInfo->dssMask[cnt]);
                /* Clear any pending status register */
                regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);
            }
        }
        else if ((IEM_EG_CHANNEL == clientInfo->eventGroup) ||
                 (IEM_EG_CLIENT == clientInfo->eventGroup))
        {
            /* Disable At the DSS only if there are no clients registered */
            if (0 == egInfo[index].numClients)
            {
                regw(egInfo[index].dssDisableReg, clientInfo->dssMask[cnt]);
                /* Clear any pending status register */
                regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);
            }
        }
        clientInfo->dssMask[cnt] = 0u;
        clientInfo->vpdmaMask[cnt] = 0u;
        clientInfo->statMask[cnt] = 0u;
    }

    /* Free up memory allocated to event arrays and clientInfo structure */
    vemDeleteClientInfo(clientInfo);

    Semaphore_post(em->sem);

#ifdef __DEBUG__
    vemPrintList(clientInfo->parent);
#endif

    return (0);
}

/**
 *  \brief Function to add a node to the linked list of clients at the
 *  appropriate position as per the given priority.
 */
/* ===================================================================
 *  @func     vemAddNode                                               
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
Void vemAddNode(VpsUtils_Node * newNode, Iem_Priority priority)
{
    UInt32 cookie;

    VpsUtils_Node *curr = NULL, *prev = NULL;

    Iem_EventManager *em = NULL;

    GT_assert(IemTrace, (NULL != newNode));

    em = ((Iem_ClientInfo *) (newNode->data))->parent;
    GT_assert(IemTrace, (NULL != em));

    newNode->prev = newNode->next = NULL;
    curr = em->headNode;
    prev = NULL;

    /* Search for place to insert new node as per priority */
    while ((NULL != curr) &&
           (((Iem_ClientInfo *) curr->data)->priority < priority))
    {
        prev = curr;
        curr = curr->next;
    }

    /* Disable Interrupt */
    cookie = Hwi_disable();

    /* Update pointers */
    newNode->next = curr;
    newNode->prev = prev;

    if (NULL == prev)
    {
        em->headNode = newNode;
    }
    else
    {
        prev->next = newNode;
    }

    if (NULL != curr)
    {
        curr->prev = newNode;
    }

    /* Enable Interrupt */
    Hwi_restore(cookie);

}

/**
 *  \brief Function to remove the node from the linked list and delete memory
 *  allocated to it.
 */
/* ===================================================================
 *  @func     vemDeleteNode                                               
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
Void vemDeleteNode(VpsUtils_Node * emNode)
{
    UInt32 cookie;

    VpsUtils_Node *curr = NULL, *prev = NULL;

    Iem_EventManager *em = NULL;

    GT_assert(IemTrace, (NULL != emNode));

    em = ((Iem_ClientInfo *) (emNode->data))->parent;
    prev = emNode->prev;
    curr = emNode->next;

    /* Disable Interrupts */
    cookie = Hwi_disable();

    if (NULL != prev)
    {
        if (NULL != curr)
        {
            prev->next = curr;
        }
        else
        {
            prev->next = NULL;
        }

    }
    else
    {
        em->headNode = curr;
        if (NULL != em->headNode)
        {
            em->headNode->prev = NULL;
        }
    }
    if (NULL != curr)
    {
        curr->prev = prev;
    }

    /* Enable Interrupts */
    Hwi_restore(cookie);
}

/**
 *  \brief Function to allocate memory for the client information structure
 *  and other arrays.
 */
/* ===================================================================
 *  @func     vemCreateClientInfo                                               
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
Iem_ClientInfo *vemCreateClientInfo(VpsUtils_Node ** node)
{
    Iem_ClientInfo *clientInfo = NULL;

    UInt32 cnt;

    for (cnt = 0u; cnt < IEM_MAX_CLIENTS; cnt++)
    {
        if (IEM_MEM_FLAG_FREE == gIem_clientInfoFlag[cnt])
        {
            clientInfo = &(gIem_clientInfoMemPool[cnt]);
            *node = &IemNode[cnt];
            gIem_clientInfoFlag[cnt] = IEM_MEM_FLAG_ALLOC;
            break;
        }
    }

    return (clientInfo);
}

/**
 *  \brief Function to free up memory for the client information structure
 *  and other arrays.
 */
/* ===================================================================
 *  @func     vemDeleteClientInfo                                               
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
static Void vemDeleteClientInfo(Iem_ClientInfo * clientInfo)
{
    UInt32 cnt;

    GT_assert(IemTrace, (NULL != clientInfo));

    for (cnt = 0u; cnt < IEM_MAX_CLIENTS; cnt++)
    {
        if (clientInfo == &(gIem_clientInfoMemPool[cnt]))
        {
            gIem_clientInfoFlag[cnt] = IEM_MEM_FLAG_FREE;
            break;
        }
    }
}

/**
 *  \brief Function to fill up client information structure.
 */
/* ===================================================================
 *  @func     vemFillClientInfo                                               
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
Void vemFillClientInfo(Iem_ClientInfo * clientInfo,
                       Iem_EventGroup eventGroup,
                       const UInt32 * event,
                       UInt32 numEvents,
                       Iem_Priority priority,
                       Iem_ClientCbFxn callback, Void * arg)
{
    UInt32 cnt, index, eventOffset;

    UInt32 regValue;

    Iem_EventGroupInfo *egInfo = clientInfo->parent->egInfo;

    GT_assert(IemTrace, (NULL != clientInfo));
    GT_assert(IemTrace, (NULL != event));
    GT_assert(IemTrace, (NULL != callback));
    GT_assert(IemTrace, (NULL != egInfo));

    clientInfo->eventGroup = eventGroup;
    clientInfo->numEvents = numEvents;
    for (cnt = 0u; cnt < numEvents; cnt++)
    {
        if ((IEM_EG_CHANNEL == eventGroup) || (IEM_EG_CLIENT == eventGroup))
        {
            index = (UInt32) eventGroup + (event[cnt] >> IEM_REGSIZE_SHIFT);
            eventOffset = event[cnt] & ((1u << IEM_REGSIZE_SHIFT) - 1u);
        }
        else
        {
            index = (UInt32) eventGroup;
            eventOffset = event[cnt];
        }
        clientInfo->allEvents[cnt] = event[cnt];
        /* Store the registers from which event status can be checked or into
         * which event status can be cleared */
        clientInfo->statusReg[cnt] = egInfo[index].statusReg;
        clientInfo->vpdmaStatClrReg[cnt] = egInfo[index].vpdmaStatClrReg;
        clientInfo->dssStatClrReg[cnt] = egInfo[index].dssStatClrReg;

        clientInfo->statMask[cnt] = 1 << ((eventOffset *
                                           egInfo[index].bitOffset) +
                                          egInfo[index].startBit);

        /* Enable the event interrupt in the enable set register */
        GT_3trace(IemTrace,
                  GT_INFO,
                  "%s: Enable Reg %x mask %x",
                  __FUNCTION__,
                  egInfo[index].vpdmaEnableReg, clientInfo->statMask[cnt]);

        if ((IEM_EG_LISTCOMPLETE == eventGroup) ||
            (IEM_EG_LISTNOTIFY == eventGroup))
        {
            /* Since the list complete and list notify are at the same
             * position in DSS as well in VPDMA, second mask is same as the
             * first mask */
            clientInfo->dssMask[cnt] = clientInfo->statMask[cnt];

            GT_3trace(IemTrace,
                      GT_INFO,
                      "%s: Enable Reg %x mask %x",
                      __FUNCTION__,
                      egInfo[index].dssEnableReg, clientInfo->dssMask[cnt]);

            /* Clean any pending interrupts on this event */
            regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);

            /* Enable the event interrupt in the enable set register */
            /* DSS registers can written directly as writing 0 has no effect */
            regw(egInfo[index].dssEnableReg, clientInfo->dssMask[cnt]);
        }
        else if ((IEM_EG_SI == eventGroup))
        {
            clientInfo->dssMask[cnt] = 1u << egInfo[index].startBit;

            /* Clean any pending interrupts on this event */
            regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);

            /* Enable interrupt */
            regw(egInfo[index].dssEnableReg, clientInfo->dssMask[cnt]);
            GT_4trace(IemTrace,
                      GT_INFO,
                      "%s: Enable Reg %x Value %x RegValue %x",
                      __FUNCTION__,
                      egInfo[index].dssEnableReg,
                      regValue, regr(egInfo[index].dssEnableReg));
        }
        else if ((IEM_EG_CHANNEL == eventGroup) ||
                 (IEM_EG_CLIENT == eventGroup))
        {
            clientInfo->dssMask[cnt] = (1u << (UInt32) eventGroup);

            /* Clean any pending interrupts on this event */
            regw(egInfo[index].dssStatClrReg, clientInfo->dssMask[cnt]);

            /* Enable interrupt */
            regw(egInfo[index].dssEnableReg, clientInfo->dssMask[cnt]);
            GT_4trace(IemTrace,
                      GT_INFO,
                      "%s: Enable Reg %x Value %x RegValue %x",
                      __FUNCTION__,
                      egInfo[index].dssEnableReg,
                      regValue, regr(egInfo[index].dssEnableReg));
        }
        /* Enable Event at the Vpdma Level */
        if (FALSE == egInfo[index].isDssOnlyIntr)
        {
            clientInfo->vpdmaMask[cnt] = clientInfo->statMask[cnt];

            /* Clean any pending interrupts on this event */
            regw(egInfo[index].vpdmaStatClrReg, clientInfo->vpdmaMask[cnt]);

            /* VPDMA interrupt registers are read-modify-write registers */
            regValue = regr(egInfo[index].vpdmaEnableReg);
            regValue |= clientInfo->vpdmaMask[cnt];

            /* Enable interrupt */
            regw(egInfo[index].vpdmaEnableReg, regValue);

            /* VPDMA mask is same as the statMask for these interrupts */
            clientInfo->vpdmaMask[cnt] = clientInfo->statMask[cnt];
        }
        GT_4trace(IemTrace,
                  GT_INFO,
                  "%s: Enable Reg %x mask %x RegValue %x",
                  __FUNCTION__,
                  egInfo[index].vpdmaEnableReg,
                  clientInfo->vpdmaMask[cnt],
                  regr(egInfo[index].vpdmaEnableReg));

        /* Increment the number of clients for this event */
        egInfo[index].numClients++;
        clientInfo->isDssOnlyIntr[cnt] = egInfo[index].isDssOnlyIntr;
    }

    clientInfo->clientCb = callback;
    clientInfo->priority = priority;

    if (NULL != arg)
    {
        clientInfo->arg = arg;
    }
    else
    {
        clientInfo->arg = NULL;
    }
}

#ifdef __DEBUG__
/* ===================================================================
 *  @func     vemPrintList                                               
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
Void vemPrintList(Iem_EventManager * em)
{
    VpsUtils_Node *tempNode = em->headNode;

    Iem_ClientInfo *clientInfo = NULL;

    UInt32 nodeNum = 0u, cnt;

    while (NULL != tempNode)
    {
        clientInfo = (Iem_ClientInfo *) tempNode->data;

        GT_2trace(IemTrace,
                  GT_INFO, "%s:Node Number %d", __FUNCTION__, nodeNum++);
        GT_2trace(IemTrace,
                  GT_INFO,
                  "%s:  Node Number %d", __FUNCTION__, clientInfo->priority);
        GT_2trace(IemTrace,
                  GT_INFO,
                  "%s:  Number of Registered Events %d",
                  __FUNCTION__, clientInfo->numEvents);
        GT_2trace(IemTrace,
                  GT_INFO,
                  "%s:  Event Group                 %d",
                  __FUNCTION__, clientInfo->eventGroup);
        for (cnt = 0u; cnt < clientInfo->numEvents; cnt++)
        {
            GT_2trace(IemTrace,
                      GT_INFO,
                      "%s:    Event Number   %d",
                      __FUNCTION__, clientInfo->allEvents[cnt]);
            GT_2trace(IemTrace,
                      GT_INFO,
                      "%s:    Raw Set Reg    %d",
                      __FUNCTION__, clientInfo->statusReg[cnt]);
            GT_2trace(IemTrace,
                      GT_INFO,
                      "%s:    Raw Clear Reg  %d",
                      __FUNCTION__, clientInfo->vpdmaStatClrReg[cnt]);
            GT_2trace(IemTrace,
                      GT_INFO,
                      "%s:    mask           %x",
                      __FUNCTION__, clientInfo->mask[cnt]);
            GT_2trace(IemTrace,
                      GT_INFO, "%s:    -----------------", __FUNCTION__);
        }
        GT_2trace(IemTrace, GT_INFO, "%s:===============\n", __FUNCTION__);
        tempNode = tempNode->next;
    }
}
#endif
