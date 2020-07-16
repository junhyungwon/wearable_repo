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
} file_state_e ;

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
unsigned long app_file_get_free_size(void);

#endif	/* _APP_FILE_H_ */
