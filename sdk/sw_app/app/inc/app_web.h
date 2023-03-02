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

#define STR_WEB_DIGEST_REALM "Authorized User"  // lighttpd/auth.conf auth.require.realm 와 같아야함.

// SSC == Self Signed Certificate
#define PATH_HTTPS_SS_KEY_NAND 	PATH_SSL_ROOT_NAND"/_ssc_private.key"   // encrypted
#define PATH_HTTPS_SS_CRT_NAND 	PATH_SSL_ROOT_NAND"/_ssc_certificate.crt"
#define PATH_HTTPS_SS_KEY_MMC 	PATH_SSL_ROOT_MMC"/_ssc_private.key"    // encrypted
#define PATH_HTTPS_SS_CRT_MMC 	PATH_SSL_ROOT_MMC"/_ssc_certificate.crt"

// Normal SSLs
#define PATH_HTTPS_KEY 	PATH_SSL_ROOT_NAND"/private.key"
#define PATH_HTTPS_CRT 	PATH_SSL_ROOT_NAND"/certificate.crt"
#define PATH_HTTPS_CA  	PATH_SSL_ROOT_NAND"/ca-bundle.crt"
#define PATH_HTTPS_PEM 	PATH_SSL_ROOT_NAND"/server.pem"


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
int     app_web_https_copy_to_sdcard();
int     app_web_reset_passphrase(char *pw);
#endif//_APP_WEB_H_
