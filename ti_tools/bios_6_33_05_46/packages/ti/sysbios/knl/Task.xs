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
 *  ======== Task.xs ========
 *
 *
 */

var Queue = null;
var Task = null;
var Idle = null;
var Program = null;
var BIOS = null;
var Memory = null;
var Settings = null;
var Clock = null;
var Swi = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    Task = this;
    Program = xdc.module('xdc.cfg.Program');

    if (Program.build.target.name.match(/C6.*/)) {
        Task.defaultStackSection = ".far:taskStackSection";
    }
    else if (Program.build.target.name.match(/C28.*/)) {
        Task.defaultStackSection = ".ebss:taskStackSection";
    }
    else {
        Task.defaultStackSection = ".bss:taskStackSection";
    }
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    BIOS = xdc.module('ti.sysbios.BIOS');
    Queue = xdc.useModule("ti.sysbios.knl.Queue");
    Idle = xdc.useModule("ti.sysbios.knl.Idle");
    Settings = xdc.module("ti.sysbios.family.Settings");

    Task.SupportProxy = xdc.module(Settings.getDefaultTaskSupportDelegate());
    
    var Defaults = xdc.useModule('xdc.runtime.Defaults');
    if (Defaults.common$.memoryPolicy == 
        xdc.module("xdc.runtime.Types").STATIC_POLICY) {
        Memory = xdc.module('xdc.runtime.Memory');
    }
    else {
        Memory = xdc.useModule('xdc.runtime.Memory');
    }

    xdc.useModule('xdc.runtime.Log');
    xdc.useModule('xdc.runtime.Assert');
    xdc.useModule("ti.sysbios.knl.Idle");
    xdc.useModule("ti.sysbios.knl.Intrinsics");
    xdc.useModule("ti.sysbios.knl.Task_SupportProxy");
    xdc.useModule("ti.sysbios.hal.Hwi");

    /* 
     * Pulling in Clock & Swi should be conditioned on BIOS.clockEnabled
     * and BIOS.swiEnabled but this causes linker issues. Pull them in
     * anyway and let their respective Mod.xs and Mod.c files
     * minimize their own code/data footprints.
     */
    Clock = xdc.useModule("ti.sysbios.knl.Clock");
    Swi = xdc.useModule("ti.sysbios.knl.Swi");

    
    /* Minimize footprint by only building 1 readyQ if Task is disabled */
    if (BIOS.taskEnabled == false) {
        Task.numPriorities = 1;
    }
    
    /* Initialize default stack sizes from proxy */

    /* undefined defaultStackSize means use proxy's defaultStackSize */
    if (Task.defaultStackSize === undefined) {
        Task.defaultStackSize = Task.SupportProxy.defaultStackSize;
    }
    
    /* undefined defaultStackHeap means use null */
    if (Task.defaultStackHeap === undefined) {
        Task.defaultStackHeap = null;
    }

    /* undefined idleTaskStackSize means use defaultStackSize */
    if (Task.idleTaskStackSize === undefined) {
        Task.idleTaskStackSize = Task.defaultStackSize;
    }

    /* undefined idleTaskStackSection means use defaultStackSection */
    if (Task.idleTaskStackSection === undefined) {
        Task.idleTaskStackSection = Task.defaultStackSection;
    }

    if (Task.allBlockedFunc === undefined) {
        Task.allBlockedFunc = Task.allBlockedFunction;
    }
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    mod.locked = true;
    mod.curSet = 0;
    mod.vitalTasks = 0;
    mod.workFlag = 0;
    mod.curTask = null;
    mod.curQ = null;

    Queue.construct(mod.inactiveQ);
    Queue.construct(mod.terminatedQ);

    mod.readyQ.length = Task.numPriorities;
    for (var i = 0; i < Task.numPriorities; i++) {
        Queue.construct(mod.readyQ[i]);
    }

    /* 
     * Capture deprecated ".taskStackSection" settings
     */
    if ((Program.sectMap[Task.defaultStackSection] === undefined) &&
        !(Program.sectMap[".taskStackSection"] === undefined)) {

        Task.$logWarning("The Task.defaultTaskSection has been " +
                         "changed to \"" + Task.defaultStackSection +
                         "\".  Please review your .cfg file for references " +
                         "to \".taskStackSection\" and replace those " +
                         "references with 'Task.defaultStackSection' or \"" +
                         Task.defaultStackSection + "\".", Task);

        Program.sectMap[Task.defaultStackSection] = new Program.SectionSpec();

        if (typeof(Program.sectMap[".taskStackSection"]) == "string") {
            /* value of sectMap entry is segment name if its not a new SectionSpec */
            Program.sectMap[Task.defaultStackSection].loadSegment = 
                Program.sectMap[".taskStackSection"];
        }
        else {
            /* copy all attrs over */
            for (var attr in Program.sectMap[".taskStackSection"]) {
                Program.sectMap[Task.defaultStackSection][attr] =
                   Program.sectMap[".taskStackSection"][attr];
            }
        }

        /* delete users's Program.sectMap entry. */
        delete Program.sectMap[".taskStackSection"];
    }

    /* minimize footprint by only creating Idle task if Task is enabled */
    if ((BIOS.taskEnabled == true) && (Task.enableIdleTask == true)) {
        /* Create Idle Task */
        var tskParams = new Task.Params();
        tskParams.priority = 0;
        tskParams.vitalTaskFlag = Task.idleTaskVitalTaskFlag;
        tskParams.stackSize = Task.idleTaskStackSize;
        tskParams.stackSection = Task.idleTaskStackSection;
        tskParams.instance.name = "ti.sysbios.knl.Task.IdleTask";

        mod.idleTask = Task.create(Idle.loop, tskParams);

        /* This is creating the symbol 'TSK_idle' for legacy purpose */
        Program.symbol["TSK_idle"] = mod.idleTask;
    }
    else {
        mod.idleTask = null;
        if (Task.allBlockedFunc == Idle.run) {
            mod.vitalTasks += 1;
        }
    }

    if (Task.deleteTerminatedTasks == true) {
        Idle.funcList.$add(Task.deleteTerminatedTasksFunc); 
    }
}

/*
 *  ======== instance$static$init ========
 */
function instance$static$init(obj, fxn, params)
{
    var Program = xdc.module('xdc.cfg.Program');
    var mod = this.$module.$object;
    var align;

    if (BIOS.taskEnabled == false) {
        Task.$logFatal("Can't create a Task with BIOS.taskEnabled == false.", this);
    }

    /* undefined stackSize means use default which is defaultStackSize */
    if (params.stackSize == 0) {        
            params.stackSize = Task.defaultStackSize;
    }

    /* undefined stackSecton means use default which is defaultStackSection */
    if (params.stackSection === undefined) {    
            params.stackSection = Task.defaultStackSection;
    }

    /* undefined stackHeap means use default which is defaultStackHeap */
    if (params.stackHeap == null) {     
            params.stackHeap = Task.defaultStackHeap;
    }

    obj.priority = params.priority;

    obj.mode = Task.Mode_READY;

    obj.pendElem = null;

    obj.stackHeap = params.stackHeap;

    align = Task.SupportProxy.stackAlignment;

    if (params.stack) {
        Task.$logError("The stack field is not supported for statically defined tasks", 
                       this, "stack");
    }
    else {
            if (align != 0) {
            /* 
             * round stackSize up to the nearest multiple of the alignment.
             */
                obj.stackSize = (params.stackSize + align - 1) & -align;
            obj.stack.length = obj.stackSize;
        }
        else {
            obj.stackSize = params.stackSize;
            obj.stack.length = obj.stackSize;
        }
        Memory.staticPlace(obj.stack, align, params.stackSection);
    }

    if (obj.stackSize != params.stackSize) {
        Task.$logWarning("stackSize was adjusted to guarantee proper alignment", this, "stackSize");
    }

    obj.fxn = fxn;
    obj.arg0 = params.arg0;
    obj.arg1 = params.arg1;

    obj.env = params.env;

    obj.vitalTaskFlag = params.vitalTaskFlag;
    if (obj.vitalTaskFlag == true) {
        mod.vitalTasks += 1;
    }

    /* 
     * the following assignment sets the number of entries in the
     * hookEnv pointer array pointed to by the obj.hookEnv ptr.
     */
    obj.hookEnv.length = Task.hooks.length;

    Queue.elemClearMeta(obj.qElem);
    obj.mask = 1 << params.priority;
    obj.context = null;
    obj.readyQ = null;

    /* add constructed tasks to constructedTasks array */
    if (this.$category == "Struct") {
        mod.constructedTasks.$add(this);
        Task.numConstructedTasks += 1;
    }
}

/*
 *  ======== addHookSet ========
 */
function addHookSet(hookSet)
{
    /* use "===" so 'null' is not flagged */
    if (hookSet.registerFxn === undefined) {
        hookSet.registerFxn = null;
    }
    if (hookSet.createFxn === undefined) {
        hookSet.createFxn = null;
    }
    if (hookSet.readyFxn === undefined) {
        hookSet.readyFxn = null;
    }
    if (hookSet.switchFxn === undefined) {
        hookSet.switchFxn = null;
    }
    if (hookSet.exitFxn === undefined) {
        hookSet.exitFxn = null;
    }
    if (hookSet.deleteFxn === undefined) {
        hookSet.deleteFxn = null;
    }

    this.hooks.$add(hookSet);
}

/*
 *  ======== module$validate ========
 *  some redundant tests are here to catch changes since
 *  module$static$init() and instance$static$init()
 */
function module$validate()
{
    if (Task.numPriorities > BIOS.bitsPerInt) {
        Task.$logError("Task.numPriorities (" + 
                        Task.numPriorities + 
                        ") can't be greater " +
                        "than the number of " +
                        "bits in an integer (" +
                        BIOS.bitsPerInt + ")",
                        Task, "numPriorities");
    }

    /* validate all "created" instances */
    for(var i = 0; i < Task.$instances.length; i++) {
        instance_validate(Task.$instances[i]);
    }

    /* validate all "constructed" instances */
    for (var i = 0; i < Task.$objects.length; i++) {
        instance_validate(Task.$objects[i]);
    }
}

/*
 *  ======== instance_validate ========
 *  common function to test instance configuration
 */
function instance_validate(instance)
{
    if (instance.$object.fxn == null) {
        Task.$logError("function cannot be null", instance);
    }
    if (instance.priority >= Task.numPriorities) {
        Task.$logError("priority " + 
                instance.priority + 
                " must be less than Task.numPriorities (" + 
                Task.numPriorities + ").", instance, "priority");
    }
    if (Task.enableIdleTask == true) {
        if ((instance.priority == 0) && 
            (instance.$object.fxn != Idle.loop)) {
            Task.$logWarning("Priority 0 reserved for Idle task", instance,
                         "priority");
            }
    }
    if ((instance.priority != 0) && 
        (instance.$object.fxn == Idle.loop)) {
        Task.$logError("Idle task priority must be 0", instance, "priority");
    }
}

/*
 *  ======== scanConstructedTasks ========
 *  Scans statically constructed Tasks to add them to the Task ROV view.
 *
 *  The Task module maintains a table of all statically constructed Tasks so
 *  that it can initialize them at runtime. Constructed objects don't appear
 *  in ROV automatically, so we scan them manually here.
 *
 *  This function does not perform any error handling because it has nowhere
 *  to display an error. If any of the APIs called within this function throw
 *  an exception, it will propogate up and be displayed to the user in ROV.
 */
function scanConstructedTasks(data, viewLevel)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Task = xdc.useModule('ti.sysbios.knl.Task');
    
    /* Check if the constructed tasks have already been scanned. */
    if (data.scannedConstructedTasks) {
        return;
    }
    
    /* 
     * Set the flag to true now to prevent recursive calls of this function
     * when we scan the constructed tasks.
     */
    data.scannedConstructedTasks = true;

    /* Get the Task module config to get the number of constructed tasks. */
    var modCfg = Program.getModuleConfig('ti.sysbios.knl.Task');
    
    var numConstrTasks = modCfg.numConstructedTasks;

    /* Just return if there are no statically constructed tasks. */
    if (numConstrTasks == 0) {
        return;
    }
    
    /* 
     * Retrieve the raw view to get at the module state.
     * This should just return, we don't need to catch exceptions. 
     */
    var rawView = Program.scanRawView('ti.sysbios.knl.Task');
    
    var constrTasksAddr = rawView.modState.constructedTasks;
    var ScalarStructs = xdc.useModule('xdc.rov.support.ScalarStructs');    
    
    /* Retrieve the array of handles to the statically constructed tasks. */
    var taskHandles = Program.fetchArray(ScalarStructs.S_Ptr$fetchDesc, 
                                         constrTasksAddr, numConstrTasks);
    /* 
     * Scan the instance view for each of the instances. 
     * The scanned views will be automatically added to the 'Basic' tab.
     */
    for each (var handle in taskHandles) {
        /* Retrieve the embedded instance */        
        var obj = Program.fetchStruct(Task.Instance_State$fetchDesc, handle.elem);

        /* 
         * Retrieve the view for the object. This will automatically add the 
         * object to the instance list.
         */
        Program.scanObjectView('ti.sysbios.knl.Task', obj, viewLevel);
    }
}

/*
 *  ======== viewInitBasic ========
 *  Initialize the 'Basic' Task instance view.
 */
function viewInitBasic(view, obj)
{    
    var Task = xdc.useModule('ti.sysbios.knl.Task');
    var Program = xdc.useModule('xdc.rov.Program');
    
    /* 
     * First, scan the Task module's array of constructed tasks so that these
     * are added to the view.
     */
    scanConstructedTasks(this, 'Basic');
    
    view.label = Program.getShortName(obj.$label);
    view.priority = obj.priority;
    
    /* Validate priority */
    var modCfg = Program.getModuleConfig(Task.$name);
    if ((obj.priority < -1) || (obj.priority > (modCfg.numPriorities - 1))) {
        Program.displayError(view, "priority",  "Corrupted data: Task " +
                             "priority is greater than Task.numPriorities");
        return;
    }
    
    /* Scan the module view to determine the current running Task. */
    try {
        var rawView = Program.scanRawView('ti.sysbios.knl.Task');
    }
    catch (e) {
        Program.displayError(view, "mode", "Error scanning raw view to " +
                             "get current task.");
        return;
    }

    /* Set the Task's mode */
    if ((Number(rawView.modState.curTask) == Number(obj.$addr))) {
        view.mode = "Running";
    }
    else if (obj.priority == -1) {
        view.mode = "Inactive";
    }
    else {
        switch (obj.mode) {
            case Task.Mode_READY:
                view.mode = "Ready";
                break;
            case Task.Mode_BLOCKED:
                view.mode = "Blocked";
                break;
            case Task.Mode_TERMINATED:
                view.mode = "Terminated";
                break;
            default:
                view.mode = "Invalid mode";
                Program.displayError(view, "mode", "Invalid mode: " + 
                                     obj.mode);
        }
    }
    
    view.stackSize = obj.stackSize;

    view.stackBase = "0x" + Number(obj.stack).toString(16);

    /* 
     * There may be more than one symbol at this address, so the function
     * name has to be an array.
     */
    view.fxn = Program.lookupFuncName(Number(obj.fxn));

    /* 
     * Special trap for legacy TSKs prior to first time running
     * arg1 contains fxn
     */
    if (view.fxn[0] == "_TSK_staticGlue" || view.fxn[0] == "dynamicGlue") {
        view.fxn = Program.lookupFuncName(Number(obj.arg1));
    }
    
    view.arg0 = obj.arg0;
    view.arg1 = obj.arg1;

    // must check that fxn[] length is > 0 before accessing it for nickName
    var fxnName = (view.fxn.length > 0) ? "" + view.fxn[0] : "";

    view.$private.nickName = view.label != "" ? "Label: " + view.label 
                            : fxnName != "" ? "Fxn: " + fxnName 
                        : "Handle: " + Number(obj.$addr).toString(16);
}

/*
 *  ======== getNickName ========
 */
function getNickName(tskView)
{
    return(tskView.$private.nickName);
}

/*
 *  ======== viewInitDetailed ========
 *  Initialize the 'Detailed' Task instance view.
 */
function viewInitDetailed(view, obj)
{   
    var Program = xdc.useModule('xdc.rov.Program');
    var Hwi = xdc.useModule('ti.sysbios.hal.Hwi');

    /* 
     * First, scan the Task module's array of constructed tasks so that these
     * are added to the view.
     */
    scanConstructedTasks(this, 'Detailed');
    
    /* Detailed view builds off basic view. */
    viewInitBasic(view, obj);
    
    /* workaround where stack of dummy task points to 0xbebebebe */
    if (obj.stack == 0xbebebebe) {
        return;
    }
    
    /* Fetch the Task stack */
    try {
        var stackData = Program.fetchArray(obj.stack$fetchDesc, obj.stack, 
                                           obj.stackSize);
    }
    catch (e) {
        Program.displayError(view, "stackPeak", "Error: Problem fetching " +
                             "Task stack: " + e.toString());
        return;
    }
    
    /* The function for calculating the stack peak is family-specific. */
    var Support = Program.$modules['ti.sysbios.knl.Task'].SupportProxy;
    var Delegate = xdc.useModule(Support.$name, true);
    view.stackPeak = Delegate.stackUsed$view(stackData);
    if (view.stackPeak == view.stackSize) {
        Program.displayError(view, "stackPeak", "Overrun!  ");
        /* (extra spaces to overcome right justify) */
    }
    else if ((obj.context < obj.stack) || 
             (obj.context > obj.stack + obj.stackSize)) {
        Program.displayError(view, "stackPeak", "SP outside stack!");
    }

    var hwiStackInfo = Hwi.viewGetStackInfo();

    if (hwiStackInfo.hwiStackPeak == hwiStackInfo.hwiStackSize) {
        Program.displayError(view, "stackPeak", "Hwi Stack Overrun!");
    }

    /* fill in blockedOn field if blocked */
    if (view.mode == "Blocked") {
            if (checkMailboxes(view, obj) == true) return;
            if (checkGateMutexes(view, obj) == true ) return;
            if (checkGateMutexPris(view, obj) == true ) return;
            if (checkSemaphores(view, obj) == true) return;
            if (checkEvents(view, obj) == true ) return;
            if (checkTaskSleep(view, obj) == true ) return;
        view.blockedOn = "Unknown";
    }
}

/*
 *  ======== checkSemPendQ ========
 *  See if task is blocked on this Semaphore
 */
function checkSemPendQ(sem, task)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');

    /* Scan the pendQ to get its elements */
    try {
        var pendQView = Program.scanObjectView('ti.sysbios.knl.Queue', sem.pendQ, 'Basic');
    }
    catch (e) {
        return false;
    }

    /* Compare each task in the pend Q with us */
    for (var j=0; j<pendQView.elems.length; j++) {

        /* Get the element address from the Queue view. */
        var pendElemAddr = pendQView.elems[j];

        /* Fetch the PendElem struct */
        try {
                var pendElem = Program.fetchStruct(Semaphore.PendElem$fetchDesc, pendElemAddr);
        }
        catch (e) {
            return false;
        }
        if (Number(pendElem.tpElem.task) == Number(task.$addr)) {
                return true;
        }
    }
    return false;
}

/*
 *  ======== checkSemaphores ========
 *  See if task is blocked on any Semaphores
 */
function checkSemaphores(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');

    /* check to see if Semaphore is used and grab raw view if so */
    try {
        var semRawView = Program.scanRawView('ti.sysbios.knl.Semaphore');
    }
    catch (e) {
        return false;
    }

    /* Check each instance */
    for (var i in semRawView.instStates) {
        if (checkSemPendQ(semRawView.instStates[i], obj)) {
            view.blockedOn = "Semaphore: 0x" + Number(semRawView.instStates[i].$addr).toString(16);
            return true;
        }
    }
    return false;
}

/*
 *  ======== checkEvents ========
 *  See if task is blocked on any Events
 */
function checkEvents(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Event = xdc.useModule('ti.sysbios.knl.Event');
 
    /* check to see if Event is used and grab raw view if so */
    try {
        var eventRawView = Program.scanRawView('ti.sysbios.knl.Event');
    }
    catch (e) {
        return false;
    }

    /* Check each instance */
    for (var i in eventRawView.instStates) {

        /* Scan the pendQ to get its elements */
        try {
            var pendQView = Program.scanObjectView('ti.sysbios.knl.Queue', eventRawView.instStates[i].pendQ, 'Basic');
        }
        catch (e) {
            return false;
        }

        /* Compare each task in the pend Q with us */
        for (var j=0; j<pendQView.elems.length; j++) {

            /* Get the element address from the Queue view. */
            var pendElemAddr = pendQView.elems[j];

            /* Fetch the PendElem struct */
            try {
                var pendElem = 
                    Program.fetchStruct(Event.PendElem$fetchDesc, 
                                        pendElemAddr);
            }
            catch (e) {
                Program.displayError(view, "blockedOn", "Problem with " +
                                     "fetching Event pend element 0x" +
                                     Number(pendElemAddr).toString(16) + 
                                     ": " + e.toString());
                return false;
            }
            if (Number(pendElem.tpElem.task) == Number(obj.$addr)) {
                view.blockedOn = "Event: 0x" + 
                    Number(eventRawView.instStates[i].$addr).toString(16);
                return true;
            }
        }
    }
    return false;
}

/*
 *  ======== checkMailboxes ========
 *  See if task is blocked on any Mailboxes
 */
function checkMailboxes(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Mailbox = xdc.useModule('ti.sysbios.knl.Mailbox');
    var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');

    /* check to see if GateMutexPri is used and grab raw view if so */
    try {
        var mailboxRawView = Program.scanRawView('ti.sysbios.knl.Mailbox');
    }
    catch (e) {
        return false;
    }

    /* Check each instance */
    for (var i in mailboxRawView.instStates) {

        /* get data sem handle */
        var dataSem = mailboxRawView.instStates[i].dataSem;

        if (checkSemPendQ(dataSem, obj)) {
            view.blockedOn = "Mailbox_pend: 0x" + 
                Number(mailboxRawView.instStates[i].$addr).toString(16);
            return true;
        }

        /* get free sem handle */
        var freeSem = mailboxRawView.instStates[i].freeSem;

        if (checkSemPendQ(freeSem, obj)) {
            view.blockedOn = "Mailbox_post: 0x" + 
                Number(mailboxRawView.instStates[i].$addr).toString(16);
            return true;
        }
    }
    return false;
}

/*
 *  ======== checkTaskSleep ========
 *  See if task is blocked in Task_sleep()
 */
function checkTaskSleep(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Task = xdc.useModule('ti.sysbios.knl.Task');

    if (obj.pendElem == 0) {
            return false;
    }

    /* Get the element address from the Queue view. */
    var pendElemAddr = obj.pendElem;

    /* Fetch the PendElem struct */
    try {
        var pendElem = 
            Program.fetchStruct(Task.PendElem$fetchDesc, pendElemAddr);
    }
    catch (e) {
        Program.displayError(view, "blockedOn", 
            "Problem with fetching Task pend element 0x" + 
            Number(pendElemAddr).toString(16) + ": " + e.toString());
        return false;
    }

    /* Check if 'clock' is null. */
    if (Number(pendElem.clock) == 0) {
        return (false);
    }
    
    /* Otherwise, retrieve the view for the clock handle. */
    try {
        var clockView = 
            Program.scanHandleView('ti.sysbios.knl.Clock', 
                                   pendElem.clock, 'Basic');
    }
    catch (e) {
        Program.displayError(view, "blockedOn", 
            "Problem scanning pending Clock 0x" +
            Number(pendElem.clock).toString(16) + ": " + e.toString());
        return false;
    }

    view.blockedOn = "Task_sleep(" + clockView.tRemaining + ")";
    return true;
}

/*
 *  ======== checkGateMutexes ========
 *  See if task is blocked on any GateMutexes
 */
function checkGateMutexes(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var GateMutex = xdc.useModule('ti.sysbios.gates.GateMutex');
    var Semaphore = xdc.useModule('ti.sysbios.knl.Semaphore');

    /* check to see if GateMutexPri is used and grab raw view if so */
    try {
        var gateRawView = Program.scanRawView('ti.sysbios.gates.GateMutex');
    }
    catch (e) {
        return false;
    }

    /* Check each instance */
    for (var i in gateRawView.instStates) {

        /* get sem handle */
        var sem = gateRawView.instStates[i].sem;

        if (checkSemPendQ(sem, obj)) {
            view.blockedOn = "GateMutex: 0x" + Number(gateRawView.instStates[i].$addr).toString(16);
            return true;
        }
    }
    return false;
}

/*
 *  ======== checkGateMutexPris ========
 *  See if task is blocked on any GateMutexPris
 */
function checkGateMutexPris(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Task = xdc.useModule('ti.sysbios.knl.Task');
    var GateMutexPri = xdc.useModule('ti.sysbios.gates.GateMutexPri');

    /* check to see if GateMutexPri is used and grab raw view if so */
    try {
        var gateRawView = Program.scanRawView('ti.sysbios.gates.GateMutexPri');
    }
    catch (e) {
        return false;
    }

    /* Check each instance */
    for (var i in gateRawView.instStates) {

        /* Scan the pendQ to get its elements */
        try {
            var pendQView = Program.scanObjectView('ti.sysbios.knl.Queue', gateRawView.instStates[i].pendQ, 'Basic');
        }
        catch (e) {
            return false;
        }

        /* Compare each task in the pend Q with us */
        for (var j = 0; j < pendQView.elems.length; j++) {

            /* Get the pendElem address from the Queue view. */
            var pendElemAddr = pendQView.elems[j];

            /* Fetch the PendElem struct */
            try {
                var pendElem = Program.fetchStruct(Task.PendElem$fetchDesc, pendElemAddr);
            }
            catch (e) {
                return false;
            }

            var currTaskHandle = pendElem.task;

            if (Number(currTaskHandle.$addr) == Number(obj.$addr)) {
                view.blockedOn = "GateMutexPri: 0x" + Number(gateRawView.instStates[i].$addr).toString(16);
                return true;
            }
        }
    }
    return false;
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Task module view.
 */
function viewInitModule(view, mod)
{
    var BIOS = xdc.useModule('ti.sysbios.BIOS');
    var Program = xdc.useModule('xdc.rov.Program');
    var Hwi = xdc.useModule('ti.sysbios.hal.Hwi');

    var swiLock = 0;
    
    var biosModConfig = Program.getModuleConfig(BIOS.$name);

    if (biosModConfig.swiEnabled == true) {
            try {
                var swiRawView = Program.scanRawView('ti.sysbios.knl.Swi');
                swiLock = swiRawView.modState.locked;
            }
        catch (e) {
            Program.displayError(view, "schedulerState", 
                "Swi scheduler state unknown");
            swiLock = 0;
            }
    }

    view.schedulerState = (mod.locked ? "Locked" : swiLock ? "Blocked by Swi scheduler" : "Unlocked");
    view.readyQMask = "0x" + mod.curSet.toString(16);
    view.workPending = mod.workFlag;
    view.numVitalTasks = mod.vitalTasks;
    view.currentTask = mod.curTask;

    var stackInfo = Hwi.viewGetStackInfo();

    if (stackInfo.hwiStackSize == 0) {
        var msg = "Error fetching Hwi stack info!";
        Program.displayError(view, "hwiStackPeak", msg);
        Program.displayError(view, "hwiStackSize", msg);
        Program.displayError(view, "hwiStackBase", msg);
    }
    else {
        view.hwiStackPeak = stackInfo.hwiStackPeak;
        view.hwiStackSize = stackInfo.hwiStackSize;
        view.hwiStackBase = "0x"+ stackInfo.hwiStackBase.toString(16);

        if (view.hwiStackPeak == view.hwiStackSize) {
            Program.displayError(view, "hwiStackPeak", "Overrun!  "); 
            /*                                                  ^^  */
            /* (extra spaces to overcome right justify)             */
        }
    }
}


/*
 *  @(#) ti.sysbios.knl; 2, 0, 0, 0,552; 5-18-2012 06:05:55; /db/vtree/library/trees/avala/avala-q37x/src/ xlibrary

 */

