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

		// admin password
		int  chk_change_web_passwd = -1;
		char curpw[128]={0};
		char newpw1[128]={0};
		char newpw2[128]={0};

		int  live_stream_account_enable = -1; // if unchecked, it is not delivered
		int  live_stream_account_enctype = -1; // if unchecked, it is not delivered
		char live_stream_account_id[32] ={0};
		char live_stream_account_pw[32] ={0};

		char onvif_id[32] ={0};
		char onvif_pw[64] ={0};

		T_CGI_USER_CONFIG t; memset(&t, 0, sizeof t);

        for(int i=0;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "chk_change_web_passwd")){
				chk_change_web_passwd = atoi(prm[i].value);
			}
            else if(!strcmp(prm[i].name, "txt_cur_pw")){
                sprintf(curpw, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_new_pw1")){
                sprintf(newpw1, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_new_pw2")){
                sprintf(newpw2, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_onvif_id")){
                sprintf(onvif_id, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_onvif_pw")){
                sprintf(onvif_pw, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "live_stream_account_enable")){
                live_stream_account_enable = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "cbo_live_stream_account_enc_type")){
                live_stream_account_enctype = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_live_stream_account_id")){
                sprintf(live_stream_account_id, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_live_stream_account_pw")){
                sprintf(live_stream_account_pw, "%s", prm[i].value);
            }
        }

		// verify to change web password
		if(chk_change_web_passwd == -1) {
			CGI_DBG("Not received chk_change_web_passwd:-1\n");
			return ERR_INVALID_PARAM;
		}
		else if(chk_change_web_passwd == 1) {

			if( strlen(curpw) < 1) {
				CGI_DBG("Current Web Password's length < 1\n");
				return ERR_INVALID_PARAM;
			}

			//  confirm current password 
			T_CGI_ACCOUNT acc;
			sprintf(acc.id, "%s", "admin");
			sprintf(acc.pw, "%s", curpw);
			if(0 != sysctl_message(UDS_CMD_CHECK_ACCOUNT, (void*)&acc, sizeof(acc))) {
				return ERR_INVALID_ACCOUNT;
			}
			
			if( strlen(newpw1) < 1) {
				CGI_DBG("New Password1's length < 1\n");
				return ERR_INVALID_PARAM;
			}

			if( strcmp(newpw1, newpw2) != 0) {
				CGI_DBG("Password1 are different, pw1:%s, pw2:%s\n", newpw1, newpw2);
				return ERR_INVALID_PARAM;
			}

			sprintf(t.web.id, "%s", "admin"); // if this is "admin", update password with newpw1, on app_uds
			sprintf(t.web.pw, "%s", newpw1);
		}
	
		//	check onvif
		if(strcmp(onvif_id, "admin")!=0){
			CGI_DBG("Invalid onvif id:%s\n", onvif_id);
			return ERR_INVALID_PARAM;
		}
		if(strlen(onvif_pw) < 1 || strlen(onvif_pw) >64){
			CGI_DBG("Invalid onvif pw:%s length = %d\n", onvif_pw, strlen(onvif_pw));
			return ERR_INVALID_PARAM;
		}
		sprintf(t.onvif.id, "%s", onvif_id);
		sprintf(t.onvif.pw, "%s", onvif_pw);
		CGI_DBG("################# onvif pw:%s\n", onvif_pw);

		// check not null
#if defined(FITT360_SECURITY)
		if( live_stream_account_enable == -1)
		{
			CGI_DBG("Not received RTSP Account Enable\n");
			return ERR_INVALID_PARAM;
		}
		else if( live_stream_account_enable == 1)
		{
			if(live_stream_account_enctype  == -1) {
				CGI_DBG("Not received RTSP Account Enctype\n");
				return ERR_INVALID_PARAM;
			}
			if(strlen(live_stream_account_id) < 1){
				CGI_DBG("RTSP ID len < 1\n");
				return ERR_INVALID_PARAM;
			}
			if(strlen(live_stream_account_pw) < 1){
				CGI_DBG("RTSP PW len < 1\n");
				return ERR_INVALID_PARAM;
			}
		}
#elif defined(NEXXONE) || defined(NEXX360W) || defined(NEXX360M) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
	|| defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
		live_stream_account_enable=1; // FIXED
		if(live_stream_account_enctype  == -1) {
			CGI_DBG("Not received RTSP Account Enctype\n");
			return ERR_INVALID_PARAM;
		}
		if(strlen(live_stream_account_id) < 1){
			CGI_DBG("RTSP ID len < 1\n");
			return ERR_INVALID_PARAM;
		}
		if(strlen(live_stream_account_pw) < 1){
			CGI_DBG("RTSP PW len < 1\n");
			return ERR_INVALID_PARAM;
		}
#else
	#error  check PRODUCT_NAME
#endif

		t.rtsp.enable = live_stream_account_enable;
		t.rtsp.enctype = live_stream_account_enctype;
		sprintf(t.rtsp.id, "%s", live_stream_account_id);
		sprintf(t.rtsp.pw, "%s", live_stream_account_pw);

        // Must finish parsing before free.
        if(isPOST){ free(contents); }


        if(0>sysctl_message(UDS_SET_USER_CONFIG, (void*)&t, sizeof t)) {
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
