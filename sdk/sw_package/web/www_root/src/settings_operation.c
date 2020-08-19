#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_operation.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_OPERATION_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_OPERATION_CONFIG, (void*)&t, sizeof t );

	/* rec interval list*/
	printf("<rec_interval_list><value>0</value><name>1</name></rec_interval_list>\n");
	printf("<rec_interval_list><value>1</value><name>5</name></rec_interval_list>\n");

	/* rec overwrite list*/
	printf("<rec_overwrite_list><value>0</value><name>OFF</name></rec_overwrite_list>\n");
	printf("<rec_overwrite_list><value>1</value><name>ON</name></rec_overwrite_list>\n");

	printf("<pre_rec>%d</pre_rec>\n", t.rec.pre_rec);				    // on or off
	printf("<auto_rec>%d</auto_rec>\n", t.rec.auto_rec);				// enable on startup
	printf("<audio_rec>%d</audio_rec>\n", t.rec.audio_rec);				// on off
	printf("<rec_interval>%d</rec_interval>\n", t.rec.interval);
	printf("<rec_overwrite>%d</rec_overwrite>\n", t.rec.overwrite);
	printf("<display_datetime>%d</display_datetime>\n", t.display_datetime);

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
