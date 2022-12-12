#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_system.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_SYSTEM_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_SYSTEM_CONFIG, (void*)&t, sizeof t );

	printf("<system>\n");
	printf("<model>%s</model>\n", t.model);
	printf("<cert_model>%s</cert_model>\n", t.cert_model);
	printf("<fwver>%s</fwver>\n", t.fwver);
	printf("<devid>%s</devid>\n", t.devid);
	printf("<uid>%s</uid>\n",     t.uid);
	printf("<mac>%s</mac>\n",     t.mac);
	printf("</system>\n");

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
