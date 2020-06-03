#ifndef __TIME_CONTROL_H__
#define __TIME_CONTROL_H__

#include <time.h>

enum
{
	Asia_Seoul=0,
	International_DateLineWest,
	Samoa,
	Hawaii,
	Alaska,
	PacificTime_US_Canada,
	MountainTime_Us_Canada,
	Chihuahua,
	AriZona,
	CentralTime_US_Canada,
	Saskatchewan,
	Monterrey,
	CentralAmerica,
	EasternTime_US_Canada,
	Indiana_East,
	BogotaLimaQuito,
	AtlanticTime_Canada,
	LaPaz,
	Santiago,
	Newfoundland,
	Brasila,
	Buenos_Aires,
	Greenland,
	MidAtlantic,
	Azores,
	CapeVerdeIs,
	Lisbon,
	Casablanca,
	Prague,
	Sarajevo,			
	Paris,	
	Amsterdam,
	WestCentralAfrica,								
	Bucharest,										
	Cairo,											
	Sofia,			
	AthensIstanbul,							
	Jerusalem,											
	Harare,										
	KuwaitRiyadh,										
	Nairobi,												
	Baghdad,												
	Tehran,									
	Moscow, ////////////////////////////
	AbuDhabiMuscat,										
	Baku,									
	Kabul,													
	Tashkent,							
	Kolkata,					
	SriJayawardenepura,		
	Kathmandu,		////
	AstanaDhaka,	////
	Ekaterinburg,	////
	Almaty,									
	Rangoon,												
	Bangkok,								
	Krasnoyarsk,											
	ChongqingUrumqi,								
	HongKong,											
	Singapore,								
	Taipei,													
	Perth,													
	Irkutsk,												
	Seoul,													
	Tokyo,									
	Darwin,													
	Adelaide,												
	Yakutsk, //////												
	Sydney,							
	Brisbane,												
	Hobart,													
	GuamPortMoresby,									
	Vladivostok,	////										
	Magadan,											
	Fiji,							
	AucklandWellington,								
	Nukualofa,
	MAX_ZONE
};

int     set_time_zone();
int     set_time_by_ntp();
int     set_time_manual_bystring(char *datetime); // MUST Be 19digit, 1999-10-31 12:34:56 yyyy-MM-DD hh:mm:ss
int     set_time_manual(int y, int m, int d, int hh, int mm, int ss); // example (1999, 10, 31, 12, 34, 56)

int gettime_from_ntp(char *server_addr);
int app_tsync_init(void) ;
void app_tsync_exit(void) ;

#define TSERVER_NAME "time.google.com"
#define TIMESYNC_READY    0
#define TIMESYNC_DONE     1

#define TIMESYNC_NTP    1
#define TIMESYNC_MANUAL 0

//#define TSERVER_NAME "216.239.35.12"



#endif // __TIME_CONTROL_H__
