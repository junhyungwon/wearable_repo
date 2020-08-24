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
typedef struct {
	char name[FILE_MAX_PATH_LEN];
	Uint32 size;  /* ?? ? ? */

} list_info_t;

/* file_entry_t? ??? */
struct disk_list {
	struct list_head queue;
	char fullname[FILE_MAX_PATH_LEN];
	Uint32 filesz; /* ??? ?? ?? */
};

typedef struct {
	OSA_MutexHndl mutex_file ;
	app_thr_obj fObj;

	int	file_state;
	int file_count;  /* total files for ftp send */
	
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
static int _check_threshold_size(app_file_t *pInfo)
{
	disk_info_t idisk;
	int ret = EFAIL;

	ret = util_disk_info(&idisk, SD_MOUNT_PATH);
	if (ret != EFAIL) {
		pInfo->disk_max = ((unsigned long long)idisk.total * 100)/100;
		pInfo->disk_avail = idisk.avail;
		
		dprintf("MAX capacity %ld(KB)\n", pInfo->disk_max);
		dprintf("available capacity %ld(KB)\n", pInfo->disk_avail);
	} else {
		eprintf("%s fault!!\n", SD_MOUNT_PATH);
	}

	return ret;
}

/*****************************************************************************
 * @brief	 check free space of disk.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static int _check_disk_free_space(void)
{
	unsigned long sum = ifile->disk_max;
	unsigned long used = ifile->disk_used;
	int ret = 0;
	
	//dprintf("Check free : %ld(KB) / USED: %ld(KB) / threshold : %ld(KB)!\n", (sum - used), used, MIN_THRESHOLD_SIZE);
	if ((used + MIN_THRESHOLD_SIZE) >= sum) 
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
	char cmd[1024] = {0,};
	FILE *f = NULL;
	Uint32 count;

	snprintf(cmd, sizeof(cmd), "/bin/ls -1 %s | /usr/bin/wc -l", dpath);
	f = popen(cmd, "r");
	if (f != NULL) {
		fscanf(f, "%u ", &count);
		pclose(f);
	} else
		count = 0;

	FILE_DBG("%u files in %s path\n", count, dpath);

	return count;
}

/*****************************************************************************
 * @brief	 delete file.
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int delete_file(const char *pathname)
{
	if (pathname == NULL) {
		eprintf("filename is null!\n");
		return -1;
	}

	if (remove(pathname) < 0) {
		eprintf("can't remove %s(%d)\n", pathname, errno);
		return -1;
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

	return (strcmp(a_name->name, b_name->name));
}

/*****************************************************************************
 * @brief	 delete header of linked-list
 * @section  DESC Description
 *	 - desc : return the size of file
 *****************************************************************************/
static Uint32 find_first_and_delete(struct list_head *head)
{
	struct disk_list *ptr = NULL;
	Uint32 sz = 0;

	ptr = list_last_entry(head, struct disk_list, queue);
	if (ptr != NULL)
	{
		if (delete_file(ptr->fullname) < 0)
			return 0;
			
		sz = ptr->filesz; /* return file size */
		FILE_DBG("DELETE FILE : %s\n", ptr->fullname);
		list_del(&ptr->queue);
		ifile->file_count--;
		ifile->disk_used -= sz;
		free(ptr);
	}

	return sz;
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
		sz = ptr->filesz; /* return file size */
		
		FILE_DBG("Get List Head FILE : %s\n", ptr->fullname);
		list_del(&ptr->queue);
		ifile->file_count--;
		ifile->disk_used -= sz;
		free(ptr);
	}

	return ((sz == 0)?-1:0);
}

/*****************************************************************************
 * @brief	 delete all file in list.
 * @section  DESC Description
 *	 - desc : return the size of file
 *****************************************************************************/
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

#if 0
static void display_node(struct list_head *head)
{
	struct list_head *iter;
	struct disk_list *ptr;

	__list_for_each(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		FILE_DBG("name %s \n", ptr->fullname);
	}
}
#endif

/*****************************************************************************
 * @brief	 add file name to tail of linked-list (normal file list)
 * @section  DESC Description
 *	 - desc : file name is full path
 *****************************************************************************/
static int add_node_tail(const char *path, struct list_head *head, unsigned int iSize)
{
	struct disk_list *ptr = NULL;

	ptr = (struct disk_list *)malloc(sizeof(struct disk_list));
	if (ptr == NULL) {
		eprintf("can't allocate list\n");
		return -1;
	}

	ptr->filesz = iSize;
	snprintf(ptr->fullname, sizeof(ptr->fullname), "%s", path);

	FILE_DBG(" Add List name: %s, size %u\n", path, iSize);

	INIT_LIST_HEAD(&ptr->queue);
	list_add(&ptr->queue, head);
	ifile->file_count++;
	ifile->disk_used += iSize;

	return OSA_SOK;
}

/*****************************************************************************
 * @brief	 add file name to head of linked-list (for ftp)
 * @section  DESC Description
 *	 - desc : file name is full path
 *****************************************************************************/
static int add_node_head(const char *path, struct list_head *head, unsigned int iSize)
{
	struct disk_list *ptr = NULL;

	ptr = (struct disk_list *)malloc(sizeof(struct disk_list));
	if (ptr == NULL) {
		eprintf("can't allocate list\n");
		return -1;
	}

	ptr->filesz = iSize;
	snprintf(ptr->fullname, sizeof(ptr->fullname), "%s", path);

//	FILE_DBG(" Add List name: %s, size %u(KB)\n", path, iSize);

	INIT_LIST_HEAD(&ptr->queue);
	list_add_tail(&ptr->queue, head);
	ifile->file_count++;
	ifile->disk_used += iSize;
	
	return OSA_SOK;
}

/*****************************************************************************
 * @brief	 make the linked-list with searched files (.avi)
 * @section  DESC Description
 *	 - desc : files are sorted to ascending. 
 *****************************************************************************/
static int _create_list(const char *search_path, char *filters,
					struct list_head *head, unsigned long *size)
{
	struct stat statbuf;
	struct dirent *entry;
	char __path[1024] = {};
	DIR *dp;
	
	size_t index, fcount;
	size_t len, total;
	int i;

	list_info_t *list, *tmp;
	total = 0;
	
	/* add slash */
	len = strlen(search_path);
	strncpy(__path, search_path, len);
	if (search_path[len - 1] != '/')
		strcat(__path, "/");

	/* opendir is not required "/" */
	if ((dp = opendir(__path)) == NULL) {
		eprintf("cannot open directory : %s\n", __path);
		*size = 0;
		return -1;
	}

	/* get file count */
	fcount = get_file_count(__path);
	if (!fcount) {
		*size = 0; closedir(dp);
		FILE_DBG("Empty %s directory\n", __path);
		return 0;
	}

	index = 0;
	list = (list_info_t *)malloc(sizeof(list_info_t) * fcount);

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

				//printf("founded index %d, fullname %s\n", index, tmp->name);
				index++; tmp++;
			}
		}
	}

	closedir(dp);
	qsort(list, index, sizeof(list_info_t), _cmpold);

	/* add linked list */
	tmp = list;
	if (index) {
		for (i = 0; i < index; i++, tmp++) {
			lstat(tmp->name, &statbuf);
			len = (statbuf.st_size / KB); /* byte -> KB */
			add_node_tail(tmp->name, head, len);
			total += len;
		}
	}

	if (list)
		free(list);

	if (size != NULL)
		*size = total;

	return 0;
}

/*****************************************************************************
 * @brief	 delete files
 * @section  DESC Description
 *	 - desc : delete files until del_size reached (or over).
 *****************************************************************************/
static int _delete_files(unsigned long min_sz)
{
	struct list_head *head = &ilist;
	
	unsigned long sum_sz  = ifile->disk_max;
	unsigned long used_sz = ifile->disk_used;
	unsigned long del_sz = 0;
	unsigned long tmp = 0;
	unsigned long free_sz;
	
	/* Free Space is reserve space (for safe) */
	if (sum_sz > 0) {
		free_sz = sum_sz - used_sz;
		
		if (min_sz > free_sz)
			del_sz  = min_sz - free_sz;
	}
	
//	printf("min capacity: %ld(KB)==== free capacity: %ld(KB), DEL: %ld(KB)\n", 
//					min_sz,  free_sz, del_sz);
						
	if (del_sz > 0) 
	{
		/* delete the oldest file (list) */
		while (!list_empty(head))
		{
			Uint32 fsize;
			fsize = find_first_and_delete(head);
			if (!fsize) {
				/* file ??? 0? ??? ???? */
				return -1;
			}
				
			ifile->disk_used -= fsize;
			FILE_DBG("DELETE FILE : (%d KB)=== tot_size = %ld\n", fsize, 
						ifile->disk_used);
			
			if (tmp >= del_sz)
				break; /* done!! */
			
			tmp += fsize;
			OSA_waitMsecs(5);
		}
	}
	
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
				dev_buzz_ctrl(80, 1);
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
		if ((full_interval %= CNT_BEEP_FULL) == 0) {
			printf("@@@@@@@@@@@@@@@@@@FILE_STATE_FULL@@@@@@@@@@@@@@@@@@@\n");				
			dev_buzz_ctrl(80, 1);
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
 * @brief	 check the amount of disk usage.
 * @section  DESC Description
 *	 - desc : 
 *****************************************************************************/
static void _set_disk_full_state(app_file_t *prm)
{
	if (prm->file_state == FILE_STATE_NORMAL)
		prm->file_state = (app_set->rec_info.overwrite)?FILE_STATE_OVERWRITE:FILE_STATE_FULL;
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
	unsigned int f_cycle=FILE_LIST_CHECK_TIME; // A©øA¨ö ¨öAAU¢¯¢®¨ù¡© CHECK START CI¥ì¥ì¡¤I
	char msg[MAX_CHAR_255] = {0,};
	int r = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		app_cfg->wd_flags |= WD_FILE;
		
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT || app_cfg->ste.b.mmc_err) {
			exit = 1;
			break;
		}
		
		//# sd??? ??? file manager  ?? ??.
		//# ??? sd??? ???? ??? ?.
		//if (app_cfg->ste.b.mmc && app_cfg->ste.b.cap) {
        if (app_cfg->ste.b.cap) {
			//# file size check and delete -- per 1 min
	        if ((f_cycle % FILE_STATE_CHECK_TIME) == 0) 
			{
				int capacity_full = 0;
				
				app_file_update_disk_usage();
				if (_check_disk_free_space() < 0) capacity_full = 1;
				else							  capacity_full = 0;
				
				if (!app_set->rec_info.overwrite) {
					/* overwrite ??? ?? ?? */
					if (capacity_full) {
						/* ?? ??? ?? ? */
						app_rec_stop(0); /* buzzer off */
					}
					
					ifile->file_state = FILE_STATE_NORMAL;
				} else {
					/* overwrite ?? ? ?? */
					_set_disk_full_state(ifile);
					
					OSA_mutexLock(&ifile->mutex_file);
					r = _delete_files(MIN_THRESHOLD_SIZE);
					OSA_mutexUnlock(&ifile->mutex_file);
					
					if (r == EFAIL) {
						eprintf("Delete file Error!!\n");
						app_cfg->ste.b.mmc_err = 1;
						app_rec_stop(ON);
						continue;
					}
				}
				
			    _check_overwite_full_led(ifile->file_state);
				f_cycle = 0;
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
int app_file_init(void)
{
	char msg[MAX_CHAR_128];
	int ret = SOK, status;
	
	if (app_cfg->ste.b.mmc == 0)
		return EFAIL;
	
    memset(ifile, 0, sizeof(app_file_t));

	sprintf(ifile->rec_root, "%s/%s", SD_MOUNT_PATH, REC_DIR);
	ifile->file_state = FILE_STATE_NORMAL;

	//#-- create directories such as DCIM, ufs
	_check_rec_dir(ifile->rec_root);
	ret = _check_threshold_size(ifile);
    if (ret < 0) {
		sprintf(msg, "[APP_FILE] !! Get threshold size failed !!!") ;
		app_log_write(MSG_LOG_WRITE, msg);
		goto err_ret;	
	}

	ret = _create_list((const char *)ifile->rec_root, AVI_EXT, &ilist, &ifile->disk_used);
	if (ret == EFAIL) 	{
		eprintf("make file list fail!! \n");
		goto err_ret;
    }
	
    //#--- create normal record thread
	status = OSA_mutexCreate(&(ifile->mutex_file));
    OSA_assert(status == OSA_SOK);
	
	if (thread_create(&ifile->fObj, THR_file_mng, APP_THREAD_PRI, NULL) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	//# ??? ?? ? watchdog ???
	app_cfg->wd_tot |= WD_FILE;
	aprintf("... done!\n");
	
	return 0;
	
err_ret:
	app_cfg->ste.b.mmc_err = 1;
	return EFAIL;
}

/*****************************************************************************
* @brief    file manager init/exit function
* @section  [desc]
*****************************************************************************/
void app_file_exit(void)
{
	app_thr_obj *tObj = &ifile->fObj;
	int ret = 0,status;
	
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		OSA_waitMsecs(20);

	thread_delete(tObj);

	status = OSA_mutexDelete(&(ifile->mutex_file));
	OSA_assert(status == OSA_SOK);

	ifile = NULL;

    aprintf("done...\n");
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
	
	percent = ((used * 100) / sum);
		
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
int app_file_add_list(const char *pathname, int size)
{
	int ret = EFAIL;
	
	OSA_mutexLock(&ifile->mutex_file);	
	add_node_tail(pathname, &ilist, (unsigned int)size);
//	FILE_DBG("ADDED FILE : %s(%ld) ===\n", pathname, ifile->file_count);
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
		
	/* ??? ?? 0? ?? ?? ?? */	
	return (1);
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
	add_node_head(pathname, &ilist, len);
	
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
	return (ifile->file_count); 
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
