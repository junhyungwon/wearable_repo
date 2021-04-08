#include <stdlib.h>

#include "app_libuv.h"
#include "app_comm.h"

// libuv mainloop. 
// fixme : replace 'thread' with the main thread.
static pthread_t thread;
static uv_loop_t uv_loop;
static int selfLoad = -1;
static int totalLoad = -1;

uv_loop_t* loop = &uv_loop;
int *procLoad = &selfLoad;
int *cpuLoad = &totalLoad;

uv_timer_t *timer;

void cpuload_timer_cb (uv_timer_t* timer, int status) {
    if (getArmCpuLoad(&selfLoad, &totalLoad) == SUCCESS) {
        // fprintf(stderr, "[LIBUV] current cpu load. self : %d, total : %d.\n", *procLoad, *cpuLoad);
    } else {
        *cpuLoad = *procLoad = -1;
    }
    // fprintf(stderr, "[LIBUV] the loop alive.\n");
}

void uv_thread(void* context){
    //Start loop
    uv_run(loop, UV_RUN_DEFAULT);

    pthread_exit(NULL);
}

/*****************************************************************************
* @brief    capture start/stop function
* @section  [desc]
*****************************************************************************/
int app_libuv_start(void)
{
    fprintf(stderr, "[LIBUV] start the event loop.\n");
    uv_loop_init(loop);

    // keep the loop alive. check cpu load on every 1s
    timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, cpuload_timer_cb, 1000* 1, 1000* 1);

    // create libuv thread
    pthread_create(&thread, NULL, uv_thread, NULL);
    pthread_setname_np(thread, __FILENAME__);

    return SOK;
}

void app_libuv_stop(void)
{
    fprintf(stderr, "[LIBUV] the loop exited.\n");
    uv_timer_stop(timer);
    uv_loop_close(loop);
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
        ERR("/proc/stat not found. Is the /proc filesystem mounted?\n");
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
        ERR("/proc/self/stat not found. Is the /proc filesystem mounted?\n");
        return FAILURE;
    }

    if (fscanf(fptr, "%*d %*s %*s %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu "
                     "%lu %lu %lu", &uTime, &sTime, &cuTime, &csTime) != 4) {
        ERR("Failed to get process load information.\n");
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