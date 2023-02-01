#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

static int submit_settings()
{
    RSA *cryptClient, *cryptServer;
    validateRsaSession(&cryptClient, &cryptServer);

    if (cryptClient == NULL || cryptServer == NULL) {
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
    char pw1_decrypted[256] = {'\0', };
    char pw2_decrypted[256] = {'\0', };
    {
        int len, decBytes;
        unsigned char *output;
    
        len = base64_decode(pw1, &output);
        CGI_DBG("decoded pw1 len %d, pw1 base64:%s\n", len, pw1);
        decBytes = RSA_private_decrypt(len, output, (unsigned char*)pw1_decrypted, cryptServer, RSA_PKCS1_PADDING);
        CGI_DBG("decBytes: %d, pw1_decrypted : %s\n", decBytes, pw1_decrypted);
        free(output);

        len = base64_decode(pw2, &output);
        CGI_DBG("decoded pw2 len %d, pw2 base64:%s\n", len, pw2);
        decBytes = RSA_private_decrypt(len, output, (unsigned char*)pw2_decrypted, cryptServer, RSA_PKCS1_PADDING);
        CGI_DBG("decBytes: %d, pw2_decrypted : %s\n", decBytes, pw2_decrypted);
        free(output);
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
