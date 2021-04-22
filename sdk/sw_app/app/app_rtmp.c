#ifdef USE_RTMP


#include "app_comm.h"
#include "app_libuv.h"
#include "app_rtmp.h"

// libuv's queue
#include "queue.h"

// wowza. fixme : need a config file
// $ ffplay rtmp://3.36.1.29:1935/live2/myStream/key
char rtmp_endpoint[256] = "rtmp://3.36.1.29:1935/live2/myStream/key";

static uv_async_t *async_rtmp_connect;
static uv_async_t *async_rtmp_disconnect;
static uv_async_t *async_rtmp_publish;
static uv_timer_t *timer;
static uv_mutex_t mutex;

// librtmp
static srs_rtmp_t rtmp = NULL;
static bool rtmp_ready = false;
static bool rtmp_enabled = false;


// streams queue
static int queue_lenth = 0;
static QUEUE queue;
struct stream_s {
    stream_info_t *ifr;
    QUEUE node;
};

static void _init_queue() {
    QUEUE *q;

    uv_mutex_lock(&mutex);
    QUEUE_FOREACH(q, &queue) {
        struct stream_s* stream = QUEUE_DATA(q, struct stream_s, node);
        free(stream);
        QUEUE_REMOVE(q);
    }
    queue_lenth = 0;
    uv_mutex_unlock(&mutex);
}

static void _rtmp_close() {
    if (rtmp != NULL) {
        fprintf(stderr, "[RTMP] close the RTMP connection.\n");
        srs_rtmp_destroy(rtmp);
        rtmp = NULL;
    }

    rtmp_ready = false;
    _init_queue();
}

static inline int _checkAsyncQueue() {
    // max async queue to (DEFAULT_FPS * 1)
    if (queue_lenth > (DEFAULT_FPS * 1)) {
        fprintf(stderr, "[RTMP] async queue overflow(queue length: %d).\n"
            , queue_lenth);

        return FAILURE;
    }
    return SUCCESS;
}

static int _checkCpuLoad() {
    if (*procLoad == -1 || *procLoad >= 50) {
        fprintf(stderr, "[RTMP] high cpu load(self: %d, total: %d).\n"
            , *procLoad, *cpuLoad);

        return FAILURE;
    }
    return SUCCESS;
}

static void rtmp_publish_async_cb(uv_async_t* async) {
    if (rtmp_enabled == false  || rtmp_ready == false)
        return;

    // fprintf(stderr, "[RTMP] rtmp_publish_async_cb fired.\n");
    struct stream_s* stream;

    while(1) {
        uv_mutex_lock(&mutex);
        bool empty = QUEUE_EMPTY(&queue);

        if (empty) {
            uv_mutex_unlock(&mutex);
            break;
        }

        QUEUE *q = QUEUE_HEAD(&queue);
        QUEUE_REMOVE(q);
        int length = --queue_lenth;
        uv_mutex_unlock(&mutex);

        // fprintf(stderr, "[RTMP] dequeue. length : %d.\n", length);

        // fixme : imem will be in a short time.
        stream = QUEUE_DATA(q, struct stream_s, node);
        stream_info_t *ifr = stream->ifr;
        free(stream);

        if (rtmp == NULL || ifr == NULL || ifr->addr == NULL || ifr->b_size <= 0) {
            fprintf(stderr, "[RTMP] wrong data.\n");
            break;
        }

        // fprintf(stderr, "async_cb %d\n", ifr->b_size);
        static int isFirst = 0;
        if (isFirst == 0 && ifr->is_key) {
            isFirst = 1;
        }

        if (isFirst == 1 && rtmp_ready) {
            char* data = ifr->addr;
            auto size = ifr->b_size;
            int nb_start_code = 0;

            u_int32_t fps = DEFAULT_FPS;
            u_int32_t dts = 1000/fps;
            u_int32_t pts = dts;

            // u_int32_t timestamp = ifr->t_sec * 1000 + ifr->t_msec;
            // dts = timestamp;
            // pts = timestamp;

            // send out the h264 packet over RTMP
            int ret = srs_h264_write_raw_frames(rtmp, data, size, dts, pts);
            if (ret != 0) {
                if (srs_h264_is_dvbsp_error(ret)) {
                    srs_human_trace("[RTMP] ignore drop video error, code=%d", ret);
                } else if (srs_h264_is_duplicated_sps_error(ret)) {
                    srs_human_trace("[RTMP] ignore duplicated sps, code=%d", ret);
                } else if (srs_h264_is_duplicated_pps_error(ret)) {
                    // ignore a 'known error'.
                    // srs_human_trace("[RTMP] ignore duplicated pps, code=%d", ret);
                } else {
                    srs_human_trace("[RTMP] send h264 raw data failed. ret=%d", ret);

                    rtmp_ready = false;
                    break;
                }
            }

            // 5bits, 7.3.1 NAL unit syntax, 
            // H.264-AVC-ISO_IEC_14496-10.pdf, page 44.
    /*
            u_int8_t nut = (char) data[nb_start_code] & 0x1f;
            srs_human_trace("sent packet: type=%s, time=%d, size=%d, fps=%d, b[%d]=%#x(%s)",
                            srs_human_flv_tag_type2string(SRS_RTMP_TYPE_VIDEO), dts, size, fps, nb_start_code,
                            (char) data[nb_start_code],
                            (nut == 7 ? "SPS" : (nut == 8 ? "PPS" : (nut == 5 ? "I" : (nut == 1 ? "P" : "Unknown")))));
    */


        }
    }
}

void rtmp_connect_timer_cb (uv_timer_t* timer, int status) {
    if (!rtmp_enabled)
        return;

    fprintf(stderr, "[RTMP] connection timer fired. rtmp status : %d.\n", rtmp_ready);

    if (_checkCpuLoad() == FAILURE) {
        // disable rtmp_ready in advanced.
        rtmp_ready = false;

        fprintf(stderr, "[RTMP] the cpu load is too high.\n");
        int r = uv_async_send(async_rtmp_disconnect);
        return;
    }

    if (!rtmp_ready) {
        if (app_cfg->ste.b.usbnet_run == 0 && app_cfg->ste.b.cradle_eth_run == 0) {
            fprintf(stderr, "[RTMP] network unavailable. try next time. usbnet_run: %d, cradle_eth_run: %d\n",
                app_cfg->ste.b.usbnet_run, app_cfg->ste.b.cradle_eth_run);
            return;
        }

        int r = uv_async_send(async_rtmp_connect);
    }
}

void rtmp_disconnect_async_cb(uv_async_t* async) {
    fprintf(stderr, "[RTMP] try to disconnect : %s.\n", rtmp_endpoint);
    _rtmp_close();
}

void rtmp_connect_async_cb(uv_async_t* async) {
    // close first
    _rtmp_close();

    fprintf(stderr, "[RTMP] try to connect : %s.\n", rtmp_endpoint);

    // librtmp
    rtmp = srs_rtmp_create(rtmp_endpoint);
    if (srs_rtmp_handshake(rtmp) != 0) {
        srs_human_trace("[RTMP] simple handshake failed.");
        return;
    }
    srs_human_trace("[RTMP] simple handshake success");

    if (srs_rtmp_connect_app(rtmp) != 0) {
        srs_human_trace("[RTMP] connect vhost/app failed.");
        return;
    }
    srs_human_trace("[RTMP] connect vhost/app success");

    if (srs_rtmp_publish_stream(rtmp) != 0) {
        srs_human_trace("[RTMP] publish stream failed.");
        return;
    }
    srs_human_trace("[RTMP] publish stream success");

    // set the default timeout
    srs_rtmp_set_timeout(rtmp, RTMP_TIMEOUT, RTMP_TIMEOUT);

    rtmp_ready = true;
}

int app_rtmp_start(void)
{
    int r;

    // stream queue.
    QUEUE_INIT(&queue);

    async_rtmp_connect = malloc(sizeof(uv_async_t));
    r = uv_async_init(loop_video, async_rtmp_connect, rtmp_connect_async_cb);

    async_rtmp_disconnect = malloc(sizeof(uv_async_t));
    r = uv_async_init(loop_video, async_rtmp_disconnect, rtmp_disconnect_async_cb);

    async_rtmp_publish = malloc(sizeof(uv_async_t));
    r = uv_async_init(loop_video, async_rtmp_publish, rtmp_publish_async_cb);

    // connection check timer in default loop.
    timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);

	return SOK;
}

void app_rtmp_stop(void)
{
	uv_timer_stop(timer);

	uv_close((uv_handle_t*)&timer, NULL);
    uv_close((uv_handle_t*)async_rtmp_connect, NULL);
    uv_close((uv_handle_t*)async_rtmp_disconnect, NULL);
    uv_close((uv_handle_t*)async_rtmp_publish, NULL);

    // close
    _rtmp_close();
}

void app_rtmp_publish_video(stream_info_t *ifr)
{
    if (rtmp_enabled == false  || rtmp_ready == false)
        return;

    int r;
    if (_checkAsyncQueue() == FAILURE) {
        // disable rtmp_ready in advanced.
        rtmp_ready = false;

        // disconect in the next eventloop.
        r = uv_async_send(async_rtmp_disconnect);
        return;
    }

    struct stream_s* stream = malloc(sizeof(struct stream_s));
    stream->ifr = ifr;
    QUEUE_INIT(&stream->node);

    uv_mutex_lock(&mutex);
    int length = ++queue_lenth;
    QUEUE_INSERT_TAIL(&queue, &stream->node);
    uv_mutex_unlock(&mutex);

    // fprintf(stderr, "[RTMP] enqueue. length : %d.\n", length);

    // fire async_cb in loop_video
    r = uv_async_send(async_rtmp_publish);
}

void app_rtmp_enable(void)
{
    fprintf(stderr, "[RTMP] app_rtmp_enable.\n");
	rtmp_enabled = true;

    // check the connection on every 10sec
    uv_timer_start(timer, rtmp_connect_timer_cb, 0, 1000 * 10);
}

void app_rtmp_disable(void)
{
    fprintf(stderr, "[RTMP] app_rtmp_disable.\n");
	rtmp_enabled = false;

	uv_timer_stop(timer);

    // close
    int r = uv_async_send(async_rtmp_disconnect);
}

void app_rtmp_set_endpoint(const char* endpoint)
{
    fprintf(stderr, "[RTMP] set endpoint from %s to %s.\n", rtmp_endpoint, endpoint);

    // fixme : validate the endpoint
	strcpy(rtmp_endpoint, endpoint);
}

const char* app_rtmp_get_endpoint()
{
	return rtmp_endpoint;
}

#endif // USE_RTMP