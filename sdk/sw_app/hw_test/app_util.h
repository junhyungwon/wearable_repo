/******************************************************************************
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_util.h
 * @brief
 */
/*****************************************************************************/

#ifndef _APP_UTIL_H_
#define _APP_UTIL_H_

/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
#if 0
#define app_malloc	util_mem_alloc
#define app_mfree	util_mem_free
#define app_memcpy	util_mem_copy
#else
#define app_malloc	malloc
#define app_mfree	free
#define app_memcpy	memcpy
#endif

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
void *util_mem_alloc(unsigned int msize);
void util_mem_free(void *ptr);
int util_mem_copy(void *pDes, void *pSrc, int size);

int util_sys_exec(char *arg);
int util_check_mount(char *dev);

int menu_get_data(void);
char menu_get_cmd(void);

#endif	/* _APP_UTIL_H_ */
