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
		int rec_fps=-1,rec_bps=-1,rec_gop=-1,rec_rc=-1;
		int stm_res=-1,stm_fps=-1,stm_bps=-1,stm_gop=-1,stm_rc=-1;

        for(;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "rec_fps")){
                rec_fps= atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "rec_bps")){
                rec_bps= atoi(prm[i].value);
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
                stm_bps= atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "stm_gop")){
                stm_gop = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "stm_rc")){
                stm_rc = atoi(prm[i].value);
            }
        }
		
		if( rec_fps == -1 || rec_bps == -1 || rec_gop == -1 || rec_rc==-1
		||  stm_res < 0 || stm_res > 2 || stm_fps == -1 || stm_bps == -1 || stm_gop == -1 || stm_rc==-1 ){
			CGI_DBG("Invalid Parameter\n");
			CGI_DBG("rec_fps:%d, rec_bps:%d, rec_gop:%d, rec_rc:%d\n", rec_fps, rec_bps, rec_gop, rec_rc);
			CGI_DBG("stm_res:%d, stm_fps:%d, stm_bps:%d, stm_gop:%d, stm_rc:%d\n", stm_res, stm_fps, stm_bps, stm_gop, stm_rc);
			return ERR_INVALID_PARAM;
		}

        CGI_DBG("rec_fps:%d, rec_bps:%d, rec_gop:%d, rec_rc:%d\n", rec_fps, rec_bps, rec_gop, rec_rc);
        CGI_DBG("stm_res:%d, stm_fps:%d, stm_bps:%d, stm_gop:%d, stm_rc:%d\n", stm_res, stm_fps, stm_bps, stm_gop, stm_rc);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        // check parameter values

        T_CGI_VIDEO_QUALITY vq;
		vq.rec.fps = rec_fps;
		vq.rec.bps = rec_bps;
		vq.rec.gop = rec_gop;
		vq.rec.rc  = rec_rc;
		vq.stm.res = stm_res; // 0:480p, 1:720p, 2:1080p
		vq.stm.fps = stm_fps;
		vq.stm.bps = stm_bps;
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
