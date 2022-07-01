#include <stdio.h>
#include <unistd.h>

#include "app_web.h"
#include "app_set.h"


#define PATH_HTTPS_SSL "/mmc/ssl"
#define PATH_SSL_ROOT  PATH_HTTPS_SSL

// SSC == Self Signed Certificate
#define PATH_HTTPS_SS_KEY 	PATH_HTTPS_SSL"/_ssc_private.key"
#define PATH_HTTPS_SS_CRT 	PATH_HTTPS_SSL"/_ssc_certificate.crt"
#define PATH_HTTPS_SS_PEM 	PATH_HTTPS_SSL"/_ssc_server.pem"

// Normal SSLs
#define PATH_HTTPS_KEY 	PATH_HTTPS_SSL"/private.key"
#define PATH_HTTPS_CRT 	PATH_HTTPS_SSL"/certificate.crt"
#define PATH_HTTPS_CA  	PATH_HTTPS_SSL"/ca-bundle.crt"
#define PATH_HTTPS_PEM 	PATH_HTTPS_SSL"/server.pem"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define USE_POPEN 0

int deleteSelfSignedCertificate()
{
	unlink(PATH_HTTPS_SS_KEY);
	unlink(PATH_HTTPS_SS_CRT);
	unlink(PATH_HTTPS_SS_PEM);

	printf("Deleted SSC files\n");
}

int createSelfSignedCertificate(char *path_key, char* path_crt)
{
	char cmd[256];

	if( access(PATH_SSL_ROOT , F_OK) != 0) {
		mkdir(PATH_SSL_ROOT);
	}

	// 입력된 정보가 올바르지 않을때, 기본으로 생성함
	if ( strlen(app_set->net_info.ssc_C) == 0 
	|| strlen(app_set->net_info.ssc_C) > 4 
	|| strlen(app_set->net_info.ssc_O)  == 0 
	|| strlen(app_set->net_info.ssc_CN) == 0 ) {
		printf("Make default Self Signed Certificate\n");

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

	printf("Self Signed Certificate, cmd:%s\n", cmd);
	system(cmd);

	if( access(path_crt, R_OK) != 0 || access(path_key, R_OK) != 0) {
		printf("Failed to create Self Signed Certificate\n");
		return -1;
	}

	printf("Succeed, create Self Signed Certificate\n");

	// make pem file for lighttpd
	sprintf(cmd, "cat %s %s > %s", path_key, path_crt, PATH_HTTPS_SS_PEM);
	printf("Make PEM file, cmd:%s\n", cmd);
	system(cmd);

	if( access(PATH_HTTPS_SS_PEM,  R_OK) != 0 ) {

		printf("Failed to create PEM file\n");
		return -1;
	}

	printf("Succeed, create SS PEM\n");


	return 0;
}

int createSignedCertificate()
{
	return 0;
}

int     app_web_boot_passwordfile()
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

int     app_web_make_passwordfile(char *id, char *pw, int lv, int authtype)
{
	char strcmd[128]={0};
	char stropt[8]={0};

	// make passwd file.
	if(authtype == 0){ // basic (default)

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
	else if(authtype == 1){ // digest

		if( 0 != access(PATH_WEB_AUTH_FILE, F_OK))
			sprintf(stropt, "-c");
		
		sprintf(strcmd, "/opt/fit/bin/htdigest %s %s %s %s", stropt, PATH_WEB_AUTH_FILE, STR_WEB_DIGEST_REALM, id);

		// 이건, 대화형이라 popen 을 사용해야...
		FILE *f = popen(strcmd, "w");
		if (f == NULL) { perror("failed open:"); return -1;}
		fprintf(f, pw);     // maybe new
		fprintf(f, "\n");
		fprintf(f, pw);     // maybe confirm
		fprintf(f, "\n");

		pclose(f);

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

#define CREATE_SSL_ON_BOOTTIME (0) // 막은 이유: 부팅시간을 잡아먹는다, 웹설정시 생성하도록 변경
int app_web_ssl_setup()
{
#if CREATE_SSL_ON_BOOTTIME
if(app_set->net_info.https_mode == 1) {
	//deleteSelfSignedCertificate();
	createSelfSignedCertificate();
} else if(app_set->net_info.https_mode == 2){
	createSignedCertificate();
}
#endif

	char *filepath = "/etc/lighttpd/conf.d/ssl.conf";
	int   bIsFile = 0; // 용도: 설정이 ssl enable이지만, certificate File 없을때는 Https모드를 활성화하지 않기 위해...

	FILE *fp = fopen(filepath, "w");
	if(fp) {
		char str[255];
		// onvif와 https_port가 공유되고 있음, 분리가 필요한 상황
		//sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", app_set->net_info.https_port);
		sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", 443);
		fputs(str, fp);

		if(app_set->net_info.https_mode==1) // 경로 정보가 conf에 추가될 경우, https disable로 설정되어도, 저장된 경로에 file이 없다면, lighttpd 실행시 에러남
		{
			if( access(PATH_HTTPS_SS_PEM, F_OK)==0) {
				sprintf(str, "ssl.pemfile = \"%s\"", PATH_HTTPS_SS_PEM);
				fputs(str, fp);
				bIsFile = 1;
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

		if( bIsFile == 1 && app_set->net_info.https_mode!=0) {
			sprintf(str, "ssl.engine = \"%s\"\n", "enable");
		}else {
			sprintf(str, "ssl.engine = \"%s\"\n", "disable");
		}

		fputs(str, fp);
		fputs("}\n", fp);

		fclose(fp);
	}
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

	return 0;
}

int     app_web_is_passwordfile()
{
	return (0 == access(PATH_WEB_AUTH_FILE, F_OK));
}

int		app_telnetd_enable(int en)
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
	int ret = createSelfSignedCertificate(PATH_HTTPS_SS_KEY, PATH_HTTPS_SS_CRT);

	return ret;
}

// thread도 파일들 만든 후, 뒤에 pem파일 생성되었는지 확인...
int app_web_https_create_ssc()
{
	pthread_t tid;
	int ret = pthread_create(&tid,NULL,(void *(*)(void *))__start_routine_create_ssc, NULL);
	if (ret != 0)
	{
		printf("failed pthread_create. ret = %d\r\n", ret);
		return ret;
	}else 
		pthread_detach(tid);

	return ret;
}
