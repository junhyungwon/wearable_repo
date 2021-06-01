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
		int  len = 0, ret = 0, i;
		int  wireless_iptype=-1;
		char wireless_ipv4[32]={0};
		char wireless_gw[32]={0};
		char wireless_mask[32]={0};

		int  cradle_iptype=-1;
		char cradle_ipv4[32]={0};
		char cradle_gw[32]={0};
		char cradle_mask[32]={0};

		char wifiap_ssid[128]={0};
		char wifiap_pass[128]={0};

		char wifilist_ssid[4][128]={0};
		char wifilist_pass[4][128]={0};

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
        str = req->getstr(req, "txt_wifi_ap_pass", false);
		if(str != NULL) {
            sprintf(wifiap_pass, "%s", str);
        }

		// additional wifi APs
        str = req->getstr(req, "txt_wifi_ap_ssid1", false);
		if(str != NULL) {
            sprintf(wifilist_ssid[0], "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_pass1", false);
		if(str != NULL) {
            sprintf(wifilist_pass[0], "%s", str);
        }
        str = req->getstr(req, "txt_wifi_ap_ssid2", false);
		if(str != NULL) {
            sprintf(wifilist_ssid[1], "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_pass2", false);
		if(str != NULL) {
            sprintf(wifilist_pass[1], "%s", str);
        }
        str = req->getstr(req, "txt_wifi_ap_ssid3", false);
		if(str != NULL) {
            sprintf(wifilist_ssid[2], "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_pass3", false);
		if(str != NULL) {
            sprintf(wifilist_pass[2], "%s", str);
        }
        str = req->getstr(req, "txt_wifi_ap_ssid4", false);
		if(str != NULL) {
            sprintf(wifilist_ssid[3], "%s", str);
        } 
        str = req->getstr(req, "txt_wifi_ap_pass4", false);
		if(str != NULL) {
            sprintf(wifilist_pass[3], "%s", str);
        }
        CGI_DBG("wireless_iptype:%d, wireless_ipv4:%s, wireless_gw:%s, wireless_mask:%s\n", 
				wireless_iptype, wireless_ipv4, wireless_gw, wireless_mask);
        CGI_DBG("cradle_iptype:%d, cradle_ipv4:%s, cradle_gw:%s, cradle_mask:%s\n", 
				cradle_iptype, cradle_ipv4, cradle_gw, cradle_mask);
        CGI_DBG("wifi_AP,  ssid:%s, pass:%s\n", wifiap_ssid, wifiap_pass);
        CGI_DBG("wifi_AP1, ssid:%s, pass:%s\n", wifilist_ssid[0], wifilist_pass[0]);
        CGI_DBG("wifi_AP2, ssid:%s, pass:%s\n", wifilist_ssid[1], wifilist_pass[1]);
        CGI_DBG("wifi_AP3, ssid:%s, pass:%s\n", wifilist_ssid[2], wifilist_pass[2]);
        CGI_DBG("wifi_AP4, ssid:%s, pass:%s\n", wifilist_ssid[3], wifilist_pass[3]);

		// check not null
		if( wireless_iptype == -1 || cradle_iptype == -1 )//|| live_stream_account_enable == -1) 
		{
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

		// check wifi AP
		len = strlen(wifiap_ssid);
		if( len == 0 || len > 32)
		{
			CGI_DBG("Invalid Parameter, WIFI SSID , len=%d\n", len);
			return ERR_INVALID_PARAM;
		}
		len = strlen(wifiap_pass);
		if ( len != 0 && (len < 8 || len > 64) ) // Allow NULL password. 2020.02.26
		{
			CGI_DBG("Invalid Parameter, wifi password, len=%d\n", len);
			return ERR_INVALID_PARAM;
		}

		for (i = 0; i < 4; i++)
		{
			len = strlen(wifilist_ssid[i]);
			if (len == 0 || len > 32) {
				CGI_DBG("Invalid Parameter, WIFI[%d] SSID, len=%d\n", i+1, len);
				return ERR_INVALID_PARAM;
			}
			len = strlen(wifilist_pass[i]);
			if (len != 0 && (len < 8 || len > 64)) // Allow NULL password. since 2020.02.26
			{
				CGI_DBG("Invalid Parameter, WIFI[%d] password, len=%d\n", i+1, len);
				return ERR_INVALID_PARAM;
			}
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

#if 0 // GUI가 다른 페이지로 이동하여, 이 파라미터들은 안씀..
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
        T_CGI_NETWORK_CONFIG2 t;
		memset(&t, 0, sizeof(t));

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
		strncpy(t.wifi_ap.pw, wifiap_pass, strlen(wifiap_pass));

		for(i=0;i<4;i++){
			strncpy(t.wifi_ap_list[i].id, wifilist_ssid[i], strlen(wifilist_ssid[i]));
			strncpy(t.wifi_ap_list[i].pw, wifilist_pass[i], strlen(wifilist_pass[i]));
		}

#if 0 // GUI가 다른 페이지로 이동하여, 이 파라미터들은 안씀..
		t.live_stream_account_enable  = live_stream_account_enable;
		t.live_stream_account_enctype = live_stream_account_enctype;
		if(live_stream_account_enable){
			strcpy(t.live_stream_account.id, live_stream_account_id);
			strcpy(t.live_stream_account.pw, live_stream_account_pw);
		}
#endif

        ret = sysctl_message(UDS_SET_NETWORK_CONFIG2, (void*)&t, sizeof(t));
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
