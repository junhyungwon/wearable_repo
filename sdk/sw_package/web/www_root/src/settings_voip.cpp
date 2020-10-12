#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_voip.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_VOIP_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_VOIP_CONFIG, (void*)&t, sizeof t );

	printf("<voip>\n");
	printf("<model>%s</model>\n", MODEL_NAME);
	printf("<private_network_only>%s</private_network_only>\n", t.private_network_only);
	printf("<ip>%s</ip>\n", t.ipaddr);
	printf("<port>%d</port>\n", t.port);
	printf("<id>%s</id>\n",     t.userid);
	printf("<pw>%s</pw>\n",     t.passwd);
	printf("<peerid>%s</peerid>\n", t.peerid);
	printf("</voip>\n");

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
