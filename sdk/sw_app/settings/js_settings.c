#include <stdio.h>
#include <json-c/json.h> 

#include "js_settings.h"
#include "board_config.h"

#if SYS_CONFIG_VOIP
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
	tmp = json_object_object_get(jobj, "private_network_only");
	set->voip.private_network_only = json_object_get_int(tmp);

	return 0;
}
#endif
static int	parseAccountInfo(app_set_t* const set, json_object* rootObj)
{
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
	sprintf(set->account_info.rtsp_userid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "rtsp_passwd");
	sprintf(set->account_info.rtsp_passwd, "%s", json_object_get_string(tmp));

	json_object* webuser = json_object_object_get(jobj, "webuser");
	type = json_object_get_type(webuser);
	if( type == json_type_object) { // must be jobject
		tmp = json_object_object_get(webuser, "authtype");
		set->account_info.webuser.authtype = json_object_get_int(tmp);
		tmp = json_object_object_get(webuser, "id");
		sprintf(set->account_info.webuser.id, "%s", json_object_get_string(tmp));
		tmp = json_object_object_get(webuser, "pw");
		sprintf(set->account_info.webuser.pw, "%s", json_object_get_string(tmp));
		tmp = json_object_object_get(webuser, "lv");
		set->account_info.webuser.lv = json_object_get_int(tmp);
	}

	json_object* onvif = json_object_object_get(jobj, "onvif");
	type = json_object_get_type(onvif);
	if( type == json_type_object) { // must be jobject
		tmp = json_object_object_get(onvif, "id");
		sprintf(set->account_info.onvif.id, "%s", json_object_get_string(tmp));
		tmp = json_object_object_get(onvif, "pw");
		sprintf(set->account_info.onvif.pw, "%s", json_object_get_string(tmp));
		tmp = json_object_object_get(onvif, "lv");
		set->account_info.onvif.lv = json_object_get_int(tmp);
	}

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
	sprintf(set->ddns_info.userId, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "passwd");
	sprintf(set->ddns_info.passwd, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "serveraddr");
	sprintf(set->ddns_info.serveraddr, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "hostname");
	sprintf(set->ddns_info.hostname, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "interval");
	set->ddns_info.interval = json_object_get_int(tmp);

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
	const char* STR_FIELD = "sys_info";
	json_object* jobj = json_object_object_get(rootObj, STR_FIELD);
	int type = json_object_get_type(jobj); // must be json_object
	if( type != json_type_object) {
		printf("JSON Parsing Error --- Cannot find wifiap\n");
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
	sprintf(set->sys_info.p2p_id, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "p2p_passwd");
	sprintf(set->sys_info.p2p_passwd, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "uid");
	sprintf(set->sys_info.uid, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "dev_cam_ch");
	set->sys_info.dev_cam_ch = json_object_get_int(tmp);

	return 0;
}
static int	parseNetworkWifiAp(app_set_t* const set, json_object* rootObj)
{
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
	sprintf(set->wifiap.pwd, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "stealth");
	set->wifiap.stealth = json_object_get_int(tmp);

	return 0;
}
static int	parseNetworkFtp(app_set_t* const set, json_object* rootObj)
{
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
	sprintf(set->ftp_info.id, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "pwd");
	sprintf(set->ftp_info.pwd, "%s", json_object_get_string(tmp));
	tmp = json_object_object_get(jobj, "ON_OFF");
	set->ftp_info.ON_OFF = json_object_get_int(tmp);

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
	tmp = json_object_object_get(jobj, "https_port");
	set->net_info.https_port = json_object_get_int(tmp);
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
	tmp = json_object_object_get(jobj, "https_enable");
	set->net_info.https_enable = json_object_get_int(tmp);
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

	// 6. read network wifiap
	parseNetworkWifiAp(set, rootObject);

	// 7. read system info
	parseSystemInfo(set, rootObject);

	// 8. read record info
	parseRecInfo(set, rootObject);

	// 9. read ddns info
	parseDdnsInfo(set, rootObject);

	// 10. read time info
	parseTimeInfo(set, rootObject);

	// 11. read account info 
	parseAccountInfo(set, rootObject);

	// 12. read voip info
#if SYS_CONFIG_VOIP
	parseVoipInfo(set, rootObject);
#endif

	// finish
	json_object_put(rootObject);

	return 0;
}

int js_write_settings(const app_set_t* const set, const char* fname)
{
	int i, ret;

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
	json_object_object_add(network_dev, "https_port", json_object_new_int(set->net_info.https_port));
	json_object_object_add(network_dev, "rtsp_port", json_object_new_int(set->net_info.rtsp_port));
	json_object_object_add(network_dev, "onvif_port", json_object_new_int(set->net_info.onvif_port));
	json_object_object_add(network_dev, "rtsp_name", json_object_new_string(set->net_info.rtsp_name));
	json_object_object_add(network_dev, "wtype", json_object_new_int(set->net_info.wtype));
	json_object_object_add(network_dev, "http_enable", json_object_new_int(set->net_info.http_enable));
	json_object_object_add(network_dev, "https_enable", json_object_new_int(set->net_info.https_enable));
	json_object_object_add(network_dev, "rtsp_enable", json_object_new_int(set->net_info.rtsp_enable));
	json_object_object_add(network_dev, "enable_onvif", json_object_new_int(set->net_info.enable_onvif));
	json_object_object_add(network_dev, "dnsFromDHCP", json_object_new_int(set->net_info.dnsFromDHCP));
	json_object_object_add(network_dev, "ntpFromDHCP", json_object_new_int(set->net_info.ntpFromDHCP));
	json_object_object_add(network_dev, "hostnameFromDHCP", json_object_new_int(set->net_info.hostnameFromDHCP));
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
	json_object_object_add(network_ftp, "id",     json_object_new_string(set->ftp_info.id));
	json_object_object_add(network_ftp, "pwd",    json_object_new_string(set->ftp_info.pwd));
	json_object_object_add(network_ftp, "ON_OFF", json_object_new_int(set->ftp_info.ON_OFF));
	json_object_object_add(rootObject,  "ftp_info", network_ftp);

	// 6. wifi ap information
	json_object* wifiap = json_object_new_object();
	json_object_object_add(wifiap, "en_key",  json_object_new_int(set->wifiap.en_key));
	json_object_object_add(wifiap, "ssid",    json_object_new_string(set->wifiap.ssid));
	json_object_object_add(wifiap, "pwd",     json_object_new_string(set->wifiap.pwd));
	json_object_object_add(wifiap, "stealth", json_object_new_int(set->wifiap.stealth));
	json_object_object_add(rootObject,  "wifiap", wifiap);

	// 7. system  information
	json_object* sys_info = json_object_new_object();
	json_object_object_add(sys_info, "fw_ver",     json_object_new_string(set->sys_info.fw_ver));
	json_object_object_add(sys_info, "hw_ver",     json_object_new_string(set->sys_info.hw_ver));
	json_object_object_add(sys_info, "deviceId",   json_object_new_string(set->sys_info.deviceId));
	json_object_object_add(sys_info, "osd_set",    json_object_new_int(set->sys_info.osd_set));
	json_object_object_add(sys_info, "P2P_ON_OFF", json_object_new_int(set->sys_info.P2P_ON_OFF));
	json_object_object_add(sys_info, "p2p_id",     json_object_new_string(set->sys_info.p2p_id));
	json_object_object_add(sys_info, "p2p_passwd", json_object_new_string(set->sys_info.p2p_passwd));
	json_object_object_add(sys_info, "uid",        json_object_new_string(set->sys_info.uid));
	json_object_object_add(sys_info, "dev_cam_ch", json_object_new_int(set->sys_info.dev_cam_ch));
	json_object_object_add(rootObject,  "sys_info", sys_info);

	// 8. record information
	json_object* rec_info = json_object_new_object();
	json_object_object_add(rec_info, "period_idx", json_object_new_int(set->rec_info.period_idx));
	json_object_object_add(rec_info, "overwrite",  json_object_new_int(set->rec_info.overwrite));
	json_object_object_add(rec_info, "pre_rec",    json_object_new_int(set->rec_info.pre_rec));
	json_object_object_add(rec_info, "auto_rec",   json_object_new_int(set->rec_info.auto_rec));
	json_object_object_add(rec_info, "audio_rec",  json_object_new_int(set->rec_info.audio_rec));
	json_object_object_add(rootObject,  "rec_info", rec_info);

	// 9. ddns information
	json_object* ddns_info = json_object_new_object();
	json_object_object_add(ddns_info, "ON_OFF",     json_object_new_int(set->ddns_info.ON_OFF));
	json_object_object_add(ddns_info, "userId",     json_object_new_string(set->ddns_info.userId));
	json_object_object_add(ddns_info, "passwd",     json_object_new_string(set->ddns_info.passwd));
	json_object_object_add(ddns_info, "serveraddr", json_object_new_string(set->ddns_info.serveraddr));
	json_object_object_add(ddns_info, "hostname",   json_object_new_string(set->ddns_info.hostname));
	json_object_object_add(ddns_info, "interval",   json_object_new_int(set->ddns_info.interval));
	json_object_object_add(rootObject,  "ddns_info", ddns_info);

	// 10. time information
	json_object* time_info = json_object_new_object();
	json_object_object_add(time_info, "time_zone",       json_object_new_int(set->time_info.time_zone));
	json_object_object_add(time_info, "time_server",     json_object_new_string(set->time_info.time_server));
	json_object_object_add(time_info, "daylight_saving", json_object_new_int(set->time_info.daylight_saving));
	json_object_object_add(time_info, "timesync_type",   json_object_new_int(set->time_info.timesync_type));
	json_object_object_add(time_info, "time_zone_abbr",  json_object_new_string(set->time_info.time_zone_abbr));
	json_object_object_add(rootObject,  "time_info", time_info);

	// 11. account information
	json_object* account_info = json_object_new_object();
	json_object_object_add(account_info, "ON_OFF",       json_object_new_int(set->account_info.ON_OFF));
	json_object_object_add(account_info, "enctype",       json_object_new_int(set->account_info.enctype));
	json_object_object_add(account_info, "rtsp_userid", json_object_new_string(set->account_info.rtsp_userid));
	json_object_object_add(account_info, "rtsp_passwd", json_object_new_string(set->account_info.rtsp_passwd));
	json_object* webuser = json_object_new_object();
	json_object_object_add(webuser, "authtype", json_object_new_int(set->account_info.webuser.authtype));
	json_object_object_add(webuser, "id",       json_object_new_string(set->account_info.webuser.id));
	json_object_object_add(webuser, "pw",       json_object_new_string(set->account_info.webuser.pw));
	json_object_object_add(webuser, "lv",       json_object_new_int(set->account_info.webuser.lv));
	json_object_object_add(account_info,  "webuser", webuser);
	json_object* onvif = json_object_new_object();
	json_object_object_add(onvif, "id",       json_object_new_string(set->account_info.onvif.id));
	json_object_object_add(onvif, "pw",       json_object_new_string(set->account_info.onvif.pw));
	json_object_object_add(onvif, "lv",       json_object_new_int(set->account_info.onvif.lv));
	json_object_object_add(account_info,  "onvif", onvif);
	json_object_object_add(rootObject,  "account_info", account_info);

#if SYS_CONFIG_VOIP
	// 12. voip information
	json_object* voip_info = json_object_new_object();
	json_object_object_add(voip_info, "ipaddr",   json_object_new_string(set->voip.ipaddr));
	json_object_object_add(voip_info, "port",   json_object_new_int(set->voip.port));
	json_object_object_add(voip_info, "userid",   json_object_new_string(set->voip.userid));
	json_object_object_add(voip_info, "passwd",   json_object_new_string(set->voip.passwd));
	json_object_object_add(voip_info, "peerid",   json_object_new_string(set->voip.peerid));
	json_object_object_add(voip_info, "private_network_only",   json_object_new_int(set->voip.private_network_only));
	json_object_object_add(rootObject,  "voip_info", voip_info);
#endif

	// Finish
	printf("%s\n", json_object_to_json_string(rootObject));

	ret = json_object_to_file_ext(fname, rootObject, JSON_C_TO_STRING_PLAIN);
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
	json_object_put(wifiap);
	json_object_put(sys_info);
	json_object_put(rec_info);
	json_object_put(ddns_info);
	json_object_put(time_info);
	json_object_put(webuser);
	json_object_put(onvif);
	json_object_put(account_info);
#if SYS_CONFIG_VOIP
	json_object_put(voip_info);
#endif
	json_object_put(rootObject);

    return ret;
}