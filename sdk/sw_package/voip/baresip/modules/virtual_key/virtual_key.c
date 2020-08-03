/**
 * @file virtual_key/virtual_key.c  communication between main process.
 *
 * Copyright (C) 2020 LF
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <re.h>
#include <baresip.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "sipc_ipc_cmd_defs.h"
#include "msg.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UA_PREFIX				"/uanew" /* long command 사용*/
#define DIAL_PREFIX				"/dial" /* long command 사용*/
#define UINPUT_DEV_STR			"/dev/uinput"

typedef struct {
	app_thr_obj mObj;
	app_thr_obj sObj; /* baresip에서 main으로 메세지를 전달할 경우 */
	
	int qid;
	int ev_fd; /* uinput fd */
	
	char user[256];     /* account informat. nomarly number */
	char svr[256];   	/* pbx server address */
	char pwd[256]; 		/* password */
	char peer[256]; 	/* peer */
	
} key_config_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static key_config_t key_obj_t;
static key_config_t *ikey = &key_obj_t;

static char ui_buf[1024];

//##############################################################################################
//#----------------------------------------------------------------------------
static int __uinput_init(void)
{
	struct uinput_user_dev uidev;
	int fd = -1, i;
	
	fd = open(UINPUT_DEV_STR, O_WRONLY | O_NONBLOCK);
	if (fd < 0) {
    	warning("Opening of uinput failed!\n");
  	}
	
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	/* 가속도 센서와 같이 press, release가 불가능 한 경우 사용 */
	//ioctl(fd, UI_SET_EVBIT, EV_SYN); 
	 
	/* 총 4개의 키가 필요함 */
	ioctl(fd, UI_SET_KEYBIT, KEY_A); /* accept call */
	ioctl(fd, UI_SET_KEYBIT, KEY_B); /* break call */
	  
	memset(&uidev, 0, sizeof(uidev));
  	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Simple Keypad");  
	
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1234;
	uidev.id.product = 0x4321;
	uidev.id.version = 2;
	
	/* writing setting */
	write(fd, &uidev, sizeof(uidev));
	ioctl(fd, UI_DEV_CREATE);
	  
	return fd;
}

static void __uinput_exit(int fd)
{
	ioctl(fd, UI_DEV_DESTROY);
}

static void __uinput_emit(int fd, int type, int code, int val)
{
	struct input_event ie;
	
	ie.type = type;
	ie.code = code;
	ie.value = val;
	
	/* timestamp values are ignored */
	ie.time.tv_sec = 0;
	ie.time.tv_usec = 0; 
	
	write(fd, &ie, sizeof(ie));
	info("uinput emit code %x, value %d\n", code, val);
}

static void ev_key_press(int fd, int code)
{
	__uinput_emit(fd, EV_KEY, code, 1);
}

static void ev_key_release(int fd, int code)
{
	__uinput_emit(fd, EV_KEY, code, 0);
}

/* 사용 안 함 : TODO */
static void ev_key_sync(int fd)
{
	__uinput_emit(fd, EV_SYN, 0, 0);
}

//###################### Baresip Helper ########################################################
static int __create_user(const char *call_num, const char *server_addr, const char *passwd)
{
	memset(ui_buf, 0, sizeof(ui_buf));
	
	//# <sip:1006@192.168.0.5>;auth_pass=1234
	snprintf(ui_buf, sizeof(ui_buf), "%s <sip:%s@%s>;auth_pass=%s", 
					UA_PREFIX, call_num, server_addr, passwd);
	
	ui_input_str((const char *)ui_buf);
	
	return 0;
}

static int __dialer_user(const char *call_num)
{
	memset(ui_buf, 0, sizeof(ui_buf));
	
	//# /dial xxxxxx
	snprintf(ui_buf, sizeof(ui_buf), "%s %s", DIAL_PREFIX, call_num);
	
	ui_input_str((const char *)ui_buf);
	
	return 0;
}

static void __call_stop(void)
{
	ev_key_press(ikey->ev_fd, KEY_B);
	delay_msecs(20);
	ev_key_release(ikey->ev_fd, KEY_B);
}

static void __call_answer(void)
{
	ev_key_press(ikey->ev_fd, KEY_A);
	delay_msecs(20);
	ev_key_release(ikey->ev_fd, KEY_A);
}

static int send_msg(int cmd)
{
	to_smain_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	
	return Msg_Send(ikey->qid, (void *)&msg, sizeof(to_smain_msg_t));
}

static int recv_msg(void)
{
	to_sipc_msg_t msg;
	
	//# blocking
	if (Msg_Rsv(ikey->qid, SIPC_MSG_TYPE_TO_CLIENT, (void *)&msg, sizeof(to_sipc_msg_t)) < 0) {
		return -1;
	}
	
	if (msg.cmd == SIPC_CMD_SIP_SET_UA) {
		/* clear user information */
		memset(ikey->user, 0, sizeof(ikey->user));
		memset(ikey->svr, 0, sizeof(ikey->svr));
		memset(ikey->pwd, 0, sizeof(ikey->pwd));
	
		strcpy(ikey->user, msg.ua);
		strcpy(ikey->svr, msg.server);
		strcpy(ikey->pwd, msg.password);
	} else if (msg.cmd == SIPC_CMD_SIP_START) {
		memset(ikey->peer, 0, sizeof(ikey->peer));
		strcpy(ikey->peer, msg.peer);
	}
	
	return msg.cmd;
}

static void *THR_sipc_poll(void *prm)
{
	app_thr_obj *tObj = &ikey->sObj;
	int done = 0, cmd;
	
	info("enter...\n");
	tObj->active = 1;
	
	while(!done)
	{
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}
		
		/* 200ms 간격으로 baresip 상태를 확인하여 전달 */
		delay_msecs(200);
	}
	
	tObj->active = 0;
	info("exit...\n");
		
	return NULL;
}

static void *THR_sipc_main(void *prm)
{
	app_thr_obj *tObj = &ikey->mObj;
	int done = 0, cmd;
	
	info("enter...\n");
	tObj->active = 1;
	
	/* message queue init */
	ikey->qid = Msg_Init(SIPC_MSG_KEY);
	send_msg(SIPC_CMD_SIP_READY);
	
	while(!done)
	{
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			warning("failed to receive sipc process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case SIPC_CMD_SIP_SET_UA:
			info("baresip create user agent\n");
			/* 계정을 등록 */
			__create_user(ikey->user, ikey->svr, ikey->pwd);
			break;
		
		case SIPC_CMD_SIP_START:
			info("baresip peer number %s\n", ikey->peer);
			__dialer_user(ikey->peer);
			break;
		
		case SIPC_CMD_SIP_ANSWER:
			info("baresip call answer\n");
			__call_answer();
			break;
			
		case SIPC_CMD_SIP_STOP:
			info("baresip call stop!\n");
			__call_stop();
			break;	
		
		case SIPC_CMD_SIP_EXIT:
			/* 프로그램 종료 */
			done = 1;
			break;
		
		default:
			break;	
		}
	}
	
	tObj->active = 0;
	Msg_Kill(ikey->qid);
	
	info("exit...\n");
		
	return NULL;
}

//#########################################################################################################
static int module_init(void)
{
	app_thr_obj *tObj = &ikey->mObj;
	int err;
	
	/* virtual key 생성을 위한 linux uinput 드라이버 초기화 */
	ikey->ev_fd = __uinput_init();
	
	/* message 수신 및 전송을 위한 Thread 생성 */
	if (thread_create(tObj, THR_sipc_main, APP_THREAD_PRI, NULL) < 0) {
		warning("create thread!\n");
		return -1;
	}
	
	tObj = &ikey->sObj;
	if (thread_create(tObj, THR_sipc_poll, APP_THREAD_PRI, NULL) < 0) {
		warning("create thread!\n");
		return -1;
	}
	
	info("module init virtual_key success\n");
	return 0;
}

static int module_close(void)
{
	app_thr_obj *tObj = &ikey->mObj;
	
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	__uinput_exit(ikey->ev_fd);
	
	return 0;
}

/* config 파일의 module_app에 등록됨 */
const struct mod_export DECL_EXPORTS(virtual_key) = {
	"virtual_key",
	"application",
	module_init,
	module_close
};
