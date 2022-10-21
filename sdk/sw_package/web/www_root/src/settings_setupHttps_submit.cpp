#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

#define REDIRECT_CGI_PATH "/"
#define CGI_FORM_PATH "settings_setupHttps.cgi"
#define HOME_PATH "/"

#define PASS_RETURNS_TO_VUEJS 0

int https_mode = 0;

static int submit_settings_qcgi()
{
    int ret=SUBMIT_NO_CHANGE;

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
        https_mode = -1;

        char *str = req->getstr(req, "https_mode", false);
        if(str != NULL) { https_mode = atoi(str);}

        if( https_mode == -1) {
            return ERR_INVALID_PARAM;
        }

        T_CGI_HTTPS_CONFIG t;
		memset(&t, 0, sizeof t);
        t.https_mode = https_mode;

        if(https_mode == 1) {
            // 이 값들로 Self Signed Certificate 생성
            str = req->getstr(req, "txt_C", false);
            if(str != NULL) { sprintf(t.C, "%s", str); }
            str = req->getstr(req, "txt_ST", false);
            if(str != NULL) { sprintf(t.ST, "%s", str); }
            str = req->getstr(req, "txt_L", false);
            if(str != NULL) { sprintf(t.L, "%s", str); }
            str = req->getstr(req, "txt_O", false);
            if(str != NULL) { sprintf(t.O, "%s", str); }
            str = req->getstr(req, "txt_OU", false);
            if(str != NULL) { sprintf(t.OU, "%s", str); }
            str = req->getstr(req, "txt_CN", false);
            if(str != NULL) { sprintf(t.CN, "%s", str); }
            str = req->getstr(req, "txt_Email", false);
            if(str != NULL) { sprintf(t.Email, "%s", str); }

        }
        else if (https_mode == 2) {
            // pem 생성 및 경로 저장
        }

        ret = sysctl_message(UDS_SET_HTTPS_CONFIG, (void*)&t, sizeof t );
        CGI_DBG("ret:%d\n", ret);
        if(0 > ret) {
            return SUBMIT_ERR;
        }
		else if( ret == ERR_NO_CHANGE) {
			return SUBMIT_NO_CHANGE;
		}

        return SUBMIT_OK;
    }

    return SUBMIT_ERR;
}

int main(int argc, char *argv[])
{
	// sesseion check
    validateSession();

	int nError;

	nError = submit_settings_qcgi();

    // for VUEJS
#if PASS_RETURNS_TO_VUEJS
	send_response(nError);
#else 
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

        if (https_mode == 1)
            wait_redirect(REDIRECT_CGI_PATH, 20); // SSC 만들때 시간이 좀 필요합니다
        else 
            wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
