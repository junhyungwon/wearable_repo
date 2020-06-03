// app_networkcfg.h

#ifndef app_networkcfg_h
#define app_networkcfg_h

#define  MAXCFG_LENGTH   1024
#define  S_OK     1
#define  S_ERR    0
#define  CREATE   malloc
#define  KEY_NUMERIC    256
#define  KEY_STRING     257
#define  KEY_ENCLOSED   258
#define  KEY_BOOLEAN    259
#define  KEY_NEWLINE    ('\n')
#define  KEY_EOF        (EOF)
#define  KEY_INVALID    260
#define  TITLESIZE      35
#define  ADDR_SIZE    16

#pragma pack(1)
typedef struct TAG_NETWORKCFG {
    char ssid[TITLESIZE] ;
    char ap_passwd[TITLESIZE] ;
    char wlan_ipaddress[TITLESIZE] ;
    char wlan_subnet[TITLESIZE] ;
    char wlan_gateway[TITLESIZE] ;
    char eth_ipaddress[TITLESIZE] ;
    char eth_subnet[TITLESIZE] ;
    char eth_gateway[TITLESIZE] ;
    char ftp_ipaddress[TITLESIZE] ;
    unsigned short ftp_port ;
    char ftp_id[TITLESIZE] ;
    char ftp_pwd[TITLESIZE];
    char srv_ipaddress[TITLESIZE] ;
    unsigned short srv_port ;
    char deviceId[TITLESIZE] ;

}NETWORKCFG ;
#pragma pack()

void setting_txtbase(void) ;
NETWORKCFG * init_global( char *) ;
NETWORKCFG *read_cfg_file( char *) ;
//static int gettoken(FILE *);

#endif // app_networkcfg_h
