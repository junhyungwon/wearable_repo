/************************************************************************/
/* Copyright (c) 2007, Dreamsecurity co., ltd. All Rights Reserved.     */
/*                                                                      */
/* 본 소스코드의 일부 또는 전체를 (주)드림시큐리티의 사전 승낙 없이     */
/* 다른 프로그램이나 다른 사람에게 복사, 변경, 전재, 배포 및 다른       */
/* 컴퓨터언어로 변환 등 기타 불법적인 사용을 금합니다.                  */
/*                                                                      */
/*     회사 : (주)드림시큐리티                                          */
/*     주소 : 서울시 송파구 문정동 150-28 서경빌딩 5층                  */
/*     연락처 : (02)2233-5533                                           */
/*     개발자 : 김대식(kalacus@dreamsecurity.com)                       */
/*              장형도(hdjang@dreamsecurity.com)                        */
/************************************************************************/

/*-[ History ]------------------------------------------------------------
설명 : tester.c 

2008-06-09 : v1.2
	- 알고리즘 ID 추가 by hdjang
2008-06-09 : v1.1.1
	- 응용프로그램 무결성값 확인 함수 추가 by hdjang
2008-05-27 : v1.1
	- 무결성값 확인 함수 추가 by hdjang
2007-03-06 : v1.0
	- 생성 by hdjang
------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <malloc.h>
#endif

#include "mcapi.h"

#define MCLOG "mc.log"
int testMenu();
int testDigest();
int testMac();
int testSign();
int testAsEncrypt();
int testEncrypt();
int testGenRandom();
int testGenKeyPair();
int testDeriveKey();
int changeApimode();
void findIntegrity();
void getStatus();
int processData(char *str);
int printOutput(unsigned char *data, unsigned int datalen);
int HexToBin(unsigned char *r, char *hexstr);
static int _log(const char *path, const char *fmt, ...);

extern int testBatch();

#define TEST_CASE_DH	1
#define TEST_CASE_KDF	2

#if 0
#ifdef _WIN32
#include <windows.h>
int create_thread_with_func(LPTHREAD_START_ROUTINE thread_func, void *param)
{	
	SECURITY_ATTRIBUTES st;
	unsigned int lThreadId;
	void* pRet;
	
	st.bInheritHandle = FALSE;
	st.lpSecurityDescriptor = NULL;
	st.nLength = sizeof(st);
	pRet = CreateThread(&st, 0,thread_func,param,0,&lThreadId);
	if ( pRet == NULL )	return -1;
	return (int)pRet;
}

#include "Thread.h"
int threadRandTest()
{
	MC_RV rv = MC_OK;
	MC_ALGORITHM mcAlg = {MC_ALGID_SHA256DRBG, NULL, 0}; // MC_ALGID_FIPS186PRNG, MC_ALGID_SHA256DRBG
	MC_HSESSION hSession = 0;
	MC_UCHAR Random[1024];
	MC_INT RandBytes = 32;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}
	
	rv = MC_SetApiMode(hSession, MC_MODE_NON_KCMV);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}
	
	rv = MC_GenerateRandom(hSession, &mcAlg, Random, RandBytes);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}
	
	MC_CloseSession(hSession);

	return rv;
}
#include <crtdbg.h> 
#endif
#endif // #if 0
MC_APIMODE gApimode = MC_MODE_NON_KCMV;	/* MC_MODE_KCMV MC_MODE_NON_KCMV */

#if 0
int testFunc()
{
	MC_RV rv = MC_OK;
	MC_HSESSION hSession = 0;
	MC_ALGORITHM mcAlg = {MC_ALGID_SEED, NULL, 0};
	MC_HOBJECT hKey = 0;

	MC_UCHAR pKey[16] = "abcdabcdabcdabcd";
	int nKeyLen = 16;

	MC_UCHAR pData[4] = "abc";
	int nLen = 3;

	MC_UCHAR pOut[32] = {0,};
	int nOutLen = sizeof(pOut);

	long start=0, end = 0;
	int i = 0;
	int count = 100000;
#if 0
	// drbg
	mcAlg.mcAlgId = MC_ALGID_SHA256DRBG;
	rv = MC_OpenSession(&hSession);
	if (rv != MC_OK) goto end;

	rv = MC_GenerateRandom(hSession, &mcAlg, pOut, 32);
	if (rv != MC_OK) goto end;

	rv = MC_CloseSession(hSession);
	if (rv != MC_OK) goto end;
#endif
	/* sha256 hash */
	start = clock();
	mcAlg.mcAlgId = MC_ALGID_SHA256;
	rv = MC_OpenSession(&hSession);
	if (rv != MC_OK) goto end;

	for (i = 0; i<count; i++) 
	{
		rv = MC_DigestInit(hSession, &mcAlg);
		if (rv != MC_OK)
		{
			printf(" [%d, %d] ERR>> %s \n", __LINE__, i, (char*)MC_GetErrorString(rv));
			//return rv;
		}
		//else printf(" MC_DigestInit ,,, success ... \n");

		rv = MC_Digest(hSession, pData, nLen, &pOut, &nOutLen);
		if (rv != MC_OK)
		{
			printf(" %d] ERR>> %s \n", i, (char*)MC_GetErrorString(rv));
			//return rv;
		}
	}
	end = clock();
	printf(" duration time [count = %d] :: %-2.4f  \n", count, (double)(end-start)/CLOCKS_PER_SEC);
//	else printf(" MC_Digest ,,, success ... \n");
	printf("Output [%d] \n", nOutLen);

	for(i=0;i<nOutLen; i++) {
		printf("%02x", pOut[i]);
	}
	printf("\n");

	rv = MC_CloseSession(hSession);
	if (rv != MC_OK) goto end;

	/* seed ecb pkcs5padding */
	mcAlg.mcAlgId = MC_ALGID_SEED;
	rv = MC_OpenSession(&hSession);
	if (rv != MC_OK) goto end;

	rv = MC_SetOption(hSession, MC_ALGMODE_ECB, MC_PADTYPE_PKCS5);
	if (rv != MC_OK) goto end;

	rv = MC_CreateObject(hSession, pKey, nKeyLen, &hKey);
	if (rv != MC_OK) goto end;

	rv = MC_EncryptInit(hSession, &mcAlg, hKey);
	if (rv != MC_OK) 
		printf(" ERR>> MC_EncryptInit ... %s \n", (char*)MC_GetErrorString(rv));
	else
		printf(" MC_EncryptInit ... success \n");

	//rv = MC_DestroyObject(hSession, hKey);
	//if (rv != MC_OK) goto end;

	rv = MC_Finalize();
	if (rv != MC_OK) goto end;


	//rv = MC_CloseSession(hSession);
	//if (rv != MC_OK) goto end;


end:
	if (rv != 0)
	{
		printf(" ERR>> %s \n", (char*)MC_GetErrorString(rv));
		return rv;
	}

	rv = MC_CloseSession(hSession);


	return 0;
}

#endif

int testGenKeyPairCheck()
{
	MC_RV rv = MC_OK;
	MC_HSESSION hSession = 0;
	MC_ALGORITHM mcAlg = {MC_ALGID_RSA2048P2V2, NULL, 0};
	MC_HOBJECT hPubKey, hPriKey;

	int i = 0;
	long start=0, end=0;

	MC_UCHAR pt[256] = "abcdabcdabcdabcd";
	MC_UINT  ptlen = sizeof(pt);

	MC_UCHAR out[256] = {0,};
	MC_UINT outlen = sizeof(out);

	rv = MC_OpenSession(&hSession);
	if (rv != MC_OK) goto end;

	printf(" Generate Key Pair (RSA 2048 v2) ... start \n");

	mcAlg.mcAlgId = MC_ALGID_RSA2048P2V2;
	
	for (i=0;i<10;i++) {
	start = clock();
	rv = MC_GenerateKeyPair(hSession, &mcAlg, &hPubKey, &hPriKey);
	end = clock();
	if (rv != MC_OK) goto end;

	printf(" GenKeyPair time [%2d] : %02.4f \n", (i+1), (double)(end-start)/CLOCKS_PER_SEC);
	}

	for (i=0;i<10;i++) 
	{
		mcAlg.mcAlgId = MC_ALGID_SHA256WithRSAOAEP;

		start = clock();
		rv = MC_EncryptInit(hSession, &mcAlg, hPubKey);
		if (rv != MC_OK) goto end;

		ptlen = 16;
		rv = MC_Encrypt(hSession, pt, ptlen, out, &outlen);
		if (rv != MC_OK) goto end;

		end = clock();

		printf(" RSA OAEP Encrypt time [%02d] : %2.4f \n", (i+1), (double)(end-start)/CLOCKS_PER_SEC);



		start = clock();
		rv = MC_DecryptInit(hSession, &mcAlg, hPriKey);
		if (rv != MC_OK) goto end;

		ptlen = 256;
		rv = MC_Decrypt(hSession, out, outlen, pt, &ptlen);
		if (rv != MC_OK) goto end;
		end = clock();
		printf(" RSA OAEP Decrypt time [%02d] : %2.4f \n", (i+1), (double)(end-start)/CLOCKS_PER_SEC);
	}

end:
	if (rv != MC_OK)
	{
		printf(" ERR>> %s \n", (char*)MC_GetErrorString(rv));
		return rv;
	}
	rv = MC_CloseSession(hSession);

	return 0;
}

int main(int argc, char* argv[])
{
	MC_RV rv = MC_OK;

	rv = MC_Initialize(NULL);
	if(rv != MC_OK) printf("%s\n", MC_GetErrorString(rv));

	if(argc > 1 && (!strcmp(argv[1], "/b") || !strcmp(argv[1], "-b")))
	{
		testBatch();
	}
	else
		testMenu();

	rv = MC_Finalize();
	if(rv != MC_OK) printf("%s\n", MC_GetErrorString(rv));

	getStatus();

	return 0;
}

#if defined(_WIN32)
#define flushing() fflush(stdin);
#include <windows.h>
#else
#define flushing() {int c; while((c=getchar()) != '\n' && c != EOF);}
#endif

int testMenu()
{
	int testId;
	MC_VERSION mcVer;
	MC_RV rv = MC_OK;

	MC_GetVersion((MC_VERSION*)&mcVer);
	printf("===============================================================================\n");
	printf("          Dreamsecurity %s API Ver.%d.%d.%d Tester \n", mcVer.name, mcVer.major, mcVer.minor, mcVer.release);
	printf("===============================================================================\n\n");

	while(1)
	{
		printf(" 1. Selftest\n");
		printf(" 2. Message Digest\n");
		printf(" 3. Sign\n");
		printf(" 4. Encrypt\n");
		printf(" 5. Asymmetric Encrypt\n");
		printf(" 6. MAC\n");
		printf(" 7. Generate Random\n");
		printf(" 8. Generate KeyPair\n");
		printf(" 9. Key Derive \n");
		printf("10. API mode\n");
		printf("11. Integrity Value\n");
		printf("12. Get Status \n");
		printf("13. Batch test \n");
		printf(" 0. Exit\n");
		printf("Select : ");
		testId = -1;
		scanf("%d", &testId);

		switch(testId)
		{
		case 1:	printf("\n"); 
			rv = MC_Selftest(); printf("\n");	
			if (rv !=0) {printf("%s\n", MC_GetErrorString(rv)); exit(rv);}break;
		case 2:	rv = testDigest();		break;
		case 3:	rv = testSign();		break;
		case 4:	rv = testEncrypt();		break;
		case 5:	rv = testAsEncrypt();	break;
		case 6:	rv = testMac();			break;
		case 7:	rv = testGenRandom();	break;
		case 8:	rv = testGenKeyPair();	break;
		case 9:	rv = testDeriveKey();	break;
		case 10:rv = changeApimode();	break;
		case 11: findIntegrity();		break;
		case 12: getStatus();			break;
		case 13: testBatch();			break;
		case 0:	return rv;
		default:
			printf("Invalid selection!\n\n");
			flushing();
			continue;
		}
	}
}

typedef struct {
	int n;
	char *s;
} menus;

#define SHOW_MENU(M, A) if(!show_menu(M, (int*)A)) break;
int show_menu(menus *menu, int *sel)
{
	int i;
	for(;;) {
		printf("\n");
		for(i=0; menu[i].n !=0; i++)
			printf("%d. %s\n", i+1, menu[i].s);
		printf("0. Main menu\n");
		printf("Select : ");
		scanf("%d", sel);

		if(*sel == 0) {
			printf("\n");
			return 0;
		}
		else if(*sel > 0 && *sel <= i) {
			*sel = menu[*sel-1].n;
			return *sel;
		}

		printf("Invalid selection!\n");
		flushing();
	}
}

#define inputData(T,D,L) { \
	MC_CHAR ch; \
	flushing(); \
	memset(D, 0, sizeof(D)); \
	printf(T); \
	for(L=0; (ch=getchar())!='\n'; L++) D[L] = ch; D[L+1] = 0; \
}

int testDigest()
{
	MC_RV rv;
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HSESSION hSession = 0, hNew = 0;
	MC_UCHAR data[4096], out[64];
	MC_UINT datalen, outlen;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	rv = MC_GetApiMode(hSession, &gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 3;}
	else printf("Current API mode : %s\n", gApimode?"KCMV":"NON-KCMV");
	
	while(1)
	{
		menus menu[] = {
			MC_ALGID_MD5, "MC_ALGID_MD5", 
			MC_ALGID_SHA1, "MC_ALGID_SHA1", 
			MC_ALGID_SHA256, "MC_ALGID_SHA256", 
			MC_ALGID_SHA512, "MC_ALGID_SHA512", 
			MC_ALGID_SHA224, "MC_ALGID_SHA224", 
			MC_ALGID_SHA384, "MC_ALGID_SHA384", 
			MC_ALGID_LSH224, "MC_ALGID_LSH224", 
			MC_ALGID_LSH256, "MC_ALGID_LSH256", 
			MC_ALGID_LSH384, "MC_ALGID_LSH384", 
			MC_ALGID_LSH512, "MC_ALGID_LSH512", 
			MC_ALGID_HAS160, "MC_ALGID_HAS160", 
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		inputData("Message : ", data, datalen);
		datalen = processData((char*)data);

		outlen = sizeof(out);
		memset(out, 0, outlen);

		rv = MC_DigestInit(hSession, &mcAlg);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		rv = MC_Digest(hSession, data, datalen, out, &outlen);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		printf("Digest(%d) : \n", outlen); 
		printOutput(out, outlen);
	}

	MC_CloseSession(hSession);

	return 0;
}

int testMac()
{
	MC_RV rv;
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hKey = 0;
	MC_HSESSION hSession = 0;
	MC_UCHAR data[4096], out[64], key[256], tmp[32], nonce[32], adata[32];
	MC_UINT datalen, outlen, keylen, tmplen;
	MC_ALGPARAM param = {NULL, NULL, 0,0,0,0};

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	while(1)
	{
		menus menu[] = {
			MC_ALGID_MD5_HMAC, "MC_ALGID_MD5_HMAC", 
			MC_ALGID_SHA1_HMAC, "MC_ALGID_SHA1_HMAC", 
			MC_ALGID_SHA256_HMAC, "MC_ALGID_SHA256_HMAC", 
			MC_ALGID_SHA384_HMAC, "MC_ALGID_SHA384_HMAC", 
			MC_ALGID_SHA512_HMAC, "MC_ALGID_SHA512_HMAC", 
			/*MC_ALGID_DESCBC_MAC, "MC_ALGID_DESCBC_MAC", */
			MC_ALGID_SEED_GMAC, "MC_ALGID_SEED_GMAC",
			MC_ALGID_ARIA_128BITKEY_GMAC, "MC_ALGID_ARIA_128BITKEY_GMAC",
			MC_ALGID_ARIA_192BITKEY_GMAC, "MC_ALGID_ARIA_192BITKEY_GMAC",
			MC_ALGID_ARIA_256BITKEY_GMAC, "MC_ALGID_ARIA_256BITKEY_GMAC",
			MC_ALGID_AES_128BITKEY_GMAC, "MC_ALGID_AES_128BITKEY_GMAC",
			MC_ALGID_AES_192BITKEY_GMAC, "MC_ALGID_AES_192BITKEY_GMAC",
			MC_ALGID_AES_256BITKEY_GMAC, "MC_ALGID_AES_256BITKEY_GMAC",
			MC_ALGID_LEA_128BITKEY_GMAC, "MC_ALGID_LEA_128BITKEY_GMAC",
			MC_ALGID_LEA_192BITKEY_GMAC, "MC_ALGID_LEA_192BITKEY_GMAC",
			MC_ALGID_LEA_256BITKEY_GMAC, "MC_ALGID_LEA_256BITKEY_GMAC",
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		inputData("MAC key [Gen]: ", key, keylen);
		keylen = processData((char*)key);
		if(keylen == 0) {
			//MC_GenerateKey(hSession, &mcAlg, &hKey);

			keylen = sizeof(key);
			{
				MC_ALGORITHM mcRandAlg = {MC_ALGID_SHA256DRBG, NULL, 0};
				MC_GenerateRandom(hSession, &mcRandAlg, key, keylen);
				MC_CreateObject(hSession, key, keylen, &hKey);

			}

			MC_GetObjectValue(hSession, hKey, key, &keylen);
			printf("MAC key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		else
			MC_CreateObject(hSession, key, keylen, &hKey);

		inputData("Message : ", data, datalen);
		datalen = processData((char*)data);

		if (mcAlg.mcAlgId >= MC_ALGID_SEED_GMAC) 
		{
			MC_ALGORITHM randAlg = {MC_ALGID_SHA256DRBG, NULL, 0};
			tmplen = 12;
			MC_GenerateRandom(hSession, &randAlg, nonce, tmplen);

			param.pNonce = nonce;
			param.nNonce = tmplen;

			tmplen = 16;
			MC_GenerateRandom(hSession, &randAlg, adata, tmplen);

			param.pAData = adata;
			param.nAData = tmplen;
			param.nTLen = 16;
			param.nDataLen = datalen;

			mcAlg.pParam = (MC_UCHAR*)&param;
			mcAlg.nParam = sizeof(param);
		}

		outlen = sizeof(out);
		memset(out, 0, outlen);

		rv = MC_CreateMacInit(hSession, &mcAlg, hKey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		rv = MC_CreateMac(hSession, data, datalen, out, &outlen);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
		
		printf("MAC(%d) : \n", outlen); 
		printOutput(out, outlen);

		rv = MC_VerifyMacInit(hSession, &mcAlg, hKey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		rv = MC_VerifyMac(hSession, data, datalen, out, outlen);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
		else printf("Verify Success\n");
	}

	MC_DestroyObject(hSession, hKey);
	MC_CloseSession(hSession);

	return 0;
}

int testSign()
{
	MC_RV rv;
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0}, mcKeyAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hPubkey = 0, hPrikey = 0;
	MC_HSESSION hSession = 0;
	MC_UCHAR data[4096] = {0,}, out[1024]= {0,}, key[4096]= {0,}, param[2048]= {0,}, tmp[2048]= {0,};
	MC_UINT datalen, outlen, keylen, paramlen, tmplen;
	MC_CHAR ch;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	rv = MC_GetApiMode(hSession, &gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 3;}
	else printf("Current API mode : %s\n", gApimode?"KCMV":"NON-KCMV");
	while(1)
	{
		/*MC_ALGID_SHA1WithECDSA, "MC_ALGID_SHA1WithECDSA", */

		menus menu[] = {
			MC_ALGID_SHA1WithRSA, "MC_ALGID_SHA1WithRSA", 
			MC_ALGID_SHA256WithRSA, "MC_ALGID_SHA256WithRSA", 
			MC_ALGID_SHA384WithRSA, "MC_ALGID_SHA384WithRSA", 
			MC_ALGID_SHA512WithRSA, "MC_ALGID_SHA512WithRSA", 
			MC_ALGID_SHA1WithRSAPSS, "MC_ALGID_SHA1WithRSAPSS", 
			MC_ALGID_SHA256WithRSAPSS, "MC_ALGID_SHA256WithRSAPSS", 
			MC_ALGID_SHA384WithRSAPSS, "MC_ALGID_SHA384WithRSAPSS", 
			MC_ALGID_SHA512WithRSAPSS, "MC_ALGID_SHA512WithRSAPSS", 
			MC_ALGID_SHA1WithKCDSA, "MC_ALGID_SHA1WithKCDSA", 
			MC_ALGID_SHA256WithKCDSA, "MC_ALGID_SHA256WithKCDSA", 
			MC_ALGID_HAS160WithKCDSA, "MC_ALGID_HAS160WithKCDSA", 
			MC_ALGID_SHA1WithECDSA_B163_3, "MC_ALGID_SHA1WithECDSA_B163_3", 
			MC_ALGID_SHA1WithECDSA_B163_5, "MC_ALGID_SHA1WithECDSA_B163_5", 
			MC_ALGID_SHA256WithECDSA_P224_12, "MC_ALGID_SHA256WithECDSA_P224_12", 
			MC_ALGID_SHA256WithECDSA_P256_r1, "MC_ALGID_SHA256WithECDSA_P256_r1", 
			MC_ALGID_SHA384WithECDSA_P256_r1, "MC_ALGID_SHA384WithECDSA_P256_r1", 
			MC_ALGID_SHA512WithECDSA_P256_r1, "MC_ALGID_SHA512WithECDSA_P256_r1", 
			MC_ALGID_SHA256WithECDSA_P256_k1, "MC_ALGID_SHA256WithECDSA_P256_k1", 
			MC_ALGID_SHA384WithECDSA_P256_k1, "MC_ALGID_SHA384WithECDSA_P256_k1", 
			MC_ALGID_SHA512WithECDSA_P256_k1, "MC_ALGID_SHA512WithECDSA_P256_k1", 
			MC_ALGID_SHA256WithECDSA_B283_r1, "MC_ALGID_SHA256WithECDSA_B283_r1", 
			MC_ALGID_SHA384WithECDSA_B283_r1, "MC_ALGID_SHA384WithECDSA_B283_r1", 
			MC_ALGID_SHA512WithECDSA_B283_r1, "MC_ALGID_SHA512WithECDSA_B283_r1", 
			MC_ALGID_SHA256WithECDSA_B283_k1, "MC_ALGID_SHA256WithECDSA_B283_k1", 
			MC_ALGID_SHA384WithECDSA_B283_k1, "MC_ALGID_SHA384WithECDSA_B283_k1", 
			MC_ALGID_SHA512WithECDSA_B283_k1, "MC_ALGID_SHA512WithECDSA_B283_k1", 
			MC_ALGID_SHA256WithECDSA_P384_r1, "MC_ALGID_SHA256WithECDSA_P384_r1", 
			MC_ALGID_SHA384WithECDSA_P384_r1, "MC_ALGID_SHA384WithECDSA_P384_r1", 
			MC_ALGID_SHA512WithECDSA_P384_r1, "MC_ALGID_SHA512WithECDSA_P384_r1", 
			MC_ALGID_SHA256WithECDSA_P521_r1, "MC_ALGID_SHA256WithECDSA_P521_r1", 
			MC_ALGID_SHA384WithECDSA_P521_r1, "MC_ALGID_SHA384WithECDSA_P521_r1", 
			MC_ALGID_SHA512WithECDSA_P521_r1, "MC_ALGID_SHA512WithECDSA_P521_r1", 
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		memset(data, 0x00, sizeof(data));
		memset(out, 0x00, sizeof(out));
		memset(key, 0x00, sizeof(key));
		memset(param, 0x00, sizeof(param));
		memset(tmp, 0x00, sizeof(tmp));
		mcAlg.pParam = NULL;	mcAlg.nParam = 0; 
		mcKeyAlg.pParam = NULL;	mcKeyAlg.nParam = 0;

		datalen = outlen = keylen = paramlen = tmplen = 0;

		inputData("Message : ", data, datalen);
		datalen = processData((char*)data);

		inputData("Private key [Gen]: ", key, keylen);
		keylen = processData((char*)key);
		if(keylen == 0) 
		{
			if(mcAlg.mcAlgId == MC_ALGID_SHA1WithRSA || mcAlg.mcAlgId == MC_ALGID_SHA256WithRSA || mcAlg.mcAlgId == MC_ALGID_SHA384WithRSA || mcAlg.mcAlgId == MC_ALGID_SHA512WithRSA ||
				mcAlg.mcAlgId == MC_ALGID_SHA1WithRSAPSS || mcAlg.mcAlgId == MC_ALGID_SHA256WithRSAPSS || mcAlg.mcAlgId == MC_ALGID_SHA384WithRSAPSS || mcAlg.mcAlgId == MC_ALGID_SHA512WithRSAPSS) 
				mcKeyAlg.mcAlgId = MC_ALGID_RSA2048;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA1WithECDSA_B163_3 || mcAlg.mcAlgId == MC_ALGID_SHA1WithECDSA) mcKeyAlg.mcAlgId = MC_ALGID_EC_B163_3;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA1WithECDSA_B163_5) mcKeyAlg.mcAlgId = MC_ALGID_EC_B163_5;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_P224_12) mcKeyAlg.mcAlgId = MC_ALGID_EC_P224_12;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_P256_r1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_P256_r1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_P256_r1) 
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P256_r1;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_P256_k1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_P256_k1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_P256_k1) 
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P256_k1;
			else if (mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_B283_r1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_B283_r1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_B283_r1)
				mcKeyAlg.mcAlgId = MC_ALGID_EC_B283_r1;
			else if (mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_B283_k1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_B283_k1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_B283_k1)
				mcKeyAlg.mcAlgId = MC_ALGID_EC_B283_k1;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_P384_r1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_P384_r1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_P384_r1) 
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P384_r1;
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithECDSA_P521_r1 || mcAlg.mcAlgId == MC_ALGID_SHA384WithECDSA_P521_r1 || mcAlg.mcAlgId == MC_ALGID_SHA512WithECDSA_P521_r1) 
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P521_r1;
			else if(mcAlg.mcAlgId == MC_ALGID_HAS160WithKCDSA || mcAlg.mcAlgId == MC_ALGID_SHA1WithKCDSA) {
				mcKeyAlg.mcAlgId = MC_ALGID_KCDSAWithParam;
				paramlen = HexToBin(param, "3082011f02818100940f2886f849956805be4d218f448ab5aa0877beb6a54c783676dcca03aae5f9ff6de10b13c0c30902053910a333d6c07a7a1652ffd3ff7576f8c1943f42950b968b889a31cef1184d1d17fde6a54b03c303155d4e3b7eaa4ed8605388c2a94ebd824cb0d0fba7f6c8583daad73e5e07ec4198a7f8499daf04474d590460cda3021500f16ef4500e045f30ca486848b02e9a0ebdbcb44b028181008c7c44bbcf8c1cd086c7be59f4aa45e07e53c34303e7bbee8e6006729ef2ff7728e693afb66f00ce7dcdad5f5ba1d4b9d93b03553d2f2cecf34ec084a8f5fc6ef44c3119bec4baff8fa989af3e87034dfd61e4e9ceab6331d628478244795924c25781ecf0f8ee5ce8118b8f69a321ccfec99629c067f9ba3225282abcf2e32a");
				mcKeyAlg.pParam = param;
				mcKeyAlg.nParam = paramlen;
			}
			else if(mcAlg.mcAlgId == MC_ALGID_SHA256WithKCDSA) {
				mcKeyAlg.mcAlgId = MC_ALGID_KCDSAWithParam;
				paramlen = HexToBin(param, "3082022D0282010100B934A7B67EE1E104461E493D03D73C2038532D54EEDA7F67EA52C72301CB965CEAE63F5E4498A2B5FB535751B70C7F4E526432A5331AE2A16B3B7E578A731ECCFF44D3A638C7D2B6AA619ABA1FDD6707FE97DE6765880AC9C57C2566B97A1D466A227B02A53A556B1B9A6CF698633DBA31760826E3443DB6FD3DD2CE707DCE001640228A9569D0CFA55DEA0A9D86B112B1270705DCB90DFC386B8334612CE4A94D15FD66A0156D0FE419B64136C82CB24437F31DD7F35CD429F809F0C7BF0B15CBEE1AE5BB27C1BC7221D6587DCE8F2F5C3B458BB61934C574808E1DEB9619743DDD86069BF68C6F3A9D24C4346567394C1882ECA4E9DA2279E0176DC806BEDB022100B1F757A332BB73D5DC784E54EC983ACBA61F8C7A20F79A7A0CB6547E5C42D77B028201010083B26D4D26AD9DBE251BBA23AF58A7D4A5FE431B22A601DA030771847729707A74E42BC121D917E9B94927E6C6AF35F57EA03121ED561ACFAD06C15D777CC18143EEE1E3C2015177B184F8EACE75FAC7B40FF1E45812F59FCAE49638F4595682BEE21EDB617883E4CE8178E73A26E7C6DA40FDCC803685330DF9119C3D1DE036A9022FA096460CD8D80E31756EC9288236B0BEBB879580D53539CA610777AB17790A5428F816AD28F5F48167C4338D03A515017D2759104A53674EDB13C4AB155E3C7907688B4FC5FB83231B632FFB04EDDD13057F5DB534970089DCA1C30F74C55DDE6C9EB7A1993E99E31E8D9DD762A21BC65D51784853CC84B971103263A2");
				mcKeyAlg.pParam = param;
				mcKeyAlg.nParam = paramlen;
			}
			else
				mcKeyAlg.mcAlgId = mcAlg.mcAlgId;
			rv = MC_GenerateKeyPair(hSession, &mcKeyAlg, &hPubkey, &hPrikey);
			if(rv != MC_OK) {printf("[%d], %s\n", __LINE__, MC_GetErrorString(rv)); break;}

			keylen = sizeof(key);
			MC_GetObjectValue(hSession, hPrikey, key, &keylen);
			printf("Private key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		else
			MC_CreateObject(hSession, key, keylen, &hPrikey);

		outlen = sizeof(out);
		memset(out, 0, outlen);

		/* pss salt length */
		//mcAlg.pParam = "helloworld";
		//mcAlg.nParam = 32;

		rv = MC_SignInit(hSession, &mcAlg, hPrikey);
		if(rv != MC_OK) {printf("[%d], %s\n", __LINE__, MC_GetErrorString(rv)); break;}
		
		rv = MC_Sign(hSession, data, datalen, out, &outlen);
		if(rv != MC_OK) {printf("[%d], %s\n", __LINE__, MC_GetErrorString(rv)); break;}

		printf("Sign(%02d) : \n", outlen);
		printOutput(out, outlen);

		printf("New sign value for verify [y/N]? "); ch = getchar();
		if(!memcmp(&ch, (char*)"y", 1))
		{
			inputData("Sign : ", out, outlen);
			outlen = processData((char*)out);
		}

		inputData("Public key [Gen]: ", key, keylen);
		keylen = processData((char*)key);

		if(keylen != 0) 
			MC_CreateObject(hSession, key, keylen, &hPubkey);
		else {
			keylen = sizeof(key);
			MC_GetObjectValue(hSession, hPubkey, key, &keylen);
			printf("Public key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		
		if(mcAlg.mcAlgId == MC_ALGID_HAS160WithKCDSA || mcAlg.mcAlgId == MC_ALGID_SHA1WithKCDSA || mcAlg.mcAlgId == MC_ALGID_SHA256WithKCDSA )
		{
			inputData("Parameter [Gen]: ", tmp, tmplen);
			tmplen = processData((char*)tmp);
			if(tmplen != 0) {
				mcAlg.pParam = tmp;
				mcAlg.nParam = tmplen;
			}
			else {
				mcAlg.pParam = mcKeyAlg.pParam;
				mcAlg.nParam = mcKeyAlg.nParam;
				printf("Parameter(%d) : \n", mcKeyAlg.nParam); 
				printOutput(mcKeyAlg.pParam, mcKeyAlg.nParam);
			}
		}

		rv = MC_VerifyInit(hSession, &mcAlg, hPubkey);
		if(rv != MC_OK) {printf("[%d], %s\n", __LINE__, MC_GetErrorString(rv)); break;}
		
		rv = MC_Verify(hSession, data, datalen, out, outlen);
		if(rv != MC_OK) {printf("[%d], %s\n", __LINE__, MC_GetErrorString(rv)); break;}
		else printf("Verify Success\n");
	}

	MC_DestroyObject(hSession, hPubkey);
	MC_DestroyObject(hSession, hPrikey);
	MC_CloseSession(hSession);

	return 0;
}

int testEncrypt()
{
	MC_RV rv;
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hKey = 0;
	MC_HSESSION hSession = 0;
	MC_ALGMODE algMode = MC_ALGMODE_CBC;
	MC_PADTYPE algPad = MC_PADTYPE_PKCS5;
	MC_UCHAR data[4096], cipher[4112], plain[4096], key[64], iv[64], ad[256];
	MC_UINT datalen, cipherlen, plainlen, keylen = 32, ivlen = 32, adlen = 256, offset = 0;
	MC_ALGORITHM prng = {MC_ALGID_SHA256DRBG, (MC_UCHAR*)"getRandom", 9};
	MC_ALGPARAM param;
	MC_CHAR ch;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	rv = MC_GetApiMode(hSession, &gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 3;}
	else printf("Current API mode : %s\n", gApimode?"KCMV":"NON-KCMV");

	while(1)
	{
		menus menu[] = {
			MC_ALGID_3DES_3KEY, "MC_ALGID_3DES_3KEY", 
			/*MC_ALGID_DES, "MC_ALGID_DES", */
			MC_ALGID_SEED, "MC_ALGID_SEED", 
			MC_ALGID_ARIA_128BITKEY, "MC_ALGID_ARIA_128BITKEY", 
			MC_ALGID_ARIA_192BITKEY, "MC_ALGID_ARIA_192BITKEY", 
			MC_ALGID_ARIA_256BITKEY, "MC_ALGID_ARIA_256BITKEY", 
			MC_ALGID_AES_128BITKEY, "MC_ALGID_AES_128BITKEY", 
			MC_ALGID_AES_192BITKEY, "MC_ALGID_AES_192BITKEY", 
			MC_ALGID_AES_256BITKEY, "MC_ALGID_AES_256BITKEY", 
			MC_ALGID_LEA_128BITKEY, "MC_ALGID_LEA_128BITKEY", 
			MC_ALGID_LEA_192BITKEY, "MC_ALGID_LEA_192BITKEY", 
			MC_ALGID_LEA_256BITKEY, "MC_ALGID_LEA_256BITKEY", 
			MC_ALGID_RC2_40BITKEY, "MC_ALGID_RC2_40BITKEY", 
			MC_ALGID_RC2_128BITKEY, "MC_ALGID_RC2_128BITKEY", 
			MC_ALGID_RC2_192BITKEY, "MC_ALGID_RC2_192BITKEY", 
			MC_ALGID_RC2_256BITKEY, "MC_ALGID_RC2_256BITKEY", 
			MC_ALGID_RC4, "MC_ALGID_RC4", 
			MC_ALGID_HIGHT, "MC_ALGID_HIGHT", 
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);
		flushing();

		printf("\n1. MC_ALGMODE_CBC\n");
		printf("2. MC_ALGMODE_ECB\n");
		printf("3. MC_ALGMODE_CTR\n");
		printf("4. MC_ALGMODE_GCM\n");
		printf("5. MC_ALGMODE_CCM\n");
		printf("Select [1]: "); ch = getchar();
		if(!memcmp(&ch, (char*)"2", 1)) algMode = MC_ALGMODE_ECB;
		else if(!memcmp(&ch, (char*)"3", 1)) algMode = MC_ALGMODE_CTR;
		else if(!memcmp(&ch, (char*)"4", 1)) algMode = MC_ALGMODE_GCM;
		else if(!memcmp(&ch, (char*)"5", 1)) algMode = MC_ALGMODE_CCM;
		else algMode = MC_ALGMODE_CBC;
		flushing();

		printf("\n1. MC_PADTYPE_PKCS5\n");
		printf("2. MC_PADTYPE_ONE\n");
		printf("3. MC_PADTYPE_NONE\n");
		printf("4. MC_PADTYPE_SSL\n");
		printf("5. MC_PADTYPE_X923\n");
		printf("Select [1]: "); ch = getchar();
		if(!memcmp(&ch, (char*)"2", 1)) algPad = MC_PADTYPE_ONE;
		else if(!memcmp(&ch, (char*)"3", 1)) algPad = MC_PADTYPE_NONE;
		else if(!memcmp(&ch, (char*)"4", 1)) algPad = MC_PADTYPE_SSL;
		else if(!memcmp(&ch, (char*)"5", 1)) algPad = MC_PADTYPE_X923;
		else algPad = MC_PADTYPE_PKCS5;
		flushing();
		
		rv = MC_SetOption(hSession, algMode, algPad);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

		inputData("Message : ", data, datalen);
		datalen = processData((char*)data);

		inputData("Key [Gen]: ", key, keylen);
		keylen = processData((char*)key);
		if(keylen == 0) {
			rv = MC_GenerateKey(hSession, &mcAlg, &hKey);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

			keylen = sizeof(key);
			MC_GetObjectValue(hSession, hKey, key, &keylen);
			printf("Key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		else
			MC_CreateObject(hSession, key, keylen, &hKey);

		mcAlg.pParam = 0;
		mcAlg.nParam = 0;
		if(algMode != MC_ALGMODE_ECB)
		{
			inputData("Iv [Gen]: ", iv, ivlen);
			ivlen = processData((char*)iv);
			if(ivlen == 0) 
			{
				if(mcAlg.mcAlgId == MC_ALGID_3DES_3KEY || mcAlg.mcAlgId == MC_ALGID_HIGHT ) ivlen = 8;
				else ivlen = 16;
				if(algMode == MC_ALGMODE_CCM)
					ivlen = 12;
				rv = MC_GenerateRandom(hSession, &prng, iv, ivlen);
				if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

				printf("Iv(%d) : \n", ivlen); 
				printOutput(iv, ivlen);
			}
			mcAlg.pParam = iv;
			mcAlg.nParam = ivlen;
		}

		if(algMode == MC_ALGMODE_GCM || algMode == MC_ALGMODE_CCM)
		{
			param.pNonce = iv;
			param.nNonce = ivlen;
			param.pAData = ad;
			param.nAData = adlen;
			param.nTLen = 16;
			param.nDataLen = datalen;

			mcAlg.pParam = (MC_UCHAR*)&param;
			mcAlg.nParam = sizeof(param);
		}

		
		cipherlen = sizeof(cipher);
		memset(cipher, 0, cipherlen);

		rv = MC_EncryptInit(hSession, &mcAlg, hKey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		if(1)
		{
 			rv = MC_Encrypt(hSession, data, datalen, cipher, &cipherlen);
 			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
		}
		else
		{
			MC_UCHAR *pCipher = cipher;

			rv = MC_EncryptUpdate(hSession, data, datalen, pCipher, &cipherlen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

			pCipher += cipherlen;
			cipherlen = sizeof(cipher) - cipherlen;

			rv = MC_EncryptFinal(hSession, pCipher, &cipherlen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

			cipherlen = (pCipher - cipher) + cipherlen;
		}
		
		printf("Ciphertext(%d) : \n", cipherlen);
		printOutput(cipher, cipherlen);

		printf("New ciphertext for decrypt [y/N]? "); ch = getchar();
		if(!memcmp(&ch, (char*)"y", 1))
		{
			inputData("Ciphertext : ", cipher, cipherlen);
			cipherlen = processData((char*)cipher);
		}

		plainlen = sizeof(plain);
		memset(plain, 0, plainlen);
		
		rv = MC_DecryptInit(hSession, &mcAlg, hKey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		if(1)
		{
			rv = MC_Decrypt(hSession, cipher, cipherlen, plain, &plainlen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
		}
		else
		{
			MC_UCHAR *pPlain = plain;
			rv = MC_DecryptUpdate(hSession, cipher, cipherlen, pPlain, &plainlen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
			
			pPlain += plainlen;
			plainlen = sizeof(plain) - plainlen;

			rv = MC_DecryptFinal(hSession, pPlain, &plainlen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

			plainlen = (pPlain - plain) + plainlen;
		}

		printf("Plaintext(%d) : \n", plainlen);
		printOutput(plain, plainlen);
	}

	MC_DestroyObject(hSession, hKey);
	MC_CloseSession(hSession);

	return 0;
}

int testAsEncrypt()
{
	MC_RV rv;
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0}, mcKeyAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hPubkey = 0, hPrikey = 0;
	MC_HSESSION hSession = 0;
	MC_UCHAR data[512], cipher[512], plain[512], key[2048];
	MC_UINT datalen, cipherlen, plainlen, keylen;
	MC_CHAR ch;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	while(1)
	{
		menus menu[] = {
			MC_ALGID_SHA1WithRSAOAEP | MC_KCMV, "MC_ALGID_SHA1WithRSAOAEP", 
			MC_ALGID_SHA256WithRSAOAEP, "MC_ALGID_SHA256WithRSAOAEP", 
			MC_ALGID_RSA, "MC_ALGID_RSA", 
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		inputData("Message : ", data, datalen);
		datalen = processData((char*)data);

		inputData("Public key [Gen]: ", key, keylen);
		keylen = processData((char*)key);
		if(keylen == 0) 
		{
			//if(mcAlg.mcAlgId == MC_ALGID_SHA1WithRSAOAEP  | MC_KCMV ||
			//	mcAlg.mcAlgId == MC_ALGID_SHA256WithRSAOAEP) mcKeyAlg.mcAlgId = MC_ALGID_RSA2048;
			//else if(mcAlg.mcAlgId == MC_ALGID_*/RSA) mcKeyAlg.mcAlgId = MC_ALGID_RSA2048;
			mcKeyAlg.mcAlgId = MC_ALGID_RSA2048;
			rv = MC_GenerateKeyPair(hSession, &mcKeyAlg, &hPubkey, &hPrikey);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

			keylen = sizeof(key);
			MC_GetObjectValue(hSession, hPubkey, key, &keylen);
			printf("Public key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		else
			MC_CreateObject(hSession, key, keylen, &hPubkey);

		cipherlen = sizeof(cipher);
		memset(cipher, 0, cipherlen);

		rv = MC_EncryptInit(hSession, &mcAlg, hPubkey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		rv = MC_Encrypt(hSession, data, datalen, cipher, &cipherlen);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		printf("Ciphertext(%d) : \n", cipherlen);
		printOutput(cipher, cipherlen);

		printf("New Ciphertext for decrypt [y/N]? "); ch = getchar();
		if(!memcmp(&ch, (char*)"y", 1))
		{
			inputData("Ciphertext : ", cipher, cipherlen);
			cipherlen = processData((char*)cipher);
		}

		inputData("Private key [Gen]: ", key, keylen);
		keylen = processData((char*)key);
		if(keylen != 0)
			MC_CreateObject(hSession, key, keylen, &hPrikey);
		else {
			keylen = sizeof(key);
			MC_GetObjectValue(hSession, hPrikey, key, &keylen);
			printf("Private key(%d) : \n", keylen); 
			printOutput(key, keylen);
		}
		
		plainlen = sizeof(plain);
		memset(plain, 0, plainlen);
		
		rv = MC_DecryptInit(hSession, &mcAlg, hPrikey);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		rv = MC_Decrypt(hSession, cipher, cipherlen, plain, &plainlen);
		if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}

		printf("Plaintext(%d) : \n", plainlen);
		printOutput(plain, plainlen);
	}

	MC_DestroyObject(hSession, hPubkey);
	MC_DestroyObject(hSession, hPrikey);
	MC_CloseSession(hSession);

	return 0;
}

int wrap(MC_HOBJECT hSession, unsigned char *pPassword, MC_HOBJECT hKey, MC_UCHAR *pWrapedkey, MC_UINT *nWrapedkeylen)
{
	MC_RV rv;	
	MC_ALGORITHM mcHashAlg = {MC_ALGID_SHA256, NULL, 0};
	MC_ALGORITHM mcEncAlg = {MC_ALGID_ARIA_128BITKEY, NULL, 0};

	MC_UCHAR param[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
	MC_UCHAR buf[32], tmp[2048];
	MC_UINT buflen = sizeof(buf), tmplen = sizeof(tmp);
	int i, count = 1024;
	MC_UINT len = sizeof(param);

	MC_HOBJECT hEncKey;

	rv = MC_DigestInit(hSession, &mcHashAlg);
	rv = MC_DigestUpdate(hSession, param, len);
	rv = MC_DigestUpdate(hSession, pPassword, strlen((char*)pPassword));
	rv = MC_DigestFinal(hSession, buf, &buflen);

	for(i=1; i<count; i++)
	{
		rv = MC_DigestInit(hSession, &mcHashAlg);
		rv = MC_Digest(hSession, buf, buflen, buf, &buflen);
	}

	MC_CreateObject(hSession, buf, 16, &hEncKey);
	MC_GetObjectValue(hSession, hKey, tmp, &tmplen);

	mcEncAlg.pParam = buf + 16;
	mcEncAlg.nParam = 16;
	rv = MC_EncryptInit(hSession, &mcEncAlg, hEncKey);
	rv = MC_Encrypt(hSession, tmp, tmplen, pWrapedkey, nWrapedkeylen);

	return rv;
}

int unwrap(MC_HOBJECT hSession, unsigned char *pPassword, MC_UCHAR *pWrapedkey, MC_UINT nWrapedkeylen, MC_HOBJECT *phKey)
{
	MC_RV rv;	
	MC_ALGORITHM mcHashAlg = {MC_ALGID_SHA256, NULL, 0};
	MC_ALGORITHM mcEncAlg = {MC_ALGID_ARIA_128BITKEY, NULL, 0};
	
	MC_UCHAR param[16] = {1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
	MC_UCHAR buf[32], tmp[2048];
	MC_UINT buflen = sizeof(buf), tmplen = sizeof(tmp);
	int i, count = 1024;
	MC_UINT len = sizeof(param);
	
	MC_HOBJECT hEncKey;
	
	rv = MC_DigestInit(hSession, &mcHashAlg);
	rv = MC_DigestUpdate(hSession, param, len);
	rv = MC_DigestUpdate(hSession, pPassword, strlen((char*)pPassword));
	rv = MC_DigestFinal(hSession, buf, &buflen);
	
	for(i=1; i<count; i++)
	{
		rv = MC_DigestInit(hSession, &mcHashAlg);
		rv = MC_Digest(hSession, buf, buflen, buf, &buflen);
	}
	
	MC_CreateObject(hSession, buf, 16, &hEncKey);
	
	mcEncAlg.pParam = buf + 16;
	mcEncAlg.nParam = 16;
	rv = MC_DecryptInit(hSession, &mcEncAlg, hEncKey);
	rv = MC_Decrypt(hSession, pWrapedkey, nWrapedkeylen, tmp, &tmplen);

	MC_CreateObject(hSession, tmp, tmplen, phKey);
	
	return rv;
}

void putPbkdfParam(MC_UCHAR *pOut, MC_UINT *nOut, MC_UCHAR *pSalt, MC_UINT nSalt, MC_UINT IterCnt)
{
	MC_UCHAR *p = pOut;
	MC_UCHAR pIterCnt[5] = {0,};
	int nIterCnt=0, nTotal=0, i=0;
/*
PBKDF ::= sequence {
 salt octet,
 iterCnt int
}
*/
	*p++ = 0x30;

	if(IterCnt & 0xff000000) nIterCnt = 4;
	else if(IterCnt & 0x00ff0000) nIterCnt = 3;
	else if(IterCnt & 0x0000ff00) nIterCnt = 2;
	else if(IterCnt & 0x000000ff) nIterCnt = 1;
	// 4 + nIterCnt + nsalt
	nTotal = 4 + nIterCnt + nSalt;

	*p++ = nTotal;
	
	*p++ = 0x04;
	*p++ = nSalt;
	memcpy(p, pSalt, nSalt);
	p += nSalt;

	*p++ = 0x02;
	for(i=0;i<nIterCnt;i++) {
		pIterCnt[nIterCnt-i-1] = IterCnt & 0xff;
		IterCnt >>= 8;
	}
	*p++ = nIterCnt;
	memcpy(p, pIterCnt, nIterCnt);

	*nOut = nTotal + 2;
}

void putKbkdfParam(MC_UCHAR *pOut, MC_UINT *nOut, MC_UCHAR *pLabel, MC_UINT nLabel, MC_UCHAR *pContext, MC_UINT nContext)
{
	MC_UCHAR *p = pOut;
	MC_UCHAR pIterCnt[5] = {0,};
	int nTotal=0, i=0;
/*
KBKDF := Sequence {
 label octet,
 context octet
}
*/
	*p++ = 0x30;
	nTotal = 4 + nLabel + nContext;
	*p++ = nTotal;
	
	*p++ = 0x04;
	*p++ = nLabel;
	memcpy(p, pLabel, nLabel);
	p += nLabel;

	*p++ = 0x04;
	*p++ = nContext;
	memcpy(p, pContext, nContext);
	p += nContext;

	*nOut = nTotal + 2;
}


int testGenKeyPair()
{
	MC_RV rv;	
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0};
	MC_ALGORITHM mcWrapAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hPubkey = 0, hPrikey = 0, hWrapkey = 0;
	MC_HSESSION hSession = 0;
	MC_UCHAR param[2048]={0,}, wrapedkey[20480]={0,}, tmp[20480]={0,};
	MC_UINT paramlen, tmplen, wrapedkeylen = sizeof(wrapedkey);


	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	while(1)
	{
		menus menu[] = {
			MC_ALGID_RSA512, "MC_ALGID_RSA512", 
			MC_ALGID_RSA1024, "MC_ALGID_RSA1024", 
			MC_ALGID_RSA2048, "MC_ALGID_RSA2048", 
			MC_ALGID_RSA3072, "MC_ALGID_RSA3072", 
			MC_ALGID_RSA4096, "MC_ALGID_RSA4096", 
			MC_ALGID_RSA1024P2V2, "MC_ALGID_RSA1024P2V2", 
			MC_ALGID_RSA2048P2V2, "MC_ALGID_RSA2048P2V2", 
			MC_ALGID_RSA3072P2V2, "MC_ALGID_RSA3072P2V2", 
			MC_ALGID_RSA4096P2V2, "MC_ALGID_RSA4096P2V2", 
			MC_ALGID_KCDSA1024, "MC_ALGID_KCDSA1024", 
			MC_ALGID_KCDSA2048, "MC_ALGID_KCDSA2048", 
			MC_ALGID_EC_B163_3, "MC_ALGID_EC_B163_3", 
			MC_ALGID_EC_B163_5, "MC_ALGID_EC_B163_5", 
			MC_ALGID_EC_P224_12, "MC_ALGID_EC_P224_12", 
			MC_ALGID_EC_P256_r1, "MC_ALGID_EC_P256_r1", 
			MC_ALGID_EC_P256_k1, "MC_ALGID_EC_P256_k1", 
			MC_ALGID_EC_B283_r1, "MC_ALGID_EC_B283_r1", 
			MC_ALGID_EC_B283_k1, "MC_ALGID_EC_B283_k1", 
			MC_ALGID_EC_P384_r1, "MC_ALGID_EC_P384_r1", 
			MC_ALGID_EC_P521_r1, "MC_ALGID_EC_P521_r1", 
			MC_ALGID_DH1024, "MC_ALGID_DH1024", 
			MC_ALGID_DH2048, "MC_ALGID_DH2048", 
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		mcAlg.pParam = 0;
		mcAlg.nParam = 0;
		if(mcAlg.mcAlgId == MC_ALGID_KCDSA1024 || mcAlg.mcAlgId == MC_ALGID_KCDSA2048)
		{
#if 0
			inputData("Parameter [Gen]: ", param, paramlen);
			paramlen = processData((char*)param);
			if(paramlen != 0)
				mcAlg.mcAlgId = MC_ALGID_KCDSAWithParam;
			mcAlg.pParam = param;
			mcAlg.nParam = paramlen;
#else
			if(mcAlg.mcAlgId == MC_ALGID_KCDSA1024)
				paramlen = HexToBin(param, "3082011f02818100940f2886f849956805be4d218f448ab5aa0877beb6a54c783676dcca03aae5f9ff6de10b13c0c30902053910a333d6c07a7a1652ffd3ff7576f8c1943f42950b968b889a31cef1184d1d17fde6a54b03c303155d4e3b7eaa4ed8605388c2a94ebd824cb0d0fba7f6c8583daad73e5e07ec4198a7f8499daf04474d590460cda3021500f16ef4500e045f30ca486848b02e9a0ebdbcb44b028181008c7c44bbcf8c1cd086c7be59f4aa45e07e53c34303e7bbee8e6006729ef2ff7728e693afb66f00ce7dcdad5f5ba1d4b9d93b03553d2f2cecf34ec084a8f5fc6ef44c3119bec4baff8fa989af3e87034dfd61e4e9ceab6331d628478244795924c25781ecf0f8ee5ce8118b8f69a321ccfec99629c067f9ba3225282abcf2e32a");
			else if(mcAlg.mcAlgId == MC_ALGID_KCDSA2048)
				paramlen = HexToBin(param, "3082022D0282010100B934A7B67EE1E104461E493D03D73C2038532D54EEDA7F67EA52C72301CB965CEAE63F5E4498A2B5FB535751B70C7F4E526432A5331AE2A16B3B7E578A731ECCFF44D3A638C7D2B6AA619ABA1FDD6707FE97DE6765880AC9C57C2566B97A1D466A227B02A53A556B1B9A6CF698633DBA31760826E3443DB6FD3DD2CE707DCE001640228A9569D0CFA55DEA0A9D86B112B1270705DCB90DFC386B8334612CE4A94D15FD66A0156D0FE419B64136C82CB24437F31DD7F35CD429F809F0C7BF0B15CBEE1AE5BB27C1BC7221D6587DCE8F2F5C3B458BB61934C574808E1DEB9619743DDD86069BF68C6F3A9D24C4346567394C1882ECA4E9DA2279E0176DC806BEDB022100B1F757A332BB73D5DC784E54EC983ACBA61F8C7A20F79A7A0CB6547E5C42D77B028201010083B26D4D26AD9DBE251BBA23AF58A7D4A5FE431B22A601DA030771847729707A74E42BC121D917E9B94927E6C6AF35F57EA03121ED561ACFAD06C15D777CC18143EEE1E3C2015177B184F8EACE75FAC7B40FF1E45812F59FCAE49638F4595682BEE21EDB617883E4CE8178E73A26E7C6DA40FDCC803685330DF9119C3D1DE036A9022FA096460CD8D80E31756EC9288236B0BEBB879580D53539CA610777AB17790A5428F816AD28F5F48167C4338D03A515017D2759104A53674EDB13C4AB155E3C7907688B4FC5FB83231B632FFB04EDDD13057F5DB534970089DCA1C30F74C55DDE6C9EB7A1993E99E31E8D9DD762A21BC65D51784853CC84B971103263A2");
			mcAlg.mcAlgId = MC_ALGID_KCDSAWithParam;
			mcAlg.pParam = param;
			mcAlg.nParam = paramlen;
#endif
		}
		else if(mcAlg.mcAlgId == MC_ALGID_DH1024 || mcAlg.mcAlgId == MC_ALGID_DH2048)
		{
			if(mcAlg.mcAlgId == MC_ALGID_DH1024) 
				paramlen = HexToBin(param, "3082011f02818100940f2886f849956805be4d218f448ab5aa0877beb6a54c783676dcca03aae5f9ff6de10b13c0c30902053910a333d6c07a7a1652ffd3ff7576f8c1943f42950b968b889a31cef1184d1d17fde6a54b03c303155d4e3b7eaa4ed8605388c2a94ebd824cb0d0fba7f6c8583daad73e5e07ec4198a7f8499daf04474d590460cda3021500f16ef4500e045f30ca486848b02e9a0ebdbcb44b028181008c7c44bbcf8c1cd086c7be59f4aa45e07e53c34303e7bbee8e6006729ef2ff7728e693afb66f00ce7dcdad5f5ba1d4b9d93b03553d2f2cecf34ec084a8f5fc6ef44c3119bec4baff8fa989af3e87034dfd61e4e9ceab6331d628478244795924c25781ecf0f8ee5ce8118b8f69a321ccfec99629c067f9ba3225282abcf2e32a");
			else if(mcAlg.mcAlgId == MC_ALGID_DH2048)
				paramlen = HexToBin(param, "3082022D0282010100B934A7B67EE1E104461E493D03D73C2038532D54EEDA7F67EA52C72301CB965CEAE63F5E4498A2B5FB535751B70C7F4E526432A5331AE2A16B3B7E578A731ECCFF44D3A638C7D2B6AA619ABA1FDD6707FE97DE6765880AC9C57C2566B97A1D466A227B02A53A556B1B9A6CF698633DBA31760826E3443DB6FD3DD2CE707DCE001640228A9569D0CFA55DEA0A9D86B112B1270705DCB90DFC386B8334612CE4A94D15FD66A0156D0FE419B64136C82CB24437F31DD7F35CD429F809F0C7BF0B15CBEE1AE5BB27C1BC7221D6587DCE8F2F5C3B458BB61934C574808E1DEB9619743DDD86069BF68C6F3A9D24C4346567394C1882ECA4E9DA2279E0176DC806BEDB022100B1F757A332BB73D5DC784E54EC983ACBA61F8C7A20F79A7A0CB6547E5C42D77B028201010083B26D4D26AD9DBE251BBA23AF58A7D4A5FE431B22A601DA030771847729707A74E42BC121D917E9B94927E6C6AF35F57EA03121ED561ACFAD06C15D777CC18143EEE1E3C2015177B184F8EACE75FAC7B40FF1E45812F59FCAE49638F4595682BEE21EDB617883E4CE8178E73A26E7C6DA40FDCC803685330DF9119C3D1DE036A9022FA096460CD8D80E31756EC9288236B0BEBB879580D53539CA610777AB17790A5428F816AD28F5F48167C4338D03A515017D2759104A53674EDB13C4AB155E3C7907688B4FC5FB83231B632FFB04EDDD13057F5DB534970089DCA1C30F74C55DDE6C9EB7A1993E99E31E8D9DD762A21BC65D51784853CC84B971103263A2");
			mcAlg.mcAlgId = MC_ALGID_DHWithParam;
			mcAlg.pParam = param;
			mcAlg.nParam = paramlen;
		}

		rv = MC_GenerateKeyPair(hSession, &mcAlg, &hPubkey, &hPrikey);
		if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}

		memset(tmp, 0x00, sizeof(tmp)); tmplen = sizeof(tmp);
		MC_GetObjectValue(hSession, hPubkey, tmp, &tmplen);
		printf("PublicKey(%d) : \n", tmplen);
		printOutput(tmp, tmplen);
		
		memset(tmp, 0x00, sizeof(tmp)); tmplen = sizeof(tmp);
		MC_GetObjectValue(hSession, hPrikey, tmp, &tmplen);
		printf("PrivateKey(%d) : \n", tmplen);
		printOutput(tmp, tmplen);

// 		wrap(hSession, "qwer1234", hPrikey, wrapedkey, &wrapedkeylen);
// 		unwrap(hSession, "qwer1234", wrapedkey, wrapedkeylen, &hPrikey);
		
		mcWrapAlg.mcAlgId = MC_ALGID_SHA256WithARIA128;
		mcWrapAlg.pParam = (MC_UCHAR*)"\x04\x08\x01\x02\x03\x04\x05\x06\x07\x08";
		mcWrapAlg.nParam = 10;
		
		MC_CreateObject(hSession, (MC_UCHAR*)"qwer1234", 8, &hWrapkey);
	
		rv = MC_WrapKey(hSession, &mcWrapAlg, hWrapkey, hPrikey, wrapedkey, &wrapedkeylen);
		if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}

		printf("Wrapped PrivateKey(%d) : \n", wrapedkeylen);
		printOutput(wrapedkey, wrapedkeylen);

		rv = MC_UnwrapKey(hSession, &mcWrapAlg, hWrapkey, wrapedkey, wrapedkeylen, &hPrikey);
		if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}
		
		memset(tmp, 0x00, sizeof(tmp)); tmplen = sizeof(tmp);
		MC_GetObjectValue(hSession, hPrikey, tmp, &tmplen);
		printf("Unwrapped PrivateKey(%d) : \n", tmplen);
		printOutput(tmp, tmplen);
	}

	rv = MC_DestroyObject(hSession, hPubkey);
	if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv));}
	rv = MC_DestroyObject(hSession, hPrikey);
	if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv));}
	rv = MC_CloseSession(hSession);
	if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv));}

	return 0;
}

int testDeriveKey()
{
	MC_RV rv;	
	MC_ALGORITHM mcAlg = {MC_ALGID_NONE, NULL, 0}, mcKeyAlg = {MC_ALGID_NONE, NULL, 0};
	MC_HOBJECT hMyPubkey = 0, hMyPrikey = 0;
	MC_HOBJECT hOtherPubkey = 0, hOtherPrikey = 0;
	MC_HSESSION hSession = 0;
	MC_UCHAR param[2048], pub[2560], key[2560];
	MC_UINT paramlen, publen, keylen = sizeof(key);

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}

	while(1)
	{
		menus menu[] = {
				MC_ALGID_DH1024, "MC_ALGID_DH1024", 
				MC_ALGID_DH2048, "MC_ALGID_DH2048", 
				MC_ALGID_ECDH_P224_12, "MC_ALGID_ECDH_P224_12", 
				MC_ALGID_ECDH_P256_r1, "MC_ALGID_ECDH_P256_r1", 
				MC_ALGID_SHA256WithPBKDF2, "MC_ALGID_SHA256WithPBKDF2", 
				MC_ALGID_SHA256WithKBKDF_CTR, "MC_ALGID_SHA256WithKBKDF_CTR", 
				0, ""};
		
		SHOW_MENU(menu, &mcAlg.mcAlgId);

		mcAlg.pParam = NULL;
		mcAlg.nParam = 0;

		if(mcAlg.mcAlgId == MC_ALGID_DH1024 || mcAlg.mcAlgId == MC_ALGID_DH2048
		   || mcAlg.mcAlgId == MC_ALGID_ECDH_P224_12 || mcAlg.mcAlgId == MC_ALGID_ECDH_P256_r1)
		{
			if(mcAlg.mcAlgId == MC_ALGID_DH1024) {
				mcKeyAlg.mcAlgId = MC_ALGID_DHWithParam;
				paramlen = HexToBin(param, "3082011f02818100940f2886f849956805be4d218f448ab5aa0877beb6a54c783676dcca03aae5f9ff6de10b13c0c30902053910a333d6c07a7a1652ffd3ff7576f8c1943f42950b968b889a31cef1184d1d17fde6a54b03c303155d4e3b7eaa4ed8605388c2a94ebd824cb0d0fba7f6c8583daad73e5e07ec4198a7f8499daf04474d590460cda3021500f16ef4500e045f30ca486848b02e9a0ebdbcb44b028181008c7c44bbcf8c1cd086c7be59f4aa45e07e53c34303e7bbee8e6006729ef2ff7728e693afb66f00ce7dcdad5f5ba1d4b9d93b03553d2f2cecf34ec084a8f5fc6ef44c3119bec4baff8fa989af3e87034dfd61e4e9ceab6331d628478244795924c25781ecf0f8ee5ce8118b8f69a321ccfec99629c067f9ba3225282abcf2e32a");
				mcKeyAlg.pParam = param;
				mcKeyAlg.nParam = paramlen;
			}
			else if(mcAlg.mcAlgId == MC_ALGID_DH2048) {
				mcKeyAlg.mcAlgId = MC_ALGID_DHWithParam;
				paramlen = HexToBin(param, "3082022D0282010100B934A7B67EE1E104461E493D03D73C2038532D54EEDA7F67EA52C72301CB965CEAE63F5E4498A2B5FB535751B70C7F4E526432A5331AE2A16B3B7E578A731ECCFF44D3A638C7D2B6AA619ABA1FDD6707FE97DE6765880AC9C57C2566B97A1D466A227B02A53A556B1B9A6CF698633DBA31760826E3443DB6FD3DD2CE707DCE001640228A9569D0CFA55DEA0A9D86B112B1270705DCB90DFC386B8334612CE4A94D15FD66A0156D0FE419B64136C82CB24437F31DD7F35CD429F809F0C7BF0B15CBEE1AE5BB27C1BC7221D6587DCE8F2F5C3B458BB61934C574808E1DEB9619743DDD86069BF68C6F3A9D24C4346567394C1882ECA4E9DA2279E0176DC806BEDB022100B1F757A332BB73D5DC784E54EC983ACBA61F8C7A20F79A7A0CB6547E5C42D77B028201010083B26D4D26AD9DBE251BBA23AF58A7D4A5FE431B22A601DA030771847729707A74E42BC121D917E9B94927E6C6AF35F57EA03121ED561ACFAD06C15D777CC18143EEE1E3C2015177B184F8EACE75FAC7B40FF1E45812F59FCAE49638F4595682BEE21EDB617883E4CE8178E73A26E7C6DA40FDCC803685330DF9119C3D1DE036A9022FA096460CD8D80E31756EC9288236B0BEBB879580D53539CA610777AB17790A5428F816AD28F5F48167C4338D03A515017D2759104A53674EDB13C4AB155E3C7907688B4FC5FB83231B632FFB04EDDD13057F5DB534970089DCA1C30F74C55DDE6C9EB7A1993E99E31E8D9DD762A21BC65D51784853CC84B971103263A2");
				mcKeyAlg.pParam = param;
				mcKeyAlg.nParam = paramlen;
			}
			else if(mcAlg.mcAlgId == MC_ALGID_ECDH_P224_12)
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P224_12;
			else if(mcAlg.mcAlgId == MC_ALGID_ECDH_P256_r1)
				mcKeyAlg.mcAlgId = MC_ALGID_EC_P256_r1;

			rv = MC_GenerateKeyPair(hSession, &mcKeyAlg, &hMyPubkey, &hMyPrikey);
			if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}

			memset(pub, 0x00, sizeof(pub)); publen = sizeof(pub);
			MC_GetObjectValue(hSession, hMyPubkey, pub, &publen);
			memset(key, 0x00, sizeof(key)); keylen = sizeof(key);
			MC_GetObjectValue(hSession, hMyPrikey, key, &keylen);
			printf("Alice's PublicKey(%d) : \n", publen);
			printOutput(pub, publen);
			printf("Alice's PrivateKey(%d) : \n", keylen);
			printOutput(key, keylen);
			
			rv = MC_GenerateKeyPair(hSession, &mcKeyAlg, &hOtherPubkey, &hOtherPrikey);
			if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}

			memset(pub, 0x00, sizeof(pub)); publen = sizeof(pub);
			MC_GetObjectValue(hSession, hOtherPubkey, pub, &publen);
			memset(key, 0x00, sizeof(key)); keylen = sizeof(key);
			MC_GetObjectValue(hSession, hOtherPrikey, key, &keylen);
			printf("Bob's PublicKey(%d) : \n", publen);
			printOutput(pub, publen);
			printf("Bob's PrivateKey(%d) : \n", keylen);
			printOutput(key, keylen);

			memset(pub, 0x00, sizeof(pub)); publen = sizeof(pub);
			MC_GetObjectValue(hSession, hOtherPubkey, pub, &publen);

			mcAlg.pParam = pub;
			mcAlg.nParam = publen;
			if(mcAlg.mcAlgId == MC_ALGID_DH1024 || mcAlg.mcAlgId == MC_ALGID_DH2048) 
				mcAlg.mcAlgId = MC_ALGID_DH;
			
			rv = MC_DeriveKey(hSession, &mcAlg, hMyPrikey, key, &keylen);
			if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}

			printf("Derived Key(%d) : \n", keylen);
			printOutput(key, keylen);

			memset(pub, 0x00, sizeof(pub)); publen = sizeof(pub);
			MC_GetObjectValue(hSession, hMyPubkey, pub, &publen);

			rv = MC_DeriveKey(hSession, &mcAlg, hOtherPrikey, key, &keylen);
			if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); break;}
		}
		else 
		{
			if (mcAlg.mcAlgId == MC_ALGID_SHA256WithPBKDF2) 
			{
				MC_UCHAR salt[32];
				MC_UINT nSalt = sizeof(salt);
				MC_UINT iterationCount = 100000;
				memcpy(salt, "\xD2\xE9\x8F\x8D\xB0\x1E\xC2\xBF\xF7\xF8\x6E\x4D\xDB\xE5\x19\x34\xB4\xF1\x73\xF0\x5E\x94\xDE\xEF\xE0\xCE\x2D\x46\x4D\x76\xAC\xA3", 32);
				putPbkdfParam(param, &paramlen, salt, nSalt, iterationCount);

				mcAlg.pParam = param;
				mcAlg.nParam = paramlen;

				MC_CreateObject(hSession, (MC_UCHAR*)"LHzhknpDXJ", 10, &hMyPrikey);
			}
			else /*MC_ALGID_SHA256WithKBKDF_CTR*/ 
			{
				MC_UCHAR keyin[32];
				MC_UCHAR label[60], context[60];
				MC_UINT nContext = sizeof(context), nLabel = sizeof(label);

				memcpy(keyin, "\x33\xBA\x02\x3F\x3C\x5A\x34\xB8\xE4\x68\x33\x7F\x83\xF0\xAC\x1E\xCC\xE7\x93\x47\xF4\xBD\x67\x0E\x17\x31\xB7\x56\xD7\x69\xE5\x89", sizeof(keyin));
				memcpy(label, "\x46\x42\x2A\x8D\x06\xE5\xDA\xF3\xAE\x9B\xE9\x71\x70\xE2\xAF\x92\xF9\xC0\xE8\xAC\x99\x57\xFF\x13\xA4\xBA\x8C\x8C\x18\xFF\x1B\x04\x7F\x95\x2F\x82\x17\xD8\x86\x5B\x31\xE3\x9F\xA5\xD7\xA5\x7A\xED\xB1\x38\x9F\x2D\x2A\x6D\x61\xF8\xCE\xAC\x64\xF8", 60);
				memcpy(context, "\xF6\xFB\x5D\x9A\xA8\x5C\x19\x08\xB9\x5C\xC1\x58\x34\xE9\x5C\x02\x00\x64\x95\x98\xBC\xF7\xBE\xB1\xEE\x29\x16\x27\x5D\xAF\x47\x82\xBB\x85\x33\xB3\x87\x3E\xF0\x06\x32\x99\x11\x52\x81\x54\x28\x72\x32\x45\x89\xF9\xE2\xA6\x50\x22\x2E\xB1\xA5\x06", 60);

				putKbkdfParam(param, &paramlen, label, nLabel, context, nContext);
				mcAlg.pParam = param;
				mcAlg.nParam = paramlen;
				MC_CreateObject(hSession, keyin, sizeof(keyin), &hMyPrikey);
			}
			
			keylen = 64;
			rv = MC_DeriveKey(hSession, &mcAlg, hMyPrikey, key, &keylen);
			if(rv != MC_OK) {printf("[%d] %s\n", __LINE__, MC_GetErrorString(rv)); break;}
		}

		printf("Derived Key(%d) : \n", keylen);
		printOutput(key, keylen);
	}

	MC_DestroyObject(hSession, hMyPubkey);
	MC_DestroyObject(hSession, hMyPrikey);
	MC_DestroyObject(hSession, hOtherPubkey);
	MC_DestroyObject(hSession, hOtherPrikey);
	MC_CloseSession(hSession);

	return 0;
}

int testGenRandom()
{
	MC_RV rv = MC_OK;
	MC_ALGORITHM mcAlg = {MC_ALGID_SHA256DRBG, NULL, 0};
	MC_HSESSION hSession = 0;
	MC_UCHAR Random[1024];
	MC_INT RandBytes;

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	rv = MC_SetApiMode(hSession, gApimode);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 2;}
	
	while(1)
	{
		menus menu[] = {
			MC_ALGID_SHA256DRBG, "MC_ALGID_SHA256DRBG", 
			/*MC_ALGID_FIPS186PRNG, "MC_ALGID_FIPS186PRNG", */
			0, ""};
		SHOW_MENU(menu, &mcAlg.mcAlgId);

	printf("\n");
	printf("Random length : ");
	scanf("%d", &RandBytes);
	if(RandBytes > sizeof(Random)) {printf("Random length too long\n\n"); return 1;}

	rv = MC_GenerateRandom(hSession, &mcAlg, Random, RandBytes);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); return 1;}

	printf("Random(%d) : \n", RandBytes);
	printOutput(Random, RandBytes);
 
	printf("\n");
	}
	
	MC_CloseSession(hSession);

	return rv;
}

int changeApimode()
{
	int mode;
	MC_HSESSION hSession;

	printf("\n");
	printf("1. KCMV mode\n");
	printf("2. Non KCMV mode\n");
	printf("0. Main menu\n");
	printf("Select : ");
	scanf("%d", &mode);
	printf("\n");

	switch(mode)
	{
	case 2:	 gApimode = MC_MODE_NON_KCMV; break;
	default: gApimode = MC_MODE_KCMV;
	}

	MC_OpenSession(&hSession);
	MC_SetApiMode(hSession, gApimode);
	MC_CloseSession(hSession);

	return 0;
}

#define MC_STAT_NONE				0x00000000
#define MC_STAT_INITIALIZED			0x10000000
#define MC_STAT_FATAL				0x20000000

void getStatus()
{
	MC_RV rv = 0;
	MC_UINT flag = 0;
	rv = MC_GetStatus(&flag);

	printf("\n MC_GetStatus rv = 0x%04x (%s), flag = 0x%08x \n", rv, MC_GetErrorString(rv), flag);

	if (flag & MC_STAT_INITIALIZED) {
		printf(" MC status is normal. (%s) \n", MC_GetErrorString(rv));
	} else if (flag & MC_STAT_FATAL) {
		printf(" MC status is fatal error ! (%s) \n", MC_GetErrorString(rv));
		exit(0);
	} else { /* (flag & MC_STAT_NONE)*/
		printf(" MC status is not initialized. (%s) \n", MC_GetErrorString(rv));
	} 

	printf("\n");
}

void findIntegrity()
{
	int i;
	MC_UCHAR *d, key[256] = {0,};
	MC_UINT l, m, keylen = 0;
	MC_CHAR f[256] = {0,};
	MC_RV rv;
	MC_HSESSION hSession = 0;
	MC_HOBJECT hKey = 0;
	MC_ALGORITHM mcAlg = {MC_ALGID_SHA1_HMAC, NULL, 0};
	MC_UCHAR out[64] = {0,};
	MC_UINT outlen = 64;
	FILE *fp;

	printf("\n");
	printf("Enter file name : ");
	scanf("%s", f);
	fp = fopen(f, "rb");
	if(fp == NULL) {
		printf("Error: cannot open \"%s\".\n\n", f);
		return; 
	}

	fseek(fp, 0, SEEK_END);
	m = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	d = (MC_UCHAR*)calloc(1, m+1);
	l = fread(d, 1, m+1, fp);

	rv = MC_OpenSession(&hSession);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); goto end;}

	printf("Ex. tmi9ueidr9tedily.st1usrr8cat_ag\n");
	printf("MAC key : ");
	scanf("%s", key);
	keylen = strlen((char*)key);
	keylen = processData((char*)key);

	MC_CreateObject(hSession, key, keylen, &hKey);

	rv = MC_CreateMacInit(hSession, &mcAlg, hKey);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); goto end;}

	rv = MC_CreateMac(hSession, d, l, out, &outlen);
	if(rv != MC_OK) {printf("%s\n", MC_GetErrorString(rv)); goto end;}
	
	printf("MAC value: "); 
	for(i=0; i<(int)outlen; i++) printf("%02X", out[i]); printf("\n\n");

end:
	free(d);
	fclose(fp);
	MC_CloseSession(hSession);
}

int processData(char *str)
{
	char token[3] = "0x";
	int len;
	unsigned char *tmp;

	if(!memcmp(str, token, 2))
	{
		tmp = (unsigned char*)calloc(strlen(str)+1, 1);
		len = HexToBin(tmp, str+2);
		if(len < 0) return -1;
		memset(str, 0, strlen(str));
		memcpy(str, tmp, len);
		free(tmp);
		return len;
	}
	else
		return strlen(str);
}

int printOutput(unsigned char *data, unsigned int datalen)
{
	unsigned int i;
	int line = 1;

	printf("   %04d - ", line++);
	for(i=1; i<=datalen; i++)
	{
		printf("%02X", data[i-1]);
		if(!(i%16) && i!= datalen) printf("\n   %04d - ", line++);
		else if(!(i%4)) printf(" ");
	}
 	printf("\n");

	return datalen;
}

#define IsHexa(a) ( (((a)>47 && (a)<58) || ((a)>64 && (a)<71) || ((a)>96 && (a)<103)) ? 1 : 0 )
#define MakeHexa(a) ( ((a)>96) ? ((a)-87) : (((a)>64) ? ((a)-55) : ((a)-48)) )

int TrimData(char *r, char *hexstr)
{
	const char seps[] = " :;=-,\t\n";
	char *token;
	char *pr = r;
	int prlen;

	token = strtok(hexstr, seps);
	while(token != NULL)
	{
		prlen = strlen(token);
		strcpy(pr, token);
		token = strtok(NULL, seps);
		pr += prlen;
	}

	return strlen(r);
}

int HexToBin(unsigned char *r, char *hexstr)
{
	int i, j = 0, l, m, n;
	char *pt;

	pt = (char*)calloc(1, strlen(hexstr)+1);
	if(!pt) return -1;
	strcpy((char*)pt, hexstr);

//	l = TrimData(pt, pt);
	l = strlen(pt);
	m = l % 2;
	n = (l + 1) / 2;

	for(i=0; i<l; i++) if(!IsHexa(pt[i])) {n = -1; goto End;}

	if(l==0)
	{
		r[0] = n = 0;
		goto End;
	}

	if(m) 
	{
		r[0] = MakeHexa(pt[0]);
		j++;
	}

	for(i=j; i<n; i++)
	{
		r[i]  = MakeHexa(pt[((j)?(i*2-1):(i*2))]) << 4; 
		r[i] |= MakeHexa(pt[((j)?(i*2):(i*2+1))]); 
	}

End:
	if(pt) free(pt);
	return n;
}

/* user defined callback function */
static int _log(const char *path, const char *fmt, ...)
{
	va_list args;
	int ret;
	FILE *fp;
	struct tm *newtime;
	time_t long_time;

	time(&long_time);
	newtime = localtime(&long_time);
	if((path == NULL) || strlen(path) == 0 || (fp = fopen(path, "a")) == NULL)
	{
		printf("[%.19s] ", asctime(newtime));
		va_start(args, fmt);
		ret = vprintf(fmt, args);
		va_end(args);
	}
	else
	{
		fprintf(fp, "[%.19s] ", asctime(newtime));
		va_start(args, fmt);
		ret = vfprintf(fp, fmt, args);
		va_end(args);
		fclose(fp);
	}
	
	return ret;
}

