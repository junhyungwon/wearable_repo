#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_servers.cgi"
#define HOME_PATH "../index.html"

static int submit_settings_qcgi()
{
    int ret=SUBMIT_NO_CHANGE;

	int bFitt360 = 1;
	if( strcmp(MODEL_NAME, "NEXX360") == 0)
		bFitt360 = 0;

#if 0 // for debug
    char *contents = get_cgi_contents();
	CGI_DBG("contents:%s\n", contents);
#endif

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
        //qcgires_setcontenttype(req, "text/plain");

        T_CGI_SERVERS_CONFIG t;
		memset(&t, 0, sizeof t);

		int  bs_enable=-1;
		int  ms_enable=-1;
		int  ddns_enable=-1;
		int  ntp_enable=-1;
		int  daylight_saving=-1;
		int  enable_onvif=-1;
		int  time_zone=-99;

        char *str= req->getstr(req, "bs_enable", false);
        if (str != NULL) {
            bs_enable = atoi(str);
        }
        str= req->getstr(req, "txt_bs_ip", false);
        if (str != NULL) {
            sprintf(t.bs.serveraddr, "%s", str);
        }
        str= req->getstr(req, "txt_bs_id", false);
        if (str != NULL) {
            sprintf(t.bs.id, "%s", str);
        }
        str= req->getstr(req, "txt_bs_pw", false);
        if (str != NULL) {
            sprintf(t.bs.pw, "%s", str);
        }
        str= req->getstr(req, "txt_bs_port", false);
        if (str != NULL) {
            t.bs.port = atoi(str);
        }
        str= req->getstr(req, "ms_enable", false);
        if (str != NULL) {
            ms_enable = atoi(str);
        }
        str= req->getstr(req, "txt_ms_ip", false);
        if (str != NULL) {
            sprintf(t.ms.serveraddr, "%s", str);
        }
        str= req->getstr(req, "txt_ms_port", false);
        if (str != NULL) {
            t.ms.port = atoi(str);
        }
        str= req->getstr(req, "ddns_enable", false);
        if (str != NULL) {
            ddns_enable = atoi(str);
        }
        str= req->getstr(req, "txt_ddns_server", false);
        if (str != NULL) {
            sprintf(t.ddns.serveraddr, "%s", str);
        }
        str= req->getstr(req, "txt_ddns_hostname", false);
        if (str != NULL) {
            sprintf(t.ddns.hostname, "%s", str);
        }
        str= req->getstr(req, "txt_ddns_id", false);
        if (str != NULL) {
            sprintf(t.ddns.id, "%s", str);
        }
        str= req->getstr(req, "txt_ddns_pw", false);
        if (str != NULL) {
            sprintf(t.ddns.pw, "%s", str);
        }
        str= req->getstr(req, "txt_dns_1", false);
        if (str != NULL) {
            sprintf(t.dns.server1, "%s", str);
        }
        str= req->getstr(req, "txt_dns_2", false);
        if (str != NULL) {
            sprintf(t.dns.server2, "%s", str);
        }
        str= req->getstr(req, "ntp_enable", false);
        if (str != NULL) {
            ntp_enable = atoi(str);
        }
        str= req->getstr(req, "txt_ntp_ip", false);
        if (str != NULL) {
            sprintf(t.ntp.serveraddr, "%s", str);
        }
        str= req->getstr(req, "daylight_saving", false);
        if (str != NULL) {
            daylight_saving = atoi(str);
        }
        str= req->getstr(req, "cbo_timezone", false);
        if (str != NULL) {
            time_zone = atoi(str);
        }
        str= req->getstr(req, "timezone_abbr", false);
        if (str != NULL) {
            sprintf(t.time_zone_abbr, "%s", str);
        }
        str= req->getstr(req, "enable_onvif", false);
        if (str != NULL) {
			CGI_DBG("str:%s\n", str);
            enable_onvif = atoi(str);
        }

        //req->free(req);

        CGI_DBG("bs_enable:%d, ms_enable:%d, ddns_enable:%d, ntp_enable:%d, daylight_saving:%d, enable_onvif:%d, time_zone:%d\n", 
				bs_enable, ms_enable, ddns_enable, ntp_enable, daylight_saving, enable_onvif, time_zone);

		if( bs_enable == -1 || ms_enable == -1 || ddns_enable == -1 || ntp_enable == -1 || daylight_saving == -1 || (bFitt360 && enable_onvif == -1)
		|| time_zone == -99) {
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}
		CGI_DBG("dns_server1:%s\n", t.dns.server1);
		CGI_DBG("dns_server2:%s\n", t.dns.server2);

		t.bs.enable = bs_enable;
		t.ms.enable = ms_enable;
		t.ddns.enable = ddns_enable;
		t.ntp.enable = ntp_enable;
		t.daylight_saving = daylight_saving;
		t.enable_onvif = enable_onvif;
		//t.time_zone = time_zone+12; // Device 에서는 +12 한값을 사용한다.
		t.time_zone = time_zone;      // A value of +12 is only device.

        // check parameter values

        ret = sysctl_message(UDS_SET_SERVERS_CONFIG, (void*)&t, sizeof t );
        CGI_DBG("ret:%d\n", ret);
        if(0 > ret) {
            return SUBMIT_ERR;
        }
		else if( ret == ERR_NO_CHANGE) {
			return SUBMIT_NO_CHANGE;
		}

        return SUBMIT_OK;
    }
}

static int submit_settings() // deprecated
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

        int  i=0;
        T_CGI_SERVERS_CONFIG t;
		memset(&t, 0, sizeof t);

		int  bs_enable=-1;
		int  ms_enable=-1;
		int  ddns_enable=-1;
		int  ntp_enable=-1;
		int  daylight_saving=-1;
		int  enable_onvif=-1;
		int  time_zone=-99;


        for(;i<cnt;i++) {

            CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
            if(!strcmp(prm[i].name, "bs_enable")){
                bs_enable = atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_bs_ip")){
				sprintf(t.bs.serveraddr, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_bs_id")){
				sprintf(t.bs.id, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_bs_pw")){
				sprintf(t.bs.pw, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_bs_port")){
                t.bs.port = atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "ms_enable")){
                ms_enable = atoi(prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_ms_ip")){
				sprintf(t.ms.serveraddr, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_ms_port")){
				t.ms.port = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "ddns_enable")){
                ddns_enable = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_ddns_server")){
                sprintf(t.ddns.serveraddr, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_ddns_hostname")){
                sprintf(t.ddns.hostname, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_ddns_id")){
				sprintf(t.ddns.id, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_ddns_pw")){
				sprintf(t.ddns.pw, "%s", prm[i].value);
            } 
            else if(!strcmp(prm[i].name, "txt_dns_1")){
                sprintf(t.dns.server1, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_dns_2")){
                sprintf(t.dns.server2, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "ntp_enable")){
                ntp_enable = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "txt_ntp_ip")){
				sprintf(t.ntp.serveraddr, "%s", prm[i].value);
            }
            else if(!strcmp(prm[i].name, "daylight_saving")){
                daylight_saving = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "enable_onvif")){
                enable_onvif = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "cbo_timezone")){
                time_zone = atoi(prm[i].value);
            }
            else if(!strcmp(prm[i].name, "timezone_abbr")){
                sprintf(t.time_zone_abbr, "%s", prm[i].value);
            }
        }
		
        // Must finish parsing before free.
        if(isPOST){ free(contents); }

        CGI_DBG("bs_enable:%d, ms_enable:%d, ddns_enable:%d, ntp_enable:%d, daylight_saving:%d, enable_onvif:%d, time_zone:%d\n", 
				bs_enable, ms_enable, ddns_enable, ntp_enable, daylight_saving, enable_onvif, time_zone);

		if( bs_enable == -1 || ms_enable == -1 || ddns_enable == -1 || ntp_enable == -1 || daylight_saving == -1 || enable_onvif == -1
		|| time_zone == -99) {
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}
		CGI_DBG("dns_server1:%s\n", t.dns.server1);
		CGI_DBG("dns_server2:%s\n", t.dns.server2);

		t.bs.enable = bs_enable;
		t.ms.enable = ms_enable;
		t.ddns.enable = ddns_enable;
		t.ntp.enable = ntp_enable;
		t.daylight_saving = daylight_saving;
		t.enable_onvif = enable_onvif;
		//t.time_zone = time_zone+12; // Device 에서는 +12 한값을 사용한다.
		t.time_zone = time_zone;      // A value of +12 is only device.

        // check parameter values

        ret = sysctl_message(UDS_SET_SERVERS_CONFIG, (void*)&t, sizeof t );
        CGI_DBG("ret:%d\n", ret);
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
	int nError = submit_settings_qcgi();

	CGI_DBG("nError:%d\n", nError);

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
