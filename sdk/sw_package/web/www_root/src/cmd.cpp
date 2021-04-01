/*
 * @file  cmd.cpp
 * @brief can receive system commands from web...
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

/*******************************************************************************
 * defines
 ******************************************************************************/
#define HOME_PATH "../index.html"

/*******************************************************************************
 * global variables
 ******************************************************************************/

T_CGI_VIDEO_QUALITY vq;

namespace NS_CMD {
	int gRefreshSec=1;
	int send_response(int errnum)
	{
#if 0
		PUT_CONTENT_TYPE_PLAIN;
		printf("(%d)\r\n", errnum);
#else
		printf("Cache-Control: no-cache, no-store\r\n"
		       "Content-type: application/json\r\n\r\n");
		
 		printf("{ return value: %d }", errnum);
#endif
	}
}

int get_vid_response(int retval)
{
	printf("\nCache-Control: no-cache, no-store\r\n"
		       "Content-type: application/json\r\n\r\n");

    printf("\"FPS\" :  %d\n",vq.stm.fps) ;
	printf("\"BPS\" :  %d\n",vq.stm.bps) ;
	printf("\"RES\" :  %d\n",vq.stm.res) ;
 	printf("{ return value: %d }", SOK);
}


int do_sysmng(char *pContents)
{
	int ret=SUBMIT_OK,i;

    int stm_res=-1,stm_fps=-1,stm_bps=-1,stm_gop=-1,stm_rc=-1;
    int rec_fps=-1,rec_bps=-1,rec_gop=-1,rec_rc=-1;

    T_CGIPRM prm[128];
    int cnt = parseContents(pContents, prm);
    CGI_DBG("cnt:%d\n", cnt);


    if(cnt>0){
        for(i=0;i<cnt;i++) {
			CGI_DBG("prm[%d].name=%s, prm[%d].value=%s\n", i, prm[i].name, i, prm[i].value);

			if(!strcmp(prm[i].name, "param")){
				if(!strcmp(prm[i].value, "restart")){
					CGI_DBG("system reboot\n");
					NS_CMD::gRefreshSec = 30;
					if( 0 != sysctl_message(UDS_CMD_RESTART, NULL,  0)) {
						return SUBMIT_ERR;
					}
					else { return SUBMIT_OK; }
				}
				else if(!strcmp(prm[i].value, "factorydefault_hard")){
					CGI_DBG("system factorydefault_hard\n");
					NS_CMD::gRefreshSec = 30;
					if(0 != sysctl_message(UDS_CMD_FACTORYDEFAULT_HARD, NULL, 0)) {
						return SUBMIT_ERR;
					}
					else { return SUBMIT_OK; }
				}
				else if(!strcmp(prm[i].value, "telnetd_on")){
					CGI_DBG("telnet_on\n");
					NS_CMD::gRefreshSec = 3;
					int nFlag = 1;
					if(0 != sysctl_message(UDS_CMD_CONTROL_TELNETD, (void*)&nFlag, sizeof(int))) {
						ret = SUBMIT_ERR;
					}
				}
				else if(!strcmp(prm[i].value, "telnetd_off")){
					CGI_DBG("telnet_off\n");
					NS_CMD::gRefreshSec = 3;
					int nFlag = 0;
					if(0 != sysctl_message(UDS_CMD_CONTROL_TELNETD, (void*)&nFlag, sizeof(int))) {
						ret = SUBMIT_ERR;
					}
				}
				else if(!strcmp(prm[i].value, "telnetd_toggle")){
					CGI_DBG("telnet_toggle\n");
					NS_CMD::gRefreshSec = 3;
					int nFlag = 2;
					if(0 != sysctl_message(UDS_CMD_CONTROL_TELNETD, (void*)&nFlag, sizeof(int))) {
						ret = SUBMIT_ERR;
					}
				}
				else if(!strcmp(prm[i].value, "rtmp_on")){
					CGI_DBG("rtmp_on\n");
					NS_CMD::gRefreshSec = 3;
					int nFlag = 1;
					if(0 != sysctl_message(UDS_CMD_CONTROL_RTMP, (void*)&nFlag, sizeof(int))) {
						ret = SUBMIT_ERR;
					}
				}
				else if(!strcmp(prm[i].value, "rtmp_off")){
					CGI_DBG("rtmp_off\n");
					NS_CMD::gRefreshSec = 3;
					int nFlag = 0;
					if(0 != sysctl_message(UDS_CMD_CONTROL_RTMP, (void*)&nFlag, sizeof(int))) {
						ret = SUBMIT_ERR;
					}
				}
            } 
			else if(!strcmp(prm[i].name, "get_videoquality"))
			{
				if(0 != sysctl_message(UDS_GET_VIDEO_QUALITY, (void*)&vq, sizeof(vq))){
					ret = SUBMIT_ERR ;
				}
				else
				{
					ret = SUBMIT_GET_VID ;
				}
			}
			else if(!strcmp(prm[i].name, "framerate"))
			{
				stm_fps = atoi(prm[i].value) ;
			    if(!strcmp(prm[i+1].name, "bitrate"))
				    stm_bps = atoi(prm[i+1].value) ;
			    if(!strcmp(prm[i+2].name, "resolution"))
				    stm_res = atoi(prm[i+2].value) ;

				vq.stm.fps = stm_fps ;
				vq.stm.bps = stm_bps ;
				vq.stm.gop = stm_fps ;
				vq.stm.res = stm_res ;

				CGI_DBG("set_dyn_video_quality\n");
				NS_CMD::gRefreshSec = 10;

				if(0 != sysctl_message(UDS_SET_DYN_VIDEO_QUALITY, (void*)&vq, sizeof(int))) {
					ret = SUBMIT_ERR;
				}
			}
			else if(!strcmp(prm[i].name, "bitrate"))
			{
			}
			else if(!strcmp(prm[i].name, "resolution"))
			{
			}
            else {
				CGI_DBG("Invalid param:%s\n", prm[i].name);
				ret = ERR_INVALID_PARAM;
				break;
            }

		}// end for

	} // end if

	return ret;
}

int do_search(char *pContents)
{
	int ret=-1;

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
		NS_CMD::send_response(ERR_INVALID_METHOD);
		return 0;
	}

	if(pQuery == NULL){
		NS_CMD::send_response(ERR_INVALID_PARAM);
		return 0;
	}
    CGI_DBG("pQuery:%s\n", pQuery);

	if( strstr(pQuery, "action=update")){
		nAction = UPDATE;
	}
	else if( strstr(pQuery, "action=logincheck")){
		nAction = LOGINCHECK;
		printf("OK");
		return 0;
	}
	else if( strstr(pQuery, "action=search")){
		nAction = SEARCH;
	}
	else if( strstr(pQuery, "action=sysmng")){
		nAction = SYSMNG;
	}
	else {
		NS_CMD::send_response(ERR_INVALID_ACTION);
		if(nMethod == POST) free(pQuery);
		return 0;
	}

	// parsing Contents
	if(nMethod == GET){
		pQuery = strstr(pQuery, "&");
		if(!pQuery){
			NS_CMD::send_response(ERR_INVALID_PARAM);
			return 0;
		}
		int len = strlen(pQuery+1);
		CGI_DBG("Param is GET, pQuery:%s, len:%d\n", pQuery+1, len);

		pContents = (char*)malloc(len+1);
		memset(pContents, 0, len+1);
		memcpy(pContents, pQuery+1, len);

	}else{ // POST
		// Not support???
	}

	if(nAction == SYSMNG){
		ret = do_sysmng(pContents);
	}
	else if(nAction == SEARCH){
		ret = do_search(pContents);
	}
	else if(nAction == LOGINCHECK){
	}
	else if(nAction == UPDATE){
		ret = do_update(pContents);
	}
	else {
		NS_CMD::send_response(ERR_INVALID_ACTION);
		return -1;
	}

#if 0
	if(ret == SUBMIT_OK){
		wait_redirect(HOME_PATH, NS_CMD::gRefreshSec);
	}
#else
    
	if(ret == SUBMIT_GET_VID){
        get_vid_response(ret) ;
	}
    else
	    send_response(ret);
#endif

	return 0;
}
