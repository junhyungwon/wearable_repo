/******************************************************************************
 * FITT Board
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_sipc.c
 * @brief   sip protocol client proc
 * @author  
 * @section MODIFY history
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "sipc_ipc_cmd_defs.h"
#include "app_comm.h"
#include "app_sipc.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SIPC_BIN_STR		"/opt/fit/bin/baresip.out"
#define SIPC_CMD_STR		"/opt/fit/bin/baresip.out &"

#define SIP_ACCOUNT_PATH	"/root/.baresip"

typedef struct {
	app_thr_obj rObj; /* sip receive object */
	app_thr_obj eObj; /* sip event handler */
	
	int init;
	int qid;
	
	sipc_status_t ste;
	
	OSA_MutexHndl lock;
	
} app_sipc_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_sipc_t t_sip;
static app_sipc_t *isip = &t_sip;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
static void __sipc_set_default_account(const char *login, const char *domain, const char *pass)
{
	struct stat st;
	char file[256] = "";
	FILE *f = NULL;

	snprintf(file, sizeof(file), "%s/accounts", SIP_ACCOUNT_PATH);
	
	/* file이 존재하는 지 검사 */
	if (stat(file, &st) < 0) {
		/* 파일을 생성 */
		mkdir(SIP_ACCOUNT_PATH, 0700);
		/* write account */
		f = fopen(file, "w");
		if (!f) {
			eprintf("can't create conf %s file\n", file);
			return;
		}
		
		fprintf(f, "<sip:%s@%s>;auth_pass=%s\n", login, domain, pass);
		//fprintf(f, "<sip:%s@%s;transport=tcp>;auth_pass=%s\n", login, domain, pass);
		fclose(f);
		sync();
	}	
}

static int send_msg(int cmd, const char *uri)
{
	to_sipc_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_CLIENT;
	msg.cmd = cmd;
	
	if (cmd == SIPC_CMD_SIP_START) {
		strcpy(msg.uri.peer_uri, uri);
	}
	
	return Msg_Send(isip->qid, (void *)&msg, sizeof(to_sipc_msg_t));
}

/* account 파일을 사용할 경우 이 명령을 필요없음 */
static int send_ua_msg(const char *login, const char *domain, const char *pass)
{
	to_sipc_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_CLIENT;
	msg.cmd = SIPC_CMD_SIP_SET_UA;
	
	strcpy(msg.uri.ua_uri, login);
	strcpy(msg.uri.pbx_uri, domain);
	strcpy(msg.uri.passwd, pass);
	
	return Msg_Send(isip->qid, (void *)&msg, sizeof(to_sipc_msg_t));
}

static int recv_msg(void)
{
	to_sipc_main_msg_t msg;
	int size;
	
	//# blocking
	if (Msg_Rsv(isip->qid, SIPC_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_sipc_main_msg_t)) < 0)
		return -1;
	
	/* baresip로부터 직접 상태정보가 수신되는 경우.
	 * 단말 등록여부, 상대방에서 전화를 종료했을 경우 등이 수신된다.
	 */ 
	if (msg.cmd == SIPC_CMD_SIP_GET_STATUS) {
		isip->ste.call_ste = msg.ste.call_ste;
		isip->ste.call_dir = msg.ste.call_dir;
		isip->ste.call_reg = msg.ste.call_reg;
		/* error 발생 시 UI에 출력할 방법이 필요함 */
		isip->ste.call_err = msg.ste.call_err;
	}
		
	return msg.cmd;
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_sipc_recv_msg(void *prm)
{
	app_thr_obj *tObj = &isip->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	//# message queue
	isip->qid = Msg_Init(SIPC_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive voip process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case SIPC_CMD_SIP_READY:
			isip->init = 1; /* from record process */
			break;
		case SIPC_CMD_SIP_EXIT:
			exit = 1;
			break;
		
		default:
			break;	
		}
	}
	
	Msg_Kill(isip->qid);
	tObj->active = 0;
	
	/* kill process ?? */
	
	aprintf("exit...\n");

	return NULL;
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_sipc_epoll(void *prm)
{
	app_thr_obj *tObj = &isip->eObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	do {
		if (isip->init) {
			break;
		}
		/* baresip가 실행 안된 상태 */
		OSA_waitMsecs(100);
	} while(1);
	
	while (!exit)
	{
		int st = 0;
		
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		} 
		else if (cmd == APP_CMD_NOTY) 
		{
			int state = isip->ste.call_ste;
			dprintf("current baresip state = %d\n", state);
			
			if (isip->ste.call_reg) {
				switch (state) {
				case SIPC_STATE_CALL_IDLE:
					/* 전화를 건다 */
					send_msg(SIPC_CMD_SIP_START, "2001");
					break;
				case SIPC_STATE_CALL_INCOMING:
					/* 전화 수신 중, 전화를 받는다. */
					send_msg(SIPC_CMD_SIP_ANSWER, NULL);
					break;
				case SIPC_STATE_CALL_RINGING:		
					/* 전화를 거는 중에 버튼을 누르면 전화 취소? 
					* 시나리오 정의가 필요함.
					*/
					send_msg(SIPC_CMD_SIP_STOP, NULL);
					break;
				case SIPC_STATE_CALL_ESTABLISHED:
					/*
					* 전화가 연결된 상태. (송/수신 포함) 전화 연결 종료
					*/ 
					send_msg(SIPC_CMD_SIP_STOP, NULL);
					break;
				case SIPC_STATE_CALL_STOP:
					/*
					* IDLE 상태와 구분이 필요한지 확인해야 함.
					* Redial 기능이 필요할 경우 자동으로 재접속 해야 함.
					*/ 
					break;		
				default:
					break;
				}
			} else {
				/* 등록이 안된 상태..UI 표시가 필요함. */
			}
		}
	}
	
	tObj->active = 0;
	aprintf("exit...\n");

	return NULL;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_sipc_init(void)
{
	struct stat sb;
	app_thr_obj *tObj;
	FILE *f = NULL;
	int status;
	
	/* /root/.baresip/accounts에 default 계정 정보 저장 */
	__sipc_set_default_account("2003", "52.78.124.88", "2003");
	
	/* execute baresip */
    if (stat(SIPC_BIN_STR, &sb) != 0) {
		eprintf("can't access baresip execute file!\n");
        return -1;
	}
	system(SIPC_CMD_STR);
	
	//# create recv msg thread.
	tObj = &isip->rObj;
	if (thread_create(tObj, THR_sipc_recv_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client Receive Msg thread\n");
		return EFAIL;
    }
	
	tObj = &isip->eObj;
	if (thread_create(tObj, THR_sipc_epoll, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client event poll thread\n");
		return EFAIL;
    }
	
	/* mutex create */
	status = OSA_mutexCreate(&isip->lock);
	OSA_assert(status == OSA_SOK);
	
    aprintf("... done!\n");

    return 0;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_sipc_exit(void)
{
	app_thr_obj *tObj = &isip->eObj;
	
    event_send(tObj, APP_CMD_EXIT, 0, 0);
    while (tObj->active)
    	app_msleep(20);
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
//	tObj = &isip->rObj;
//	thread_delete(tObj);
	OSA_mutexDelete(&(isip->lock));
	
	dprintf("... done!\n");
}

/*****************************************************************************
* @brief    voip manager
*   - desc
*****************************************************************************/
void app_sipc_set_event(void)
{
	OSA_mutexLock(&isip->lock);
	event_send(&isip->eObj, APP_CMD_NOTY, 0, 0);
	OSA_mutexUnlock(&isip->lock);
}

/*****************************************************************************
* @brief    create user
*   - desc
*****************************************************************************/
void app_sipc_create_user(const char *info, const char *addr, const char *passwd)
{
	send_ua_msg(info, addr, passwd);
}

/*****************************************************************************
* @brief    create user
*   - desc
*****************************************************************************/
void app_sipc_update_account(const char *login, const char *domain, const char *pass)
{
	struct stat st;
	char file[256] = "";
	FILE *f = NULL;

	snprintf(file, sizeof(file), "%s/accounts", SIP_ACCOUNT_PATH);
	
	/* file이 존재하는 지 검사 */
	if (stat(file, &st) == 0) {
		/* delete file */
		remove(file);
	} 
	
	/* 파일을 생성 */
	f = fopen(file, "w");
	if (!f) {
		eprintf("can't create conf %s file\n", file);
		return;
	}
	
	fprintf(f, "<sip:%s@%s>;auth_pass=%s\n", login, domain, pass);
	//fprintf(f, "<sip:%s@%s;transport=tcp>;auth_pass=%s\n", login, domain, pass);
	fclose(f);
	sync();
}

/*****************************************************************************
* @brief    baresip exit
*   - desc
*****************************************************************************/
void app_sipc_client_exit(void)
{
	send_msg(SIPC_CMD_SIP_EXIT, NULL);
}
