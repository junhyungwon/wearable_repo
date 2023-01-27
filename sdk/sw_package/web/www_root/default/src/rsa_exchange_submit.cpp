#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "qdecoder.h"

static int submit_settings() {
    char *method = getenv("REQUEST_METHOD");
    if (method == NULL) return ERR_INVALID_METHOD;
    CGI_DBG("method : %s\n", method);

    char *contents = qcgireq_getquery(Q_CGI_POST); // application/json
    if (contents == NULL)
        return ERR_INVALID_PARAM;

    json_object *req_obj = json_tokener_parse(contents);
    if (req_obj == NULL)
        return SUBMIT_ERR;

    json_object *req_pubkey_obj = json_object_object_get(req_obj, "pubkey");
    if (req_pubkey_obj == NULL) {
        json_object_put(req_obj);
        return SUBMIT_ERR;
    }

    const char *client_pubkey = json_object_get_string(req_pubkey_obj);
    int client_pubkey_len = json_object_get_string_len(req_pubkey_obj);

    // check parameter values
    if (client_pubkey_len == 0) {
        CGI_DBG("Invalid parameter - client_pubkey_len = 0\n");
        return SUBMIT_ERR;
    }

    // validate the pem format.
    BIO *bio = BIO_new_mem_buf(client_pubkey, -1);
    RSA* cryptClient = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (cryptClient == NULL) {
        CGI_DBG("Invalid parameter - wrong client pem format\n");
        return SUBMIT_ERR;
    }
    RSA_free(cryptClient);

    // start session
    qentry_t *req = qcgireq_parse(NULL, Q_CGI_COOKIE);
    qentry_t *sess = qcgisess_init(req, NULL);

    // set the timeout and user id.
    qcgisess_settimeout(sess, SESSION_TIMEOUT);
    req->putstr(sess, "identity", "admin", true);
    req->putstr(sess, "pubkey", client_pubkey, true);
    // CGI_DBG("client's pubkey : %d, %s \n", client_pubkey_len, client_pubkey);

    // save session
    qcgisess_save(sess);

    // free client's req
    // json_object_put(req_pubkey_obj); // caution : req_pubkey_obj will be freed by BIO.
    json_object_put(req_obj);

    char server_pubkey[PUBKEY_FILE_SIZE] = {'\0',};
    long size = read_file(SERVER_PUBKEY_PATH, server_pubkey, PUBKEY_FILE_SIZE);
    if (size <= 0) {
        return SUBMIT_ERR;
    }
    // CGI_DBG("server_pubkey: %d %s", size, server_pubkey);

    // send the server's pubkey
    {
        json_object *myobj = json_object_new_object();
        json_object_object_add(myobj, "return value", json_object_new_int(SUBMIT_OK));
        json_object_object_add(myobj, "pubkey", json_object_new_string(server_pubkey));
        printf(
            "Cache-Control: no cache, no store\r\n"
            "Content-type: application/json\r\n\r\n");
        printf("%s\r\n", json_object_to_json_string(myobj));

        json_object_put(myobj);
    }

    sess->free(sess);
    req->free(req);

    return SUBMIT_OK;
}

int main(int argc, char *argv[]) {
    int nError = submit_settings();

    if (nError != SUBMIT_OK)
        send_response(nError);

    return 0;
}
