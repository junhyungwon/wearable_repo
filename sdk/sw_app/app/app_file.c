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
#define FILE_LED_CYCLE         		(500)
#define FILE_STATE_CHECK_BEEP		(1000)		//1sec
#define FILE_STATE_FULL_BEEP_TIME	(5*1000)	//5SEC

#define CNT_BEEP_CHECK 				(FILE_STATE_CHECK_BEEP/FILE_LED_CYCLE)
#define CNT_BEEP_FULL 				(FILE_STATE_FULL_BEEP_TIME/FILE_LED_CYCLE)

#if FREC_TEST
	#define MIN_THRESHOLD_SIZE 	    (100*MB)/KB		//# 100MB
#else
	#define MIN_THRESHOLD_SIZE 	    (1024*MB)/KB	//# 1GB
#endif

#define VIDEO_LIST_NAME				"video.lst"

typedef struct {
	char name[FILE_MAX_PATH_LEN];
	int type;
} linked_info_t;

struct disk_list {
	struct list_head queue;
	linked_info_t node;
};

typedef struct {
	OSA_MutexHndl mutex_file ;
	app_thr_obj fObj;
	app_thr_obj lObj; /* LED */
	
	size_t file_count;   /* total files for ftp send */
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

static LIST_HEAD(storageList);

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

	/* SD ī�� ���� �� ����Ʈ�� �ȵż� �ڵ����� ������ ��. 
	 * ���� error�� �߻����� �����Ƿ� ���ϰ� üũ�ϴ� �κ� ����
	 */
	util_disk_info(&idisk, SD_MOUNT_PATH);
	
	pInfo->disk_max 	= idisk.total;
	pInfo->disk_avail 	= idisk.avail;
	pInfo->disk_used	= (idisk.total-idisk.avail);
//	dprintf("MAX capacity %ld(KB)\n", pInfo->disk_max);
//	dprintf("available capacity %ld(KB)\n", pInfo->disk_avail);
}

static int _check_file_size(const char *fname, Uint32 *sz)
{
	struct stat st;
	Uint32 len=0;
	
	if (fname != NULL) {
		if (stat(fname, &st) == 0) {
			len = (st.st_size / KB); /* byte -> KB */
			//dprintf("check file size %s(%dKB)\n", fname, len);
			*sz = len;
			return 0;
		}
	} 
		
	return -1;
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
 * @brief	 compare file name
 * @section  DESC Description
 *	 - desc
 *****************************************************************************/
static int _cmpold(const void *a, const void *b)
{
	linked_info_t *a_name = (linked_info_t *)a;
	linked_info_t *b_name = (linked_info_t *)b;

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
	int ret=0;
	
	if (del_sz == NULL)
		return -1;
	
	*del_sz = 0; /* default */	
	ptr = list_last_entry(head, struct disk_list, queue);
	if (ptr != NULL) {
		ret = _check_file_size((const char *)ptr->node.name, &sz);
		if (!ret) {
			/* delete file */
			if (remove(ptr->node.name) == 0) {
				OSA_waitMsecs(5);
				//dprintf("DELETE FILE : %s (%d KB)\n", ptr->node.name, sz);
				list_del(&ptr->queue);
				ifile->file_count--;
				if(!strstr(ptr->node.name, NORMAL_FILE))
					ifile->efile_count-- ;
				free(ptr);
				*del_sz = sz;
				return 0; /* success */
			}
		}
	}
	
	eprintf("Failed to delete files for overwriting!\n");
	return -1; /* failure */
}

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

	snprintf(ptr->node.name, sizeof(ptr->node.name), "%s", path);
//	dprintf(" Add List name: %s\n", path);
	INIT_LIST_HEAD(&ptr->queue);
	list_add(&ptr->queue, head);
	ifile->file_count++;
	
	if(strstr(ptr->node.name, NORMAL_FILE) == NULL)
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

	snprintf(ptr->node.name, sizeof(ptr->node.name), "%s", path);
//	dprintf(" Add List name: %s\n", ptr->node.name);
	INIT_LIST_HEAD(&ptr->queue);
	list_add_tail(&ptr->queue, head);
	ifile->file_count++;
	
	if(strstr(ptr->node.name, NORMAL_FILE) == NULL)
	    ifile->efile_count++ ;
	
	return OSA_SOK;
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
		dprintf("name %s \n", ptr->node.name);
	}
}
#endif

/*****************************************************************************
 * @brief	 delete files
 * @section  DESC Description
 *	 - desc : delete files until del_size reached (or over).
 *****************************************************************************/
static int _delete_files(unsigned long del_sz)
{
	struct list_head *head = &storageList;
	unsigned long tmp = 0;
					
//	dprintf("Required space %ld(KB)\n", del_sz);					
	if (del_sz > 0) 
	{
		/* delete the oldest file (list) */
		while (!list_empty(head))
		{
			Uint32 fsize=0;
			
			if (find_first_and_delete(head, &fsize) < 0) {
				/* file is empty */
				return -1;
			}
			
			if (tmp >= del_sz) {
				//sync();
				break;
			}
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

	linked_info_t *list=NULL;
	linked_info_t *tmp=NULL;
	
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

	list = (linked_info_t *)malloc(sizeof(linked_info_t)*bcnt);
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
	qsort(list, index, sizeof(linked_info_t), _cmpold);
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
	linked_info_t *list, *tmp;
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
	list = (linked_info_t *)malloc(sizeof(linked_info_t)*lcnt);
	if (list == NULL) {
		eprintf("failed to allocate memory with file list!\n");
		fclose(f);
		return -1;
	}
	
	fread(list, sizeof(linked_info_t)*lcnt, 1, f);
	qsort(list, lcnt, sizeof(linked_info_t), _cmpold);
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
	struct list_head *head = &storageList;
	struct list_head *iter;
	struct disk_list *ptr;
	linked_info_t info;
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
			strcpy(info.name, ptr->node.name);
			//dprintf("saved name %s, in video list!\n", ptr->fullname);
			fwrite(&info, sizeof(linked_info_t), 1, f);	
		}
	}
	fflush(f);
	fclose(f);
	
	return 0;
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
	int r = 0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		cmd = event_wait(tObj);
		if(cmd == APP_CMD_EXIT) {
			dprintf("file manager exit!\n");
			break;
		}
				
		if (app_cfg->ste.b.mmc && app_cfg->ste.b.cap && !app_cfg->ste.b.mmc_err) 
		{
			//# file size check and delete -- per 1 min, Don't delete file during ftp sending..
			int capacity_full = 0;

			//# Get Disk MAX Size.
			_check_threshold_size(ifile);
			app_file_update_disk_usage();
			capacity_full = (app_file_check_disk_free_space() == 0) ? 0 : 1;

			if (app_cfg->rec_overwrite) {
				if (capacity_full) {
					ifile->file_state = FILE_STATE_OVERWRITE;
					OSA_mutexLock(&ifile->mutex_file);
					//# 1GB - available size = delete size
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
					printf("REC_OVERWRITE Capacity_full\n") ;
					if(app_rec_state() == 2) // SOS 
						app_sos_send_stop(ON) ; // send sos stop packet to client

					app_rec_stop(OFF); /* buzzer off */
					ifile->file_state = FILE_STATE_FULL;
				} else {
					ifile->file_state = FILE_STATE_NORMAL;
				}
			}
		} /* if (app_cfg->ste.b.mmc && app_cfg->ste.b.cap && !app_cfg->ste.b.mmc_err)  */
	}
	
	tObj->active = 0;
	aprintf("...exit\n");
	sync();

	return NULL;
}

/*****************************************************************************
* @brief    file capacity led manager thread function
*****************************************************************************/
static void *THR_file_led_mng(void *prm)
{
	app_thr_obj *tObj = &ifile->lObj;

	int cmd, exit=0;
	
	int once_over_beep = 0;
	int once_normal_led = 0;
	
	unsigned long b_cycle=0;
	unsigned long f_cycle=0;
	
	aprintf("enter...\n");
	tObj->active = 1;
	
	while (!exit)
	{
		int state;
		
		cmd = tObj->cmd;
		if (cmd == APP_CMD_EXIT) {
			break;
		}
		
		//##### SD Capacity LED Update ###########################
		//# file state check for beep -- per 1 sec
		if (b_cycle >= CNT_BEEP_CHECK) 
		{
			state = get_write_status(); 
			if (state == FILE_STATE_OVERWRITE) {
				f_cycle = 0;
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
						once_over_beep = 0;
					}
					else
						app_leds_mmc_ctrl(LED_MMC_GREEN_BLINK);
				}
			} else if (state == FILE_STATE_FULL) {
				if (f_cycle >= CNT_BEEP_FULL) {
					printf("@@@@@@@@@@@@@@@@@@FILE_STATE_FULL@@@@@@@@@@@@@@@@@@@\n");
					f_cycle = 0;
					app_buzz_ctrl(80, 1);
				} else 
					f_cycle++;
				
				if (app_cfg->ste.b.disk_full == OFF) {
					app_cfg->ste.b.disk_full = ON;
				}
					app_leds_mmc_ctrl(LED_MMC_RED_ON);
			} else {
				//# normal record state.
				f_cycle = 0;
				if (!once_normal_led) {
					printf("@@@@@@@@@@@@@@@@@@FILE_STATE_NORMAL@@@@@@@@@@@@@@@@@@@\n");								
					app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
					once_normal_led = 1;
				}
				
				if (app_cfg->ste.b.disk_full == ON) {
					app_cfg->ste.b.disk_full = OFF;
				}
					app_leds_mmc_ctrl(LED_MMC_GREEN_ON);
			}
			b_cycle = 0;
		} else {
			b_cycle++;
		}
		app_msleep(FILE_LED_CYCLE);
	}
	
	tObj->active = 0;
	aprintf("...exit\n");

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
	res = __load_file_list((const char *)flist_path, &storageList, num_of_files);
	if (res < 0) {
		status = __create_list((const char *)ifile->rec_root, AVI_EXT, &storageList, num_of_files);
		if (status == EFAIL) 	{
			sysprint("Failed to make file list!!\n");
			return status;
		}
	}
	
	//# default LED ���¿� �뷮�� üũ (overwrite ��� �����ϸ� ��ȭ �ȵ�)
	_check_threshold_size(ifile);
	app_file_update_disk_usage();
	
    //#--- create normal record thread
	status = OSA_mutexCreate(&(ifile->mutex_file));
    OSA_assert(status == OSA_SOK);
	
	if (thread_create(&ifile->fObj, THR_file_mng, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return -1;
	}
	
	/* Create File LED Thread */
	if (thread_create(&ifile->lObj, THR_file_led_mng, APP_THREAD_PRI, NULL, __FILENAME__) < 0) {
		eprintf("create thread\n");
		return -1;
	}

	aprintf("... done!\n");
	return 0;
}

/*****************************************************************************
* @brief    file manager init/exit function
* @section  [desc]
*****************************************************************************/
void app_file_exit(void)
{
	app_thr_obj *tObj;
	int status;
	
	/* delete LED Thread */
	tObj = &ifile->lObj;
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		OSA_waitMsecs(20);
	thread_delete(tObj);
	
	tObj = &ifile->fObj;	
	event_send(tObj, APP_CMD_EXIT, 0, 0);
	while (tObj->active)
		OSA_waitMsecs(20);
	thread_delete(tObj);
	
	/* overwrite ����� �� file thread�� ������� ������ ����Ʈ ���� �� delete �Ǵ� ���Ϸ� 
	 * ���ؼ� ���� ���� ���� ������ ����Ʈ�� ������ ��ġ���� ����.
	 * ����� ����.
	 */
	__save_file_list();
	status = OSA_mutexDelete(&(ifile->mutex_file));
	OSA_assert(status == OSA_SOK);
	ifile = NULL;

    aprintf("... done!\n");
}
/*****************************************************************************
* @brief    file manager callback function
* @section  [desc]
*****************************************************************************/
void app_file_fxn(void)
{
	event_send(&ifile->fObj, APP_CMD_NOTY, 0, 0);
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
	add_node_tail(pathname, &storageList);
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
int get_ftp_send_file(int index, char *path)
{
	struct list_head *head = &storageList;
	struct list_head *iter;
	struct disk_list *ptr;
	int count=0;
	
	if (path == NULL) {
		eprintf("path is null!\n");
		return -1;
	}
	
	/* To get nth index file */
	list_for_each_prev(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		if (ptr != NULL) {
			if (count == index) {
				/* file name copy */
				strcpy(path, ptr->node.name);
				//dprintf("index %d file name is %s\n", count, path);
				return 0;
			} else {
				count++;
			}
		} else {
			//dprintf("index %d not founded file\n", count);
		}
	}
	
	return -1;
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
	struct list_head *head = &storageList;
	struct stat sb;
	size_t len = 0;
	
    lstat(pathname, &sb);
	len = (sb.st_size / KB);
	add_node_head(pathname, head);
	
	return 0;
}

/*****************************************************************************
* @brief    Delete file that is backup done with ftp.
* @section  
  - desc :  - if success return 0, the other -1.
*****************************************************************************/
void delete_ftp_send_file(const char *path)
{
	struct list_head *head = &storageList;
	struct list_head *iter;
	struct disk_list *ptr;
	
	if (path == NULL)
		return;
	
	/* iterate over a list */	
	__list_for_each(iter, head) {
		ptr = list_entry(iter, struct disk_list, queue);
		if (ptr != NULL) {
			if (strcmp(path, ptr->node.name) == 0) {
				/* valid check (access() ) ??? */
				//dprintf("delete fname ==> %s\n", path);
				remove(path); //# or unlink
				ifile->file_count--;
				if (strstr(path, NORMAL_FILE) == NULL) {
					/* event file */
					ifile->efile_count--;
				}
				/* list delete */
				list_del(&ptr->queue);
				free(ptr);
				return;
			}
		}
	}
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
