/*
 * @file  get_image.c
 * @brief for jpeg file download. /tmp/fitt360.jpeg
 *
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>

#include "cgi.h"
#include "cgi_param.h"

#include "qdecoder.h"

int send_jpeg_file()
{
	int ret=0;
	qentry_t *req = qcgireq_parse(NULL, Q_CGI_ALL);
	if(req){
		ret = qcgires_download(req, "/tmp/fitt360.jpeg", "image/jpeg");
		req->free(req);
	}
	return ret; // sent_bytes
}

int main(int argc, char *argv[])
{
	int ret,isPOST=0;
    T_CGIPRM prm[128];

    char *method = getenv("REQUEST_METHOD");
    CGI_DBG("method : %s\n", method);

    char *contents=NULL;
	if(0 == strcmp(method, "GET")){
		isPOST = FALSE;
		contents = getenv("QUERY_STRING");
		CGI_DBG("contents:%s\n", contents);

		if(strstr(contents, "jpeg_download")){
			ret = send_jpeg_file();
		}
	}

	return 0;
}
