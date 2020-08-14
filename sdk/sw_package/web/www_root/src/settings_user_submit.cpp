#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_user.cgi"
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

		char curpw[128]={0};
		char newpw1[128]={0};
		char newpw2[128]={0};

        for(int i=0;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "txt_cur_pw")){
                sprintf(curpw, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_new_pw1")){
                sprintf(newpw1, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_new_pw2")){
                sprintf(newpw2, "%s", prm[i].value);
            }
        }
		
		if( strlen(curpw) < 1 || strlen(newpw1) < 1 || strlen(newpw2) < 1 ) {
			CGI_DBG("Password is null\n");
			return ERR_INVALID_PARAM;
		}
		if( strlen(newpw1) < 1) {
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

        CGI_DBG("curpw:%s, newpw1:%s, newpw2:%s\n", curpw, newpw1, newpw2);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }

		T_CGI_ACCOUNT acc;
        sprintf(acc.id, "%s", "admin");
        sprintf(acc.pw, "%s", curpw);

        if(0 != sysctl_message(UDS_CMD_CHECK_ACCOUNT, (void*)&acc, sizeof(acc))) {
            return ERR_INVALID_ACCOUNT;
        }

        T_CGI_USER user;
        sprintf(user.id, "%s", "admin");
        sprintf(user.pw, "%s", newpw1);
        user.lv = USER_LV_ADMINISTRATOR;
        user.authtype = 0;

        if(0!=sysctl_message(UDS_CMD_UPDATE_USER, (void*)&user, sizeof(user))) {
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
		wait_redirect(REDIRECT_CGI_PATH, 2);
	}
	else if(nError == ERR_INVALID_ACCOUNT){
		refresh_page(REDIRECT_CGI_PATH, nError);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
