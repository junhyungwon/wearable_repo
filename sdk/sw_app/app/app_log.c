/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
/**
 * @file    app_log.c
 * @brief	log file management with linked-list
 * @author  sksung
 * @section MODIFY history
 *     - 2014.11.25 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <glob.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define _GNU_SOURCE
#include <ftw.h>
#include <fnmatch.h>
#include <fts.h>

#include "app_comm.h"
#include "dev_common.h"
#include "app_main.h"
#include "app_set.h"
#include "app_util.h"
#include "app_dev.h"

/*----------------------------------------------------------------------------
 Definitions and macro
 -----------------------------------------------------------------------------*/
//#define USE_FTS_UTIL

#define LOG_FILE_MAX		(2)
#define LOG_MAX_SIZE		(500*KB)

#define LOG_PREFIX			"ulog"
#define LOG_EXT				"log"
#define LOG_ROOT_PATH		"/mmc/log"

#define LOG_MSG_KEY         0x9992

#ifndef SAFE_FREE
#define SAFE_FREE(p) if(NULL != p) {free(p); p=NULL;}
#endif

/*----------------------------------------------------------------------------
Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
	log_info_t file_info;

	void *prev;
	void *next;
} app_log_entry_t;

typedef struct {
	app_log_entry_t *head;
	app_log_entry_t *tail;
} app_log_list_t;

typedef struct {
	app_thr_obj 	lObj;		//# file checking thread

	OSA_MutexHndl 	mutex_log;

	app_log_list_t 	log_list;

	unsigned int	file_cnt;
	unsigned int	last_idx;
	unsigned int	log_size;

	char last_fname[LOG_NAME_MAX];
	FILE *fd;

} app_log_t;

static app_log_t t_log;
static app_log_t *ilog = &t_log;

/*----------------------------------------------------------------------------
Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
local function
-----------------------------------------------------------------------------*/
#ifndef __SYSLOGD_ENABLE__
/*****************************************************************************
 * @brief	 filelist create/delete function
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int cmpold(const void * a, const void * b)
{
	log_info_t *a_name = (log_info_t *)a;
	log_info_t *b_name = (log_info_t *)b;

	return (strcmp(a_name->fname, b_name->fname));
}

static int log_file_add(char* path)
{
	struct stat sb;
	log_info_t lInfo;
	app_log_list_t* plist = &ilog->log_list;

	lstat( path, &sb );
	sprintf( lInfo.fname, "%s", path );
	lInfo.size = sb.st_size;	//bytes

	app_log_entry_t* pNew	= app_malloc(sizeof(app_log_entry_t));
	app_log_entry_t* pTail 	= (app_log_entry_t*)plist->tail;

	if(pNew == NULL)
	{
		eprintf("file entry create failed!!\n");
		return EFAIL;
	}

	pNew->prev = NULL;
	pNew->next = NULL;
	pNew->file_info.size = lInfo.size;
	sprintf(pNew->file_info.fname, "%s", lInfo.fname);

	//#--- link process
	if(pTail == NULL)		//# first file
	{
		plist->head = pNew;
		plist->tail = pNew;
	}
	else
	{
		if(pTail->next == NULL)
		{
			pTail->next = pNew;
			pNew->prev = pTail;
		}

		plist->tail = pNew;
	}

	//#--- update total file size & file count
	ilog->file_cnt++;
	ilog->log_size = pNew->file_info.size;

	dprintf(" [LOG] added %s file\n", plist->tail->file_info.fname);
	dprintf(" [LOG] %03d files ---------------\n", ilog->file_cnt);

	return SOK;
}

static int log_file_delete( void )
{
	char cmd[MAX_CHAR_128]={0,};

	app_log_list_t* plist = &ilog->log_list;

	while( ilog->file_cnt > LOG_FILE_MAX )
	{
		if(plist->head == NULL || plist->head->next == NULL)
			break;

		app_log_entry_t* pTmp = plist->head;

		plist->head 		= (app_log_entry_t*)pTmp->next;
		plist->head->prev	= NULL;
		ilog->file_cnt--;

		sprintf(cmd, "rm -f %s", pTmp->file_info.fname);
		dev_execlp(cmd);

		dprintf(" Delete file : %s \n", pTmp->file_info.fname);

		free(pTmp);
		pTmp = NULL;
		app_msleep(1);
	}

	dprintf(" %03d files ---------------\n", ilog->file_cnt);
	return SOK;
}

static int log_open(int new_file)
{
	int ret = SOK;
	char path[MAX_CHAR_128];

	if (ilog->fd) {
		fclose(ilog->fd);
		ilog->fd = NULL;
	}

	if (new_file) {
		ilog->last_idx++;
	}

	sprintf(ilog->last_fname, "%s_%08x.%s", LOG_PREFIX, ilog->last_idx, LOG_EXT);
	sprintf(path, "%s/%s", LOG_ROOT_PATH, ilog->last_fname);

    ilog->fd = fopen(path, "a");
	if (ilog->fd == NULL) {
		eprintf("Log file open failed!!! path: %s\n", path);
		return EFAIL;
	}

	if (new_file) {
		log_file_add(path);
	}

	if (ilog->file_cnt > LOG_FILE_MAX)
		log_file_delete();

	dprintf("@@@@@@@@@@@@@@ LOG File Open [%s] @@@@@@@@@@@@@@\n", path);

	return ret;
}

static int get_log_count(char* path, char*filters)
{
	int fcnt = 0;

	glob_t globbuf;
	static char	extPath[255];

	memset(&globbuf, 0, sizeof(glob_t));

	sprintf(extPath, "%s/%s", path, filters);
    if(glob(extPath, GLOB_DOOFFS, NULL, &globbuf) == 0)
   		fcnt = globbuf.gl_pathc;
	else
		eprintf("Not found firmware file: %s\n",path);

	globfree(&globbuf);

	return fcnt;
}

static int log_list_check(char*filters)
{
	FTS *tree;
	FTSENT *node;

	char const *scan[] = {NULL, NULL};
	int fcnt = 0, i = 0;

	if(-1 == access(NAND_ROOT, 0)) {
		eprintf(" Can't create Log path : %s !!!\n", NAND_ROOT);
		return -1;
	 }

    if(-1 == access(LOG_ROOT_PATH, 0)) {
        mkdir(LOG_ROOT_PATH, 0775);
        chmod(LOG_ROOT_PATH, 0775);

		return 0;
    }

	//# get count of log files.
	fcnt = get_log_count(LOG_ROOT_PATH, filters);

	if(fcnt > 0)
	{
		scan[0] = LOG_ROOT_PATH;

		tree = fts_open((char * const *)scan, FTS_NOCHDIR, NULL);
		if (!tree) {
			eprintf("Failed to fts_open\n");
			return EFAIL;
		}

		log_info_t* list;
		list = app_malloc(sizeof(log_info_t)*fcnt);

		//# sort log files
		while ((node = fts_read(tree)))
		{
			if (node->fts_info & FTS_F)
			{
				if (fnmatch(filters, node->fts_name, FNM_CASEFOLD) == 0)
					strcpy(list[i++].fname, node->fts_name);
			}
		}

		if (fts_close(tree)) {
			eprintf("fts_close");
		}

		qsort(list, fcnt, sizeof(log_info_t), cmpold);

		//# Make list
		char path[MAX_CHAR_128];
		for( i = 0; i < fcnt; i++ )
		{
			printf("file : %s\n", list[i].fname);
			sprintf(path, "%s/%s", LOG_ROOT_PATH, list[i].fname);
			log_file_add( path );
		}


		//# get last file index
		ilog->last_idx = (unsigned int)atoi( strtok( &list[fcnt-1].fname[5], "." ) );
		ilog->file_cnt = fcnt;

		app_mfree(list);
	}

	return 0;
}

/*****************************************************************************
 * @brief    system log write thred main function
 * @section  DESC Description
 *   - desc
 *****************************************************************************/
static void *THR_log(void *prm)
{
    app_thr_obj *tObj =  &ilog->lObj;
    ucx_msg_info msg_buf;
    int exit = 0;

	//# msg queue create
    app_cfg->msgqId = msgget(LOG_MSG_KEY, 0);
    if(app_cfg->msgqId < 0){
        app_cfg->msgqId = msgget(LOG_MSG_KEY, IPC_CREAT|0666);
        if(app_cfg->msgqId < 0){
            eprintf("Faled make the MSG KEY for log write!!!\n");
            return NULL;
        }
    }

    aprintf("enter...\n");
	tObj->active = 1;

	app_cfg->ste.b.log = 1;

    while(!exit)
    {
		if(msgrcv(app_cfg->msgqId, &msg_buf, sizeof(msg_buf)-sizeof(long), MSG_TYPE_LOG, 0) < 0) {
            eprintf("Receive msg failed!!!\n");
            break;
        }

		switch(msg_buf.msgCmd)
		{
			case MSG_LOG_WRITE:
				if(ilog->fd != NULL && app_cfg->ste.b.mmc)
				{
					//# Write Log
					fprintf(ilog->fd, "%s\r\n", msg_buf.msgstr);
					fseek(ilog->fd, 0, SEEK_END);
					ilog->log_size += strlen( msg_buf.msgstr );

					//# size check and open a new file.
					if( LOG_MAX_SIZE <= ilog->log_size ){
						if(log_open(1) != 0){
							exit = 1;
							ilog->fd = NULL;
						}
					}
				}
				break;
			case MSG_LOG_SHUTDOWN:
				if(ilog->fd != NULL)
				{
					//# Write Log and exit!
					fprintf(ilog->fd, "%s\r\n", msg_buf.msgstr);
					exit = 1;
				}
				break;
			case MSG_LOG_EXIT:
				aprintf("Recv MSG_LOG_EXIT.........\n");
				exit = 1;
				break;
		}
    }

	if(ilog->fd != NULL){
		fclose(ilog->fd);
		ilog->fd = NULL;
		sync();
	}

//	sd_copy_log();
	app_cfg->ste.b.log = 0;
	tObj->active = 0;
    aprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief    system log thread start/stop
* @section  [desc]
*****************************************************************************/
int app_log_init(void)
{
	int status, new_file=0;
    app_thr_obj *tObj;
	char filter[MAX_CHAR_64] = {0,};

	if (!app_cfg->ste.b.mmc)
		return EFAIL;

    memset(ilog, 0, sizeof(app_log_t));

	//# check exist log files and get last file name and date
	sprintf(filter, "%s_*.%s",LOG_PREFIX, LOG_EXT);
	if (log_list_check(filter) != 0) {
		eprintf("Log file check failed!!! \n");
		return EFAIL;
	}

	if (ilog->file_cnt == 0)
		new_file = 1;

	//# logfile open and write thread start
	if (log_open(new_file) == 0) {
	    status = OSA_mutexCreate(&(ilog->mutex_log));
	    OSA_assert(status == OSA_SOK);

	    //#--- create log write thread
	    tObj = &ilog->lObj;
		if (thread_create(tObj, THR_log, FXN_TSK_PRI+2, NULL, NULL) < 0) {
			eprintf("create log write thread\n");
			return EFAIL;
		}
		app_msleep(500);
	}

	aprintf("..done!\n");

    return SOK;
}

void app_log_exit(void)
{
	app_thr_obj *tObj;

    if (!app_cfg->ste.b.log)
		return;

	app_log_write(MSG_LOG_EXIT, "<<<< FITT_MAIN is EXIT >>>>");

	tObj = &ilog->lObj;
	while (tObj->active) {
		app_msleep(20);
	}
	thread_delete(tObj);

	OSA_mutexDelete(&ilog->mutex_log);

	aprintf("..done!\n");
}

/*****************************************************************************
* @brief    system log write helper function.
* @section  [desc]
*****************************************************************************/
void app_log_write(int cmd, char *msg)
{
	ucx_msg_info msgBuf;
	struct tm 	ts;
	time_t tm1;
	int ret;

	char dtime[MAX_CHAR_32];

#ifndef USE_LOG_FILE
	return;
#endif

	if(!app_cfg->ste.b.log)
		return;

	OSA_mutexLock(&ilog->mutex_log);
	memset(&msgBuf, 0, sizeof(ucx_msg_info));

	time(&tm1);
	localtime_r(&tm1, &ts);

	strftime(dtime, sizeof(dtime), "%Y:%2m:%2d:%2H:%2M:%2S", &ts);

	msgBuf.msgDes	= MSG_TYPE_LOG;
	msgBuf.msgSrc	= MSG_TYPE_UCX;
	msgBuf.msgCmd	= cmd;
	msgBuf.msgParm1 = 0;
	msgBuf.msgParm2 = 0;
	msgBuf.msgParm3 = 0;
	if(msg != NULL)
		sprintf(msgBuf.msgstr, "[%s]	%s", dtime, msg);

	ret = msgsnd(app_cfg->msgqId, &msgBuf, sizeof(ucx_msg_info)-sizeof(long), 0);

	OSA_mutexUnlock(&ilog->mutex_log);
}
#else
void app_log_write(int cmd, char *msg)
{
	if (msg != NULL)
		syslog(LOG_INFO, "%s\n", msg);
}
#endif
