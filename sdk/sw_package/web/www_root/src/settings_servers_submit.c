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
#if 0 // for debug
    char *contents = get_cgi_contents();
	CGI_DBG("contents:%s\n", contents);
#endif

    int ret=SUBMIT_NO_CHANGE;

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
        //qcgires_setcontenttype(req, "text/plain");

        T_CGI_SERVERS_CONFIG t;
		memset(&t, 0, sizeof t);
        t.voip.port = 6060;

		int  bs_enable=-1;
		int  bs_upload_files=-1;
		int  fota_enable=-1;
		int  mediaserver_enable=-1;
		int  ms_enable=-1;
		int  ddns_enable=-1;
		int  ntp_enable=-1;
		int  daylight_saving=-1;
		int  enable_https=-1;
		int  enable_onvif=-1;
		int  time_zone=-99;
		int  enable_p2p=1; // 무조건 1...


        // backup server --- start
        char *str= req->getstr(req, "bs_enable", false);
        if (str != NULL) { bs_enable = atoi(str); }
        str= req->getstr(req, "bs_upload_files", false);
        if (str != NULL) { bs_upload_files = atoi(str); }
        str= req->getstr(req, "txt_bs_ip", false);
        if (str != NULL) { sprintf(t.bs.serveraddr, "%s", str); }
        str= req->getstr(req, "txt_bs_id", false);
        if (str != NULL) { sprintf(t.bs.id, "%s", str); }
        str= req->getstr(req, "txt_bs_pw", false);
        if (str != NULL) { sprintf(t.bs.pw, "%s", str); }
        str= req->getstr(req, "txt_bs_port", false);
        if (str != NULL) { t.bs.port = atoi(str); }
        // backup server --- end

        // FOTA --- start
        str= req->getstr(req, "fota_enable", false);
        if (str != NULL) { fota_enable = atoi(str); }
        str= req->getstr(req, "fota_server_info", false);
        if (str != NULL) { t.fota.server_info = atoi(str); } // 0: same backup server info, 1: manual
        str= req->getstr(req, "txt_fota_ip", false);
        if (str != NULL) { sprintf(t.fota.serveraddr, "%s", str); }
        str= req->getstr(req, "txt_fota_id", false);
        if (str != NULL) { sprintf(t.fota.id, "%s", str); }
        str= req->getstr(req, "txt_fota_pw", false);
        if (str != NULL) { sprintf(t.fota.pw, "%s", str); }
        str= req->getstr(req, "txt_fota_port", false);
        if (str != NULL) { t.fota.port = atoi(str); }
        // FOTA --- end

        // Media Server --- start
        str= req->getstr(req, "mediaserver_enable", false);
        if (str != NULL) { mediaserver_enable = atoi(str); }
        //if(mediaserver_enable==1)
        {
            str= req->getstr(req, "mediaserver_use_full_path_url", false);
            if (str != NULL) { t.mediaserver.use_full_path_url = atoi(str); }
            str= req->getstr(req, "mediaserver_full_path_url", false);
            if (str != NULL) { sprintf(t.mediaserver.full_path_url, "%s", str); }
            str= req->getstr(req, "mediaserver_server_addr", false);
            if (str != NULL) { sprintf(t.mediaserver.serveraddr, "%s", str); }
            str= req->getstr(req, "mediaserver_port", false);
            if (str != NULL) { t.mediaserver.port = atoi(str); }
        }
        // Media Server --- end
        
        // Management Server --- start
        str= req->getstr(req, "ms_enable", false);
        if (str != NULL) { ms_enable = atoi(str); }
        str= req->getstr(req, "txt_ms_ip", false);
        if (str != NULL) { sprintf(t.ms.serveraddr, "%s", str); }
        str= req->getstr(req, "txt_ms_port", false);
        if (str != NULL) { t.ms.port = atoi(str); }

        // DDNS -- start
        str= req->getstr(req, "ddns_enable", false);
        if (str != NULL) { ddns_enable = atoi(str); }
        str= req->getstr(req, "txt_ddns_server", false);
        if (str != NULL) { sprintf(t.ddns.serveraddr, "%s", str); }
        str= req->getstr(req, "txt_ddns_hostname", false);
        if (str != NULL) { sprintf(t.ddns.hostname, "%s", str); }
        str= req->getstr(req, "txt_ddns_id", false);
        if (str != NULL) { sprintf(t.ddns.id, "%s", str); }
        str= req->getstr(req, "txt_ddns_pw", false);
        if (str != NULL) { sprintf(t.ddns.pw, "%s", str); }

        str= req->getstr(req, "txt_dns_1", false);
        if (str != NULL) { sprintf(t.dns.server1, "%s", str); }
        str= req->getstr(req, "txt_dns_2", false);
        if (str != NULL) { sprintf(t.dns.server2, "%s", str); }

        // NTP Server
        str= req->getstr(req, "ntp_enable", false);
        if (str != NULL) { ntp_enable = atoi(str); }
        str= req->getstr(req, "txt_ntp_ip", false);
        if (str != NULL) { sprintf(t.ntp.serveraddr, "%s", str); }
        str= req->getstr(req, "daylight_saving", false);
        if (str != NULL) { daylight_saving = atoi(str); }
        str= req->getstr(req, "cbo_timezone", false);
        if (str != NULL) { time_zone = atoi(str); }
        str= req->getstr(req, "timezone_abbr", false);
        if (str != NULL) { sprintf(t.time_zone_abbr, "%s", str); }

        // HTTPS
        str= req->getstr(req, "enable_https", false);
        if (str != NULL) { enable_https = atoi(str); }

        // ONVIF
        str= req->getstr(req, "enable_onvif", false);
        if (str != NULL) { enable_onvif = atoi(str); }
        str= req->getstr(req, "txt_onvif_id", false);
        if (str != NULL) { sprintf(t.onvif.id, "%s", str); }
        str= req->getstr(req, "txt_onvif_pw", false);
        if (str != NULL) { sprintf(t.onvif.pw, "%s", str); }

#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE) \
 || defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
        str= req->getstr(req, "voip_enable", false);
        if (str != NULL) {
            t.voip.enable = atoi(str);
        }
        str= req->getstr(req, "voip_use_stun", false);
        if (str != NULL) {
            t.voip.use_stun = atoi(str);
        }
        str= req->getstr(req, "txt_voip_ip", false);
        if (str != NULL) {
            sprintf(t.voip.ipaddr, "%s", str);
        }
        /* Port, Password는 default로 사용합니다.
        str= req->getstr(req, "txt_voip_port", false);
        if (str != NULL) {
            t.voip.port = atoi(str);
        }
        str= req->getstr(req, "txt_voip_pw", false);
        if (str != NULL) {
            sprintf(t.voip.passwd, "%s", str);
        }
        */
        str= req->getstr(req, "txt_voip_id", false);
        if (str != NULL) {
            sprintf(t.voip.userid, "%s", str);
        }
        str= req->getstr(req, "txt_voip_peerid", false);
        if (str != NULL) {
            sprintf(t.voip.peerid, "%s", str);
        }
		str= req->getstr(req, "p2p_enable", false);
		if (str != NULL) {
			enable_p2p = atoi(str);
		}
        CGI_DBG("enable:%d, use stun:%d, voip.ip:%s, voip.id:%s, voip.peerid:%s\n", 
        t.voip.enable,
        t.voip.use_stun, t.voip.ipaddr, t.voip.userid, t.voip.peerid);
#endif

        //req->free(req);

        CGI_DBG("bs_enable:%d, fota_enable:%d, ms_enable:%d, ddns_enable:%d, ntp_enable:%d, daylight_saving:%d, enable_https:%d, enable_onvif:%d, time_zone:%d, onvif.id:%s, onvif.pw:%s\n", 
				bs_enable, fota_enable, ms_enable, ddns_enable, ntp_enable, daylight_saving, enable_https, enable_onvif, time_zone, t.onvif.id, t.onvif.pw);

		if( bs_enable == -1 || fota_enable == -1 || ms_enable == -1 
			|| ddns_enable == -1 || ntp_enable == -1 
			|| daylight_saving == -1 
		    || time_zone == -99) {
			CGI_DBG("Invalid Parameter\n");
			return ERR_INVALID_PARAM;
		}

        if( enable_https == -1) {
			CGI_DBG("Invalid Parameter HTTPS Enable variable\n");
			return ERR_INVALID_PARAM;
        }

        if( mediaserver_enable == -1) {
			CGI_DBG("Invalid Parameter Media Server Enable variable\n");
			return ERR_INVALID_PARAM;
        }

#if defined(FITT360_SECURITY)
		if(enable_onvif==-1){
			CGI_DBG("Incorrect enable_onvif\n");
			return ERR_INVALID_PARAM;
		}
#endif

#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
 || defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
		if(enable_p2p == -1){
			CGI_DBG("Invalid Parameter:enable_p2p\n");
			return ERR_INVALID_PARAM;
		}
#endif

		CGI_DBG("dns_server1:%s\n", t.dns.server1);
		CGI_DBG("dns_server2:%s\n", t.dns.server2);

		t.bs.enable = bs_enable;
        t.bs.upload_files = bs_upload_files;
		t.fota.enable = fota_enable;
		t.mediaserver.enable = mediaserver_enable;
		t.ms.enable = ms_enable;
		t.ddns.enable = ddns_enable;
		t.ntp.enable = ntp_enable;
		t.daylight_saving = daylight_saving;
		//t.time_zone = time_zone+12; // Device 에서는 +12 한값을 사용한다.
		t.time_zone = time_zone;      // A value of +12 is only device.
		t.https.enable = enable_https;
		t.onvif.enable = enable_onvif;
		t.p2p.enable = enable_p2p; // FIXED 1.. 사용자 편의성을 위해서, 나중에 다시 풀라고 할지도 모름..-_-;;;

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

    return SUBMIT_ERR;
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
