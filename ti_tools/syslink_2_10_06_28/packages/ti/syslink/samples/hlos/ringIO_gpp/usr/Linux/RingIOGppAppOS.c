/*
 *  @file   RingIOGppAppOS.c
 *
 *  @brief      OS-specific sample application driver module for RingIO module
 *
 *              TODO: write description
 *
 *
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */



#include <ti/syslink/Std.h>
#include "RingIOGppAppOS.h"


/** ============================================================================
 *  @brief   Function to create a new thread or a Process.
 *  ============================================================================
 */
UInt32
RingIOGppApp_Create_client (RingIOGppApp_ClientInfo * pInfo, Ptr funcPtr, Ptr args)
{
    Int  status      = 0;

#ifdef RingIOGppApp_MULTIPROCESS
    pid_t       processId;
    int         result;
    Int  retStatus   = 0;
    int         (*lptrToFun)(void*) = NULL;

#endif

#ifdef RingIOGppApp_MULTIPROCESS
    /* Create a child process */
    processId = fork();
    if (processId ==  0)  {
        /* In Child Process */

        /* Get the access privileges for the child process */
        lptrToFun = funcPtr;
        /* Call the user function */
        result =(lptrToFun) (args);

        /* Exit from the child process */
        exit (0);
    }
    else if (processId < 0) {
        status = -1;
        Osal_Printf ("Call to fork failed. Status [0x%x]\n",
                         status);
    }
    else {
        /* In parent Process */
        pInfo->pid = processId;
        status = 0;
    }

#else
    status = pthread_create (&pInfo->tid,
                             NULL,/* Attributes of the thread.*/
                             (void*) funcPtr, /* Pointer to Function.*/
                             args);
#endif

    switch(status) {
        case 0:
            return (0);
        case -1:
            return (-1);
    }

    return (status);
}

/** ============================================================================
 *  @brief   Wait for thread/process to terminate.
 *  ============================================================================
 */
Int
RingIOGppApp_Join_client (RingIOGppApp_ClientInfo *pInfo)
{
    Int  status   = -1;
#ifdef RingIOGppApp_MULTIPROCESS
    int         statLoc;
#endif

#ifdef RingIOGppApp_MULTIPROCESS
    do {
        status = waitpid (pInfo->pid,
                          &statLoc,
                          (   WSTOPPED
                            | WUNTRACED
                           ) );
        if (status < 0) {
            if (EINTR == errno){
                Osal_Printf ("Signal received in Main process !!! \n");
                Osal_Printf ("Terminate the child process \n");
                status = API_RESTART;
            }
            else {
                Osal_Printf ("Error exit from wait. Status [0x%x]\n",
                                status);
                Osal_Printf ( "errno is %d \n", errno );
                status = -1;
            }
        }
        else {
            if (WIFEXITED(statLoc) != 0) {
                /* Child process exited normally */
                status = 0;
            }
            else if (WIFSIGNALED(statLoc)) {

                /* Child exited due to uncaught signal */
                Osal_Printf (" Child process exited due to signal %d \n",
                                  WTERMSIG (statLoc));
                /* Terminate the child processes and Main process also */
                kill(0, SIGABRT);
                status = -1;

            } else {
                 Osal_Printf (" Child process exited due to status 0%x \n",
                                   status);
            }
        }

    } while (status == API_RESTART);

#else
    status = pthread_join (pInfo->tid, NULL);
#endif

    if (status != 0) {
        status = -1;
    }
    else {
        status = 0;
    }
    return (status);
}

/** ============================================================================
 *  @brief   Function to exit from a caller task(thread/process).
 *
 *  ============================================================================
 */
Int
RingIOGppApp_Exit_client (RingIOGppApp_ClientInfo *pInfo)
{
    Int status   = 0;

#ifdef RingIOGppApp_MULTIPROCESS
    exit (0);
#else
    pthread_exit(NULL);
#endif

    return (status);
}

/** ============================================================================
 *  @brief   Converts ascii to long int
 *  ============================================================================
 */
UInt32
RingIOGppApp_Atoll (Char * str)
{
     UInt32 val = 0;
     val = strtoll (str, NULL, 16);
     return val;
}

