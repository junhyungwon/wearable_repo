#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

//# remove warning: 'struct mmsghdr' declared inside parameter list
#define __USE_GNU
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <net/if.h>
#include "app_set.h"
#include "app_networkcfg.h"

NETWORKCFG Networkcfg ;

static unsigned int lineno;
static int lexval;
static char lexbuf[MAXCFG_LENGTH];

static int gettoken(FILE *fp)
{
    int ch;

    while(((ch = getc(fp))) != EOF )
    {
        if (isspace(ch))
        	continue;

        if (ch == '\n') {
            lineno++;
            return KEY_NEWLINE;
        }

        if (ch == '#') {
        	fgets(lexbuf, MAXCFG_LENGTH, fp);
        	return KEY_NEWLINE;
        }

        if (ch == ':') {
            fgets(lexbuf, BUFSIZE, fp);
            return KEY_STRING;
        }

        if (isdigit(ch)) {
            ungetc(ch, fp);
            fscanf(fp, "%i", &lexval);
            return KEY_NUMERIC;
        }

        if (isalpha(ch) || ch == '_')
        {
            ungetc(ch, fp);
            fscanf(fp, "%[a-zA-Z0-9_]", lexbuf);

            if (!strcasecmp(lexbuf, "y") || !strcasecmp( lexbuf, "yes")
                                || !strcasecmp(lexbuf, "on"))
            {
                lexval = 1;
                return KEY_BOOLEAN;
            }

            if (!strcasecmp( lexbuf, "n" ) || !strcasecmp( lexbuf, "no")
                                        || !strcasecmp( lexbuf, "off"))

            {
                lexval = 0;
                return  KEY_BOOLEAN;
            }

            return  KEY_STRING;
         }

         if(ch =='-')
         {
             ungetc(ch, fp);
             fscanf(fp, "%i", &lexval);
             return KEY_NUMERIC;
         }

         if( ch == '"')
         {
            register int                i = 0;
            while(( ch = fgetc(fp)) !=EOF && ch != '\n' )
            {
                if( ch == '"' )
                {
                    lexbuf[i] = '\0';
                    return              KEY_ENCLOSED;
                }
                if( ch == '\\')
                {
                    lexbuf[i++] = (int) fgets( lexbuf, MAXCFG_LENGTH, fp );
                }
                else
                lexbuf[i++] = ch;
            }
                return KEY_INVALID;
        }

        return ch;
    }

    return KEY_EOF;
}

void setting_txtbase(void)
{
    NETWORKCFG *Networkcfg;
    char cfg_file[1024];

    strcpy(cfg_file, "/mmc/cfg/");
    strcat(cfg_file, "settings");

    Networkcfg = init_global(cfg_file);

    return ;
}

NETWORKCFG *init_global(char *cfgfile)
{
    NETWORKCFG *Networkcfg;

    Networkcfg = read_cfg_file(cfgfile);

    return Networkcfg;
}

NETWORKCFG *read_cfg_file(char *filename)
{
    FILE *fp = NULL;
    int token, retval = S_OK, intval = 0, i = 0, j = 0, k = 0;
//    int Fps = 0, Bps = 0, Period = 0, Overwrite = 1 ;
    char *key, *strval = NULL;
#if 0	
    char ssid[BUFSIZE] = {0, } ;
    char ap_passwd[BUFSIZE + 30] = {0, };
    char wlan_ipaddress[BUFSIZE] = {0, };
    char wlan_gateway[BUFSIZE] = {0, };
    char wlan_subnet[BUFSIZE] = {0, };

    char eth_ipaddress[BUFSIZE] = {0, };
    char eth_gateway[BUFSIZE] = {0, };
    char eth_subnet[BUFSIZE] = {0, };

    char ftp_ipaddress[BUFSIZE] = {0, };
    char ftp_id[BUFSIZE] = {0, };
    char ftp_pwd[BUFSIZE] = {0, };
	
    char srv_ipaddress[BUFSIZE] = {0, } ;
    char deviceId[BUFSIZE] = {0, } ;
#endif
    char MacAddr[12] ;
 
    memset(&Networkcfg, 0, sizeof(NETWORKCFG)) ;

    if (filename == NULL) {
        retval = S_ERR;
    }

    if ((fp = fopen(filename, "r")) == NULL)
    {
/*
        if((fd = fopen(filename,"w")) == NULL)
        {
            TRACE_INFO("fitt360_settings file open error\n") ;
        }
        else
        {
            sprintf(ssid,"%s","ollehEgg_488") ;
            sprintf(ap_passwd, "%s", "info00788") ;
            sprintf(wlan_ipaddress, "%s", "192.168.0.252") ;
            sprintf(wlan_subnet, "%s","255.255.255.0") ;
            sprintf(wlan_gateway, "%s","192.168.0.1") ;

            sprintf(eth_ipaddress, "%s", "192.168.1.252") ;
            sprintf(eth_subnet, "%s","255.255.255.0") ;
            sprintf(eth_gateway, "%s","192.168.1.1") ;

            sprintf(ftp_ipaddress, "%s", "192.168.1.23") ;
            sprintf(ftp_id, "%s","test") ;
            sprintf(ftp_pwd, "%s","test") ;

            sprintf(srv_ipaddress, "%s", "192.168.1.24") ;
            sprintf(deviceId, "%s","_MACADDRESS_") ;

            fprintf(fd, "SSID=\"%s\"\n",ssid) ;
            fprintf(fd, "APPASSWD=\"%s\"\n",ap_passwd) ;
            fprintf(fd, "WLAN_IPADDR=\"%s\"\n",wlan_ipaddress) ;
            fprintf(fd, "WLAN_SUBNET=\"%s\"\n",wlan_subnet) ;
            fprintf(fd, "WLAN_GATEWAY=\"%s\"\n",wlan_gateway) ;

            fprintf(fd, "ETH_IPADDR=\"%s\"\n",eth_ipaddress) ;
            fprintf(fd, "ETH_SUBNET=\"%s\"\n",eth_subnet) ;
            fprintf(fd, "ETH_GATEWAY=\"%s\"\n",eth_gateway) ;

            fprintf(fd, "FTP_IPADDR=\"%s\"\n",ftp_ipaddress) ;
            fprintf(fd, "FTP_PORT=%d\n",ftp_port) ;
            fprintf(fd, "FTP_ID=\"%s\"\n",ftp_id) ;
            fprintf(fd, "FTP_PASSWD=\"%s\"\n",ftp_pwd) ;

            fprintf(fd, "SRV_IPADDR=\"%s\"\n",srv_ipaddress) ;
            fprintf(fd, "SRV_PORT=%d\n",srv_port) ;
            fprintf(fd, "DeviceId=\"%s\"\n",deviceId) ;

            fprintf(fd, "REC_FRAMERATE=%d\n",Fps) ;
            fprintf(fd, "REC_BITRATE=%d\n",Bps) ;
            fprintf(fd, "LIVE_FRAMERATE=%d\n",Fps) ;
            fprintf(fd, "LIVE_BITRATE=%d\n",Bps) ;

            fprintf(fd, "REC_PERIOD=%d\n",Period) ;
            fprintf(fd, "OVER_WRITE=%d\n",Overwrite) ;


            fclose(fd);

            strcpy(Networkcfg.ssid, ssid) ;
            strcpy(Networkcfg.ap_passwd, ap_passwd) ;
            strcpy(Networkcfg.wlan_ipaddress, wlan_ipaddress) ;
            strcpy(Networkcfg.wlan_subnet, wlan_subnet ) ;
            strcpy(Networkcfg.wlan_gateway, wlan_gateway) ;
            strcpy(Networkcfg.eth_ipaddress, eth_ipaddress) ;
            strcpy(Networkcfg.eth_subnet, eth_subnet ) ;
            strcpy(Networkcfg.eth_gateway, eth_gateway) ;
            strcpy(Networkcfg.ftp_ipaddress, ftp_ipaddress) ;
            Networkcfg.ftp_port = ftp_port ;
            strcpy(Networkcfg.ftp_id, ftp_id ) ;
            strcpy(Networkcfg.ftp_pwd, ftp_pwd) ;
            strcpy(Networkcfg.srv_ipaddress, srv_ipaddress ) ;
            Networkcfg.srv_port = srv_port ;
            strcpy(Networkcfg.deviceId, deviceId ) ;

        }
*/
    }
    else
    {
        for(;;)
        {
       	    token = gettoken(fp);

            if (token == KEY_NEWLINE ) continue;

            if (token == KEY_EOF)
        		break;
            if (token != KEY_STRING)
            {
              	retval = S_ERR;
            	continue;
            }

            key = strdup(lexbuf);

            if (gettoken(fp) != '=')
            {
            	break;
            }

            token = gettoken(fp);

            switch(token)
            {
                case  KEY_STRING:
//                      strcpy(&Buf[j][i], lexbuf) ;
                        i++ ;
                        if( i == 21  )
                        {
                            i = 0;
                            j++ ;
                        }
                        break ;

                case  KEY_ENCLOSED:
                        strval = strdup(lexbuf);

                        if(i == 0)
                        {
                            if(strlen(strval) <= MAX_CHAR_32)
                                sprintf(app_set->wifiap.ssid, "%s",strval) ;
                        }
                        if(i == 1)
                        {
                            if(strlen(strval) <= MAX_CHAR_64)
                                sprintf(app_set->wifiap.pwd, "%s",strval) ;
                        }
                        if(i == 2)
                        {
                            sprintf(app_set->net_info.wlan_ipaddr,"%s", strval) ;
                        }
                        if(i == 3)
                        {
                            sprintf(app_set->net_info.wlan_netmask, "%s", strval) ;
                        }
                        if(i == 4)
                        {
                            sprintf(app_set->net_info.wlan_gateway, "%s", strval) ;
                        }
                        if(i == 5)
                        {
                            sprintf(app_set->net_info.eth_ipaddr, "%s", strval) ;
                        }
                        if(i == 6)
                        {
                            sprintf(app_set->net_info.eth_netmask, "%s", strval) ;
                        }
                        if(i == 7)
                        {
                            sprintf(app_set->net_info.eth_gateway, "%s", strval) ;
                        }
                        if(i == 8)
                        {
                            sprintf(app_set->ftp_info.ipaddr, "%s", strval) ;
                        }
                        if(i == 10)
                        {
                            sprintf(app_set->ftp_info.id, "%s", strval) ;
                        }
                        if(i == 11)
                        {
                            sprintf(app_set->ftp_info.pwd, "%s", strval) ;
                        }
                        if(i == 12)
                        {
                            sprintf(app_set->srv_info.ipaddr, "%s", strval) ;
                        }
                        if(i == 14)
                        {
                            if(strcmp(strval, "_MACADDRESS_")== 0)
                            {
                                if(!DefaultGetMac(MacAddr))
                                {
                                    strncpy(app_set->sys_info.deviceId ,MacAddr, 12);
                                }
                                else
                                {
                                    TRACE_INFO( "Fatal error: Failed to get local host's MAC address\n" );
                                }
                            }
                            else
                                sprintf(app_set->sys_info.deviceId, "%s", strval) ;

                            TRACE_INFO("app_set->sys_info.deviceId = %s\n",app_set->sys_info.deviceId) ;
                        }

                        i++ ;
                        if(i == 21)
                        {
                            i = 0;
                            j++ ;
                        }
                        if(strval)
                            free(strval) ;
                        token  = KEY_STRING;
                        break;

                case  KEY_NUMERIC:
                        if(i == 9)
                        {
                            app_set->ftp_info.port = lexval ;
                        }
                        if(i == 13)
                        {
                            app_set->srv_info.port = lexval ;
                        }
                        if(i == 15)
                        {
                            for(k = 0; k < MODEL_CH_NUM; k++)
                            {
                                app_set->ch[k].framerate = lexval ;
                            }
                        }
                        if(i == 16)
                        {
                            for(k = 0; k < MODEL_CH_NUM; k++)
                            {
                                app_set->ch[k].quality = lexval ;
                            }
                        }   
                        if(i == 17)
                        {
                            app_set->ch[k].framerate = lexval ;
                        }
                        if(i == 18)
                        {
                            app_set->ch[k].quality = lexval ;
                        }
                        if(i == 19)
                        {
                            app_set->rec_info.period_idx = lexval ;
                        }                        
                        if(i == 20)
                        {
                            app_set->rec_info.overwrite = lexval ;
                        }                        


                        i++ ;
                        if( i == 21 )
                        {
                            i = 0;
                            j++;
                        }
                        break ;

                case  KEY_BOOLEAN:
                    intval = lexval;
                    break;

                default:
                	retval = S_ERR;
                    continue;
	        }
        }
        fclose(fp);
    }

    return &Networkcfg;
}
