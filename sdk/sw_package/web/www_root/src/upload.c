#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "cgi_debug.h"
#include "qdecoder.h"
#include "cgi_uds.h"


#define FW_FILE_PATH		"/tmp" // tmp 에 만들고, fitt에서 지지고볶고 난리를...

enum {
	ERR_RECORD = -1000,
	ERR_EXIST_ID,
	ERR_PASSWORD,
	ERR_UNKNOWN,
	ERR_SET_FRAME,
	ERR_SETUP,
	ERR_INPUT,
	ERR_HOLIDAY,
	ERR_EMAIL_TEST,
	ERR_FW_UPDATE,
	ERR_NO_SDCARD,				// SDCARD 없음(마운트 안됨)
	ERR_NO_FWFILE,				// 파일 없음
    ERR_FWFILE_SIZE,            // 사이즈에 문제가 있음.
    ERR_FWFILE_NOTATION,        // 모델에 맞는 펌웨어 검사..
    ERR_FWFILE_CONTENTS,        // 펌웨어 파일 내용 이상.

	OK_FW_UPDATE=0,
};

int main(int argc, char **argv)
{
	int nStat = ERR_FW_UPDATE;
	int status = -1, value;
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
		nStat = ERR_NO_FWFILE;
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
			nStat = ERR_FWFILE_NOTATION;
			goto __FWUPGRADE_END;
		}
	}
#endif

	sprintf(filepath, "%s/%s", FW_FILE_PATH, filename);
	//CGI_DBG("filepath : %s\n", filepath);

	if( 0 != rename(savepath, filepath)){
		//CGI_DBG("Failed rename fw file...%s\n", strerror(errno));
		nStat = ERR_FWFILE_NOTATION;
		goto __FWUPGRADE_END;
	}


	// send message of start fwupdate to system and wait for done
	char strCmd[256] = "FWUPDATE";
	char strPath[128];
	sprintf(strPath, "%s", filepath);
	nStat = sysctl_message(UDS_CMD_FWUPDATE, (void*)strPath, sizeof(strPath));
	//CGI_DBG("fw update done...\n");

__FWUPGRADE_END:	
	printf("Content-type: text/html;\r\n\r\n");
	{
		if(nStat == OK_FW_UPDATE){
			printf("%s\r\n", "OK");
			//Reboot(); main에서 직접 실행한다. 이미 서버는 죽었다...
		}
		else{

			//CGI_DBG("fw update error : %d\r\n", nStat);

#if 0
			// share the status by file
			//StartProcess("echo -1 > /tmp/upgrade_state");  // error
#else
			// directly,  send error message to system
			status = -1;
			//CGI_DBG("Failed FWUPGRADE....... \n");
#endif

#if 1
			// Send resume message to system
			value = 0;
			//CGI_DBG("RESUME_FROM_UPGRADE ....... \n");
#endif
		}
	}

	if(req) req->free(req);

	return 0;

}
