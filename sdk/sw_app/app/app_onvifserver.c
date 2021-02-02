/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_onvifserver.c
 * @brief
 * @author  BKKIM
 * @section MODIFY history
 *     - 2018.12.17 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "app_onvifserver.h"
#include "app_uds.h"
#include "app_main.h"
#include "app_log.h"
#include "app_rec.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_ctrl.h"
#include "app_rtsptx.h"
#include "app_timesrv.h"
#include "app_p2p.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#ifndef APPNAME
#define APPNAME "onvifserver"
#endif
#ifndef APPEXEC
#define APPEXEC "./bin/"APPNAME"&"
#endif
#ifndef APPSTOP
#define APPSTOP "killall "APPNAME
#endif

#define ONVIF_CFG_PATH "/tmp/onvifconfig.xml"

#define ONVIFSERVER_READY       0
#define ONVIFSERVER_LOADED        1

#define ENABLED_POPEN 0

/*************************************************************************/
#define ENABLE_ONVIF_DEBUG 1
#if ENABLE_ONVIF_DEBUG
#define __D(fmt, args...) {fprintf(stderr, "[APP_ONVIF_DBG] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args);}
#define __D_FUNC_ENTER { __D("[ONVIFS] Enter >>>>>>>>>\n");}
#else
#define __D(fmt, args...) {}
#define __D_FUNC_ENTER {}
#endif//ENABLE_ONVIF_DEBUG
#define __E(fmt, args...) {fprintf(stderr, "[APP_ONVIF_ERR] %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args);}
#define DBG_ONVIF __D
#define DBG_ENTER __D_FUNC_ENTER
/*************************************************************************/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static unsigned int	g_running;
static pthread_t  	g_tid;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
pthread_t sys_os_create_thread(void * thread_func, void * argv)
{
	pthread_t tid = 0;

#if	(__VXWORKS_OS__ || __LINUX_OS__)

	int ret = pthread_create(&tid,NULL,(void *(*)(void *))thread_func,argv);
	if (ret != 0)
	{
		log_print(LOG_ERR, "sys_os_create_thread::pthread_create failed, ret = %d\r\n", ret);
	}

	pthread_detach(tid);

#elif __WIN32_OS__

	HANDLE hret = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_func, argv, 0, &tid);
	if (hret == NULL || tid == 0)
	{
		log_print(LOG_ERR, "sys_os_create_thread::CreateThread hret=%u, tid=%u, err=%u\r\n", hret, tid, GetLastError());
	}

	CloseHandle(hret);

#endif

	return tid;
}

/*
 * return if 1, succeed
 */
static int  onvif_is_wireless_up()
{
    int i, retval=0;    
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

		if ((ifr->ifr_flags & IFF_UP) == 0) {
			ifr++; continue;
		}

		if(0 != strcmp(ifr->ifr_name, "eth0")) {
			struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
			DBG_ONVIF("check upped wireless device : %s, %s\n", ifr->ifr_name, inet_ntoa(sin->sin_addr));
			retval = 1;
			break;
		}
		
		ifr++;
	}

	close(socket_fd);

	return retval;  // if 1 , upped, else down
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

	DBG_ONVIF("%s:%s\n", devName, out_gw);
	
	return ret;
}

// devName에 맞은 인터페이스에서 네트워크 정보를 가져온다.
static int onvif_get_net_info(const char *devName, ONVIF_NET_INFO * out_info)
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
			
			DBG_ONVIF("%s : ipaddr:%s, netmask:%s, gateway:%s\n", devName, out_info->ipaddr, out_info->netmask, out_info->gateway);

			return 0; // succeed
		}
		
		ifr++;
	}

	return -1;
}

// You must free the result if result is non-NULL.
static char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) // warning: suggest parentheses around assignment used as truth value
	{
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

/*****************************************************************************
* @brief    create onvif config.xml
* @section  DESC Description
*   - refer main config
* @return 0 if creating file succeed,  other value if failed
*****************************************************************************/
int app_onvif_init_config()
{
	FILE *fp = fopen(ONVIF_CFG_PATH, "w");
	if(fp){
		char str[255];

		fputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n", fp);
		fputs("<config>\n", fp);

		// servver information
		fputs("	<server_ip></server_ip>\n", fp);
        sprintf(str,"     <server_port>%d</server_port>\n",app_set->net_info.onvif_port) ; 
		fputs(str, fp);
		fputs("	<http_max_users>16</http_max_users>\n", fp);
		fputs("	<https_enable>0</https_enable>\n", fp);
		fputs("	<need_auth>1</need_auth>\n", fp);
		fputs("	<log_enable>0</log_enable>\n", fp); // debug log file. ipsee.txt

		// product information
		fputs("	<information>\n", fp);
		fputs("		<Manufacturer>LinkFlow</Manufacturer>\n", fp);
#if defined(NEXXONE)
		fputs("		<Model>NEXX ONE</Model>\n", fp);
#elif defined(NEXX360B)
		fputs("		<Model>NEXX360B</Model>\n", fp);
#elif defined(NEXX360W)
		fputs("		<Model>NEXX360W</Model>\n", fp);
#elif defined(NEXX360H)
		fputs("		<Model>NEXX360H</Model>\n", fp);
#elif defined(FITT360_SECURITY)
		fputs("		<Model>FITT360 Security</Model>\n", fp);
#else
#error "Check Model Name"
#endif
		{            
			sprintf(str, "		<FirmwareVersion>%s</FirmwareVersion>\n", app_set->sys_info.fw_ver);
			fputs(str, fp);
			sprintf(str, "		<HardwareId>%s</HardwareId>\n", app_set->sys_info.hw_ver);
			fputs(str, fp);
			sprintf(str, "		<SerialNumber>%s</SerialNumber>\n", app_set->sys_info.deviceId);
			fputs(str, fp);
		}
		fputs("	</information>\n", fp);

		// user settings, Set default only, 
		{

			fputs("	<user>\n", fp);
			fputs("		<username>admin</username>\n", fp); // Fixed admin
			sprintf(str, "	<password>%s</password>\n", app_set->account_info.onvif.pw);
			fputs(str, fp);
			fputs("		<userlevel>Administrator</userlevel>\n", fp);
			fputs("	</user>\n", fp);
		}

		// profile1 1280x720
		fputs("<profile>\n", fp);
		fputs("		<video_source>\n", fp);
		fputs("			<width>1280</width>\n", fp);
		fputs("			<height>720</height>\n", fp);
		fputs("		</video_source>\n", fp);
		fputs("		<video_encoder>\n", fp);
		fputs("			<width>1280</width>\n", fp);
		fputs("			<height>720</height>\n", fp);
		fputs("			<quality>5</quality>\n", fp);
		fputs("			<session_timeout>60</session_timeout>\n", fp);
#if defined(NEXXONE) || defined(NEXX360H)
		fputs("			<framerate>30</framerate>\n", fp);
#elif defined(NEXX360B) || defined(NEXX360W) 
		fputs("			<framerate>15</framerate>\n", fp);
#elif defined(FITT360_SECURITY)
		fputs("			<framerate>15</framerate>\n", fp);
#endif
		fputs("			<bitrate_limit>8000</bitrate_limit>\n", fp);
		fputs("			<encoding_interval>1</encoding_interval>\n", fp);
		fputs("			<encoding>H264</encoding>\n", fp);
		fputs("			<h264>\n", fp);
#if defined(NEXXONE) || defined(NEXX360H)
        fputs("				<gov_length>30</gov_length>\n", fp);
#elif defined(NEXX360B) || defined(NEXX360W)
        fputs("				<gov_length>15</gov_length>\n", fp);
#elif defined(FITT360_SECURITY)
        fputs("				<gov_length>15</gov_length>\n", fp);
#endif
		fputs("				<h264_profile>High</h264_profile>\n", fp);
		fputs("			</h264>\n", fp);
		fputs("		</video_encoder>\n", fp);
#if 0 // not supported
		fputs("		<audio_source></audio_source>\n", fp);
		fputs("		<audio_encoder>\n", fp);
		fputs("			<session_timeout>30</session_timeout>\n", fp);
		fputs("			<sample_rate>16</sample_rate>\n", fp);
		fputs("			<bitrate>8</bitrate>\n", fp);
		fputs("			<encoding>PCMU</encoding>\n", fp);
		fputs("		</audio_encoder>\n", fp);
#endif
		{
			// TODO:BKKIM, Have to read streamer info
			if(onvif_is_wireless_up()){
				sprintf(str, "<stream_uri>rtsp://%s:%d/%s</stream_uri>\n", 
						app_set->net_info.wlan_ipaddr, app_set->net_info.rtsp_port, app_set->net_info.rtsp_name);
			}
			else {
				sprintf(str, "<stream_uri>rtsp://%s:%d/%s</stream_uri>\n", 
						app_set->net_info.eth_ipaddr, app_set->net_info.rtsp_port, app_set->net_info.rtsp_name);
			}
			fputs(str, fp);
		}
		fputs("	</profile>\n", fp);

		fputs("<profile>\n", fp); 
		fputs("		<video_source>\n", fp);
		fputs("			<width>1280</width>\n", fp);
		fputs("			<height>720</height>\n", fp);
		fputs("		</video_source>\n", fp);
		fputs("		<video_encoder>\n", fp);
		fputs("			<width>720</width>\n", fp);
		fputs("			<height>480</height>\n", fp);
		fputs("			<session_timeout>60</session_timeout>\n", fp);
#if defined(NEXXONE) || defined(NEXX360H)
		fputs("			<framerate>30</framerate>\n", fp);
#elif defined(NEXX360B) || defined(NEXX360W)
		fputs("			<framerate>15</framerate>\n", fp);
#elif defined(FITT360_SECURITY)
		fputs("			<framerate>15</framerate>\n", fp);
#endif
		fputs("			<bitrate_limit>4000</bitrate_limit>\n", fp);
		fputs("			<quality>5</quality>\n", fp);
		fputs("			<encoding_interval>1</encoding_interval>\n", fp);
#if 0 // H264
		fputs("			<encoding>H264</encoding>\n", fp);
		fputs("			<h264>\n", fp);
        fputs("				<gov_length>15</gov_length>\n", fp);
		fputs("				<h264_profile>High</h264_profile>\n", fp);
		fputs("			</h264>\n", fp);
#else // JPEG
		fputs("			<encoding>JPEG</encoding>\n", fp);
		fputs("			<jpeg></jpeg>\n", fp);
#endif
		fputs("		</video_encoder>\n", fp);
#if 0 // Audio not supported
		fputs("		<audio_source></audio_source>\n", fp);
		fputs("		<audio_encoder>\n", fp);
		fputs("			<session_timeout>30</session_timeout>\n", fp);
		fputs("			<sample_rate>16</sample_rate>\n", fp);
		fputs("			<bitrate>8</bitrate>\n", fp);
		fputs("			<encoding>PCMU</encoding>\n", fp);
		fputs("		</audio_encoder>\n", fp);
#endif
		{
			// TODO:BKKIM, Have to read streamer info
			if(onvif_is_wireless_up()){
				sprintf(str, "<stream_uri>rtsp://%s:8552/%s</stream_uri>\n", 
						app_set->net_info.wlan_ipaddr, app_set->net_info.rtsp_name);
			}
			else {
				sprintf(str, "<stream_uri>rtsp://%s:8552/%s</stream_uri>\n", 
						app_set->net_info.eth_ipaddr, app_set->net_info.rtsp_name);
			}
			fputs(str, fp);
		}
		fputs("	</profile>\n", fp);

		// scopes
		fputs("	<scope>onvif://www.onvif.org/Profile/Streaming</scope>\n", fp); // Profile S
		//fputs("	<scope>onvif://www.onvif.org/Profile/G</scope>\n", fp); // Profile G
		fputs("	<scope>onvif://www.onvif.org/location/country/korea</scope>\n", fp);
		fputs("	<scope>onvif://www.onvif.org/type/video_encoder</scope>\n", fp);

		if(strstr(MODEL_NAME, " ")){
			char org[32]={0};
			strcpy(org, MODEL_NAME);
			char *ret = str_replace(org, " ", "%20");
			if(ret){
				sprintf(str, " <scope>onvif://www.onvif.org/name/%s</scope>\n", ret);
				fputs(str, fp);
				sprintf(str, " <scope>onvif://www.onvif.org/hareware/%s</scope>\n", ret);
				fputs(str, fp);

				free(ret);
			}

		}else {
			sprintf(str, " <scope>onvif://www.onvif.org/name/%s</scope>\n", MODEL_NAME);
			fputs(str, fp);
			sprintf(str, " <scope>onvif://www.onvif.org/hareware/%s</scope>\n", MODEL_NAME);
			fputs(str, fp);
		}
		//fputs("	<scope>onvif://www.onvif.org/name/Fitt360&#32;Security</scope>\n", fp);
		//fputs("	<scope>onvif://www.onvif.org/hardware/Wearable360&#176;&#32;Camera</scope>\n", fp);

		// event
		fputs("	<event>\n", fp);
		fputs("		<renew_interval>60</renew_interval>\n", fp);
		fputs("		<simulate_enable>1</simulate_enable>\n", fp);
		fputs("		<simulate_interval>10</simulate_interval>\n", fp);
		fputs("	</event>\n", fp);

		fputs("</config>\n", fp);

		fclose(fp);
		return 0;
	}

	return -1;
}

void * onvifserver_thread(void * argv)
{
    int exit = FALSE, onvifserver_status = ONVIFSERVER_READY;
    int cradle_pre_status,  eth0_pre_status, eth0_cur_status;
	int usbnet_pre_status;
	
    cradle_pre_status = app_cfg->ste.b.cradle_eth_ready;
    eth0_pre_status   = app_cfg->ste.b.cradle_eth_run;
	usbnet_pre_status = app_cfg->ste.b.usbnet_run;

	// 초기화...
	while (1) {
		// USB Network Device (Wi-Fi / LTE / USB2Ether)
		if ((app_cfg->ste.b.usbnet_run) && onvifserver_status == ONVIFSERVER_READY && onvif_is_wireless_up()) {
			init_onvifserver();
			onvifserver_status = ONVIFSERVER_LOADED;
			break;
		} 
		// eth0
		else if (app_cfg->ste.b.cradle_eth_ready && onvifserver_status == ONVIFSERVER_READY) {
			init_onvifserver();
			onvifserver_status = ONVIFSERVER_LOADED ;
			break;
		}
		sleep(1);
	}

	// 상태변화에 따른 대응
    while (!exit)
    {
		// changed cradle status
        if (cradle_pre_status != app_cfg->ste.b.cradle_eth_ready) {
			cradle_pre_status = app_cfg->ste.b.cradle_eth_ready;
			onvifserver_status = ONVIFSERVER_READY;

			// USB Network connected..
		    if (app_cfg->ste.b.usbnet_run && onvifserver_status == ONVIFSERVER_READY && onvif_is_wireless_up()) {
				init_onvifserver() ;
				onvifserver_status = ONVIFSERVER_LOADED ;
			}

			// eth0
			if (app_cfg->ste.b.cradle_eth_ready && onvifserver_status == ONVIFSERVER_READY) {
				init_onvifserver() ;
				onvifserver_status = ONVIFSERVER_LOADED ;
			}
		}

		// changed usb network status
        if (usbnet_pre_status != app_cfg->ste.b.usbnet_run) {
			usbnet_pre_status = app_cfg->ste.b.usbnet_run;
			onvifserver_status = ONVIFSERVER_READY;

			// usb network connected
			if (app_cfg->ste.b.usbnet_run && onvifserver_status == ONVIFSERVER_READY && onvif_is_wireless_up()) {
				init_onvifserver() ;
				onvifserver_status = ONVIFSERVER_LOADED ;
			}
			else if (!app_cfg->ste.b.usbnet_run) {
				// eth0
				if (app_cfg->ste.b.cradle_eth_ready && onvifserver_status == ONVIFSERVER_READY) {
					init_onvifserver() ;
					onvifserver_status = ONVIFSERVER_LOADED ;
				}
			}
		}

		// changed ethernet status
		eth0_cur_status = app_cfg->ste.b.cradle_eth_run;
		if (app_cfg->ste.b.cradle_eth_ready && eth0_pre_status != eth0_cur_status) {
		    eth0_pre_status = eth0_cur_status ;
			init_onvifserver() ;
			onvifserver_status = ONVIFSERVER_LOADED ;
		}

        sleep(1) ;
    }
	return (void*)NULL;
}

/*****************************************************************************
* @brief    onvifserver start/stop function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_onvifserver_start(void)
{
	app_uds_start();

	g_running = 1;

	int ret = pthread_create(&g_tid,NULL, onvifserver_thread, NULL);
	if (ret != 0) {
		__E("pthread_create failed, ret = %d\r\n", ret);
		g_running= 0;
		return -1;
	}
	pthread_detach(g_tid);

	return 0;
}

int app_onvifserver_stop(void)
{
	app_uds_stop();

#if ENABLED_POPEN // Not work
	FILE *fd = popen(APPSTOP, "r");
	if(fd == NULL){
        DBG_ONVIF("failed popen, "APPSTOP"\n");
		return -1;
    }
	pclose(fd);
#else
    char cmd[255];
    int ret;
    sprintf(cmd, "%s", APPSTOP);
    ret = system(cmd);
    DBG_ONVIF("%s, ret = %d\n", cmd, ret);
#endif

    return 0;
}

int app_onvifserver_restart() // just app
{
	return init_onvifserver();
}

int app_onvifserver_restart_all() // app, thread and uds_thread
{
    app_onvifserver_stop();

    sleep(1) ;

    app_onvifserver_start();

    return 0 ;
}

int init_onvifserver()
{
    char cmd[255];
    int ret ;

    sprintf(cmd, "%s", APPSTOP);
    ret = system(cmd);
    DBG_ONVIF("%s, ret = %d\n", cmd, ret);

	app_onvif_init_config();

	// copy snapshot file to /tmp
	system("cp -f /opt/fit/bin/onvifsnapshot.jpg /tmp");

#if ENABLED_POPEN // Not work for well
	FILE *fd = popen(APPEXEC, "r");
	if (fd == NULL)
	{
		DBG_ONVIF("failed popen, " APPEXEC "\n");
		return -1;
	}
	pclose(fd);
#else
	sprintf(cmd, "%s", APPEXEC);
	if(app_set->net_info.enable_onvif)
		ret = system(cmd);
	DBG_ONVIF("%s, ret = %d\n", cmd, ret);
#endif

	return ret;
}

//EOF
