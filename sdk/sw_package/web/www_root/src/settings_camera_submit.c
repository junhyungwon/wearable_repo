#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_camera.cgi"
#define HOME_PATH "../index.html"

static int submit_settings()
{
    int isPOST = 0;
    T_CGIPRM prm[128];
	const int recording_kbps[] = {512, 1000, 2000, 3000, 4000};
	const int streaming_kbps[] = {128, 256, 512, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
	const int MAX_CNT_REC_KBPS = sizeof(recording_kbps)/sizeof(int);
	const int MAX_CNT_STM_KBPS = sizeof(streaming_kbps)/sizeof(int);

    char *method = getenv("REQUEST_METHOD");
	if(method == NULL) return ERR_INVALID_METHOD;
    CGI_DBG("method : %s\n", method);

    char *contents=NULL;
    if(0 == strcmp(method, "POST")){
        contents = get_cgi_contents();
        isPOST = TRUE;
    }
    else {
        contents = getenv("QUERY_STRING");
        isPOST = FALSE;
    }

	if(contents == NULL) return ERR_INVALID_PARAM;
    CGI_DBG("contents:%s\n", contents);

    int cnt = parseContents(contents, prm);
    CGI_DBG("cnt:%d\n", cnt);

    if(cnt>0){

        int i=0;
		int rec_fps=-1,           rec_bps_idx=-1,rec_gop=-1,rec_rc=-1;
		int stm_res=-1,stm_fps=-1,stm_bps_idx=-1,stm_gop=-1,stm_rc=-1;

        for(;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "rec_fps")){
                rec_fps= atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "rec_bps")){
                rec_bps_idx= atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "rec_gop")){
                rec_gop = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "rec_rc")){
                rec_rc = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "stm_resolution")){
                stm_res= atoi(prm[i].value);
				stm_res = 2 - stm_res; // 0:1080p, 1:720p, 2:480p
            } 
            else if(!strcmp(prm[i].name, "stm_fps")){
                stm_fps= atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "stm_bps")){
                stm_bps_idx= atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "stm_gop")){
                stm_gop = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "stm_rc")){
                stm_rc = atoi(prm[i].value);
            }
        }
		
		if( rec_fps == -1 || rec_bps_idx == -1 || rec_gop == -1 || rec_rc==-1
		||  stm_res < 0 || stm_res > 2 || stm_fps == -1 || stm_bps_idx == -1 || stm_gop == -1 || stm_rc==-1 ){
			CGI_DBG("Invalid Parameter\n");
			CGI_DBG("rec_fps:%d, rec_bps_idx:%d, rec_gop:%d, rec_rc:%d\n", rec_fps, rec_bps_idx, rec_gop, rec_rc);
			CGI_DBG("stm_res:%d, stm_fps:%d, stm_bps_idx:%d, stm_gop:%d, stm_rc:%d\n", stm_res, stm_fps, stm_bps_idx, stm_gop, stm_rc);
			return ERR_INVALID_PARAM;
		}

        CGI_DBG("rec_fps:%d, rec_bps_idx:%d, rec_gop:%d, rec_rc:%d\n", rec_fps, rec_bps_idx, rec_gop, rec_rc);
        CGI_DBG("stm_res:%d, stm_fps:%d, stm_bps_idx:%d, stm_gop:%d, stm_rc:%d\n", stm_res, stm_fps, stm_bps_idx, stm_gop, stm_rc);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        // check parameter values

        T_CGI_VIDEO_QUALITY vq;

#if defined(NEXXONE) || defined(NEXX360B) || defined(NEXX360W) || defined(NEXX360H) || defined(NEXXB) || defined(NEXX360W_MUX)
		// Record Options
		int rec_kbps = 512;
		if(rec_bps_idx >= 0 && rec_bps_idx < MAX_CNT_REC_KBPS)
			rec_kbps = recording_kbps[rec_bps_idx];

		vq.rec.fps = rec_fps+1;
		vq.rec.bps = rec_kbps;

		// Streaming Options
		int stm_kbps = 512;
		if(stm_bps_idx >= 0 && stm_bps_idx < MAX_CNT_STM_KBPS)
			stm_kbps = streaming_kbps[stm_bps_idx];
		
		vq.stm.fps = stm_fps+1;
		vq.stm.bps = stm_kbps;

#elif defined(FITT360_SECURITY)
		vq.rec.fps = rec_fps;
		vq.rec.bps = rec_bps_idx;  // fitt360은  인덱스로 사용중...
		vq.stm.fps = stm_fps;
		vq.stm.bps = stm_bps_idx;  // fitt360은  인덱스로 사용중...
#else
#error "Invalid Product Name!!!"
#endif
		vq.rec.gop = rec_gop;
		vq.rec.rc  = rec_rc;
		vq.stm.res = stm_res;
		vq.stm.gop = stm_gop;
		vq.stm.rc  = stm_rc;

        if(0!=sysctl_message(UDS_SET_VIDEO_QUALITY, (void*)&vq, sizeof(vq))) {
            return SUBMIT_ERR;
        }

        return SUBMIT_OK;
    }
    else {
        return SUBMIT_ERR;
    }

}

int main(int argc, char *argv[])
{
	int nError = submit_settings();

	send_response(nError);

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect(REDIRECT_CGI_PATH, 2);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
