#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_system.cgi"
#define HOME_PATH "../index.html"

static int submit_settings()
{
    int ret, isPOST = 0;
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

        T_CGI_SYSTEM_CONFIG t;
		memset(&t, 0, sizeof t);

        for(int i=0;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "txt_devid")){
                sprintf(t.devid, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_uid")){
                sprintf(t.uid, "%s", prm[i].value);
            }
        }
		
		if( strlen(t.devid) < 1) {
			CGI_DBG("DEVICEID Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

#if defined(NEXXONE) || defined(NEXX360B) || defined(NEXX360W) || defined(NEXX360H) || defined(NEXXB)
		if( strlen(t.uid) < 1) {
			CGI_DBG("UID Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}
#endif

        CGI_DBG("devid:%s, uid:%s\n", t.devid, t.uid);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        ret = sysctl_message(UDS_SET_SYSTEM_CONFIG, (void*)&t, sizeof t );
        if(0 > ret) {
            return SUBMIT_ERR;
        } else if(0 == ret) {
            return SUBMIT_NO_CHANGE;
        }

        return SUBMIT_OK;
    }
    else {
        return SUBMIT_ERR;
    }

}


int main(int argc, char *argv[])
{
	int nError;

	nError = submit_settings();

	send_response(nError);

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
