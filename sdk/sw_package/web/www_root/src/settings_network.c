#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_network.xsl"

int main()
{
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_NETWORK_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_NETWORK_CONFIG, (void*)&t, sizeof t );

	/* ip type list*/
	printf("<ip_type_list><value>0</value><name>STATIC</name></ip_type_list>\n");
	printf("<ip_type_list><value>1</value><name>DHCP</name></ip_type_list>\n");

	printf("<wireless>");
	printf("<addr_type>%d</addr_type>\n", t.wireless.addr_type); // static or dhcp
	printf("<ipv4>%s</ipv4>\n",       t.wireless.ipv4);
	printf("<gateway>%s</gateway>\n", t.wireless.gw);
	printf("<netmask>%s</netmask>\n", t.wireless.mask);
	printf("</wireless>");

	printf("<cradle>");
	printf("<addr_type>%d</addr_type>\n", t.cradle.addr_type); // static or dhcp
	printf("<ipv4>%s</ipv4>\n",       t.cradle.ipv4);
	printf("<gateway>%s</gateway>\n", t.cradle.gw);
	printf("<netmask>%s</netmask>\n", t.cradle.mask);
	printf("</cradle>");

	printf("<wifi_ap>\n");
	printf("<ssid>%s</ssid>\n", t.wifi_ap.id);
	printf("<pw>%s</pw>\n",     t.wifi_ap.pw);
	printf("</wifi_ap>\n");

	/* live stream account enc type list*/
	printf("<enc_type_list><value>0</value><name>NONE</name></enc_type_list>\n");
	printf("<enc_type_list><value>1</value><name>AES</name></enc_type_list>\n");

	printf("<live_stream_account>\n");
	printf("<enable>%d</enable>\n",     t.live_stream_account_enable);
	printf("<enc_type>%d</enc_type>\n", t.live_stream_account_enctype);
	printf("<id>%s</id>\n",             t.live_stream_account.id);
	printf("<pw>%s</pw>\n",             t.live_stream_account.pw);
	printf("</live_stream_account>\n");

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
