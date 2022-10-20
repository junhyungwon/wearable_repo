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

    qcgisess_destroy(sess);
    // qcgires_setcontenttype(req, "text/plain");
    // printf("Session destroyed.\n");

    // empty return
    send_response(SUBMIT_OK);

    req->free(req);
    return 0;
}
