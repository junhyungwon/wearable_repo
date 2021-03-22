#ifdef USE_RTMP

#include "app_comm.h"
#include "app_libuv.h"
#include "app_rtmp.h"

// wowza. fixme : need a config file
// $ ffplay rtmp://3.35.55.222:1935/live2/myStream/key
const char *rtmp_endpoint  = "rtmp://3.35.55.222:1935/live2/myStream/key";

uv_async_t *async_rtmp_connect;
uv_timer_t *timer;

// librtmp
static srs_rtmp_t rtmp = NULL;
static int rtmp_ready = 0;

void rtmp_connect_timer_cb (uv_timer_t* timer, int status) {
    fprintf(stderr, "[RTMP] timer check rtmp status %d.\n", rtmp_ready);
    if (rtmp_ready == 0) {
        int r = uv_async_send(async_rtmp_connect);
    }
}

void rtmp_connect_async_cb(uv_async_t* async) {
    rtmp_ready = 0;

    // close first
    if (rtmp != NULL)
        srs_rtmp_destroy(rtmp);

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

    rtmp_ready = 1;
}


void rtmp_video_async_cb(uv_async_t* async) {
    // fixme : imem will be in a short time.
    stream_info_t *ifr = (stream_info_t*)async->data;

    // fprintf(stderr, "async_cb %d\n", ifr->b_size);
    static int isFirst = 0;
    if (isFirst == 0 && ifr->is_key) {
        isFirst = 1;
    }

    if (isFirst == 1 && rtmp_ready == 1) {
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
                srs_human_trace("[RTMP] ignore duplicated pps, code=%d", ret);
            } else {
                srs_human_trace("[RTMP] send h264 raw data failed. ret=%d", ret);

                rtmp_ready = 0;
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

    uv_close((uv_handle_t*)async, NULL);
}


int app_rtmp_start(void)
{
    async_rtmp_connect = malloc(sizeof(uv_async_t));
    int r = uv_async_init(loop, async_rtmp_connect, rtmp_connect_async_cb);

    // fixme : keep the loop alive. timer every 10s
    timer = malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    uv_timer_start(timer, rtmp_connect_timer_cb, 1000* 10, 1000* 10);

	return SOK;
}

void app_rtmp_stop(void)
{
	uv_timer_stop(timer);
}


#endif // USE_RTMP