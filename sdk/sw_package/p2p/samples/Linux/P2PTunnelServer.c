#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>
#include "P2PTunnelAPIs.h"
// #include "IOTCAPIs.h"
// Enable auto launch PasswordServer.
#define LAUNCH_PASSWORD_PROC 0

char gUID[21];

int gSID_Used[20];
int gSID_StartTime[20];

// default password if no "passwd.txt" exist
char gUsername[32]="linkflow";
char gPassword[32]="12345678";

int p2pServer = 0x00;

#if LAUNCH_PASSWORD_PROC
int gPassPID=0;
#endif

typedef struct st_AuthData
{
	char szUsername[64];
	char szPassword[64];
} sAuthData;

void trimRightLF(char *str){
	int len = strlen(str);
	if(len>0 && str[len-1]=='\n')
		str[len-1] = '\0';
}

void TunnelStatusCB(int nErrorCode, int nSID, void *pArg)
{
	if(nErrorCode == TUNNEL_ER_DISCONNECTED)
	{
		printf("SID[%d] TUNNEL_ER_DISCONNECTED Log file here!\n", nSID);
		gSID_Used[nSID] = 0;
		if(pArg != NULL)
			printf("MyArg = %s\n", (char *)pArg);
	}
}

void *Thread_TestCloseSession(void *arg)
{
	pthread_detach(pthread_self());
	
	int SID = *((int *)arg);
	free(arg);
	srand(time(NULL));
    int nDelayTime = rand() % 60;
    printf("nDelayTime to close[%d]\n", nDelayTime);
	sleep(nDelayTime);
	printf("P2PTunnelServer_Disconnect SID[%d] ret[%d]\n", SID, P2PTunnelServer_Disconnect(SID));
	
	return 0;
}

int TunnelSessionInfoCB(sP2PTunnelSessionInfo *sSessionInfo, void *pArg)
{
	printf("TunnelSessionInfoCB trigger\n");
	if(pArg != NULL) printf("pArg = %s\n", (char *)pArg);
	printf("[Client Session Info]\n");
	printf("  Connection Mode = %d, NAT type = %d\n", sSessionInfo->nMode, sSessionInfo->nNatType);
	printf("  P2PTunnel Version = %X, SID = %d\n", (unsigned int)sSessionInfo->nVersion, sSessionInfo->nSID);
	printf("  IP Address = %s:%d\n", sSessionInfo->szRemoteIP, sSessionInfo->nRemotePort);
	
	if(sSessionInfo->nAuthDataLen == 0 || sSessionInfo->pAuthData == NULL)
		return -777;
	else if(sSessionInfo->nAuthDataLen > 0)
	{
		st_P2PTunnel_Connect_Info sessionInfo;
		
		P2PTunnelServer_Check(sSessionInfo->nSID, &sessionInfo);
		
		printf("----------Session(%d) Ready: %s----------\n", sSessionInfo->nSID, (sessionInfo.bMode == 0) ? "P2P" : "RLY");
		printf("Socket : %d\n", sessionInfo.Skt);

#if 1
		printf("Remote Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.RemoteAddr.sin_addr), ntohs(sessionInfo.RemoteAddr.sin_port));
		printf("My Lan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyLocalAddr.sin_addr), ntohs(sessionInfo.MyLocalAddr.sin_port));
		printf("My Wan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyWanAddr.sin_addr), ntohs(sessionInfo.MyWanAddr.sin_port));
#endif

		printf("Connection time : %d second before\n", sessionInfo.ConnectTime);
		printf("I am %s\n", (sessionInfo.bCorD ==0) ? "Client" : "Device");
		printf("Connection mode: %s\n", (sessionInfo.bMode == 0) ? "P2P" : "RLY");
		printf("----------End of Session info :----------\n\n");
				
		FILE *pfd = fopen("/tmp/passwd.txt","r");
		if(pfd!=NULL)
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

		sAuthData *pAuthData = (sAuthData *)sSessionInfo->pAuthData;
		printf("  Auth data length = %d, username = %s, passwd = %s\n", sSessionInfo->nAuthDataLen, pAuthData->szUsername, pAuthData->szPassword);
		printf("  password.txt user = %s, passwd = %s\n", gUsername, gPassword);
		if(strcmp(pAuthData->szUsername, gUsername) != 0 || strcmp(pAuthData->szPassword, gPassword) != 0) {
			printf("Failed authentication\n");
			return -888;
		}
	}
	gSID_Used[sSessionInfo->nSID] = 1;
	gSID_StartTime[sSessionInfo->nSID] = time(NULL);
	// if(P2PTunnel_SetBufSize(sSessionInfo->nSID, 10000) < 0)
	// if(P2PTunnel_SetBufSize(sSessionInfo->nSID, 512000) < 0)
	if(P2PTunnel_SetBufSize(sSessionInfo->nSID, 256000) < 0)
		printf("P2PTunnel_SetBufSize error SID[%d]\n", sSessionInfo->nSID);
	
	
	char buffer[1024];
	int size;
	long Capibility, Available;
	st_Device_status_headers deviceStatusHeader;
	st_Device_status_videoLoss deviceStatusVideoLoss;
	st_Device_status_hdd deviceStatusHdd_1, deviceStatusHdd_2;
	st_Device_status_networks deviceStatusNetwork;
	st_Device_status_info deviceStatusInfo;
	
	deviceStatusHeader.type = INFO_STATUS;
	deviceStatusHeader.number = 1;
	deviceStatusHeader.size = sizeof(st_Device_status_videoLoss)*deviceStatusHeader.number;
	strcpy(deviceStatusInfo.DeviceType,"DeviceType");
	memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
	memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusInfo, sizeof(st_Device_status_info));
	size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_info)*deviceStatusHeader.number;
	P2PTunnelServer_SetStatus(buffer, size);

	/*
	deviceStatusHeader.type = VIDEO_STATUS;
	deviceStatusHeader.number = 1;
	deviceStatusHeader.size = sizeof(st_Device_status_videoLoss)*deviceStatusHeader.number;
	deviceStatusVideoLoss.videoLoss_1 = 1;
	deviceStatusVideoLoss.videoLoss_2 = 2;
	deviceStatusVideoLoss.videoLoss_3 = 3;
	deviceStatusVideoLoss.videoLoss_4 = 4;
	deviceStatusVideoLoss.timestamp = 1346426869;
	memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
	memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusVideoLoss, sizeof(st_Device_status_videoLoss));
	size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_videoLoss)*deviceStatusHeader.number;
	P2PTunnelServer_SetStatus(buffer, size);
	
	deviceStatusHeader.type = NETWORK_STATUS;
	deviceStatusHeader.number = 1;
	deviceStatusHeader.size = sizeof(st_Device_status_networks)*deviceStatusHeader.number;
	deviceStatusNetwork.linkon = 1;
	deviceStatusNetwork.ip = 4;
	deviceStatusNetwork.timestamp = 1346426869;
	memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
	memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusNetwork, sizeof(st_Device_status_networks));
	size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_networks)*deviceStatusHeader.number;
	P2PTunnelServer_SetStatus(buffer, size);
	
	deviceStatusHeader.type = HDD_STATUS;
	deviceStatusHeader.number = 2;
	deviceStatusHeader.size = sizeof(st_Device_status_hdd)*deviceStatusHeader.number;
	deviceStatusHdd_1.number = 0;
	deviceStatusHdd_1.status = 1;
	deviceStatusHdd_1.flag = 2;
	deviceStatusHdd_1.timestamp = 1346426869;
	deviceStatusHdd_1.CapacityHigh = 0;
	deviceStatusHdd_1.CapacityLow = 200;
	deviceStatusHdd_1.AvailableHigh = 0;
	deviceStatusHdd_1.AvailableLow = 100;
						Capibility = deviceStatusHdd_1.CapacityHigh;
						printf("Capibility = %ld\n", Capibility);
						Capibility = Capibility<<32;
						printf("Capibility = %ld\n", Capibility);
						Capibility = Capibility + deviceStatusHdd_1.CapacityLow;
						printf("Capibility = %ld\n", Capibility);
						Available = deviceStatusHdd_1.AvailableHigh;
						printf("Available = %ld\n", Available);
						Available = Available<<32;
						printf("Available = %ld\n", Available);
						Available = Available + deviceStatusHdd_1.AvailableLow;
						printf("Available = %ld\n", Available);	
	strcpy(deviceStatusHdd_1.ModalName,"abc");

	deviceStatusHdd_2.number = 3;
	deviceStatusHdd_2.status = 4;
	deviceStatusHdd_2.flag = 5;
	deviceStatusHdd_2.timestamp = 1346426869;
	deviceStatusHdd_2.CapacityHigh = 0;
	deviceStatusHdd_2.CapacityLow = 400;
	deviceStatusHdd_2.AvailableHigh = 0;
	deviceStatusHdd_2.AvailableLow = 300;	
	strcpy(deviceStatusHdd_2.ModalName,"cde");
	memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
	memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusHdd_1, sizeof(st_Device_status_hdd));	
	memcpy(buffer+sizeof(st_Device_status_headers)+ sizeof(st_Device_status_hdd), &deviceStatusHdd_2, sizeof(st_Device_status_hdd));
	size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_hdd)*deviceStatusHeader.number;	
	P2PTunnelServer_SetStatus(buffer, size);
	*/
	
	#if 0 // test random time to close session
	int *SID = (int *)malloc(sizeof(int));
	if(SID != NULL)
	{
		*SID = sSessionInfo->nSID;
		pthread_t threadID;
		pthread_create(&threadID, NULL, &Thread_TestCloseSession, (void *)SID);
	}
	#endif
	
	return 0;
}

#if LAUNCH_PASSWORD_PROC
// Clean up Password setting process.
void clean_up_passd(int sig)
{
	printf("Clean up PasswordServer process.\n");
	waitpid(gPassPID, 0, WNOHANG);
}
#endif

int main(int argc, char *argv[])
{
	int errCode =0, retry=0;
	int updateCount = 0;
	
	// printf("---- %s ----\n", inet_ntoa(2) );
//	if(argc < 2)
	if(argc < 4)
	{
		printf("P2PTunnelServer UID ShowDebug P2PServer!\n");
		printf("P2PServer 0 = Formal Server, 1 = Test Server\n");
		return 0;
	}
	
	strcpy(gUID, argv[1]);
	int i,j;
	for(i=0;i<20;i++)
		gSID_Used[i]=0;

#if LAUNCH_PASSWORD_PROC		
	// create password process
	// note: you can launch process manually, depend on your product.
	gPassPID = fork();
	if(gPassPID==0)	//child process
	{
		printf("PasswordServer Created!!\n");
		execlp("./PasswordServer", "PasswordServer", "9000",NULL);
		exit(0);
	}
	signal(SIGCHLD, clean_up_passd);
	printf("PasswordServer PID=%d\n", gPassPID);
#endif
/*	
	if(argc >=3)
		P2PTunnel_ShowDebug(atoi(argv[2]));	

	if(argc >=4)
		p2pServer = atoi(argv[3]);
*/

	char *s = "My arg Pass to call back function";
	P2PTunnelServer_GetStatus(TunnelStatusCB, (void *)s);
	// printf("Tunnel Version[%X]\n", P2PTunnel_Version());
	printf("Tunnel Version[%X]\n", TF_P2PTunnel_Version());
	
//	P2PTunnel_ShowDebug(2) ;
	P2PTunnel_SetServer(p2pServer);
	int ret = P2PTunnelServerInitialize(20);
	if(ret < 0)
	{
		printf("P2PTunnelServerInitialize error[%d]!\n", ret);
		return -1;
	}

#define WEB_MAPPING_REMOTE_PORT		80
#define ONVIF_MAPPING_REMOTE_PORT   9221
#define RTSP_MAPPING_REMOTE_PORT	8551

	st_User_Profile userProfile;
	userProfile.httpPort   = WEB_MAPPING_REMOTE_PORT;
	userProfile.userPort_1 = ONVIF_MAPPING_REMOTE_PORT;
	userProfile.userPort_2 = RTSP_MAPPING_REMOTE_PORT;
	userProfile.userPort_3 = 8401;
	strcpy(userProfile.FirmwareVersion,argv[3]);
	strcpy(userProfile.ModalName,argv[2]);
	strcpy(userProfile.DeviceName,argv[4]);
	for (j = 0 ; j <sizeof(userProfile.Reserved) ; j++)
	{
	    userProfile.Reserved[j] = j;
	}	
	
	P2PTunnelServer_SetProfile(&userProfile);
	
	errCode = P2PTunnelServer_Start(gUID);
	if( errCode<0 )
	{
		// No Internet
		if( errCode == IOTC_ER_SERVER_NOT_RESPONSE || errCode == IOTC_ER_NETWORK_UNREACHABLE )
			printf("No Internet, error[%d]!! Reconnect after 15sec...\n", errCode);
		else
		{
			printf("P2PTunnelServer_Start error[%d]!\n", errCode);
			return -1;
		}
	}
	else
		printf("P2PTunnelServer_Start Success, I can connected by Internet.\n");
	
	/* If you don't want to use authentication mechanism, you can give NULL argument
	ret = P2PTunnelServer_GetSessionInfo(TunnelSessionInfoCB, NULL);
	*/
	ret = P2PTunnelServer_GetSessionInfo(TunnelSessionInfoCB, (void *)s);
	
	printf("Call P2PTunnelServer_GetSessionInfo ret[%d]\n", ret);
	
	while(1)
	{
		int end =0;
		
		// Retry connect to Internet every 15 sec.
		if( errCode<0 && ++retry%15==0)
		{
			errCode = P2PTunnelServer_Start(gUID);
			if( errCode<0 )
			{
				// No Internet
				if( errCode == IOTC_ER_SERVER_NOT_RESPONSE || errCode == IOTC_ER_NETWORK_UNREACHABLE)
					printf("No Internet, error[%d]!! Reconnect after 15sec...\n", errCode);
				else
				{
					printf("P2PTunnelServer_Start error[%d]!\n", errCode);
					return -1;
				}
			}
			else
				printf("P2PTunnelServer_Start Success, I can connected by Internet.\n");
			retry=0;
		}
		for(i=0; i<20; i++)
		// for(i=0; i<1; i++)
		{
			if(gSID_Used[i])
			{
				/*
				int used, available;
				P2PTunnelServer_Check_Buffer(i, 80, &used, &available);
				
				printf("Memory used = %d, available = %d\n", used, available);
				*/
				
				/*
				st_P2PTunnel_Connect_Info sessionInfo;
				
				P2PTunnelAgent_Check(gSID_Used[i].nSID, &sessionInfo);
				
				printf("----------Session(%d) Ready: %s----------\n", gSID_Used[i].nSID, (sessionInfo.bMode == 0) ? "P2P" : "RLY");
				printf("Socket : %d\n", sessionInfo.Skt);
				printf("Remote Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.RemoteAddr.sin_addr), ntohs(sessionInfo.RemoteAddr.sin_port));
				printf("My Lan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyLocalAddr.sin_addr), ntohs(sessionInfo.MyLocalAddr.sin_port));
				printf("My Wan Addr : %s:%d\n", (char*)inet_ntoa(sessionInfo.MyWanAddr.sin_addr), ntohs(sessionInfo.MyWanAddr.sin_port));
				printf("Connection time : %d second before\n", sessionInfo.ConnectTime);
				printf("I am %s\n", (sessionInfo.bCorD ==0) ? "Client" : "Device");
				printf("Connection mode: %s\n", (sessionInfo.bMode == 0) ? "P2P" : "RLY");
				printf("----------End of Session info :----------\n");
				*/
				
				/*
				int access_time = P2PTunnel_LastIOTime(i);
				if(access_time < 0)
				{
					printf("P2PTunnel_LastIOTime Error Code %d\n", access_time);
					gSID_Used[i] = 0;
				}
				else
					printf("SID %d:%u, ", i, access_time);
				end =1;
				*/
			}
		}
		
		if(errCode>=0)
		{
			/*
			printf("Begin Update\n");
			char buffer[1024];
			int size;
			long Capibility, Available;
			st_Device_status_headers deviceStatusHeader;
			st_Device_status_videoLoss deviceStatusVideoLoss;
			st_Device_status_hdd deviceStatusHdd_1, deviceStatusHdd_2;
			st_Device_status_networks deviceStatusNetwork;
			st_Device_status_info deviceStatusInfo;
			st_Device_status_reboot deviceStatusReboot_1, deviceStatusReboot_2;
			st_Device_enable_auracare deviceEnableAuracare;
			
			deviceStatusHeader.type = INFO_STATUS;
			deviceStatusHeader.number = 1;
			deviceStatusHeader.size = sizeof(st_Device_status_info)*deviceStatusHeader.number;
			strcpy(deviceStatusInfo.DeviceType,"DeviceType");
			strcpy(deviceStatusInfo.DeviceSerialNumber,"DeviceSerialNumber");
			strcpy(deviceStatusInfo.DeviceBoard,"DeviceBoard");
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusInfo, sizeof(st_Device_status_info));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_info)*deviceStatusHeader.number;
			P2PTunnelServer_SetStatus(buffer, size);
	
			deviceStatusHeader.type = REBOOT_STATUS;
			deviceStatusHeader.number = 2;
			deviceStatusHeader.size = sizeof(st_Device_status_reboot)*deviceStatusHeader.number;
			deviceStatusReboot_1.on_off = 1;
			deviceStatusReboot_1.timestamp = 1346426869;
			deviceStatusReboot_2.on_off = 0;
			deviceStatusReboot_2.timestamp = 1346436869;			
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusReboot_1, sizeof(st_Device_status_reboot));	
			memcpy(buffer+sizeof(st_Device_status_headers)+ sizeof(st_Device_status_reboot), &deviceStatusReboot_2, sizeof(st_Device_status_reboot));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_reboot)*deviceStatusHeader.number;
			P2PTunnelServer_SetStatus(buffer, size);
			
			deviceStatusHeader.type = ENABLE_AURACARE;
			deviceStatusHeader.number = 1;
			deviceStatusHeader.size = sizeof(st_Device_enable_auracare)*deviceStatusHeader.number;
			deviceEnableAuracare.on_off = 1;
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceEnableAuracare, sizeof(st_Device_enable_auracare));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_enable_auracare)*deviceStatusHeader.number;
			P2PTunnelServer_SetStatus(buffer, size);
			*/
			
			/*
			deviceStatusHeader.type = VIDEO_STATUS;
			deviceStatusHeader.number = 1;
			deviceStatusHeader.size = sizeof(st_Device_status_videoLoss)*deviceStatusHeader.number;
			deviceStatusVideoLoss.videoLoss_1 = 1;
			deviceStatusVideoLoss.videoLoss_2 = 2;
			deviceStatusVideoLoss.videoLoss_3 = 3;
			deviceStatusVideoLoss.videoLoss_4 = 4;
			deviceStatusVideoLoss.timestamp = 1346426869;
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusVideoLoss, sizeof(st_Device_status_videoLoss));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_videoLoss)*deviceStatusHeader.number;
			P2PTunnelServer_SetStatus(buffer, size);
			printf("Update Video\n");
			
			deviceStatusHeader.type = NETWORK_STATUS;
			deviceStatusHeader.number = 1;
			deviceStatusHeader.size = sizeof(st_Device_status_networks)*deviceStatusHeader.number;
			deviceStatusNetwork.linkon = 1;
			deviceStatusNetwork.ip = 4;
			deviceStatusNetwork.timestamp = 1346426869;
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusNetwork, sizeof(st_Device_status_networks));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_networks)*deviceStatusHeader.number;
			P2PTunnelServer_SetStatus(buffer, size);
			printf("Update Network\n");
			
			deviceStatusHeader.type = HDD_STATUS;
			deviceStatusHeader.number = 2;
			deviceStatusHeader.size = sizeof(st_Device_status_hdd)*deviceStatusHeader.number;
			deviceStatusHdd_1.number = 0;
			deviceStatusHdd_1.status = 1;
			deviceStatusHdd_1.flag = 2;
			deviceStatusHdd_1.timestamp = 1346426869;
			deviceStatusHdd_1.CapacityHigh = 0;
			deviceStatusHdd_1.CapacityLow = updateCount;
			deviceStatusHdd_1.AvailableHigh = 0;
			deviceStatusHdd_1.AvailableLow = 100;
			strcpy(deviceStatusHdd_1.ModalName,"abc");

			deviceStatusHdd_2.number = 3;
			deviceStatusHdd_2.status = 4;
			deviceStatusHdd_2.flag = 5;
			deviceStatusHdd_2.timestamp = 1346426869;
			deviceStatusHdd_2.CapacityHigh = 0;
			deviceStatusHdd_2.CapacityLow = updateCount;
			deviceStatusHdd_2.AvailableHigh = 0;
			deviceStatusHdd_2.AvailableLow = 300;	
			strcpy(deviceStatusHdd_2.ModalName,"cde");
			memcpy(buffer, &deviceStatusHeader, sizeof(st_Device_status_headers));
			memcpy(buffer+sizeof(st_Device_status_headers), &deviceStatusHdd_1, sizeof(st_Device_status_hdd));	
			memcpy(buffer+sizeof(st_Device_status_headers)+ sizeof(st_Device_status_hdd), &deviceStatusHdd_2, sizeof(st_Device_status_hdd));
			size = sizeof(st_Device_status_headers) + sizeof(st_Device_status_hdd)*deviceStatusHeader.number;	
			P2PTunnelServer_SetStatus(buffer, size);
			printf("Update HDD\n");
			
			updateCount++;
			
			printf("Update %d\n",updateCount);
			*/
		}
		
		if(end)
			printf("\n");
		sleep(1);
		// usleep(1000*100);
	}
	
	return 0;
}

