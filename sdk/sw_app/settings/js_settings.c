#include <stdio.h>
#include <json-c/json.h> 

#include "app_base64.h"
#include "js_settings.h"
#include "board_config.h"

json_object *json_find_obj (json_object * jobj, char *find_key) 
{ 
	size_t key_len = strlen(find_key); 
	json_object_object_foreach(jobj, key, val) 
	{ 
		if (strlen(key) == key_len && !memcmp (key, find_key, key_len)) 
			return val;
	} 
	return NULL; // not found
}

char * js_base64_decode_str(json_object *jobj, size_t * dec_size)
{
	char * tmp = (char*)json_object_get_string(jobj);
	char * ret = base64_decode(tmp, strlen(tmp), dec_size);

	return ret;
}

static int	parseSSLvpnInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "sslvpn_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find sslvpn_info\n");
		return -1;
	}

	json_object* tmp ;
    if(json_find_obj(jobj, "ON_OFF") != NULL)
    {
		tmp = json_object_object_get(jobj, "ON_OFF");
		set->sslvpn_info.ON_OFF = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN ON_OFF 항목 없음\n") ;
		set->sslvpn_info.ON_OFF = -1;
	}

    if(json_find_obj(jobj, "vendor") != NULL)
    {
		tmp = json_object_object_get(jobj, "vendor");
		set->sslvpn_info.vendor = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN vendor 항목 없음\n") ;
		set->sslvpn_info.vendor = -1;
	}

    if(json_find_obj(jobj, "vpn_id") != NULL)
    {
		tmp = json_object_object_get(jobj, "vpn_id");
		sprintf(set->sslvpn_info.vpn_id, "%s", json_object_get_string(tmp));
	}
	else
	{
		sprintf(set->sslvpn_info.vpn_id, "%s",  SSLVPN_ID);
	}

    if(json_find_obj(jobj, "heartbeat_interval") != NULL)
    {
		tmp = json_object_object_get(jobj, "heartbeat_interval");
		set->sslvpn_info.heartbeat_interval = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN heartbeat_interval 항목 없음\n") ;
		set->sslvpn_info.heartbeat_interval = -1;
	}

    if(json_find_obj(jobj, "heartbeat_threshold") != NULL)
    {
		tmp = json_object_object_get(jobj, "heartbeat_threshold");
		set->sslvpn_info.heartbeat_threshold = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN heartbeat_threshold 항목 없음\n") ;
		set->sslvpn_info.heartbeat_threshold = -1;
	}

    if(json_find_obj(jobj, "protocol") != NULL)
    {
		tmp = json_object_object_get(jobj, "protocol");
		set->sslvpn_info.protocol = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN protocol 항목 없음\n") ;
		set->sslvpn_info.protocol = -1;
	}

    if(json_find_obj(jobj, "port") != NULL)
    {
		tmp = json_object_object_get(jobj, "port");
		set->sslvpn_info.port = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN port 항목 없음\n") ;
		set->sslvpn_info.port = -1;
	}

    if(json_find_obj(jobj, "queue") != NULL)
    {
		tmp = json_object_object_get(jobj, "queue");
		set->sslvpn_info.queue = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN queue 항목 없음\n") ;
		set->sslvpn_info.queue = -1;
	}

    if(json_find_obj(jobj, "key") != NULL)
    {
		tmp = json_object_object_get(jobj, "key");
		sprintf(set->sslvpn_info.key, "%s", json_object_get_string(tmp));
	}
	else
	{
		printf("SSLVPN key 항목 없음\n") ;
		sprintf(set->sslvpn_info.key, "%s", SSLVPN_KEY) ;
	}

    if(json_find_obj(jobj, "encrypt_type") != NULL)
    {
		tmp = json_object_object_get(jobj, "encrypt_type");
		set->sslvpn_info.encrypt_type = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN encrypt_type 항목 없음\n") ;
		set->sslvpn_info.encrypt_type = -1;
	}

    if(json_find_obj(jobj, "ipaddr") != NULL)
    {
		tmp = json_object_object_get(jobj, "ipaddr");
		sprintf(set->sslvpn_info.ipaddr, "%s", json_object_get_string(tmp));
	}
	else
	{
		printf("SSLVPN ipaddr 항목 없음\n") ;
		sprintf(set->sslvpn_info.ipaddr, "%s", SSLVPN_IPADDRESS) ;
	}

    if(json_find_obj(jobj, "NI") != NULL)
    {
		tmp = json_object_object_get(jobj, "NI");
		set->sslvpn_info.NI = json_object_get_int(tmp);
	}
	else
	{
		printf("SSLVPN NI 항목 없음\n") ;
		set->sslvpn_info.NI = -1;
	}

	return 0;
}

static int	parseRtmpInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "rtmp_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find rtmp_info\n");
		return -1;
	}
	json_object* tmp ;
    if(json_find_obj(jobj, "ON_OFF") != NULL)
    {
		tmp = json_object_object_get(jobj, "ON_OFF");
		set->rtmp.ON_OFF = json_object_get_int(tmp);
	}
	else
	{
		printf("RTMP ON_OFF 항목 없음\n") ;
		set->rtmp.ON_OFF = -1;
	}

    if(json_find_obj(jobj, "USE_URL") != NULL)
    {
		tmp = json_object_object_get(jobj, "USE_URL");
		set->rtmp.USE_URL = json_object_get_int(tmp);
	}
	else
	{
		printf("RTMP USE_URL 항목 없음\n") ;
		set->rtmp.USE_URL = -1;
	}

    if(json_find_obj(jobj, "FULL_URL") != NULL)
    {
		tmp = json_object_object_get(jobj, "FULL_URL");
		sprintf(set->rtmp.FULL_URL, "%s", json_object_get_string(tmp));
	}
	else
	{
		printf("RTMP FULL_URL 항목 없음\n") ;
		sprintf(set->rtmp.FULL_URL, "%s",  RTMP_SERVER_URL);
	}

	tmp = json_object_object_get(jobj, "ipaddr");
	sprintf(set->rtmp.ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "port");
	set->rtmp.port = json_object_get_int(tmp);

	return 0;
}

static int	parseVoipInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "voip_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find voip_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "ipaddr");
	sprintf(set->voip.ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "port");
	set->voip.port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "userid");
	sprintf(set->voip.userid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "passwd");
	sprintf(set->voip.passwd, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "peerid");
	sprintf(set->voip.peerid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "use_stun");
	set->voip.use_stun = json_object_get_int(tmp);

    if(json_find_obj(jobj, "ON_OFF") != NULL)
    {
		tmp = json_object_object_get(jobj, "ON_OFF");
		set->voip.ON_OFF = json_object_get_int(tmp);
	}
	else
		set->voip.ON_OFF = -1;

	return 0;
}

static int	parseAccountInfo(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "account_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find account_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "ON_OFF");
	set->account_info.ON_OFF = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "enctype");
	set->account_info.enctype = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "rtsp_userid");
	memcpy(set->account_info.rtsp_userid, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "rtsp_passwd");
	dec_size = 0;
	memcpy(set->account_info.rtsp_passwd, js_base64_decode_str(tmp, &dec_size), dec_size);

	printf("set->account_info.rtsp_userid: %s\n", set->account_info.rtsp_userid);
	printf("set->account_info.rtsp_passwd: %s\n", set->account_info.rtsp_passwd);

	json_object* webuser = json_object_object_get(jobj, "webuser");
	type = json_object_get_type(webuser);
	if( type == json_type_object) { // must be jobject
		tmp = json_object_object_get(webuser, "authtype");
		set->account_info.webuser.authtype = json_object_get_int(tmp);
		tmp = json_object_object_get(webuser, "id");
		dec_size = 0;
		memcpy(set->account_info.webuser.id, js_base64_decode_str(tmp, &dec_size), dec_size);
 		tmp = json_object_object_get(webuser, "pw");
		dec_size = 0;
		memcpy(set->account_info.webuser.pw, js_base64_decode_str(tmp, &dec_size), dec_size);
		tmp = json_object_object_get(webuser, "lv");
		set->account_info.webuser.lv = json_object_get_int(tmp);

		printf("set->account_info.webuser.id: %s\n", set->account_info.webuser.id);
		printf("set->account_info.webuser.pw: %s\n", set->account_info.webuser.pw);
	}

	json_object* onvif = json_object_object_get(jobj, "onvif");
	type = json_object_get_type(onvif);
	if( type == json_type_object) { // must be jobject
		tmp = json_object_object_get(onvif, "id");
		memcpy(set->account_info.onvif.id, js_base64_decode_str(tmp, &dec_size), dec_size);
		tmp = json_object_object_get(onvif, "pw");
		memcpy(set->account_info.onvif.pw, js_base64_decode_str(tmp, &dec_size), dec_size);
		tmp = json_object_object_get(onvif, "lv");
		set->account_info.onvif.lv = json_object_get_int(tmp);

		printf("set->account_info.onvif.pw: %s\n", set->account_info.onvif.pw);
		printf("set->account_info.onvif.id: %s\n", set->account_info.onvif.id);
	}

	return 0;
}
static int  parseMultiapInfo(app_set_t* const set, json_object* rootObj)
{
	//size_t dec_size = 0;
	const char* STR_FIELD = "multi_ap";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find multi_ap\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "ON_OFF");
	set->multi_ap.ON_OFF = json_object_get_int(tmp);
	return 0;
}

static int	parseTimeInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "time_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find time_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "time_zone");
	set->time_info.time_zone = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "time_server");
	sprintf(set->time_info.time_server, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "daylight_saving");
	set->time_info.daylight_saving = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "timesync_type");
	set->time_info.timesync_type = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "time_zone_abbr");
	sprintf(set->time_info.time_zone_abbr, "%s", json_object_get_string(tmp));

	return 0;
}
static int	parseDdnsInfo(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "ddns_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find ddns_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "ON_OFF");
	set->ddns_info.ON_OFF = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "userId");
	memcpy(set->ddns_info.userId, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "passwd");
	memcpy(set->ddns_info.passwd, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "serveraddr");
	sprintf(set->ddns_info.serveraddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "hostname");
	sprintf(set->ddns_info.hostname, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "interval");
	set->ddns_info.interval = json_object_get_int(tmp);

	printf("set->ddns_info.userId: %s\n", set->ddns_info.userId);
	printf("set->ddns_info.passwd: %s\n", set->ddns_info.passwd);

	return 0;
}

static int	parseStmInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "stm_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find stm_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "enable_audio");
	set->stm_info.enable_audio = json_object_get_int(tmp);

	return 0;

}

static int	parseRecInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "rec_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find rec_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "period_idx");
	set->rec_info.period_idx = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "overwrite");
	set->rec_info.overwrite = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "pre_rec");
	set->rec_info.pre_rec = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "auto_rec");
	set->rec_info.auto_rec = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "audio_rec");
	set->rec_info.audio_rec = json_object_get_int(tmp);

	return 0;
}
static int	parseSystemInfo(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "sys_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find sys_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "fw_ver");
	sprintf(set->sys_info.fw_ver, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "hw_ver");
	sprintf(set->sys_info.hw_ver, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "deviceId");
	sprintf(set->sys_info.deviceId, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "osd_set");
	set->sys_info.osd_set = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "P2P_ON_OFF");
	set->sys_info.P2P_ON_OFF = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "p2p_id");
	memcpy(set->sys_info.p2p_id, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "p2p_passwd");
	memcpy(set->sys_info.p2p_passwd, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "uid");
	sprintf(set->sys_info.uid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "dev_cam_ch");
	set->sys_info.dev_cam_ch = json_object_get_int(tmp);

	printf("set->sys_info.p2p_id: %s\n", set->sys_info.p2p_id);
	printf("set->sys_info.p2p_passwd: %s\n", set->sys_info.p2p_passwd);

	return 0;
}
static int	parseNetworkWifiAp(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "wifiap";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find wifiap\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "en_key");
	set->wifiap.en_key = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ssid");
	sprintf(set->wifiap.ssid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "pwd");
	memcpy(set->wifiap.pwd, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "stealth");
	set->wifiap.stealth = json_object_get_int(tmp);

	printf("set->wifiap.pwd: %s\n", set->wifiap.pwd);

	return 0;
}

static int parseNetworkWifilist(app_set_t* const set, json_object* rootObj)
{
    int i = 0 ;
	size_t dec_size = 0;
	const char* STR_FIELD = "wifilist";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) 
	{
        if( type != json_type_array) 
		{
		    printf("JSON Parsing Error --- Cannot find wifilist\n");
		    return -1;
		}
		else 
		{
			for(i = 0 ; i < json_object_array_length(jobj); i++)
			{
                json_object* wifiInfo = json_object_array_get_idx(jobj, i) ;
			    type = json_object_get_type(wifiInfo) ;
			    if(type == json_type_object)
				{
					json_object* tmp = json_object_object_get(wifiInfo, "en_key") ;
			        set->wifilist[i].en_key = json_object_get_int(tmp) ;
					tmp = json_object_object_get(wifiInfo, "ssid") ;
			        sprintf(set->wifilist[i].ssid, "%s", json_object_get_string(tmp)) ;
					tmp = json_object_object_get(wifiInfo, "pwd");
					memcpy(set->wifilist[i].pwd, js_base64_decode_str(tmp, &dec_size), dec_size);
					tmp = json_object_object_get(wifiInfo, "stealth");
					set->wifilist[i].stealth = json_object_get_int(tmp);
				}
            }
		}
	}
	return 0;
}

static int	parseNetworkFtp(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "ftp_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find ftp_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "port");
	set->ftp_info.port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ipaddr");
	sprintf(set->ftp_info.ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "id");
	memcpy(set->ftp_info.id, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "pwd");
	memcpy(set->ftp_info.pwd, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "file_type"); // 0:all, 1:event only
	set->ftp_info.file_type = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ON_OFF");
	set->ftp_info.ON_OFF = json_object_get_int(tmp);

	printf("set->ftp_info.id: %s\n", set->ftp_info.id);
	printf("set->ftp_info.pwd: %s\n", set->ftp_info.pwd);

	return 0;
}

static int	parseNetworkFota(app_set_t* const set, json_object* rootObj)
{
	size_t dec_size = 0;
	const char* STR_FIELD = "fota_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find fota_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "ON_OFF");
	set->fota_info.ON_OFF = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "type"); // 0:ftp . //ftps
	set->fota_info.type = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "port");
	set->fota_info.port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "svr_info");
	set->fota_info.svr_info = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ipaddr");
	sprintf(set->fota_info.ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "id");
	memcpy(set->fota_info.id,  js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "pwd");
	memcpy(set->fota_info.pwd, js_base64_decode_str(tmp, &dec_size), dec_size);
	tmp = json_object_object_get(jobj, "confname");
	sprintf(set->fota_info.confname, "%s", json_object_get_string(tmp));

	printf("set->fota_info.id: %s\n", set->fota_info.id);
	printf("set->fota_info.pwd: %s\n", set->fota_info.pwd);

	return 0;
}

static int	parseNetworkSrv(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "srv_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find srv_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "port");
	set->srv_info.port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ipaddr");
	sprintf(set->srv_info.ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ON_OFF");
	set->srv_info.ON_OFF = json_object_get_int(tmp);

	return 0;
}
static int	parseNetworkDev(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "net_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find net_info\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "type");
	set->net_info.type = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "wlan_ipaddr");
	sprintf(set->net_info.wlan_ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "wlan_netmask");
	sprintf(set->net_info.wlan_netmask, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "wlan_gateway");
	sprintf(set->net_info.wlan_gateway, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "eth_ipaddr");
	sprintf(set->net_info.eth_ipaddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "eth_netmask");
	sprintf(set->net_info.eth_netmask, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "eth_gateway");
	sprintf(set->net_info.eth_gateway, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "dns_server1");
	sprintf(set->net_info.dns_server1, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "dns_server2");
	sprintf(set->net_info.dns_server2, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "http_port");
	set->net_info.http_port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "rtsp_port");
	set->net_info.rtsp_port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "onvif_port");
	set->net_info.onvif_port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "rtsp_name");
	sprintf(set->net_info.rtsp_name, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "wtype");
	set->net_info.wtype = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "http_enable");
	set->net_info.http_enable = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "rtsp_enable");
	set->net_info.rtsp_enable = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "enable_onvif");
	set->net_info.enable_onvif = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "dnsFromDHCP");
	set->net_info.dnsFromDHCP= json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ntpFromDHCP");
	set->net_info.ntpFromDHCP= json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "hostnameFromDHCP");
	set->net_info.hostnameFromDHCP= json_object_get_int(tmp);

	// https setup -- start
	tmp = json_object_object_get(jobj, "https_enable");
	set->net_info.https_enable = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "https_port");
	set->net_info.https_port = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "https_mode");
	set->net_info.https_mode = json_object_get_int(tmp);
	tmp = json_object_object_get(jobj, "ssc_C");
	if(tmp) sprintf(set->net_info.ssc_C, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_ST");
	if(tmp) sprintf(set->net_info.ssc_ST, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_L");
	if(tmp) sprintf(set->net_info.ssc_L, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_O");
	if(tmp) sprintf(set->net_info.ssc_O, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_OU");
	if(tmp) sprintf(set->net_info.ssc_OU, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_CN");
	if(tmp) sprintf(set->net_info.ssc_CN, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ssc_Email");
	if(tmp) sprintf(set->net_info.ssc_Email, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "cert_name");
	if(tmp) sprintf(set->net_info.cert_name, "%s", json_object_get_string(tmp));
	// https setup -- end

	return 0;
}

static int	parseWatchDog(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "wd";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find wd\n");
		return -1;
	}
	json_object* tmp = json_object_object_get(jobj, "gsn");
	set->wd.gsn = json_object_get_int(tmp);

	return 0;
}

static int	parseChInfo(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "ch";
	int i=0, type;
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	if( json_object_get_type(jobj) != json_type_array) {
		printf("JSON Parsing Error --- Cannot find ch array\n");
		return -1;
	}

	for(i=0;i<json_object_array_length(jobj);i++){
		json_object* chinfo = json_object_array_get_idx(jobj, i);
		type = json_object_get_type(chinfo); // must be json_object
		if(type == json_type_object){
			json_object* tmp = json_object_object_get(chinfo, "ch");
			int n = json_object_get_int(tmp);

			// TODO: 디버깅 끝나면 활성화할것
			//if( n < TOT_CH_INFO && n >= 0)
			{
				tmp = json_object_object_get(chinfo, "res");
				set->ch[n].resol = json_object_get_int(tmp);
				tmp = json_object_object_get(chinfo, "fps");
				set->ch[n].framerate = json_object_get_int(tmp);
				tmp = json_object_object_get(chinfo, "quality");
				set->ch[n].quality = json_object_get_int(tmp);
				tmp = json_object_object_get(chinfo, "rc");
				set->ch[n].rate_ctrl = json_object_get_int(tmp);
				tmp = json_object_object_get(chinfo, "motion");
				set->ch[n].motion = json_object_get_int(tmp);
				tmp = json_object_object_get(chinfo, "gop");
				set->ch[n].gop = json_object_get_int(tmp);
			}
		}
	}

	return 0;
}

static int	parseModel(app_set_t* const set, json_object* rootObj)
{
	const char* STR_FIELD = "model";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	printf("JSON Parse Model:%s\n", json_object_get_string(jobj));
	return 0;
}

int js_read_settings(app_set_t* const set, const char* fname)
{
	int ret = EFAIL;
	json_object *rootObject = json_object_from_file(fname);
	if(!rootObject) {
		printf("load JSON data from %s failed.\n", fname);
		return ret;
	}

	printf("Read JSON Data\n");
	printf("%s", json_object_to_json_string(rootObject));
    printf("\n");

	// 0. read model
	ret = parseModel(set, rootObject);

	// 1. read ch info
	parseChInfo(set, rootObject);

	// 2. read watch dog
	parseWatchDog(set, rootObject);

	// 3. read network dev
	parseNetworkDev(set, rootObject);

	// 4. read network srv
	parseNetworkSrv(set, rootObject);

	// 5. read network ftp 
	parseNetworkFtp(set, rootObject);

	// 6. read network fota 
	parseNetworkFota(set, rootObject);

	// 7. read network wifiap
	parseNetworkWifiAp(set, rootObject);

	// 8. read wifilist
	parseNetworkWifilist(set, rootObject) ;

	// 9. read system info
	parseSystemInfo(set, rootObject);

	// 10. read record info
	parseRecInfo(set, rootObject);

	// 11. read ddns info
	parseDdnsInfo(set, rootObject);

	// 12. read time info
	parseTimeInfo(set, rootObject);

	// 13. read account info 
	parseAccountInfo(set, rootObject);

    // 14. multap info
	parseMultiapInfo(set, rootObject);

	// 15. read voip info
	
	parseVoipInfo(set, rootObject);

	// 16. read rtmpserver info
	
	parseRtmpInfo(set, rootObject);

	// 17. stream info
	parseStmInfo(set, rootObject);

	// 18. sslvpn info
	parseSSLvpnInfo(set, rootObject);


	// finish
	json_object_put(rootObject);

	return 0;
}

int js_write_settings(const app_set_t* const set, const char* fname)
{
	int i, ret;
	size_t enc_size = 0;
	
	json_object *rootObject;
	json_object* chinfo_array;

	rootObject = json_object_new_object();
	json_object_object_add(rootObject, "model", json_object_new_string(MODEL_NAME));

	// 1. save channel information
	json_object* chinfo[MODEL_CH_NUM + 1];
	chinfo_array = json_object_new_array();
	for(i = 0; i < MODEL_CH_NUM + 1 ; i++){
		chinfo[i] = json_object_new_object();
		json_object_object_add(chinfo[i], "ch",      json_object_new_int(i));
		json_object_object_add(chinfo[i], "res",     json_object_new_int(set->ch[i].resol));
		json_object_object_add(chinfo[i], "fps",     json_object_new_int(set->ch[i].framerate));
		json_object_object_add(chinfo[i], "quality", json_object_new_int(set->ch[i].quality));
		json_object_object_add(chinfo[i], "rc",      json_object_new_int(set->ch[i].rate_ctrl));
		json_object_object_add(chinfo[i], "motion",  json_object_new_int(set->ch[i].motion));
		json_object_object_add(chinfo[i], "gop",     json_object_new_int(set->ch[i].gop));

		json_object_array_add(chinfo_array, chinfo[i]);
	}
	json_object_object_add(rootObject, "ch", chinfo_array);

	// 2. watch dog 
	json_object* watchdog = json_object_new_object();
	json_object_object_add(watchdog, "gsn", json_object_new_int(set->wd.gsn));
	json_object_object_add(rootObject, "wd", watchdog);

	// 3. network_dev
	json_object* network_dev = json_object_new_object();
	json_object_object_add(network_dev, "type", json_object_new_int(set->net_info.type));
	json_object_object_add(network_dev, "wlan_ipaddr", json_object_new_string(set->net_info.wlan_ipaddr));
	json_object_object_add(network_dev, "wlan_netmask", json_object_new_string(set->net_info.wlan_netmask));
	json_object_object_add(network_dev, "wlan_gateway", json_object_new_string(set->net_info.wlan_gateway));
	json_object_object_add(network_dev, "eth_ipaddr", json_object_new_string(set->net_info.eth_ipaddr));
	json_object_object_add(network_dev, "eth_netmask", json_object_new_string(set->net_info.eth_netmask));
	json_object_object_add(network_dev, "eth_gateway", json_object_new_string(set->net_info.eth_gateway));
	json_object_object_add(network_dev, "dns_server1", json_object_new_string(set->net_info.dns_server1));
	json_object_object_add(network_dev, "dns_server2", json_object_new_string(set->net_info.dns_server2));
	json_object_object_add(network_dev, "http_port", json_object_new_int(set->net_info.http_port));
	json_object_object_add(network_dev, "rtsp_port", json_object_new_int(set->net_info.rtsp_port));
	json_object_object_add(network_dev, "onvif_port", json_object_new_int(set->net_info.onvif_port));
	json_object_object_add(network_dev, "rtsp_name", json_object_new_string(set->net_info.rtsp_name));
	json_object_object_add(network_dev, "wtype", json_object_new_int(set->net_info.wtype));
	json_object_object_add(network_dev, "http_enable", json_object_new_int(set->net_info.http_enable));
	json_object_object_add(network_dev, "rtsp_enable", json_object_new_int(set->net_info.rtsp_enable));
	json_object_object_add(network_dev, "enable_onvif", json_object_new_int(set->net_info.enable_onvif));
	json_object_object_add(network_dev, "dnsFromDHCP", json_object_new_int(set->net_info.dnsFromDHCP));
	json_object_object_add(network_dev, "ntpFromDHCP", json_object_new_int(set->net_info.ntpFromDHCP));
	json_object_object_add(network_dev, "hostnameFromDHCP", json_object_new_int(set->net_info.hostnameFromDHCP));

	// https setup -- start
	json_object_object_add(network_dev, "https_enable", json_object_new_int   (set->net_info.https_enable));
	json_object_object_add(network_dev, "https_port",   json_object_new_int   (set->net_info.https_port));
	json_object_object_add(network_dev, "https_mode",   json_object_new_int   (set->net_info.https_mode));
	json_object_object_add(network_dev, "ssc_C",        json_object_new_string(set->net_info.ssc_C));
	json_object_object_add(network_dev, "ssc_ST",       json_object_new_string(set->net_info.ssc_ST));
	json_object_object_add(network_dev, "ssc_L",        json_object_new_string(set->net_info.ssc_L));
	json_object_object_add(network_dev, "ssc_O",        json_object_new_string(set->net_info.ssc_O));
	json_object_object_add(network_dev, "ssc_OU",       json_object_new_string(set->net_info.ssc_OU));
	json_object_object_add(network_dev, "ssc_CN",       json_object_new_string(set->net_info.ssc_CN));
	json_object_object_add(network_dev, "ssc_Email",    json_object_new_string(set->net_info.ssc_Email));
	json_object_object_add(network_dev, "cert_name",    json_object_new_string(set->net_info.cert_name));
	// https setup -- end

	json_object_object_add(rootObject, "net_info", network_dev);

	// 4. server network information
	json_object* network_srv = json_object_new_object();
	json_object_object_add(network_srv, "port",   json_object_new_int(set->srv_info.port));
	json_object_object_add(network_srv, "ipaddr", json_object_new_string(set->srv_info.ipaddr));
	json_object_object_add(network_srv, "ON_OFF", json_object_new_int(set->srv_info.ON_OFF));
	json_object_object_add(rootObject,  "srv_info", network_srv);

	// 5. ftp server information
	json_object* network_ftp = json_object_new_object();
	json_object_object_add(network_ftp, "port",   json_object_new_int(set->ftp_info.port));
	json_object_object_add(network_ftp, "ipaddr", json_object_new_string(set->ftp_info.ipaddr));
	json_object_object_add(network_ftp, "id",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->ftp_info.id, MAX_CHAR_16, &enc_size)));
	json_object_object_add(network_ftp, "pwd",    json_object_new_string((const char*)base64_encode((const unsigned char*)set->ftp_info.pwd, MAX_CHAR_16, &enc_size)));
	json_object_object_add(network_ftp, "ON_OFF",    json_object_new_int(set->ftp_info.ON_OFF));
	json_object_object_add(network_ftp, "file_type", json_object_new_int(set->ftp_info.file_type)); // 0:all, 1:event
	json_object_object_add(rootObject,  "ftp_info", network_ftp);

	// 5. fota server information
	json_object* network_fota = json_object_new_object();
	json_object_object_add(network_fota, "ON_OFF", json_object_new_int(set->fota_info.ON_OFF));
	json_object_object_add(network_fota, "type",   json_object_new_int(set->fota_info.type));
	json_object_object_add(network_fota, "port",   json_object_new_int(set->fota_info.port));
	json_object_object_add(network_fota, "svr_info",   json_object_new_int(set->fota_info.svr_info));
	json_object_object_add(network_fota, "ipaddr",  json_object_new_string(set->fota_info.ipaddr));
	json_object_object_add(network_fota, "id",      json_object_new_string((const char*)base64_encode((const unsigned char*)set->fota_info.id, MAX_CHAR_16, &enc_size)));
	json_object_object_add(network_fota, "pwd",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->fota_info.pwd, MAX_CHAR_16, &enc_size)));
	json_object_object_add(network_fota, "confname",json_object_new_string(set->fota_info.confname));
	json_object_object_add(rootObject,  "fota_info", network_fota);

	// 6. wifi ap information
	
	json_object* wifiap = json_object_new_object();
	json_object_object_add(wifiap, "en_key",  json_object_new_int(set->wifiap.en_key));
	json_object_object_add(wifiap, "ssid",    json_object_new_string(set->wifiap.ssid));
	json_object_object_add(wifiap, "pwd",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->wifiap.pwd, MAX_CHAR_64, &enc_size)));
	json_object_object_add(wifiap, "stealth", json_object_new_int(set->wifiap.stealth));
	json_object_object_add(rootObject,  "wifiap", wifiap);

    // 7. wifi list information 

    json_object* wifiInfo[4] ;
	json_object* wifilist = json_object_new_array();
    
	for(i = 0 ; i < 4 ; i++)
	{
        wifiInfo[i] = json_object_new_object() ;

	    json_object_object_add(wifiInfo[i], "en_key",  json_object_new_int(set->wifilist[i].en_key));
	    json_object_object_add(wifiInfo[i], "ssid",    json_object_new_string(set->wifilist[i].ssid));
	    json_object_object_add(wifiInfo[i], "pwd",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->wifilist[i].pwd, MAX_CHAR_64, &enc_size)));
	    json_object_object_add(wifiInfo[i], "stealth", json_object_new_int(set->wifilist[i].stealth));

		json_object_array_add(wifilist, wifiInfo[i]);
	}
	json_object_object_add(rootObject, "wifilist", wifilist) ;

	// 8. system  information
	json_object* sys_info = json_object_new_object();
	json_object_object_add(sys_info, "fw_ver",     json_object_new_string(set->sys_info.fw_ver));
	json_object_object_add(sys_info, "hw_ver",     json_object_new_string(set->sys_info.hw_ver));
	json_object_object_add(sys_info, "deviceId",   json_object_new_string(set->sys_info.deviceId));
	json_object_object_add(sys_info, "osd_set",    json_object_new_int(set->sys_info.osd_set));
	json_object_object_add(sys_info, "P2P_ON_OFF", json_object_new_int(set->sys_info.P2P_ON_OFF));
	json_object_object_add(sys_info, "p2p_id",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->sys_info.p2p_id, MAX_CHAR_32, &enc_size)));
	json_object_object_add(sys_info, "p2p_passwd", json_object_new_string((const char*)base64_encode((const unsigned char*)set->sys_info.p2p_passwd, MAX_CHAR_32, &enc_size)));
	json_object_object_add(sys_info, "uid",        json_object_new_string(set->sys_info.uid));
	json_object_object_add(sys_info, "dev_cam_ch", json_object_new_int(set->sys_info.dev_cam_ch));
	json_object_object_add(rootObject,  "sys_info", sys_info);

	// 9. record information
	json_object* rec_info = json_object_new_object();
	json_object_object_add(rec_info, "period_idx", json_object_new_int(set->rec_info.period_idx));
	json_object_object_add(rec_info, "overwrite",  json_object_new_int(set->rec_info.overwrite));
	json_object_object_add(rec_info, "pre_rec",    json_object_new_int(set->rec_info.pre_rec));
	json_object_object_add(rec_info, "auto_rec",   json_object_new_int(set->rec_info.auto_rec));
	json_object_object_add(rec_info, "audio_rec",  json_object_new_int(set->rec_info.audio_rec));
	json_object_object_add(rootObject,  "rec_info", rec_info);

	// 10. ddns information
	json_object* ddns_info = json_object_new_object();
	json_object_object_add(ddns_info, "ON_OFF",     json_object_new_int(set->ddns_info.ON_OFF));
	json_object_object_add(ddns_info, "userId",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->ddns_info.userId, MAX_CHAR_32, &enc_size)));
	json_object_object_add(ddns_info, "passwd",     json_object_new_string((const char*)base64_encode((const unsigned char*)set->ddns_info.passwd, MAX_CHAR_32, &enc_size)));
	json_object_object_add(ddns_info, "serveraddr", json_object_new_string(set->ddns_info.serveraddr));
	json_object_object_add(ddns_info, "hostname",   json_object_new_string(set->ddns_info.hostname));
	json_object_object_add(ddns_info, "interval",   json_object_new_int(set->ddns_info.interval));
	json_object_object_add(rootObject,  "ddns_info", ddns_info);

	// 11. time information
	json_object* time_info = json_object_new_object();
	json_object_object_add(time_info, "time_zone",       json_object_new_int(set->time_info.time_zone));
	json_object_object_add(time_info, "time_server",     json_object_new_string(set->time_info.time_server));
	json_object_object_add(time_info, "daylight_saving", json_object_new_int(set->time_info.daylight_saving));
	json_object_object_add(time_info, "timesync_type",   json_object_new_int(set->time_info.timesync_type));
	json_object_object_add(time_info, "time_zone_abbr",  json_object_new_string(set->time_info.time_zone_abbr));
	json_object_object_add(rootObject,  "time_info", time_info);

	// 12. account information
	json_object* account_info = json_object_new_object();
	json_object_object_add(account_info, "ON_OFF",       json_object_new_int(set->account_info.ON_OFF));
	json_object_object_add(account_info, "enctype",       json_object_new_int(set->account_info.enctype));
	json_object_object_add(account_info, "rtsp_userid", json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.rtsp_userid, MAX_CHAR_32, &enc_size)));
	json_object_object_add(account_info, "rtsp_passwd", json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.rtsp_passwd, MAX_CHAR_32, &enc_size)));
	json_object* webuser = json_object_new_object();
	json_object_object_add(webuser, "authtype", json_object_new_int(set->account_info.webuser.authtype));
	json_object_object_add(webuser, "id",       json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.webuser.id, MAX_CHAR_32, &enc_size)));
	json_object_object_add(webuser, "pw",       json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.webuser.pw, MAX_CHAR_32, &enc_size)));
	json_object_object_add(webuser, "lv",       json_object_new_int(set->account_info.webuser.lv));
	json_object_object_add(account_info,  "webuser", webuser);
	json_object* onvif = json_object_new_object();
	json_object_object_add(onvif, "id",       json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.onvif.id, MAX_CHAR_32, &enc_size)));
	json_object_object_add(onvif, "pw",       json_object_new_string((const char*)base64_encode((const unsigned char*)set->account_info.onvif.pw, MAX_CHAR_32, &enc_size)));
	json_object_object_add(onvif, "lv",       json_object_new_int(set->account_info.onvif.lv));
	json_object_object_add(account_info,  "onvif", onvif);
	json_object_object_add(rootObject,  "account_info", account_info);


	// 13. account information
	json_object* multi_ap = json_object_new_object();
	json_object_object_add(multi_ap, "ON_OFF",       json_object_new_int(set->multi_ap.ON_OFF));
    json_object_object_add(rootObject,  "multi_ap", multi_ap);


	// 14. voip information
	json_object* voip_info = json_object_new_object();
	json_object_object_add(voip_info, "ipaddr",   json_object_new_string(set->voip.ipaddr));
	json_object_object_add(voip_info, "port",     json_object_new_int(set->voip.port));
	json_object_object_add(voip_info, "userid",   json_object_new_string(set->voip.userid));
	json_object_object_add(voip_info, "passwd",   json_object_new_string(set->voip.passwd));
	json_object_object_add(voip_info, "peerid",   json_object_new_string(set->voip.peerid));
	json_object_object_add(voip_info, "use_stun", json_object_new_int(set->voip.use_stun));
	json_object_object_add(voip_info, "ON_OFF",   json_object_new_int(set->voip.ON_OFF));
	json_object_object_add(rootObject, "voip_info", voip_info);

	// 15. rtmp information
	json_object* rtmp_info = json_object_new_object();
	json_object_object_add(rtmp_info, "ON_OFF",   json_object_new_int(set->rtmp.ON_OFF));
	json_object_object_add(rtmp_info, "USE_URL",   json_object_new_int(set->rtmp.USE_URL));
	json_object_object_add(rtmp_info, "ipaddr",   json_object_new_string(set->rtmp.ipaddr));
	json_object_object_add(rtmp_info, "FULL_URL",   json_object_new_string(set->rtmp.FULL_URL));
	json_object_object_add(rtmp_info, "port",     json_object_new_int(set->rtmp.port));	
//	json_object_object_add(voip_info, "userid",   json_object_new_string(set->rtmp.userid));
//	json_object_object_add(voip_info, "passwd",   json_object_new_string(set->rtmp.passwd));
	json_object_object_add(rootObject, "rtmp_info", rtmp_info);

	// 16. streaming information
	json_object* stm_info = json_object_new_object();
	json_object_object_add(stm_info, "enable_audio",   json_object_new_int(set->stm_info.enable_audio));
	json_object_object_add(rootObject, "stm_info", stm_info);

	// 17. sslvpn information
	json_object* sslvpn_info = json_object_new_object();
	json_object_object_add(sslvpn_info, "ON_OFF",   json_object_new_int(set->sslvpn_info.ON_OFF));
	json_object_object_add(sslvpn_info, "vendor",   json_object_new_int(set->sslvpn_info.vendor));
	json_object_object_add(sslvpn_info, "vpn_id",   json_object_new_string(set->sslvpn_info.vpn_id));
	json_object_object_add(sslvpn_info, "heartbeat_interval",     json_object_new_int(set->sslvpn_info.heartbeat_interval));	
	json_object_object_add(sslvpn_info, "heartbeat_threshold",     json_object_new_int(set->sslvpn_info.heartbeat_threshold));	
	json_object_object_add(sslvpn_info, "protocol",     json_object_new_int(set->sslvpn_info.protocol));	
	json_object_object_add(sslvpn_info, "port",     json_object_new_int(set->sslvpn_info.port));	
	json_object_object_add(sslvpn_info, "queue",     json_object_new_int(set->sslvpn_info.queue));	
	json_object_object_add(sslvpn_info, "key",   json_object_new_string(set->sslvpn_info.key));
	json_object_object_add(sslvpn_info, "encrypt_type",     json_object_new_int(set->sslvpn_info.encrypt_type));	
	json_object_object_add(sslvpn_info, "ipaddr",   json_object_new_string(set->sslvpn_info.ipaddr));
	json_object_object_add(sslvpn_info, "NI",     json_object_new_int(set->sslvpn_info.NI));	
	json_object_object_add(rootObject, "sslvpn_info", sslvpn_info);


	// Finish
	printf("Write JSON Data\n");
	printf("%s\n", json_object_to_json_string(rootObject));
	printf("\n");

	ret = json_object_to_file_ext((char*)fname, rootObject, JSON_C_TO_STRING_PLAIN);
	if(ret == -1){
		printf("Failed save JSON File.\n");
	}

	for(i = 0; i < MODEL_CH_NUM + 1 ; i++) 
		json_object_put(chinfo[i]);
	json_object_put(chinfo_array); 
	json_object_put(watchdog);
	json_object_put(network_dev);
	json_object_put(network_srv);
	json_object_put(network_ftp);
	json_object_put(network_fota);
	json_object_put(wifiap);
	json_object_put(wifilist);
	json_object_put(sys_info);
	json_object_put(rec_info);
	json_object_put(ddns_info);
	json_object_put(time_info);
	json_object_put(webuser);
	json_object_put(onvif);
	json_object_put(account_info);
	json_object_put(multi_ap);

	json_object_put(voip_info);
	json_object_put(rtmp_info);

	json_object_put(stm_info);
    json_object_put(sslvpn_info) ;

	json_object_put(rootObject);

    return ret;
}
