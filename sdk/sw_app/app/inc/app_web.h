/******************************************************************************
 * web server controls
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_web.h
 * @brief
 * @author	bkkim
 * @section	history
 */
/*****************************************************************************/
#ifndef _APP_WEB_H_
#define _APP_WEB_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define PATH_WEB_AUTH_FILE  "/tmp/lighttpd.user" //"/etc/lighttpd/lighttpd.user"

#define STR_WEB_DIGEST_REALM MODEL_NAME // this is from Rules.make

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
 Declares variables
-----------------------------------------------------------------------------*/
extern const char* const WEB_DEFAULT_ID;
extern const char* const WEB_DEFAULT_PW;
extern const char* const ONVIF_DEFAULT_ID;
extern const char* const ONVIF_DEFAULT_PW;

int     app_web_boot_passwordfile(); // on boot time
int     app_web_make_passwordfile(char *id, char *pw, int lv, int authtype);
int     app_web_is_passwordfile();
int     app_web_start_server();
int     app_web_stop_server();
int     app_web_restart_server();
int		app_telnetd_enable(int en);

int     app_web_https_create_ssc();
#endif//_APP_WEB_H_
