#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_setupHttps.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_HTTPS_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_HTTPS_CONFIG, (void*)&t, sizeof t );

	// 굳이 저장할 필요가 있을까?일회성으로 사용될 경우가 많을듯
	printf("<setupHttps>\n");
	printf("<https_mode>%d</https_mode>\n", t.https_mode);
	printf("<enable>%d</enable>\n", t.enable);
	printf("<port>%d</port>\n", t.port);
	printf("<C>%s</C>\n",   t.C);
	printf("<ST>%s</ST>\n", t.ST);
	printf("<L>%s</L>\n",   t.L);
	printf("<O>%s</O>\n",   t.O);
	printf("<OU>%s</OU>\n", t.OU);
	printf("<CN>%s</CN>\n", t.CN);
	printf("<Email>%s</Email>\n", t.Email);
	printf("<cert_name>%s</cert_name>\n", t.cert_name);
	printf("</setupHttps>\n");

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
