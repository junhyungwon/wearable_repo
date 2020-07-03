/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
  * @file	 app_file.c
  * @brief	 recorded file management with linked-list
  * @author  sksung
  * @section MODIFY history
  * 	- 2018.12.04 : First Created
  */
 /*****************************************************************************/

#ifndef _APP_FILE_H_
#define _APP_FILE_H_


/*----------------------------------------------------------------------------
 Defines referenced	header files
 -----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
 -----------------------------------------------------------------------------*/
#define FREC_TEST	(0)
#define REC_DIR		"DCIM"

typedef struct {
	unsigned long total ;
	unsigned long used ;
	unsigned long free ;
} disk_size_t ;


typedef enum {
	FILE_STATE_NORMAL	= 100, // # NO OVERWRITE
	FILE_STATE_OVERWRITE,      // # DISK FULL, USE OVERWIRTE MODE
	FILE_STATE_FULL,           // # DISK FULL, NOT USE OVERWRITE MODE
	FILE_STATE_MAX
}file_state_e ;

typedef struct {
	char fname[64];
	unsigned int size ;
	void* prev ;
    void* next ;
}file_entry_t ;


typedef struct {
	file_entry_t* head;
	file_entry_t* tail ;
	unsigned long tot_size ;
	unsigned long file_cnt ;
}file_list_t ;


typedef struct {
	OSA_MutexHndl mutex_file ;
	app_thr_obj   *fObj;
	file_list_t   flist;

	int           file_state;
	char          rec_root[32];
	unsigned long disk_avail ;
	unsigned long size_max ;
} app_file_t ;

extern app_file_t *ifile ;


/*----------------------------------------------------------------------------
 Declares a	function prototype
 -----------------------------------------------------------------------------*/
int app_file_start(void);
void app_file_stop(void);

int app_file_add(char* path);

unsigned long get_ftp_send_file(char* path);
int restore_ftp_file(char* path);
int delete_ftp_send_file(char* path);
int get_recorded_file_count(void) ;
void set_display_disk_usage(void) ;
int get_write_status(void);
int _get_disk_kb_info(app_file_t *prm, disk_size_t *sz) ;


#endif	/* _APP_FILE_H_ */



