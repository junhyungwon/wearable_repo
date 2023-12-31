/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_gps.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_GPS_H_
#define _APP_GPS_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/
#include <time.h>

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
typedef struct {
    int enable;		//# 0: invalid, 1:valid

	struct tm gtm;			//# GPS time 
	
	double subsec;
	double speed;
	double lat; 				//# latitude : (+)N, (-)S
	double lot; 				//# longitude: (+)E, (-) W
	double dir; 				//# forward direction (degree)

} app_gps_meta_t;

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
int app_gps_init(void);
int app_gps_exit(void);

int app_gps_start(void);
int app_gps_stop(void);

int app_gps_get_rmc_data(app_gps_meta_t *pdata);

#endif	/* _APP_REC_H_ */
