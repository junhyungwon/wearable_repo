#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

#define REDIRECT_CGI_PATH "../index.html"
#define CGI_FORM_PATH "settings_voip.cgi"
#define HOME_PATH "../index.html"

static int submit_settings_qcgi()
{
#if 0 // for debug
    char *contents = get_cgi_contents();
	CGI_DBG("contents:%s\n", contents);
#endif

    int ret=SUBMIT_NO_CHANGE;

    qentry_t *req = qcgireq_parse(NULL, Q_CGI_POST);
    if (req)
    {
        //qcgires_setcontenttype(req, "text/plain");

        T_CGI_VOIP_CONFIG t;
		memset(&t, 0, sizeof t);

        char * str= req->getstr(req, "voipo_private_network_only", false);
        if (str != NULL) {
            t.private_network_only = atoi(str);
        }
        str= req->getstr(req, "txt_voip_ip", false);
        if (str != NULL) {
            sprintf(t.ipaddr, "%s", str);
        }
        str= req->getstr(req, "txt_voip_port", false);
        if (str != NULL) {
            t.port = atoi(str);
        }
        str= req->getstr(req, "txt_voip_id", false);
        if (str != NULL) {
            sprintf(t.userid, "%s", str);
        }
        str= req->getstr(req, "txt_voip_pw", false);
        if (str != NULL) {
            sprintf(t.passwd, "%s", str);
        }
        str= req->getstr(req, "txt_voip_peerid", false);
        if (str != NULL) {
            sprintf(t.peerid, "%s", str);
        }

        //req->free(req);

		if( t.port == 0 
			|| strlen(t.ipaddr) == 0 
			|| strlen(t.userid) == 0 
			|| strlen(t.passwd) == 0 
			|| strlen(t.peerid) == 0 
		) {
			CGI_DBG("Invalid Parameter\n");
			CGI_DBG("voip.ipaddr:%s\n", t.ipaddr);
			CGI_DBG("voip.port  :%d\n", t.port);
			CGI_DBG("voip.userid:%s\n", t.userid);
			CGI_DBG("voip.passwd:%s\n", t.passwd);
			CGI_DBG("voip.peerid:%s\n", t.peerid);
			return ERR_INVALID_PARAM;
		}else{
			// check voip values
			CGI_DBG("voip.ipaddr:%s\n", t.ipaddr);
			CGI_DBG("voip.port  :%d\n", t.port);
			CGI_DBG("voip.userid:%s\n", t.userid);
			CGI_DBG("voip.passwd:%s\n", t.passwd);
			CGI_DBG("voip.peerid:%s\n", t.peerid);
		}

        ret = sysctl_message(UDS_SET_VOIP_CONFIG, (void*)&t, sizeof t );
        CGI_DBG("ret:%d\n", ret);
        if(0 > ret) {
            return SUBMIT_ERR;
        }
		else if( ret == ERR_NO_CHANGE) {
			return SUBMIT_NO_CHANGE;
		}

        return SUBMIT_OK;
    }
}


int main(int argc, char *argv[])
{
	int nError = submit_settings_qcgi();

	CGI_DBG("nError:%d\n", nError);

	send_response(nError);

#if 0
	if(nError == SUBMIT_OK){
		// Must restart web server with new admin's password.
		// 1. show ok window
		// 2. and restart webserver

		wait_redirect(REDIRECT_CGI_PATH, 3);
	}
	else{
		// return home
		refresh_page(HOME_PATH, nError);
	}
#endif

	return 0;
}
