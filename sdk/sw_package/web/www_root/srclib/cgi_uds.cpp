#include <mutex>

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> // sockaddr_un 

#include "cgi.h"

class A {
	public:
		A();
		~A();

	private:
		std::mutex _mutex;
};
A::A(){
	_mutex.lock();
}
A::~A(){
	_mutex.unlock();
}


int sysctl_message(
		int   cmd,  // Command : Reboot, fwupdate, and etc.
		void *data,    
		int data_len)
{
	//A a;

	int			ret  = 0;
	char		wbuf[256]={0}; 		// write buffer
	char		rbuf[256]={0};      // read  buffer

	int  		cs;					// client socket
	struct sockaddr_un	caddr;		// client address

	cs = socket(AF_UNIX, SOCK_STREAM, 0);
	if(cs < 0 ) {
		perror("socket : ");
		return ERR_FAILED_CREATE_DESCRIPTOR;
	}

#if 0
	int option = TRUE;               //네이글 알고리즘 on/off
	setsockopt(cs,             //해당 소켓
           IPPROTO_TCP,          //소켓의 레벨
           TCP_NODELAY,          //설정 옵션
           (const char*)&option, // 옵션 포인터
           sizeof(option));      //옵션 크기
#else
	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;
	setsockopt(cs, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
#endif

	bzero(&caddr, sizeof(caddr));
	caddr.sun_family = AF_UNIX;
	strcpy(caddr.sun_path, FILE_UDS_SYSTEM);
	ret = sizeof(caddr);
	if(connect(cs, (struct sockaddr *)&caddr, ret) < 0) {
		perror("Failed Connect System UDS : ");
		close(cs);
		return ERR_FAILED_CONNECT;
	}

	CGI_DBG("cmd=%d\n", cmd);

	switch(cmd) {
		case UDS_GET_SYSTEM_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_GET_SYSTEM_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : %s\n", wbuf);

				// 2. wait response and read sizeof T_CGI_SYSTEM_CONFIG bytes
				ret = read(cs, (T_CGI_SYSTEM_CONFIG*)data, sizeof(T_CGI_SYSTEM_CONFIG));
				CGI_DBG("Read, size:%d\n", ret);

				if(ret > 0){
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_SET_SYSTEM_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_SET_SYSTEM_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : %s\n", wbuf);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				ret = write(cs, data, sizeof(T_CGI_SYSTEM_CONFIG));
				CGI_DBG("Sent Data, written len = %d\n", ret);

				// 3. wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){

					// check return value
					if(strcmp(rbuf, "OK") == 0){
						ret = ERR_NO_ERROR;
					}
					else if(strcmp(rbuf, "NO CHANGE") == 0){
						ret = ERR_NO_CHANGE;
					}

					close(cs);
					return ret;
				}
			}
			break;
		case UDS_GET_SERVERS_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_GET_SERVERS_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD:%s\n", wbuf);

				// wait response and read sizeof T_CGI_SERVERS_CONFIG bytes
				ret = read(cs, (T_CGI_SERVERS_CONFIG*)data, sizeof(T_CGI_SERVERS_CONFIG));
				CGI_DBG("Read, size:%d\n", ret);

				if(ret > 0){
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_SET_SERVERS_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_SET_SERVERS_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : %s\n", wbuf);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write data
				ret = write(cs, data, sizeof(T_CGI_SERVERS_CONFIG));
				CGI_DBG("Sent Data, written len = %d\n", ret);

				// 4. wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){

					// check return value
					if(strcmp(rbuf, "OK") == 0){
						ret = ERR_NO_ERROR;
					}
					else if(strcmp(rbuf, "NO CHANGE") == 0){
						ret = ERR_NO_CHANGE;
					}

					close(cs);
					return ret;
				}
			}
			break;
		case UDS_GET_NETWORK_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_GET_NETWORK_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD:%s\n",wbuf);

				// wait response and read sizeof T_CGI_NETWORK_CONFIG bytes
				ret = read(cs, data, sizeof(T_CGI_NETWORK_CONFIG));
				CGI_DBG("Read, size:%d\n", ret);

				if(ret > 0){
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_SET_NETWORK_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_SET_NETWORK_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD:%s\n", wbuf);

				// 2. check READY
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

#if 1
				// 3. write id/password info
				ret = write(cs, data, sizeof(T_CGI_NETWORK_CONFIG));
				CGI_DBG("Sent Data, written len = %d\n", ret);
#else
				sprintf(wbuf, "%d %s %s %s %d %s %s %s %s %s %d %s %s", 
							t.wireless.addr_type,
							t.wireless.ipv4,
							t.wireless.gw,
							t.wireless.mask,
							t.cradle.addr_type,
							t.cradle.ipv4,
							t.cradle.gw,
							t.cradle.mask,
							t.wifi_ap.id,
							t.wifi_ap.pw,
							t.live_stream_account_enable,
							t.live_stream_account.id,
							t.live_stream_account.pw
							);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent Data:%s, written len = %d\n", wbuf, ret);
#endif

				// 4. wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){

					// check return value
					if(strcmp(rbuf, "OK") == 0){
						ret = ERR_NO_ERROR;
					}
					else if(strcmp(rbuf, "NO CHANGE") == 0){
						ret = ERR_NO_CHANGE;
					}

					close(cs);
					return ret;
				}
			}
			break;
		case UDS_GET_OPERATION_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_GET_OPERATION_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : cs:(%d), %s, len = %d \n", cs, wbuf, ret);

				// wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){
					T_CGI_OPERATION_CONFIG *t = (T_CGI_OPERATION_CONFIG*)data;
					sscanf(rbuf, "%d %d %d %d %d %d %d %s %s", 
							&t->rec.pre_rec,
							&t->rec.auto_rec,
							&t->rec.audio_rec,
							&t->rec.interval,
							&t->rec.overwrite,
							&t->display_datetime,
							&t->p2p.enable,
							t->p2p.username,
							t->p2p.password);
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_SET_OPERATION_CONFIG:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_SET_OPERATION_CONFIG);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : %s\n", wbuf);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write id/password info
				T_CGI_OPERATION_CONFIG t;
				memcpy(&t, data, sizeof(t));
				sprintf(wbuf, "%d %d %d %d %d %d %d %s %s", 
						t.rec.pre_rec, 
						t.rec.auto_rec, 
						t.rec.audio_rec, 
						t.rec.interval, 
						t.rec.overwrite, 
						t.display_datetime,
					   	t.p2p.enable,
						t.p2p.username,
						t.p2p.password);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent Data:%s, written len = %d\n", wbuf, ret);

				// 4. wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){

					// check return value
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_FWUPDATE:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_CMD_FWUPDATE);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. send , fwfile path
				sprintf(wbuf, "%s", (char*)data);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent Data:%s, written len = %d\n", wbuf, ret);

				// wait response and read 256 bytes for fw update status
				ret = read(cs, rbuf, sizeof rbuf);
				if(ret > 0){
					CGI_DBG("read:%s\n", rbuf);

					if(0==strcmp(rbuf, "SUCCEED")) {
						close(cs);
						return 0;
					}
					else if(0==strcmp(rbuf, "INVALID_FWFILE")) {
						close(cs);
						return ERR_INVALID_FWFILE;
					}
				}
			}
			break;

		case UDS_GET_VIDEO_ENCODER_INFORMATION:
			{
				sprintf(wbuf, "%s", STR_MSG_GET_ENCODER_INFO);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// wait response and read 256 bytes
				ret = read(cs, rbuf, sizeof rbuf);
				if(ret > 0){
					CGI_DBG("read:%s\n", rbuf);

					if(strcmp(rbuf, "NULL") != 0) {
						int w=0, h=0;
						int kbps=0;
						int fps=0, ei=0, gop=0;
						sscanf(rbuf, "w=%d,h=%d,kbps=%d,fps=%d,ei=%d,gov=%d", &w, &h, &kbps, &fps, &ei, &gop);

CGI_DBG("\nw:%d, h:%d, kbps:%d, fps:%d, ei:%d, gov:%d\n", w, h, kbps, fps, ei, gop);

						close(cs);
						return 0;
					}
				}

			}
			break;
		case UDS_CMD_CHECK_ACCOUNT:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_CMD_CHECK_ACCOUNT);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write account
				ret = write(cs, data, sizeof(T_CGI_ACCOUNT));
				CGI_DBG("Sent Account Data: written len = %d\n", ret);

				// 4. read response
				ret = read(cs, rbuf, sizeof rbuf);
				if(ret>0){
					CGI_DBG("read:%s\n", rbuf);

					if(0==strcmp(rbuf, "OK")) {
						close(cs);
						return 0;
					}
					else if(0==strcmp(rbuf, "INVALID_ID")) {
						close(cs);
						return ERR_INVALID_ID;
					}
					else if(0==strcmp(rbuf, "INVALID_PW")) {
						close(cs);
						return ERR_INVALID_PW;
					}
					else {
						close(cs);
						return ERR_INVALID_ACCOUNT;
					}

					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_UPDATE_USER:
			{
				// 1.write command
				sprintf(wbuf, "%s", STR_MSG_CMD_UPDATE_USER);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write user info
				ret = write(cs, data, sizeof(T_CGI_USER));
				CGI_DBG("Sent Data: written len = %d\n", ret);

				// 4. read response
				ret = read(cs, rbuf, sizeof rbuf);
				if(ret>0){
					CGI_DBG("read:%s\n", rbuf);
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_UPDATE_ONVIF_USER:
			{
				// 1.write command
				sprintf(wbuf, "%s", STR_MSG_CMD_UPDATE_ONVIF_USER);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check ready
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write onvif user info
				ret = write(cs, data, sizeof(T_CGI_ONVIF_USER));
				CGI_DBG("Sent T_CGI_ONVIF_USER Data: written len = %d\n", ret);

				// 4. read response
				ret = read(cs, rbuf, sizeof rbuf);
				if(ret>0){
					CGI_DBG("read:%s\n", rbuf);
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_CHANGE_PASSWORD:
			{
				// 1.write command
				sprintf(wbuf, "%s", STR_MSG_CMD_CHANGE_PASSWORD);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check READY
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write id/password info
				ret = write(cs, data, sizeof(T_CGI_USER));
				CGI_DBG("Sent Data, written len = %d\n", ret);

				// 4. read response
				ret = read(cs, rbuf, sizeof(rbuf));
				if(ret>0)
				{
					CGI_DBG("read:%s\n", rbuf);
					close(cs);
					return 0;
				}

			}
			break;
		case UDS_GET_VIDEO_QUALITY:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_GET_VIDEO_QUALITY);
				ret = write(cs, wbuf, sizeof wbuf);
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, len = %d \n", cs, wbuf, ret);

				// wait response and read 256 bytes
				T_CGI_VIDEO_QUALITY p; memset(&p, 0, sizeof(p));
				ret = read(cs, &p, sizeof p);
				CGI_DBG("read T_CGI_VIDEO_QUALITY, ret=%d\n", ret);
				if(ret > 0){
					memcpy(data, &p, sizeof(p));
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_SET_VIDEO_QUALITY:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_SET_VIDEO_QUALITY);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check READY
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write id/password info
				ret = write(cs, data, sizeof(T_CGI_VIDEO_QUALITY));
				CGI_DBG("Sent T_CGI_VIDEO_QUALITY, written len = %d\n", ret);

				// 4. wait response and read OK
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);
				if(ret > 0){
					// check return value
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_RESTART:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_CMD_RESTART);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD:%s, cs:(%d), wrtten len = %d \n", wbuf, cs, ret);

				if(ret > 0){
					// check return value
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_FACTORYSET_HARD:
		case UDS_CMD_FACTORYDEFAULT_HARD:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_CMD_FACTORYDEFAULT);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent CMD : cs:(%d), CMD:%s, wrtten len = %d \n", cs, wbuf, ret);

				// 2. check READY
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write type
				sprintf(wbuf, "type=%d", 1); // if type=1 means all, others ...I don't know..
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent Data:%s, written len = %d\n", wbuf, ret);

				if(ret > 0){
					// check return value
					close(cs);
					return 0;
				}
			}
			break;
		case UDS_CMD_FACTORYSET:
		case UDS_CMD_FACTORYSET_SOFT:
			break;

		case UDS_CMD_CONTROL_TELNETD:
			{
				// 1. write command
				sprintf(wbuf, "%s", STR_MSG_CMD_CONTRL_TELNETD);
				ret = write(cs, wbuf, sizeof(wbuf));
				CGI_DBG("Sent %s: cs:(%d), wrtten len = %d \n", wbuf, cs, ret);

				// 2. check READY
				ret = read(cs, rbuf, sizeof rbuf);
				CGI_DBG("read:%s, ret=%d\n", rbuf, ret);

				// 3. write flag
				int flag = *(int*)data;
				sprintf(wbuf, "flag=%d", flag); // flag, 1:on, 0:off.
				ret = write(cs, &flag, sizeof(flag));
				CGI_DBG("Sent Data:%s, written len = %d\n", wbuf, ret);

				if(ret > 0){
					// check return value
					close(cs);
					return 0;
				}
			}
			break;

		default:
			CGI_DBG("Invalid Command %d\n", cmd);
			break;
	}

	close(cs);

	return ERR_ERROR;
}

