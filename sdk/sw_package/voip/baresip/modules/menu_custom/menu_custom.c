/**
 * @file menu_custom/c_menu.c  communication between main process.
 *
 * Copyright (C) 2020 LF
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>	//# waitpid
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
//#define DBG_CALL_STAT
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
	char *callid;                 /**< Call-id of active call         */
	
	uint64_t start_ticks;         /**< Ticks when app started         */
	
} key_config_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static key_config_t key_obj_t;
static key_config_t *ikey = &key_obj_t;
static char ui_buf[2048];

/* aic3x audio codec output volume percentage */
static int __aic3x_output_level[3] = {
	/*45, 50, 75*/ 	
	60, 70, 90
};

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/
static int ipc_send_msg(int cmd, int state, int dir, int reg, int err);
static int ipc_recv_msg(void);
static void ipc_set_snd_vol(void);

//-----------------------------------------------------------------------------------------------
//###################### Baresip Helper ########################################################
static int cmenu_get_rndis_type(void) 
{
	int res = 1; /* default usb0 */
	
	if (0 == access("/sys/class/net/eth1/operstate", R_OK)) {
		res = 0;
	} 
	
	return res;	
}

static void cmenu_call_stop(void)
{
	struct ua *ua = call_get_ua(uag_call_find(ikey->callid));
	
	if (ua != NULL) {
		/* Stop any ongoing ring-tones */
		ikey->play = mem_deref(ikey->play);
		ua_hangup(ua, NULL, 0, NULL);
	}
}

static void cmenu_call_answer(void)
{
	struct ua *ua = call_get_ua(uag_call_find(ikey->callid));
	
	/* Stop any ongoing ring-tones */
	ikey->play = mem_deref(ikey->play);
	ua_answer(ua, NULL, VIDMODE_OFF);
}

static int cmenu_register_user(int network, int enable, short port, int level, const char *call_num, const char *server_addr, 
			const char *passwd, const char *stun_domain)
{
	struct network *net = baresip_network();
	char devname[16] = {0,};
	
	struct ua *ua = NULL;
	int err = 0, percent;
	struct account *acc;
	
	/* set alsa output level */
	percent = __aic3x_output_level[level];	
	alsa_mixer_set_volume(SND_VOLUME_P, percent);
//	printf("set voip default sound volume %d,%d(percent)!\n", level, percent);

	memset(ui_buf, 0, sizeof(ui_buf));
	memset(devname, 0, sizeof(devname));
	
	if (network == SIPC_NET_TYPE_WLAN) {
		strcpy(devname, "wlan0");
	} 
	else if (network == SIPC_NET_TYPE_RNDIS) {
		if (cmenu_get_rndis_type() == 1) {
			strcpy(devname, "usb0");
		} else {
			strcpy(devname, "eth1");
		}
	} else {
		strcpy(devname, "eth1");
	}
	
	net_set_ifname(net, (const char *)devname);
	delay_msecs(500); /* wait 0.5s */
	//######## IP-CHANGE-HANDLER #######################################
	net_check(net);
	info("IP-address changed: %j\n", net_laddr_af(net, AF_INET));
	(void)uag_reset_transp(true, true);
	//#####################################################################
			 
	//# <sip:1006@192.168.0.5>;auth_pass=1234
	if (enable) {
		/* stun server enable */
		snprintf(ui_buf, sizeof(ui_buf), "%s <sip:%s@%s:%d;transport=tcp>;auth_pass=%s;stunserver=stun:%s;medianat=stun", 
					UA_PREFIX, call_num, server_addr, port, passwd, stun_domain);
//	snprintf(ui_buf, sizeof(ui_buf), "%s <sip:%s@%s:%d;transport=tcp>;auth_pass=%s;stunserver=stun:52.78.124.88:3478;medianat=ice;stunuser=test;stunpass=test", 
//					UA_PREFIX, call_num, server_addr, port, passwd);
	} else {
		snprintf(ui_buf, sizeof(ui_buf), "%s <sip:%s@%s:%d;transport=tcp>;auth_pass=%s", 
					UA_PREFIX, call_num, server_addr, port, passwd);	
	}
	
	sysprint("Creating UA for %s ....\n", ui_buf);
	err = ua_alloc(&ua, ui_buf);
	if (err) {
		aprintf("create_ua failed: %x\n", err);
		return -1;
	}
	
	/* return ua->acc (account) */
	acc = ua_account(ua);
	if (!acc) {
		aprintf("account: no account for this ua\n");
		return ENOENT;
	}
		
	if (account_regint(acc)) {
		if (!account_prio(acc))
			(void)ua_register(ua);
		else
			(void)ua_fallback(ua);
		
		printf("--- User Agents (%u) ---\n", list_count(uag_list()));
	}
	
	return 0;
}

static void cmenu_unregister_user(void)
{
	struct le *le;
	
	/* stop active call (네트워크 연결이 안되기 때문에 전송할 필요가 없음 */
	for (le = list_head(uag_list()); le; le = le->next) {
		struct ua *ua = le->data;
		if (ua_isregistered(ua)) 
		{
			struct account *acc = ua_account(ua);
			
			info("unregister %s\n", account_aor(acc));
			ua_unregister(ua);
			//mem_deref(ua);
		}	
	}
	info("unregister done!!\n");
}

static int cmenu_dialer_user(const char *call_num)
{
	struct ua *ua=NULL;
	struct player *player = baresip_player();
	int err = 0;
	struct config *cfg;
	struct mbuf *uribuf = NULL;
	struct call *call;
	char *uri=NULL;
	
	cfg = conf_config();
	
	aprintf("cmenu: make a call uri->%s\n", call_num);
	
	/* device number와 peer number가 같은 경우 error */
	if (strcmp(ikey->uri.ua_uri, call_num) == 0) {
		aprintf("cmenu: Don't allow same number between call and ua!\n");
		
		ikey->play = mem_deref(ikey->play);
		(void)play_file(&ikey->play, player, 
						"error.wav", 1, cfg->audio.play_mod, cfg->audio.play_dev);
		delay_msecs(1000);
		ikey->play = mem_deref(ikey->play);
		err = -1;
	} else {
		//# 8617001 형태로 입력
		ua = uag_find_requri(call_num);
		if (!ua) {
			sysprint("could not find UA for %s\n", call_num);
			return -1;
		}
		
		uribuf = mbuf_alloc(64);
		if (!uribuf)
			return -1;
		
		//# 완성형으로 작성해야 함. (sip:8617001@52.78.124.88:6060)
		err = account_uri_complete(ua_account(ua), uribuf, call_num);
		if (err) {
			sysprint("ua_connect failed to complete uri\n");
			return -1;
		}
		
		uribuf->pos = 0;
		err = mbuf_strdup(uribuf, &uri, uribuf->end);
		if (err)
			goto out;
		
		aprintf("cmenu: account uri complete ->%s\n", uri);	
		err = ua_connect(ua, &call, NULL, uri, VIDMODE_OFF);
		if (err) {
			sysprint("cmenu: ua_connect failed!\n");
			goto out;
		}
		
		aprintf("peer call id: %s\n", call_id(call));
	}

out:
	mem_deref(uribuf);
	mem_deref(uri);
	return err;
}

static void check_registrations(void)
{
	static bool ual_ready = false;
	struct le *le;
	uint32_t n;

	if (ual_ready)
		return;

	for (le = list_head(uag_list()); le; le = le->next) {
		struct ua *ua = le->data;

		if (!ua_isregistered(ua) && !account_prio(ua_account(ua)))
			return;
	}

	n = list_count(uag_list());
	/* We are ready */
	printf("%u useragents registered successfully!\n", n);
	ual_ready = true;
}

static const char *translate_errorcode(uint16_t scode)
{
	switch (scode) {
	case 486: 
		return "busy.wav";
	case 404: /* notfound.wav */
	case 487: /* ignore */
	default:  
		return "error.wav";
	}
}

static struct call *cmenu_find_call_state(enum call_state st)
{
	struct le *le;

	for (le = list_head(uag_list()); le; le = le->next) {
		struct ua *ua = le->data;
		struct call *call = ua_find_call_state(ua, st);

		if (call)
			return call;
	}

	return NULL;
}

static void cmenu_selcall(struct call *call)
{
	int i;
	enum call_state state[] = {
		CALL_STATE_INCOMING,
		CALL_STATE_OUTGOING,
		CALL_STATE_RINGING,
		CALL_STATE_EARLY,
		CALL_STATE_ESTABLISHED,
	};

	if (!call) {
		/* select another call */
		for (i = ARRAY_SIZE(state)-1; i >= 0; --i) {
			call = cmenu_find_call_state(state[i]);

			if (!str_cmp(call_id(call), ikey->callid))
				call = NULL;

			if (call)
				break;
		}
	}

	ikey->callid = mem_deref(ikey->callid);

	if (call) {
		str_dup(&ikey->callid, call_id(call));
		call_set_current(ua_calls(call_get_ua(call)), call);
	}
}

static bool cmenu_active_call_test(const struct call* call)
{
	return call_state(call) == CALL_STATE_ESTABLISHED && 
				!call_is_onhold(call);
}

static void cmenu_find_first_call(struct call *call, void *arg)
{
	struct call **callp = arg;

	if (!*callp)
		*callp = call;
}

static struct call *cmenu_find_call(call_match_h *matchh)
{
	struct call *call = NULL;

	uag_filter_calls(cmenu_find_first_call, matchh, &call);

	return call;
}

static void cmenu_stop_play(void)
{
	ikey->play = mem_deref(ikey->play);
}

static void cmenu_set_ipc_state(int ste, int dir, int reg, int err)
{
	ikey->ste.call_ste = ste;
	ikey->ste.call_dir = dir;
	ikey->ste.call_reg = reg;
	ikey->ste.call_err = err;
}

/*
 * user agent에 의해서 이벤트가 발생되면 callback 함수로 호출됨.
 */ 
static void ua_event_handler(struct ua *ua, enum ua_event ev,
			     struct call *call, const char *prm, void *arg)
{
	struct player *player = baresip_player();
	struct account *acc = ua_account(ua);
	uint32_t count;
	uint16_t err_code = 0;
	struct config *cfg;
	
	(void)prm;
	(void)arg;
	
	/*
	 * ignore events..
	 * 최초 프로그램 시작 시 전달되는 메시지 : UA_EVENT_REGISTERING
	 * 전화 돌려주기 기능                  : UA_EVENT_CALL_TRANSFER, UA_EVENT_CALL_TRANSFER_FAILED 
	 *                                    : UA_EVENT_CALL_RTCP
	 * 음성(RTP) 연결됨                    : UA_EVENT_CALL_RTPESTAB
	 */
	if ((ev == UA_EVENT_REGISTERING)   || 
		(ev == UA_EVENT_CALL_TRANSFER) ||
		(ev == UA_EVENT_CALL_TRANSFER_FAILED) || 
		(ev == UA_EVENT_CALL_RTCP) ||
		(ev == UA_EVENT_CALL_RTPESTAB))
	{
		return;
	}
	
	pthread_mutex_lock(&ikey->lock);
	cfg = conf_config();
	
	aprintf("[ ua=%s call=%s ] event: %s (%s)\n",
	      account_aor(acc), call_id(call), uag_event_str(ev), prm);
	
	/* 진행 중인 통화 갯수 */
	count = uag_call_count();
		
	switch (ev) {
	case UA_EVENT_CALL_INCOMING:
		/* set the current User-Agent to the one with the call */
		cmenu_selcall(call);
		
		/* stop any ringtones (auplay_destructor함수가 mem_deref에 의해서 호출됨) */
		cmenu_stop_play();
		cmenu_set_ipc_state(SIPC_STATE_CALL_INCOMING, call_is_outgoing(call), 1, 0);
		info("%s: Incoming call from: %s %s\n", account_aor(acc), 
				call_peername(call), call_peeruri(call));
		
		/* 수신 중일 경우 */
		if (cmenu_find_call(cmenu_active_call_test)) {
			/* TODO */
			//(void)play_file(&menu.play, player, "callwaiting.wav", 3);
		} else {
			/* Alert user struct player; */
			play_file(&ikey->play, player, "ring.wav", -1,
					cfg->audio.alert_mod, cfg->audio.alert_dev);
		}
		break;
	
	case UA_EVENT_CALL_RINGING:
		cmenu_selcall(call);
		/* stop any ringtones */
		cmenu_stop_play();
		/* play ringback (-1 repeat) */
		(void)play_file(&ikey->play, player, 
					"ringback.wav", -1, cfg->audio.play_mod, cfg->audio.play_dev);
		cmenu_set_ipc_state(SIPC_STATE_CALL_RINGING, call_is_outgoing(call), 1, 0);
		break;
	
	case UA_EVENT_CALL_PROGRESS:
		cmenu_selcall(call);
		cmenu_stop_play();
		break;
	
	case UA_EVENT_CALL_ANSWERED:
		cmenu_stop_play();
		break;
		
	case UA_EVENT_CALL_ESTABLISHED:
		cmenu_selcall(call);
		/* stop any ringtones */
		cmenu_stop_play();
		cmenu_set_ipc_state(SIPC_STATE_CALL_ESTABLISHED, call_is_outgoing(call), 1, 0);
		break;
	
	case UA_EVENT_CALL_CLOSED:
		/* stop any ringtones */
		cmenu_stop_play();
		cmenu_selcall(NULL);
		
		err_code = call_scode(call);
		cmenu_set_ipc_state(SIPC_STATE_CALL_IDLE, 0, 1, (int)err_code);
		if (err_code) {
			const char *tone;
			tone = translate_errorcode(err_code);
			if (tone) {
				play_file(&ikey->play, player, 
						tone, 1, cfg->audio.play_mod, cfg->audio.play_dev);
			}
		} else {
			(void)play_file(&ikey->play, player, 
					"audio_end.wav", 1, cfg->audio.play_mod, cfg->audio.play_dev);
		}
		break;
	
	/* Remote session description protocol */
	case UA_EVENT_CALL_REMOTE_SDP: 
		cmenu_stop_play();
		if (!str_cmp(prm, "answer") && call_state(call) == CALL_STATE_ESTABLISHED) {
			cmenu_selcall(call);
		}
		pthread_mutex_unlock(&ikey->lock);
		return;
	
	/* 
	 * Device에서 상대방에 전화를 걸때 내 장치의 
	 * Session 상태를 확인하기 위한 이벤트
	 * Local session description protocol
	 */ 
	case UA_EVENT_CALL_LOCAL_SDP:
		ikey->ste.call_ste = SIPC_STATE_CALL_LOCAL_SDP;
		break;
		
	case UA_EVENT_REGISTER_OK:
		/* 통화대기 중 이 REGISTER OK 메세지가 전달될 경우에 hang 걸림 */
		if (cmenu_find_call_state(CALL_STATE_ESTABLISHED) != NULL) {
			aprintf("invalid REGISTER_OK.....ignore!!\n");
			pthread_mutex_unlock(&ikey->lock);
			return;
		}
		check_registrations();
		/* 최초 resister ok event가 전송됨 */
		cmenu_set_ipc_state(SIPC_STATE_CALL_IDLE, 0, 1, 0);
		break;
	
	case UA_EVENT_UNREGISTERING:
		ikey->ste.call_ste = SIPC_STATE_CALL_IDLE;
		cmenu_set_ipc_state(SIPC_STATE_CALL_IDLE, 0, 0, 0);
		break;
		
	case UA_EVENT_REGISTER_FAIL:
		if (strcmp(prm, "Connection timed out") == 0) {
			/* ETIMEOUT (네트워크에 문제가 있음 */
		} else if (strcmp(prm, "Protocol not supported") == 0) {
			/* 등록이 진행 중임 */
		} else if (strcmp(prm, "No route to host") == 0) {
			/* 서버 연결이 안 됨 */
		}
		cmenu_set_ipc_state(SIPC_STATE_CALL_IDLE, 0, 0, 0);
		break;
	
	default:
		aprintf("unknown ua_event 0x%x\n", (int)ev);
		pthread_mutex_unlock(&ikey->lock);
		return;
	}
	
	/* poll 쓰레드로 메시지 전달 */
	event_send(&ikey->sObj, APP_CMD_NOTY, 0, 0);
	pthread_mutex_unlock(&ikey->lock);
}

//-------------------------------------------IPC EVENT Handler --------------------------------------------------------------------------
static int ipc_send_msg(int cmd, int state, int dir, int reg, int err)
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

static int ipc_recv_msg(void)
{
	to_sipc_msg_t msg;
	
	//# blocking
	if (Msg_Rsv(ikey->qid, SIPC_MSG_TYPE_TO_CLIENT, (void *)&msg, sizeof(to_sipc_msg_t)) < 0) {
		return -1;
	}
	
	if (msg.cmd == SIPC_CMD_SIP_REGISTER_UA) {
		/* clear user information */
		memset(ikey->uri.ua_uri, 0, sizeof(ikey->uri.ua_uri));
		memset(ikey->uri.pbx_uri, 0, sizeof(ikey->uri.pbx_uri));
		memset(ikey->uri.passwd, 0, sizeof(ikey->uri.passwd));
		memset(ikey->uri.stun_uri, 0, sizeof(ikey->uri.stun_uri));
	
		strcpy(ikey->uri.ua_uri, msg.uri.ua_uri);
		strcpy(ikey->uri.pbx_uri, msg.uri.pbx_uri);
		strcpy(ikey->uri.passwd, msg.uri.passwd);
		strcpy(ikey->uri.stun_uri, msg.uri.stun_uri);
		
		ikey->uri.en_stun = msg.uri.en_stun;
		ikey->uri.port = msg.uri.port;
		ikey->uri.net_if = msg.uri.net_if;
		ikey->uri.spk_lv = msg.uri.spk_lv;
	} 
	else if (msg.cmd == SIPC_CMD_SIP_START) {
		memset(ikey->uri.peer_uri, 0, sizeof(ikey->uri.peer_uri));
		strcpy(ikey->uri.peer_uri, msg.uri.peer_uri);
	}
	else if (msg.cmd == SIPC_CMD_SIP_SET_SOUND) {
		ikey->uri.spk_lv = msg.uri.spk_lv;
	}
	
	return msg.cmd;
}

static void ipc_set_snd_vol(void)
{
	//	static struct re_printf pf_stderr = {print_handler, NULL};
	struct player *player = baresip_player();
	struct config *cfg;
	int level = ikey->uri.spk_lv;
	int percent;
	
	cfg = conf_config();
	
	percent = __aic3x_output_level[level];	
	alsa_mixer_set_volume(SND_VOLUME_P, percent);
	
	if (ikey->ste.call_ste == SIPC_STATE_CALL_IDLE) {
		/* Stop any ongoing ring-tones */
		ikey->play = mem_deref(ikey->play);
		(void)play_file(&ikey->play, player, 
						"audio_end.wav", 1, cfg->audio.play_mod, cfg->audio.play_dev);
		delay_msecs(1000);
		ikey->play = mem_deref(ikey->play);				
	}
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
		ipc_send_msg(SIPC_CMD_SIP_GET_STATUS, ikey->ste.call_ste, 
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
	(void)prm;
	
	/* message queue init */
	ikey->qid = Msg_Init(SIPC_MSG_KEY);
	ipc_send_msg(SIPC_CMD_SIP_READY, 0, 0, 0, 0);
	
	while(!done)
	{
		//# wait cmd
		cmd = ipc_recv_msg();
		if (cmd < 0) {
			printf("failed to receive sipc process msg!\n");
			continue;
		}
		
		info("Received command is 0x%x\n", cmd);
		switch (cmd) {
		case SIPC_CMD_SIP_REGISTER_UA:
			/* 계정을 등록 */
			cmenu_register_user(ikey->uri.net_if, ikey->uri.en_stun, ikey->uri.port, ikey->uri.spk_lv, ikey->uri.ua_uri, 
					ikey->uri.pbx_uri, ikey->uri.passwd, ikey->uri.stun_uri);
			break;
		
		case SIPC_CMD_SIP_UNREGISTER_UA:
			cmenu_unregister_user();
			break;
			
		case SIPC_CMD_SIP_START:
			cmenu_dialer_user(ikey->uri.peer_uri);
			break;
		
		case SIPC_CMD_SIP_ANSWER:
			cmenu_call_answer();
			break;
			
		case SIPC_CMD_SIP_STOP:
			cmenu_call_stop();
			break;	
		
		case SIPC_CMD_SIP_SET_SOUND:
			ipc_set_snd_vol();
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
	
	aprintf("menu_custom: init!\n");
	
	ikey->start_ticks = tmr_jiffies();
	ikey->dialbuf = mbuf_alloc(64);
	if (!ikey->dialbuf)
		return ENOMEM;
		
	/* message 수신 및 전송을 위한 Thread 생성 */
	if (thread_create(tObj, THR_sipc_main, APP_THREAD_PRI, NULL) < 0) {
		printf("create thread!\n");
		return -1;
	}
	
	/* baresip의 event_handler에 의해서 전달되는 상태메시지를 main으로 전달하기 위한 thread */
	tObj = &ikey->sObj;
	if (thread_create(tObj, THR_sipc_poll, APP_THREAD_PRI-1, NULL) < 0) {
		printf("create thread!\n");
		return -1;
	}
	
	/* Create Mutex Handle */
	err = pthread_mutex_init(&ikey->lock, NULL);
	err = uag_event_register(ua_event_handler, NULL);
	if (err)
		return err;
		
	return 0;
}

static int module_close(void)
{
	app_thr_obj *tObj = &ikey->mObj;
	
	aprintf("menu_custom: close\n");
	
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
