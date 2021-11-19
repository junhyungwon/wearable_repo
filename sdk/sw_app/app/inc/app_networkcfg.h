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
#define  BUFSIZE      35
#define  ADDR_SIZE    16

#pragma pack(1)
typedef struct TAG_NETWORKCFG {
    char ssid[BUFSIZE] ;
    char ap_passwd[BUFSIZE] ;
    char wlan_ipaddress[BUFSIZE] ;
    char wlan_subnet[BUFSIZE] ;
    char wlan_gateway[BUFSIZE] ;
    char eth_ipaddress[BUFSIZE] ;
    char eth_subnet[BUFSIZE] ;
    char eth_gateway[BUFSIZE] ;
    char ftp_ipaddress[BUFSIZE] ;
    unsigned short ftp_port ;
    char ftp_id[BUFSIZE] ;
    char ftp_pwd[BUFSIZE];
    char srv_ipaddress[BUFSIZE] ;
    unsigned short srv_port ;
    char deviceId[BUFSIZE] ;

}NETWORKCFG ;
#pragma pack()

void setting_txtbase(void) ;
NETWORKCFG * init_global( char *) ;
NETWORKCFG *read_cfg_file( char *) ;
//static int gettoken(FILE *);

#endif // app_networkcfg_h
