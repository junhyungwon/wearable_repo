/******************************************************************************
 * UBX Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_timesrv.c
 * @brief
 * @author  hwjun
 * @section MODIFY history
 *    - 2018.06.14 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
   Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU

#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <app_timesrv.h>

#include "dev_common.h"
#include "app_dev.h"
#include "app_main.h"
#include "app_comm.h"
#include "app_ctrl.h"
#include "ti_vcap.h"
#include "app_set.h"
#include "app_rec.h"

/*------------------------------------------------------------------------------
Defines
------------------------------------------------------------------------------*/
#define PATH_LOCALTIME  "/etc/localtime"
#define PATH_ZONE_INFO  "/usr/share/zoneinfo"
#define PATH_ZONE_LOCAL "/usr/share/zoneinfo/localtime"
#define PATH_ZONE_POSIX "/usr/share/zoneinfo/posixrules"

// -12 ~ +14
static char *TZfiles[][2] = {
	{"Etc/GMT+12", "Pacific/Kwajalein"},
	{"Etc/GMT+11", "Pacific/Midway"},
	{"Etc/GMT+10", "US/Hawaii"},
	{"Etc/GMT+9",  "US/Alaska"},
	{"Etc/GMT+8",  "PST8PDT"},
	{"MST"      ,  "MST7MDT"},
	{"Etc/GMT+6",  "US/Central"},
	{"EST",        "EST5EDT"},					/* Eastern Standard, Eastern Daylight */
	{"Etc/GMT+4",  "Canada/Atlantic"},
	{"Etc/GMT+3",  "America/Buenos_Aires"},
	{"Etc/GMT+2",  "Etc/GMT+2"},
	{"Etc/GMT+1",  "Atlantic/Azores"},
	{"Etc/GMT",    "Europe/London"},
	{"Etc/GMT-1",  "Europe/Berlin"},
	{"Etc/GMT-2",  "Europe/Athens"},
	{"Etc/GMT-3",  "Europe/Moscow"},
	{"Etc/GMT-4",  "Asia/Muscat"},
	{"Etc/GMT-5",  "Asia/Karachi"},
	{"Etc/GMT-6",  "Asia/Dhaka"},
	{"Etc/GMT-7",  "Asia/Bangkok"},
	{"Etc/GMT-8",  "Asia/Taipei"},
	{"Etc/GMT-9",  "Asia/Seoul"},
	{"Etc/GMT-10", "Australia/Brisbane"},
	{"Etc/GMT-11", "Asia/Magadan"},
	{"Etc/GMT-12", "Pacific/Fiji"},
	{"Etc/GMT-13", "Etc/GMT-13"},
	{"Etc/GMT-14", "Etc/GMT-14"}
};

typedef struct {
    app_thr_obj tsyncObj;
    int tsync_status;
    OSA_MutexHndl mutex;
} app_tsync_t;

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static app_tsync_t t_tsync;
static app_tsync_t *itsync=&t_tsync;

static const unsigned char rtc_days_in_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int is_leap_year(unsigned int year)
{
	return (!(year % 4) && (year % 100)) || !(year % 400);
}

static int __rtc_month_days(unsigned int month, unsigned int year)
{
	return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}

static int __rtc_valid_tm(struct tm *ptm)
{
	/* 2000년도 아래이거나 간헐적으로 2066년이 출력되는 경우 RTC가 Reset된 경우임 */
	if (ptm->tm_year < 100
		|| ptm->tm_year > 165
		|| ((unsigned)ptm->tm_mon) >= 12
		|| ptm->tm_mday < 1
		|| ptm->tm_mday > __rtc_month_days(ptm->tm_mon, ptm->tm_year + 1900)
		|| ((unsigned)ptm->tm_hour) >= 24
		|| ((unsigned)ptm->tm_min) >= 60
		|| ((unsigned)ptm->tm_sec) >= 60) {
		
		dprintf("Invalid tm: --- %d-%d-%d %02d:%02d:%02d\n",
				ptm->tm_year, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		return -1;
	}

	return 0;
}

static int update_m3_time()
{
	time_t now;
	struct tm *pgm;
	int retval = FALSE;

//	dprintf("--- update m3 time ---\n");

	// time(), gmtime(), it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 (UTC)
	time(&now);
   	pgm = gmtime(&now);
	if (__rtc_valid_tm(pgm) < 0) {
		/* invalid rtc time --> set 2020/01/01 00-00-00 */
		pgm->tm_year = 120; /* 1900+120=2020*/
		pgm->tm_mon  = 0;
		pgm->tm_mday = 1;
		pgm->tm_hour = 0;
		pgm->tm_min  = 0;
		pgm->tm_sec  = 0;
	}
	
	dprintf("m3 time: %d-%d-%d %d:%d:%d\n", pgm->tm_year + 1900, pgm->tm_mon + 1,
				pgm->tm_mday, pgm->tm_hour, pgm->tm_min, pgm->tm_sec);
	if (dev_rtc_set_time(*pgm) < 0) {
		dprintf("Failed to set system time to rtc\n!!!");
	} else {
		char buff[MAX_CHAR_128]={0};
		sprintf(buff, "/opt/fit/bin/tz_set &") ;  // it needs file create time sync with windows browser
		system(buff) ;
		retval = TRUE;

		app_msleep(100);
		Vsys_datetime_init2(app_set->time_info.daylight_saving);   //# m3 Date/Time init
	}

	return retval;
}

/*
 * The parameter timezone ranges from -12 to +14
 */
char *get_timezone (int timezone, int daylightsaving)
{
	if(daylightsaving<0 || daylightsaving > 1) {
		dprintf("Please, check for daylightsaving(%d). It must be 0 or 1", daylightsaving);
		daylightsaving = 0;
	}

	int tz_idx = timezone + 12;
    static char buffer[128];
    memset (&buffer, 0, sizeof(buffer));

    sprintf (buffer, "%s", TZfiles[tz_idx][daylightsaving]);
    return buffer ;
}

int set_time_zone()
{
    FILE *Fp ;
    char buf[120] ;
    int retval = 0 ;

	int timezone = app_set->time_info.time_zone - 12; 
    int daylight  = app_set->time_info.daylight_saving;  //# 0:false, 1:automatically for daylight saving time changes.
    
	sprintf(buf,"rm -f /etc/adjtime;rm -f %s; ln -s %s/%s %s",
			PATH_LOCALTIME,
			PATH_ZONE_INFO,
			get_timezone(timezone, daylight),
			PATH_LOCALTIME) ;
    Fp = popen(buf, "w") ;
    if(Fp != NULL)
		pclose(Fp) ;
    else {
		dprintf("ERROR setting Timezone \n") ;
        return retval ;
	}

	// No guaranteed completion PIPE???
	update_m3_time();
	dprintf("....done!!\n");

	return 1 ;
}

int set_time_manual(int year, int month, int day, int hour, int minute, int second)
{
	int retval = FALSE;
	struct tm stm;
	time_t set;

	dprintf("--- changed time by manual ---\n");
	stm.tm_year = year  - 1900; 
	stm.tm_mon  = month - 1;
	stm.tm_mday = day ;
	stm.tm_hour = hour;
	stm.tm_min  = minute;
	stm.tm_sec  = second;

	set = mktime(&stm);
	stime(&set);

	retval = update_m3_time();

	return retval;
}

/*
 datetime's format is "YYYY-MM-DD hh:mm:dd" 19 digit
 */
int set_time_manual_bystring(char *datetime)
{
	int year, month, day, hh, mm, ss;
	sscanf(datetime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hh, &mm, &ss);

	return set_time_manual(year, month, day, hh, mm, ss);
}

int check_ipaddress(char *addr)
{
	int len = 0, i = 0, dotcnt = 0, numcnt = 0 ;

    if(strcmp(addr, " ") == 0)
	{
		return FALSE ;
	}

    len = strlen(addr) ;

    for(i = 0 ; i < len; i++)
	{
		if(addr[i] < '0' || addr[i] > '9')
		{
			if(addr[i] == '.')
			{
				++dotcnt ;
				numcnt = 0 ;
			}
		}
		else
		{
			if(++numcnt > 3)
				return FALSE ;
		}
	}
    if(dotcnt != 3)
		return FALSE ;
	
	return TRUE ;
}

int set_time_by_ntp()
{
	int retval = FALSE, cnt=0;
    struct hostent *hp;
    char buff[MAX_CHAR_128], server_addr[MAX_CHAR_32] ;
	FILE *fd;

    if(check_ipaddress(app_set->time_info.time_server))
	{
		strncpy(server_addr, app_set->time_info.time_server, strlen(app_set->time_info.time_server));
	}
	else
	{	
        hp = gethostbyname(app_set->time_info.time_server) ;
        if (!hp) {
		    perror("hostent is NULL:");
            return FALSE;
        }

		strncpy(server_addr, inet_ntoa(*((struct in_addr *)hp->h_addr_list[0])), strlen(inet_ntoa(*((struct in_addr *)hp->h_addr_list[0]))));
	}
	
	/* SD 카드에 error 발생 시 readonly로 변경됨. ntp.txt 저장 안 됨.*/
	if (ctrl_mmc_check_writable() > 0) {
    	sprintf(buff,"/opt/fit/bin/ntpclient -h %s -d -s -i 4 > /mmc/ntp.txt",server_addr) ;
    	system(buff) ;
	} else {
		return FALSE;
	}
	
    while(1)
    {
        if(access("/mmc/ntp.txt", F_OK) != 0)
        {
            if(cnt == 5)
                break ;
            cnt += 1 ;
        }
        else
        {
            break ;
        }
        OSA_waitMsecs(1000);
    }
    if((fd = fopen("/mmc/ntp.txt", "r")) != NULL)
    {
        int readsize = fread(buff, sizeof buff, 1,  fd) ;
		if (readsize < 0) {
			; /* TODO */
		}

        if(strstr(buff, "Configuration"))
        {
            retval = TRUE ;
            fclose(fd) ;
            remove("/mmc/ntp.txt") ;
        }
        else
        {
            fclose(fd) ;
            remove("/mmc/ntp.txt") ; // vacant file..
        }
    }

	if(retval){
		dprintf("succeed set time ntp\n");
		update_m3_time();
	}else {
		dprintf("failed set time ntp\n");
	}

	return retval;
}

int gettime_from_ntp(char *server_addr)
{
    FILE *fd;
    int  readsize = 0 ;
    char buff[MAX_CHAR_128] ;
    char buffer[1024] ;

    int retval = -1, cnt = 0 ;

    if(set_time_zone())  // temp
    {
		//# 0:computer(same as manual), 1:syncronize with NTP server
		if(app_set->time_info.timesync_type != TIMESYNC_NTP) {
			// -1 로 리턴하면 시간이 2020년으로 설정됨.
			return FALSE; // 0
		}
		
		/* SD 카드에 error 발생 시 readonly로 변경됨. ntp.txt 저장 안 됨.*/
		if (ctrl_mmc_check_writable() > 0) {
        	sprintf(buff,"/opt/fit/bin/ntpclient -h %s -d -s -i 4 > /mmc/ntp.txt",server_addr) ;
			system(buff) ;
		} else {
			return FALSE;
		}
    }

    while(1)
    {
        if(access("/mmc/ntp.txt", F_OK) != 0)
        {
            if(cnt == 5)
                break ;
            cnt += 1 ;
        }
        else
        {
            break ;
        }
        OSA_waitMsecs(100);
    }
    if((fd = fopen("/mmc/ntp.txt", "r")) != NULL)
    {
        readsize = fread(buffer, 754, 1,  fd) ;
		if(readsize > 0)
		{
            if(strstr(buffer, "Configuration"))
            {
				if(!strstr(buffer, "rejected"))
				{
                    retval = TRUE ;
				}
                fclose(fd) ;
                remove("/mmc/ntp.txt") ;
            }
            else
            {
                fclose(fd) ;
                remove("/mmc/ntp.txt") ;
            }
		}
		else
		{
            fclose(fd) ;
            remove("/mmc/ntp.txt") ;
		}
    }

    return retval;
}

static int time_sync(void)
{
    int retval = FALSE;
    char buff[16], timesrv_addr[32] = {0, } ;
    time_t timeval, time_val, set ;
    struct tm tp, tv;
    struct hostent *hp;
	
	//dprintf("Enter time sync!...\n");
    if(check_ipaddress(app_set->time_info.time_server)) {
		strncpy(timesrv_addr, app_set->time_info.time_server, strlen(app_set->time_info.time_server));
	}
	else {
        hp = gethostbyname(app_set->time_info.time_server);
        if (!hp) {
		    perror("gethostbyname:");
            return FALSE;
        }
		strncpy(timesrv_addr, inet_ntoa(*((struct in_addr *)hp->h_addr_list[0])), strlen(inet_ntoa(*((struct in_addr *)hp->h_addr_list[0]))));
    }

    if(gettime_from_ntp(timesrv_addr) == TRUE)
    {
        if(app_rec_state()) {
			app_rec_stop(ON);
            app_cfg->ste.b.prerec_state = 1 ;
		}
		/* wait for record stop..*/
        app_msleep(100);
        Vsys_datetime_init2(app_set->time_info.daylight_saving);   //# m3 Date/Time init

        time(&timeval) ;
		//localtime_r(&timeval, &tp);
		gmtime_r(&timeval, &tp);
		dprintf("ntp time: %d-%d-%d %d:%d:%d\n", tp.tm_year + 1900, tp.tm_mon + 1,
				tp.tm_mday, tp.tm_hour, tp.tm_min, tp.tm_sec);
		if (dev_rtc_set_time(tp) < 0) {
            dprintf("Failed to set system time to rtc\n!!!");
        } else {
            dprintf("--- changed time from Time server ---\n");
            sprintf(buff, "/opt/fit/bin/tz_set &") ;  // it needs file create time sync with windows browser
            system(buff) ;
            retval = TRUE;
        }

    } else if(gettime_from_ntp(timesrv_addr) == FALSE) {  // TIME SYNC 사용 안할때 , 더이상 시도 하지 않도록 
		retval = TRUE ;
	}
    else if(gettime_from_ntp(timesrv_addr) == -1)  // TIME SYNC 실패시 
    {
		//dprintf("%s, %d\n", __func__, __LINE__);
        time(&time_val) ;
        localtime_r(&time_val, &tv);
		if (__rtc_valid_tm(&tv) < 0) {
			/* invalid rtc time --> set 2020/01/01 00-00-00 */
			tv.tm_year = 120; /* 1900+120=2020*/
			tv.tm_mon  = 0;
			tv.tm_mday = 1;
			tv.tm_hour = 0;
			tv.tm_min  = 0;
			tv.tm_sec  = 0;
			
			dprintf("Failed sync: %d-%d-%d %d:%d:%d\n", tp.tm_year + 1900, tp.tm_mon + 1,
				tp.tm_mday, tp.tm_hour, tp.tm_min, tp.tm_sec);
					
			set = mktime(&tv);
            stime(&set) ;
            Vsys_datetime_init();   //# m3 Date/Time init
            app_msleep(100);
            if (dev_rtc_set_time(tv) < 0) {
                dprintf("Failed to set system time to rtc\n!!!");
            }
            else
                dprintf("--- changed time default 2000 ---\n");
		}
		retval = FALSE ;
    }

	if(app_cfg->ste.b.prerec_state && !app_rec_state() && !app_cfg->ste.b.ftp_run)
	{
		app_rec_start() ;
		app_cfg->ste.b.prerec_state = 0 ;
	}

    return retval;
}

/*****************************************************************************
* @brief    meta main function
* @section  DESC Description
*   - desc
*****************************************************************************/
static void *THR_tsync(void *prm)
{
    app_thr_obj *tObj = &itsync->tsyncObj;
    int cmd, retry_cnt = 0;
    int exit = FALSE, retval = FALSE;

    dprintf("enter...\n");
    itsync->tsync_status = TIMESYNC_READY;
    tObj->active = 1;

    set_time_zone() ;  

    while (!exit)
    {
        cmd = tObj->cmd;
        if (cmd == APP_CMD_STOP)  {
            break;
        }

		if(app_cfg->ste.b.cradle_net_run || app_cfg->ste.b.usbnet_run) 
		{
            if (itsync->tsync_status == TIMESYNC_READY)
			{ 
                retval = time_sync() ;
			}
        } 
		
		if(!app_cfg->ste.b.cradle_net_ready && !app_cfg->ste.b.usbnet_ready)
		{
            itsync->tsync_status = TIMESYNC_READY ;
		}

        if(retval) {
            itsync->tsync_status = TIMESYNC_DONE ;
            retval = FALSE ;
        } else {
			if (retry_cnt == 59) // retry per 5 sec (60 x 5)  5Min retry
			{
                itsync->tsync_status = TIMESYNC_DONE ;
				retry_cnt = 0;
            } else
			    retry_cnt++ ;
        }

        OSA_waitMsecs(1000) ;
    }

    tObj->active = 0;
    dprintf("...exit\n");

    return NULL;
}

/*****************************************************************************
* @brief    tsync thread init/exit function
* @section  DESC Description
*   - desc
*****************************************************************************/
int app_tsync_init(void)
{
    app_thr_obj *tObj ;

    memset(itsync, 0x0, sizeof(app_tsync_t)) ;

    //# create tsync thread ;

    tObj = &itsync->tsyncObj ;
    if(thread_create(tObj, THR_tsync, APP_THREAD_PRI, NULL, __FILENAME__) < 0)
    {
        dprintf("create tsync thread\n") ;
        return EFAIL ;
    }
    dprintf("... done!\n");
    
	return 0 ;
}

void app_tsync_exit(void)
{
    app_thr_obj *tObj ;

    tObj = &itsync->tsyncObj ;
    event_send(tObj, APP_CMD_STOP, 0, 0) ;
    while(tObj->active)
        OSA_waitMsecs(20);

    thread_delete(tObj);

    dprintf("... done!\n");
}

