/******************************************************************************
 * UBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
  * @file	 app_log.c
  * @brief	 LOG file management with linked-list
  * @author  sksung
  * @section MODIFY history
  * 	- 2014.11.25 : First Created
  */
 /*****************************************************************************/

#ifndef _APP_LOG_H_
#define _APP_LOG_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
 -----------------------------------------------------------------------------*/
#define LOG_NAME_MAX	(40)
/*----------------------------------------------------------------------------
 Definitions and macro
 -----------------------------------------------------------------------------*/
#define USE_LOG_FILE

#define MSG_LOG_WRITE		0xA001		//# log data write
#define MSG_LOG_SHUTDOWN	0xA100		//# log process stop now!
#define MSG_LOG_EXIT		0xAFF0		//# log proc exit

/*------------------------------------------------
 String Index
 ------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
 -----------------------------------------------------------------------------*/
typedef struct {
	unsigned int size;
	char fname[LOG_NAME_MAX];
} log_info_t;

/*----------------------------------------------------------------------------
 Declares a	function prototype
 -----------------------------------------------------------------------------*/
int app_log_init(void);
void app_log_exit(void);
void app_log_write(int cmd, char* msg);

#endif	/* _APP_LOG_H_ */
