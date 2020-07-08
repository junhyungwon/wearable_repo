#include <stdio.h>
#include <string.h>

#include "cgi.h"
#include "cgi_param.h"
#include "cgi_uds.h"

#define HREF_PAGE "/html/settings_servers.xsl"

int main()
{
	int i=0;
	printf("Content-type: text/xml\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", HREF_PAGE);

	printf("<settings_body lang=\"%s\">\n", SUPPORT_LANG);
	printf("<settings_contents>\n");
	
	T_CGI_SERVERS_CONFIG t;memset(&t,0, sizeof t);
	sysctl_message(UDS_GET_SERVERS_CONFIG, (void*)&t, sizeof t );

	/* time zone list*/
#if 0
	for(i=-12;i<=14;i++){
		printf("<timezone_list><value>%d</value><name>UTC%+d:00</name></timezone_list>\n", i+12, i);
	}
#else
printf("<timezone_list><value>-12</value><name>UTC-12,BIT,Baker Island Time</name></timezone_list>\n");
printf("<timezone_list><value>-12</value><name>UTC-12,IDLW,International Day Line West time zone</name></timezone_list>\n");
printf("<timezone_list><value>-11</value><name>UTC-11,NUT,Niue Time</name></timezone_list>\n");
printf("<timezone_list><value>-11</value><name>UTC-11,SST,Samoa Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-10</value><name>UTC-10,CKT,Cook Island Time</name></timezone_list>\n");
printf("<timezone_list><value>-10</value><name>UTC-10,HST,HawaiiAleutian Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-10</value><name>UTC-10,SDT,Samoa Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-10</value><name>UTC-10,TAHT,Tahiti Time</name></timezone_list>\n");
printf("<timezone_list><value>-09</value><name>UTC-09,AKST,Alaska Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-09</value><name>UTC-09,GAMT,Gambier Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>-09</value><name>UTC-09,GIT,Gambier Island Time</name></timezone_list>\n");
printf("<timezone_list><value>-09</value><name>UTC-09,HDT,HawaiiAleutian Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-08</value><name>UTC-08,AKDT,Alaska Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-08</value><name>UTC-08,CIST,Clipperton Island Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-08</value><name>UTC-08,PST,Pacific Standard Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-07</value><name>UTC-07,MST,Mountain Standard Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-07</value><name>UTC-07,PDT,Pacific Daylight Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-06</value><name>UTC-06,CST,Central Standard Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-06</value><name>UTC-06,EAST,Easter Island Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-06</value><name>UTC-06,GALT,Galápagos Time</name></timezone_list>\n");
printf("<timezone_list><value>-06</value><name>UTC-06,MDT,Mountain Daylight Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,ACT,Acre Time</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,CDT,Central Daylight Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,COT,Colombia Time</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,CST,Cuba Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,EASST,Easter Island Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,ECT,Ecuador Time</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,EST,Eastern Standard Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-05</value><name>UTC-05,PET,Peru Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,AMT,Amazon Time (Brazil)</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,AST,Atlantic Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,BOT,Bolivia Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,CDT,Cuba Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,CLT,Chile Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,COST,Colombia Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,ECT,Eastern Caribbean Time (does not recognise DST)</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,EDT,Eastern Daylight Time (North America)</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,FKT,Falkland Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,GYT,Guyana Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,PYT,Paraguay Time</name></timezone_list>\n");
printf("<timezone_list><value>-04</value><name>UTC-04,VET,Venezuelan Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,ADT,Atlantic Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,AMST,Amazon Summer Time (Brazil)</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,ART,Argentina Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,BRT,Brasilia Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,CLST,Chile Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,FKST,Falkland Islands Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,GFT,French Guiana Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,PMST,Saint Pierre and Miquelon Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,PYST,Paraguay Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,ROTT,Rothera Research Station Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,SRT,Suriname Time</name></timezone_list>\n");
printf("<timezone_list><value>-03</value><name>UTC-03,UYT,Uruguay Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-02</value><name>UTC-02,BRST,Brasília Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-02</value><name>UTC-02,FNT,Fernando de Noronha Time</name></timezone_list>\n");
printf("<timezone_list><value>-02</value><name>UTC-02,GST,South Georgia and the South Sandwich Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>-02</value><name>UTC-02,PMDT,Saint Pierre and Miquelon Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>-02</value><name>UTC-02,UYST,Uruguay Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>-01</value><name>UTC-01,AZOT,Azores Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>-01</value><name>UTC-01,CVT,Cape Verde Time</name></timezone_list>\n");
printf("<timezone_list><value>-01</value><name>UTC-01,EGT,Eastern Greenland Time</name></timezone_list>\n");
printf("<timezone_list><value>00</value><name>UTC±00,AZOST,Azores Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>00</value><name>UTC±00,EGST,Eastern Greenland Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>00</value><name>UTC±00,GMT,Greenwich Mean Time</name></timezone_list>\n");
printf("<timezone_list><value>00</value><name>UTC±00,UTC,Coordinated Universal Time</name></timezone_list>\n");
printf("<timezone_list><value>00</value><name>UTC±00,WET,Western European Time</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,CET,Central European Time</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,DFT,AIX-specific equivalent of Central European Time</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,IST,Irish Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,MET,Middle European Time Same zone as CET</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,WAT,West Africa Time</name></timezone_list>\n");
printf("<timezone_list><value>01</value><name>UTC+01,WEST,Western European Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,CAT,Central Africa Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,CEST,Central European Summer Time (Cf. HAEC)</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,EET,Eastern European Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,HAEC,Heure Avancée d'Europe Centrale French-language name for CEST</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,IST,Israel Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,KALT,Kaliningrad Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,MEST,Middle European Summer Time Same zone as CEST</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,SAST,South African Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>02</value><name>UTC+02,WAST,West Africa Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,AST,Arabia Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,EAT,East Africa Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,EEST,Eastern European Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,FET,Further-eastern European Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,IDT,Israel Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,IOT,Indian Ocean Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,MSK,Moscow Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,SYOT,Showa Station Time</name></timezone_list>\n");
printf("<timezone_list><value>03</value><name>UTC+03,TRT,Turkey Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,AMT,Armenia Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,AZT,Azerbaijan Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,GET,Georgia Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,GST,Gulf Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,MUT,Mauritius Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,RET,Réunion Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,SAMT,Samara Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,SCT,Seychelles Time</name></timezone_list>\n");
printf("<timezone_list><value>04</value><name>UTC+04,VOLT,Volgograd Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,AQTT,Aqtobe Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,HMT,Heard and McDonald Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,MAWT,Mawson Station Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,MVT,Maldives Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,ORAT,Oral Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,PKT,Pakistan Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,TFT,French Southern and Antarctic Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,TJT,Tajikistan Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,TMT,Turkmenistan Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,UZT,Uzbekistan Time</name></timezone_list>\n");
printf("<timezone_list><value>05</value><name>UTC+05,YEKT,Yekaterinburg Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,ALMT,Alma-Ata Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,BIOT,British Indian Ocean Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,BST,Bangladesh Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,BTT,Bhutan Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,KGT,Kyrgyzstan Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,OMST,Omsk Time</name></timezone_list>\n");
printf("<timezone_list><value>06</value><name>UTC+06,VOST,Vostok Station Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,CXT,Christmas Island Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,DAVT,Davis Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,HOVT,Hovd Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,ICT,Indochina Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,KRAT,Krasnoyarsk Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,NOVT,Novosibirsk Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,THA,Thailand Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>07</value><name>UTC+07,WIT,Western Indonesian Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,AWST,Australian Western Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,BDT,Brunei Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,CHOT,Choibalsan Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,CIT,Central Indonesia Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,CST,China Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,CT,China Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,HKT,Hong Kong Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,IRKT,Irkutsk Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,MST,Malaysia Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,MYT,Malaysia Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,PHT,Philippine Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,PST,Philippine Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,SGT,Singapore Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,SST,Singapore Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,ULAT,Ulaanbaatar Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>08</value><name>UTC+08,WST,Western Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,CHOST,Choibalsan Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,EIT,Eastern Indonesian Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,JST,Japan Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,KST,Korea Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,TLT,Timor Leste Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,ULAST,Ulaanbaatar Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>09</value><name>UTC+09,YAKT,Yakutsk Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,AEST,Australian Eastern Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,CHST,Chamorro Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,CHUT,Chuuk Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,DDUT,Dumont d'Urville Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,PGT,Papua New Guinea Time</name></timezone_list>\n");
printf("<timezone_list><value>10</value><name>UTC+10,VLAT,Vladivostok Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,AEDT,Australian Eastern Daylight Savings Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,BST,Bougainville Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,KOST,Kosrae Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,LHST,Lord Howe Summer Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,MIST,Macquarie Island Station Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,NCT,New Caledonia Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,NFT,Norfolk Island Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,PONT,Pohnpei Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,SAKT,Sakhalin Island Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,SBT,Solomon Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,SRET,Srednekolymsk Time</name></timezone_list>\n");
printf("<timezone_list><value>11</value><name>UTC+11,VUT,Vanuatu Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,ANAT,Anadyr Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,FJT,Fiji Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,GILT,Gilbert Island Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,MAGT,Magadan Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,MHT,Marshall Islands Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,NZST,New Zealand Standard Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,PETT,Kamchatka Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,TVT,Tuvalu Time</name></timezone_list>\n");
printf("<timezone_list><value>12</value><name>UTC+12,WAKT,Wake Island Time</name></timezone_list>\n");
printf("<timezone_list><value>13</value><name>UTC+13,NZDT,New Zealand Daylight Time</name></timezone_list>\n");
printf("<timezone_list><value>13</value><name>UTC+13,PHOT,Phoenix Island Time</name></timezone_list>\n");
printf("<timezone_list><value>13</value><name>UTC+13,TKT,Tokelau Time</name></timezone_list>\n");
printf("<timezone_list><value>13</value><name>UTC+13,TOT,Tonga Time</name></timezone_list>\n");
printf("<timezone_list><value>14</value><name>UTC+14,LINT,Line Islands Time</name></timezone_list>\n");

#if 0//old version
	printf("<timezone_list><value>-11</value><name>UTC-11:00</name></timezone_list>\n");
	printf("<timezone_list><value>-10</value><name>UTC-10:00</name></timezone_list>\n");
	printf("<timezone_list><value>-9</value><name>UTC-9:00</name></timezone_list>\n");
	printf("<timezone_list><value>-8</value><name>UTC-8:00</name></timezone_list>\n");
	printf("<timezone_list><value>-7</value><name>UTC-7:00</name></timezone_list>\n");
	printf("<timezone_list><value>-6</value><name>UTC-6:00</name></timezone_list>\n");
	printf("<timezone_list><value>-5</value><name>UTC-5:00</name></timezone_list>\n");
	printf("<timezone_list><value>-4</value><name>UTC-4:00</name></timezone_list>\n");
	printf("<timezone_list><value>-3</value><name>UTC-3:00</name></timezone_list>\n");
	printf("<timezone_list><value>-2</value><name>UTC-2:00</name></timezone_list>\n");
	printf("<timezone_list><value>-1</value><name>UTC-1:00</name></timezone_list>\n");
	printf("<timezone_list><value>0</value><name>UTC</name></timezone_list>\n");
	printf("<timezone_list><value>1</value><name>UTC+1:00</name></timezone_list>\n");
	printf("<timezone_list><value>2</value><name>UTC+2:00</name></timezone_list>\n");
	printf("<timezone_list><value>3</value><name>UTC+3:00</name></timezone_list>\n");
	printf("<timezone_list><value>4</value><name>UTC+4:00</name></timezone_list>\n");
	printf("<timezone_list><value>5</value><name>UTC+5:00</name></timezone_list>\n");
	printf("<timezone_list><value>6</value><name>UTC+6:00</name></timezone_list>\n");
	printf("<timezone_list><value>7</value><name>UTC+7:00</name></timezone_list>\n");
	printf("<timezone_list><value>8</value><name>UTC+8:00</name></timezone_list>\n");
	printf("<timezone_list><value>9</value><name>UTC+9:00</name></timezone_list>\n");
	printf("<timezone_list><value>10</value><name>UTC+10:00</name></timezone_list>\n");
	printf("<timezone_list><value>11</value><name>UTC+11:00</name></timezone_list>\n");
	printf("<timezone_list><value>12</value><name>UTC+12:00</name></timezone_list>\n");
	printf("<timezone_list><value>13</value><name>UTC+13:00</name></timezone_list>\n");
	printf("<timezone_list><value>14</value><name>UTC+14:00</name></timezone_list>\n");
#endif

#endif


	printf("<bs>\n");
	printf("<enable>%d</enable>\n", t.bs.enable);
	printf("<serveraddr>%s</serveraddr>\n",     t.bs.serveraddr);
	printf("<id>%s</id>\n",         t.bs.id);
	printf("<pw>%s</pw>\n",         t.bs.pw);
	printf("<port>%d</port>\n",     t.bs.port);
	printf("</bs>\n");

	printf("<ms>\n");
	printf("<enable>%d</enable>\n", t.ms.enable);
	printf("<serveraddr>%s</serveraddr>\n",     t.ms.serveraddr);
	printf("<port>%d</port>\n",     t.ms.port);
	printf("</ms>\n");
	
	printf("<ddns>\n");
	printf("<enable>%d</enable>\n", t.ddns.enable);
	printf("<serveraddr>%s</serveraddr>\n", t.ddns.serveraddr);
	printf("<hostname>%s</hostname>\n", t.ddns.hostname);
	printf("<id>%s</id>\n",				t.ddns.id);
	printf("<pw>%s</pw>\n",				t.ddns.pw);
	printf("</ddns>\n");

	printf("<dns>\n");
	printf("<server1>%s</server1>\n", t.dns.server1);
	printf("<server2>%s</server2>\n", t.dns.server2);
	printf("</dns>\n");

	printf("<ntp>\n");
	printf("<enable>%d</enable>\n", t.ntp.enable);
	printf("<serveraddr>%s</serveraddr>\n",     t.ntp.serveraddr);
	printf("</ntp>\n");

	printf("<timezone>%d</timezone>\n", t.time_zone); // already -12 on Device
	printf("<timezone_abbr>%s</timezone_abbr>\n", t.time_zone_abbr);
	printf("<daylightsaving>%d</daylightsaving>\n", t.daylight_saving);

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</settings_contents>\n");
	printf("</settings_body>\n");

	return 0;
}
