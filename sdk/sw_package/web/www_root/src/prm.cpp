/*
 * @file  prm.cpp
 * @brief process actions...search, update, delete ...
 * @etc js_settings.c에 있는 json 파일과 field name들이 다르다. 이 파일의 json들은 curl등을 위한  Web API용이니 헷갈리지 말자!
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <json-c/json.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"
/******************************************************************************/


/*******************************************************************************
 * defines
 ******************************************************************************/
#define PLAIN 0
#define XML   1
#define JSON  2


#if 0
// 이걸 사용할 경우, all_config, network_config, servers_config 에서 header와 contents가 구분이 안되는 증상이 발생됨.
// PUT_CRLF를 printf를 써서 그런지도 모름.
static char out_buf[1024];
#define PUTSTR(fmt, args...)	write(STDOUT_FILENO, out_buf, sprintf(out_buf, fmt, ##args))
#else
#define PUTSTR printf
#endif

#if defined(NEXXONE) || defined(NEXXB_ONE)
	#define MAX_FPS 30
#elif defined(NEXX360W) || defined(NEXXB) || defined(NEXX360W_MUX)\
   || defined(NEXX360B) || defined(NEXX360M) || defined(NEXX360C) || defined(NEXX360W_CCTV)
	#define MAX_FPS 15
#elif defined(FITT360_SECURITY)
	#define MAX_FPS 15
#else
	#error "Check PRODUCT_NAME in Rules.make"
#endif

/******************************************************************************/

/*******************************************************************************
 * global variables
 ******************************************************************************/
namespace NS_PRM {
	int gRefreshSec=0;

	int send_response(int errnum)
	{
		PUT_CONTENT_TYPE_PLAIN;
		PUT_CRLF;
		PUTSTR("(%d)\r\n", errnum);
	}
}

static int setParam(char *arg)
{
	// support only GET
    T_CGIPRM prm[128];

	// check
    char *method = getenv("REQUEST_METHOD");
    CGI_DBG("method : %s\n", method);

    char *contents = getenv("QUERY_STRING");

    CGI_DBG("contents:%s\n", contents);

	if(!contents){
		return ERR_INVALID_PARAM;
	}

    int cnt = parseContents(contents, prm);
    CGI_DBG("cnt:%d\n", cnt);

	if(cnt>0){
		NS_PRM::gRefreshSec = 2;
		for(int i=0;i<cnt;i++) {
			CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);

			if(!strcmp(prm[i].name, "system")){
				if(!strcmp(prm[i].value, "restart")){
					CGI_DBG("system reboot\n");
					NS_PRM::gRefreshSec = 30;
					if(0 != sysctl_message(UDS_CMD_RESTART, NULL,  0)) {
						return SUBMIT_ERR;
					}
				}
				else if(!strcmp(prm[i].value, "factorydefault_hard")){
					CGI_DBG("system factorydefault_hard\n");
					NS_PRM::gRefreshSec = 30;
					if(0 != sysctl_message(UDS_CMD_FACTORYDEFAULT_HARD, NULL, 0)) {
						return SUBMIT_ERR;
					}
				}
				else {
				}
			}
		}
	}

	return 0;
}

int do_sysmng(char *pContents)
{
	int ret=-1;

	return ret;
}

int put_json_all_config()
{
	int ret=-1, i;

	json_object *all_config;
	json_object *camera_obj, *recordobj, *streamobj;
	json_object *operation_obj, *stm_obj, *misc_obj, *rec_obj, *p2p_obj;
	json_object *network_obj, *wireless_obj, *cradle_obj, *wifiap_obj, *livestm_obj, *wifilist, *wifiInfo[4], *sslvpn;
	json_object *servers_obj, *bs_obj, *fota_obj, *mediaserver_obj, *ms_obj, *ddns_obj, *dns_obj, *ntp_obj, *onvif_obj, *voip_obj;
	json_object *system_obj;
	json_object *user_obj;

	// all, entire...
	all_config = json_object_new_object();
	json_object_object_add(all_config, "model", json_object_new_string(MODEL_NAME));

	// Camera Settings
	camera_obj = json_object_new_object();
	recordobj = json_object_new_object();
	streamobj = json_object_new_object();
	{
		T_CGI_VIDEO_QUALITY p;memset(&p,0, sizeof p);
		if(sysctl_message(UDS_GET_VIDEO_QUALITY, (void*)&p, sizeof(p)) < 0) {
			ret = -1;
			goto _FREE_CAMERA_OBJ;
		}

		// record info
#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXX360M) || defined(NEXX360W_MUX) \
    || defined(NEXXB) || defined(NEXXB_ONE) \
    || defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
		int fpsIdx = p.rec.fps-1;
		if(fpsIdx < 0 ) fpsIdx = 0;
		if(fpsIdx > MAX_FPS) fpsIdx = MAX_FPS-1; // fpsIdx is Zero-based number.

		int bpsIdx = 0;
		int kbps = p.rec.bps; // kbps
		switch (kbps) {
			case 512:
				bpsIdx = 0;
				break;
			case 1000:
				bpsIdx = 1;
				break;
			case 2000:
				bpsIdx = 2;
				break;
			case 3000:
				bpsIdx = 3;
				break;
			case 4000:
				bpsIdx = 4;
				break;
		}

		json_object_object_add(recordobj, "fps", json_object_new_int(fpsIdx));
		json_object_object_add(recordobj, "bps", json_object_new_int(bpsIdx)); // kbps
#else
		json_object_object_add(recordobj, "fps", json_object_new_int(p.rec.fps));
		json_object_object_add(recordobj, "bps", json_object_new_int(p.rec.bps));
#endif
		json_object_object_add(recordobj, "gop", json_object_new_int(p.rec.gop));
		json_object_object_add(recordobj, "rc",  json_object_new_int(p.rec.rc));
		//json_object_object_add(recordobj, "desc",  json_object_new_string("Video Quality for Recording"));
		json_object_object_add(camera_obj, "record", recordobj);

		// streaming info
		json_object_object_add(streamobj, "resolution", json_object_new_int(p.stm.res));
#if defined(NEXXONE)|| defined(NEXX360W) || defined(NEXX360M) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
  || defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)

		fpsIdx = p.stm.fps-1;
		if(fpsIdx < 0 ) fpsIdx = 0;
		if(fpsIdx > MAX_FPS) fpsIdx = MAX_FPS-1;

		bpsIdx = 0;
		kbps = p.stm.bps; // kbps
		switch (kbps) {
			case 128:
				bpsIdx = 0;
				break;
			case 256:
				bpsIdx = 1;
				break;
			case 512:
				bpsIdx = 2;
				break;
			case 1000:
				bpsIdx = 3;
				break;
			case 2000:
				bpsIdx = 4;
				break;
			case 3000:
				bpsIdx = 5;
				break;
			case 4000:
				bpsIdx = 6;
				break;
			case 5000:
				bpsIdx = 7;
				break;
			case 6000:
				bpsIdx = 8;
				break;
			case 7000:
				bpsIdx = 9;
				break;
			case 8000:
				bpsIdx = 10;
				break;
		}
		json_object_object_add(streamobj, "fps", json_object_new_int(fpsIdx));
		json_object_object_add(streamobj, "bps", json_object_new_int(bpsIdx)); // kbps
#else
		json_object_object_add(streamobj, "fps", json_object_new_int(p.stm.fps));
		json_object_object_add(streamobj, "bps", json_object_new_int(p.stm.bps));
#endif
		json_object_object_add(streamobj, "gop", json_object_new_int(p.stm.gop));
		json_object_object_add(streamobj, "rc",  json_object_new_int(p.stm.rc));
		//json_object_object_add(recordobj, "desc",  json_object_new_string("Video Quality for Streaming"));
		json_object_object_add(camera_obj, "stream", streamobj);
	}
	json_object_object_add(all_config, "camera_settings", camera_obj);

	// Operation Settings
	operation_obj = json_object_new_object();
	stm_obj  = json_object_new_object();
	rec_obj  = json_object_new_object();
	misc_obj = json_object_new_object();
	{
		T_CGI_OPERATION_CONFIG p;memset(&p,0, sizeof p);
		if(0>sysctl_message(UDS_GET_OPERATION_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_OPERATION_OBJ;
		}

		json_object_object_add(stm_obj, "enable_audio",    json_object_new_int(p.stm.enable_audio));
		json_object_object_add(operation_obj, "stream", stm_obj);

		json_object_object_add(rec_obj, "pre_rec",   json_object_new_int(p.rec.pre_rec));
		json_object_object_add(rec_obj, "auto_rec", json_object_new_int(p.rec.audio_rec));
		json_object_object_add(rec_obj, "audio_rec",  json_object_new_int(p.rec.auto_rec));
		json_object_object_add(rec_obj, "rec_interval", json_object_new_int(p.rec.interval));
		json_object_object_add(rec_obj, "rec_overwrite",    json_object_new_int(p.rec.overwrite));
		json_object_object_add(operation_obj, "record", rec_obj);

		json_object_object_add(misc_obj, "display_datetime", json_object_new_int(p.display_datetime));
		json_object_object_add(operation_obj, "misc", misc_obj);

	}
	json_object_object_add(all_config, "operation_settings", operation_obj);

	// network
	network_obj  = json_object_new_object();
	wireless_obj = json_object_new_object();
	cradle_obj   = json_object_new_object();
	wifiap_obj   = json_object_new_object();
	livestm_obj  = json_object_new_object();
	wifilist     = json_object_new_array();
	sslvpn       = json_object_new_object();
	{
		T_CGI_NETWORK_CONFIG2 p;memset(&p, 0, sizeof p);
		if(0>sysctl_message(UDS_GET_NETWORK_CONFIG2, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_NETWORK_OBJ;
		}
		json_object_object_add(network_obj, "wifi_ap_multi", json_object_new_int(   p.wifi_ap_multi));

		json_object_object_add(wireless_obj, "addr_type", json_object_new_int(   p.wireless.addr_type));
		json_object_object_add(wireless_obj, "ipv4",      json_object_new_string(p.wireless.ipv4));
		json_object_object_add(wireless_obj, "gateway",   json_object_new_string(p.wireless.gw));
		json_object_object_add(wireless_obj, "netmask",   json_object_new_string(p.wireless.mask));
		json_object_object_add(network_obj, "wireless",   wireless_obj);

		json_object_object_add(cradle_obj, "addr_type", json_object_new_int(   p.cradle.addr_type));
		json_object_object_add(cradle_obj, "ipv4",      json_object_new_string(p.cradle.ipv4));
		json_object_object_add(cradle_obj, "gateway",   json_object_new_string(p.cradle.gw));
		json_object_object_add(cradle_obj, "netmask",   json_object_new_string(p.cradle.mask));
		json_object_object_add(network_obj, "cradle",   cradle_obj);

		json_object_object_add(wifiap_obj, "ssid",     json_object_new_string(p.wifi_ap.id));
		json_object_object_add(wifiap_obj, "pw",       json_object_new_string(p.wifi_ap.pw));
		json_object_object_add(network_obj, "wifi_ap", wifiap_obj);

		json_object_object_add(livestm_obj, "enable", json_object_new_int(   p.live_stream_account_enable));
		json_object_object_add(livestm_obj, "enctype", json_object_new_int(  p.live_stream_account_enctype));
		json_object_object_add(livestm_obj, "id",     json_object_new_string(p.live_stream_account.id));
		json_object_object_add(livestm_obj, "pw",     json_object_new_string(p.live_stream_account.pw));
		json_object_object_add(network_obj, "live_stream_account", livestm_obj);

		char fname[10];
		for(i = 0 ; i < 4 ; i++)
		{
			wifiInfo[i] = json_object_new_object() ;

			sprintf(fname,"ssid%d", i+1);
			json_object_object_add(wifiInfo[i], fname,    json_object_new_string(p.wifi_ap_list[i].id));
			sprintf(fname,"pwd%d", i+1);
			json_object_object_add(wifiInfo[i], fname,     json_object_new_string(p.wifi_ap_list[i].pw));

			json_object_array_add(wifilist, wifiInfo[i]);
		}
		json_object_object_add(network_obj, "wifilist", wifilist) ;


		// ssl vpn
		{
			json_object_object_add(sslvpn, "enable",              json_object_new_int(    p.sslvpn.enable ));
			json_object_object_add(sslvpn, "vendor",              json_object_new_int(    p.sslvpn.vendor ));
			json_object_object_add(sslvpn, "vpn_id",              json_object_new_string( p.sslvpn.vpn_id ));
			json_object_object_add(sslvpn, "heartbeat_interval",  json_object_new_int(    p.sslvpn.heartbeat_interval ));
			json_object_object_add(sslvpn, "heartbeat_threshold", json_object_new_int(    p.sslvpn.heartbeat_threshold ));
			json_object_object_add(sslvpn, "protocol",            json_object_new_int(    p.sslvpn.protocol ));
			json_object_object_add(sslvpn, "port",                json_object_new_int(    p.sslvpn.port ));
			json_object_object_add(sslvpn, "queue_size",          json_object_new_int(    p.sslvpn.queue_size ));
			json_object_object_add(sslvpn, "key",                 json_object_new_string( p.sslvpn.key ));
			json_object_object_add(sslvpn, "encrypt_type",        json_object_new_int(    p.sslvpn.encrypt_type ));
			json_object_object_add(sslvpn, "ipaddress",           json_object_new_string( p.sslvpn.ipaddress ));
			json_object_object_add(sslvpn, "network_interface",   json_object_new_int(    p.sslvpn.network_interface ));

			json_object_object_add( network_obj, "sslvpn", sslvpn ) ;
		}
	}
	json_object_object_add(all_config, "network_settings", network_obj);


	// servers
	servers_obj = json_object_new_object();
	bs_obj      = json_object_new_object();
	fota_obj    = json_object_new_object();
	mediaserver_obj    = json_object_new_object();
	ms_obj      = json_object_new_object();
	ddns_obj    = json_object_new_object();
	dns_obj     = json_object_new_object();
	ntp_obj     = json_object_new_object();
	onvif_obj   = json_object_new_object();
	p2p_obj     = json_object_new_object();
	voip_obj    = json_object_new_object();
	{
		T_CGI_SERVERS_CONFIG p;memset(&p, 0, sizeof p);
		if(0>sysctl_message(UDS_GET_SERVERS_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_SERVERS_OBJ;
		}

		json_object_object_add(bs_obj, "enable",       json_object_new_int   (p.bs.enable));
		json_object_object_add(bs_obj, "upload_files", json_object_new_int   (p.bs.upload_files));
		json_object_object_add(bs_obj, "serveraddr",   json_object_new_string(p.bs.serveraddr));
		json_object_object_add(bs_obj, "port",         json_object_new_int   (p.bs.port));
		json_object_object_add(bs_obj, "id",           json_object_new_string(p.bs.id));
		json_object_object_add(bs_obj, "pw",           json_object_new_string(p.bs.pw));
		json_object_object_add(servers_obj, "bs", bs_obj);

		json_object_object_add(fota_obj, "enable",      json_object_new_int   (p.fota.enable));
		json_object_object_add(fota_obj, "server_info", json_object_new_int(p.fota.server_info));
		json_object_object_add(fota_obj, "serveraddr",  json_object_new_string(p.fota.serveraddr));
		json_object_object_add(fota_obj, "port",        json_object_new_int   (p.fota.port));
		json_object_object_add(fota_obj, "id",          json_object_new_string(p.fota.id));
		json_object_object_add(fota_obj, "pw",          json_object_new_string(p.fota.pw));
		json_object_object_add(servers_obj, "fota", fota_obj);

		json_object_object_add(mediaserver_obj, "enable",            json_object_new_int   (p.mediaserver.enable));
		json_object_object_add(mediaserver_obj, "use_full_path_url", json_object_new_int   (p.mediaserver.use_full_path_url));
		json_object_object_add(mediaserver_obj, "full_path_url",     json_object_new_string(p.mediaserver.full_path_url));
		json_object_object_add(mediaserver_obj, "serveraddr",        json_object_new_string(p.mediaserver.serveraddr));
		json_object_object_add(mediaserver_obj, "port",              json_object_new_int   (p.mediaserver.port));
		// json_object_object_add(mediaserver_obj, "id",          json_object_new_string(p.mediaserver.id));
		// json_object_object_add(mediaserver_obj, "pw",          json_object_new_string(p.mediaserver.pw));
		json_object_object_add(servers_obj, "fota", mediaserver_obj);

		json_object_object_add(ms_obj, "enable",     json_object_new_int   (p.ms.enable));
		json_object_object_add(ms_obj, "serveraddr", json_object_new_string(p.ms.serveraddr));
		json_object_object_add(ms_obj, "port",       json_object_new_int   (p.ms.port));
		json_object_object_add(servers_obj, "ms", ms_obj);

		json_object_object_add(ddns_obj, "enable",   json_object_new_int(   p.ddns.enable));
		json_object_object_add(ddns_obj, "serveraddr",   json_object_new_string(p.ddns.serveraddr));
		json_object_object_add(ddns_obj, "hostname", json_object_new_string(p.ddns.hostname));
		json_object_object_add(ddns_obj, "id",       json_object_new_string(p.ddns.id));
		json_object_object_add(ddns_obj, "pw", json_object_new_string(p.ddns.pw));
		json_object_object_add(servers_obj, "ddns", ddns_obj);

		json_object_object_add(dns_obj, "server1",     json_object_new_string(p.dns.server1));
		json_object_object_add(dns_obj, "server2",     json_object_new_string(p.dns.server2));
		json_object_object_add(servers_obj, "dns", dns_obj);

		json_object_object_add(ntp_obj, "enable",      json_object_new_int(   p.ntp.enable));
		json_object_object_add(ntp_obj, "serveraddr", json_object_new_string(p.ntp.serveraddr));
		json_object_object_add(servers_obj, "ntp", ntp_obj);

		json_object_object_add(servers_obj, "timezone",       json_object_new_int(p.time_zone));
		json_object_object_add(servers_obj, "timezone_abbr",  json_object_new_string(p.time_zone_abbr));
		json_object_object_add(servers_obj, "daylightsaving", json_object_new_int(p.daylight_saving));

		// onvif server settings
		json_object_object_add(onvif_obj,   "enable",  json_object_new_int(p.onvif.enable));
		json_object_object_add(onvif_obj,   "id",      json_object_new_string(p.onvif.id));
		json_object_object_add(onvif_obj,   "pw",      json_object_new_string(p.onvif.pw));
		json_object_object_add(servers_obj, "onvif", onvif_obj);
		
#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXX360M) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
  || defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
		json_object_object_add(p2p_obj, "p2p_enable",   json_object_new_int(p.p2p.enable));
		//json_object_object_add(p2p_obj, "p2p_username", json_object_new_string(p.p2p.username));
		//json_object_object_add(p2p_obj, "p2p_password", json_object_new_string(p.p2p.password));
		//json_object_object_add(operation_obj, "p2p", p2p_obj);
		json_object_object_add(servers_obj, "p2p", p2p_obj);
#endif
		
		// voip
		json_object_object_add(voip_obj, "enable", json_object_new_int(p.voip.enable));
		json_object_object_add(voip_obj, "use_stun", json_object_new_int(p.voip.use_stun));
		json_object_object_add(voip_obj, "ipaddr", json_object_new_string(p.voip.ipaddr));
		json_object_object_add(voip_obj, "port",   json_object_new_int(   p.voip.port));
		json_object_object_add(voip_obj, "userid", json_object_new_string(p.voip.userid));
		json_object_object_add(voip_obj, "passwd", json_object_new_string(p.voip.passwd));
		json_object_object_add(voip_obj, "peerid", json_object_new_string(p.voip.peerid));
		json_object_object_add(servers_obj, "voip", voip_obj);
	}
	json_object_object_add(all_config, "servers_settings", servers_obj);


	// system
	system_obj = json_object_new_object();
	{
		T_CGI_SYSTEM_CONFIG p;memset(&p, 0, sizeof p);
		if(0>sysctl_message(UDS_GET_SYSTEM_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_SYSTEM_OBJ;
		}

		json_object_object_add(system_obj, "model", json_object_new_string(p.model));
		json_object_object_add(system_obj, "fwver", json_object_new_string(p.fwver));
		json_object_object_add(system_obj, "devid", json_object_new_string(p.devid));
		json_object_object_add(system_obj, "uid", json_object_new_string(p.uid));

		remove_rn_char_from_string(p.mac);
		json_object_object_add(system_obj, "mac",   json_object_new_string(p.mac));
	}
	json_object_object_add(all_config, "system_settings", system_obj);



	// put results
	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	PUTSTR("%s", json_object_to_json_string(all_config));


	// free
_FREE_SYSTEM_OBJ:
	json_object_put(system_obj);

_FREE_SERVERS_OBJ:
	json_object_put(bs_obj);
	json_object_put(fota_obj);
	json_object_put(mediaserver_obj);
	json_object_put(ms_obj);
	json_object_put(ddns_obj);
	json_object_put(dns_obj);
	json_object_put(ntp_obj);
	json_object_put(onvif_obj);
	json_object_put(voip_obj);
	json_object_put(servers_obj);

_FREE_NETWORK_OBJ:
	json_object_put(wireless_obj);
	json_object_put(cradle_obj);
	json_object_put(wifiap_obj);
	json_object_put(livestm_obj);
	for(i = 0; i < 4 ; i++)json_object_put(wifiInfo[i]);
	json_object_put(wifilist);
	json_object_put(sslvpn);
	json_object_put(network_obj);

_FREE_OPERATION_OBJ:
	json_object_put(stm_obj);
	json_object_put(rec_obj);
	json_object_put(misc_obj);
	json_object_put(p2p_obj);
	json_object_put(operation_obj);

_FREE_CAMERA_OBJ:
	json_object_put(recordobj);
	json_object_put(streamobj);
	json_object_put(camera_obj);

	json_object_put(all_config);

	return ret;

}//put_json_all_config

void put_json_system_info(T_CGI_SYSTEM_INFO *p)
{
	json_object *root_obj = json_object_new_object();

	json_object_object_add(root_obj, "model", json_object_new_string(p->model));
	json_object_object_add(root_obj, "fwver", json_object_new_string(p->fwver));
	json_object_object_add(root_obj, "ftp",   json_object_new_int(   p->ftp));
	json_object_object_add(root_obj, "onvif", json_object_new_int(   p->onvif));
	json_object_object_add(root_obj, "p2p",   json_object_new_int(   p->p2p));
	json_object_object_add(root_obj, "https", json_object_new_int(   p->https));
	json_object_object_add(root_obj, "rec",   json_object_new_int(   p->rec));

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;
	PUTSTR("%s\r\n", json_object_to_json_string(root_obj));

	// free
	json_object_put(root_obj);
}

void put_json_voip_config(T_CGI_VOIP_CONFIG *p)
{
	json_object *voip_obj;

	voip_obj     = json_object_new_object();

	json_object_object_add(voip_obj, "model",  json_object_new_string(MODEL_NAME));
	json_object_object_add(voip_obj, "enable", json_object_new_int(p->enable));
	json_object_object_add(voip_obj, "use_stun", json_object_new_int(p->use_stun));
	json_object_object_add(voip_obj, "ipaddr", json_object_new_string(p->ipaddr));
	json_object_object_add(voip_obj, "port",   json_object_new_int(   p->port));
	json_object_object_add(voip_obj, "userid", json_object_new_string(p->userid));
	json_object_object_add(voip_obj, "passwd", json_object_new_string(p->passwd));
	json_object_object_add(voip_obj, "peerid", json_object_new_string(p->peerid));

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;
	PUTSTR("%s\r\n", json_object_to_json_string(voip_obj));

	// free
	json_object_put(voip_obj);

}

void put_json_user_config(T_CGI_USER_CONFIG *p)
{
	json_object *onvif_obj, *rtsp_acc_obj, *user_obj;

	onvif_obj    = json_object_new_object();
	rtsp_acc_obj = json_object_new_object();
	user_obj     = json_object_new_object();

	json_object_object_add(user_obj,  "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(onvif_obj, "id",    json_object_new_string(p->onvif.id));
	json_object_object_add(onvif_obj, "pw",    json_object_new_string(p->onvif.pw));
	json_object_object_add(user_obj,  "onvif", onvif_obj);

	json_object_object_add(rtsp_acc_obj, "enable",  json_object_new_int(   p->rtsp.enable));
	json_object_object_add(rtsp_acc_obj, "enctype", json_object_new_int(   p->rtsp.enctype));
	json_object_object_add(rtsp_acc_obj, "id",      json_object_new_string(p->rtsp.id));
	json_object_object_add(rtsp_acc_obj, "pw",      json_object_new_string(p->rtsp.pw));
	json_object_object_add(user_obj, "live_stream_account", rtsp_acc_obj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;
	PUTSTR("%s\r\n", json_object_to_json_string(user_obj));

	// free
	json_object_put(onvif_obj);
	json_object_put(rtsp_acc_obj);
	json_object_put(user_obj);
}

void put_json_system_config(T_CGI_SYSTEM_CONFIG *p)
{
	json_object *myobj, *systemobj;

	myobj   = json_object_new_object();
	systemobj   = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(systemobj, "model", json_object_new_string(p->model));
	json_object_object_add(systemobj, "fwver", json_object_new_string(p->fwver));
	json_object_object_add(systemobj, "devid", json_object_new_string(p->devid));
	json_object_object_add(systemobj, "uid",   json_object_new_string(p->uid));

	remove_rn_char_from_string(p->mac);
	json_object_object_add(systemobj, "mac",   json_object_new_string(p->mac));

	json_object_object_add(myobj, "system", systemobj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;
	PUTSTR("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(systemobj);
	json_object_put(myobj);
}

void put_json_servers_config(T_CGI_SERVERS_CONFIG *p)
{
	json_object *myobj, *bs_obj, *fota_obj, *mediaserver_obj, *msobj, *ddnsobj, *dnsobj, *ntpobj, *onvif_obj, *p2p_obj, *voip_obj;

	myobj     = json_object_new_object();
	bs_obj    = json_object_new_object();
	fota_obj  = json_object_new_object();
	mediaserver_obj  = json_object_new_object();
	msobj     = json_object_new_object();
	ddnsobj   = json_object_new_object();
	dnsobj    = json_object_new_object();
	ntpobj    = json_object_new_object();
	onvif_obj = json_object_new_object();
	p2p_obj   = json_object_new_object();
	voip_obj  = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(bs_obj, "enable",       json_object_new_int( p->bs.enable));
	json_object_object_add(bs_obj, "upload_files", json_object_new_int( p->bs.upload_files));
	json_object_object_add(bs_obj, "serveraddr",   json_object_new_string(p->bs.serveraddr));
	json_object_object_add(bs_obj, "port",         json_object_new_int(   p->bs.port));
	json_object_object_add(bs_obj, "id",           json_object_new_string(p->bs.id));
	json_object_object_add(bs_obj, "pw",           json_object_new_string(p->bs.pw));
	//json_object_object_add(bs_obj, "desc",      json_object_new_string("Backup Server(FTP)"));
	json_object_object_add(myobj, "bs", bs_obj);

	json_object_object_add(fota_obj, "enable",      json_object_new_int   (p->fota.enable));
	json_object_object_add(fota_obj, "server_info", json_object_new_int   (p->fota.server_info));
	json_object_object_add(fota_obj, "serveraddr",  json_object_new_string(p->fota.serveraddr));
	json_object_object_add(fota_obj, "port",        json_object_new_int   (p->fota.port));
	json_object_object_add(fota_obj, "id",          json_object_new_string(p->fota.id));
	json_object_object_add(fota_obj, "pw",          json_object_new_string(p->fota.pw));
	json_object_object_add(myobj, "fota", fota_obj);

	json_object_object_add(mediaserver_obj, "enable",            json_object_new_int   (p->mediaserver.enable));
	json_object_object_add(mediaserver_obj, "use_full_path_url", json_object_new_int   (p->mediaserver.use_full_path_url));
	json_object_object_add(mediaserver_obj, "full_path_url",     json_object_new_string(p->mediaserver.full_path_url));
	json_object_object_add(mediaserver_obj, "serveraddr",        json_object_new_string(p->mediaserver.serveraddr));
	json_object_object_add(mediaserver_obj, "port",              json_object_new_int   (p->mediaserver.port));
	// json_object_object_add(mediaserver_obj, "id",          json_object_new_string(p->fota.id));
	// json_object_object_add(mediaserver_obj, "pw",          json_object_new_string(p->fota.pw));
	json_object_object_add(myobj, "mediaserver", mediaserver_obj);

	json_object_object_add(msobj, "enable",     json_object_new_int(   p->ms.enable));
	json_object_object_add(msobj, "serveraddr", json_object_new_string(p->ms.serveraddr));
	json_object_object_add(msobj, "port",       json_object_new_int(   p->ms.port));
	//json_object_object_add(msobj, "desc",      json_object_new_string("Management Server"));
	json_object_object_add(myobj, "ms", msobj);

	json_object_object_add(ddnsobj, "enable",     json_object_new_int(   p->ddns.enable));
	json_object_object_add(ddnsobj, "serveraddr", json_object_new_string(p->ddns.serveraddr));
	json_object_object_add(ddnsobj, "hostname",   json_object_new_string(p->ddns.hostname));
	json_object_object_add(ddnsobj, "id",         json_object_new_string(p->ddns.id));
	json_object_object_add(ddnsobj, "pw",         json_object_new_string(p->ddns.pw));
	//json_object_object_add(ddnsobj, "desc",       json_object_new_string("Dynamic DNS"));
	json_object_object_add(myobj, "ddns", ddnsobj);

	json_object_object_add(dnsobj, "server1",     json_object_new_string(p->dns.server1));
	json_object_object_add(dnsobj, "server2",     json_object_new_string(p->dns.server2));
	json_object_object_add(myobj, "dns", dnsobj);

	json_object_object_add(ntpobj, "enable",     json_object_new_int(   p->ntp.enable));
	json_object_object_add(ntpobj, "serveraddr", json_object_new_string(p->ntp.serveraddr));
	json_object_object_add(myobj, "ntp", ntpobj);

	json_object_object_add(myobj, "timezone",       json_object_new_int(p->time_zone));
	json_object_object_add(myobj, "timezone_abbr",  json_object_new_string(p->time_zone_abbr));
	json_object_object_add(myobj, "daylightsaving", json_object_new_int(p->daylight_saving));

	// onvif server settings
	json_object_object_add(onvif_obj, "enable",  json_object_new_int   (p->onvif.enable));
	json_object_object_add(onvif_obj, "id",      json_object_new_string(p->onvif.id));
	json_object_object_add(onvif_obj, "pw",      json_object_new_string(p->onvif.pw));
	json_object_object_add(myobj, "onvif", onvif_obj);

	// p2p
	json_object_object_add(p2p_obj, "p2p_enable",  json_object_new_int   (p->p2p.enable));
	json_object_object_add(myobj, "p2p", p2p_obj);

	// voip
	json_object_object_add(voip_obj, "enable",  json_object_new_int(p->voip.enable));
	json_object_object_add(voip_obj, "use_stun", json_object_new_int(p->voip.use_stun));
	json_object_object_add(voip_obj, "ipaddr", json_object_new_string(p->voip.ipaddr));
	json_object_object_add(voip_obj, "port",   json_object_new_int(   p->voip.port));
	json_object_object_add(voip_obj, "userid", json_object_new_string(p->voip.userid));
	json_object_object_add(voip_obj, "passwd", json_object_new_string(p->voip.passwd));
	json_object_object_add(voip_obj, "peerid", json_object_new_string(p->voip.peerid));
	json_object_object_add(myobj, "voip", voip_obj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	PUTSTR("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(bs_obj);
	json_object_put(fota_obj);
	json_object_put(mediaserver_obj);
	json_object_put(msobj);
	json_object_put(ddnsobj);
	json_object_put(dnsobj);
	json_object_put(ntpobj);
	json_object_put(onvif_obj);
	json_object_put(p2p_obj);
	json_object_put(voip_obj);
	json_object_put(myobj);

}//put_json_servers_config

// deprecate?
void put_json_network_config(T_CGI_NETWORK_CONFIG *p)
{
	json_object *myobj, *wirelessobj, *cradleobj, *wifiapobj, *livestmobj;

	myobj       = json_object_new_object();
	wirelessobj = json_object_new_object();
	cradleobj   = json_object_new_object();
	wifiapobj   = json_object_new_object();
	livestmobj  = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	//json_object_object_add(myobj, "wifi_ap_multi", json_object_new_int(   p->wifi_ap_multi));

	json_object_object_add(wirelessobj, "addr_type", json_object_new_int(p->wireless.addr_type));
	json_object_object_add(wirelessobj, "ipv4",      json_object_new_string(p->wireless.ipv4));
	json_object_object_add(wirelessobj, "gateway",   json_object_new_string(p->wireless.gw));
	json_object_object_add(wirelessobj, "netmask",   json_object_new_string(p->wireless.mask));
	json_object_object_add(myobj, "wireless", wirelessobj);

	json_object_object_add(cradleobj, "addr_type", json_object_new_int(p->cradle.addr_type));
	json_object_object_add(cradleobj, "ipv4",      json_object_new_string(p->cradle.ipv4));
	json_object_object_add(cradleobj, "gateway",   json_object_new_string(p->cradle.gw));
	json_object_object_add(cradleobj, "netmask",   json_object_new_string(p->cradle.mask));
	json_object_object_add(myobj, "cradle", cradleobj);

	json_object_object_add(wifiapobj, "ssid", json_object_new_string(p->wifi_ap.id));
	json_object_object_add(wifiapobj, "pw",   json_object_new_string(p->wifi_ap.pw));
	json_object_object_add(myobj, "wifi_ap", wifiapobj);

	json_object_object_add(livestmobj, "enable",  json_object_new_int(   p->live_stream_account_enable));
	json_object_object_add(livestmobj, "enctype", json_object_new_int(   p->live_stream_account_enctype));
	json_object_object_add(livestmobj, "id",      json_object_new_string(p->live_stream_account.id));
	json_object_object_add(livestmobj, "pw",      json_object_new_string(p->live_stream_account.pw));
	json_object_object_add(myobj, "live_stream_account", livestmobj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	PUTSTR("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(wirelessobj);
	json_object_put(cradleobj);
	json_object_put(wifiapobj);
	json_object_put(livestmobj);
	json_object_put(myobj);
}

// multiple Wifi-AP
void put_json_network_config2(T_CGI_NETWORK_CONFIG2 *p)
{
	int i;
	json_object *network, *wirelessobj, *cradleobj, *wifiapobj, *livestmobj, *wifilist;
	json_object *sslvpn;
    json_object* wifiInfo[4];

	network     = json_object_new_object();
	wirelessobj = json_object_new_object();
	cradleobj   = json_object_new_object();
	wifiapobj   = json_object_new_object();
	wifilist    = json_object_new_array();
	livestmobj  = json_object_new_object();
	sslvpn      = json_object_new_object();

	json_object_object_add(network, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(network, "wifi_ap_multi", json_object_new_int(p->wifi_ap_multi));

	json_object_object_add(wirelessobj, "addr_type", json_object_new_int(p->wireless.addr_type));
	json_object_object_add(wirelessobj, "ipv4",      json_object_new_string(p->wireless.ipv4));
	json_object_object_add(wirelessobj, "gateway",   json_object_new_string(p->wireless.gw));
	json_object_object_add(wirelessobj, "netmask",   json_object_new_string(p->wireless.mask));
	json_object_object_add(network, "wireless", wirelessobj);

	json_object_object_add(cradleobj, "addr_type", json_object_new_int(p->cradle.addr_type));
	json_object_object_add(cradleobj, "ipv4",      json_object_new_string(p->cradle.ipv4));
	json_object_object_add(cradleobj, "gateway",   json_object_new_string(p->cradle.gw));
	json_object_object_add(cradleobj, "netmask",   json_object_new_string(p->cradle.mask));
	json_object_object_add(network, "cradle", cradleobj);

	json_object_object_add(wifiapobj, "ssid", json_object_new_string(p->wifi_ap.id));
	json_object_object_add(wifiapobj, "pass",   json_object_new_string(p->wifi_ap.pw));
	json_object_object_add(network, "wifi_ap", wifiapobj);

	char fname[10];
	for(i = 0 ; i < 4 ; i++)
	{
        wifiInfo[i] = json_object_new_object() ;

		sprintf(fname,"ssid");
		json_object_object_add(wifiInfo[i], fname,    json_object_new_string(p->wifi_ap_list[i].id));
		sprintf(fname,"pass");
		json_object_object_add(wifiInfo[i], fname,    json_object_new_string(p->wifi_ap_list[i].pw));

		json_object_array_add(wifilist, wifiInfo[i]);
	}
	json_object_object_add(network, "wifilist", wifilist) ;

	json_object_object_add(livestmobj, "enable",  json_object_new_int(   p->live_stream_account_enable));
	json_object_object_add(livestmobj, "enctype", json_object_new_int(   p->live_stream_account_enctype));
	json_object_object_add(livestmobj, "id",      json_object_new_string(p->live_stream_account.id));
	json_object_object_add(livestmobj, "pw",      json_object_new_string(p->live_stream_account.pw));
	json_object_object_add(network,    "live_stream_account", livestmobj);


	// ssl vpn
	{
		json_object_object_add(sslvpn, "enable",              json_object_new_int(    p->sslvpn.enable ));
		json_object_object_add(sslvpn, "vendor",              json_object_new_int(    p->sslvpn.vendor ));
		json_object_object_add(sslvpn, "vpn_id",              json_object_new_string( p->sslvpn.vpn_id ));
		json_object_object_add(sslvpn, "heartbeat_interval",  json_object_new_int(    p->sslvpn.heartbeat_interval ));
		json_object_object_add(sslvpn, "heartbeat_threshold", json_object_new_int(    p->sslvpn.heartbeat_threshold ));
		json_object_object_add(sslvpn, "protocol",            json_object_new_int(    p->sslvpn.protocol ));
		json_object_object_add(sslvpn, "port",                json_object_new_int(    p->sslvpn.port ));
		json_object_object_add(sslvpn, "queue_size",          json_object_new_int(    p->sslvpn.queue_size ));
		json_object_object_add(sslvpn, "key",                 json_object_new_string( p->sslvpn.key ));
		json_object_object_add(sslvpn, "encrypt_type",        json_object_new_int(    p->sslvpn.encrypt_type ));
		json_object_object_add(sslvpn, "ipaddress",           json_object_new_string( p->sslvpn.ipaddress ));
		json_object_object_add(sslvpn, "network_interface",   json_object_new_int(    p->sslvpn.network_interface ));

		json_object_object_add(network, "sslvpn", sslvpn ) ;
	}

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	PUTSTR("%s\r\n", json_object_to_json_string(network));

	// free
	json_object_put(wirelessobj);
	json_object_put(cradleobj);
	json_object_put(wifiapobj);
	for(i = 0 ; i < 4 ; i++) json_object_put(wifiInfo[i]);
	json_object_put(wifilist);
	json_object_put(livestmobj);
	json_object_put(sslvpn);
	json_object_put(network);
}

void put_json_operation_config(T_CGI_OPERATION_CONFIG *p)
{
	json_object *myobj, *streamobj, *misc_obj, *recordobj;

	myobj = json_object_new_object();
	streamobj = json_object_new_object();
	recordobj = json_object_new_object();
	misc_obj  = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(streamobj, "enable_audio",  json_object_new_int(p->stm.enable_audio));
	json_object_object_add(myobj, "stream", streamobj);

	json_object_object_add(recordobj, "pre_rec", json_object_new_int(p->rec.pre_rec));
	json_object_object_add(recordobj, "auto_rec", json_object_new_int(p->rec.auto_rec));
	json_object_object_add(recordobj, "audio_rec", json_object_new_int(p->rec.audio_rec));
	json_object_object_add(recordobj, "rec_interval",  json_object_new_int(p->rec.interval));
	json_object_object_add(recordobj, "rec_overwrite",  json_object_new_int(p->rec.overwrite));
	json_object_object_add(myobj, "record", recordobj);

	json_object_object_add(misc_obj, "display_datetime", json_object_new_int(p->display_datetime));
	json_object_object_add(myobj, "misc", misc_obj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	printf("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(streamobj);
	json_object_put(recordobj);
	json_object_put(misc_obj);
	json_object_put(myobj);
}

void put_json_camera_config(T_CGI_VIDEO_QUALITY *p)
{
	json_object *myobj, *deviceobj, *recordobj, *streamobj;

	myobj = json_object_new_object();
	deviceobj = json_object_new_object();
	recordobj = json_object_new_object();
	streamobj = json_object_new_object();

#if 0
	json_object_object_add(deviceobj, "model_name", json_object_new_string(MODEL_NAME));
	json_object_object_add(deviceobj, "fw_ver", json_object_new_string("2.2.2"));
	json_object_object_add(deviceobj, "hw_ver", json_object_new_string("1.1.1"));
	json_object_object_add(myobj, "device", deviceobj);
#else
	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));
#endif

	// record info
#if defined(NEXXONE)|| defined(NEXX360W) || defined(NEXX360M) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
	|| defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
	int fpsIdx = p->rec.fps-1;
	if(fpsIdx < 0 ) fpsIdx = 0;
	if(fpsIdx > MAX_FPS) fpsIdx = MAX_FPS-1;

	int bpsIdx = 0;
	int kbps = p->rec.bps; // kbps
	switch (kbps) {
		case 512:
			bpsIdx = 0;
			break;
		case 1000:
			bpsIdx = 1;
			break;
		case 2000:
			bpsIdx = 2;
			break;
		case 3000:
			bpsIdx = 3;
			break;
		case 4000:
			bpsIdx = 4;
			break;
	}
	json_object_object_add(recordobj, "fps", json_object_new_int(fpsIdx));
	json_object_object_add(recordobj, "bps", json_object_new_int(bpsIdx)); // kbps
#else
	json_object_object_add(recordobj, "fps", json_object_new_int(p->rec.fps));
	json_object_object_add(recordobj, "bps", json_object_new_int(p->rec.bps));
#endif
	json_object_object_add(recordobj, "gop", json_object_new_int(p->rec.gop));
	json_object_object_add(recordobj, "rc",  json_object_new_int(p->rec.rc));
	json_object_object_add(myobj, "record", recordobj);

	// streaming info
	json_object_object_add(streamobj, "resolution", json_object_new_int(p->stm.res));
#if defined(NEXXONE) || defined(NEXX360W) || defined(NEXX360M) || defined(NEXXB) || defined(NEXX360W_MUX) || defined(NEXXB_ONE)\
	|| defined(NEXX360B) || defined(NEXX360C) || defined(NEXX360W_CCTV)
	fpsIdx = p->stm.fps-1;
	if(fpsIdx < 0 ) fpsIdx = 0;
	if(fpsIdx > MAX_FPS) fpsIdx = MAX_FPS-1;

	bpsIdx = 0;
	kbps = p->stm.bps; // kbps
	switch (kbps) {
		case 128:
			bpsIdx = 0;
			break;
		case 256:
			bpsIdx = 1;
			break;
		case 512:
			bpsIdx = 2;
			break;
		case 1000:
			bpsIdx = 3;
			break;
		case 2000:
			bpsIdx = 4;
			break;
		case 3000:
			bpsIdx = 5;
			break;
		case 4000:
			bpsIdx = 6;
			break;
		case 5000:
			bpsIdx = 7;
			break;
		case 6000:
			bpsIdx = 8;
			break;
		case 7000:
			bpsIdx = 9;
			break;
		case 8000:
			bpsIdx = 10;
			break;
	}

	json_object_object_add(streamobj, "fps", json_object_new_int(fpsIdx));
	json_object_object_add(streamobj, "bps", json_object_new_int(bpsIdx)); // kbps
#else
	json_object_object_add(streamobj, "fps",        json_object_new_int(p->stm.fps));
	json_object_object_add(streamobj, "bps",        json_object_new_int(p->stm.bps));
#endif
	json_object_object_add(streamobj, "gop",        json_object_new_int(p->stm.gop));
	json_object_object_add(streamobj, "rc",         json_object_new_int(p->stm.rc));
	json_object_object_add(myobj, "stream", streamobj);

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	printf("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(deviceobj);
	json_object_put(recordobj);
	json_object_put(streamobj);
	json_object_put(myobj);
}

int do_search(char *pContents)
{
	int ret=-1,i;

    T_CGIPRM prm[128];
    int cnt = parseContents(pContents, prm);
    CGI_DBG("cnt:%d\n", cnt);

    if(cnt>0){
        for(i=0;i<cnt;i++) {
			CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);
			if(!strcmp(prm[i].name, "param")){
				if(!strcmp(prm[i].value, "all_config")){
					put_json_all_config();
					return 0;
				}
				else if(!strcmp(prm[i].value, "camera_config")){
					T_CGI_VIDEO_QUALITY q;memset(&q,0, sizeof q);
					sysctl_message(UDS_GET_VIDEO_QUALITY, (void*)&q, sizeof(q));
					put_json_camera_config(&q);
					return 0;
				}
				else if(!strcmp(prm[i].value, "operation_config")){
					T_CGI_OPERATION_CONFIG t;memset(&t,0, sizeof t);
					sysctl_message(UDS_GET_OPERATION_CONFIG, (void*)&t, sizeof t );
					put_json_operation_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "network_config")){
					T_CGI_NETWORK_CONFIG t;memset(&t,0, sizeof t);
					sysctl_message(UDS_GET_NETWORK_CONFIG, (void*)&t, sizeof t );
					put_json_network_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "network_config2")){
					// Multiple Wifi-ap
					T_CGI_NETWORK_CONFIG2 t;memset(&t,0, sizeof t);
					sysctl_message(UDS_GET_NETWORK_CONFIG2, (void*)&t, sizeof t );
					put_json_network_config2(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "servers_config")){
					T_CGI_SERVERS_CONFIG t;memset(&t,0, sizeof t);
					sysctl_message(UDS_GET_SERVERS_CONFIG, (void*)&t, sizeof t );
					put_json_servers_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "system_config")){
					T_CGI_SYSTEM_CONFIG t;memset(&t,0, sizeof t);
					sysctl_message(UDS_GET_SYSTEM_CONFIG, (void*)&t, sizeof t );
					put_json_system_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "user_config")){
					T_CGI_USER_CONFIG t;memset(&t, 0, sizeof t);
					sysctl_message(UDS_GET_USER_CONFIG, (void*)&t, sizeof t);
					put_json_user_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "voip_config")){
					T_CGI_VOIP_CONFIG t;memset(&t, 0, sizeof t);
					sysctl_message(UDS_GET_VOIP_CONFIG, (void*)&t, sizeof t);
					put_json_voip_config(&t);
					return 0;
				}
				else if(!strcmp(prm[i].value, "system_info")){
					T_CGI_SYSTEM_INFO t;memset(&t, 0, sizeof t);
					sysctl_message(UDS_GET_SYSTEM_INFO, (void*)&t, sizeof t);
					put_json_system_info(&t);
					return 0;
				}
				else {
				}
            } 
            else {
				// nothing ...
            }
		}
	}

	return ret;
}

int do_update(char *pContents)
{
	int ret=-1;

	return ret;
}

int main(int argc, char *argv[])
{
	int ret, nMethod, nAction;
	char *pQuery = NULL;
    char *pContents=NULL;

	pQuery = getenv("REQUEST_METHOD");
	if(pQuery != NULL){
		if(strcmp(pQuery, "GET") == 0){
			nMethod = GET;
			pQuery = getenv("QUERY_STRING");
		}
		else {
			nMethod = POST;
			pQuery = get_cgi_contents();
		}
	}else{
		NS_PRM::send_response(ERR_INVALID_METHOD);
		return 0;
	}

	if(pQuery == NULL){
		NS_PRM::send_response(ERR_INVALID_PARAM);
		return 0;
	}
    CGI_DBG("pQuery:%s\n", pQuery);

	if( strstr(pQuery, "action=update")){
		nAction = UPDATE;
	}
	else if( strstr(pQuery, "action=search")){
		nAction = SEARCH;
	}
	else if( strstr(pQuery, "action=sysmng")){
		nAction = SYSMNG;
	}
	else {
		NS_PRM::send_response(ERR_INVALID_ACTION);
		if(nMethod == POST) free(pQuery);
		return 0;
	}

	if(nMethod == GET){
		pQuery = strstr(pQuery, "&");
		if(!pQuery){
			NS_PRM::send_response(ERR_INVALID_PARAM);
			return 0;
		}
		int len = strlen(pQuery+1);
		CGI_DBG("Param is GET, pQuery:%s, len:%d\n", pQuery+1, len);

		pContents = (char*)malloc(len+1);
		memset(pContents, 0, len+1);
		memcpy(pContents, pQuery+1, len);

	}else{ // POST
	}


	if(nAction == SYSMNG){
		ret = do_sysmng(pContents);
		NS_PRM::send_response(ERR_INVALID_PARAM);
	}
	else if(nAction == SEARCH){
		ret = do_search(pContents);
	}
	else if(nAction == UPDATE){
		ret = do_update(pContents);
	}
	else {
		NS_PRM::send_response(ERR_INVALID_ACTION);
	}

	if(pContents)
		free(pContents);

	if(ret != 0 ) 
		NS_PRM::send_response(ret);

#if 0
	if(ret == SUBMIT_OK){
	}
	else //if(ret != 0 ) 
	{
	}
#endif

	return 0;
}

