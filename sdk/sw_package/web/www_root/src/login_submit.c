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

    // fixme : authentication. check from file.
    // fixme : count check. by session??
    req->putint(sess, "count", 0, true);

    // set the timeout and user id.
    qcgisess_settimeout(sess, SESSION_TIMEOUT);
    req->putstr(sess, "identity", "admin", true);

    // screen out
    // qcgires_setcontenttype(req, "text/plain");
    // req->print(sess, stdout, true);

    // save session & free allocated memories
    qcgisess_save(sess);

    // empty return
    send_response(SUBMIT_OK);

    sess->free(sess);
    req->free(req);
    return 0;
}
