/****************************************************************************
* fitt360 Black Box Board
* Copyright by UDWorks, Incoporated. All Rights Reserved.
*---------------------------------------------------------------------------*/
/**
* @file        app_set.c
* @brief
* @author      hwjun
* @section     MODIFY history
*/
/*****************************************************************************/

/*----------------------------------------------------------------------------
   Defines referenced     header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app_set.h"

/*----------------------------------------------------------------------------
  Declares variables
-----------------------------------------------------------------------------*/
app_set_t *app_set=NULL;
app_set_t app_sys_set;

int read_cfg(char *fileName, unsigned char *addr, unsigned int readSize, unsigned int *actualReadSize)
{
    int retVal = 0 ;
    unsigned char  *curAddr;

    unsigned int readDataSize, fileSize, chunkSize=1024*100;
    unsigned int userReadSize;

    FILE *hndlFile;

    hndlFile = fopen(fileName, "rb");

    if(hndlFile == NULL) {
        retVal = EFAIL;
        goto exit;
    }   

    curAddr = addr;
    fileSize = 0;

    userReadSize = readSize;

    while(1) 
    {
        if(userReadSize != 0) 
        { 
            if(chunkSize > userReadSize)
                chunkSize = userReadSize; 

            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize += readDataSize;
            if(chunkSize != readDataSize)
                break;          
            if(userReadSize==fileSize)
                break;          
            curAddr += chunkSize;
        }       
        else 
        {  
            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize+=readDataSize;
            if(chunkSize!=readDataSize)
            break;
            curAddr+=chunkSize;
        }
    }   
    fclose(hndlFile);

exit:
    if(retVal!=0) 
    {
        fileSize=0;
    }
    if(actualReadSize != NULL)
        *actualReadSize = fileSize;

    return retVal;
}

int get_account_open(char *name, char *passwd) 
{
    int readSize = 0, app_set_size = 0 ;
    int ret=0;

    //#--- ucx app setting param
    app_set = (app_set_t *)&app_sys_set;

    app_set_size = sizeof(app_set_t);

    if(-1 == access(CFG_FILE_MMC, 0)) 
        ret = EFAIL ;

    read_cfg(CFG_FILE_MMC, (unsigned char*)app_set, app_set_size, (unsigned int*)&readSize) ;
    if(readSize == 0 || readSize != app_set_size) 
    {
        ret = EFAIL;
    } 
    
    printf("WIS app_set->account_info.rtsp_userid = %s\n",app_set->account_info.rtsp_userid) ;
	printf("WIS app_set->account_info.rtsp_passwd = %s\n",app_set->account_info.rtsp_passwd) ;

	if(app_set->account_info.ON_OFF != 1)
    {
        ret = EFAIL;
    }
    else 
    {
        sprintf(name,"%s",app_set->account_info.rtsp_userid) ;
        sprintf(passwd,"%s",app_set->account_info.rtsp_passwd) ;
    }  

    return ret ;

    printf("done\n");

}

