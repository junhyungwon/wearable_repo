
/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_uds.c
 * @brief
 * @author  BKKIM
 * @section MODIFY history
 *     - 2019.7.19 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __USE_GNU
#include <pthread.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>		/* read */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/un.h>

#include "dev_common.h"
#include "app_comm.h"
#include "app_main.h"
#include "app_rec.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_ctrl.h"
#include "app_rtsptx.h"
#include "app_timesrv.h"
#include "app_uds.h"
#include "app_onvifserver.h"
#include "app_web.h"
#include "app_cap.h"
#include "app_p2p.h"
#include "dev_micom.h"
#include "cgi_param.h"
#include "app_encrypt.h"
#include "app_decrypt.h"
#include "app_rtmp.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define ENABLE_UDS_DEBUG 1
#if ENABLE_UDS_DEBUG
#define __D(fmt, args...) {fprintf(stderr, "[APP_UDS_DBG] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args);}
#define __D_FUNC_ENTER { __D("[UDSS] Enter >>>>>>>>>\n");}
#else
#define __D(fmt, args...) {}
#define __D_FUNC_ENTER {}
#endif//ENABLE_UDS_DEBUG
#define __E(fmt, args...) {fprintf(stderr, "[APP_UDS_ERR] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args);}
#define DBG_UDS __D
#define DBG_ENTER __D_FUNC_ENTER

#define FILE_UDS_SYSTEM "/tmp/system.socket"
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static unsigned int	g_thd_running;	    // uds thread running flag
static pthread_t  	g_tid;              // uds task id

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static int setStreamVideoQuality( int stm_res, int stm_fps, int stm_bps, int stm_gop)
{
	int ch = STM_CH_NUM;
	// STM
#if defined(NEXXONE) || defined(NEXXB_ONE)
	if(stm_fps <= DEFAULT_FPS && stm_fps > 0 && app_set->ch[ch].framerate != stm_fps)
	{
	    ctrl_vid_framerate(ch, stm_fps) ;
	}
	if(stm_bps <= MAX_STM_BITRATE && stm_bps >= MIN_STM_BITRATE && app_set->ch[ch].quality != stm_bps)
    {
	    ctrl_vid_bitrate(ch, stm_bps) ;
	}
	if(stm_gop <= DEFAULT_FPS && stm_gop > 0 && app_set->ch[ch].gop != stm_gop)
	{
	    ctrl_vid_gop_set(ch, stm_gop) ;
	}
	if(stm_res < RESOL_1080P && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
		ctrl_vid_resolution(stm_res);
	
#else
	if(stm_fps <= DEFAULT_FPS && stm_fps > 0 && app_set->ch[ch].framerate != stm_fps)
	{
	    ctrl_vid_framerate(ch, stm_fps) ;
	}
	if(stm_bps <= MAX_STM_BITRATE && stm_bps >= MIN_STM_BITRATE && app_set->ch[ch].quality != stm_bps)
    {
	    ctrl_vid_bitrate(ch, stm_bps) ;
	}
	if(stm_gop <= DEFAULT_FPS && stm_gop > 0 && app_set->ch[ch].gop != stm_gop)
	{
	    ctrl_vid_gop_set(ch, stm_gop) ;
	}
	if(stm_res < MAX_RESOL && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
	{
		ctrl_vid_resolution(stm_res);
	}

#endif
	app_set->ch[ch].gop = stm_gop; // save

	DBG_UDS("[STM] ch:%d, fps:%d, bps:%d, gop:%d resolution = %d\n", ch, stm_fps, stm_bps, stm_gop, stm_res);

	return 0;

}

static int onvif_setVideoEncoderConfiguration(int enctype, int w, int h, int kbps, int fps, int ei, int gov)
{
	if (enctype == ENC_JPEG) // JPEG
	{
		int newRes=app_set->ch[STM_CH_NUM].resol;
#if defined(NEXXONE) || defined(NEXXB_ONE)
		if (h == 480  && app_set->ch[STM_CH_NUM].resol != RESOL_480P)
			newRes = RESOL_480P;
		else if (h == 720 && app_set->ch[STM_CH_NUM].resol != RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080 && app_set->ch[STM_CH_NUM].resol == RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080 && app_set->ch[STM_CH_NUM].resol == RESOL_480P)
			newRes = RESOL_480P;
#else
		if (h == 720 && app_set->ch[STM_CH_NUM].resol != RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080 && app_set->ch[STM_CH_NUM].resol != RESOL_1080P)
			newRes = RESOL_1080P;
		else if (h == 480 && app_set->ch[STM_CH_NUM].resol != RESOL_480P)
			newRes = RESOL_480P;
#endif
		if (newRes != app_set->ch[STM_CH_NUM].resol)
			ctrl_vid_resolution(newRes);
	}
	else if (enctype == ENC_H264) // H264
	{
		int newRes=app_set->ch[STM_CH_NUM].resol;
#if defined(NEXXONE) || defined(NEXXB_ONE)
		if (h == 480  && app_set->ch[STM_CH_NUM].resol != RESOL_480P)
			newRes = RESOL_480P;
		else if(h == 720 && app_set->ch[STM_CH_NUM].resol != RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080  && app_set->ch[STM_CH_NUM].resol == RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080  && app_set->ch[STM_CH_NUM].resol == RESOL_480P)
			newRes = RESOL_480P;
#else
		if (h == 720  && app_set->ch[STM_CH_NUM].resol != RESOL_720P)
			newRes = RESOL_720P;
		else if (h == 1080  && app_set->ch[STM_CH_NUM].resol != RESOL_1080P)
			newRes = RESOL_1080P;
		else if (h == 480  && app_set->ch[STM_CH_NUM].resol != RESOL_480P)
			newRes = RESOL_480P;
#endif
		int newKbps = app_set->ch[STM_CH_NUM].quality;
		if (kbps <= 250) newKbps = 128;
		else if (kbps <= 500) newKbps = 256;
		else if (kbps <= 750) newKbps = 512;
		else if (kbps <= 1500) newKbps = 1000;
		else if (kbps <= 2500) newKbps = 2000;
		else if (kbps <= 3500) newKbps = 3000;
		else if (kbps <= 4500) newKbps = 4000;
		else if (kbps <= 5500) newKbps = 5000;
		else if (kbps <= 6500) newKbps = 6000;
		else if (kbps <= 7500) newKbps = 7000;
		else newKbps = 8000;
			
#if 0 // 이건 필요 	없을 듯... dynamic적용..
		if ( newRes  != app_set->ch[STM_CH_NUM].resol 
		  || newKbps != app_set->ch[STM_CH_NUM].quality
	      //|| gov != app_set->ch[STM_CH_NUM].gop // gov eq fps
		  || fps != app_set->ch[STM_CH_NUM].framerate)
		ctrl_full_vid_setting(STM_CH_NUM, newRes, newKbps, fps, gov);
		DBG_UDS("STM_CH_NUM=%d, newRes=%d, newKbps=%d,fps=%d, gov=%d\n",STM_CH_NUM, newRes, newKbps,fps, gov);
#else

		setStreamVideoQuality(newRes, fps, newKbps, gov);
		DBG_UDS("newRes=%d, newKbps=%d,fps=%d, gov=%d\n", newRes, newKbps,fps, gov);
#endif
	}
	else
	{
		DBG_UDS("Not supported Encoding:%d\n", enctype);
	}
	return 0;
}

static int  onvif_get_gateway(const char *devName, char *out_gw)
{
	int ret = -1;
    char line[100], *p, *c, *g, *saveptr;
	
    FILE *fp = fopen("/proc/net/route" , "r");
	if (NULL == fp) {
		perror("faile open route file:");
		return -1;
	}

    while (fgets(line, 100, fp))
    {
        p = strtok_r(line, " \t", &saveptr);
        c = strtok_r(NULL, " \t", &saveptr);
        g = strtok_r(NULL, " \t", &saveptr);

        if (p != NULL && c != NULL)
        {
			if (strcmp(c, "00000000") == 0 && strcmp(p, devName)==0)
            {
                if (g) {
                    char *p_end;
                    int gw = strtol(g, &p_end, 16);
                    
                    struct in_addr addr;
                    addr.s_addr = gw;
                    
                    strcpy(out_gw, inet_ntoa(addr));
                    ret = 0;
                }
                
                break;
            }
        }
    }

	fclose(fp);

	DBG_UDS("%s:%s\n", devName, out_gw);
	
	return ret;
}

// devName에 맞은 인터페이스에서 네트워크 정보를 가져온다.
static int getNetworkInfo(const char *devName, ONVIF_NET_INFO * out_info)
{
    int i;
	int socket_fd;
	struct ifreq *ifr;
	struct ifconf conf;
	struct ifreq ifs[8];
	int num;
	
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	conf.ifc_len = sizeof(ifs);
	conf.ifc_req = ifs;
	
	ioctl(socket_fd, SIOCGIFCONF, &conf);
	
	num = conf.ifc_len / sizeof(struct ifreq);
	ifr = conf.ifc_req;
	
	for (i=0; i<num; i++)
	{
		if (ifr->ifr_addr.sa_family != AF_INET) {
			ifr++; continue;
		}

		if (ioctl(socket_fd, SIOCGIFFLAGS, ifr) != 0) {
			ifr++; continue;
		}
		
		// not loopback interface
		if( (ifr->ifr_flags & IFF_LOOPBACK) != 0)  {
			ifr++; continue;
		}

		if(strlen(ifr->ifr_name) > 1 && 0 == strcmp(devName, ifr->ifr_name)) {

			if ((ifr->ifr_flags & IFF_UP) == 0) {
				out_info->up = 1;
			}

			// 1.get ipaddr
			struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
			strcpy(out_info->ipaddr, inet_ntoa(sin->sin_addr));

			// 2.get netmask
			if (ioctl(socket_fd, SIOCGIFNETMASK, ifr) == 0)
			{
				sin = (struct sockaddr_in *)(&ifr->ifr_netmask);
				strcpy(out_info->netmask, inet_ntoa(sin->sin_addr));
			}
			else {
				strcpy(out_info->netmask, "255.255.255.0"); // default
			}
			close(socket_fd);

			// 3. get gateway
			onvif_get_gateway(ifr->ifr_name, out_info->gateway);
			
			DBG_UDS("%s : ipaddr:%s, netmask:%s, gateway:%s\n", devName, out_info->ipaddr, out_info->netmask, out_info->gateway);

			return 0; // succeed
		}
		
		ifr++;
	}

	return -1;
}

void *thdSetFactoryDefault(void *arg)
{
	do {
		int type = *((int*)arg);
		app_setting_reset(type);
	}while(0);
	return NULL;
}
static int SetFactoryDefault(int type)
{

#if 1 // 인위적으로 client에 응답을 주지 않는다. Restarting 메시지 표시 때문에...
	app_setting_reset(type);
	sleep(10);
#else // 바로 응답을 주는 경우....
	static pthread_t tid_factoryset; // restart onvif
	int status = pthread_create(&tid_factoryset, NULL, thdSetFactoryDefault, (void *)&type);
#endif
	return 0;
}

void *thdRestartOnvifServer(void *arg)
{
	// 쓰레드 미사용하고, kill하면,  socket이 물려 있어서 정상종료되지 않는 경우가 있다, 
	// 그래서 이렇게 하긴 했는데 더 좋은 방법이 있을것 같은데 뭘까나..
	
	int cnt=0;
	char buff = 'e';
	int nCS = *((int *)arg);
	do {
		usleep(10000);
		int n2 = send(nCS, &buff, 1, MSG_NOSIGNAL);
		if(n2 == -1)
			break;
		if(cnt++>3)
			break;
	}while(1);

	// 이걸로 onvifserver를 kill함, 그래서 socket이 강제로 끊어짐.
	app_onvifserver_restart();
	
	return NULL;
}	

/* 이 함수의 trigger는 onvifserver process에서 보내오는 메시지이다. 
 * onvif user가 변경되었으니, 사용자 계정이 변경되었다는 신호이다.
 * app_fitt.out이 onvifserver를 실행시키는 주체이므로, 
 * 이 user 변경 신호를 받으면 onvifserver를 restart(kill and start)를 시키게된다.
 * UDS를 요청한 주체를 kill하기 때문에 소켓이 연결된 상태로 끊어지기(Kill) 때문에,
 * Normally socket close가 일어나지 않은 상태로 onvifserver가 재시작이 될 수 있다.
 * 그래서, thread로 분리한 것이며, NOSIGNAL을 보내 PIPE에러를 회피한다.
 */
static int SetOnvifUser(T_ONVIF_USER *pUser, int cs)
{
	if(strcmp(pUser->UserName, "admin")==0) {
		strcpy(app_set->account_info.onvif.pw, pUser->Password);
		app_set->account_info.onvif.lv = pUser->UserLevel;

		int nCS = cs;
		static pthread_t tid_restart_onvif; // restart onvif
		int status = pthread_create(&tid_restart_onvif, NULL, thdRestartOnvifServer, (void *)&nCS);
		if (status < 0) {
			; /* TODO */
		}
		pthread_setname_np(tid_restart_onvif, __FILENAME__);

		return 0;
	}

	return -1;
}

static int onvif_setHostname(int fromDHCP, char *hname)
{
	DBG_UDS("fromDHCP:%d, hostname:%s\n", fromDHCP, hname);

	if(app_set->net_info.hostnameFromDHCP != fromDHCP) {
		app_set->net_info.hostnameFromDHCP = fromDHCP;
		// TODO: 실제 동작 구현은 어떻게...
	}

	if(!fromDHCP){

		char strcmd[128]={0};
		sprintf(strcmd, "/bin/hostname %s", hname);

		//execl("/bin/hostname", "/bin/hostname", hname, NULL); // 이거 사용하면 則死.
		//system(wbuf);

		FILE *fp = popen(strcmd, "r");
		if(fp == NULL) { perror("failed open:"); return -1;}
		pclose(fp);
	}

	return 0;
}

int setNetworkProtocols(onvif_NetworkProtocol* np){

	if(app_set->net_info.http_enable != np->HTTPEnabled) {
		app_set->net_info.http_enable = np->HTTPEnabled;
		if(np->HTTPEnabled)
		{
			if(app_set->net_info.http_port != np->HTTPPort[0]){
				app_set->net_info.http_port = np->HTTPPort[0];

				//app_web_restart_server()
			}
		}
		else {
			//app_web_stop_server();
		}
	}

	if(app_set->net_info.https_enable != np->HTTPSEnabled) {
		app_set->net_info.https_enable = np->HTTPSEnabled;
		if(np->HTTPSEnabled)
		{
			if(app_set->net_info.https_port != np->HTTPSPort[0]){
				app_set->net_info.https_port = np->HTTPSPort[0];

				//app_web_restart_server()
			}
		}
		else {
			//app_web_stop_server();
		}
	}

	if(app_set->net_info.rtsp_enable != np->RTSPEnabled) {
		app_set->net_info.rtsp_enable = np->RTSPEnabled;

		if(np->RTSPEnabled)
		{
			if(app_set->net_info.rtsp_port != np->RTSPPort[0]){
				app_set->net_info.rtsp_port = np->RTSPPort[0];

				//app_rtsptx_stop_start() ; 
			}
		}
		else {
			//app_rtsptx_stop() ; 
		}
	}

	return 0;
}

static int setNetworkConfiguration2(T_CGI_NETWORK_CONFIG2 *t)
{
	int isChanged=0, i;

	if(t->wifi_ap_multi != app_set->multi_ap.ON_OFF) {
		app_set->multi_ap.ON_OFF = (short)t->wifi_ap_multi;
		DBG_UDS("app_set->multi_ap.ON_OFF=%d\n", app_set->multi_ap.ON_OFF);
		isChanged++;
	}

	// wireless
	if(t->wireless.addr_type != app_set->net_info.wtype) {
		app_set->net_info.wtype = t->wireless.addr_type;
		DBG_UDS("app_set->net_info.wtype=%d\n", app_set->net_info.wtype);
		isChanged++;
	}

	if(t->wireless.addr_type == NET_TYPE_STATIC){

		if(0!=strcmp(t->wireless.ipv4, app_set->net_info.wlan_ipaddr)){
			strcpy(app_set->net_info.wlan_ipaddr, t->wireless.ipv4);
			DBG_UDS("app_set->net_info.wlan_ipaddr=%s\n", app_set->net_info.wlan_ipaddr);
			isChanged++;
		}
		if(0!=strcmp(t->wireless.gw, app_set->net_info.wlan_gateway)){
			strcpy(app_set->net_info.wlan_gateway, t->wireless.gw);
			DBG_UDS("app_set->net_info.wlan_gateway=%s\n", app_set->net_info.wlan_gateway);
			isChanged++;
		}
		if(0!=strcmp(t->wireless.mask, app_set->net_info.wlan_netmask)){
			strcpy(app_set->net_info.wlan_netmask, t->wireless.mask);
			DBG_UDS("app_set->net_info.wlan_netmask=%s\n", app_set->net_info.wlan_netmask);
			isChanged++;
		}
	}

	// cradle
	if(t->cradle.addr_type != app_set->net_info.type) {
		app_set->net_info.type = t->cradle.addr_type;
		DBG_UDS("app_set->net_info.type=%d\n", app_set->net_info.type);
		isChanged++;
	}

	if(t->cradle.addr_type == NET_TYPE_STATIC){

		if(0!=strcmp(t->cradle.ipv4, app_set->net_info.eth_ipaddr)){
			strcpy(app_set->net_info.eth_ipaddr, t->cradle.ipv4);
			DBG_UDS("app_set->net_info.eth_ipaddr=%s\n", app_set->net_info.eth_ipaddr);
			isChanged++;
		}
		if(0!=strcmp(t->cradle.gw, app_set->net_info.eth_gateway)){
			strcpy(app_set->net_info.eth_gateway, t->cradle.gw);
			DBG_UDS("app_set->net_info.eth_gateway=%s\n", app_set->net_info.eth_gateway);
			isChanged++;
		}
		if(0!=strcmp(t->cradle.mask, app_set->net_info.eth_netmask)){
			strcpy(app_set->net_info.eth_netmask, t->cradle.mask);
			DBG_UDS("app_set->net_info.eth_netmask=%s\n", app_set->net_info.eth_netmask);
			isChanged++;
		}
	}
// under working .. needs remove wifiap.ssid part 

	// wifi ap info, NOT NULL
	if(0!=strcmp(t->wifi_ap.id, app_set->wifiap.ssid)){
		strcpy(app_set->wifiap.ssid, t->wifi_ap.id);
		DBG_UDS("app_set->wifiap.ssid=%s\n", app_set->wifiap.ssid);
		isChanged++;
	}
	if(0!=strcmp(t->wifi_ap.pw, app_set->wifiap.pwd)){
		strcpy(app_set->wifiap.pwd, t->wifi_ap.pw);
		DBG_UDS("app_set->wifiap.pwd=%s\n", app_set->wifiap.pwd);
		isChanged++;
	}
	
	for(i = 0; i < 4; i++)
	{
	    if(0!=strcmp(t->wifi_ap_list[i].id, app_set->wifilist[i].ssid)){
		    strcpy(app_set->wifilist[i].ssid, t->wifi_ap_list[i].id);
		    DBG_UDS("app_set->wifilist[%d].ssid=%s\n", i, app_set->wifilist[i].ssid);
		    isChanged++;
	    }
 	    if(0!=strcmp(t->wifi_ap_list[i].pw, app_set->wifilist[i].pwd)){
		    strcpy(app_set->wifilist[i].pwd, t->wifi_ap_list[i].pw);
		    DBG_UDS("app_set->wifilist[%d].pwd=%s\n", i, app_set->wifilist[i].pwd);
		    isChanged++;
	    }
	}

	if(isChanged>0) {
		DBG_UDS("isChanged:%d\n", isChanged);
	}

	return isChanged;
}

static int setNetworkConfiguration(T_CGI_NETWORK_CONFIG *t)
{
	int isChanged=0;

	// wireless
	if(t->wireless.addr_type != app_set->net_info.wtype) {
		app_set->net_info.wtype = t->wireless.addr_type;
		DBG_UDS("app_set->net_info.wtype=%d\n", app_set->net_info.wtype);
		isChanged++;
	}

	if(t->wireless.addr_type == NET_TYPE_STATIC){

		if(0!=strcmp(t->wireless.ipv4, app_set->net_info.wlan_ipaddr)){
			strcpy(app_set->net_info.wlan_ipaddr, t->wireless.ipv4);
			DBG_UDS("app_set->net_info.wlan_ipaddr=%s\n", app_set->net_info.wlan_ipaddr);
			isChanged++;
		}
		if(0!=strcmp(t->wireless.gw, app_set->net_info.wlan_gateway)){
			strcpy(app_set->net_info.wlan_gateway, t->wireless.gw);
			DBG_UDS("app_set->net_info.wlan_gateway=%s\n", app_set->net_info.wlan_gateway);
			isChanged++;
		}
		if(0!=strcmp(t->wireless.mask, app_set->net_info.wlan_netmask)){
			strcpy(app_set->net_info.wlan_netmask, t->wireless.mask);
			DBG_UDS("app_set->net_info.wlan_netmask=%s\n", app_set->net_info.wlan_netmask);
			isChanged++;
		}
	}

	// cradle
	if(t->cradle.addr_type != app_set->net_info.type) {
		app_set->net_info.type = t->cradle.addr_type;
		DBG_UDS("app_set->net_info.type=%d\n", app_set->net_info.type);
		isChanged++;
	}

	if(t->cradle.addr_type == NET_TYPE_STATIC){

		if(0!=strcmp(t->cradle.ipv4, app_set->net_info.eth_ipaddr)){
			strcpy(app_set->net_info.eth_ipaddr, t->cradle.ipv4);
			DBG_UDS("app_set->net_info.eth_ipaddr=%s\n", app_set->net_info.eth_ipaddr);
			isChanged++;
		}
		if(0!=strcmp(t->cradle.gw, app_set->net_info.eth_gateway)){
			strcpy(app_set->net_info.eth_gateway, t->cradle.gw);
			DBG_UDS("app_set->net_info.eth_gateway=%s\n", app_set->net_info.eth_gateway);
			isChanged++;
		}
		if(0!=strcmp(t->cradle.mask, app_set->net_info.eth_netmask)){
			strcpy(app_set->net_info.eth_netmask, t->cradle.mask);
			DBG_UDS("app_set->net_info.eth_netmask=%s\n", app_set->net_info.eth_netmask);
			isChanged++;
		}
	}
// under working .. needs remove wifiap.ssid part 

	// wifi ap info, NOT NULL
	if(0!=strcmp(t->wifi_ap.id, app_set->wifiap.ssid)){
		strcpy(app_set->wifiap.ssid, t->wifi_ap.id);
		DBG_UDS("app_set->wifiap.ssid=%s\n", app_set->wifiap.ssid);
		isChanged++;
	}
	if(0!=strcmp(t->wifi_ap.pw, app_set->wifiap.pwd)){
		strcpy(app_set->wifiap.pwd, t->wifi_ap.pw);
		DBG_UDS("app_set->wifiap.pwd=%s\n", app_set->wifiap.pwd);
		isChanged++;
	}
	
	/* todo
	for(i = 0; i < 4; i++)
	{
	     
	    if(0!=strcmp(t->wifilist[i].id, app_set->wifilist[i].ssid)){
		    strcpy(app_set->wifilist[i].ssid, t->wifi_list[i].id);
		    DBG_UDS("app_set->wifilist.ssid=%s\n", i, app_set->wifilist[i].ssid);
		    isChanged++;
	    }
 	    if(0!=strcmp(t->wifi_list[i].pw, app_set->wifilist[i].pwd)){
		    strcpy(app_set->wifilist[i].pwd, t->wifi_list[i].pw);
		    DBG_UDS("app_set->wifilist[%d].pwd=%s\n", i, app_set->wifilist[i].pwd);
		    isChanged++;
	    }
	}
	
	
	*/

#if 0
	if(t->live_stream_account_enable != app_set->account_info.ON_OFF) {
		app_set->account_info.ON_OFF = t->live_stream_account_enable;
		DBG_UDS("app_set->account_info.ON_OFF=%d\n", app_set->account_info.ON_OFF);
		isChanged++;
	}

	if(t->live_stream_account_enable){
		char dec_ID[32]={0};
		char dec_PW[32]={0};

		// 기존꺼 첵크
		if(app_set->account_info.enctype) {// 1, AES
			decrypt_aes(app_set->account_info.rtsp_userid, dec_ID, 32) ;
			decrypt_aes(app_set->account_info.rtsp_passwd, dec_PW, 32) ;
		} else {
			strcpy(dec_ID, app_set->account_info.rtsp_userid);
			strcpy(dec_PW, app_set->account_info.rtsp_passwd);
		}

		// check enctype
		if(app_set->account_info.enctype != t->live_stream_account_enctype){
			isChanged++;
			DBG_UDS("t->account_info.enctype=%d\n", t->live_stream_account_enctype);
		}

		// 새로 들어온값 check
		if(0!=strcmp(t->live_stream_account.id, dec_ID)
		|| 0!=strcmp(t->live_stream_account.pw, dec_PW)
		|| app_set->account_info.enctype != t->live_stream_account_enctype){

			app_set->account_info.enctype = t->live_stream_account_enctype;

			DBG_UDS("app_set->account_info.enctype    =%d\n", t->live_stream_account_enctype);
			DBG_UDS("app_set->account_info.rtsp_userid=%s\n", t->live_stream_account.id);
			DBG_UDS("app_set->account_info.rtsp_passwd=%s\n", t->live_stream_account.pw);

			if(app_set->account_info.enctype) {//1,  AES
				char enc_ID[32]={0};
				char enc_PW[32]={0};
                encrypt_aes(t->live_stream_account.id, enc_ID, 32);
                encrypt_aes(t->live_stream_account.pw, enc_PW, 32);
				
				//# if aes output is zero, strncpy can't copy all of data..
				//strncpy(app_set->account_info.rtsp_userid, enc_ID, 32);
				//strncpy(app_set->account_info.rtsp_passwd, enc_PW, 32);
				memcpy(app_set->account_info.rtsp_userid, enc_ID, 32);
				memcpy(app_set->account_info.rtsp_passwd, enc_PW, 32);

				//printf("result:cipertext");
			}
			else {
				strcpy(app_set->account_info.rtsp_userid, t->live_stream_account.id);
				strcpy(app_set->account_info.rtsp_passwd, t->live_stream_account.pw);
			}
			isChanged++;
		}
	}
#endif

	if(isChanged>0) {
		DBG_UDS("isChanged:%d\n", isChanged);

#if 0   // save and reboot; The system restart is changed by User Manually.
		// restart 후에 적용됨. 2019년 8월 26일, 웹에서 apply,누르면 restart는 하는걸로...
		sysprint("[APP] --- The network settings are changed and the System restarts ---\n");

		if(app_rec_state())
		{
			sleep(1) ;
			app_rec_stop(ON);
		}

		app_set_write();

		mic_exit_state(OFF_RESET, 0);
		app_main_ctrl(APP_CMD_EXIT, 0, 0);
#endif
	}

	return isChanged;
}

int updateOnvifUser(T_CGI_ONVIF_USER *t)
{
	if(!strcmp(t->id, ONVIF_DEFAULT_ID)) {
		strcpy(app_set->account_info.onvif.pw, t->pw);
		app_set->account_info.onvif.lv = t->lv;

		app_onvifserver_restart();

		return 0;
	}

	aprintf(" Can't set %s's password to %s\n", t->id, t->pw);
	return -1;
}

int updateUser(T_CGI_USER *t)
{
	if(!strcmp(t->id, WEB_DEFAULT_ID)) {
		strcpy(app_set->account_info.webuser.pw, t->pw);
		app_set->account_info.webuser.lv = t->lv;
		app_set->account_info.webuser.authtype = t->authtype; // 0:basic, 1:digest

		return 0;
	}

	aprintf(" Can't set %s's password to %s\n", t->id, t->pw);
	return -1;
}

int checkAccount(T_CGI_ACCOUNT *acc)
{
	DBG_UDS("old web account, id:%s, pw:%s\n", app_set->account_info.webuser.id, app_set->account_info.webuser.pw);
	if(!strcmp(app_set->account_info.webuser.id, acc->id)
	&& !strcmp(app_set->account_info.webuser.pw, acc->pw)) {
		return 0;
	}
	return -1;
}


int getVoipConfiguration(T_CGI_VOIP_CONFIG *t)
{
	// voip
	t->enable   = app_set->voip.ON_OFF;
	t->use_stun = app_set->voip.use_stun;
	t->port     = app_set->voip.port;
	strcpy(t->ipaddr, app_set->voip.ipaddr);
	strcpy(t->userid, app_set->voip.userid);
	strcpy(t->passwd, app_set->voip.passwd);
	strcpy(t->peerid, app_set->voip.peerid);

	// TODO:
	// current status, connected or disconnected

	return 0;
}

int setVoipConfiguration(T_CGI_VOIP_CONFIG *t)
{
	int isChanged=0;

	// voip config
	if(app_set->voip.ON_OFF != t->enable){
		app_set->voip.ON_OFF = t->enable;
		isChanged++;
	}

	if(app_set->voip.ON_OFF == ON) {
		if(app_set->voip.use_stun != t->use_stun){
			app_set->voip.use_stun = t->use_stun;
			isChanged++;
		}
		if(strcmp(app_set->voip.ipaddr,t->ipaddr)){
			strcpy(app_set->voip.ipaddr,t->ipaddr);
			isChanged++;
		}
		if(app_set->voip.port != t->port){
			app_set->voip.port = t->port;
			isChanged++;
		}
		if(strcmp( app_set->voip.userid, t->userid)){
			strcpy(app_set->voip.userid, t->userid);
			isChanged++;
		}
		if(strlen(t->passwd) && strcmp( app_set->voip.passwd,t->passwd)){
			strcpy(app_set->voip.passwd,t->passwd);
			isChanged++;
		}
		if(strcmp( app_set->voip.peerid, t->peerid)){
			strcpy(app_set->voip.peerid, t->peerid);
			isChanged++;
		}
	}

	return isChanged;
	// 웹에서 apply,누르면 restart는 하는걸로...
}


int getServersConfiguration(T_CGI_SERVERS_CONFIG *t)
{
	// backup server(ftp)
	t->bs.enable = app_set->ftp_info.ON_OFF;
	t->bs.upload_files = app_set->ftp_info.file_type; // 1: event, 0:all
	t->bs.port   = app_set->ftp_info.port;
	strcpy(t->bs.serveraddr, app_set->ftp_info.ipaddr);
	strcpy(t->bs.id, app_set->ftp_info.id);
	strcpy(t->bs.pw, app_set->ftp_info.pwd);

	// fota server(ftp2)
	t->fota.enable      = app_set->fota_info.ON_OFF;
	t->fota.server_info = app_set->fota_info.svr_info;
	t->fota.port        = app_set->fota_info.port;
	strcpy(t->fota.serveraddr, app_set->fota_info.ipaddr);
	strcpy(t->fota.id, app_set->fota_info.id);
	strcpy(t->fota.pw, app_set->fota_info.pwd);

	// media server(rtmp)
	t->mediaserver.enable            = app_set->rtmp.ON_OFF;
	t->mediaserver.use_full_path_url = app_set->rtmp.USE_URL;
	t->mediaserver.port              = app_set->rtmp.port;
	strcpy(t->mediaserver.full_path_url, app_set->rtmp.FULL_URL);
	strcpy(t->mediaserver.serveraddr,    app_set->rtmp.ipaddr);
	// strcpy(t->mediaserver.id, app_set->fota_info.id);
	// strcpy(t->mediaserver.pw, app_set->fota_info.pwd);

	// manage server
	t->ms.enable = app_set->srv_info.ON_OFF;
	t->ms.port   = app_set->srv_info.port;
	strcpy(t->ms.serveraddr, app_set->srv_info.ipaddr); // size:128, SERVER_URL_SIZE

	// ddns
	t->ddns.enable = app_set->ddns_info.ON_OFF;
	strcpy(t->ddns.serveraddr, app_set->ddns_info.serveraddr); // 64
	strcpy(t->ddns.hostname, app_set->ddns_info.hostname);     // 32
	strcpy(t->ddns.id, app_set->ddns_info.userId);
	strcpy(t->ddns.pw, app_set->ddns_info.passwd);

	// dns
	strcpy(t->dns.server1, app_set->net_info.dns_server1);
	strcpy(t->dns.server2, app_set->net_info.dns_server2);

	// ntp
	t->ntp.enable = app_set->time_info.timesync_type;
	strcpy(t->ntp.serveraddr, app_set->time_info.time_server); // 64

	t->time_zone = app_set->time_info.time_zone-12;				  // return (0 ~ 26) - 12  
	strcpy(t->time_zone_abbr, app_set->time_info.time_zone_abbr);
	t->daylight_saving = app_set->time_info.daylight_saving;

	// onvifserver
	t->onvif.enable = app_set->net_info.enable_onvif;
	strcpy(t->onvif.id, app_set->account_info.onvif.id); // max 32
	strcpy(t->onvif.pw, app_set->account_info.onvif.pw); // max 32
	
	t->p2p.enable = app_set->sys_info.P2P_ON_OFF;

	// voip
	t->voip.enable = app_set->voip.ON_OFF;
	t->voip.port   = app_set->voip.port;
	t->voip.use_stun = app_set->voip.use_stun;
	strcpy(t->voip.ipaddr, app_set->voip.ipaddr);
	strcpy(t->voip.userid, app_set->voip.userid);
	strcpy(t->voip.passwd, app_set->voip.passwd);
	strcpy(t->voip.peerid, app_set->voip.peerid);

	return 0;
}

int setServersConfiguration(T_CGI_SERVERS_CONFIG *t)
{
	int isChanged=0;

	// backup server(ftp)
	if(app_set->ftp_info.ON_OFF != t->bs.enable){
		app_set->ftp_info.ON_OFF = t->bs.enable;
		isChanged++;
	}
	if(app_set->ftp_info.file_type != t->bs.upload_files){
		// 0:all, 1:event
		app_set->ftp_info.file_type = t->bs.upload_files;
		isChanged++;
	}
	if(app_set->ftp_info.ON_OFF){
		if(app_set->ftp_info.port != t->bs.port){
			app_set->ftp_info.port = t->bs.port;
			isChanged++;
		}
		if(strcmp(app_set->ftp_info.ipaddr,t->bs.serveraddr)){
			strcpy(app_set->ftp_info.ipaddr,t->bs.serveraddr);
			isChanged++;
		}
		if(strcmp(app_set->ftp_info.id,t->bs.id)){
			strcpy(app_set->ftp_info.id,t->bs.id);
			isChanged++;
		}
		if(strcmp(app_set->ftp_info.pwd,t->bs.pw)){
			strcpy(app_set->ftp_info.pwd,t->bs.pw);
			isChanged++;
		}
	}

	// FOTA server
	if(app_set->fota_info.ON_OFF != t->fota.enable){
		app_set->fota_info.ON_OFF = t->fota.enable;
		isChanged++;
	}
	if(app_set->fota_info.ON_OFF){

		if(app_set->fota_info.svr_info != t->fota.server_info){ 
			app_set->fota_info.svr_info= t->fota.server_info;			// 0:same ftp, 1:manual
			isChanged++;
		}
		if (app_set->fota_info.svr_info == 1) // manual
		{
			if(app_set->fota_info.port != t->fota.port){
				app_set->fota_info.port = t->fota.port;
				isChanged++;
			}
			if (strcmp(app_set->fota_info.ipaddr, t->fota.serveraddr)) {
				strcpy(app_set->fota_info.ipaddr, t->fota.serveraddr);
				isChanged++;
			}
			if (strcmp(app_set->fota_info.id, t->fota.id)) {
				strcpy(app_set->fota_info.id, t->fota.id);
				isChanged++;
			}
			if (strcmp(app_set->fota_info.pwd, t->fota.pw)) {
				strcpy(app_set->fota_info.pwd, t->fota.pw);
				isChanged++;
			}
		}
	}

	//Media Server(rtmp)
	if(app_set->rtmp.ON_OFF != t->mediaserver.enable){
		app_set->rtmp.ON_OFF = t->mediaserver.enable;
		isChanged++;
	}
	if(app_set->rtmp.ON_OFF){
		if(app_set->rtmp.USE_URL != t->mediaserver.use_full_path_url){ 
			app_set->rtmp.USE_URL = t->mediaserver.use_full_path_url;
			isChanged++;
		}
		if (app_set->rtmp.USE_URL == 1) // full path url
		{
			if (strcmp(app_set->rtmp.FULL_URL, t->mediaserver.full_path_url)) {
				strcpy(app_set->rtmp.FULL_URL, t->mediaserver.full_path_url);
				isChanged++;
			}
		} else {
			if (strcmp(app_set->rtmp.ipaddr, t->mediaserver.serveraddr)) {
				strcpy(app_set->rtmp.ipaddr, t->mediaserver.serveraddr);
				isChanged++;
			}
			if(app_set->rtmp.port != t->mediaserver.port){
				app_set->rtmp.port = t->mediaserver.port;
				isChanged++;
			}
			// if (strcmp(app_set->rtmp.id, t->mediaserver.id)) {
			// 	strcpy(app_set->rtmp.id, t->mediaserver.id);
			// 	isChanged++;
			// }
			// if (strcmp(app_set->rtmp.pwd, t->mediaserver.pw)) {
			// 	strcpy(app_set->rtmp.pwd, t->mediaserver.pw);
			// 	isChanged++;
			// }
		}
	}

	// manage server
	if(app_set->srv_info.ON_OFF != t->ms.enable){
		app_set->srv_info.ON_OFF = t->ms.enable;
		isChanged++;
	}
	if(app_set->srv_info.ON_OFF){
		if(app_set->srv_info.port != t->ms.port){
			app_set->srv_info.port = t->ms.port;
			isChanged++;
		}
		if(strcmp(app_set->srv_info.ipaddr, t->ms.serveraddr)){
			strcpy(app_set->srv_info.ipaddr, t->ms.serveraddr); // size:128, SERVER_URL_SIZE
			isChanged++;
		}
	}

	// ddns
	if(app_set->ddns_info.ON_OFF != t->ddns.enable){
		app_set->ddns_info.ON_OFF = t->ddns.enable;
		isChanged++;
	}
	if(app_set->ddns_info.ON_OFF){
		if(strcmp(app_set->ddns_info.serveraddr, t->ddns.serveraddr)){
			strcpy(app_set->ddns_info.serveraddr, t->ddns.serveraddr); // 64
			isChanged++;
		}
		if(strcmp(app_set->ddns_info.hostname, t->ddns.hostname)){
			strcpy(app_set->ddns_info.hostname, t->ddns.hostname); // 32 
			isChanged++;
		}
		if(strcmp(app_set->ddns_info.userId, t->ddns.id)){
			strcpy(app_set->ddns_info.userId, t->ddns.id); // 32 
			isChanged++;
		}
		if(strcmp(app_set->ddns_info.passwd, t->ddns.pw)){
			strcpy(app_set->ddns_info.passwd, t->ddns.pw); // 32 
			isChanged++;
		}
	}

	// dns
	if(strcmp(app_set->net_info.dns_server1, t->dns.server1)){
		strcpy(app_set->net_info.dns_server1, t->dns.server1);
		isChanged++;
	}
	if(strcmp(app_set->net_info.dns_server2, t->dns.server2)){
		strcpy(app_set->net_info.dns_server2, t->dns.server2);
		isChanged++;
	}

	// ntp
	if(app_set->time_info.timesync_type != t->ntp.enable){
		app_set->time_info.timesync_type = t->ntp.enable;
		isChanged++;
	}
	if(app_set->time_info.timesync_type){
		if(strcmp(app_set->time_info.time_server, t->ntp.serveraddr)){
			strcpy(app_set->time_info.time_server, t->ntp.serveraddr); // 64
			isChanged++;
		}
	}

	t->time_zone += 12; // A value of +12 is for device only.
	if(app_set->time_info.time_zone != t->time_zone){
		app_set->time_info.time_zone = t->time_zone;
		isChanged++;
	}
	if(app_set->time_info.daylight_saving != t->daylight_saving){
		app_set->time_info.daylight_saving = t->daylight_saving;
		isChanged++;
	}

	if(strcmp(app_set->time_info.time_zone_abbr, t->time_zone_abbr)){
		strcpy(app_set->time_info.time_zone_abbr, t->time_zone_abbr); // 6
		isChanged++;
	}

	// onvifserver
	if(app_set->net_info.enable_onvif != t->onvif.enable){
		app_set->net_info.enable_onvif = t->onvif.enable;
		isChanged++;
	}

#if 0
	if(0!=strcmp(app_set->account_info.onvif.pw, t->onvif.pw)){
		strcpy(app_set->account_info.onvif.pw, t->onvif.pw); // 31
		isChanged++;

		app_onvifserver_restart();
	}
#endif

#if 0 // 항상 Enable로 변경됨
	if(app_set->sys_info.P2P_ON_OFF != t->p2p.enable){
		app_set->sys_info.P2P_ON_OFF = t->p2p.enable;
		isChanged++;
	}
#endif


	// voip config
	if(app_set->voip.ON_OFF != t->voip.enable){
		app_set->voip.ON_OFF = t->voip.enable;
		isChanged++;
	}
	if(app_set->voip.ON_OFF == ON) {
		if(app_set->voip.use_stun != t->voip.use_stun){
			app_set->voip.use_stun = t->voip.use_stun;
			isChanged++;
		}
		if(strcmp(app_set->voip.ipaddr,t->voip.ipaddr)){
			strcpy(app_set->voip.ipaddr,t->voip.ipaddr);
			isChanged++;
		}
		if(app_set->voip.port != t->voip.port){
			app_set->voip.port = t->voip.port;
			isChanged++;
		}
		if(strcmp( app_set->voip.userid, t->voip.userid)){
			strcpy(app_set->voip.userid, t->voip.userid);
			isChanged++;
		}
		if(strlen(t->voip.passwd) && strcmp( app_set->voip.passwd,t->voip.passwd)){
			strcpy(app_set->voip.passwd,t->voip.passwd);
			isChanged++;
		}
		if(strcmp( app_set->voip.peerid, t->voip.peerid)){
			strcpy(app_set->voip.peerid, t->voip.peerid);
			isChanged++;
		}
	}
	
	return isChanged;
	// restart 후에 적용됨. 2019년 8월 22일, 웹에서 apply,누르면 restart는 하는걸로...
}

static int getUserConfiguration(T_CGI_USER_CONFIG *t)
{
	sprintf(t->web.id, "%s", app_set->account_info.webuser.id);
	sprintf(t->web.pw, "%s", app_set->account_info.webuser.pw);

	t->onvif.enable = app_set->net_info.enable_onvif;
	sprintf(t->onvif.id, "%s", app_set->account_info.onvif.id);
	sprintf(t->onvif.pw, "%s", app_set->account_info.onvif.pw);

	char rtsp_user[32]={0};
	char rtsp_pass[32]={0};
	if(app_set->account_info.enctype) // if AES
	{
		decrypt_aes(app_set->account_info.rtsp_userid, rtsp_user, 32);
		decrypt_aes(app_set->account_info.rtsp_passwd, rtsp_pass, 32);
	}
	else {
		strcpy(rtsp_user, app_set->account_info.rtsp_userid);
		strcpy(rtsp_pass, app_set->account_info.rtsp_passwd);
	}
	t->rtsp.enable = app_set->account_info.ON_OFF;
	t->rtsp.enctype = app_set->account_info.enctype;
	strcpy(t->rtsp.id, rtsp_user);
	strcpy(t->rtsp.pw, rtsp_pass);

	return 0;
}

int setUserConfiguration(T_CGI_USER_CONFIG *t)
{
	int isChanged=0;

	// web part
	if(0 == app_set_web_password(t->web.id, t->web.pw, 0, 0)) {
		DBG_UDS("Updated web password: id:%s, pw:%s\n", t->web.id, t->web.pw);
		isChanged++;
	}else {
		DBG_UDS("Failed set Web Account, id:%s, pw:%s\n", t->web.id, t->web.pw);
	}

	// onvif part
	//if(0==strcmp(t->onvif.id, app_set->account_info.onvif.id))
	if(0==strcmp(t->onvif.id, "admin"))
	{
		//strcpy(app_set->account_info.onvif.id, "admin"); // fixed
		if(0!=strcmp(t->onvif.pw, app_set->account_info.onvif.pw)){
			strcpy(app_set->account_info.onvif.pw, t->onvif.pw);
			DBG_UDS("Updated app_set->account_info.onvif.pw=%s\n", app_set->account_info.onvif.pw);
			isChanged++;
		}
	}
	else {
		DBG_UDS("Failed set ONVIF Account, id:%s, pw:%s\n", t->onvif.id, t->onvif.pw);
	}

	// rtsp account part
	if(t->rtsp.enable != app_set->account_info.ON_OFF) {
		app_set->account_info.ON_OFF = t->rtsp.enable;
		DBG_UDS("app_set->account_info.ON_OFF=%d\n", app_set->account_info.ON_OFF);
		isChanged++;
	}

#if 1
	if(t->rtsp.enable){
		char dec_ID[32]={0};
		char dec_PW[32]={0};

		// 기존꺼 첵크
		if(app_set->account_info.enctype) {// 1, AES
			decrypt_aes(app_set->account_info.rtsp_userid, dec_ID, 32) ;
			decrypt_aes(app_set->account_info.rtsp_passwd, dec_PW, 32) ;
		} else {
			strcpy(dec_ID, app_set->account_info.rtsp_userid);
			strcpy(dec_PW, app_set->account_info.rtsp_passwd);
		}

		// check enctype
		if(app_set->account_info.enctype != t->rtsp.enctype){
			isChanged++;
			DBG_UDS("t->account_info.enctype=%d\n", t->rtsp.enctype);
		}

		// 새로 들어온값 check
		if(0!=strcmp(t->rtsp.id, dec_ID)
		|| 0!=strcmp(t->rtsp.pw, dec_PW)
		|| app_set->account_info.enctype != t->rtsp.enctype){

			app_set->account_info.enctype = t->rtsp.enctype;

			DBG_UDS("app_set->account_info.enctype    =%d\n", t->rtsp.enctype);
			DBG_UDS("app_set->account_info.rtsp_userid=%s\n", t->rtsp.id);
			DBG_UDS("app_set->account_info.rtsp_passwd=%s\n", t->rtsp.pw);

			if(app_set->account_info.enctype) {//1,  AES
				char enc_ID[32]={0};
				char enc_PW[32]={0};
                encrypt_aes(t->rtsp.id, enc_ID, 32);
                encrypt_aes(t->rtsp.pw, enc_PW, 32);
				
				memcpy(app_set->account_info.rtsp_userid, enc_ID, 32);
				memcpy(app_set->account_info.rtsp_passwd, enc_PW, 32);
			}
			else {
				strcpy(app_set->account_info.rtsp_userid, t->rtsp.id);
				strcpy(app_set->account_info.rtsp_passwd, t->rtsp.pw);
			}
			isChanged++;
		}
	}
#endif

	return isChanged;
}

static int getSystemInfo(T_CGI_SYSTEM_INFO *t){

	sprintf(t->model, "%s", MODEL_NAME);
	sprintf(t->fwver, "%s", app_set->sys_info.fw_ver);
    t->https = app_set->net_info.https_enable;
    t->onvif = app_set->net_info.enable_onvif;
    t->p2p   = app_set->sys_info.P2P_ON_OFF;
	t->rec   = app_rec_state(); 
	t->ftp   = app_cfg->ste.b.ftp_run;

	return 0;
}

static int getSystemConfiguration(T_CGI_SYSTEM_CONFIG *t)
{
	char MacAddr[12]={0} ;

	sprintf(t->model, "%s", MODEL_NAME);
	sprintf(t->fwver, "%s", app_set->sys_info.fw_ver);
	sprintf(t->devid, "%s", app_set->sys_info.deviceId);
	sprintf(t->uid, "%s",   app_set->sys_info.uid);
	if(!DefaultGetMac(MacAddr)){
		sprintf(t->mac, "%s", MacAddr);
	}else {
		sprintf(t->mac, "%s", "_MACADDRESS_");
	}

	return 0;
}

static int setSystemConfiguration(T_CGI_SYSTEM_CONFIG *t)
{
	int isChanged=0;
	if(strlen(t->devid) > 0 && 0!=strcmp(t->devid, app_set->sys_info.deviceId)) {
		sprintf(app_set->sys_info.deviceId, "%s", t->devid);
		isChanged++;
	}
	else {
		//return -1;
	}

#if 0 // Block NSS UID
	if(strlen(t->uid) > 0 && 0!=strcmp(t->uid, app_set->sys_info.uid)) {
		sprintf(app_set->sys_info.uid, "%s", t->uid);
		isChanged++;
	}
#endif

	return isChanged;
}

unsigned long prefix2mask(int prefix)
{
	struct in_addr mask;
	memset(&mask, 0, sizeof(mask));
	if (prefix) {
		return htonl(~((1 << (32 - prefix)) - 1));
	} else {
		return htonl(0);
	}
}

/*
 * deprecated, if param VideoEncoding is 0, jpeg
 * deprecated, if param VideoEncoding is 2, h264
 */
static int onvif_getResolution(int VideoEncoding, int *w, int *h)
{
    if(app_set->ch[STM_CH_NUM].resol == RESOL_1080P) // FHD
	{
		*w = 1920;
		*h = 1080;
	}
    else if(app_set->ch[STM_CH_NUM].resol == RESOL_480P) // SD
	{
		*w = 720;
		*h = 480;
	}
    else // default 720p if(app_set->ch[STM_CH_NUM].resol == RESOL_720P) // HD
	{
		*w = 1280;
		*h = 720;
	}

	return 0;
}

static int getResolutionIdx(int ch)
{
	int res_idx = 0; // 0:1080p, 1:720p, 2:480p

    if(app_set->ch[ch].resol == RESOL_1080P) // FHD
		res_idx = 0;
    else if(app_set->ch[ch].resol == RESOL_720P) // HD
		res_idx = 1;
    else if(app_set->ch[ch].resol == RESOL_480P) // SD
		res_idx = 2;

	return res_idx;
}

// The parameter "ch" means  Recording or Streaming
// if ch is 4, it will return streaming information, 
// else return recording information.
static int getKbps(int ch)
{
	int br = app_set->ch[ch].quality;
	return br;
}

#if 0
static int getKbpsIdx(int ch)
{
	int idx = app_set->ch[ch].quality;
	return idx;
}
#endif

// The parameter "ch" means  Recording or Streaming
// if ch is 4, it will return streaming information, 
// else return recording information.
static int getRC(int ch) // rate control
{
	int rc = app_set->ch[ch].rate_ctrl;
    return  rc;
}

// The parameter "ch" means  Recording or Streaming
// if ch is 4, it will return streaming information, 
// else return recording information.
static int getFps(int ch)
{
    int fps = app_set->ch[ch].framerate;
	return fps ;
}

#if 0
static int getFpsIdx(int ch)
{
    int idx = app_set->ch[ch].framerate;
	return idx;
}
#endif

// The parameter "ch" means  Recording or Streaming
// if ch is 4, it will return streaming information, 
// else return recording information.
// GOP means I-Frame Interval
static int getGop(int ch)
{
    int gop = app_set->ch[ch].gop;
	return gop;
}

static int setRecordOptions(int pre_rec,int auto_rec, int audio_rec, int rec_interval, int rec_overwrite)
{
	app_set->rec_info.auto_rec   = auto_rec;     // 0:OFF, 1:ON

	if(app_set->rec_info.period_idx != rec_interval
	|| app_set->rec_info.pre_rec    != pre_rec
	|| app_set->rec_info.audio_rec  != audio_rec
	|| app_set->rec_info.overwrite  != rec_overwrite){

		app_set->rec_info.pre_rec    = pre_rec;
		app_set->rec_info.period_idx = rec_interval; // 0:1MIN, 1:5MIN
		app_set->rec_info.overwrite  = rec_overwrite;
		app_set->rec_info.audio_rec  = audio_rec;

#if 0
		// 설정만 변경합니다. 부팅 하면서  적용됩니다.
		app_rec_stop(ON);
		sleep(1);
		app_rec_start();
#endif
	}
	return 0;
}

static int setDisplayDateTime(int display_datetime)
{
	if(app_set->sys_info.osd_set != display_datetime){
		app_set->sys_info.osd_set = display_datetime;

#if 0
		app_cap_stop();
		app_cap_start();
#endif
	}

	return 0;
}
#if 0
static int setP2PServer(int p2p_enable, char *username, char *password)
{
	if(app_set->sys_info.P2P_ON_OFF != p2p_enable
	|| strcmp (app_set->sys_info.p2p_id, username) != 0
	|| strcmp (app_set->sys_info.p2p_passwd, password) != 0) {
		app_set->sys_info.P2P_ON_OFF = p2p_enable;
		sprintf(app_set->sys_info.p2p_id, "%s", username);           // 32
		sprintf(app_set->sys_info.p2p_passwd, "%s", password);       // 32

#if 0
		if(p2p_enable){
			app_p2p_stop();
			sleep(1);
			app_p2p_start();
		}
		else {
			app_p2p_stop();
		}
#endif
	}

	return 0;
}
#endif
static int setDynamicVideoQuality(int rec_fps, int rec_bps, int rec_gop, int rec_rc,
              int stm_res, int stm_fps, int stm_bps, int stm_gop, int stm_rc)
{
	int ch = STM_CH_NUM;
	// STM
#if defined(NEXXONE) || defined(NEXXB_ONE)
	if(stm_fps <= DEFAULT_FPS && stm_fps > 0 && app_set->ch[ch].framerate != stm_fps)
	{
	    ctrl_vid_framerate(ch, stm_fps) ;
	}
	if(stm_bps <= MAX_STM_BITRATE && stm_bps >= MIN_STM_BITRATE && app_set->ch[ch].quality != stm_bps)
    {
	    ctrl_vid_bitrate(ch, stm_bps) ;
	}
	if(stm_gop <= DEFAULT_FPS && stm_gop > 0 && app_set->ch[ch].gop != stm_gop)
	{
	    ctrl_vid_gop_set(ch, stm_gop) ;
	}
	if(stm_res < RESOL_1080P && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
		ctrl_vid_resolution(stm_res);
	
#else
	if(stm_fps <= DEFAULT_FPS && stm_fps > 0 && app_set->ch[ch].framerate != stm_fps)
	{
	    ctrl_vid_framerate(ch, stm_fps) ;
	}
	if(stm_bps <= MAX_STM_BITRATE && stm_bps >= MIN_STM_BITRATE && app_set->ch[ch].quality != stm_bps)
    {
	    ctrl_vid_bitrate(ch, stm_bps) ;
	}
	if(stm_gop <= DEFAULT_FPS && stm_gop > 0 && app_set->ch[ch].gop != stm_gop)
	{
	    ctrl_vid_gop_set(ch, stm_gop) ;
	}
	if(stm_res < MAX_RESOL && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
	{
		ctrl_vid_resolution(stm_res);
	}

#endif

	DBG_UDS("[STM] ch:%d, fps:%d, bps:%d, gop:%d resolution = %d\n", ch, stm_fps, stm_bps, stm_gop, stm_res);

	return 0;

}

static int setVideoQuality(int rec_fps, int rec_bps, int rec_gop, int rec_rc,
              int stm_res, int stm_fps, int stm_bps, int stm_gop, int stm_rc)
{
	int ch=0;
	
	// first, set recording channels
	for(ch = 0; ch < MODEL_CH_NUM; ch++)
	{
		DBG_UDS("[SET Rec VideoQuality] ch:%d, fps:%d, bps:%d, gop:%d, rc:%d\n", ch, rec_fps, rec_bps, rec_gop, rec_rc);
		if(rec_fps <= DEFAULT_FPS && rec_fps > 0 && app_set->ch[ch].framerate != rec_fps) 
			app_set->ch[ch].framerate = rec_fps ;
		if(rec_bps <= MAX_REC_BITRATE && rec_bps >= MIN_REC_BITRATE && app_set->ch[ch].quality != rec_bps)
			app_set->ch[ch].quality = rec_bps ;
		if(rec_gop <= DEFAULT_FPS && rec_gop > 0 && app_set->ch[ch].gop != rec_gop) 
			app_set->ch[ch].gop = rec_gop ;
		if(rec_rc != app_set->ch[ch].rate_ctrl)
			app_set->ch[ch].rate_ctrl = rec_rc ;
	}
	// end set recoridng channels

	// now start to set streaming channel
	ch=STM_CH_NUM;
#if defined(NEXXONE) || defined(NEXXB_ONE)
	if(stm_res < RESOL_1080P && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
		ctrl_vid_resolution(stm_res);
#else 
	if(stm_res < MAX_RESOL && stm_res >= 0 && app_set->ch[ch].resol != stm_res)
		ctrl_vid_resolution(stm_res);
#endif

	if(stm_fps <= DEFAULT_FPS && stm_fps > 0 && app_set->ch[ch].framerate != stm_fps)
	{
	    ctrl_vid_framerate(ch, stm_fps) ;
	}
	if(stm_bps <= MAX_STM_BITRATE && stm_bps >= MIN_STM_BITRATE && app_set->ch[ch].quality != stm_bps)
    {
	    ctrl_vid_bitrate(ch, stm_bps) ;
	}
	if(stm_gop <= DEFAULT_FPS && stm_gop > 0 && app_set->ch[ch].gop != stm_gop)
	{
	    ctrl_vid_gop_set(ch, stm_gop) ;
	}
	if(stm_rc != app_set->ch[ch].rate_ctrl){
        ctrl_vid_rate(ch, stm_rc, 0) ;
	}
	// finish set streaming channel
	DBG_UDS("[SET Stm Video Quality] ch:%d, res:%d, fps:%d, bps:%d, gop:%d, rc:%d\n", ch, stm_res, stm_fps, stm_bps, stm_gop, stm_rc);

	return 0;
}

int setNTP(int fromDHCP, char *ntp)
{
	int isChanged=0;
	if(app_set->net_info.ntpFromDHCP != fromDHCP){
		app_set->net_info.ntpFromDHCP = fromDHCP;
		isChanged+=1;
	}

	if(!fromDHCP && strlen(ntp)> 0 && strcmp(app_set->time_info.time_server, ntp)) {
		strcpy(app_set->time_info.time_server, ntp) ;
		isChanged+=1;
	}

	return isChanged;
}

int setDNS(int fromDHCP, char *dns)
{
	int isChanged=0;
	if(app_set->net_info.dnsFromDHCP != fromDHCP){
		app_set->net_info.dnsFromDHCP = fromDHCP;
		isChanged+=1;
	}

	if(!fromDHCP && strlen(dns)> 0 && strcmp(app_set->net_info.dns_server1, dns)) {
		strcpy(app_set->net_info.dns_server1, dns) ;
		isChanged+=1;
	}

	return isChanged;
}

void *myFunc(void *arg)
{
	int ret;
	int ipaddr;
	int cs_uds;
	cs_uds = *((int *)arg);

	char rbuf[256]={0}, wbuf[256]={0};
	do {
#if 1
		ret = read(cs_uds, rbuf, sizeof rbuf);
#else
		ret = recvfrom(cs_uds, rbuf, sizeof rbuf, 0, (struct sockaddr *)&cs_addr, &cs_len);
#endif
		if (ret < 0) {
			perror("read cmd error: ");
			sleep(1);
			return NULL; //break;
		}
		DBG_UDS("read cmd:%s, ret:%d\n", rbuf, ret);

		if (strcmp(rbuf, "eXit") == 0)
		{
			printf("received eXit\n");
			//sprintf(wbuf, "OK");
			//ret = send(cs_uds, wbuf, strlen(wbuf), 0);
			//if(ret == -1) {
			//	ERR_PRINT("failed send\n");
			//}
		}
		else if (strcmp(rbuf, "ControlTelnetd") == 0) {
			sysprint("[APP_UDS] --- ControlTelnetd ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){

				int flag=0; // 0:disable 1:enable, 2:toggle
				ret = read(cs_uds, &flag, sizeof flag);
				DBG_UDS("ControlTelnetd read Data, ret:%d, flag:%d\n", ret, flag);
				if(ret>0)
				{
					app_telnetd_enable(flag) ;
				}
				else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read:");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "ControlRtmp") == 0 && app_set->rtmp.ON_OFF) {
			sysprint("[APP_UDS] --- ControlRtmp ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){

				int flag=0; // 0:disable 1:enable
				ret = read(cs_uds, &flag, sizeof flag);
				DBG_UDS("ControlRtmp read Data, ret:%d, flag:%d\n", ret, flag);
				if(ret>0) {
					if (flag == 0) {
						app_rtmp_disable();
					} else if (flag == 1) {
						app_rtmp_enable();
					} else {
						__builtin_unreachable();
					}
				}
				else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read:");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetSystemDateAndTime") == 0)
		{
			//sysprint("[APP_UDS] --- GetSystemDateAndTime---\n");
			char str[128] = {0};
			int timezone = app_set->time_info.time_zone - 12;
			sprintf(str, "TZ=%d,DS=%d,ST=%d", timezone,
					app_set->time_info.daylight_saving,
					app_set->time_info.timesync_type );

			ret = write(cs_uds, str, sizeof str);
			if (ret > 0) {
				DBG_UDS("write, ret=%d, str:%s\n", ret, str);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}
		}
		else if (strcmp(rbuf, "GetNetworkInterface") == 0){
			//sysprint("[APP_UDS] --- GetNetworkInterfaces ---\n");

			// read interface name, interface의 이름을 확인합니다.
			char iface[128]={0};
			ret = read(cs_uds, iface, sizeof iface);
			if (ret > 0)
			{
				DBG_UDS("read iface:%s, ret:%d\n", iface, ret);

				ONVIF_NET_INFO st;
				char ipinfo[128] = "NULL"; // fix128bytes, 호출쪽에서 읽을 예정이라,  쓰레기 문자열 이라도 써줘야함.
				// 정보가 정확하지 않으면, Milestone에서 등록 실패.

				if(0 == getNetworkInfo(iface, &st)){

					if(0 == strcmp(iface, "eth0")){
						st.dhcp = app_set->net_info.type;
					}
					else {
						st.dhcp = app_set->net_info.wtype;
					}

					//dhcp,ipaddr,subnet,gateway
					sprintf(ipinfo, "%d,%s,%s,%s",
							st.dhcp,
							st.ipaddr,
							st.netmask,
							st.gateway);
					DBG_UDS("GetNetworkInterface : %s\n", ipinfo);
				}

				// ping-pong이기때문에 꼭 write 야함.
				ret = write(cs_uds, ipinfo, sizeof ipinfo);
				if (ret > 0)
				{
					DBG_UDS("written bytes:%d\n", ret); // TODO check verify
				}
				else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed write:");
				}
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "GetNetworkProtocols") == 0){
			//sysprint("[APP_UDS] --- GetNetworkProtocols ---\n");

			// http, https, rtsp 의 순으로 보냅니다.
			// 구조체 형식으로 보내지 않으면, 데이터 크기, 개수 변경시 뒈짐
			char strProtocols[128] = {0};
			sprintf(strProtocols, "%d %d %d %d %d %d",
					app_set->net_info.http_port,
					app_set->net_info.https_port,
					app_set->net_info.rtsp_port,
					app_set->net_info.http_enable,
					app_set->net_info.https_enable,			// not supported
					app_set->net_info.rtsp_enable 
				   );

			ret = write(cs_uds, strProtocols, sizeof strProtocols);
			if (ret > 0) {
				// TODO something...
				DBG_UDS("sent, ret=%d, strProtocols:%s\n", ret, strProtocols);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}
		}
		else if (strcmp(rbuf, "GetNTP") == 0)
		{
			sysprint("[APP_UDS] --- GetNTP ---\n");

			char strNTP[128] = {0};
			//TODO, sprintf(strNTP, "%s", app_set->time_info.time_server);
			sprintf(strNTP, "%d %s", app_set->net_info.ntpFromDHCP, app_set->time_info.time_server);

			ret = write(cs_uds, strNTP, sizeof strNTP);
			if (ret > 0) {
				DBG_UDS("sent, ret=%d, strNTP:%s\n", ret, strNTP);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}
		}
		else if (strcmp(rbuf, "GetDNS") == 0)
		{
			sysprint("[APP_UDS] --- GetDNS ---\n");

			char strDNS[128] = {0};
			//TODO, sprintf(strDNS, "%s", app_set->net_info.dns_server1[0]);
			sprintf(strDNS, "%d %s", app_set->net_info.dnsFromDHCP, app_set->net_info.dns_server1);

			ret = write(cs_uds, strDNS, sizeof strDNS);
			if (ret > 0) {
				DBG_UDS("sent, ret=%d, strDNS:%s\n", ret, strDNS);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}
		}
		else if (strcmp(rbuf, "GetHostname") == 0) {
			sysprint("[APP_UDS] --- Get Hostname ---\n");

			char hname[100]={0};
			char sz[128] = {0}; // write buffer with 128 bytes

			// 1. get, 이gethostname 호출할꺼면 직접 Client에서 해도 되지만, 앞으로 어떻게 될지 모르니,
			// Server에서 처리하는 개념으로..., name 저장은 안함.
			gethostname(hname, sizeof(hname));

			// 2. make data 
			sprintf(sz, "%d %s", app_set->net_info.hostnameFromDHCP, hname);

			// 3. send
			ret = write(cs_uds, sz, sizeof sz);
			if (ret > 0) {
				DBG_UDS("write, ret=%d, strHostName:%s\n", ret, sz);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}
		}
		else if (strcmp(rbuf, "SetOnvifUser") == 0){
			sysprint("[APP_UDS] --- SetOnvifUser ---\n");

			T_ONVIF_USER st;
			memset(&st, 0, sizeof st);
			ret = read(cs_uds, &st, sizeof st);
			if(ret > 0){
				DBG_UDS("SetOnvifUser id:%s, pw:%s\n", st.UserName, st.Password);
				SetOnvifUser(&st, cs_uds);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetNetworkInterfaces") == 0){ //subnet -> prefixlen
			sysprint("[APP_UDS] --- SetNetworkInterfaces ---\n");

			T_ONVIF_NETWORK_INTERFACE st;
			memset(&st, 0, sizeof st);
			ret = read(cs_uds, &st, sizeof st);
			if(ret > 0){
				ipaddr = prefix2mask(st.ipv4_prefixlen) ;

				DBG_UDS("\nenabled:%d, token:%s, mtu:%d, ipv4_flag:%d, ipv4_enabled:%d, ipv4_dhcp:%d, ipv4_addr:%s, ipv4_prefixlen:%d, subnet = %s\n",
						st.enabled,        // eth or wlan 
						st.token,
						st.mtu,           
						st.ipv4_flag,
						st.ipv4_enabled,   
						st.ipv4_dhcp,
						st.ipv4_addr,
						st.ipv4_prefixlen, inet_ntoa(*(struct in_addr *)&ipaddr));

				ctrl_set_network(st.ipv4_dhcp, st.token, st.ipv4_addr, inet_ntoa(*(struct in_addr *)&ipaddr)) ;
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetNetworkDefaultGateway") == 0){
			char gw[32]={0};
			sysprint("[APP_UDS] --- SetNetworkDefaultGateway ---\n");

			ret = read(cs_uds, gw, sizeof gw);
			if(ret > 0){
				ctrl_set_gateway(gw) ;
				// TODO something...
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write:");
			}

			DBG_UDS("\n gw:%s\n", gw);
		}
		else if (strcmp(rbuf, "SetNetworkProtocols") == 0)
		{
			onvif_NetworkProtocol np;
			sysprint("[APP_UDS] --- SetNetworkProtocols ---\n");

			memset(&np, 0, sizeof np);
			ret = read(cs_uds, &np, sizeof np);
			if(ret > 0){
				DBG_UDS("\nHTTPEnable:%d, HTTPFlag:%d, HTTPPort[0]:%d\n"
						"HTTPSEnable:%d, HTTPSFlag:%d, HTTPSPort[0]:%d\n"
						"RTSPEnable:%d, RTSPFlag:%d, RTSPPort[0]:%d\n",
						np.HTTPEnabled,  np.HTTPFlag,  np.HTTPPort[0],
						np.HTTPSEnabled, np.HTTPSFlag, np.HTTPSPort[0],
						np.RTSPEnabled,  np.RTSPFlag,  np.RTSPPort[0]);

				setNetworkProtocols(&np);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetZeroConfiguration") == 0)
		{
			sysprint("[APP_UDS] --- SetZeroConfiguration ---\n");

			int  Enabled;
			memset(rbuf, 0, sizeof rbuf);
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0) {
				sscanf(rbuf, "Enabled=%d", &Enabled);
				DBG_UDS("SetZeroConfiguration, Enabled:%d\n", Enabled);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetNTP") == 0)
		{
			sysprint("[APP_UDS] --- Set NTP ---\n");

			int  FromDHCP;    // 0:no, 1:dhcp
			char ntp[100]=""; // ntp server 
			memset(rbuf, 0, sizeof rbuf);
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0)
			{
				sscanf(rbuf, "FromDHCP=%d,ntp=%s", &FromDHCP, ntp);
				setNTP(FromDHCP, ntp);
				DBG_UDS("SetNTP, FromDHCP:%d, ntp:%s\n", FromDHCP, ntp);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetDNS") == 0)
		{
			//sysprint("[APP_UDS] --- Set DNS ---\n");

			int  FromDHCP;    // 0:no, 1:dhcp
			char dns[100]=""; // dns server
			memset(rbuf, 0, sizeof rbuf);
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0)
			{
				sscanf(rbuf, "FromDHCP=%d,dns=%s", &FromDHCP,dns);
				setDNS(FromDHCP, dns);
				DBG_UDS("SetDNS, FromDHCP:%d, dns:%s\n", FromDHCP, dns);
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		else if (strcmp(rbuf, "SetHostname") == 0)
		{// ONVIF
			//sysprint("[APP_UDS] --- Set Hostname ---\n");

			int fromDHCP=0; // 0:no, 1:dhcp
			char str[100]={0}; // hostname's size of onvifserver module
			memset(rbuf, 0, sizeof rbuf);
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0)
			{
				sscanf(rbuf, "fromDHCP=%d,name=%s", &fromDHCP, str);

				// TODO verify hostname RFC1123???, 
				// 이건 클라이언트(onvifserver or CGI)에서 첵크하고 미리 보낼것..onvifserver 모듈 참고.
				if(strlen(str)>1){
					onvif_setHostname(fromDHCP, str);
				}
			}
			else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:");
			}
		}
		////// maintenance 
		else if (strcmp(rbuf, "SystemFactoryDefault") == 0)
		{
			sysprint("[APP_UDS] --- System Factory Default ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){

				int type; // 0:soft, 1:hard
				memset(rbuf, 0, sizeof rbuf);
				ret = read(cs_uds, rbuf, sizeof rbuf);
				DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
				if (ret > 0)
				{
					sscanf(rbuf, "type=%d", &type); // 1:all
					DBG_UDS("SystemFactoryDefault type:%d\n", type);

					if (type >= 0 && type < 2){
						SetFactoryDefault(type);
					}
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read:"); 
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "SetSystemDateAndTime") == 0)
		{
			sysprint("[APP_UDS] --- System Date and Time ---\n");

			int ST=TIMESYNC_NTP; // Sync Type 0:Computer(Manual), 1:NTP
			int DS=0;            // Daylight Savings 0:disable, 1:enable
			char rcvDateTime[20]={0}; // "YYYY-MM-DD HH:MM:SS" 19c
			int TZ;                   // time zone , added -12 ~ +14 
			int timezone;             // This is 12 plus the TZ for app_set

			memset(rbuf, 0, sizeof rbuf); // must be 128
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("read Data, ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0)
			{
				sscanf(rbuf, "ST=%d,DS=%d,TZ=%d,DateTime=%19c", 
						&ST, &DS, &TZ, rcvDateTime);
				DBG_UDS("ST=%d, DS:%d, DateTime:%s, TimeZone:%d\n", ST, DS, rcvDateTime, TZ);
				// set timezone, daylight
				timezone = TZ + 12;
				if(timezone != app_set->time_info.time_zone 
						|| DS != app_set->time_info.daylight_saving){
					app_set->time_info.daylight_saving = DS;
					app_set->time_info.time_zone       = timezone;
					set_time_zone();
				}

				// set datetime
				app_set->time_info.timesync_type = ST;
				if( ST == TIMESYNC_MANUAL ){
					set_time_manual_bystring(rcvDateTime);
				}
				else {
					set_time_by_ntp();
				}
			} else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read:"); 
			}

		}
		else if (strcmp(rbuf, "FWUPDATE") == 0)
		{
			sysprint("[APP_CGI] --- FWUPDATE ---\n");

			if (app_cfg->ste.b.ftp_run)
			{
				sprintf(wbuf, "FTP_RUNNING");
				ret = write(cs_uds, wbuf, sizeof wbuf);
			}
			else
			{
				// 1. send READY
				sprintf(wbuf, "READY");
				ret = write(cs_uds, wbuf, sizeof wbuf);

				if (ret > 0)
				{
					// read file path
					memset(rbuf, 0, sizeof rbuf);
					ret = read(cs_uds, rbuf, sizeof rbuf);
					DBG_UDS("FWUPDATE ret:%d, rbuf:%s\n", ret, rbuf);
					if (ret > 0)
					{
						if (access(rbuf, F_OK) != 0)
						{
							DBG_UDS("No FW File:%s\n", rbuf);
							sprintf(wbuf, "NO_FILE");
							ret = write(cs_uds, wbuf, sizeof wbuf);
						}
						else
						{
							ret = ctrl_update_firmware_by_cgi(rbuf);
							//ctrl_update_firmware_by_cgi("/tmp");
							if (ret == 0)
							{
								sprintf(wbuf, "SUCCEED");
								ret = write(cs_uds, wbuf, sizeof wbuf);
								DBG_UDS("OK, Succeed Update\n");
							}
							else {
								sprintf(wbuf, "INVALID_FILE");
								ret = write(cs_uds, wbuf, sizeof wbuf);
								DBG_UDS("Error, Failed Update\n");
							}
						}
					}
					else
					{
						DBG_UDS("ret:%d, ", ret);
						perror("failed read:");

						sprintf(wbuf, "NETWORK_ERROR");
						ret = write(cs_uds, wbuf, sizeof wbuf);
					}
				}
				else
				{
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "SystemRestore") == 0)
		{
			sysprint("[APP_UDS] --- System Restore ---\n");
			//app_restore(); //TODO
		}
		else if (strcmp(rbuf, "SystemReboot") == 0)
		{
			sysprint("[APP_UDS] --- System Reboot ---\n");
			ctrl_sys_halt(0); /* reboot */
			sleep(10); // client에 응답을 주지 않는다. Restarting....메시지 표시 때문에...
		}
		else if (strcmp(rbuf, "reload_config") == 0)
		{
		}
				
		else if (strcmp(rbuf, "GetVoipConfiguration") == 0) {
			sysprint("[APP_UDS] --- GetVoipConfiguration ---\n");

			T_CGI_VOIP_CONFIG t;memset(&t,0, sizeof t);
			if(0 == getVoipConfiguration(&t)){
				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);
				if (ret > 0) {

					// TODO something...
				}
				else { 
					DBG_UDS("ret:%d, ", ret);
					perror("failed sent:"); 
				}
			}
		}
		else if (strcmp(rbuf, "SetVoipConfiguration") == 0) {
			sysprint("[APP_UDS] --- SetVoipConfiguration ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				// 2. read data
				T_CGI_VOIP_CONFIG t;
				ret = read(cs_uds, &t, sizeof t);
				DBG_UDS("Read, size=%d\n", ret);
				if(ret > 0){
					ret = setVoipConfiguration(&t);

					char str[128] = "OK";
					if(ret == 0)
						sprintf(str, "%s", "NO CHANGE");
					else if(ret < 0)
						sprintf(str, "%s", "ERROR");
					ret = write(cs_uds, str, sizeof str);
				}
				else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read: "); 
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetServersConfiguration") == 0) {
			sysprint("[APP_UDS] --- GetServersConfiguration ---\n");

			T_CGI_SERVERS_CONFIG t;memset(&t,0, sizeof t);
			if(0 == getServersConfiguration(&t)){
				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);
				if (ret > 0) {

					// TODO something...
				}
				else { 
					DBG_UDS("ret:%d, ", ret);
					perror("failed sent:"); 
				}
			}
		}
		else if (strcmp(rbuf, "SetServersConfiguration") == 0) {
			sysprint("[APP_UDS] --- SetServersConfiguration ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				// 2. read data
				T_CGI_SERVERS_CONFIG t;
				ret = read(cs_uds, &t, sizeof t);
				DBG_UDS("Read, size=%d\n", ret);
				if(ret > 0){
					ret = setServersConfiguration(&t);

					char strOptions[128] = "OK";
					if(ret == 0)
						sprintf(strOptions, "%s", "NO CHANGE");
					else if(ret < 0)
						sprintf(strOptions, "%s", "ERROR");
					ret = write(cs_uds, strOptions, sizeof strOptions);
				}
				else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read: "); 
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetUserConfiguration") == 0) {
			sysprint("[APP_UDS] --- GetUserconfiguration ---\n");

			T_CGI_USER_CONFIG t;memset(&t,0, sizeof t);
			if(0 == getUserConfiguration(&t)){
				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);
				if (ret > 0) {

					// TODO something...
				} else {
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "SetUserConfiguration") == 0) {
			sysprint("[APP_UDS] --- SetUserConfiguration ---\n");

			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);
			if(ret > 0){
				T_CGI_USER_CONFIG t; memset(&t, 0, sizeof t);
				ret = read(cs_uds, &t, sizeof t);
				DBG_UDS("Read T_CGI_USER_CONFIG, size=%d\n", ret);
				if(ret > 0){
					ret = setUserConfiguration(&t);

					char strOptions[128] = "OK";
					if(ret == 0)
						sprintf(strOptions, "%s", "NO CHANGE");
					else if(ret < 0){
						// maybe does not occur
						sprintf(strOptions, "%s", "ERROR");
					}
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetSystemInfo") == 0) {
			T_CGI_SYSTEM_INFO t;memset(&t,0, sizeof t);
			if(0 == getSystemInfo(&t)){
				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);
				if (ret > 0) {
					DBG_UDS("model:%s\n", t.model);
					DBG_UDS("fwver:%s\n", t.fwver);
				} else {
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "GetSystemConfiguration") == 0) {
			sysprint("[APP_UDS] --- GetSystemconfiguration ---\n");

			T_CGI_SYSTEM_CONFIG t;memset(&t,0, sizeof t);
			if(0 == getSystemConfiguration(&t)){
				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);
				if (ret > 0) {
					// TODO something...
				} else {
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "SetSystemConfiguration") == 0) {
			sysprint("[APP_UDS] --- SetSystemConfiguration ---\n");

			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);
			if(ret > 0){
				T_CGI_SYSTEM_CONFIG t; memset(&t, 0, sizeof t);
				ret = read(cs_uds, &t, sizeof(T_CGI_SYSTEM_CONFIG));
				DBG_UDS("Read, size=%d\n", ret);
				if(ret > 0){
					ret = setSystemConfiguration(&t);

					char strOptions[128] = "OK";
					if(ret == 0)
						sprintf(strOptions, "%s", "NO CHANGE");
					else if(ret < 0){
						// maybe does not occur
						sprintf(strOptions, "%s", "ERROR");
					}
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetNetworkConfiguration2") == 0)
		{
			//sprintf(wbuf, "[APP_UDS] --- GetNetworkConfiguration2 ---");
			{
				int i;
				/* start init */
				T_CGI_NETWORK_CONFIG2 t;
				memset(&t, 0, sizeof t);
				T_NETWORK_INFO wireless, cradle;
				char rtsp_user[32] = {0};
				char rtsp_pass[32] = {0};

				wireless.dhcp = NET_TYPE_STATIC;
				sprintf(wireless.ipaddr, "192.168.0.252");
				sprintf(wireless.gateway, "192.168.0.1");
				sprintf(wireless.netmask, "255.255.0.0");
				cradle.dhcp = NET_TYPE_STATIC;
				sprintf(cradle.ipaddr, "192.168.1.252");
				sprintf(cradle.gateway, "192.168.1.1");
				sprintf(cradle.netmask, "255.255.0.0");
				/* end init */

				wireless.dhcp = app_set->net_info.wtype;
				if (wireless.dhcp)
				{
					getNetworkInfo("wlan0", &wireless);
				}
				else
				{
					strcpy(wireless.ipaddr, app_set->net_info.wlan_ipaddr);
					strcpy(wireless.gateway, app_set->net_info.wlan_gateway);
					strcpy(wireless.netmask, app_set->net_info.wlan_netmask);
				}

				cradle.dhcp = app_set->net_info.type;
				if (cradle.dhcp)
				{
					getNetworkInfo("eth0", &cradle);
				}
				else
				{
					strcpy(cradle.ipaddr, app_set->net_info.eth_ipaddr);
					strcpy(cradle.gateway, app_set->net_info.eth_gateway);
					strcpy(cradle.netmask, app_set->net_info.eth_netmask);
				}

				if (app_set->account_info.enctype) // if AES
				{
					decrypt_aes(app_set->account_info.rtsp_userid, rtsp_user, 32);
					decrypt_aes(app_set->account_info.rtsp_passwd, rtsp_pass, 32);
				}
				else
				{
					strcpy(rtsp_user, app_set->account_info.rtsp_userid);
					strcpy(rtsp_pass, app_set->account_info.rtsp_passwd);
				}
				DBG_UDS("rtsp_user=%s\n", rtsp_user);
				DBG_UDS("rtsp_pass=%s\n", rtsp_pass);

				t.wifi_ap_multi = app_set->multi_ap.ON_OFF;
				t.wireless.addr_type = wireless.dhcp;
				strcpy(t.wireless.ipv4, wireless.ipaddr);
				strcpy(t.wireless.gw, wireless.gateway);
				strcpy(t.wireless.mask, wireless.netmask);
				t.cradle.addr_type = cradle.dhcp;
				strcpy(t.cradle.ipv4, cradle.ipaddr);
				strcpy(t.cradle.gw, cradle.gateway);
				strcpy(t.cradle.mask, cradle.netmask);

				// needs modify
				strcpy(t.wifi_ap.id, app_set->wifiap.ssid);
				strcpy(t.wifi_ap.pw, app_set->wifiap.pwd);

				for(i = 0 ; i < 4 ; i++)
				{
				    strcpy(t.wifi_ap_list[i].id, app_set->wifilist[i].ssid);
				    strcpy(t.wifi_ap_list[i].pw, app_set->wifilist[i].pwd);
				}

				t.live_stream_account_enable = app_set->account_info.ON_OFF;
				t.live_stream_account_enctype = app_set->account_info.enctype;
				strcpy(t.live_stream_account.id, rtsp_user);
				strcpy(t.live_stream_account.pw, rtsp_pass);

				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);

				if (ret > 0)
				{

					// TODO something...
				}
				else
				{
					DBG_UDS("ret:%d, ", ret);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "SetNetworkConfiguration2") == 0 ) {
			sysprint("[APP_UDS] --- SetNetworkConfiguration2 ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_NETWORK_CONFIG2 t;
				memset(&t, 0, sizeof t);
				ret = read(cs_uds, &t, sizeof t);
				DBG_UDS("Read, size=%d\n", ret);
				if(ret > 0){
					ret = setNetworkConfiguration2(&t);

					char strOptions[128] = "OK";
					if(ret == 0)
						sprintf(strOptions, "%s", "NO CHANGE");
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetNetworkConfiguration") == 0) {
			//sysprint("[APP_UDS] --- GetNetworkConfiguration ---\n");
			{
				/* start init */
				T_CGI_NETWORK_CONFIG t;
				memset(&t, 0, sizeof t);
				T_NETWORK_INFO wireless,cradle;
				char rtsp_user[32]={0};
				char rtsp_pass[32]={0};
				wireless.dhcp = NET_TYPE_STATIC;
				sprintf(wireless.ipaddr, "192.168.0.252");
				sprintf(wireless.gateway, "192.168.0.1");
				sprintf(wireless.netmask, "255.255.0.0");
				cradle.dhcp = NET_TYPE_STATIC;
				sprintf(cradle.ipaddr, "192.168.1.252");
				sprintf(cradle.gateway, "192.168.1.1");
				sprintf(cradle.netmask, "255.255.0.0");
				/* end init */

				wireless.dhcp = app_set->net_info.wtype;
				if(wireless.dhcp){
					getNetworkInfo("wlan0",  &wireless);
				}
				else {
					strcpy(wireless.ipaddr, app_set->net_info.wlan_ipaddr);
					strcpy(wireless.gateway, app_set->net_info.wlan_gateway);
					strcpy(wireless.netmask, app_set->net_info.wlan_netmask);
				}

				cradle.dhcp = app_set->net_info.type;
				if(cradle.dhcp){
					getNetworkInfo("eth0",  &cradle);
				}
				else {
					strcpy(cradle.ipaddr,  app_set->net_info.eth_ipaddr);
					strcpy(cradle.gateway, app_set->net_info.eth_gateway);
					strcpy(cradle.netmask, app_set->net_info.eth_netmask);
				}

				if(app_set->account_info.enctype) // if AES
				{
					decrypt_aes(app_set->account_info.rtsp_userid, rtsp_user, 32);
					decrypt_aes(app_set->account_info.rtsp_passwd, rtsp_pass, 32);
				}
				else {
					strcpy(rtsp_user, app_set->account_info.rtsp_userid);
					strcpy(rtsp_pass, app_set->account_info.rtsp_passwd);
				}
				DBG_UDS("rtsp_user=%s\n", rtsp_user);
				DBG_UDS("rtsp_pass=%s\n", rtsp_pass);

				t.wireless.addr_type  = wireless.dhcp;
				strcpy(t.wireless.ipv4, wireless.ipaddr);
				strcpy(t.wireless.gw,   wireless.gateway);
				strcpy(t.wireless.mask, wireless.netmask);
				t.cradle.addr_type  = cradle.dhcp;
				strcpy(t.cradle.ipv4, cradle.ipaddr);
				strcpy(t.cradle.gw,   cradle.gateway);
				strcpy(t.cradle.mask, cradle.netmask);
				
				// needs modify
				strcpy(t.wifi_ap.id, app_set->wifiap.ssid);
				strcpy(t.wifi_ap.pw, app_set->wifiap.pwd);
				/*
				needs modify 
				for(i = 0 ; i < 5 ; i++)
				{
				    strcpy(t.wifi_list[i].id, app_set->wifilist[i].ssid);
				    strcpy(t.wifi_list[i].pw, app_set->wifilist[i].pwd);
				}
				*/
				
				t.live_stream_account_enable  = app_set->account_info.ON_OFF;
				t.live_stream_account_enctype = app_set->account_info.enctype;
				strcpy(t.live_stream_account.id, rtsp_user);
				strcpy(t.live_stream_account.pw, rtsp_pass);

				ret = write(cs_uds, &t, sizeof t);
				DBG_UDS("sent, ret=%d \n", ret);

				if (ret > 0) {

					// TODO something...
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed write: ");
				}
			}
		}
		else if (strcmp(rbuf, "SetNetworkConfiguration") == 0 ) {
			sysprint("[APP_UDS] --- SetNetworkConfiguration ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_NETWORK_CONFIG t;
				memset(&t, 0, sizeof t);
				ret = read(cs_uds, &t, sizeof t);
				DBG_UDS("Read, size=%d\n", ret);
				if(ret > 0){
					ret = setNetworkConfiguration(&t);

					char strOptions[128] = "OK";
					if(ret == 0)
						sprintf(strOptions, "%s", "NO CHANGE");
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetVideoQuality") == 0){
			//sysprint("[APP_UDS] --- GetVideoQuality ---\n");

			T_CGI_VIDEO_QUALITY p; memset(&p, 0, sizeof(p));

			// get recording inforamtion

			p.rec.res = getResolutionIdx(0);
			p.rec.fps = getFps(0);
			p.rec.bps = getKbps(0);
			p.rec.gop = getGop (0);
			p.rec.rc  = getRC  (0);

			// get streaming information
			p.stm.res = getResolutionIdx(STM_CH_NUM);
			p.stm.fps = getFps (STM_CH_NUM);
			p.stm.bps = getKbps(STM_CH_NUM);
			p.stm.gop = getGop (STM_CH_NUM);
			p.stm.rc  = getRC  (STM_CH_NUM);

			ret = write(cs_uds, &p, sizeof p);
			if (ret > 0) {
				// TODO something...
				DBG_UDS("sent, ret=%d\n", ret);
			} else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "SetVideoQuality") == 0){
			//sysprint("[APP_UDS] --- SetVideoQuality ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_VIDEO_QUALITY p; memset(&p, 0, sizeof(p));
				ret = read(cs_uds, &p, sizeof p);
				DBG_UDS("read T_CGI_VIDEO_QUALITY, ret=%d\n", ret);
				if(ret > 0){
					setVideoQuality(p.rec.fps, p.rec.bps, p.rec.gop, p.rec.rc,
							p.stm.res, p.stm.fps, p.stm.bps, p.stm.gop, p.stm.rc);

					char strOptions[128] = "OK"; // Send DONE
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("failed read T_CGI_VIDEO_QUALITY, ret:%d, ", ret);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "SetDynVideoQuality") == 0){
			//sysprint("[APP_UDS] --- SetDynVideoQuality ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_VIDEO_QUALITY p; memset(&p, 0, sizeof(p));
				ret = read(cs_uds, &p, sizeof p);
				DBG_UDS("read Dynamic T_CGI_VIDEO_QUALITY, ret=%d\n", ret);
				if(ret > 0){
					setDynamicVideoQuality(p.rec.fps, p.rec.bps, p.rec.gop, p.rec.rc,
							p.stm.res, p.stm.fps, p.stm.bps, p.stm.gop, p.stm.rc);

					char strOptions[128] = "OK"; // Send DONE
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("failed read Dynamic T_CGI_VIDEO_QUALITY, ret:%d, ", ret);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "GetVideoEncoderConfiguration") == 0){
			// Onvifserver uses this command
			//sysprint("[APP_UDS] --- GetVideoEncoderConfiguration---\n");

			// Read Encoding Type
			ret = read(cs_uds, rbuf, sizeof rbuf);
			if(ret > 0){
				DBG_UDS("ret:%d, rbuf:%s\n", ret, rbuf);
				int VideoEncoding=atoi(rbuf); // 0:jpeg, 2:h264

				char strOptions[128] = {0};
				int w=0, h=0;
				int kbps=0;
				int fps=0, ei=0, gop=0;
				onvif_getResolution(VideoEncoding, &w,&h);
				kbps = getKbps(STM_CH_NUM);
				gop  = getGop( STM_CH_NUM);
				ei   = 1; // Fixed, ei means, Interval at which images are encoded and transmitted. 
				          // (A value of 1 means that every frame is encoded, a value of 2 means that every 2nd frame is encoded ...)
				fps  = getFps(STM_CH_NUM);
				sprintf(strOptions, "%d %d %d %d %d %d", 
						w, h, kbps, fps, ei, gop);

				ret = write(cs_uds, strOptions, sizeof strOptions);
				if (ret > 0) {
					// TODO something...
					DBG_UDS("sent, ret=%d, strOptions:%s\n", ret, strOptions);
				}else {
					perror("failed write: ");
				}
			} else {
				DBG_UDS("ret:%d\n", ret);
				perror("failed read: ");
			}
		}
		else if (strcmp(rbuf, "GetOperationConfiguration") == 0){
			//sysprint("[APP_UDS] --- GetOperationConfiguration---\n");

			DBG_UDS("ret:%d, rbuf:%s\n", ret, rbuf);
			char strOptions[256] = {0};
			{
				sprintf(strOptions, "%d %d %d %d %d %d",
						app_set->rec_info.pre_rec,     // 0:on, 1:off
						app_set->rec_info.auto_rec,    // 0:on, 1:off
						app_set->rec_info.audio_rec,   // 0:on, 1:off
						app_set->rec_info.period_idx,  // 0:1MIN, 1:5MIN
						app_set->rec_info.overwrite,
						app_set->sys_info.osd_set);
			}
			ret = write(cs_uds, strOptions, sizeof strOptions);
			if (ret > 0) {
				// TODO something...
				DBG_UDS("sent, ret=%d, strOptions:%s\n", ret, strOptions);
			} else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "SetOperationConfiguration") == 0){
			//sprintf("[APP_UDS] --- SetOperationConfig ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				ret = read(cs_uds, rbuf, sizeof rbuf);
				DBG_UDS("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){
					int pre_rec=0, auto_rec=0, audio_rec=0, rec_interval=0, rec_overwrite=0;
					int display_datetime=0;
					sscanf(rbuf, "%d %d %d %d %d %d", 
							&pre_rec,
							&auto_rec,
							&audio_rec,
							&rec_interval,
							&rec_overwrite,
							&display_datetime);
					setRecordOptions(pre_rec, auto_rec, audio_rec, rec_interval, rec_overwrite);
					setDisplayDateTime(display_datetime);

					char strOptions[128] = "OK";
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "SetVideoEncoderConfiguration") == 0) // streaming from ONVIF
		{
			//OnvifServer uses this command
			sprintf(wbuf, "[APP_UDS] --- SetVideoEncoderConfiguration---");
			memset(rbuf, 0, sizeof rbuf);
			ret = read(cs_uds, rbuf, sizeof rbuf);
			DBG_UDS("ret:%d, rbuf:%s\n", ret, rbuf);
			if (ret > 0)
			{
				int encoding=-1; // 0:JPEG, 2:H264
				int rcv_width, rcv_height, rcv_kbps, rcv_fps, rcv_ei, rcv_gov; // if these have zero, do not need to set. ei means encodingInterval

				// ENCType width height kbps fps ei gov
				sscanf(rbuf, "%d %d %d %d %d %d %d", 
					&encoding, &rcv_width, &rcv_height, &rcv_kbps, &rcv_fps, &rcv_ei, &rcv_gov);
				DBG_UDS("SetVideoEncoderConfiguration:Enc:%d, width:%d,height=%d,kbps=%d,fps=%d,ei=%d,gov=%d\n", 
					encoding, rcv_width, rcv_height, rcv_kbps, rcv_fps, rcv_ei, rcv_gov);

				// fps == gov
				onvif_setVideoEncoderConfiguration(encoding, rcv_width, rcv_height, rcv_kbps, rcv_fps, rcv_ei, rcv_fps);

			} else {
				DBG_UDS("ret:%d, ", ret);
				perror("failed read: ");
			}
		}
		else if (strcmp(rbuf, "CheckAccount") == 0)
		{
			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_ACCOUNT acc;
				ret = read(cs_uds, &acc, sizeof(acc));
				if(ret > 0){
					char strOptions[128] = "OK";
					if(0==checkAccount(&acc)){
					}
					else {
						sprintf(strOptions, "%s", "INVALID ACCOUNT");
					}

					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed write: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "UpdateUser") == 0)
		{
			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_USER user;

				// 1. read user info
				ret = read(cs_uds, &user, sizeof(user));
				if(ret > 0){

					char strOptions[128] = "OK";
					// 2. update user info
					if(0==updateUser(&user)){
						// Just save changes, this settings will adjust after system restart.
					}
					else {
						sprintf(strOptions, "%s", "ERROR UPDATE USER");
					}

					// 3. send result
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed write: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "UpdateOnvifUser") == 0)
		{
			sysprint("[APP_UDS] --- UpdateOnvifUser ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_ONVIF_USER user;

				// 1. read user info
				ret = read(cs_uds, &user, sizeof(user));
				if(ret > 0){

					char strOptions[128] = "OK";
					// 2. update user info
					if(0==updateOnvifUser(&user)){
						// Just save changes, this settings will adjust after system restart.
					}
					else {
						sprintf(strOptions, "%s", "ERROR UPDATE ONVIFUSER");
					}

					// 3. send result
					ret = write(cs_uds, strOptions, sizeof strOptions);
				} else {
					DBG_UDS("ret:%d, ", ret);
					perror("failed write: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else if (strcmp(rbuf, "ChangePassword") == 0)
		{
			sysprint("[APP_UDS] --- ChangePassword ---\n");

			// 1. send READY
			sprintf(wbuf, "READY");
			ret = write(cs_uds, wbuf, sizeof wbuf);

			if(ret > 0){
				T_CGI_USER user;
				memset(&user, 0, sizeof user);

				// 1. read user info
				ret = read(cs_uds, &user, sizeof(T_CGI_USER));
				DBG_UDS("ret:%d\n", ret);
				if (ret > 0){

					DBG_UDS("id:%s, pw:%s, lv:%d, authtype=%d\n", user.id, user.pw, user.lv, user.authtype);

					if(0 == app_set_web_password(user.id, user.pw, user.lv, user.authtype))
					{
						if( 0 == app_web_make_passwordfile(user.id, user.pw, user.lv, user.authtype)){

							char strOptions[128] = "OK";
							ret = write(cs_uds, strOptions, sizeof(strOptions));
							DBG_UDS("restart web server...ret:%d\n", ret);
							app_web_restart_server();
							DBG_UDS("done restart web server...\n");
						}

					}
					// 1. make a password file (basic or digest)
					// 2. save user info to system_cfg file
					perror("failed read: ");
				}
			} else {
				DBG_UDS("ret:%d, cs:%d", ret, cs_uds);
				perror("failed write: ");
			}
		}
		else {
			DBG_UDS("Invaild Command %s\n", rbuf);
		}

	}while(0);

	shutdown(cs_uds, SHUT_RDWR);
	close(cs_uds);

	return (void *)0;
}

void * uds_syscmd_thread(void * arg)
{
	int listenfd;
	int cs_uds;				///< client socket
	socklen_t cs_len;
	int s_state;
	struct sockaddr_un cs_addr;
	struct sockaddr_un servaddr;

	//int err_cnt=0;

	// internet 기반의 스트림 소켓을 만들도록 한다.
	if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}
	//if (access(FILE_UDS_SYSTEM, F_OK) == 0)	{ }
	unlink(FILE_UDS_SYSTEM); // MUST ?

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path, FILE_UDS_SYSTEM);

	s_state = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (s_state == -1) {
		perror("bind error : ");
		exit(0);
	}

	s_state = listen(listenfd, 1/*LISTENQ*/);
	if (s_state == -1) {
		perror("listen error : ");
		exit(0);
	}

	while (g_thd_running)
	{
		cs_uds = accept(listenfd, (struct sockaddr *)&cs_addr, &cs_len);
		DBG_UDS("accept ... cs=%d, len=%d\n", cs_uds, cs_len);

		if(cs_uds == -1){
			perror("Accept error : ");
			sleep(1);
		} else {

#if 0
			int status = pthread_create(&tid, NULL, myFunc, (void *)&cs_uds);
			if(status != 0)
			{
				perror("Thread Create Error: ");
				break;// return 1;
			}else {
				pthread_detach(tid);
			}
#else
			myFunc(&cs_uds);
#endif
		}
		//usleep(30000);

	}// while(g_thd_running)
	
	close(listenfd);

	g_tid = 0;

	return NULL;
}

int app_uds_start()
{
	g_thd_running = 1;
	g_tid  = 0;

	int ret = pthread_create(&g_tid, NULL, uds_syscmd_thread, NULL);
	if (ret != 0) {
		__E("pthread_create failed, ret = %d\r\n", ret);
		g_thd_running = 0;
		return -1;
	}
	pthread_setname_np(g_tid, __FILENAME__);
	pthread_detach(g_tid);

	return 0;
}

int app_uds_stop()
{
	g_thd_running = 0;

	int			wbytes=0;
	char		wbuf[128]={0};

	struct sockaddr_un	caddr;		// client address

	int	clen=0;		// client length
	int cs = socket(AF_UNIX, SOCK_STREAM, 0); // client socket
	if(cs < 0 ) {
		perror("socket : ");
		return -1;
	}

	bzero(&caddr, sizeof(caddr));
	caddr.sun_family = AF_UNIX;
	strcpy(caddr.sun_path, FILE_UDS_SYSTEM);
	clen = sizeof(caddr);
	if(connect(cs, (struct sockaddr *)&caddr, clen) < 0) {
		perror("Connect UDS : ");
		return -1;
	}

	sprintf(wbuf, "%s", "eXit");
	wbytes=write(cs, wbuf, strlen(wbuf)+1);
	printf("Write UDS : cs=(%d), written bytes = %d \n", cs, wbytes);

	close(cs);

	while (g_tid != 0)
	{
		usleep(1000);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//EOF
