#ifdef USE_RTMP

#include <stdlib.h>

#include "app_libuv.h"
#include "app_comm.h"

// libuv mainloop. 
// fixme : replace 'thread' with the main thread.
static pthread_t thread;
static uv_loop_t uv_loop;
uv_loop_t* loop = &uv_loop;
uv_timer_t *timer;

void empty_timer_cb (uv_timer_t* timer, int status) {
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

    // fixme : keep the loop alive. timer every 1s
    timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, empty_timer_cb, 1000* 1, 1000* 1);

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

#endif // USE_RTMP