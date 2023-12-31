/* 
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */
/*
 *  ======== EventCombiner.xdc ========
 *
 *
 */

package ti.sysbios.family.c64p;

import xdc.runtime.Error;

/*!
 *  ======== EventCombiner ========
 *  Event Combiner Manager module
 *
 *  The event combiner allows the user to combine up to 32 system events
 *  into a single combined event.  The events 0, 1, 2, and 3 are the events
 *  associated with the event combiner.  Using the EventCombiner module
 *  along with the Hwi module, allows the user to route a combined event
 *  to any of the 12 maskable CPU interrupts available on GEM.  The
 *  EventCombiner supports up to 128 system events.  Users can specify
 *  a function and an argument for each system event and can choose to
 *  enable whichever system events they want.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center"></colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th><th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                                                                                 -->
 *    <tr><td> {@link #disableEvent}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #dispatch}        </td><td>   Y    </td><td>   N    </td><td>   N    </td><td>   N    </td><td>   N    </td></tr>
 *    <tr><td> {@link #dispatchPlug}    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td> {@link #enableEvent}     </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td><td>   Y    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started (e.g. EventCombiner_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *             <li> In your module startup before this module is started (e.g. EventCombiner_Module_startupDone() returns FALSE).</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 */

@ModuleStartup

module EventCombiner
{
    /*!
     *  ======== EventView ========
     *  @_nodoc
    */
    metaonly struct EventView {
        UInt         eventId;
        String       fxn;
        String       arg;
    };

    /*!
     *  ======== rovViewInfo ========
     *  @_nodoc
     */
    @Facet
    metaonly config xdc.rov.ViewInfo.Instance rovViewInfo =
        xdc.rov.ViewInfo.create({
            viewMap: [
                ['Events',
                    {
                        type: xdc.rov.ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewInitEvents',
                        structName: 'EventView'
                    }
                ]
            ]
        });

    // -------- Module Constants --------

    /*! C64+ supports 128 events. */
    const Int NUM_EVENTS = 128;

    // -------- Module Types --------

    /*! Event Combiner dispatcher function type definition. */
    typedef Void (*FuncPtr)(UArg);

    /*!
     *  Event Configuration Object.
     *
     *  unmask - Boolean value that specifies if an event should be
     *      unmasked in the C64+ EVTMASK registers.
     *  fxn - function to call when this event occurs.
     *  arg - arg to fxn.
     */
    metaonly struct EventObj {
        Bool unmask;
        FuncPtr fxn;
        UArg arg;
    };
    
    /*!
     *  Error raised when an unplug Event is executed.
     */
    config Error.Id E_unpluggedEvent = {
        msg: "E_unpluggedEvent: Event# %d is unplugged"
    };

    // -------- Module Parameters --------

    /*!
     *  ======== EVTMASK ========
     *  Holds the initialization values for the C64+ EVTMASK registers (0-3).
     *  
     *  It is assigned values based on the 'unmask' member of the 'events'
     *  configuration array.  It can also be assigned manually in the program
     *  configuration script.
     */
    config Bits32 EVTMASK[4];

    /*!
     *  ======== events ========
     *  For holding configuration values for all C64+ events.
     *  
     *  Array elements can be configured in the program
     *  configuration script.
     */
    metaonly config EventObj events[NUM_EVENTS];

    /*!
     *  ======== eventGroupHwiNum ========
     *  Configures the mapping of a C64+ combined event group to an interrupt.
     *
     *  There is one element per combined event group (0-3).
     */
    metaonly config Int eventGroupHwiNum[4];

    /*!
     *  ======== dispatchEventGroup ========
     *  Configuration method for assigning the eventGroupHwiNum array.
     *  
     *  It accomplishes the same thing as directly setting
     *  eventGroupHwiNum[0-3].
     *  
     */
    metaonly Void dispatchEventGroup(UInt evt, UInt hwiVec);

    /*!
     *  ======== disableEvent ========
     *  Disables 'evt' from contributing to its combined event group.
     *  
     *  It accomplishes this by setting the corresponding
     *  bit in the C64+ EVTMASK array to 1 (to mask it).
     */
    Void disableEvent(UInt evt);

    /*!
     *  ======== enableEvent ========
     *  Enables 'evt' to contribute to its combined event group.
     *  
     *  It accomplishes this by setting the corresponding
     *  bit in the C64+ EVTMASK array to 0 (to unmask it).
     */
    Void enableEvent(UInt evt);

    /*!
     *  ======== dispatch ========
     *  The Event Combiner dispatcher.
     *
     *  It is mostly used internally, but can be used directly by the user.
     */
    Void dispatch(UInt evt);

    /*!
     *  ======== dispatchPlug ========
     *  Used to configure a dispatcher entry for 'evt'.
     *
     *  The parameters correspond to the same ones for static configuration
     *  in EventObj.  dispatchPlug does not map the corresponding combined
     *  event group to an Hwi interrupt - such an action needs to be
     *  performed either using the event combiner configuration or using the Hwi
     *  module.
     */
    Void dispatchPlug(UInt evt, FuncPtr fxn, UArg arg, Bool unmask);

    /*!
     *  @_nodoc
     *  ======== unused ========
     *  unused exists simply to map a call in the Ecm dispatcher calling
     *  context to the System_exit calling context (casts UArg to Int).
     */
    Void unused(UArg arg);

internal:

    struct DispatchTabElem {
        FuncPtr fxn;
        UArg arg;
    };

    struct Module_State {
        DispatchTabElem dispatchTab[NUM_EVENTS];
    };

}

/*
 *  @(#) ti.sysbios.family.c64p; 2, 0, 0, 0,555; 5-18-2012 06:04:48; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

