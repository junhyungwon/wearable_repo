#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <json-c/json.h>

#include "cgi.h"

char g_lang[32] = "en";
char g_out[PATH_MAX];


// @desc remove linefeed and carriage return character from given string
// @param str will be changed...
void		remove_char_from_string(char subc, char *str)
{
	char *src, *dst;

	for(src=dst=str; *src !='\0'; src++){
		*dst = *src;
		if(*dst != subc)
			dst++;
	}
	*dst = '\0';
}

// @desc remove linefeed and carriage return character from given string
// @param str will be changed...
void		remove_rn_char_from_string(char *str)
{
	char *src, *dst;

	for(src=dst=str; *src !='\0'; src++){
		*dst = *src;
		if(*dst != '\r' && *dst != '\n')
			dst++;
	}
	*dst = '\0';
}

char* GetCgiStr(char *str)
{
	FILE *fp;
	char tmpStr[PATH_MAX], buf[PATH_MAX], tmpStr2[PATH_MAX];
	int len = strlen(str), i=0;

	sprintf(tmpStr, "%s/%s.xml", LANGUAGE_PATH, g_lang);

	fp = fopen(tmpStr, "r");
	if(!fp){
		return str;
	}

	sprintf(g_out, "%s", str);
	sprintf(tmpStr2, "<%s>", str);
	while(fgets(tmpStr, PATH_MAX, fp) != NULL){
		char *p = strstr(tmpStr, tmpStr2);

		if(p != NULL){
			strcpy(buf, p+len+2);
			while(buf[i] != '<' ){
				g_out[i] = buf[i];
				i++;
			}
			g_out[i] = '\0';
			break;
		}
	}
	fclose(fp);

	return g_out;
}

static int get_error_str(int err, char* out)
{
	switch(err){
		case ERR_INVALID_ACCOUNT:
			sprintf(out, "%s", GetCgiStr("invalid_account"));
			break;
		case ERR_NOT_SUPPORT:
			sprintf(out, "%s", GetCgiStr("not_supported"));
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

char *getQueryString() {

	return getenv("QUERY_STRING");
}

static char* parseLine(char **buf)
{
	char *p = *buf;
	char *q = p;
	if(!*q)
		return NULL;
	do
	{
		if(*q=='&' || (*q=='\\' && *(q+1)=='&' ))
		{
			*q++ = 0;
			break;
		}
		else
			q++;
	} while(*q);
	*buf = q;
	return (char *)p;
}

/*
 * parsing 해도 정상적인 값이 아닐때, 
 * contents 메모리 해제를 너무 빨리하는지 check할것.
 */
int parseContents(char *buf, T_CGIPRM *prm)
{
	char *p;
	int idx=0;

	while((p=parseLine(&buf)) != NULL)
	{
		char *q = (char *)strchr(p, '=');
		if(q==NULL) continue;
		*q++ = 0;

		prm[idx].name = p;
		prm[idx].value= q;

		//printf("%s[%d]=%s\n", prm[idx].name, idx, prm[idx].value);

		idx++;
	}
	return idx;
}



void wait_redirect(const char *redirect_page, int nTimeout )
{
	char strLang[128]={"en"};

	PUT_CONTENT_TYPE_XML;
	PUT_CRLF;

	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		   "<?xml-stylesheet type=\"text/xsl\" href=\"/html/wait_redirect.xsl\"?>\n");
	printf("<waiting_info lang=\"%s\">\n", strLang);
	printf("<waiting>\n");

	printf("<redirect_page>%s</redirect_page>\n", redirect_page);
	printf("<timeout>%d</timeout>\n", nTimeout);		// ms
	printf("<top_refresh>0</top_refresh>\n");

	printf("</waiting>\n");
	printf("</waiting_info>\n");
}

void refresh_page(const char *page, int errIndex)
{
	char str[1024];

	PUT_CONTENT_TYPE_HTML;
	PUT_CRLF;

	printf("<html>\n\
			<title>Submit Page</title>\n\
			<script type=\"text/javascript\">\n\
			function go(){\n");

	if(get_error_str(errIndex, str) == TRUE){
		printf("alert(\"%s\");\n", str);
	}

	printf("location.href = '%s';\n\
			}\n\
			</script>\n\
			<body onLoad=\"go()\">\n\
			</body>\n\
			</html>\n", page);
}

/*
 * malloc 하고 return 합니다~ free 꼭 해주삼
 */
char *get_cgi_contents()
{
	char *buf=NULL, charBuf;
	int bufsize, i;

	if( getenv("CONTENT_LENGTH") == NULL)
	{
		printf("CONTENT_LENGTH !\n");
		return NULL;
	}

	/* 메세지 길이을 얻어옴 */
	bufsize = atoi((char *)getenv("CONTENT_LENGTH"));
	/* 버퍼할당 */

	buf = (char*)malloc(bufsize+1);
	if(buf == NULL) return NULL;

	memset(buf,0x00, bufsize+1);

	/* 메세지 수신 */
	for(i=0 ; i<bufsize ; ++i){
		if(fread(&charBuf, 1, 1, stdin) == 0)
			break;

		if(charBuf == '+'){
			buf[i] = ' ';
		}
		else if(charBuf == '%'){
			char tmpStr[3]={0,};
			int tmpHex;
			fread(tmpStr, 2, 1, stdin);
			tmpHex = atoi(tmpStr);
			sscanf(tmpStr, "%x", &tmpHex);
			buf[i] = tmpHex;
			bufsize -= 2;
		}
		else
			buf[i] = charBuf;
	}
	buf[bufsize] = '\0';

	return buf;
}


int send_response(int errnum)
{
	json_object *myobj = json_object_new_object();

	json_object_object_add(myobj, "return value", json_object_new_int(errnum));

	printf("Cache-Control: no cache, no store\r\n"
	       "Content-type: application/json\r\n\r\n");

	printf("%s\r\n", json_object_to_json_string(myobj));

	// free
	json_object_put(myobj);

}

