#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qdecoder.h>

#include "cgi.h"
#include "cgi_param.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_user.cgi"
#define HOME_PATH "../index.html"

static int submit_settings()
{
    int isPOST = 0;
    T_CGIPRM prm[128];

    RSA *cryptClient, *cryptServer;
    validateRsaSession(&cryptClient, &cryptServer);

	// fixme : enough for rsa bitsize + base64 + padding.
	unsigned char buffer[RSA_size(cryptClient) * 2];

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
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

        char *str= req->getstr(req, "chk_change_web_passwd", false);
        if (str != NULL) { chk_change_web_passwd = atoi(str); }

        str= req->getstr(req, "txt_cur_pw", false);
        if (str != NULL) {
            CGI_DBG("txt_cur_pw  %s\n", str);
            int len = rsa_base64_de(cryptServer, str, buffer); // fixme : assert
            sprintf(curpw, "%s", buffer); 
            CGI_DBG("txt_cur_pw de(rsa+base64): %s\n",  buffer);
        }
        str= req->getstr(req, "txt_new_pw1", false);
        if (str != NULL) {
            CGI_DBG("txt_new_pw1  %s\n", str);
            int len = rsa_base64_de(cryptServer, str, buffer); // fixme : assert
            sprintf(newpw1, "%s", buffer); 
            CGI_DBG("txt_new_pw1 de(rsa+base64): %s\n",  buffer);
        }
        str= req->getstr(req, "txt_new_pw2", false);
        if (str != NULL) {
            CGI_DBG("txt_new_pw2  %s\n", str);
            int len = rsa_base64_de(cryptServer, str, buffer); // fixme : assert
            sprintf(newpw2, "%s", buffer); 
            CGI_DBG("txt_new_pw2 de(rsa+base64): %s\n",  buffer);
        }
        str= req->getstr(req, "txt_onvif_id", false);
        if (str != NULL) { sprintf(onvif_id, "%s", str); }
        str= req->getstr(req, "txt_onvif_pw", false);
        if (str != NULL) {
            CGI_DBG("txt_onvif_pw  %s\n", str);
            int len = rsa_base64_de(cryptServer, str, buffer); // fixme : assert
            sprintf(onvif_pw, "%s", buffer); 
            CGI_DBG("txt_onvif_pw de(rsa+base64): %s\n",  buffer);
        }
        str= req->getstr(req, "live_stream_account_enable", false);
        if (str != NULL) { live_stream_account_enable = atoi(str); }
        str= req->getstr(req, "cbo_live_stream_account_enc_type", false);
        if (str != NULL) { live_stream_account_enctype = atoi(str); }
        str= req->getstr(req, "txt_live_stream_account_id", false);
        if (str != NULL) { sprintf(live_stream_account_id, "%s", str); }
        str= req->getstr(req, "txt_live_stream_account_pw", false);
        if (str != NULL) {
            CGI_DBG("txt_live_stream_account_pw  %s\n", str);
            int len = rsa_base64_de(cryptServer, str, buffer); // fixme : assert
            sprintf(live_stream_account_pw, "%s", buffer); 
            CGI_DBG("txt_live_stream_account_pw de(rsa+base64): %s\n",  buffer);
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
	// sesseion check
    validateSession();

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
