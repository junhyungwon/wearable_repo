#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>

#include <sys/socket.h>		//13:AF_INET
#include <netinet/in.h>		//22:struct sockaddr_in
#include <netdb.h>


#include <arpa/inet.h>

#include "P2PTunnelAPIs.h"

#define MAX_SERVER_CONNECT_NUM		4
#define WEB_MAPPING_LOCAL_PORT		10000
#define SSH_MAPPING_LOCAL_PORT		20000
#define TELNET_MAPPING_LOCAL_PORT	30000
#define PASSWORD_MAPPING_LOCAL_PORT	40000
#define WEB_MAPPING_REMOTE_PORT		8551
#define SSH_MAPPING_REMOTE_PORT		22
#define TELNET_MAPPING_REMOTE_PORT	23
#define PASSWORD_MAPPING_REMOTE_PORT	9000

#define SERVER_IPV4_ADDR		"127.0.0.1"

char *gUID[MAX_SERVER_CONNECT_NUM];
int gProcessRun = 1;
int gWebIndex[MAX_SERVER_CONNECT_NUM];
int gTelnetIndex[MAX_SERVER_CONNECT_NUM];
int gSshIndex[MAX_SERVER_CONNECT_NUM];
int gPasswordIndex[MAX_SERVER_CONNECT_NUM];
int gRetryConnectFailedCnt[MAX_SERVER_CONNECT_NUM];

int gWebPort = WEB_MAPPING_LOCAL_PORT;

// default password if no "p2p.txt" exist
char gUsername[32]="linkflow";
char gPassword[32]="12345678";
int connectTime = 0x07;
int relayType = 0x02;
int p2pServer = 0x00;

typedef struct st_AuthData
{
	char szUsername[64];
	char szPassword[64];
} sAuthData;

typedef struct st_SessionHandler
{
	int nSID;
	int bException;
} sSessionHandler;

sSessionHandler gsSessionHandler[MAX_SERVER_CONNECT_NUM];

int TunnelAgentStart(sAuthData *pAuthData)
{
  
	P2PTunnel_SetServer(p2pServer);
	int ret = P2PTunnelAgentInitialize(MAX_SERVER_CONNECT_NUM);
	if(ret < 0)
	{
		printf("P2PTunnelAgentInitialize error[%d]!\n", ret);
		return -1;
	}

	
	int i,j;
	for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
	{
		if(gUID[i] == NULL) continue;

		char DID[24];
	
		int nErrFromDevice = 0;
		/* If you don't want to use authentication mechanism, you can give NULL argument
		gsSessionHandler[i].nSID = P2PTunnelAgent_Connect(gUID[i], NULL, 0, &nErrFromDevice);
		*/
		

		printf("connectTime = 0x%X\n",connectTime);
		P2PTunnel_SetRelayVersion(relayType);
		// P2PTunnelAgent_SetConnectTime(0x06);
		// P2PTunnelAgent_SetConnectTime(0x1E);
		P2PTunnelAgent_SetConnectTime(connectTime);
		
		gsSessionHandler[i].nSID = P2PTunnelAgent_Connect(gUID[i], (void *)pAuthData, sizeof(sAuthData), &nErrFromDevice);

		printf("gsSessionHandler[%d].nSID = %d, nErrFromDevice:%d\n", i, gsSessionHandler[i].nSID, nErrFromDevice);

		//PPPP_QueryDID(gUID[i], DID, 24);
		//printf("SERVER DID[%s]\n", DID);
		st_P2PTunnel_Connect_Info sessionInfo;
		/*
		P2PTunnelAgent_Check(gsSessionHandler[i].nSID, &sessionInfo);
		
		printf("----------Session(%d) Ready: %s----------\n", gsSessionHandler[i].nSID, (sessionInfo.bMode == 0) ? "P2P" : "RLY");
		printf("Socket : %d\n", sessionInfo.Skt);
		printf("Remote Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.RemoteAddr.sin_addr), ntohs(sessionInfo.RemoteAddr.sin_port));
		printf("My Lan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyLocalAddr.sin_addr), ntohs(sessionInfo.MyLocalAddr.sin_port));
		printf("My Wan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyWanAddr.sin_addr), ntohs(sessionInfo.MyWanAddr.sin_port));
		printf("Connection time : %d second before\n", sessionInfo.ConnectTime);
		printf("I am %s\n", (sessionInfo.bCorD ==0) ? "Client" : "Device");
		printf("Connection mode: %s\n", (sessionInfo.bMode == 0) ? "P2P" : "RLY");
		printf("----------End of Session info :----------\n");
		*/
		
		if(gsSessionHandler[i].nSID < 0)
		{
			printf("P2PTunnelAgent_Connect failed[%d], device respond reject reason[%d]\n", gsSessionHandler[i].nSID, nErrFromDevice);
			return -1;
		}
		else
		{
			printf("P2PTunnelAgent_Connect OK SID[%d]\n", gsSessionHandler[i].nSID);
			st_User_Profile userProfile;
			P2PTunnelAgent_GetProfile(gsSessionHandler[i].nSID, &userProfile);
			/*
			printf("httpPort:        0x%X\n", userProfile.httpPort);
			printf("userPort_1:      0x%X\n", userProfile.userPort_1);
			printf("userPort_2:      0x%X\n", userProfile.userPort_2);
			printf("userPort_3:      0x%X\n", userProfile.userPort_3);
			printf("FirmwareVersion: %s\n", userProfile.FirmwareVersion);
			printf("ModalName:       %s\n", userProfile.ModalName);
			printf("DeviceName:      %s\n", userProfile.DeviceName);

			for (j = 0 ; j <sizeof(userProfile.Reserved) ; j++)
			{
				printf("0x%02X ", (unsigned char)userProfile.Reserved[j]);
			}
			
			printf("] size(%d)\n", sizeof(userProfile.Reserved));	
			*/
		}

		if(P2PTunnel_SetBufSize(gsSessionHandler[i].nSID, 5120000) < 0)
			printf("P2PTunnel_SetBufSize error SID[%d]\n", gsSessionHandler[i].nSID);
		
		
		gWebIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, gWebPort+i, WEB_MAPPING_REMOTE_PORT);
		if(gWebIndex[gsSessionHandler[i].nSID] < 0)
		{
			printf("P2PTunnelAgent_PortMapping WEB error[%d]!\n", gWebIndex[gsSessionHandler[i].nSID]);
		}		
		else {
			printf("P2PTunnelAgent_PortMapping WEB port[%d]!\n", gWebIndex[gsSessionHandler[i].nSID]);
		}
		
		
		gTelnetIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, TELNET_MAPPING_LOCAL_PORT+i, TELNET_MAPPING_REMOTE_PORT);
		if(gTelnetIndex[gsSessionHandler[i].nSID] < 0)
		{
			printf("P2PTunnelAgent_PortMapping Telnet error[%d]!\n", gTelnetIndex[gsSessionHandler[i].nSID]);
		}
		
		gSshIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, SSH_MAPPING_LOCAL_PORT+i, SSH_MAPPING_REMOTE_PORT);
		if(gSshIndex[gsSessionHandler[i].nSID] < 0)
		{
			printf("P2PTunnelAgent_PortMapping SSH error[%d]!\n", gSshIndex[gsSessionHandler[i].nSID]);
		}

		gPasswordIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, PASSWORD_MAPPING_LOCAL_PORT+i, PASSWORD_MAPPING_REMOTE_PORT);
		if(gPasswordIndex[gsSessionHandler[i].nSID] < 0)
		{
			printf("P2PTunnelAgent_PortMapping Password error[%d]!\n", gPasswordIndex[gsSessionHandler[i].nSID]);
		}
		
	}

	// sleep(3);
	
	
	int Skt;
	int remotePort = 10000;
	
	Skt = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IPV4_ADDR);
	server_addr.sin_port = htons(remotePort);
	int Connected = 0;
	if (connect(Skt, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0 ) {
	    printf("Fail to Connected to %s:%d.\n", SERVER_IPV4_ADDR, remotePort);
	}
	else
	{
	    printf("Success to Connected to %s:%d.\n", SERVER_IPV4_ADDR, remotePort);
	}
	close(Skt);
	
	
	return 0;
}

void TunnelStatusCB(int nErrorCode, int nSID, void *pArg)
{
    /* Don't do any Tunnel API call, because it maybe cause process dead lock. */

	printf("nErrorCode = %d, nSID = %d, MyArg = %s\n", nErrorCode,nSID, (char *)pArg);
	if(nErrorCode == TUNNEL_ER_DISCONNECTED)
	{
		if(pArg != NULL)
			printf("MyArg = %s\n", (char *)pArg);

		int i;
		for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
		{
			if(gsSessionHandler[i].nSID == nSID)
			{
				gsSessionHandler[i].bException = 1;
				printf("Stop nSID = %d\n",nSID);
				break;
			}
		}
	}
}

void TunnelStatusHandler(void)
{
	int i;
    for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
    {
		if(gsSessionHandler[i].bException)
		{
            int nSID = gsSessionHandler[i].nSID;

            P2PTunnelAgent_Disconnect(nSID);

            P2PTunnelAgent_StopPortMapping(gWebIndex[nSID]);
            P2PTunnelAgent_StopPortMapping(gTelnetIndex[nSID]);
            P2PTunnelAgent_StopPortMapping(gSshIndex[nSID]);
            P2PTunnelAgent_StopPortMapping(gPasswordIndex[nSID]);

            gsSessionHandler[i].nSID = -1;
            gsSessionHandler[i].bException = 0;
		}
	}
}

void InitGlobalArgument()
{
	int i;
	for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
	{
		gUID[i] = NULL;
		gsSessionHandler[i].nSID = -1;
		gsSessionHandler[i].bException = 0;
		gWebIndex[i] = -1;
		gSshIndex[i] = -1;
		gTelnetIndex[i] = -1;
		gPasswordIndex[i] = -1;
		gRetryConnectFailedCnt[i] = 0;
	}
}

void trimRightLF(char *str){
	int len = strlen(str);
	if(len>0 && str[len-1]=='\n')
		str[len-1] = '\0';
}


int main(int argc, char *argv[])
{
	FILE *pfd = NULL;
	
	struct sockaddr_in RemoteAddr;
	
	if(argc < 2)
	{
		printf("P2PTunnelAgent UID ShowDebug ConnecTime RelayType P2PServer!\n");
		printf("ConnecTime = 7\n");
		printf("RelayType 1 = Old Relay, 2 = New Relay\n");
		printf("P2PServer 0 = Formal Server, 1 = Test Server\n");
		return 0;
	}

	if(argc >=3)
		P2PTunnel_ShowDebug(atoi(argv[2]));
	
	if(argc >=4)
		connectTime = atoi(argv[3]);
	
	if(argc >=5)
		relayType = atoi(argv[4]);
	
	if(argc >=6)
		p2pServer = atoi(argv[5]);
	
	InitGlobalArgument();

	int i, j = 0;
	// for(i=1;i<argc;i++)
	for(i=1;i<=1;i++)
	{
		gUID[j] = (char *)malloc(21);
		if(gUID[j] != NULL)
		{
			strcpy(gUID[j], argv[i]);
			printf("SERVER UID[%s]\n", gUID[j]);
			j++;
		}
	}

	char *s = "My arg Pass to call back function";
	P2PTunnelAgent_GetStatus(TunnelStatusCB, (void *)s);
	
	sAuthData authData;

	if( (pfd=fopen("p2p.txt","r")) !=NULL )
	{
		memset(gUsername, 0, sizeof(gUsername));
		if ( fgets(gUsername, sizeof(gUsername), pfd) != NULL ) {
			trimRightLF(gUsername);
		}

		memset(gPassword, 0, sizeof(gPassword));
		if ( fgets(gPassword, sizeof(gPassword), pfd) != NULL ) {
			trimRightLF(gPassword);
		}

		fclose(pfd);
	}

	strcpy(authData.szUsername, gUsername);
	strcpy(authData.szPassword, gPassword);

	if(TunnelAgentStart(&authData) < 0)
	{
		printf("TunnelAgentStart failed\n");
		return -1;
	}

	int count = 0;
	while(gProcessRun)
	// while(count < 20)
	{
		int i;
		for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
		{
			if(gsSessionHandler[i].nSID >= 0)
			{
				// Lance
				//int access_time = P2PTunnel_LastIOTime(gsSessionHandler[i].nSID);
				//if(access_time >= 0)
				//	printf("SID %d:%u\n", gsSessionHandler[i].nSID, access_time);
			}
			if(gUID[i] != NULL && gsSessionHandler[i].nSID < 0)
			{
				printf("Reconnect to UID[%s]\n", gUID[i]);
				int nErrFromDevice;
				gsSessionHandler[i].nSID = P2PTunnelAgent_Connect(gUID[i], (void *)&authData, sizeof(sAuthData), &nErrFromDevice);
				if(gsSessionHandler[i].nSID < 0)
				{
					printf("P2PTunnelAgent_Connect failed[%d], UID[%s], device respond reject reason[%d]\n", gsSessionHandler[i].nSID, gUID[i], nErrFromDevice);
					if(++gRetryConnectFailedCnt[i] > 3)
					{
						printf("Retry connection timeout UID[%s]\n", gUID[i]);
						free(gUID[i]);
						gUID[i] = NULL;
					}
				}
				else
				{
					if(P2PTunnel_SetBufSize(gsSessionHandler[i].nSID, 5120000) < 0)
						printf("P2PTunnel_SetBufSize error SID[%d]\n", gsSessionHandler[i].nSID);
					
					gWebIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, WEB_MAPPING_LOCAL_PORT+i, WEB_MAPPING_REMOTE_PORT);
					if(gWebIndex[gsSessionHandler[i].nSID] < 0)
					{
						printf("P2PTunnelAgent_PortMapping WEB error[%d]!\n", gWebIndex[gsSessionHandler[i].nSID]);
					}
					
					gTelnetIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, TELNET_MAPPING_LOCAL_PORT+i, TELNET_MAPPING_REMOTE_PORT);
					if(gTelnetIndex[gsSessionHandler[i].nSID] < 0)
					{
						printf("P2PTunnelAgent_PortMapping Telnet error[%d]!\n", gTelnetIndex[gsSessionHandler[i].nSID]);
					}

					
					gSshIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, SSH_MAPPING_LOCAL_PORT+i, SSH_MAPPING_REMOTE_PORT);
					if(gSshIndex[gsSessionHandler[i].nSID] < 0)
					{
						printf("P2PTunnelAgent_PortMapping SSH error[%d]!\n", gSshIndex[gsSessionHandler[i].nSID]);
					}
					gPasswordIndex[gsSessionHandler[i].nSID] = P2PTunnelAgent_PortMapping(gsSessionHandler[i].nSID, PASSWORD_MAPPING_LOCAL_PORT+i, PASSWORD_MAPPING_REMOTE_PORT);
					if(gPasswordIndex[gsSessionHandler[i].nSID] < 0)
					{
						printf("P2PTunnelAgent_PortMapping Password error[%d]!\n", gPasswordIndex[gsSessionHandler[i].nSID]);
					}
					
					gRetryConnectFailedCnt[i] = 0;
				}
			}
		}

		sleep(1);
		count++;
		TunnelStatusHandler();
	}

	/*
	for(i=0;i<MAX_SERVER_CONNECT_NUM;i++)
	{
		if(gsSessionHandler[i].nSID >= 0)
		{
		    P2PTunnelAgent_StopPortMapping(gTelnetIndex[gsSessionHandler[i].nSID]);
		    //P2PTunnelAgent_Disconnect(gsSessionHandler[i].nSID);
		}
	}
	
	while(gProcessRun)
	{
	    sleep(1);
	}
	
	P2PTunnelAgentDeInitialize();
	*/
	
	return 0;
}

