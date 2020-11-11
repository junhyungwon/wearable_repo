#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "cgi_debug.h"
#include "qdecoder.h"
#include "cgi_uds.h"
#include "cgi_error.h"


#define FW_FILE_PATH		"/tmp" // tmp 에 만들고, fitt에서 지지고볶고 난리를...

int main(int argc, char **argv)
{
	int nStat = ERR_FWUPDATE;
	int status = -1;
	qentry_t *req = NULL;

	req = qcgireq_setoption(NULL, true, FW_FILE_PATH, 1024*1024*48);
	req = qcgireq_parse(req, Q_CGI_POST);
	if(NULL == req) {
		CGI_DBG("Failed qdecoder alloc\n");
		goto __FWUPGRADE_END;
	}

	const char *filedata    = req->getstr(req, "fw", false);
	int        filelength   = req->getint(req, "fw.length");
	const char *filename    = req->getstr(req, "fw.filename", false);
	const char *contenttype = req->getstr(req, "fw.contenttype", false);
	const char *savepath    = req->getstr(req, "fw.savepath", false);
	char  filepath[128];
	ssize_t count=0;

	/*
	CGI_DBG("filename    = %s\n", filename);
	CGI_DBG("filelength  = %d\n", filelength);
	CGI_DBG("contenttype = %s\n", contenttype);
	CGI_DBG("savepath    = %s\n", savepath);
	*/

	if (filename == NULL || filelength < 1 || savepath == NULL) {
		//CGI_DBG("Select File, Please....\n");
		//qcgires_error(req, "Select file, please.");
		nStat = ERR_FWUPDATE_NOFILE;
		goto __FWUPGRADE_END;
	}

	// 1. check file size
	// 2. check file name(prefix, postfix and etc)
	// 3. md5 check sum
	
	/* Make download path */
#if 0 // We use /tmp directory.
	if(access(FW_FILE_PATH, R_OK) != 0){
		CGI_DBG("mkdir %s\n", FW_FILE_PATH);
		if( 0 != mkdir(FW_FILE_PATH, 0777)){
			CGI_DBG("Failed mkdir :%s\n", strerror(errno));
			nStat = ERR_FWUPDATE_NOTATION;
			goto __FWUPGRADE_END;
		}
	}
#endif

	sprintf(filepath, "%s/%s", FW_FILE_PATH, filename);
	CGI_DBG("savepath : %s\n", savepath);

	if( 0 != rename(savepath, filepath)){
		//CGI_DBG("Failed rename fw file...%s\n", strerror(errno));
		nStat = ERR_FWUPDATE_NOTATION;
		goto __FWUPGRADE_END;
	}

	CGI_DBG("rename: %s\n", filepath);


	// send message of start fwupdate to system and wait for done
	char strCmd[256] = "FWUPDATE";
	char strPath[128];
	sprintf(strPath, "%s", filepath);
	nStat = sysctl_message(UDS_CMD_FWUPDATE, (void*)strPath, sizeof(strPath));
	CGI_DBG("fw update res...%d\n", nStat);

__FWUPGRADE_END:	
	printf("Content-type: text/html;\r\n\r\n");
	{
		if (nStat == ERR_FWUPDATE_INVALID_FILE){
			fprintf(stdout, "%s\r\n", "ERR_FWUPDATE_INVALID_FILE");
			CGI_DBG("%s\r\n", "ERR_FWUPDATE_INVALID_FILE");
		}
		else if (nStat == ERR_FWUPDATE_FTP_RUNNING){
			fprintf(stdout, "%s\r\n", "ERR_FWUPDATE_FTP_RUNNING");
			CGI_DBG("%s\r\n", "ERR_FWUPDATE_FTP_RUNNING");
		}
		else if (nStat == ERR_FWUPDATE_SIZE) {
			fprintf(stdout, "%s\r\n", "ERR_FWUPDATE_SIZE");
			CGI_DBG("%s\r\n", "ERR_FWUPDATE_SIZE");
		}
		else if (nStat == OK_FW_UPDATE){
			fprintf(stdout, "%s\r\n", "OK_FW_UPDATE");
			CGI_DBG("%s\r\n", "OK_FW_UPDATE");
		}
		else {
			// ERROR
			fprintf(stdout, "%s\r\n", "ERR_FWUPDATE");
			CGI_DBG("fw update error : %d\r\n", nStat);
		}

	}

	if(req) req->free(req);

	sync();

	return nStat;

}
