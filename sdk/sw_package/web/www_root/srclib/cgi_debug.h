#ifndef __CGI_DEBUG_H__
#define __CGI_DEBUG_H__
////////////////////////////////////////////////////////////////////////////////

#include <time.h>

#define CGI_DEBUG_ENABLE 1
#if CGI_DEBUG_ENABLE
long dbg_prev_sec, dbg_curr_sec;
#define CGI_DBG(fmt, args...) \
	{\
		FILE *fp;\
		char temp[5*1024], tmp22[1024];\
		dbg_curr_sec = time(NULL); \
		sprintf(tmp22, fmt, ##args);\
		sprintf(temp, "[%s-%s-%d-%ld] %s", __FILE__, __func__, __LINE__ , (dbg_curr_sec-dbg_prev_sec), tmp22); \
		fp = fopen("/tmp/cgi_debug.txt", "a");\
		fwrite(temp, 1, strlen(temp), fp);\
		fclose(fp);\
		dbg_prev_sec = dbg_curr_sec;\
	}
#else
#define CGI_DBG(fmt, args...) {}
#endif

////////////////////////////////////////////////////////////////////////////////
#endif//__CGI_DEBUG_H__
