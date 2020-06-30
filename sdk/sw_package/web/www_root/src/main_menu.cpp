#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/main_menu.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
