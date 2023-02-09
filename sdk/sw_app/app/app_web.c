/******************************************************************************
 * FITT Board
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_web.c
 * @brief   
 * @author  
 * @section MODIFY history
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include "app_main.h"
#include "app_comm.h"
#include "app_web.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define USE_POPEN 1

int deleteSelfSignedCertificate()
{
	unlink(PATH_HTTPS_SS_KEY_NAND);
	unlink(PATH_HTTPS_SS_CRT_NAND);
	unlink(PATH_HTTPS_SS_PEM_NAND);

	TRACE_INFO("Deleted SSC files\n");
	
	return 0;
}

int createSelfSignedCertificate(char *path_key, char* path_crt, bool force)
{
	char cmd[256];

	if( access(PATH_SSL_ROOT_NAND , F_OK) != 0) {
		mkdir(PATH_SSL_ROOT_NAND, 0775);
	}

	TRACE_INFO("createSelfSignedCertificate. force = %d\n", force);
	if (force) {
		deleteSelfSignedCertificate();
	} else {
		if( access(path_crt, R_OK) == 0 && access(path_key, R_OK) == 0) {
			TRACE_INFO("Self Signed Certificate is already created.\n");

			// copy to /tmp
			app_web_https_copy_to_tmp();
			return SUCC;
		}
	}

	// 입력된 정보가 올바르지 않을때, 기본으로 생성함
	if ( strlen(app_set->net_info.ssc_C) == 0 
	|| strlen(app_set->net_info.ssc_C) > 4 
	|| strlen(app_set->net_info.ssc_O)  == 0 
	|| strlen(app_set->net_info.ssc_CN) == 0 ) {
		TRACE_INFO("Make default Self Signed Certificate\n");

		sprintf(cmd, "openssl req -new -x509 -days 365 -sha256 -newkey rsa:2048 -nodes \
		              -keyout %s -out %s -subj \'/C=KR/O=%s/CN=%s/\' -config /etc/ssl/openssl.cnf",
		path_key, path_crt, "Linkflow Corporation", "linkflow.co.kr");
	}
	else {
		sprintf(cmd, "openssl req -new -x509 -days 365 -sha256 -newkey rsa:2048 -nodes \
		              -keyout %s -out %s -subj \'/C=%s/L=%s/ST=%s/O=%s/OU=%s/CN=%s/\' -config /etc/ssl/openssl.cnf",
		path_key, path_crt,
		app_set->net_info.ssc_C,
		app_set->net_info.ssc_L,
		app_set->net_info.ssc_ST,
		app_set->net_info.ssc_O,
		app_set->net_info.ssc_OU,
		app_set->net_info.ssc_CN);
	}

	TRACE_INFO("Self Signed Certificate, cmd:%s\n", cmd);
	system(cmd);

	if( access(path_crt, R_OK) != 0 || access(path_key, R_OK) != 0) {
		TRACE_INFO("Failed to create Self Signed Certificate\n");
		return FAIL;
	}

	TRACE_INFO("Succeed, create Self Signed Certificate\n");

	// make pem file for lighttpd
	sprintf(cmd, "cat %s %s > %s", path_key, path_crt, PATH_HTTPS_SS_PEM_NAND);
	TRACE_INFO("Make PEM file, cmd:%s\n", cmd);
	system(cmd);

	if( access(PATH_HTTPS_SS_PEM_NAND,  R_OK) != 0 ) {
		TRACE_INFO("Failed to create PEM file\n");
		return FAIL;
	}

	TRACE_INFO("Succeed, create SS PEM\n");

	// copy certs to /sdcard
	app_web_https_copy_to_sdcard();
	// copy to /tmp
	app_web_https_copy_to_tmp();

	return SUCC;
}

int createSignedCertificate()
{
	return 0;
}

int app_web_boot_passwordfile()
{
	// check default
	if( 0 == strcmp(app_set->account_info.webuser.pw, WEB_DEFAULT_PW)){
	}
	else {
		app_web_make_passwordfile( app_set->account_info.webuser.id,
				app_set->account_info.webuser.pw,
				app_set->account_info.webuser.lv,
				app_set->account_info.webuser.authtype);
	}

	return 0;
}

int app_web_make_passwordfile(char *id, char *pw, int lv, int authtype)
{
	char strcmd[1024]={0};
	char stropt[8]={0};

	TRACE_INFO("generating the password file. authtype : %d(%s)\n", authtype, (authtype == 0) ? "basic" : "digest");
	// make passwd file.
	if(authtype == 0){ // basic

		if( 0 == access(PATH_WEB_AUTH_FILE, F_OK))
			sprintf(stropt, "-bm");
		else
			sprintf(stropt, "-bcm");

		sprintf(strcmd, "/opt/fit/bin/htpasswd %s %s %s %s", stropt, PATH_WEB_AUTH_FILE, id, pw);

#if USE_POPEN
		FILE *f = popen(strcmd, "r");
		if (f == NULL) { perror("failed open:"); return -1;}
		pclose(f);
#else
		system(strcmd);
#endif

		return 0; // succeed
	}
	else if(authtype == 1){ // digest (default)
		TRACE_INFO("reset the htdigest password. %s %s %s %s\n", id, STR_WEB_DIGEST_REALM, pw, PATH_WEB_AUTH_FILE);
		// tta인증. htdigest가 md5밖에 지원하지 않으므로, md5sum 과 sha256sum 을 이용하여 쉘로 파일 직접 생성.
		// /opt/fit/bin/htdigest.sh 'admin' 'Authorized User' 'admin' '/tmp/lighttpd.user'
		sprintf(strcmd, "/opt/fit/bin/htdigest.sh '%s' '%s' '%s' '%s'", id, STR_WEB_DIGEST_REALM, pw, PATH_WEB_AUTH_FILE);
		system(strcmd);

		return 0; // succeed
	}

	return -1;
}

int app_web_stop_server()
{
	char *cmd = "/opt/fit/bin/S50lighttpd stop";
#if USE_POPEN
	FILE *fp = popen(cmd, "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd);
#endif

	return 0;
}

int app_web_ssl_setup()
{
if(app_set->net_info.https_mode == 1) {
	createSelfSignedCertificate(PATH_HTTPS_SS_KEY_NAND, PATH_HTTPS_SS_CRT_NAND, false);
} else if(app_set->net_info.https_mode == 2){
	createSignedCertificate();
}

	char *filepath = "/etc/lighttpd/conf.d/ssl.conf";
	int   bIsFile = 0; // 용도: 설정이 ssl enable이지만, certificate File 없을때는 Https모드를 활성화하지 않기 위해...

	FILE *fp = fopen(filepath, "w");
	if(!fp)
		return FAIL;

	char str[255];

	// onvif와 https_port가 공유되고 있음, 분리가 필요한 상황
	//sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", app_set->net_info.https_port);
	fprintf(fp, "server.modules += ( \"mod_openssl\" )\n");
	fprintf(fp, "ssl.disable-client-renegotiation = \"enable\"\n");
	sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", DEFAULT_HTTPS_PORT);
	fputs(str, fp);

	TRACE_INFO("app_set->net_info.https_mode = %d\n", app_set->net_info.https_mode);
	// 경로 정보가 conf에 추가될 경우, https disable로 설정되어도, 저장된 경로에 file이 없다면, lighttpd 실행시 에러남
	if(app_set->net_info.https_mode == 0 || app_set->net_info.https_mode == 1)
	{
		if( access(PATH_HTTPS_SS_PEM_NAND, F_OK)==0) {
			sprintf(str, "ssl.pemfile = \"%s\"\n", PATH_HTTPS_SS_PEM_NAND);
			fputs(str, fp);
			bIsFile = 1;
		} else {
			TRACE_ERR("ssl.pemfile %s not exists!!\n", PATH_HTTPS_SS_PEM_NAND);
		}
	}
	else if (app_set->net_info.https_mode==2)
	{
		if (access(PATH_HTTPS_PEM, F_OK) == 0)
		{
			sprintf(str, "ssl.pemfile = \"%s\"", PATH_HTTPS_PEM);
			fputs(str, fp);
			bIsFile = 1;
		}

		if( access(PATH_HTTPS_CA, F_OK)==0) {
			sprintf(str, "ssl.ca-file = \"%s\"", PATH_HTTPS_CA); // Optional... 실행해봐야할듯
			fputs(str, fp);
		}
	}

	// 항상, ssl enable
	sprintf(str, "ssl.engine = \"%s\"\n", "enable");

	fputs(str, fp);
	fprintf(fp, "ssl.openssl.ssl-conf-cmd = (\"Protocol\" => \"-ALL, TLSv1.2, TLSv1.3\")\n");
	fputs("}\n\n", fp);

	// overwrite(:=) port
	if (app_set->net_info.https_mode != 0) {
		sprintf(str, "server.port := %d\n",  DEFAULT_HTTPS_PORT);
		fputs(str, fp);
	}


	fclose(fp);

	return SUCC;
}

int app_web_start_server()
{
	app_web_ssl_setup();

	char *cmd = "/opt/fit/bin/S50lighttpd start";
#if USE_POPEN
	FILE *fp = popen(cmd, "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd);
#endif
	LOGD("WEB Server starts successfully.\n");
	return 0;
}

int app_web_restart_server()
{
	app_web_ssl_setup();

	char *cmd = "/opt/fit/bin/S50lighttpd restart";
#if USE_POPEN
	FILE *fp = popen(cmd, "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd);
#endif
	LOGD("WEB Server restarts successfully.\n");
	return 0;
}

int app_web_is_passwordfile()
{
	return (0 == access(PATH_WEB_AUTH_FILE, F_OK));
}

int app_telnetd_enable(int en)
{
	char *cmd[] = { "killall telnetd",
		            "/usr/sbin/telnetd -l /bin/sh",
	              };

#if USE_POPEN
	FILE *fp = popen(cmd[en], "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd[en]);
#endif

	return 0;
}

static int __start_routine_create_ssc(void *pargs)
{
	// force to create the self signed certificate by manually
	int ret = createSelfSignedCertificate(PATH_HTTPS_SS_KEY_NAND, PATH_HTTPS_SS_CRT_NAND, true);

	return ret;
}

// thread도 파일들 만든 후, 뒤에 pem파일 생성되었는지 확인...
int app_web_https_create_ssc()
{
	pthread_t tid;
	int ret = pthread_create(&tid,NULL,(void *(*)(void *))__start_routine_create_ssc, NULL);
	if (ret != 0)
	{
		TRACE_INFO("failed pthread_create. ret = %d\r\n", ret);
		return ret;
	}else 
		pthread_detach(tid);

	return ret;
}

int app_web_https_copy_to_sdcard() {
	// 최초, 랜덤 생성
	char passphrase[SHA256_DIGEST_LENGTH*2+1] = {'\0', };
	int passphrase_len, olen;
    if(access(PATH_SSL_PASSPHRASE_NAND, F_OK) !=0) {
		char buf[128];
		urandom_value(buf, 64);	// read 64 bytes
		unsigned char* encoded = (char *)base64_encode(buf, 64, &olen);
		strncpy(passphrase, encoded, olen);

		TRACE_INFO("%s not exists. create with a random bytes. %s\n", PATH_SSL_PASSPHRASE_NAND, passphrase);
		app_rsa_save_passphrase(passphrase);
		free(encoded);
    }
	if (app_rsa_load_passphrase(passphrase, &passphrase_len) != SUCC) {
		return FAIL;
	}

	char cmd[256];
	sprintf(cmd, "mkdir -p %s", PATH_SSL_ROOT_MMC);
	system(cmd);

	// copy a cert
	sprintf(cmd, "cp -f %s %s", PATH_HTTPS_SS_CRT_NAND, PATH_HTTPS_SS_CRT_MMC);
	TRACE_INFO("copy a cert. cmd: %s\n", cmd);
	system(cmd);

	// copy & encrypt a privatekey
	sprintf(cmd, "openssl rsa -aes128 -in %s -passout pass:%s -out %s"
		, PATH_HTTPS_SS_KEY_NAND
		, passphrase
		, PATH_HTTPS_SS_KEY_MMC);
	TRACE_INFO("copy&encrypt a private key. cmd: %s\n", cmd);
	system(cmd);

	return SUCC;
}

int app_web_https_copy_to_tmp() {
	char cmd[256];
	char passphrase[SHA256_DIGEST_LENGTH*2+1] = {'\0', };
	int passphrase_len, olen;

	if (app_rsa_load_passphrase(passphrase, &passphrase_len) != SUCC) {
		return FAIL;
	}

	// copy & encrypt a privatekey to /tmp
	sprintf(cmd, "openssl rsa -aes128 -in %s -passout pass:%s -out %s"
		, PATH_HTTPS_SS_KEY_NAND
		, passphrase
		, PATH_HTTPS_SS_KEY_TMP);
	TRACE_INFO("copy&encrypt a private key to /tmp/ cmd: %s\n", cmd);
	system(cmd);

	return SUCC;
}