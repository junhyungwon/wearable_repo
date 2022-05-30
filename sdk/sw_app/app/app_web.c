#include <stdio.h>
#include <unistd.h>

#include "app_web.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

#define USE_POPEN 0

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

int app_web_ssl_setup()
{
	char *filepath = "/etc/lighttpd/conf.d/ssl.conf";

	FILE *fp = fopen(filepath, "w");
	if(fp) {
		char str[255];
		// onvif와 https_port가 공유되고 있음, 분리가 필요한 상황
		//sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", app_set->net_info.https_port);
		sprintf(str, "$SERVER[\"socket\"] == \":%d\" {\n", 443);
		fputs(str, fp);
		sprintf(str, "ssl.pemfile = \"/opt/fit/server.pem\"\n");
		fputs(str, fp);
		sprintf(str, "ssl.engine = \"%s\"\n", app_set->net_info.https_enable?"enable":"disable");
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
