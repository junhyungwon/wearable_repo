/******************************************************************************
 * FITT Board
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
#define	FREC_TEST			(0)
#define	REC_DIR				"DCIM"
#define	FILE_MAX_PATH_LEN	128	

typedef enum {
	FILE_STATE_NORMAL	= 100, // # NO OVERWRITE
	FILE_STATE_OVERWRITE,      // # DISK FULL, USE OVERWIRTE MODE
	FILE_STATE_FULL,           // # DISK FULL, NOT USE OVERWRITE MODE
	FILE_STATE_MAX

} file_state_e ;

/*----------------------------------------------------------------------------
 Declares a	function prototype
 -----------------------------------------------------------------------------*/
int app_file_init(void);
void app_file_exit(void);
unsigned long app_file_get_free_size(void);
void app_file_update_disk_usage(void);
int app_file_add_list(const char *pathname, int size);
int get_ftp_send_file(char *path);
int restore_ftp_file(char *path);
int delete_ftp_send_file(char *path);
int get_recorded_file_count(void);
int get_write_status(void);
int app_file_check_disk_free_space(void);
int app_file_save_flist(void);

#endif	/* _APP_FILE_H_ */
