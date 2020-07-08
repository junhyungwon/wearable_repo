/*----------------------------------------------------------------------------
 Defines referenced header files
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <glob.h>

#include "dev_disk.h"
#include "dev_common.h"

#include "app_main.h"
#include "app_comm.h"
#include "app_rec.h"
#include "app_leds.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_file.h"

/*----------------------------------------------------------------------------
 Definitions and macro
 -----------------------------------------------------------------------------*/

#define FILE_LIST_CYCLE         	(100)
#define FILE_STATE_CHECK_TIME		(1000)		//1sec
#define FILE_STATE_FULL_BEEP_TIME	(5*1000)	//5SEC
#define CNT_BEEP_FULL 				(FILE_STATE_FULL_BEEP_TIME/FILE_STATE_CHECK_TIME)

#if FREC_TEST
	#define MIN_THRESHOLD_SIZE 	    (100*MB)/KB		//# 100MB
	#define FILE_LIST_CHECK_TIME    (10*1000)   	//10 SEC
#else
	#define MIN_THRESHOLD_SIZE 	    (1024*MB)/KB	//# 1GB
	#define FILE_LIST_CHECK_TIME    (60*1000)   	//1min	
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) if(NULL != p){free(p);p=NULL;}
#endif

#define OPEN_CREATE(fd, path) (NULL != (fd = fopen(path, "wb+")))
#define OPEN_EMPTY(fd, path) (NULL != (fd = fopen(path, "wb+")))
#define OPEN_RDONLY(fd, path) (NULL != (fd = fopen(path, "rb")))
#define OPEN_RDWR(fd, path) (NULL != (fd = fopen(path, "rb+")))

#define SAFE_CLOSE(fd) if(fd){ fclose(fd);fd=NULL;};
#define CLOSE(fd) fclose(fd)

#define READ_PTSZ(fd, pt, size)   ( size == fread(pt, 1, size, fd))
#define WRITE_PTSZ(fd, pt, sz)   ( sz == fwrite(pt, 1, sz, fd))

#define LSEEK_END(fd, pos) (-1 != fseek(fd, pos, SEEK_END))
#define LSEEK_CUR(fd, pos) (-1 != fseek(fd, pos, SEEK_CUR))
#define LSEEK_SET(fd, pos) (-1 != fseek(fd, pos, SEEK_SET))

#define LTELL(fd) ftell(fd)

/*------------------------------------------------------------------*/
#define FILE_DEBUG (1)
#if FILE_DEBUG
#define FILE_DBG(msg, args...) printf("[FILE] - (%d):" msg, __LINE__, ##args)
#define FILE_ERR(msg, args...) printf("[FILE] - (%d):\t%s:" msg, __LINE__, __FUNCTION__, ##args)
#else
#define FILE_DBG(msg, args...) ((void)0)
#define FILE_ERR(msg, args...) ((void)0)
#endif
/*------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_file_t t_file;
app_file_t *ifile = &t_file;

static file_list_t ilist;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Implementation
 -----------------------------------------------------------------------------*/

/*****************************************************************************
 * @brief	 compare file name
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int _cmpold(const void * a, const void * b)
{
	file_entry_t *a_name = (file_entry_t *)a;
	file_entry_t *b_name = (file_entry_t *)b;

	return (strcmp(a_name->fname, b_name->fname));
}

/*****************************************************************************
 * @brief	 check record path
 * @section  DESC Description
 *	 - desc : if not exist working directgory, create it.   
 *****************************************************************************/
static void _check_rec_dir(char* rec_root)
{
	//# record directory init ---
	if(-1 == access(rec_root, 0)) {
		mkdir(rec_root, 0775);
		chmod(rec_root, 0775);
	}
}

static int get_threshold_size(app_file_t* pInfo)
{
	disk_info_t idisk ;
	int ret = EFAIL ;

	if(app_cfg->ste.b.mmc_err)
		return EFAIL ;

	ret = util_disk_info(&idisk, SD_MOUNT_PATH ) ;

	dprintf("idisk.total = %ld\n",idisk.total) ;
	dprintf("idisk.avail = %ld\n",idisk.avail) ;
	if(ret != EFAIL)
	{
		pInfo->size_max = ((unsigned long long)idisk.total * 100)/100 ;
		pInfo->disk_avail = idisk.avail ;
	}

	return ret ;
}

/*****************************************************************************
 * @brief	 get file size (KiB)
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static unsigned long _get_file_size(char* fname)
{
	unsigned long fsize = 0;
	struct stat sb;

	if(lstat(fname, &sb) == SOK)
		fsize = (unsigned long)(sb.st_size/KB);
	
	return fsize;
}

/*****************************************************************************
 * @brief	 get disk space (KiB)
 * @section  DESC Description
 *	 - desc : returnd total, used, free
 *****************************************************************************/
int _get_disk_kb_info(app_file_t *prm, disk_size_t *sz)
{
    if(prm->flist.tot_size == 0)
	{
        sz->total = prm->size_max ;
        sz->free = prm->disk_avail ;
	    sz->used = prm->size_max - prm->disk_avail  ;
	    prm->flist.tot_size = sz->used  ;
	}
	else
	{
        sz->total = prm->size_max ;
        sz->free = sz->total - prm->flist.tot_size ;
		sz->used = prm->flist.tot_size ;
	}

	return SOK;
}

/*****************************************************************************
 * @brief	 search recorded file (*.avi)
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static int _search_files(char* search_path, char* filter, file_entry_t** file_list)
{
	int fcnt = 0;
	char globpath[MAX_CHAR_255] = {0,};
	glob_t globbuf;

	//# ufs file search in mmc/DCIM or mmc/DCIM/ufs directory
	memset(&globbuf, 0, sizeof(glob_t));
	sprintf(globpath, "%s/*.%s", search_path, filter);
	
	if(glob(globpath, GLOB_DOOFFS, NULL, &globbuf) == 0)
   		fcnt = globbuf.gl_pathc;
	else
		eprintf("Not found files: %s\n", globpath);

	if(fcnt) {
		int fidx = 0;
		*file_list = (file_entry_t*)malloc(sizeof(file_entry_t)*fcnt);
	
		FILE_DBG("FOUND %s FILES COUNT : %d ===\n", filter, fcnt);	
		for(fidx = 0; fidx < fcnt; fidx++) {
			sprintf((*file_list)[fidx].fname, "%s", globbuf.gl_pathv[fidx]);
		}
	}

	return fcnt;
}

/*****************************************************************************
 * @brief	 delete header of linked-list
 * @section  DESC Description
 *	 - desc : return the full path of file
 *****************************************************************************/
static int _delete_rec_file_head(file_list_t* plist, char* del_path)
{
	int ret = EFAIL;	
	
	if(plist->head != NULL) {
		file_entry_t* pTmp = plist->head;

		if(pTmp->next != NULL) {
			plist->head			= (file_entry_t*)pTmp->next;
			plist->head->prev	= NULL;		
		}
		else {
			plist->head = NULL;
			plist->tail = NULL;
		}
		
		sprintf(del_path, "%s", pTmp->fname);
		if(pTmp){
			free(pTmp);
			pTmp = NULL;
		}

		ret = SOK;
	}

	return ret;
}

/*****************************************************************************
 * @brief	 add file name to tail of linked-list
 * @section  DESC Description
 *	 - desc : file name is full path
 *****************************************************************************/
static int _add_ufs_list_tail(app_file_t* prm, char* ufs_path)
{
	file_list_t* plist = &prm->flist;

	file_entry_t* pNew	= (file_entry_t*)malloc(sizeof(file_entry_t));
	file_entry_t* pTail	= (file_entry_t*)plist->tail;

    struct stat sb;
	char path[128];

    sprintf(path, "%s",ufs_path) ;
    lstat(path, &sb) ;

	if(pNew == NULL)
	{
		eprintf("file entry create failed!!\n");
		return EFAIL;
	}
	
	pNew->prev = NULL;
	pNew->next = NULL;
	pNew->size = sb.st_size/KB ;

	sprintf(pNew->fname, "%s", ufs_path);

	//#--- link process
	if(pTail == NULL)		//# first file
	{
		plist->head = pNew;
		plist->tail = pNew;
	}
	else
	{
		pTail->next = pNew;
		pNew->prev = pTail;

		plist->tail = pNew;
	}

	plist->file_cnt++ ;
	plist->tot_size += pNew->size ;

	return SOK;
}

static int _add_ufs_list_head(app_file_t* prm, char* ufs_path)
{
	file_list_t* plist = &prm->flist;

	file_entry_t* pNew	= (file_entry_t*)malloc(sizeof(file_entry_t));
	file_entry_t* pHead	= (file_entry_t*)plist->head;

	if(pNew == NULL)
	{
		eprintf("file entry create failed!!\n");
		return EFAIL;
	}

	pNew->prev = NULL;
	pNew->next = NULL;
	sprintf(pNew->fname, "%s", ufs_path);

	//#--- link process
	if(pHead == NULL)		//# first file
	{
		plist->head = pNew;
		plist->tail = pNew;
	}
	else
	{
		pNew->next	 = pHead;
		plist->head = pNew;
	}

	plist->file_cnt++ ;
	plist->tot_size += pNew->size ;

	return SOK;
}

/*****************************************************************************
 * @brief	 make the linked-list with searched files (.avi)
 * @section  DESC Description
 *	 - desc : files are sorted to ascending. 
 *****************************************************************************/
static int _make_file_list(app_file_t* prm)
{
	int idx = 0,fcnt = 0;
	file_entry_t* rec_files = NULL;

	//#-- search ufs files...
	fcnt = _search_files(prm->rec_root, AVI_EXT, &rec_files);

	//# make ufs linked-list
	if(fcnt > 0) {
		qsort(rec_files, fcnt, sizeof(file_entry_t), _cmpold);

		for(idx = 0; idx < fcnt; idx++) {
			if(_add_ufs_list_tail(prm , rec_files[idx].fname) == EFAIL)
				return EFAIL;
			
//			prm->file_cnt++;
		}

		if(rec_files != NULL)
			free(rec_files);
	}

	return fcnt;
}

/*****************************************************************************
 * @brief	 delete files
 * @section  DESC Description
 *	 - desc : delete files until del_size reached (or over).
 *****************************************************************************/
static int _delete_files(app_file_t* prm, unsigned long del_size)
{
	unsigned long tot_size ; 

	if(ifile->flist.file_cnt == 0 && !app_cfg->ste.b.pwr_off)
		return SOK;

	OSA_mutexLock(&ifile->mutex_file);

	FILE_DBG("==== DELETE SIZE : %ld ====\n", del_size);

	int ret = SOK;
	char del_file[MAX_CHAR_64] = {0, };
	
	//# delete file
	if(prm->flist.file_cnt > 0)
	{
		int fsize = 0;
		unsigned long del_size_sum = 0;
		while(del_size > del_size_sum) {
			if(_delete_rec_file_head(&prm->flist, del_file) == EFAIL) {
				ret = EFAIL;
				goto del_err;
			}
			
			fsize =  _get_file_size(del_file);
			if(fsize) {
				if(remove(del_file) != SOK) {
					eprintf("remove fault!! %s \n", del_file);
					ret = EFAIL;
					goto del_err;
				}

				del_size_sum += fsize;
				prm->flist.tot_size -= fsize ;
				prm->flist.file_cnt--;

				FILE_DBG("DELETE FILE : %s(%ld) (%d KB)=== tot_size = %d\n", del_file, prm->flist.file_cnt, fsize, prm->flist.tot_size);

				if(prm->flist.file_cnt == 0)
					break;				
			}

		}
	}

del_err:
	OSA_mutexUnlock(&ifile->mutex_file);

	return ret;
}

/*****************************************************************************
 * @brief	 check the overwrite recording.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static int full_interval = 0, once_over_beep = 0, once_normal_led = 0;
static void _check_overwite_full_led(int file_state)
{
	switch (file_state)
	{
		case FILE_STATE_OVERWRITE:
			if(!once_over_beep){
                if(app_rec_state())
                {
				    printf("@@@@@@@@@@@@@@@@@@FILE_STATE_OVERWRITE@@@@@@@@@@@@@@@@@@@\n");				
				    app_leds_mmc_ctrl(LED_MMC_GREEN_BLINK);

				    dev_buzz_ctrl(80, 1);
				    once_over_beep = 1;
                }
			}
            else
            {
                if(!app_rec_state())
                {
				    app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
                    once_over_beep = 0 ;
                }
            }
			break;
		case FILE_STATE_FULL:
			full_interval++;
			if( (full_interval %= CNT_BEEP_FULL) == 0) {
				printf("@@@@@@@@@@@@@@@@@@FILE_STATE_FULL@@@@@@@@@@@@@@@@@@@\n");				
				dev_buzz_ctrl(80, 1);
				full_interval = 0;
			}
			
			if(app_cfg->ste.b.disk_full == OFF) {
				app_leds_mmc_ctrl(LED_MMC_RED_ON);
				app_cfg->ste.b.disk_full = ON;
			}
			break;
		default:	//# normal state
			if(!once_normal_led) {
				printf("@@@@@@@@@@@@@@@@@@FILE_STATE_NORMAL@@@@@@@@@@@@@@@@@@@\n");								
				app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
				once_normal_led = 1;
			}
			if(app_cfg->ste.b.disk_full == ON) {
				app_cfg->ste.b.disk_full = OFF;
				app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
			}
			break;
	}
}

/*****************************************************************************
 * @brief	 check the amount of disk usage.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
#define DISK_USED_MID	66
#define DISK_USED_MIN	33
static void _display_disk_usage(disk_size_t* sz)
{
	int used_per = 0;

	if (app_cfg->ste.b.mmc) {
		used_per = (((unsigned long long)sz->used * 100) / sz->total);
		
		if (used_per > DISK_USED_MID)
			app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_3);	//# x > 66%
		else if(used_per < DISK_USED_MIN )
			app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_1);	//# x < 33%
		else
			app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_2);	//# 33% < x < 66%
	}

}

/*****************************************************************************
 * @brief	 check the amount of disk usage.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static void _set_disk_full_state(app_file_t* prm)
{
	if(prm->file_state == FILE_STATE_NORMAL)
		prm->file_state = (app_set->rec_info.overwrite)?FILE_STATE_OVERWRITE:FILE_STATE_FULL;
}

#if FREC_TEST
static void _frec_test(unsigned long used)
{
	FILE_DBG("Check used : %ld / threshold : %d \n", used, MIN_THRESHOLD_SIZE);
	if(used > MIN_THRESHOLD_SIZE) {
		_set_disk_full_state(ifile);
		_delete_files(ifile, used - MIN_THRESHOLD_SIZE);
	}
}
#endif

/*****************************************************************************
* @brief    file manangement thread function
* @section  - check rec_dir size & delete old files
            - do every 1 min
*****************************************************************************/
static void *THR_file_mng(void *prm)
{
	app_thr_obj *tObj = (app_thr_obj*)prm;

	int cmd, exit=0, first = 0;
	unsigned int f_cycle=FILE_LIST_CHECK_TIME; // A©øA¨ö ¨öAAU¢¯¢®¨ù¡© CHECK START CI¥ì¥ì¡¤I
	disk_size_t sz_info;
	char msg[MAX_CHAR_255] = {0,};
	
	aprintf("start...\n");

	tObj->active = 1;
	while(!exit)
	{
		app_cfg->wd_flags |= WD_FILE;
			
		cmd = tObj->cmd;
		if(cmd == APP_CMD_EXIT || app_cfg->ste.b.mmc_err) {
			exit = 1;
			break;
		}

		if(app_cfg->ste.b.mmc && app_cfg->ste.b.cap) {
            if(!first)
			{
				ifile->flist.tot_size = 0 ;
				if(_get_disk_kb_info(ifile, &sz_info) != EFAIL)
				{
				    printf("sz_info.total = %ld\n",sz_info.total) ;
			    	printf("sz_info.used = %ld\n",sz_info.used) ;
				    printf("sz_info.free = %ld\n",sz_info.free) ;

				    _display_disk_usage(&sz_info);
				}
                first = 1 ;
            } 
			//# file size check and delete -- per 1 min
//	        if(( (f_cycle % FILE_LIST_CHECK_TIME) == 0 ))     {
	        if(( (f_cycle % FILE_STATE_CHECK_TIME) == 0 ))     {
				if(_get_disk_kb_info(ifile, &sz_info) == EFAIL) {
					app_cfg->ste.b.mmc_err = 1;
					eprintf("Get disk size fail!!");
					sprintf(msg, "[APP_FILE] !!! Get disk size failed !!! - %s", ifile->rec_root);
					app_log_write( MSG_LOG_WRITE, msg );

					app_rec_stop(ON);	//buzzer on
					continue;
				}
				_display_disk_usage(&sz_info);

#if FREC_TEST
				_frec_test(sz_info.used);
#else
//				FILE_DBG("Check free : %ld / threshold : %d \n", sz_info.free, MIN_THRESHOLD_SIZE);
				if(sz_info.free < MIN_THRESHOLD_SIZE) {
					_set_disk_full_state(ifile);
                    
                    if(ifile->file_state == FILE_STATE_OVERWRITE) 
                    {
					    if(_delete_files(ifile, MIN_THRESHOLD_SIZE - sz_info.free) == EFAIL) {
						    eprintf("Delete file error!!");
						    app_cfg->ste.b.mmc_err = 1;
						    app_rec_stop(ON);
						    continue;
					    }
				        dprintf("MIN_THRESHOLD_SIZE = %d sz_info.free = %ld\n",MIN_THRESHOLD_SIZE,sz_info.free) ;
                    }
				}
				else if(!app_set->rec_info.overwrite)
				{
	                ifile->file_state = FILE_STATE_NORMAL;
				}
#endif
        	}

			if( (f_cycle % FILE_STATE_CHECK_TIME) == 0 ) {
//				if(sz_info.free < MIN_THRESHOLD_SIZE) 
				    _check_overwite_full_led(ifile->file_state);
				f_cycle = 0 ;
			}
		}
	
		OSA_waitMsecs(FILE_LIST_CYCLE);
		f_cycle += FILE_LIST_CYCLE;
	}
	
	tObj->active = 0;
	aprintf("exit\n");

	sync();

	return NULL;
}

/*****************************************************************************
* @brief    file manager init/exit function
* @section  [desc]
*****************************************************************************/
int app_file_start(void)
{
	int status = SOK;
	char msg[MAX_CHAR_128] ;
	
    app_thr_obj *tObj;	

	if(app_cfg->ste.b.mmc == 0)
		return EFAIL;
	
    ifile = &t_file;
    memset(ifile, 0, sizeof(app_file_t));
	memset(&ilist, 0, sizeof(file_list_t));

	sprintf(ifile->rec_root, "%s/%s", SD_MOUNT_PATH, "DCIM");
	ifile->file_state = FILE_STATE_NORMAL;

    aprintf(" [app] %s start...\n", __func__);

	//#-- create directories such as DCIM, ufs
	_check_rec_dir(ifile->rec_root);

    if(get_threshold_size(ifile) == EFAIL)
	{
		sprintf(msg, "[APP_FILE] !! Get threshold size failed !!!") ;
		app_log_write(MSG_LOG_WRITE, msg) ;
		app_cfg->ste.b.mmc_err = 1;
	}

    dprintf("ifile->size max = %ld\n",ifile->size_max) ;
    dprintf("ifile->disk_avail = %ld\n",ifile->disk_avail) ;

	if(!app_cfg->ste.b.mmc_err)
	{
	    app_cfg->wd_flags = WD_FILE;
	    struct timeval t1, t2;
	    gettimeofday(&t1, NULL);

	    if(_make_file_list(ifile) == EFAIL) 	{
		    eprintf("make file list fail!! \n");
		    app_cfg->ste.b.mmc_err = 1;
		    return EFAIL;
	    }

	    gettimeofday(&t2, NULL);
	    FILE_DBG("msec[%ld] COUNT FILES : %ld -----------\n", 
		((t2.tv_sec*1000)+(t2.tv_usec/1000))-((t1.tv_sec*1000)+(t1.tv_usec/1000)), ifile->flist.file_cnt);	
    }
	
    status = OSA_mutexCreate(&(ifile->mutex_file));
    OSA_assert(status == OSA_SOK);

    //#--- create normal record thread
	tObj = &ifile->fObj;
	if(thread_create(tObj, THR_file_mng, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	OSA_waitMsecs(300);
	
	aprintf("... done!\n");

	return status;
}

void app_file_stop(void)
{
	app_thr_obj *tObj = &ifile->fObj;
	int status = 0;
	
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while(tObj->active)
		OSA_waitMsecs(20);

	thread_delete(tObj);

	status = OSA_mutexDelete(&(ifile->mutex_file));
	OSA_assert(status == OSA_SOK);

	ifile = NULL;

    aprintf("done...\n");
}

int app_file_add(char* path)
{
	int ret = EFAIL;
	
	if(ifile == NULL) 
		return EFAIL;

	OSA_mutexLock(&ifile->mutex_file);	
	
	if(_add_ufs_list_tail(ifile, path) == EFAIL)
		goto add_fail;
	
//	ifile->file_cnt++;
	FILE_DBG("ADDED FILE : %s(%ld) ===\n", path, ifile->flist.file_cnt);
	ret = SOK;
	
add_fail:
	OSA_mutexUnlock(&ifile->mutex_file);

	return SOK;
}

/*****************************************************************************
* @brief    Get file name and size for ftp backup
* @section  
  - desc :  - if success, return the file size(KB), the other 0.
  			- path argument is full path.
*****************************************************************************/
unsigned long get_ftp_send_file(char* path)
{
	unsigned long fsize = 0;
	
	if(ifile == NULL)
		goto get_err;

	//# get file name and remove list
	if(_delete_rec_file_head(&ifile->flist, path) == EFAIL) {
		goto get_err;
	}

	//# get file size
	fsize =  _get_file_size(path);

get_err:
	return fsize;
}

/*****************************************************************************
* @brief    resotre file that was failed send on ftp.
* @section  
  - desc :  - this path is added to head of linkd-list.
  			- if success 0 is returned, others -1.
*****************************************************************************/
int restore_ftp_file(char* path)
{
	if(ifile == NULL)
		return EFAIL;

	return _add_ufs_list_head(ifile, path);
}

/*****************************************************************************
* @brief    Delete file that is backup done with ftp.
* @section  
  - desc :  - if success return 0, the other -1.
*****************************************************************************/
int delete_ftp_send_file(char* path)
{
	int ret = SOK;

	if(ifile == NULL || ifile->flist.file_cnt <= 0)
		return SOK;

	if(remove(path) != SOK) {
		eprintf("remove fault!! %s \n", path);
		ret = EFAIL;
	}

	ifile->flist.file_cnt--;

	return ret;
}

/*****************************************************************************
* @brief    check Recorded file exist  .
* @section 
  - desc :  - if success return file count, the other 0.
*****************************************************************************/
int get_recorded_file_count()
{
	int cnt = 0;
	cnt = ( (ifile) ? ifile->flist.file_cnt : 0 ); 
	
    return cnt;
}

/*****************************************************************************
* @brief    set dispaly_disk usage  .
* @section 
  - desc :  .
*****************************************************************************/
void set_display_disk_usage()
{
	disk_size_t sz_info;

	if(_get_disk_kb_info(ifile, &sz_info) != EFAIL) 
	{
	    _display_disk_usage(&sz_info);
	}
}

/*****************************************************************************
* @brief    check SD write status  .
* @section 
  - desc :  - overwrite mode or not
*****************************************************************************/
int get_write_status()
{
	return ifile->file_state; 
}
