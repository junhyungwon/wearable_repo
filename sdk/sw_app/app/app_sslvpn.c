#include <stdio.h>
#include <unistd.h>

#include "app_sslvpn.h"
#include "app_set.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/

char *protocol_type[2] = {"tcp", "udp"} ;
char *enc_type[7] = {"aes128", "aes256", "arial128", "arial256", "lea128", "lea256", "seed"} ;
char *ni_type[4] = {"eth0", "wlan0", "usb0", "eth1"} ;

void app_set_sslvpnconf(void)
{
   	FILE *fp = NULL ;
   	char buffer[64]= {0,};
	
	fp = fopen("/etc/sslvpn_config.conf","w") ;
	if (fp != NULL)
	{
		sprintf(buffer, "SSLVPN_HOME=%s\n", SSLVPN_CONF_PATH) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);
		
		sprintf(buffer, "SSLVPN_ID=%s\n", app_set->sslvpn_info.vpn_id) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);
		
		sprintf(buffer, "\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "interface tap0\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap version 3\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);

		sprintf(buffer, "	tap heartbeat interval %d threshold %d\n",app_set->sslvpn_info.heartbeat_interval, app_set->sslvpn_info.heartbeat_threshold) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap proto %s port %d queue %d\n", protocol_type[app_set->sslvpn_info.protocol], app_set->sslvpn_info.port, app_set->sslvpn_info.queue) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap key %s\n",app_set->sslvpn_info.key) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap algorithm %s\n",enc_type[app_set->sslvpn_info.encrypt_type]) ;		
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap destination %s\n", app_set->sslvpn_info.ipaddr) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	tap source %s\n", ni_type[app_set->sslvpn_info.NI]) ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "	no shutdown\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);
//		sysprint(buffer);

		sprintf(buffer, "!\n") ;
		fwrite(buffer, strlen(buffer), 1, fp);

		fclose(fp);
	}
}

int app_sslvpn_stop()
{
	char *cmd = "killall -9 sslvpnd";
#if USE_POPEN
	FILE *fp = popen(cmd, "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd);
#endif

	return 0;
}

int app_sslvpn_start()
{
	char *cmd = "/var/vpn/sslvpnd &";
#if USE_POPEN
	FILE *fp = popen(cmd, "w");
	if(fp == NULL) return -1;
	pclose(fp);
#else
	system(cmd);
#endif
	
	return 0;
}


