/*----------------------------------------------------------------------------
 Defines referenced header files
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include "list.h"

#define _GNU_SOURCE
#include <fnmatch.h>

#include "dev_disk.h"
#include "dev_micom.h"
#include "dev_common.h"

#include "app_main.h"
#include "app_comm.h"
#include "app_rec.h"
#include "app_leds.h"
#include "app_dev.h"
#include "app_set.h"
#include "app_mcu.h"
#include "app_file.h"
#include "app_buzz.h"

/*----------------------------------------------------------------------------
 Definitions and macro
 -----------------------------------------------------------------------------*/
#define FILE_LIST_CYCLE         	(100)
#define FILE_STATE_CHECK_BEEP		(1000)		//1sec
#define FILE_STATE_CHECK_TIME		(60*1000)	//60sec
#define FILE_STATE_FULL_BEEP_TIME	(5*1000)	//5SEC
#define CNT_BEEP_FULL 				(FILE_STATE_FULL_BEEP_TIME/FILE_STATE_CHECK_BEEP)

#if FREC_TEST
	#define MIN_THRESHOLD_SIZE 	    (100*MB)/KB		//# 100MB
	#define FILE_LIST_CHECK_TIME    (10*1000)   	//10 SEC
#else
	#define MIN_THRESHOLD_SIZE 	    (1024*MB)/KB	//# 1GB
	#define FILE_LIST_CHECK_TIME    (60*1000)   	//1min	
#endif

#define VIDEO_LIST_NAME				"video.lst"

typedef struct {
	char name[FILE_MAX_PATH_LEN];
	Uint32 padding;  /* for align */

} list_info_t;

/* file_entry_t? ??? */
struct disk_list {
	struct list_head queue;
	char fullname[FILE_MAX_PATH_LEN];
};

typedef struct {
	OSA_MutexHndl mutex_file ;
	app_thr_obj fObj;
	
	size_t file_count;  /* total files for ftp send */
	size_t efile_count;  /* event files for ftp send */
	int	file_state;
	
	char rec_root[MAX_CHAR_32];
	
	unsigned long disk_avail;
	unsigned long disk_max;
	unsigned long disk_used;
	
} app_file_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_file_t t_file;
static app_file_t *ifile = &t_file;

static LIST_HEAD(ilist);

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

/*****************************************************************************
 * @brief	 check record path
 * @section  DESC Description
 *	 - desc : if not exist working directgory, create it.   
 *****************************************************************************/
static void _check_rec_dir(const char *dir)
{
	//# record directory init ---
	if (-1 == access(dir, 0)) {
		mkdir(dir, 0775);
		chmod(dir, 0775);
	}
}

/*****************************************************************************
 * @brief	 Get Disk total / avail size.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static void _check_threshold_size(app_file_t *pInfo)
{
	disk_info_t idisk;

	/* SD 카드 오류 시 마운트가 안돼서 자동으로 리부팅 됨. 
	 * 따라서 error가 발생하지 않으므로 리턴값 체크하는 부분 삭제
	 */
	util_disk_info(&idisk, SD_MOUNT_PATH);
	
	pInfo->disk_max 	= idisk.total;
	pInfo->disk_avail 	= idisk.avail;
	pInfo->disk_used	= (idisk.total-idisk.avail);
//	dprintf("MAX capacity %ld(KB)\n", pInfo->disk_max);
//	dprintf("available capacity %ld(KB)\n", pInfo->disk_avail);
}

static Uint32 _check_file_size(const char *fname)
{
	struct stat st;
	Uint32 len=0;
	
	if (fname != NULL) {
		stat(fname, &st);
		len = (st.st_size / KB); /* byte -> KB */
		dprintf("file size checking %s(%dKB)\n", fname, len);
	} 
		
	return len;
}

/*****************************************************************************
 * @brief	 check free space of disk.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
int app_file_check_disk_free_space(void)
{
	unsigned long avail = ifile->disk_avail;
	int ret = 0;
	
//	dprintf("Check free : %lu(KB) / threshold : %lu(KB)!\n", avail, MIN_THRESHOLD_SIZE);
	if (avail < MIN_THRESHOLD_SIZE) 
		ret = -1; /* disk full state */
	
	return ret;
}

/*----------------------------------------------------------------------------
 Implementation File List
 -----------------------------------------------------------------------------*/
/*****************************************************************************
 * @brief	 count the number of files in directory.
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static Uint32 get_file_count(const char *dpath)
{
	struct dirent *entry;
	DIR *dcim;
	Uint32 count = 0;
	
	dcim = opendir(dpath);
	if (dcim != NULL) 
	{
		while ((entry = readdir(dcim)))
		{
			if ((strcmp(entry->d_name,".")==0) || 
				(strcmp(entry->d_name,"..")==0) ||
				(strcmp(entry->d_name,"video.lst")==0)) 
				continue;
			
			/* If the entry is a regular file */
			if (entry->d_type == DT_REG) { 
         		count++;
    		}
		}
		closedir(dcim);
	} else {
		count = 0;
	}
	//dprintf("%u files in %s path\n", count, dpath);

	return count;
}

/*****************************************************************************
 * @brief	 delete file.
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int delete_file(const char *pathname)
{
	int ret=0;
	
	if (pathname != NULL) {
		ret = remove(pathname);
		if (ret < 0) {
			eprintf("can't remove %s(%d)\n", pathname, errno);
			return -1;
		}
	}
	
	return 0;
}

/*****************************************************************************
 * @brief	 compare file name
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int _cmpold(const void *a, const void *b)
{
	list_info_t *a_name = (list_info_t *)a;
	list_info_t *b_name = (list_info_t *)b;

	return (strcmp(&a_name->name[13], &b_name->name[13]));  // /mmc/DCIM/R_ sorting for event file 
}

/*****************************************************************************
 * @brief	 delete header of linked-list
 * @section  DESC Description
 *	 - desc : return the size of file
 *****************************************************************************/
static int find_first_and_delete(struct list_head *head, Uint32 *del_sz)
{
	struct disk_list *ptr = NULL;
	Uint32 sz = 0;

	ptr = list_last_entry(head, struct disk_list, queue);
	if (ptr != NULL) {
		if (delete_file(ptr->fullname) < 0) {
			*del_sz = 0;
			return -1;
		}
			
		sz = _check_file_size((const char *)ptr->fullname); /* return file size */
		dprintf("DELETE FILE : %s (%d KB)\n", ptr->fullname, sz);
		list_del(&ptr->queue);
		ifile->file_count--;
	    if(!strstr(ptr->fullname, NORMAL_FILE))
		    ifile->efile_count-- ;
		free(ptr);
	}
	
	*del_sz = sz;
	return 0;
}

/*****************************************************************************
 * @brief	 get header of linked-list
 * @section  DESC Description
 *	 - desc : return the full path of file
 *****************************************************************************/
static int _get_rec_file_head(struct list_head *head, char *path)
{
	struct disk_list *ptr = NULL;
	Uint32 sz = 0;
	
	ptr = list_last_entry(head, struct disk_list, queue);
	if (ptr != NULL) {
		sprintf(path, "%s", ptr->fullname);
		 /* return file size */
		sz = _check_file_size((const char *)ptr->fullname);
//		dprintf("Get List Head FILE : %s\n", ptr->fullname);
		list_del(&ptr->queue);
		ifile->file_count--;
	    if(!strstr(ptr->fullname, NORMAL_FILE))
		    ifile->efile_count-- ;
		free(ptr);
	}

	return ((sz == 0)?-1:0);
}

/*****************************************************************************
 * @brief	 delete all file in list.
 * @section  DESC Description
 *	 - desc : return the size of file
 *****************************************************************************/
#if 0
static void delete_all_node(struct list_head *head)
{
	struct list_head *iter;
	struct disk_list *ptr;

redo:
	__list_for_each(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		list_del(&ptr->queue);
		free(ptr);
		goto redo;
	}
}

static void display_node(struct list_head *head)
{
	struct list_head *iter;
	struct disk_list *ptr;

	__list_for_each(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		dprintf("name %s \n", ptr->fullname);
	}
}
#endif

/*****************************************************************************
 * @brief	 add file name to tail of linked-list (normal file list)
 * @section  DESC Description
 *	 - desc : file name is full path
 *****************************************************************************/
static int add_node_tail(const char *path, struct list_head *head)
{
	struct disk_list *ptr = NULL;

	ptr = (struct disk_list *)malloc(sizeof(struct disk_list));
	if (ptr == NULL) {
		eprintf("can't allocate list\n");
		return -1;
	}

	snprintf(ptr->fullname, sizeof(ptr->fullname), "%s", path);
//	dprintf(" Add List name: %s\n", path);
	INIT_LIST_HEAD(&ptr->queue);
	list_add(&ptr->queue, head);
	ifile->file_count++;
	if(!strstr(ptr->fullname, NORMAL_FILE))
		ifile->efile_count++ ;

	return OSA_SOK;
}

/*****************************************************************************
 * @brief	 add file name to head of linked-list (for ftp)
 * @section  DESC Description
 *	 - desc : file name is full path
 *****************************************************************************/
static int add_node_head(const char *path, struct list_head *head)
{
	struct disk_list *ptr = NULL;

	ptr = (struct disk_list *)malloc(sizeof(struct disk_list));
	if (ptr == NULL) {
		eprintf("can't allocate list\n");
		return -1;
	}

	snprintf(ptr->fullname, sizeof(ptr->fullname), "%s", path);
//	dprintf(" Add List name: %s\n", path);
	INIT_LIST_HEAD(&ptr->queue);
	list_add_tail(&ptr->queue, head);
	ifile->file_count++;
	if(!strstr(ptr->fullname, NORMAL_FILE))
	    ifile->efile_count++ ;
	
	return OSA_SOK;
}

/*****************************************************************************
 * @brief	 delete files
 * @section  DESC Description
 *	 - desc : delete files until del_size reached (or over).
 *****************************************************************************/
static int _delete_files(unsigned long del_sz)
{
	struct list_head *head = &ilist;
	unsigned long tmp = 0;
						
	if (del_sz > 0) 
	{
		/* delete the oldest file (list) */
		while (!list_empty(head))
		{
			Uint32 fsize;
			
			if (find_first_and_delete(head, &fsize) < 0) {
				/* file is empty */
				return -1;
			}
			
			if (tmp >= del_sz)
				break; /* done!! */
			
			tmp += fsize;
			OSA_waitMsecs(5);
		}
	}
	
	return 0;
}

/*****************************************************************************
 * @brief	 make the linked-list with searched files (.avi)
 * @section  DESC Description
 *	 - desc : files are sorted to ascending. 
 *****************************************************************************/
static int __create_list(const char *search_path, char *filters, struct list_head *head, size_t bcnt)
{
	struct stat statbuf;
	struct dirent *entry;
	char __path[256] = {0,};
	DIR *dp;
	
	size_t index=0, len;
	int i;

	list_info_t *list=NULL;
	list_info_t *tmp=NULL;
	
	/* add slash */
	len = strlen(search_path); //# /mmc/DCIM
	strncpy(__path, search_path, len);
	if (search_path[len - 1] != '/')
		strcat(__path, "/"); //# /mmc/DCIM/
	
	/* opendir is not required "/" */
	if ((dp = opendir(__path)) == NULL) {
		eprintf("cannot open directory : %s\n", __path);
		return -1;
	}

	list = (list_info_t *)malloc(sizeof(list_info_t)*bcnt);
	if (list == NULL) {
		closedir(dp);
		return -1;
	}
	
	/* traverse directory (assume -> subdir isn't existed) */
	tmp = list;
	while ((entry = readdir(dp)) != NULL)
	{
		/* checking directory */
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
				continue;

			if (fnmatch(filters, entry->d_name, FNM_CASEFOLD) == 0) {
				strcpy(tmp->name, __path); /* fname[][0] except (/) */
				strcat(tmp->name, entry->d_name);
				index++; tmp++;
			}
		}
	}
	//dprintf("directory scanning done!!\n");
	closedir(dp);
	qsort(list, index, sizeof(list_info_t), _cmpold);
	/* add linked list */
	tmp = list;
	if (index) {
		for (i = 0; i < index; i++, tmp++) {
			add_node_tail(tmp->name, head);
			//dprintf("index %d, fullname %s\n", i, tmp->name);
		}
	}

	if (list != NULL)
		free(list);

	return 0;
}

/* avi list loads from /usr/share/video.lst */
static int __load_file_list(const char *path, struct list_head *head, size_t bcnt)
{
	char msg[MAX_CHAR_128]={0,};
	list_info_t *list, *tmp;
	FILE *f = NULL;
	size_t lcnt;
	int i;
	
	f = fopen(path, "r");
	if (f == NULL) {
		eprintf("failed to open %s!\n", path);
		return -1;
	}
	
	fread((void *)&lcnt, sizeof(int), 1, f);  //# total file count
	snprintf(msg, sizeof(msg), "saved file number is %d in video lst", lcnt);
	notice("%s\n", msg);
	
	if ((lcnt == 0) || (bcnt != lcnt)) {
		memset(msg, 0, sizeof(msg));
		snprintf(msg, sizeof(msg), "file list mismatched (dir-%d, list-%d)!!\n", bcnt, lcnt);
		notice("%s\n", msg);
		return -1;
	}
	
	//# memory alloc
	list = (list_info_t *)malloc(sizeof(list_info_t)*lcnt);
	if (list == NULL) {
		eprintf("failed to allocate memory with file list!\n");
		fclose(f);
		return -1;
	}
	
	fread(list, sizeof(list_info_t)*lcnt, 1, f);
	qsort(list, lcnt, sizeof(list_info_t), _cmpold);
	/* add linked list */
	tmp = list;
	for (i = 0; i < lcnt; i++, tmp++) {
		//("%s video file loaded from list\n", tmp->name);
		add_node_tail(tmp->name, head);
	}
		
	fclose(f);
	if (list != NULL)
		free(list);
	
	return 0;
}

/* avi list save to /usr/share/video.lst */
static int __save_file_list(void)
{
	struct list_head *head = &ilist;
	struct list_head *iter;
	struct disk_list *ptr;
	list_info_t info;
	char path[256]={0,};
	
	FILE *f = NULL;
	int res;
	size_t scnt = ifile->file_count;
	
	/* set file list path */
	sprintf(path, "%s/%s", ifile->rec_root, VIDEO_LIST_NAME);
	res = access(path, R_OK|W_OK);
    if ((res == 0) || (errno == EACCES)) {
		/* delete file */
      	unlink(path); 
    }
	
	f = fopen(path, "w");
	if (f == NULL) {
		eprintf("failed to open %s!\n", path);
		return -1;
	}
	
	notice("[APP_FILE] %d files saved in video list\n", scnt);
	fwrite(&scnt, sizeof(size_t), 1, f);    //# total file count
	list_for_each_prev(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		if (ptr != NULL) {
			strcpy(info.name, ptr->fullname);
			//dprintf("saved name %s, in video list!\n", ptr->fullname);
			fwrite(&info, sizeof(list_info_t), 1, f);	
		}
	}
	fflush(f);
	fclose(f);
	
	return 0;
}

/*****************************************************************************
 * @brief	 check the overwrite recording.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static int full_interval = 0, once_over_beep = 0, once_normal_led = 0;
static void _check_overwite_full_led(int file_state)
{
	int ste = file_state;
	
	switch (ste) {
	case FILE_STATE_OVERWRITE:
		if (!once_over_beep) {
			if (app_rec_state()) {
				printf("@@@@@@@@@@@@@@@@@@FILE_STATE_OVERWRITE@@@@@@@@@@@@@@@@@@@\n");				
				app_leds_mmc_ctrl(LED_MMC_GREEN_BLINK);
				app_buzz_ctrl(80, 1);
				once_over_beep = 1;
			}
		} else {
			if (!app_rec_state()) {
				app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
				once_over_beep = 0 ;
			}
		}
		break;
	case FILE_STATE_FULL:
		full_interval++;
		if ((full_interval % CNT_BEEP_FULL) == 0) {
			printf("@@@@@@@@@@@@@@@@@@FILE_STATE_FULL@@@@@@@@@@@@@@@@@@@\n");				
			app_buzz_ctrl(80, 1);
			full_interval = 0;
		}
		
		if (app_cfg->ste.b.disk_full == OFF) {
			app_leds_mmc_ctrl(LED_MMC_RED_ON);
			app_cfg->ste.b.disk_full = ON;
		}
		break;
	default:	//# normal state
		if (!once_normal_led) {
			printf("@@@@@@@@@@@@@@@@@@FILE_STATE_NORMAL@@@@@@@@@@@@@@@@@@@\n");								
			app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
			once_normal_led = 1;
		}
		if (app_cfg->ste.b.disk_full == ON) {
			app_cfg->ste.b.disk_full = OFF;
			app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
		}
		break;
	}
}

/*****************************************************************************
* @brief    file manangement thread function
* @section  - check rec_dir size & delete old files
            - do every 1 min
*****************************************************************************/
static void *THR_file_mng(void *prm)
{
	app_thr_obj *tObj = &ifile->fObj;

	int cmd, exit=0;
	unsigned int f_cycle=FILE_LIST_CHECK_TIME;
	unsigned int b_cycle=FILE_STATE_CHECK_BEEP;
	int r = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		if (!app_cfg->ste.b.mmc_err)
			app_cfg->wd_flags |= WD_FILE;
				
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			/* watchdog disable */
			app_cfg->wd_flags &= ~WD_FILE;
			app_cfg->wd_tot &= ~WD_FILE; 
			exit = 1;
			break;
		}
		
		if (app_cfg->ste.b.mmc && app_cfg->ste.b.cap) 
		{
			//# file size check and delete -- per 1 min
			if ((f_cycle % FILE_STATE_CHECK_TIME) == 0) 
			{
				int capacity_full = 0;

				//# Get Disk MAX Size.
				_check_threshold_size(ifile);
				app_file_update_disk_usage();
				capacity_full = (app_file_check_disk_free_space() == 0) ? 0 : 1;

				if (app_set->rec_info.overwrite) {
					if (capacity_full) {
						ifile->file_state = FILE_STATE_OVERWRITE;
						OSA_mutexLock(&ifile->mutex_file);
						//1GB - available size = delete size
						r = _delete_files((MIN_THRESHOLD_SIZE-ifile->disk_avail));
						OSA_mutexUnlock(&ifile->mutex_file);
						if (r == EFAIL) {
							eprintf("[APP_FILE] !! SD Card Error...reboot!!\n");
							if (app_rec_state() > 0) {
								app_rec_stop(ON); 
								app_msleep(500); /* wait for record done!! */
							}
							app_buzz_ctrl(80, 2); //# Power Off Buzzer
							app_mcu_pwr_off(OFF_RESET);
							continue;
						}
					}
				} else {
					if (capacity_full) {
						app_rec_stop(OFF); /* buzzer off */
						ifile->file_state = FILE_STATE_FULL;
					} else {
						ifile->file_state = FILE_STATE_NORMAL;
					}					 
				}
				f_cycle = 0;
			}
			
			//# file state check for beep -- per 1 sec
			if ((b_cycle % FILE_STATE_CHECK_BEEP) == 0) 
			{
				_check_overwite_full_led(ifile->file_state);
				b_cycle = 0;
			}
		} /* if (app_cfg->ste.b.mmc && app_cfg->ste.b.cap)  */
		
		OSA_waitMsecs(FILE_LIST_CYCLE);
		f_cycle += FILE_LIST_CYCLE;
		b_cycle += FILE_LIST_CYCLE;
	}
	
	tObj->active = 0;
	aprintf("...exit\n");
	sync();

	return NULL;
}

/*****************************************************************************
* @brief    file manager init/exit function
* @section  [desc]
*****************************************************************************/
int app_file_init(void)
{
	char flist_path[256]={0,};
	int res, status;
	size_t num_of_files;
	
    memset(ifile, 0, sizeof(app_file_t));
	
	ifile->file_state = FILE_STATE_NORMAL;
	sprintf(ifile->rec_root, "%s/%s", SD_MOUNT_PATH, REC_DIR);
	
	//#-- create directories such as DCIM, ufs
	_check_rec_dir((const char *)ifile->rec_root);
	/* Get the number of files in DCIM */
	num_of_files = get_file_count((const char *)ifile->rec_root);
	/* set file list path */
	sprintf(flist_path, "%s/%s", ifile->rec_root, VIDEO_LIST_NAME);
	res = __load_file_list((const char *)flist_path, &ilist, num_of_files);
	if (res < 0) {
		status = __create_list((const char *)ifile->rec_root, AVI_EXT, &ilist, num_of_files);
		if (status == EFAIL) 	{
			sysprint("Failed to make file list!!\n");
			return status;
		}
	}
	
	/* To check mmc threshold size */
	_check_threshold_size(ifile);
	app_file_update_disk_usage();
	if (app_file_check_disk_free_space() < 0) {
		/* 최소 공간 보다 작을 경우, overwrite 모드 */
		if (app_set->rec_info.overwrite) {
			ifile->file_state = FILE_STATE_OVERWRITE;
			//# 1GB - available size = delete size
			res = _delete_files((MIN_THRESHOLD_SIZE-ifile->disk_avail));
			if (res < 0)
			{
				eprintf("[APP_FILE] !! SD Card Error...reboot!!\n");
				app_buzz_ctrl(80, 2); //# Power Off Buzzer
				app_mcu_pwr_off(OFF_RESET);
				return res;
			}
		} else {
			ifile->file_state = FILE_STATE_FULL;
		}
	}
	
    //#--- create normal record thread
	status = OSA_mutexCreate(&(ifile->mutex_file));
    OSA_assert(status == OSA_SOK);
	
	if (thread_create(&ifile->fObj, THR_file_mng, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	/* watchdog file enable */
	app_cfg->wd_tot |= WD_FILE;
	aprintf("... done!\n");
	
	return 0;
}

/*****************************************************************************
* @brief    file manager init/exit function
* @section  [desc]
*****************************************************************************/
void app_file_exit(void)
{
	app_thr_obj *tObj = &ifile->fObj;
	int status;
	
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		OSA_waitMsecs(20);

	thread_delete(tObj);
	/* overwrite 모드일 때 file thread가 종료되지 않으면 리스트 갱신 후 delete 되는 파일로 
	 * 인해서 실제 파일 저장 갯수와 리스트의 갯수가 일치되지 않음.
	 * 여기로 변경.
	 */
	__save_file_list();
	status = OSA_mutexDelete(&(ifile->mutex_file));
	OSA_assert(status == OSA_SOK);
	ifile = NULL;

    aprintf("... done!\n");
}

/*****************************************************************************
 * @brief	 check the amount of disk usage.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
#define DISK_USED_MID	66
#define DISK_USED_MIN	33
void app_file_update_disk_usage(void)
{
	unsigned long percent = 0;
	unsigned long sum  = ifile->disk_max;
	unsigned long used = ifile->disk_used;
	unsigned long long denominator;
	
	denominator = ((unsigned long long)used * 100);
	percent = (unsigned long)(denominator / sum);
	
//	dprintf("file manger total %u(KB), used %u(KB), percent %d%%\n", sum, used, percent);	
	if (percent > DISK_USED_MID)
		app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_3);	//# x > 66%
	else if(percent < DISK_USED_MIN )
		app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_1);	//# x < 33%
	else
		app_leds_mmc_capacity_ctrl(LED_DISK_USAGE_ON_2);	//# 33% < x < 66%
}

/*****************************************************************************
* @brief    get disk free size
* @section  [desc]
*****************************************************************************/
unsigned long app_file_get_free_size(void)
{
	unsigned long sum_sz  = ifile->disk_max;
	unsigned long used_sz = ifile->disk_used;
	
	return (sum_sz - used_sz);
}

/*****************************************************************************
* @brief    add file to list
* @section  [desc]
*****************************************************************************/
int app_file_add_list(const char *pathname)
{
	OSA_mutexLock(&ifile->mutex_file);	
	add_node_tail(pathname, &ilist);
//	dprintf("ADDED FILE : %s(%ld) ===\n", pathname, ifile->file_count);
	OSA_mutexUnlock(&ifile->mutex_file);

	return SOK;
}

/*****************************************************************************
* @brief    Get file name and size for ftp backup
* @section  
  - desc :  - if success, return the file size(KB), the other 0.
  			- path argument is full path.
*****************************************************************************/
int get_ftp_send_file(char *path)
{
	int ret;
	
	//# get file name and remove list
	ret = _get_rec_file_head(&ilist, path);
	if (ret < 0)
		return 0;
		
	return (1);
}

void save_filelist(void)
{
	__save_file_list();
}

/*****************************************************************************
* @brief    resotre file that was failed send on ftp.
* @section  
  - desc :  - this path is added to head of linkd-list.
  			- if success 0 is returned, others -1.
*****************************************************************************/
int restore_ftp_file(char *pathname)
{
	struct stat sb;
	size_t len = 0;
	
    lstat(pathname, &sb);
	len = (sb.st_size / KB);
	add_node_head(pathname, &ilist);
	
	return 0;
}

/*****************************************************************************
* @brief    Delete file that is backup done with ftp.
* @section  
  - desc :  - if success return 0, the other -1.
*****************************************************************************/
int delete_ftp_send_file(char *path)
{
	return delete_file((const char *)path);
}

/*****************************************************************************
* @brief    check Recorded file exist  .
* @section 
  - desc :  - if success return file count, the other 0.
*****************************************************************************/
int get_recorded_file_count(void)
{
	return (int)(ifile->file_count); 
}

/*****************************************************************************
* @brief    check Recorded event file exist  .
* @section 
  - desc :  - if success return file count, the other 0.
*****************************************************************************/
int get_recorded_efile_count(void)
{
	return (int)(ifile->efile_count); 
}

/*****************************************************************************
* @brief    check SD write status  .
* @section 
  - desc :  - overwrite mode or not
*****************************************************************************/
int get_write_status(void)
{
	return ifile->file_state; 
}
