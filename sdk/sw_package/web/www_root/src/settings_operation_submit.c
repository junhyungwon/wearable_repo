#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_operation.cgi"
#define HOME_PATH "../index.html"

static int submit_settings_fitt360()
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

		int  i=0;
		int  pre_rec=-1;
		int  auto_rec =-1;
		int  audio_rec =-1;
		int  rec_interval=-1;
		int  rec_overwrite=-1;
		int  display_datetime=-1;

		for(;i<cnt;i++) {

			CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
			if(!strcmp(prm[i].name, "pre_rec")){
				pre_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "auto_rec")){
				auto_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "audio_rec")){
				audio_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "rec_interval")){
				rec_interval= atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "rec_overwrite")){
				rec_overwrite= atoi(prm[i].value);
			}
			else if(!strcmp(prm[i].name, "display_datetime")){
				display_datetime = atoi(prm[i].value);
			}
		}

		if( pre_rec == -1 || audio_rec == -1 || auto_rec == -1 || rec_interval == -1 || rec_overwrite == -1 
				|| display_datetime == -1 ){
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

		CGI_DBG("pre_rec:%d, auto_rec:%d, audio_rec:%d, rec_interval:%d, rec_overwrite:%d, display_datetime:%d\n", 
				pre_rec, auto_rec, audio_rec, rec_interval, rec_overwrite, display_datetime);

		// Must finish parsing before free.
		if(isPOST){ free(contents); }

		// check parameter values
		T_CGI_OPERATION_CONFIG t;
		memset(&t, 0, sizeof(t));
		t.rec.pre_rec      = pre_rec;
		t.rec.auto_rec     = auto_rec;
		t.rec.audio_rec    = audio_rec;
		t.rec.interval     = rec_interval;
		t.rec.overwrite    = rec_overwrite;
		t.display_datetime = display_datetime;

		if(0 != sysctl_message(UDS_SET_OPERATION_CONFIG, (void*)&t, sizeof t )) {
			return SUBMIT_ERR;
		}

		return SUBMIT_OK;
	}
	else {
		return SUBMIT_ERR;
	}
}

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

		int  i=0;
		int  pre_rec=-1;
		int  auto_rec =-1;
		int  audio_rec =-1;
		int  rec_interval=-1;
		int  rec_overwrite=-1;
		int  display_datetime=-1;

		for(;i<cnt;i++) {

			CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
			if(!strcmp(prm[i].name, "pre_rec")){
				pre_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "auto_rec")){
				auto_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "audio_rec")){
				audio_rec = atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "rec_interval")){
				rec_interval= atoi(prm[i].value);
			} 
			else if(!strcmp(prm[i].name, "rec_overwrite")){
				rec_overwrite= atoi(prm[i].value);
			}
			else if(!strcmp(prm[i].name, "display_datetime")){
				display_datetime = atoi(prm[i].value);
			}
		}

		if( pre_rec == -1 || audio_rec == -1 || auto_rec == -1 || rec_interval == -1 || rec_overwrite == -1 
				|| display_datetime == -1 ){
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

		CGI_DBG("pre_rec:%d, auto_rec:%d, audio_rec:%d, rec_interval:%d, rec_overwrite:%d, display_datetime:%d\n", 
				pre_rec, auto_rec, audio_rec, rec_interval, rec_overwrite, display_datetime);

		// Must finish parsing before free.
		if(isPOST){ free(contents); }

		// check parameter values
		T_CGI_OPERATION_CONFIG t;
		memset(&t, 0, sizeof(t));
		t.rec.pre_rec       = pre_rec;
		t.rec.auto_rec      = auto_rec;
		t.rec.audio_rec     = audio_rec;
		t.rec.interval      = rec_interval;
		t.rec.overwrite     = rec_overwrite;
		t.display_datetime = display_datetime;

		if(0 != sysctl_message(UDS_SET_OPERATION_CONFIG, (void*)&t, sizeof t )) {
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
	int nError = SUBMIT_ERR;

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
