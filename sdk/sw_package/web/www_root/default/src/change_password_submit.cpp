#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

static int submit_settings()
{
    RSA *cryptServer;
    validateRsaSession(&cryptServer);

    if (cryptServer == NULL) {
        return ERR_INVALID_PARAM;
    }

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
	if(req == NULL)
        return ERR_INVALID_PARAM;

    char *pw1= req->getstr(req, "password1", false);
    char *pw2= req->getstr(req, "password2", false);
    int authtype = req->getint(req, "authtype");  // 0:basic, 1:digest(default)

    CGI_DBG("authtype:%d, pw1:%s, pw2:%s\n", authtype, pw1, pw2);

    if (pw1 == NULL || pw2 == NULL) {
        return ERR_INVALID_PARAM;
    }

    // base64 decode -> decrypt by server's private key
    int buffer_size = RSA_size(cryptServer);
    char pw1_decrypted[buffer_size];
    char pw2_decrypted[buffer_size];
    {
        int len;

        CGI_DBG("pw1 base64:%s\n", pw1);
        len = lf_base64_de(cryptServer, pw1, (unsigned char*)pw1_decrypted); // fixme : assert
        CGI_DBG("decBytes: %d, base64 de(rsa+base64): %s\n",  len, pw1_decrypted);

        CGI_DBG("pw2 base64:%s\n", pw2);
        len = lf_base64_de(cryptServer, pw2, (unsigned char*)pw2_decrypted); // fixme : assert
        CGI_DBG("decBytes: %d, base64 de(rsa+base64): %s\n",  len, pw2_decrypted);
    }

    // check parameter values
    if(strlen(pw1_decrypted) < 1 || strcmp(pw1_decrypted, pw2_decrypted) || authtype == -1){
        CGI_DBG("Invalid parameter\n");
        return SUBMIT_ERR;
    }

    T_CGI_USER acc;
    acc.lv = USER_LV_ADMINISTRATOR;
    acc.authtype = authtype;
    sprintf(acc.id, "%s", "admin");
    sprintf(acc.pw, "%s", pw1_decrypted);

    if(0!=sysctl_message(UDS_CMD_CHANGE_PASSWORD, (void*)&acc, sizeof(acc))) {
        return SUBMIT_ERR;
    }

    return SUBMIT_OK;
}


int main(int argc, char *argv[])
{
	int nError = submit_settings();

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect("../index.html", 5);
	}
	else{
		// return home
		refresh_page("../index.html", nError);
	}
#else
	send_response(nError);
#endif

	return 0;
}
