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
    char fname[256];

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

    req = qcgireq_parse(req, Q_CGI_ALL);
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

        str = req->getstr(req, "cert_operation", false);
        if (str == NULL) {
            CGI_DBG("Invalid paramter. cert_operation\n");
            goto install_cert_out; 
        }

        t.cert_operation = atoi(str); // 1: install, 0:delete
        CGI_DBG("cert_operation = %d\n", t.cert_operation);

        str = req->getstr(req, "cert_https_mode", false);
        if (str == NULL) {
            CGI_DBG("Invalid paramter. cert_https_mode\n");
            goto install_cert_out; 
        }
        int cert_https_mode = atoi(str);
        CGI_DBG("cert_https_mode = %d\n", cert_https_mode);

        // get uploaded files
        if( t.cert_operation == 1 )
        {
            sprintf(fname, "%s", req->getstr(req, "cert_file", false));
            rename(fname, TMP_SVR_CRT);
            sprintf(fname, "%s", req->getstr(req, "key_file", false));
            rename(fname, TMP_SVR_KEY);
            sprintf(fname, "%s", req->getstr(req, "ca_file", false));
            rename(fname, TMP_SVR_CA);

            if (verify_crt(TMP_SVR_CRT, TMP_SVR_KEY, TMP_SVR_CA) < 0)
            {
                CGI_DBG("Invalid crt/key.\n");
                goto install_cert_out;
            }

            sprintf(t.crt_file, "%s", TMP_SVR_CRT);
            sprintf(t.key_file, "%s", TMP_SVR_KEY);
            sprintf(t.ca_file,  "%s", TMP_SVR_CA);
        }

        t.https_mode = cert_https_mode;

        ret = sysctl_message(UDS_INSTALL_CERT_FILE, (void*)&t, sizeof t );

        CGI_DBG("ret:%d\n", ret);
        if(0 > ret) {
            return SUBMIT_ERR;
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
	int nError = submit_settings_qcgi();

    // for VUEJS
#if PASS_RETURNS_TO_VUEJS
	send_response(nError);
#else 
	if(nError == SUBMIT_OK){
        wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}

// EOF