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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dev_sound.h"
#include "sipc_ipc_cmd_defs.h"
#include "app_comm.h"
#include "app_main.h"
#include "app_voip.h"
#include "app_leds.h"
#include "app_log.h"
#include "app_ctrl.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define SIPC_BIN_STR		"/opt/fit/bin/baresip.out"
/* IPV4 Only */
#define SIPC_CMD_STR		"/opt/fit/bin/baresip.out -4 &" 
#define SIP_ACCOUNT_PATH	"/root/.baresip"
#define SIP_VOLUME_CONF		"/usr/share/sound.conf"

typedef struct {
	app_thr_obj rObj; 	/* sip receive object */
	app_thr_obj eObj; 	/* sip event handler */
	
	int init;
	int qid;
	int snd_level;		/* sound level */
	
	short svr_port;
	int enable_stun;
	int net_type;
	
	char dev_num[SIPC_DATA_SZ];     /* local information. 일반적으로 단말기 번호 */
	char server[SIPC_DATA_SZ];    	/* 교환기 주소 */
	char passwd[SIPC_DATA_SZ];     /* 단말기 등록 비밀번호 */
	char peer_num[SIPC_DATA_SZ];	  /* 연결할 상대 단말 정보 */
	
	char stun_svr[SIPC_DATA_SZ];	  /* stun 연결 시 주소 */
	
	sipc_status_t st;
	
	OSA_MutexHndl lock;
	
} app_voip_t;

typedef enum {
	SND_LEVEL_LOW = 0,
	SND_LEVEL_MID,
	SND_LEVEL_HIGH

} SND_AIC3X_LEVEL;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_voip_t t_voip;
static app_voip_t *ivoip = &t_voip;

/* aic3x audio codec output volume percentage */
static int __aic3x_output_level[3] = {
	60, 80, 100 	
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 local function
-----------------------------------------------------------------------------*/
#if SYS_CONFIG_VOIP
static const char *__action_str(int act)
{
	switch (act) {
	case SIPC_STATE_CALL_IDLE:          return "IDLE";
	case SIPC_STATE_CALL_INCOMING:      return "CALL_INCOMING";
	case SIPC_STATE_CALL_RINGING:       return "CALL_RINGING";
	case SIPC_STATE_CALL_ESTABLISHED:   return "CALL_ESTABLISHED";
	case SIPC_STATE_CALL_LOCAL_SDP:     return "CALL_LOCAL_SDP";
	case SIPC_STATE_CALL_STOP:          return "CALL_CLOSED";
	default: return "?";
	}
}

static int send_msg(int cmd, const char *uri)
{
	to_sipc_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_CLIENT;
	msg.cmd = cmd;
	
	if (cmd == SIPC_CMD_SIP_START) {
		strcpy(msg.uri.peer_uri, uri);
	} else if (cmd == SIPC_CMD_SIP_SET_SOUND) {
		msg.uri.spk_lv = ivoip->snd_level;
	}
	
	return Msg_Send(ivoip->qid, (void *)&msg, sizeof(to_sipc_msg_t));
}

/* account 파일을 사용할 경우 이 명령을 필요없음 */
static int send_ua_msg(int cmd, int net, int enable, short port, int snd_lv, const char *login, 
			const char *domain, const char *pass, const char *stun_domain)
{
	to_sipc_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_CLIENT;
	msg.cmd = cmd;
	
	if (login != NULL)
		strcpy(msg.uri.ua_uri, login);
	if (domain != NULL)
		strcpy(msg.uri.pbx_uri, domain);
	if (pass != NULL)
		strcpy(msg.uri.passwd, pass);
	if (stun_domain != NULL)
		strcpy(msg.uri.stun_uri, stun_domain);	
	
	msg.uri.en_stun = enable;
	msg.uri.port = port;
	msg.uri.net_if = net;
	msg.uri.spk_lv = snd_lv;
	
	return Msg_Send(ivoip->qid, (void *)&msg, sizeof(to_sipc_msg_t));
}

static int recv_msg(void)
{
	to_sipc_main_msg_t msg;
	
	//# blocking
	if (Msg_Rsv(ivoip->qid, SIPC_MSG_TYPE_TO_MAIN, (void *)&msg, sizeof(to_sipc_main_msg_t)) < 0)
		return -1;
	
	/* baresip로부터 직접 상태정보가 수신되는 경우.
	 * 단말 등록여부, 상대방에서 전화를 종료했을 경우 등이 수신된다.
	 */ 
	if (msg.cmd == SIPC_CMD_SIP_GET_STATUS) {
		ivoip->st.call_ste = msg.ste.call_ste;
		ivoip->st.call_dir = msg.ste.call_dir;
		ivoip->st.call_reg = msg.ste.call_reg;
		/* error 발생 시 UI에 출력할 방법이 필요함 */
		ivoip->st.call_err = msg.ste.call_err;
	}
		
	return msg.cmd;
}

#if 0
/* /root/.baresip/accounts에 default 계정 정보 저장 */
//	__set_default_account("2003", "52.78.124.88", "2003");
//	__set_default_account("2003", "192.168.40.95", "2003");
static void __set_default_account(const char *login, const char *domain, const char *pass)
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
		
		//fprintf(f, ";ptime=80\n");
		fprintf(f, "<sip:%s@%s>;auth_pass=%s\n", login, domain, pass);
		//fprintf(f, "<sip:%s@%s;transport=tcp>;auth_pass=%s\n", login, domain, pass);
		fclose(f);
		sync();
	} 	
}
#endif

static int __get_voip_play_volume(int *level)
{
	FILE *f = NULL;
	int res = 0;
	char buf[256 + 1] = {0,};
	char tbuf[2];
	
	res = access(SIP_VOLUME_CONF, R_OK|W_OK);
    if ((res == 0) || (errno == EACCES)) 
	{
		f = fopen(SIP_VOLUME_CONF, "r");
		if (f != NULL) 
		{
			while (fgets(buf, sizeof(buf), f) != NULL)
			{
				char *s;
				int val;

				s = strstr(buf, "level=");
				if (s != NULL) {
					s += 6; /* level=X */
					tbuf[0] = s[0];
					tbuf[1] = '\0';
					
					val = atoi(tbuf); /* ex) 1 */
					fclose(f);
					*level = val;
					
					//dprintf("level = %d\n", *level);
					return 0;
				}
			}
		}
    }
	
	eprintf("couldn't open %s\n", SIP_VOLUME_CONF);
	return -1;	
}

static void __set_voip_play_volume(int level)
{
	int res = 0;
	FILE *f = NULL;
	
	res = access(SIP_VOLUME_CONF, R_OK|W_OK);
    if ((res == 0) || (errno == EACCES)) {
		/* delete file */
      	unlink(SIP_VOLUME_CONF); 
    }
	
	f = fopen(SIP_VOLUME_CONF, "wb");
	if (f != NULL) {
		fprintf(f, "level=%d\n", level);
		fflush(f);
		fclose(f);
		chmod(SIP_VOLUME_CONF, 0660);
	} 
	else {
		eprintf("couldn't create %s config file\n", SIP_VOLUME_CONF);
	}
}

static void __call_register_handler(void)
{
	char msg[256] = {0, };
	
	dprintf("baresip user register start.....\n");
	/* create ua and register */
	send_ua_msg(SIPC_CMD_SIP_REGISTER_UA, ivoip->net_type, ivoip->enable_stun, ivoip->svr_port, 
			ivoip->snd_level, ivoip->dev_num,  ivoip->server, ivoip->passwd, ivoip->stun_svr);
	
	if (ivoip->enable_stun) {
		snprintf(msg, sizeof(msg), "STUN URL %s@%s:%d (pw: %s) ", ivoip->dev_num, ivoip->stun_svr, 
				ivoip->svr_port, ivoip->passwd);
	} else {
		snprintf(msg, sizeof(msg), "URL %s@%s:%d (pw: %s)", ivoip->dev_num, ivoip->server, 
				ivoip->svr_port, ivoip->passwd);
	}
    app_log_write(MSG_LOG_WRITE, msg);
	
	dprintf("%s\n", msg);
}

static void __call_unregister_handler(void)
{
	dprintf("baresip user unregister start.....\n");
	
	send_msg(SIPC_CMD_SIP_UNREGISTER_UA, NULL);
}

static void __call_event_handler(void)
{
	char msg[128] = {0,};
	int action = ivoip->st.call_ste;
	
	if (!ivoip->st.call_reg) {
		eprintf("not registered yet!!\n");
		return;
	}
	
	snprintf(msg, sizeof(msg), "baresip state is %s, send btn...", __action_str(action));
	app_log_write(MSG_LOG_WRITE, msg);
	dprintf("%s\n", msg);
	
	switch (action) {
	case SIPC_STATE_CALL_IDLE:
		/* 전화를 건다 */
		send_msg(SIPC_CMD_SIP_START, ivoip->peer_num);
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
	
	case SIPC_STATE_CALL_LOCAL_SDP:
		/*
		* 전화 송신 전 SDP 데이터 전송. 
		* 네트워크에 문제가 발생했을 경우 이 상태에서 중지됨. 
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
}

/*****************************************************************************
* @brief    
*   - desc
*       : baresip로 earphone volume level을 변경하는 메시지 전달.
*****************************************************************************/
static void __call_snd_volume_handler(void)
{
	send_msg(SIPC_CMD_SIP_SET_SOUND, NULL);
}

/*****************************************************************************
* @brief    
*   - desc
*       : __call_event_handler() 에 대한 응답이 baresip로부터 수신.
*****************************************************************************/
static void __call_status_handler(void)
{
	char msg[128] = {0,};
	int is_reg = ivoip->st.call_reg;
	int errcode = ivoip->st.call_err;
	int action = ivoip->st.call_ste;
	
	snprintf(msg, sizeof(msg), "baresip response is %s", __action_str(action));
	app_log_write(MSG_LOG_WRITE, msg);
	dprintf("%s\n", msg);
	/* BLINK 상태 확인이 필요함 */
	if (is_reg) 
	{
		switch (action) {
		case SIPC_STATE_CALL_ESTABLISHED:
			/* 단말이 PBX에 등록된 상태 Camera 3 LED ON(Green) */
			app_leds_voip_ctrl(DEV_LED_BLINK);
			ctrl_swosd_userstr("C", 1);
			if (app_cfg->ste.b.voip_buzz)
				app_cfg->ste.b.voip_buzz = 0;
			break;
			
		case SIPC_STATE_CALL_INCOMING:
			/* buzzer output enable */
			if (app_cfg->ste.b.voip_buzz == 0) {
				app_cfg->ste.b.voip_buzz = 1;
			}
			break;
		
		case SIPC_STATE_CALL_LOCAL_SDP:
			/* ignore Local SDP Session */
			break;
			
		default:
			app_leds_voip_ctrl(DEV_LED_ON);
			ctrl_swosd_userstr("C", 0);
			if (app_cfg->ste.b.voip_buzz)
				app_cfg->ste.b.voip_buzz = 0;
			break;	
		}
	} 
	else {
		app_leds_voip_ctrl(DEV_LED_OFF);
		ctrl_swosd_userstr("C", 0);
	}
}

static void __call_send_cmd(int cmd)
{
	app_thr_obj *tObj = &ivoip->eObj;
	
	OSA_mutexLock(&ivoip->lock);
	event_send(tObj, cmd, 0, 0);
	OSA_mutexUnlock(&ivoip->lock);
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_voip_main(void *prm)
{
	app_thr_obj *tObj = &ivoip->eObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		} else if (cmd == APP_CMD_START) {
			__call_register_handler();
		} else if (cmd == APP_CMD_NOTY) {
			__call_event_handler();
		} else if (cmd == APP_CMD_STOP) {
			__call_unregister_handler();
		} else if (cmd == APP_CMD_PAUSE) {
			__call_snd_volume_handler();
		}
	}
	
	tObj->active = 0;
	aprintf("exit...\n");

	return NULL;
}

/*****************************************************************************
* @brief    voip main function
* @section  DESC Description
*   - desc  baresip로부터 전달되는 메시지를 수신
*****************************************************************************/
static void *THR_voip_recv_msg(void *prm)
{
	app_thr_obj *tObj = &ivoip->rObj;
	int exit = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	//# message queue
	ivoip->qid = Msg_Init(SIPC_MSG_KEY);
	
	while (!exit) {
		//# wait cmd
		cmd = recv_msg();
		if (cmd < 0) {
			eprintf("failed to receive voip process msg!\n");
			continue;
		}
		
		switch (cmd) {
		case SIPC_CMD_SIP_READY:
			ivoip->init = 1;
			break;
		
		case SIPC_CMD_SIP_GET_STATUS:
			__call_status_handler();
			break;
			
		case SIPC_CMD_SIP_EXIT:
			exit = 1;
			break;
		
		default:
			break;	
		}
	}
	
	Msg_Kill(ivoip->qid);
	tObj->active = 0;
	
	/* kill process ?? */
	aprintf("exit...\n");

	return NULL;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_voip_init(void)
{
	app_thr_obj *tObj;
	struct stat sb;
	int percent, lv;
	int status;
	
	memset(ivoip, 0, sizeof(app_voip_t));
	
	/* alsa volume */
//	amixer cset numid=17 50% # DAC_L1 to HPLOUT Volume Control
//	amixer cset numid=1 90%  # Left / Right DAC Digital Volume
	status = __get_voip_play_volume(&lv);
	if (status < 0) {
		/* set default level */
		ivoip->snd_level = SND_LEVEL_HIGH;
	} else {
		ivoip->snd_level = lv;
	}
	
	/* execute baresip */
    if (stat(SIPC_BIN_STR, &sb) != 0) {
		eprintf("can't access baresip execute file!\n");
        return -1;
	}
	system(SIPC_CMD_STR);
	
	//# create recv msg thread.
	tObj = &ivoip->rObj;
	if (thread_create(tObj, THR_voip_recv_msg, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client Receive Msg thread\n");
		return EFAIL;
    }
	
	tObj = &ivoip->eObj;
	if (thread_create(tObj, THR_voip_main, APP_THREAD_PRI, NULL) < 0) {
    	eprintf("create SIP Client event poll thread\n");
		return EFAIL;
    }
	
	/* mutex create */
	status = OSA_mutexCreate(&ivoip->lock);
	OSA_assert(status == OSA_SOK);
	
    aprintf("... done!\n");

    return 0;
}

/*****************************************************************************
* @brief    voip manager
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_voip_exit(void)
{
	app_thr_obj *tObj = &ivoip->eObj;
	
    event_send(tObj, APP_CMD_EXIT, 0, 0);
    while (tObj->active)
    	app_msleep(20);
	thread_delete(tObj);
	
	//#--- stop message receive thread. 
	//# 프로세스에서 이미 종료가 되므로 APP_CMD_EXIT를 하면 안됨.
	send_msg(SIPC_CMD_SIP_EXIT, NULL);
	OSA_mutexDelete(&(ivoip->lock));
	
	dprintf("... done!\n");
}

/*****************************************************************************
* @brief    pbx register....
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_voip_start(int net_type, int enable_stun, short server_port, const char *uag, const char *server, 
			const char *passwd, const char *peer, const char *stun_server)
{
	if (uag != NULL)
		strcpy(ivoip->dev_num, uag);
	if (server != NULL)
		strcpy(ivoip->server, server);
	if (passwd != NULL)
		strcpy(ivoip->passwd, passwd);
	if (peer != NULL)
		strcpy(ivoip->peer_num, peer);
	if (stun_server != NULL)
		strcpy(ivoip->stun_svr, stun_server);
		
	if (server_port <= 0)
		ivoip->svr_port = 6060; //# set default port
	else
		ivoip->svr_port = server_port;
	
	ivoip->enable_stun = enable_stun;
	ivoip->net_type = net_type;
	
	__call_send_cmd(APP_CMD_START);
}

/*****************************************************************************
* @brief    pbx unregister....
* @section  DESC Description
*   - desc
*****************************************************************************/
void app_voip_stop(void)
{
	__call_send_cmd(APP_CMD_STOP);
}

/*****************************************************************************
* @brief    
*   - desc
*       : user가 call 버튼을 눌렀을 때 실행. 연속으로 버튼일 눌리는 걸 방지해야 함.
*         voip_main thread로 NOTY 전달. __call_event_handler() 가 실행됨.
*****************************************************************************/
void app_voip_event_noty(void)
{
	if (!ivoip->st.call_reg) {
		eprintf("Not registered!\n");
		return;
	}
	__call_send_cmd(APP_CMD_NOTY);
}

/*****************************************************************************
* @brief    voip playback volume control
*   - desc
*****************************************************************************/
void app_voip_set_play_volume(void)
{
	int level = ivoip->snd_level;
	
	if (!ivoip->st.call_reg) {
		eprintf("Not registered!\n");
		return;
	}
	
	level++;
	/* round */
	if (level > SND_LEVEL_HIGH)
		level = SND_LEVEL_LOW;
	else if (level < SND_LEVEL_LOW) /* ??? */
		level = SND_LEVEL_LOW;
	
	ivoip->snd_level = level;
	
	__call_send_cmd(APP_CMD_PAUSE);
}

/*****************************************************************************
* @brief    save to storage playback volume 
*   - desc
*****************************************************************************/
void app_voip_save_config(void)
{
	int level = ivoip->snd_level;
	__set_voip_play_volume(level);
	dprintf("last sound level %d saved!\n", level);
}

/*****************************************************************************
* @brief    
*   - desc
*       : PBX에 등록이 성공되었는지 확인.
*         voip_main thread로 NOTY 전달. __call_event_handler() 가 실행됨.
*****************************************************************************/
int app_voip_is_registered(void)
{
	int res = 0;
	
	if (ivoip->st.call_reg)
		res = 1; 
	return res;	
}
#endif /* end of #ifdef SYS_CONFIG_VOIP */
