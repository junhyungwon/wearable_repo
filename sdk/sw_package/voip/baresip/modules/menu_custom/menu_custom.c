/**
 * @file menu_custom/c_menu.c  communication between main process.
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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <linux/input.h>

#include "sipc_ipc_cmd_defs.h"
#include "msg.h"
#include "alsa_mixer.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define UA_PREFIX				"/uanew" /* long command 사용*/
#define DIAL_PREFIX				"/dial" /* long command 사용*/

typedef struct {
	app_thr_obj mObj;
	app_thr_obj sObj; /* baresip에서 main으로 메세지를 전달할 경우 */
	
	pthread_mutex_t lock;
	int qid;
	
	sipc_uri_t uri;
	sipc_status_t ste;
	
	struct play *play; 			  /**< Current audio player state     */
	struct tmr tmr_stat;          /**< Call status timer              */
	struct mbuf *dialbuf;         /**< Buffer for dialled number      */
	char bell;         			  /**< ANSI Bell alert enabled        */
	
	uint64_t start_ticks;         /**< Ticks when app started         */
	
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
//---------------------------- UI CALL HELPER --------------------------------------------------
static const char *translate_errorcode(uint16_t scode)
{
	switch (scode) {
	case 404: return "notfound.wav";
	case 486: return "busy.wav";
	case 487: return NULL; /* ignore */
	default:  return "error.wav";
	}
}

static char have_active_calls(void)
{
	struct le *le;

	for (le = list_head(uag_list()); le; le = le->next) {
		struct ua *ua = le->data;
		if (ua_call(ua))
			return 1;
	}

	return 0;
}

static void tmrstat_handler(void *arg)
{
	struct call *call;
	(void)arg;

	/* the UI will only show the current active call */
	call = ua_call(uag_current());
	if (!call)
		return;

	tmr_start(&ikey->tmr_stat, 100, tmrstat_handler, 0);

	(void)re_fprintf(stderr, "%H\r", call_status, call);
}

static void update_callstatus(void)
{
	/* if there are any active calls, enable the call status view */
	if (have_active_calls())
		tmr_start(&ikey->tmr_stat, 100, tmrstat_handler, 0);
	else
		tmr_cancel(&ikey->tmr_stat);
}

/*
 * user agent에 의해서 이벤트가 발생되면 callback 함수로 호출됨.
 */ 
static void ua_event_handler(struct ua *ua, enum ua_event ev,
			     struct call *call, const char *prm, void *arg)
{
	struct player *player = baresip_player();
	uint16_t err_code = 0;
	struct config *cfg;
	
	(void)prm;
	(void)arg;
	
	dprintf("c_menu: [ ua=%s call=%s ] event: %s (%s)\n",
	      ua_aor(ua), call_id(call), uag_event_str(ev), prm);
	
	cfg = conf_config();
		
	switch (ev) {
	case UA_EVENT_REGISTERING:
		/* 최초 프로그램 시작 시 전달되는 메시지 */
		return;
	case UA_EVENT_CALL_INCOMING:
		/* stop any ringtones (auplay_destructor함수가 mem_deref에 의해서 호출됨) */
		ikey->play = mem_deref(ikey->play);
		
		ikey->ste.call_ste = SIPC_STATE_CALL_INCOMING;
		ikey->ste.call_dir = call_is_outgoing(call);
		ikey->ste.call_reg = 1;
		ikey->ste.call_err = 0;
		
		/* set the current User-Agent to the one with the call */
		uag_current_set(ua);
		debug("%s: Incoming call from: %s %s\n",
			ua_aor(ua), call_peername(call), call_peeruri(call));
		
		/* 수신 중일 경우 */
		if (list_count(ua_calls(ua)) > 1) {
			/* TODO */
			//(void)play_file(&menu.play, player, "callwaiting.wav", 3);
		} else {
			/* Alert user struct player; */
			play_file(&ikey->play, player, "ring.wav", -1,
					cfg->audio.alert_mod, cfg->audio.alert_dev);
		}
		//if (ikey->bell)
			//alert_start(0);
		break;
	
	case UA_EVENT_CALL_RINGING:
		/* stop any ringtones */
		ikey->play = mem_deref(ikey->play);
		/* play ringback */
		(void)play_file(&ikey->play, player, 
					"ringback.wav", -1, cfg->audio.play_mod, cfg->audio.play_dev);
		
		ikey->ste.call_ste = SIPC_STATE_CALL_RINGING;
		ikey->ste.call_dir = call_is_outgoing(call);
		ikey->ste.call_reg = 1;
		ikey->ste.call_err = 0;
		break;
	
	case UA_EVENT_CALL_ESTABLISHED:
		/* stop any ringtones */
		ikey->play = mem_deref(ikey->play);
		
		ikey->ste.call_ste = SIPC_STATE_CALL_ESTABLISHED;
		ikey->ste.call_dir = call_is_outgoing(call);
		ikey->ste.call_reg = 1;
		ikey->ste.call_err = 0;
		break;
	
	case UA_EVENT_CALL_CLOSED:
		/* stop any ringtones */
		ikey->play = mem_deref(ikey->play);
		
		err_code = call_scode(call);
		ikey->ste.call_ste = SIPC_STATE_CALL_IDLE;
		ikey->ste.call_dir = 0;
		ikey->ste.call_reg = 1;
		ikey->ste.call_err = (int)err_code;
		if (err_code) {
			const char *tone;
			tone = translate_errorcode(err_code);
			if (tone) {
				/* TODO */
				//(void)play_file(&menu.play, player, tone, 1);
			}
		}
		break;
	
	case UA_EVENT_REGISTER_OK:
		/* 최초 resister ok event가 전송됨 */ 
		ikey->ste.call_ste = SIPC_STATE_CALL_IDLE;
		ikey->ste.call_dir = 0;
		ikey->ste.call_reg = 1;
		ikey->ste.call_err = 0;
		break;
	
	case UA_EVENT_REGISTER_FAIL:
		ikey->ste.call_ste = SIPC_STATE_CALL_IDLE;
		ikey->ste.call_dir = 0;
		ikey->ste.call_reg = 0;
		ikey->ste.call_err = 0;
		break;
	
	case UA_EVENT_CALL_REMOTE_SDP:
		ikey->play = mem_deref(ikey->play);
		return;
		
	//case UA_EVENT_AUDIO_STOP:
		/* stop any ringtones (auplay_destructor함수가 mem_deref에 의해서 호출됨) */
	//	ikey->play = mem_deref(ikey->play);
	//	return;
		
	case UA_EVENT_CALL_TRANSFER:
	case UA_EVENT_CALL_TRANSFER_FAILED:
		/* 전화 돌려주기 기능 */
	case UA_EVENT_CALL_RTCP:
		/* 통화 연결 후 RTCP 이벤트가 전송됨 */
	case UA_EVENT_UNREGISTERING:
		return;
	
	default:
		warning("unknown ua_event %x\n", (int)ev);
		return;
	}
	
	/* poll 쓰레드로 메시지 전달 */
	event_send(&ikey->sObj, APP_CMD_NOTY, 0, 0);
	//# for debugging
	//update_callstatus();
}
//-----------------------------------------------------------------------------------------------
//###################### Baresip Helper ########################################################
static int __create_user(const char *call_num, const char *server_addr, const char *passwd)
{
	struct ua *ua = NULL;
	int err = 0;
	
	memset(ui_buf, 0, sizeof(ui_buf));
	
	//# <sip:1006@192.168.0.5>;auth_pass=1234
	snprintf(ui_buf, sizeof(ui_buf), "%s <sip:%s@%s>;auth_pass=%s", 
					UA_PREFIX, call_num, server_addr, passwd);
	dprintf("Creating UA for %s ...\n", ui_buf);
	
	err = ua_alloc(&ua, ui_buf);
	if (err) {
		eprintf("custom_menu: create_ua failed: %x\n", err);
		return -1;
	}
			
	if (account_regint(ua_account(ua))) {
		(void)ua_register(ua);
	}
	
	return 0;
}

static int __dialer_user(const char *call_num)
{
	int err = 0;
	
	/* redial 기능을 위해서 필요함. 현재는 사용안됨 */
	//mbuf_rewind(ikey->dialbuf);
	//(void)mbuf_write_str(ikey->dialbuf, call_num);
	
	err = ua_connect(uag_current(), NULL, NULL, call_num, VIDMODE_OFF);
	if (err) {
		warning("menu: ua_connect failed: %m\n", err);
	}

	return err;
}

static void __call_stop(void)
{
	/* Stop any ongoing ring-tones */
	ikey->play = mem_deref(ikey->play);
	ua_hangup(uag_current(), NULL, 0, NULL);
}

static void __call_answer(void)
{
	struct ua *ua = uag_current();
	
	/* Stop any ongoing ring-tones */
	ikey->play = mem_deref(ikey->play);
	ua_hold_answer(ua, NULL);
}

static int send_msg(int cmd, int state, int dir, int reg, int err)
{
	to_sipc_main_msg_t msg;
	
	msg.type = SIPC_MSG_TYPE_TO_MAIN;
	msg.cmd = cmd;
	
	msg.ste.call_ste = state;
	msg.ste.call_dir = dir;
	msg.ste.call_reg = reg;
	msg.ste.call_err = err;
	
	return Msg_Send(ikey->qid, (void *)&msg, sizeof(to_sipc_main_msg_t));
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
		memset(ikey->uri.ua_uri, 0, sizeof(ikey->uri.ua_uri));
		memset(ikey->uri.pbx_uri, 0, sizeof(ikey->uri.pbx_uri));
		memset(ikey->uri.passwd, 0, sizeof(ikey->uri.passwd));
	
		strcpy(ikey->uri.ua_uri, msg.uri.ua_uri);
		strcpy(ikey->uri.pbx_uri, msg.uri.pbx_uri);
		strcpy(ikey->uri.passwd, msg.uri.passwd);
	} 
	else if (msg.cmd == SIPC_CMD_SIP_START) {
		memset(ikey->uri.peer_uri, 0, sizeof(ikey->uri.peer_uri));
		strcpy(ikey->uri.peer_uri, msg.uri.peer_uri);
	}
	
	return msg.cmd;
}

static void *THR_sipc_poll(void *prm)
{
	app_thr_obj *tObj = &ikey->sObj;
	int done = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while(!done)
	{
		cmd = event_wait(tObj);
		if (cmd == APP_CMD_EXIT) {
			break;
		} 
		//# APP_CMD_NOTY
		send_msg(SIPC_CMD_SIP_GET_STATUS, ikey->ste.call_ste, 
					ikey->ste.call_dir, ikey->ste.call_reg, ikey->ste.call_err);
	}
	
	aprintf("exit...\n");
	tObj->active = 0;
		
	return NULL;
}

static void *THR_sipc_main(void *prm)
{
	app_thr_obj *tObj = &ikey->mObj;
	int done = 0, cmd;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	/* message queue init */
	ikey->qid = Msg_Init(SIPC_MSG_KEY);
	send_msg(SIPC_CMD_SIP_READY, 0, 0, 0, 0);
	
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
			__create_user(ikey->uri.ua_uri, ikey->uri.pbx_uri, ikey->uri.passwd);
			break;
		
		case SIPC_CMD_SIP_START:
			info("baresip peer number %s\n", ikey->uri.peer_uri);
			__dialer_user(ikey->uri.peer_uri);
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
	
	aprintf("exit...\n");
		
	return NULL;
}

//#########################################################################################################
static int module_init(void)
{
	app_thr_obj *tObj = &ikey->mObj;
	int err;
	
	ikey->bell = 1; /* 전화 수신 시 bell을 출력할 필요가 있을 경우 */
	ikey->start_ticks = tmr_jiffies();
	
	ikey->dialbuf = mbuf_alloc(64);
	if (!ikey->dialbuf)
		return ENOMEM;
		
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
	
	/* Create Mutex Handle */
	err = pthread_mutex_init(&ikey->lock, NULL);
	err = uag_event_register(ua_event_handler, NULL);
	if (err)
		return err;
	
	/* alsa mixer config */
	amixer_set_volume(SND_VOLUME_C, 80);
	amixer_set_volume(SND_VOLUME_P, 0);
	
	amixer_set_input_path();
	amixer_set_output_path();
	
	amixer_set_volume(SND_VOLUME_P, 90);
		
	return 0;
}

static int module_close(void)
{
	app_thr_obj *tObj = &ikey->mObj;
	
   	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		delay_msecs(20);

    thread_delete(tObj);
	pthread_mutex_destroy(&ikey->lock);
	uag_event_unregister(ua_event_handler);
	
	ikey->play = mem_deref(ikey->play);
	ikey->dialbuf = mem_deref(ikey->dialbuf);
	
	return 0;
}

/* config 파일의 module_app에 등록됨 */
const struct mod_export DECL_EXPORTS(menu_custom) = {
	"menu_custom",
	"application",
	module_init,
	module_close
};
