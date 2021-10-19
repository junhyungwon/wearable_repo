/************************************************************************/
/* Copyright (c) 2007, Dreamsecurity co., ltd. All Rights Reserved.     */
/*                                                                      */
/* �� �ҽ��ڵ��� �Ϻ� �Ǵ� ��ü�� (��)�帲��ť��Ƽ�� ���� �³� ����     */
/* �ٸ� ���α׷��̳� �ٸ� ������� ����, ����, ����, ���� �� �ٸ�       */
/* ��ǻ�;��� ��ȯ �� ��Ÿ �ҹ����� ����� ���մϴ�.                  */
/*                                                                      */
/*     ���� : MagicCrypto V1.0.0                                        */
/*     ȸ�� : (��)�帲��ť��Ƽ                                          */
/*     �ּ� : ����� ���ı� ������ 150-28 ������� 5��                  */
/*     ����ó : (02)2233-5533                                           */
/*     ������ : ����(kalacus@dreamsecurity.com)                       */
/*              ������(hdjang@dreamsecurity.com)                        */
/************************************************************************/

/*-[ History ]------------------------------------------------------------
���� : mcapi.h 

2017-08-17 : v2.2
	- �Լ� export ��� ���� by hdjang
	  (MC_Initialize)
2015-12-15 : v2.1
	- Ű���� �Լ� �߰� by hdjang
	  (MC_DeriveKey)
2008-04-14 : v1.1.1
	- API ���� ���ϴ� �Լ� �߰� by hdjang
	  (MC_GetApiMode)
2007-10-24 : v1.1
	- �ʱ�ȭ �Լ� ������Ÿ�� ���� by hdjang
	  (MC_Initialize, MC_Finalize)
2007-03-06 : v1.0
	- ���� by hdjang
------------------------------------------------------------------------*/

#ifndef _MC_HEADER_6D7F1268_9777_413F_BD45_82CF07AD2027
#define _MC_HEADER_6D7F1268_9777_413F_BD45_82CF07AD2027

#ifdef __cplusplus 
extern "C" { 
#endif /* __cplusplus */ 

/* 2017-08-17 : v2.2 �Լ� export ��� ���� */
#if defined(_WIN32)
 #if defined(MAGICCRYPTO_EXPORTS)
  #define MCAPI __declspec(dllexport)  
 #elif !defined(MAGICCRYPTO_EXPORTS) 
  #define MCAPI __declspec(dllimport)
 #endif
#elif __GNUC__ >= 4 /*linux/qnx/tizen/android/ios/macos*/|| defined(_AIX) 
 #define MCAPI __attribute__((visibility ("default")))
#elif defined(__sun)
 #define MCAPI __global
#elif defined(__hpux)
 #define MCAPI __declspec(dllexport)
#else
 #define MCAPI
#endif

#include "mcapi_type.h"

MCAPI MC_RV MC_GetVersion(OUT MC_VERSION *pVersion);
MCAPI MC_RV MC_GetStatus(OUT MC_UINT *pFlag);

MCAPI MC_RV MC_Initialize(IN MC_VOID *pInitArgs);
MCAPI MC_RV MC_Finalize();
MCAPI MC_RV MC_Selftest();
MCAPI MC_STR MC_GetErrorString(MC_RV nRv);

MCAPI MC_RV MC_OpenSession(OUT MC_HSESSION *phSession);
MCAPI MC_RV MC_CloseSession(IN MC_HSESSION hSession);
MCAPI MC_RV MC_CopySession(IN MC_HSESSION hSession, OUT MC_HSESSION *phSession);
MCAPI MC_STR MC_GetError(IN MC_HSESSION hSession);

MCAPI MC_RV MC_SetApiMode(IN MC_HSESSION hSession, IN MC_APIMODE mcMode);
MCAPI MC_RV MC_GetApiMode(IN MC_HSESSION hSession, OUT MC_APIMODE *pmcApiMode);
MCAPI MC_RV MC_SetOption(IN MC_HSESSION hSession, IN MC_ALGMODE mcAlgMode, IN MC_PADTYPE mcPadType);

MCAPI MC_RV MC_CreateObject(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nLength, OUT MC_HOBJECT *phObject);
MCAPI MC_RV MC_DestroyObject(IN MC_HSESSION hSession, IN MC_HOBJECT hObject);
MCAPI MC_RV MC_GetObjectValue(IN MC_HSESSION hSession, IN MC_HOBJECT hObject, OUT MC_UCHAR *pData, OUT MC_UINT *pnLength);

MCAPI MC_RV MC_SignInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hPriKey);
MCAPI MC_RV MC_Sign(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, OUT MC_UCHAR *pSign, OUT MC_UINT *pnSignLen);
MCAPI MC_RV MC_SignUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen);
MCAPI MC_RV MC_SignFinal(IN MC_HSESSION hSession, OUT MC_UCHAR *pSign, OUT MC_UINT *pnSignLen);
MCAPI MC_RV MC_VerifyInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hPubKey);
MCAPI MC_RV MC_Verify(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, IN MC_UCHAR *pSign, IN MC_UINT nSignLen);
MCAPI MC_RV MC_VerifyUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen);
MCAPI MC_RV MC_VerifyFinal(IN MC_HSESSION hSession, IN MC_UCHAR *pSign, IN MC_UINT nSignLen);

MCAPI MC_RV MC_EncryptInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hKey);
MCAPI MC_RV MC_Encrypt(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, OUT MC_UCHAR *pCipher, OUT MC_UINT *pnCipherLen);
MCAPI MC_RV MC_EncryptUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, OUT MC_UCHAR *pCipher, OUT MC_UINT *pnCipherLen);
MCAPI MC_RV MC_EncryptFinal(IN MC_HSESSION hSession, OUT MC_UCHAR *pCipher, OUT MC_UINT *pnCipherLen);
MCAPI MC_RV MC_DecryptInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hKey);
MCAPI MC_RV MC_Decrypt(IN MC_HSESSION hSession, IN MC_UCHAR *pCipher, IN MC_UINT nCipherLen, OUT MC_UCHAR *pData, OUT MC_UINT *pnDataLen);
MCAPI MC_RV MC_DecryptUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pCipher, IN MC_UINT nCipherLen, OUT MC_UCHAR *pData, OUT MC_UINT *pnDataLen);
MCAPI MC_RV MC_DecryptFinal(IN MC_HSESSION hSession, OUT MC_UCHAR *pData, OUT MC_UINT *pnDataLen);

MCAPI MC_RV MC_DigestInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg);
MCAPI MC_RV MC_Digest(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, OUT MC_UCHAR *pDigest, OUT MC_UINT *pnDigestLen);
MCAPI MC_RV MC_DigestUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen);
MCAPI MC_RV MC_DigestFinal(IN MC_HSESSION hSession, OUT MC_UCHAR *pDigest, OUT MC_UINT *pnDigestLen);

MCAPI MC_RV MC_CreateMacInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hKey);
MCAPI MC_RV MC_CreateMac(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, OUT MC_UCHAR *pMac, OUT MC_UINT *pnMacLen);
MCAPI MC_RV MC_CreateMacUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen);
MCAPI MC_RV MC_CreateMacFinal(IN MC_HSESSION hSession, OUT MC_UCHAR *pMac, OUT MC_UINT *pnMacLen);
MCAPI MC_RV MC_VerifyMacInit(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hKey);
MCAPI MC_RV MC_VerifyMac(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen, IN MC_UCHAR *pMac, IN MC_UINT nMacLen);
MCAPI MC_RV MC_VerifyMacUpdate(IN MC_HSESSION hSession, IN MC_UCHAR *pData, IN MC_UINT nDataLen);
MCAPI MC_RV MC_VerifyMacFinal(IN MC_HSESSION hSession, IN MC_UCHAR *pMac, IN MC_UINT nMacLen);

MCAPI MC_RV MC_GenerateRandom(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, OUT MC_UCHAR *pRandomData, IN MC_UINT nRandomLen);
MCAPI MC_RV MC_GenerateKey(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, OUT MC_HOBJECT *phKey);
MCAPI MC_RV MC_GenerateKeyPair(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, OUT MC_HOBJECT *phPubKey, OUT MC_HOBJECT *phPriKey);
MCAPI MC_RV MC_WrapKey(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hWrappingKey, IN MC_HOBJECT hKey, OUT MC_UCHAR *pWrappedKey, OUT MC_UINT *pnWrappedKeyLen);
MCAPI MC_RV MC_UnwrapKey(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hUnwrappingKey, IN MC_UCHAR *pWrappedKey, IN MC_UINT nWrappedKeyLen, OUT MC_HOBJECT *phKey);
MCAPI MC_RV MC_DeriveKey(IN MC_HSESSION hSession, IN MC_ALGORITHM *pAlg, IN MC_HOBJECT hPriKey, OUT MC_UCHAR *pDerivedKey, OUT MC_UINT *pnDerivedKeyLen);



#undef MCAPI 
#ifdef __cplusplus 
} 
#endif /* __cplusplus */ 

#endif /* _MC_HEADER_6D7F1268_9777_413F_BD45_82CF07AD2027 */
