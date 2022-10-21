#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "qdecoder.h"

#include "cgi.h"
#include "cgi_param.h"

int main(void)
{
    qentry_t *req = qcgireq_parse(NULL, 0);

    // start session.
    qentry_t *sess = qcgisess_init(req, NULL);
    char* sessionId = qcgisess_getid(sess);

    char *identity  = req->getstr(req, "identity", false);
    char *password  = req->getstr(req, "password", false);

    // tries count
    int count = req->getint(sess, "count"); // default 0

    // threshold 넘어갈 경우, LOGIN_FAILURE_TIMEOUT 시간동안 항상 error.
    if (count >= LOGIN_MAX_FAILURE) {
        send_response(ERR_NOT_AVAILABLE);
        return 0;
    }

    bool isValidated = false;

    // fixme : authentication.
    if (strcmp(identity,"admin") == 0) {
        T_CGI_ACCOUNT acc;
        sprintf(acc.id, "%s", identity);
        sprintf(acc.pw, "%s", password);
        if(0 == sysctl_message(UDS_CMD_CHECK_ACCOUNT, (void*)&acc, sizeof(acc))) {
            isValidated = true;
        }
    }

    if (isValidated) {
        // reset count
        req->putint(sess, "count", 0, true);

        // set the timeout and user id.
        qcgisess_settimeout(sess, SESSION_TIMEOUT);
        req->putstr(sess, "identity", "admin", true);

        // screen out
        // qcgires_setcontenttype(req, "text/plain");
        // req->print(sess, stdout, true);
    } else {
        if (count == 0) {
            qcgisess_settimeout(sess, LOGIN_FAILURE_TIMEOUT);
        }

        count++;
        req->putint(sess, "count", count, true);
    }

    // save session & free allocated memories
    qcgisess_save(sess);

    // empty return
    send_response(isValidated ? SUBMIT_OK : ERR_INVALID_ACCOUNT);

    sess->free(sess);
    req->free(req);
    return 0;
}
