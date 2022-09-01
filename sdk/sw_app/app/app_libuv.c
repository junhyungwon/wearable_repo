#include <stdio.h>
#include <stdlib.h>
#define __USE_GNU
#include <pthread.h>

#include "app_libuv.h"
#include "app_comm.h"

// libuv mainloop. 
// fixme : replace 'thread' with the main thread.
static pthread_t thread, thread_video;
static uv_loop_t uv_loop, uv_loop_video;
static int selfLoad = -1;
static int totalLoad = -1;

uv_loop_t* loop = &uv_loop;
uv_loop_t* loop_video = &uv_loop_video;
int *procLoad = &selfLoad;
int *cpuLoad = &totalLoad;

uv_timer_t *timer_cpu;
uv_timer_t *timer_empty;

void cpuload_timer_cb (uv_timer_t* timer, int status) {
    if (getArmCpuLoad(&selfLoad, &totalLoad) == SUCCESS) {
        // TRACE_INFO("[LIBUV] current cpu load. self : %d, total : %d.\n", *procLoad, *cpuLoad);
    } else {
        *cpuLoad = *procLoad = -1;
    }
    // TRACE_INFO("[LIBUV] the loop alive.\n");
}

void empty_timer_cb (uv_timer_t* timer, int status) {
    // TRACE_INFO("[LIBUV] the loop alive.\n");
}

void uv_thread(void* context){
    //Start loop
    int r = uv_run(loop, UV_RUN_DEFAULT);
    TRACE_INFO("[LIBUV] the default event loop exited with %d\n", r);
    pthread_exit(NULL);
}

void uv_thread_video(void* context){
    //Start loop for video
    int r = uv_run(loop_video, UV_RUN_DEFAULT);
    TRACE_INFO("[LIBUV] the video event loop exited with %d\n", r);
    pthread_exit(NULL);
}

/*****************************************************************************
* @brief    capture start/stop function
* @section  [desc]
*****************************************************************************/
int app_libuv_start(void)
{
    TRACE_INFO("[LIBUV] start the event loop.\n");
    uv_loop_init(loop);
    uv_loop_init(loop_video);

    // keep the loop alive. check cpu load on every 1s
    timer_cpu = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer_cpu);
    uv_timer_start(timer_cpu, (uv_timer_cb)cpuload_timer_cb, 0, 1000* 1);

    // keep the loop alive.
    timer_empty = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop_video, timer_empty);
    uv_timer_start(timer_empty, (uv_timer_cb)empty_timer_cb, 0, 1000* 1);

    // create libuv thread
    pthread_create(&thread, NULL, (void *)uv_thread, NULL);
    pthread_setname_np(thread, __FILENAME__);

    pthread_create(&thread_video, NULL, (void *)uv_thread_video, NULL);
    pthread_setname_np(thread_video, __FILENAME__);

    return SOK;
}

void app_libuv_stop(void)
{
    TRACE_INFO("[LIBUV] the loop exited.\n");
    uv_timer_stop(timer_cpu);
    uv_timer_stop(timer_empty);
    uv_loop_close(loop);
    uv_loop_close(loop_video);
}

/******************************************************************************
 * getArmCpuLoad
 ******************************************************************************/
int getArmCpuLoad(int *procLoad, int *cpuLoad)
{
    static unsigned long prevIdle     = 0;
    static unsigned long prevTotal    = 0;
    static unsigned long prevProc     = 0;
    int                  cpuLoadFound = FALSE;
    unsigned long        user, nice, sys, idle, total, proc;
    unsigned long        uTime, sTime, cuTime, csTime;
    unsigned long        deltaTotal, deltaIdle, deltaProc;
    char                 textBuf[4];
    FILE                *fptr;

    /* Read the overall system information */
    fptr = fopen("/proc/stat", "r");
    if (fptr == NULL) {
        LOGE("/proc/stat not found. Is the /proc filesystem mounted?\n");
        return FAILURE;
    }

    /* Scan the file line by line */
    while (fscanf(fptr, "%4s %lu %lu %lu %lu %*[^\n]", textBuf,
                  &user, &nice, &sys, &idle) != EOF) {
        if (strcmp(textBuf, "cpu") == 0) {
            cpuLoadFound = TRUE;
            break;
        }
    }

    if (fclose(fptr) != 0) {
        return FAILURE;
    }

    if (!cpuLoadFound) {
        return FAILURE;
    }

    /* Read the current process information */
    fptr = fopen("/proc/self/stat", "r");
    if (fptr == NULL) {
        LOGE("/proc/self/stat not found. Is the /proc filesystem mounted?\n");
        return FAILURE;
    }

    if (fscanf(fptr, "%*d %*s %*s %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu "
                     "%lu %lu %lu", &uTime, &sTime, &cuTime, &csTime) != 4) {
        LOGE("Failed to get process load information.\n");
        fclose(fptr);
        return FAILURE;
    }

    if (fclose(fptr) != 0) {
        return FAILURE;
    }

    total = user + nice + sys + idle;
    proc = uTime + sTime + cuTime + csTime;

    /* Check if this is the first time, if so init the prev values */
    if (prevIdle == 0 && prevTotal == 0 && prevProc == 0) {
        prevIdle = idle;
        prevTotal = total;
        prevProc = proc;
        return SUCCESS;
    }

    deltaIdle = idle - prevIdle;
    deltaTotal = total - prevTotal;
    deltaProc = proc - prevProc;

    prevIdle = idle;
    prevTotal = total;
    prevProc = proc;

    *cpuLoad = 100 - deltaIdle * 100 / deltaTotal;
    *procLoad = deltaProc * 100 / deltaTotal;

    return SUCCESS;
}