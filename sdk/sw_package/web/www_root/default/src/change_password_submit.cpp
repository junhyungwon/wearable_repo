#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"

static int submit_settings()
{
    int isPOST = 0;
    T_CGIPRM prm[128];
	memset(prm, 0, sizeof prm);

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

        char pw1[256]={0};
        char pw2[256]={0};
        int  authtype= -1; // 0:basic, 1:digest(default)

        for(int i=0;i<cnt;i++) {
            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "password1")){
                sprintf(pw1, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "password2")){
                sprintf(pw2, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "authtype")){
                authtype= atoi(prm[i].value);
            }
        }

        CGI_DBG("pw1:%s, pw2:%s, authtype:%d\n", pw1, pw2, authtype);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        // check parameter values
        if(strlen(pw1) < 1 || strcmp(pw1, pw2) || authtype == -1){
            CGI_DBG("Invalid parameter\n");
            return SUBMIT_ERR;
        }

		T_CGI_USER acc;
        acc.lv = USER_LV_ADMINISTRATOR;
        acc.authtype = authtype;
        sprintf(acc.id, "%s", "admin");
        sprintf(acc.pw, "%s", pw1);

        if(0!=sysctl_message(UDS_CMD_CHANGE_PASSWORD, (void*)&acc, sizeof(acc))) {
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

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect("../index.html", 5);
	}
	else{
		// return home
		refresh_page("../index.html", nError);
	}
#else
	send_response(nError);
#endif

	return 0;
}
