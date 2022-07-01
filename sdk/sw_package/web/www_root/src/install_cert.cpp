#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

#define REDIRECT_CGI_PATH "/"
#define CGI_FORM_PATH "settings_setupHttps.cgi"
#define HOME_PATH "/"

#define PASS_RETURNS_TO_VUEJS 0

int https_mode = 0;

#define TMP_SVR_CRT     "/tmp/svr.crt"
#define TMP_SVR_KEY     "/tmp/svr.key"
#define TMP_SVR_CA      "/tmp/svr_ca.crt"

int get_popen_read(char *path, char *buf, int size)
{
    FILE *fin;
    memset(buf, 0, size);
    if ((fin = popen(path, "r"))) {
        fread(buf, 1, size, fin);
    }
    buf[size-1] = '\0';
    return fclose(fin);
}

int verify_crt(char *crt, char *key, char *ca)
{
    int ret;
    char tmp[256];
    char buf1[1024];
    char buf2[1024];

    // verify match key and certificate
 
    if (ca) {
        sprintf(tmp, "openssl x509 -noout -modulus -in %s", ca);
        get_popen_read(tmp, buf1, 1024);
        if (!strstr(buf1, "Modulus=")) {
            CGI_DBG("invalid crt file...FAIL\n");
            return -1;
        }
    }

    sprintf(tmp, "openssl x509 -noout -modulus -in %s", crt);
    get_popen_read(tmp, buf1, 1024);
    if (!strstr(buf1, "Modulus=")) {
        CGI_DBG("invalid crt file...FAIL\n");
        return -1;
    }
    
    sprintf(tmp, "openssl x509 -noout -modulus -in %s | \
        openssl md5 2>&1", crt);
    get_popen_read(tmp, buf1, 1024);

    CGI_DBG("%s\n", buf1);

    sprintf(tmp, "openssl rsa -noout -modulus -in %s", key);
    get_popen_read(tmp, buf2, 1024);
    if (!strstr(buf2, "Modulus=")) {
        CGI_DBG("invalid key file...FAIL\n");
        return -1;
    }
    
    sprintf(tmp, "openssl rsa -noout -modulus -in %s | \
        openssl md5 2>&1", key);
    get_popen_read(tmp, buf2, 1024);

    CGI_DBG("%s\n", buf2);

    if (!strcmp(buf1, buf2)) {
        CGI_DBG("verify key/crt matching...OK\n");
    } else {
        CGI_DBG("verify key/crt matching...FAIL\n");
        return -1;
    }

    return 0;
}

static int submit_settings_qcgi()
{
    int ret=SUBMIT_NO_CHANGE;
    qentry_t *req = NULL;

    CGI_DBG("install_cert.cgi...\n");

    if (getenv("CONTENT_LENGTH") == NULL) {
        CGI_DBG("No content-length\n");
        goto install_cert_out;
    }

    if (atoi(getenv("CONTENT_LENGTH")) > 16*1024) {
        CGI_DBG("Invalid size\n");
        goto install_cert_out; 
    }

    req = qcgireq_setoption(NULL, true, "/tmp", 60);
    if (req == NULL) {
        CGI_DBG("Can't set option.\n");
        goto install_cert_out; 
    }

    req = qcgireq_parse(req, 0);
    if (req == NULL) {
        CGI_DBG("Can't parse req..\n");
        goto install_cert_out; 
    }

    
    // get parameters
    {
        T_CGI_HTTPS_CONFIG t;
		memset(&t, 0, sizeof t);

        char *str = req->getstr(req, "txt_cert_name", false);
        if(str == NULL) { 
            CGI_DBG("Invalid parameter. txt_cert_name\n");
            goto install_cert_out;
        }
        strcpy(t.cert_name, str);

        ret = sysctl_message(UDS_SET_HTTPS_CONFIG, (void*)&t, sizeof t );
        CGI_DBG("ret:%d\n", ret);
        if(0 > ret) {
            return SUBMIT_ERR;
        }
		else if( ret == ERR_NO_CHANGE) {
			return SUBMIT_NO_CHANGE;
		}

        return SUBMIT_OK;
    }

install_cert_out:
    if(req)
        req->free(req);

    return SUBMIT_ERR;
}

int main(int argc, char *argv[])
{
	int nError;

	nError = submit_settings_qcgi();

    // for VUEJS
#if PASS_RETURNS_TO_VUEJS
	send_response(nError);
#else 
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

        if (https_mode == 1)
            wait_redirect(REDIRECT_CGI_PATH, 20); // SSC 만들때 시간이 좀 필요합니다
        else 
            wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
