#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_network.cgi"
#define HOME_PATH "../index.html"

static int submit_settings_qcgi()
{
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
		int  len = 0, ret = 0;
		int  wireless_iptype=-1;
		char wireless_ipv4[32]={0};
		char wireless_gw[32]={0};
		char wireless_mask[32]={0};

		int  cradle_iptype=-1;
		char cradle_ipv4[32]={0};
		char cradle_gw[32]={0};
		char cradle_mask[32]={0};

		char wifiap_ssid[128]={0};
		char wifiap_pw[128]={0};

		int  live_stream_account_enable = -1; // if unchecked, it is not delivered
		int  live_stream_account_enctype = -1; // if unchecked, it is not delivered
		char live_stream_account_id[32] ={0};
		char live_stream_account_pw[32] ={0};

        char *str= req->getstr(req, "cbo_wireless_ip_type", false);
        if (str != NULL) {
            wireless_iptype = atoi(str);
        }
        str= req->getstr(req, "txt_wireless_ipv4", false);
        if (str != NULL) {
            sprintf(wireless_ipv4, "%s", str);
        }
        str= req->getstr(req, "txt_wireless_gw", false);
        if(str != NULL) {
            sprintf(wireless_gw, "%s", str);
        } 
        str = req->getstr(req, "txt_wireless_mask", false);
		if(str != NULL) {
            sprintf(wireless_mask, "%s", str);
        } 
        str = req->getstr(req, "cbo_cradle_ip_type", false);
		if(str != NULL) {
            cradle_iptype = atoi(str);
        } 
        str = req->getstr(req, "txt_cradle_ipv4", false);
		if(str != NULL) {
            sprintf(cradle_ipv4, "%s", str);
        } 
        str = req->getstr(req, "txt_cradle_gw", false);
		if(str != NULL) {
            sprintf(cradle_gw, "%s", str);
        } 
        str = req->getstr(req, "txt_cradle_mask", false);
		if(str != NULL) {
            sprintf(cradle_mask, "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_ssid", false);
		if(str != NULL) {
            sprintf(wifiap_ssid, "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_pw", false);
		if(str != NULL) {
            sprintf(wifiap_pw, "%s", str);
        }

        CGI_DBG("wireless_iptype:%d, wireless_ipv4:%s, wireless_gw:%s, wireless_mask:%s\n", 
				wireless_iptype, wireless_ipv4, wireless_gw, wireless_mask);
        CGI_DBG("cradle_iptype:%d, cradle_ipv4:%s, cradle_gw:%s, cradle_mask:%s\n", 
				cradle_iptype, cradle_ipv4, cradle_gw, cradle_mask);
        CGI_DBG("wifi_ap, ssid:%s, pw:%s\n", wifiap_ssid, wifiap_pw);

		// check not null
		if( wireless_iptype == -1 || cradle_iptype == -1 )//|| live_stream_account_enable == -1) 
		{
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}


		len = strlen(wifiap_ssid);
		if( len == 0 || len > 32)
		{
			CGI_DBG("Invalid Parameter, WIFI SSID , len=%d\n", len);
			return ERR_INVALID_PARAM;
		}

		len = strlen(wifiap_pw);
		if ( len != 0 && (len < 8 || len > 64) ) // Allow NULL password. 2020.02.26
		{
			CGI_DBG("Invalid Parameter, wifi password, len=%d\n", len);
			return ERR_INVALID_PARAM;
		}

		if( wireless_iptype == 0
		&& (strlen(wireless_ipv4) == 0
		||  strlen(wireless_gw  ) == 0
		||  strlen(wireless_mask) == 0 ))
	   	{
			CGI_DBG("Invalid Parameter, wireless\n");
			return ERR_INVALID_PARAM;
		}
		if( cradle_iptype == 0
		&& (strlen(cradle_ipv4) == 0
		||  strlen(cradle_gw  ) == 0
		||  strlen(cradle_mask) == 0 ))
	   	{
			CGI_DBG("Invalid Parameter, cradle\n");
			return ERR_INVALID_PARAM;
		}
#if 0
		if( live_stream_account_enable == 1
		&& (live_stream_account_enctype  == -1
		||  strlen(live_stream_account_id) == 0
		||  strlen(live_stream_account_pw) == 0
		    )
		)
	   	{
			CGI_DBG("Invalid Parameter, live stream account\n");
			return ERR_INVALID_PARAM;
		}
#endif
        // check parameter values
        T_CGI_NETWORK_CONFIG t;
		memset(&t, 0, sizeof(T_CGI_NETWORK_CONFIG));

		t.wireless.addr_type         = wireless_iptype;
		if(t.wireless.addr_type == 0) { // static
			sprintf(t.wireless.ipv4, "%s", wireless_ipv4);
			sprintf(t.wireless.gw,   "%s", wireless_gw);
			sprintf(t.wireless.mask, "%s", wireless_mask);
		}

		t.cradle.addr_type         = cradle_iptype;
		if(t.cradle.addr_type == 0) { // static
			sprintf(t.cradle.ipv4, "%s", cradle_ipv4);
			sprintf(t.cradle.gw,   "%s", cradle_gw);
			sprintf(t.cradle.mask, "%s", cradle_mask);
		}

		strncpy(t.wifi_ap.id, wifiap_ssid, strlen(wifiap_ssid));
		strncpy(t.wifi_ap.pw, wifiap_pw,   strlen(wifiap_pw));

#if 0
		t.live_stream_account_enable  = live_stream_account_enable;
		t.live_stream_account_enctype = live_stream_account_enctype;
		if(live_stream_account_enable){
			strcpy(t.live_stream_account.id, live_stream_account_id);
			strcpy(t.live_stream_account.pw, live_stream_account_pw);
		}
#endif

        ret = sysctl_message(UDS_SET_NETWORK_CONFIG, (void*)&t, sizeof(T_CGI_NETWORK_CONFIG));
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
static int submit_settings()
{
	int len=0;
    int ret,isPOST = 0;
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

		int  wireless_iptype=-1;
		char wireless_ipv4[32]={0};
		char wireless_gw[32]={0};
		char wireless_mask[32]={0};

		int  cradle_iptype=-1;
		char cradle_ipv4[32]={0};
		char cradle_gw[32]={0};
		char cradle_mask[32]={0};

		char wifiap_ssid[128]={0};
		char wifiap_pw[128]={0};

		int  live_stream_account_enable = -1; // if unchecked, it is not delivered
		int  live_stream_account_enctype = -1; // if unchecked, it is not delivered
		char live_stream_account_id[32] ={0};
		char live_stream_account_pw[32] ={0};

        for(;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);

            if(!strcmp(prm[i].name, "cbo_wireless_ip_type")){
                wireless_iptype = atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_wireless_ipv4")){
                sprintf(wireless_ipv4, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_wireless_gw")){
                sprintf(wireless_gw, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_wireless_mask")){
                sprintf(wireless_mask, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "cbo_cradle_ip_type")){
                cradle_iptype = atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_cradle_ipv4")){
                sprintf(cradle_ipv4, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_cradle_gw")){
                sprintf(cradle_gw, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_cradle_mask")){
                sprintf(cradle_mask, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_wifi_ap_ssid")){
                sprintf(wifiap_ssid, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_wifi_ap_pw")){
                sprintf(wifiap_pw, "%s", prm[i].value);
            }
#if 0
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
#endif
        }
		
        CGI_DBG("wireless_iptype:%d, wireless_ipv4:%s, wireless_gw:%s, wireless_mask:%s\n", 
				wireless_iptype, wireless_ipv4, wireless_gw, wireless_mask);
        CGI_DBG("cradle_iptype:%d, cradle_ipv4:%s, cradle_gw:%s, cradle_mask:%s\n", 
				cradle_iptype, cradle_ipv4, cradle_gw, cradle_mask);
        CGI_DBG("wifi_ap, ssid:%s, pw:%s\n", wifiap_ssid, wifiap_pw);

#if 0
        CGI_DBG("live_stream_account_enable:%d, id:%s, pw:%s\n", 
				live_stream_account_enable, live_stream_account_id, live_stream_account_pw);
#endif

		// check not null
		if( wireless_iptype == -1 || cradle_iptype == -1 )//|| live_stream_account_enable == -1) 
		{
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}


		len = strlen(wifiap_ssid);
		if( len == 0 || len > 32)
		{
			CGI_DBG("Invalid Parameter, WIFI SSID , len=%d\n", len);
			return ERR_INVALID_PARAM;
		}

		len = strlen(wifiap_pw);
		if ( len != 0 && (len < 8 || len > 64) ) // Allow NULL password. 2020.02.26
		{
			CGI_DBG("Invalid Parameter, wifi password, len=%d\n", len);
			return ERR_INVALID_PARAM;
		}

		if( wireless_iptype == 0
		&& (strlen(wireless_ipv4) == 0
		||  strlen(wireless_gw  ) == 0
		||  strlen(wireless_mask) == 0 ))
	   	{
			CGI_DBG("Invalid Parameter, wireless\n");
			return ERR_INVALID_PARAM;
		}
		if( cradle_iptype == 0
		&& (strlen(cradle_ipv4) == 0
		||  strlen(cradle_gw  ) == 0
		||  strlen(cradle_mask) == 0 ))
	   	{
			CGI_DBG("Invalid Parameter, cradle\n");
			return ERR_INVALID_PARAM;
		}

#if 0
		if( live_stream_account_enable == 1
		&& (live_stream_account_enctype  == -1
		||  strlen(live_stream_account_id) == 0
		||  strlen(live_stream_account_pw) == 0
		    )
		)
	   	{
			CGI_DBG("Invalid Parameter, live stream account\n");
			return ERR_INVALID_PARAM;
		}
#endif


        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        // check parameter values
        T_CGI_NETWORK_CONFIG t;
		memset(&t, 0, sizeof(T_CGI_NETWORK_CONFIG));

		t.wireless.addr_type         = wireless_iptype;
		if(t.wireless.addr_type == 0) { // static
			sprintf(t.wireless.ipv4, "%s", wireless_ipv4);
			sprintf(t.wireless.gw,   "%s", wireless_gw);
			sprintf(t.wireless.mask, "%s", wireless_mask);
		}

		t.cradle.addr_type         = cradle_iptype;
		if(t.cradle.addr_type == 0) { // static
			sprintf(t.cradle.ipv4, "%s", cradle_ipv4);
			sprintf(t.cradle.gw,   "%s", cradle_gw);
			sprintf(t.cradle.mask, "%s", cradle_mask);
		}

		strncpy(t.wifi_ap.id, wifiap_ssid, strlen(wifiap_ssid));
		strncpy(t.wifi_ap.pw, wifiap_pw,   strlen(wifiap_pw));

#if 0
		t.live_stream_account_enable  = live_stream_account_enable;
		t.live_stream_account_enctype = live_stream_account_enctype;
		if(live_stream_account_enable){
			strcpy(t.live_stream_account.id, live_stream_account_id);
			strcpy(t.live_stream_account.pw, live_stream_account_pw);
		}
#endif

        ret = sysctl_message(UDS_SET_NETWORK_CONFIG, (void*)&t, sizeof(T_CGI_NETWORK_CONFIG));
        if(0 > ret) {
            return SUBMIT_ERR;
        }
		else if( ret == ERR_NO_CHANGE) {
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
	//int nError = submit_settings();
	int nError = submit_settings_qcgi();

	send_response(nError);

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else if(nError == SUBMIT_NO_CHANGE) {
		wait_redirect(REDIRECT_CGI_PATH, 3);
		//wait_redirect(CGI_FORM_PATH, 1);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
