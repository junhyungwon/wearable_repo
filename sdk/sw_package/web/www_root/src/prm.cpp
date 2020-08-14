/*
 * @file  prm.cpp
 * @brief process actions...search, update, delete ...
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
#define XML  1
#define JSON 2


#if 0
// 이걸 사용할 경우, all_config, network_config, servers_config 에서 header와 contents가 구분이 안되는 증상이 발생됨.
// PUT_CRLF를 printf를 써서 그런지도 모름.
static char out_buf[1024];
#define PUTSTR(fmt, args...)	write(STDOUT_FILENO, out_buf, sprintf(out_buf, fmt, ##args))
#else
#define PUTSTR printf
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
	int ret=-1;

	json_object *all_config;
	json_object *camera_obj, *recordobj, *streamobj;
	json_object *operation_obj, *misc_obj, *rec_obj, *p2p_obj;
	json_object *network_obj, *wireless_obj, *cradle_obj, *wifiap_obj, *livestm_obj;
	json_object *servers_obj, *bs_obj, *ms_obj, *ddns_obj, *dns_obj, *ntp_obj, *onvif_obj;
	json_object *system_obj;

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

		json_object_object_add(recordobj, "fps", json_object_new_int(p.rec.fps));
		json_object_object_add(recordobj, "bps", json_object_new_int(p.rec.bps));
		json_object_object_add(recordobj, "gop", json_object_new_int(p.rec.gop));
		json_object_object_add(recordobj, "rc",  json_object_new_int(p.rec.rc));
		//json_object_object_add(recordobj, "desc",  json_object_new_string("Video Quality for Recording"));
		json_object_object_add(camera_obj, "record", recordobj);

		json_object_object_add(streamobj, "resolution", json_object_new_int(p.stm.res));
		json_object_object_add(streamobj, "fps", json_object_new_int(p.stm.fps));
		json_object_object_add(streamobj, "bps", json_object_new_int(p.stm.bps));
		json_object_object_add(streamobj, "gop", json_object_new_int(p.stm.gop));
		json_object_object_add(streamobj, "rc",  json_object_new_int(p.stm.rc));
		//json_object_object_add(recordobj, "desc",  json_object_new_string("Video Quality for Streaming"));
		json_object_object_add(camera_obj, "stream", streamobj);
	}
	json_object_object_add(all_config, "camera_settings", camera_obj);


	// Operation Settings
	operation_obj = json_object_new_object();
	rec_obj  = json_object_new_object();
	misc_obj = json_object_new_object();
	p2p_obj  = json_object_new_object();
	{
		T_CGI_OPERATION_CONFIG p;memset(&p,0, sizeof p);
		if(0>sysctl_message(UDS_GET_OPERATION_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_OPERATION_OBJ;
		}

		json_object_object_add(rec_obj, "pre_rec",   json_object_new_int(p.rec.pre_rec));
		json_object_object_add(rec_obj, "auto_rec", json_object_new_int(p.rec.audio_rec));
		json_object_object_add(rec_obj, "audio_rec",  json_object_new_int(p.rec.auto_rec));
		json_object_object_add(rec_obj, "rec_interval", json_object_new_int(p.rec.interval));
		json_object_object_add(rec_obj, "rec_overwrite",    json_object_new_int(p.rec.overwrite));
		json_object_object_add(operation_obj, "record", rec_obj);

		json_object_object_add(misc_obj, "display_datetime", json_object_new_int(p.display_datetime));
		json_object_object_add(operation_obj, "misc", misc_obj);

		if( strcmp(MODEL_NAME, "NEXX360") == 0){
			json_object_object_add(p2p_obj, "p2p_enable",   json_object_new_int(p.p2p.enable));
			json_object_object_add(p2p_obj, "p2p_username", json_object_new_string(p.p2p.username));
			json_object_object_add(p2p_obj, "p2p_password", json_object_new_string(p.p2p.password));
			json_object_object_add(operation_obj, "p2p", p2p_obj);
		}
	}
	json_object_object_add(all_config, "operation_settings", operation_obj);

	// network
	network_obj  = json_object_new_object();
	wireless_obj = json_object_new_object();
	cradle_obj   = json_object_new_object();
	wifiap_obj   = json_object_new_object();
	livestm_obj  = json_object_new_object();
	{
		T_CGI_NETWORK_CONFIG p;memset(&p, 0, sizeof p);
		if(0>sysctl_message(UDS_GET_NETWORK_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_NETWORK_OBJ;
		}
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
	}
	json_object_object_add(all_config, "network_settings", network_obj);


	// servers
	servers_obj = json_object_new_object();
	bs_obj = json_object_new_object();
	ms_obj = json_object_new_object();
	ddns_obj = json_object_new_object();
	dns_obj = json_object_new_object();
	ntp_obj = json_object_new_object();
	onvif_obj = json_object_new_object();
	{
		T_CGI_SERVERS_CONFIG p;memset(&p, 0, sizeof p);
		if(0>sysctl_message(UDS_GET_SERVERS_CONFIG, (void*)&p, sizeof p )){
			ret = -1;
			goto _FREE_SERVERS_OBJ;
		}

		json_object_object_add(bs_obj, "enable",     json_object_new_int(   p.bs.enable));
		json_object_object_add(bs_obj, "serveraddr", json_object_new_string(p.bs.serveraddr));
		json_object_object_add(bs_obj, "port",       json_object_new_int(   p.bs.port));
		json_object_object_add(bs_obj, "id",         json_object_new_string(p.bs.id));
		json_object_object_add(bs_obj, "pw",         json_object_new_string(p.bs.pw));
		json_object_object_add(servers_obj, "bs", bs_obj);

		json_object_object_add(ms_obj, "enable",     json_object_new_int(   p.ms.enable));
		json_object_object_add(ms_obj, "serveraddr", json_object_new_string(p.ms.serveraddr));
		json_object_object_add(ms_obj, "port",       json_object_new_int(   p.ms.port));
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
		json_object_object_add(system_obj, "fwver.", json_object_new_string(p.fwver));
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
	json_object_put(ms_obj);
	json_object_put(ddns_obj);
	json_object_put(dns_obj);
	json_object_put(ntp_obj);
	json_object_put(onvif_obj);
	json_object_put(servers_obj);

_FREE_NETWORK_OBJ:
	json_object_put(wireless_obj);
	json_object_put(cradle_obj);
	json_object_put(wifiap_obj);
	json_object_put(livestm_obj);
	json_object_put(network_obj);

_FREE_OPERATION_OBJ:
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
	json_object *myobj, *bsobj, *msobj, *ddnsobj, *dnsobj, *ntpobj, *onvif_obj;

	myobj   = json_object_new_object();
	bsobj   = json_object_new_object();
	msobj   = json_object_new_object();
	ddnsobj = json_object_new_object();
	dnsobj  = json_object_new_object();
	ntpobj  = json_object_new_object();
	onvif_obj = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(bsobj, "enable",     json_object_new_int(   p->bs.enable));
	json_object_object_add(bsobj, "serveraddr", json_object_new_string(p->bs.serveraddr));
	json_object_object_add(bsobj, "port",       json_object_new_int(   p->bs.port));
	json_object_object_add(bsobj, "id",         json_object_new_string(p->bs.id));
	json_object_object_add(bsobj, "pw",         json_object_new_string(p->bs.pw));
	//json_object_object_add(bsobj, "desc",      json_object_new_string("Backup Server(FTP)"));
	json_object_object_add(myobj, "bs", bsobj);

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

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	PUTSTR("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(bsobj);
	json_object_put(msobj);
	json_object_put(ddnsobj);
	json_object_put(dnsobj);
	json_object_put(ntpobj);
	json_object_put(onvif_obj);
	json_object_put(myobj);
}

void put_json_network_config(T_CGI_NETWORK_CONFIG *p)
{
	json_object *myobj, *wirelessobj, *cradleobj, *wifiapobj, *livestmobj;

	myobj       = json_object_new_object();
	wirelessobj = json_object_new_object();
	cradleobj   = json_object_new_object();
	wifiapobj   = json_object_new_object();
	livestmobj  = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

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

void put_json_operation_config(T_CGI_OPERATION_CONFIG *p)
{
	json_object *myobj, *misc_obj, *recordobj, *p2pobj;

	myobj = json_object_new_object();
	recordobj = json_object_new_object();
	p2pobj    = json_object_new_object();
	misc_obj  = json_object_new_object();

	json_object_object_add(myobj, "model", json_object_new_string(MODEL_NAME));

	json_object_object_add(recordobj, "pre_rec", json_object_new_int(p->rec.pre_rec));
	json_object_object_add(recordobj, "auto_rec", json_object_new_int(p->rec.auto_rec));
	json_object_object_add(recordobj, "audio_rec", json_object_new_int(p->rec.audio_rec));
	json_object_object_add(recordobj, "rec_interval",  json_object_new_int(p->rec.interval));
	json_object_object_add(recordobj, "rec_overwrite",  json_object_new_int(p->rec.overwrite));
	json_object_object_add(myobj, "record", recordobj);

	json_object_object_add(misc_obj, "display_datetime", json_object_new_int(p->display_datetime));
	json_object_object_add(myobj, "misc", misc_obj);

	if( strcmp(MODEL_NAME, "NEXX360") == 0){
		json_object_object_add(p2pobj, "p2p_enable",   json_object_new_int(p->p2p.enable));
		json_object_object_add(p2pobj, "p2p_username", json_object_new_string(p->p2p.username));
		json_object_object_add(p2pobj, "p2p_password", json_object_new_string(p->p2p.password));
		json_object_object_add(myobj, "p2p", p2pobj);
	}

	PUT_CACHE_CONTROL_NOCACHE;
	PUT_CONTENT_TYPE_JSON;
	PUT_CRLF;

	printf("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(recordobj);
	json_object_put(misc_obj);
	json_object_put(p2pobj);
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

	json_object_object_add(recordobj, "fps", json_object_new_int(p->rec.fps));
	json_object_object_add(recordobj, "bps", json_object_new_int(p->rec.bps));
	json_object_object_add(recordobj, "gop", json_object_new_int(p->rec.gop));
	json_object_object_add(recordobj, "rc",  json_object_new_int(p->rec.rc));
	json_object_object_add(myobj, "record", recordobj);

	json_object_object_add(streamobj, "resolution", json_object_new_int(p->stm.res));
	json_object_object_add(streamobj, "fps",        json_object_new_int(p->stm.fps));
	json_object_object_add(streamobj, "bps",        json_object_new_int(p->stm.bps));
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

