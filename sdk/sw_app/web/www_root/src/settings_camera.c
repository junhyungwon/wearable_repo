#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_camera.xsl"
//#define HREF_PAGE "/html/change_password.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
#if 1
	T_CGI_VIDEO_QUALITY q;memset(&q,0, sizeof q);
	sysctl_message(UDS_GET_VIDEO_QUALITY, (void*)&q, sizeof(q));

	/* fps list*/
	printf("<fps_list><value>0</value><name>HIGH</name></fps_list>\n");
	printf("<fps_list><value>1</value><name>MEDIUM</name></fps_list>\n");
	printf("<fps_list><value>2</value><name>LOW</name></fps_list>\n");

	/* bps list*/
	printf("<bps_list><value>0</value><name>HIGH</name></bps_list>\n");
	printf("<bps_list><value>1</value><name>MEDIUM</name></bps_list>\n");
	printf("<bps_list><value>2</value><name>LOW</name></bps_list>\n");

	/* rc list*/
	printf("<rc_list><value>0</value><name>VBR</name></rc_list>\n");
	printf("<rc_list><value>1</value><name>CBR</name></rc_list>\n");

	printf("<rec_fps>%d</rec_fps>\n", q.rec.fps);
	printf("<rec_bps>%d</rec_bps>\n", q.rec.bps);
	printf("<rec_gop>%d</rec_gop>\n", q.rec.gop);
	printf("<rec_rc>%d</rec_rc>\n",   q.rec.rc);
	printf("<stm_fps>%d</stm_fps>\n", q.stm.fps);
	printf("<stm_bps>%d</stm_bps>\n", q.stm.bps);
	printf("<stm_gop>%d</stm_gop>\n", q.stm.gop);
	printf("<stm_rc>%d</stm_rc>\n",   q.stm.rc);
#endif

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
