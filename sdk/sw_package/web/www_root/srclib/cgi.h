#include "cgi_param.h"
#include "cgi_uds.h"
#include "cgi_debug.h"
#include "cgi_error.h"

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define POST 0
#define GET  1


// ACTION...
#define UPDATE 0 
#define SEARCH 1
#define SYSMNG 2
#define LOGINCHECK 3


#define SUPPORT_LANG "en"
#define MAX_ACCOUNT 16
#define LANGUAGE_PATH "/var/www/language"

#define PUT_CRLF                printf("\r\n")
#define PUT_CONTENT_TYPE_PLAIN  printf("Content-type: text/plain\r\n")
#define PUT_CONTENT_TYPE_XML    printf("Content-type: text/xml\r\n")
#define PUT_CONTENT_TYPE_HTML   printf("Content-type: text/html\r\n")
//#define PUT_CACHE_CONTROL_NOCACHE printf("Cache-Control: no-cache, no-store, must-revalidate\r\n")
#define PUT_CACHE_CONTROL_NOCACHE printf("Cache-Control: no-cache, no-store\r\n")

#define ENABLE_CONTENT_TYPE_JSON 1
#if ENABLE_CONTENT_TYPE_JSON 
#define PUT_CONTENT_TYPE_JSON 	printf("Content-type: application/json\r\n")
#else
#define PUT_CONTENT_TYPE_JSON 
#endif


#if defined(FITT360_BASIC)
#define MODEL_NAME "FITT360 Security"
#elif defined(FITT360)
#define MODEL_NAME "FITT360 Security"
#elif defined(NEXX360B)
#define MODEL_NAME "NEXX360B"
#elif defined(NEXX360W)
#define MODEL_NAME "NEXX360W"
#elif defined(NEXX360W_MUX)
#define MODEL_NAME "NEXX360W_MUX"
#elif defined(NEXXB)
#define MODEL_NAME "NEXXB"
#elif defined(NEXXB_ONE)
#define MODEL_NAME "NEXXB_ONE"
#elif defined(NEXX360M)
#define MODEL_NAME "NEXX360M"
#elif defined(NEXXONE_VOIP)
#define NEXXONE
#define MODEL_NAME "NEXXONE"
#elif defined(NEXX360C)
#define MODEL_NAME "NEXX360C"
#elif defined(NEXX360W_CCTV)
#define MODEL_NAME "NEXX360W_CCTV"
#endif



#ifdef __cplusplus
extern "C" {
#endif

int send_response(int errnum);

char *      getQueryString();
int         parseContents(char *buf, T_CGIPRM *prm);
void        wait_redirect(const char *redirect_page, int nTimeout );
void        refresh_page(const char *page, int errIndex);
char *      get_cgi_contents();

void		remove_char_from_string(char subc, char *str);
void		remove_rn_char_from_string(char *str);


#ifdef __cplusplus
}
#endif
