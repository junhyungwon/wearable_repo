/************************************************************************/
/* Copyright (c) 2007, Dreamsecurity co., ltd. All Rights Reserved.     */
/*                                                                      */
/* 본 소스코드의 일부 또는 전체를 (주)드림시큐리티의 사전 승낙 없이     */
/* 다른 프로그램이나 다른 사람에게 복사, 변경, 전재, 배포 및 다른       */
/* 컴퓨터언어로 변환 등 기타 불법적인 사용을 금합니다.                  */
/*                                                                      */
/*     정보 : MagicCrypto V1.0.0                                        */
/*     회사 : (주)드림시큐리티                                          */
/*     주소 : 서울시 송파구 문정동 150-28 서경빌딩 5층                  */
/*     연락처 : (02)2233-5533                                           */
/*     개발자 : 김대식(kalacus@dreamsecurity.com)                       */
/*              장형도(hdjang@dreamsecurity.com)                        */
/************************************************************************/

/*-[ History ]------------------------------------------------------------
설명 : mcapi_type.h 

2017-08-17 : v1.2
	- 알고리즘 추가 및 ID변경 by hdjang
2015-12-15 : v2.1
	- 알고리즘 추가 및 ID변경 by hdjang
2015-04-22 : v2.0.1
	- Windows 64비트에서 포인터 처리를 위한 MC_HSESSION, MC_HOBJECT 데이터 타입변경 by hdjang
2012-02-28 : v2.0
	- 검증대상 알고리즘 변경 by hdjang
	- AES, LEA 알고리즘 추가 by hdjang
2010-04-06 : v1.0.3
	- SHA256 RSA OAEP/PSS, KCDSA ID 추가 by hdjang
2008-01-21 : v1.0.2
	- 미사용 ID 삭제 by hdjang
2007-09-14 : v1.0.1
	- 난수생성기 명칭 변경 by hdjang
	  (MC_ALGID_FIPS186PRNG)
2007-03-06 : v1.0
	- 생성 by hdjang
------------------------------------------------------------------------*/

#ifndef _MC_HEADER_708CA1EB_1BEB_4FE9_8408_BDD575B0D27B
#define _MC_HEADER_708CA1EB_1BEB_4FE9_8408_BDD575B0D27B

typedef unsigned char  MC_UCHAR;
typedef unsigned int   MC_UINT;
typedef unsigned long  MC_ULONG;
typedef char     MC_CHAR;
typedef int      MC_INT;
typedef long     MC_LONG;
typedef void     MC_VOID;

typedef MC_CHAR* MC_STR;
typedef MC_UINT  MC_RV;
typedef MC_VOID  MC_API;
typedef MC_VOID* MC_HSESSION;
typedef MC_VOID* MC_HOBJECT;

typedef struct mc_version {
	MC_INT major;
	MC_INT minor;
	MC_INT release;
	/*MC_INT build;*/
	MC_CHAR name[32];
} MC_VERSION;

#define MC_OK	0
#define MC_FAIL	1

/* supported algorithm
hash : sha1 sha2 lsh has160 md5 
sign : rsa rsapss kcdsa ecdsa
enc  : rsaoaep 3des aria seed lea hight
mac  : sha1hmac, sha256hmac
key  : rsa(512/1024/2048/4096) kcdsa(1024/2048) ecdsa(163#3/163#5/P224r1/P256r1/B283r1/B283k1)
drbg : iso18031
*/
#define MC_KCMV						  0x01000000
typedef enum mc_algId {
	MC_ALGID_NONE					= 0x00000000,

	/* hash */
	MC_ALGID_MD5					= 0x00000100,
	MC_ALGID_SHA1					= 0x00000200,
	MC_ALGID_HAS160					= 0x00000300,
	MC_ALGID_SHA224					= 0x00000400 | MC_KCMV,
	MC_ALGID_SHA256					= 0x00000500 | MC_KCMV,
	MC_ALGID_SHA384					= 0x00000600 | MC_KCMV,
	MC_ALGID_SHA512					= 0x00000700 | MC_KCMV,
	MC_ALGID_LSH224					= 0x00000800 | MC_KCMV,
	MC_ALGID_LSH256					= 0x00000900 | MC_KCMV,
	MC_ALGID_LSH384					= 0x00000A00 | MC_KCMV,
	MC_ALGID_LSH512					= 0x00000B00 | MC_KCMV,

	/* sign */
	MC_ALGID_SHA1WithRSA			= 0x10010200,
	MC_ALGID_SHA256WithRSA			= 0x10010500,
	MC_ALGID_SHA384WithRSA			= 0x10010600,
	MC_ALGID_SHA512WithRSA			= 0x10010700,
	MC_ALGID_SHA1WithRSAPSS			= 0x10020200,
	MC_ALGID_SHA256WithRSAPSS		= 0x10020500 | MC_KCMV,
	MC_ALGID_SHA384WithRSAPSS		= 0x10020600,
	MC_ALGID_SHA512WithRSAPSS		= 0x10020700,
	MC_ALGID_SHA1WithKCDSA			= 0x10030200,
	MC_ALGID_SHA256WithKCDSA		= 0x10030500 | MC_KCMV,
	MC_ALGID_HAS160WithKCDSA		= 0x10030300,
	MC_ALGID_SHA1WithECDSA			= 0x10040200,
	MC_ALGID_SHA1WithECDSA_B163_3	= 0x10040203,
	MC_ALGID_SHA1WithECDSA_B163_5	= 0x10040205,
	MC_ALGID_SHA256WithECDSA_P224_12= 0x1004050C | MC_KCMV,
	MC_ALGID_SHA256WithECDSA_P256_r1= 0x1004050D | MC_KCMV,
	MC_ALGID_SHA384WithECDSA_P256_r1= 0x1004060D,
	MC_ALGID_SHA512WithECDSA_P256_r1= 0x1004070D,
	MC_ALGID_SHA256WithECDSA_P256_k1= 0x1004050E,
	MC_ALGID_SHA384WithECDSA_P256_k1= 0x1004060E,
	MC_ALGID_SHA512WithECDSA_P256_k1= 0x1004070E,
	MC_ALGID_SHA256WithECDSA_B283_r1= 0x10040510 | MC_KCMV,
	MC_ALGID_SHA384WithECDSA_B283_r1= 0x10040610,
	MC_ALGID_SHA512WithECDSA_B283_r1= 0x10040710,
	MC_ALGID_SHA256WithECDSA_B283_k1= 0x10040511 | MC_KCMV,
	MC_ALGID_SHA384WithECDSA_B283_k1= 0x10040611,
	MC_ALGID_SHA512WithECDSA_B283_k1= 0x10040711,
	MC_ALGID_SHA256WithECDSA_P384_r1= 0x10040512,
	MC_ALGID_SHA384WithECDSA_P384_r1= 0x10040612,
	MC_ALGID_SHA512WithECDSA_P384_r1= 0x10040712,
	MC_ALGID_SHA256WithECDSA_P521_r1= 0x10040513,
	MC_ALGID_SHA384WithECDSA_P521_r1= 0x10040613,
	MC_ALGID_SHA512WithECDSA_P521_r1= 0x10040713,

	/* asymmetric encryption */
	MC_ALGID_RSA					= 0x20010000,
	MC_ALGID_SHA1WithRSAOAEP		= 0x20020200,
	MC_ALGID_SHA256WithRSAOAEP		= 0x20020500 | MC_KCMV,

	/* symmetric encryption */
	/*MC_ALGID_DES					= 0x20030001, */ /*v2.2 이후 제거*/
	MC_ALGID_3DES_3KEY				= 0x20030003,
	MC_ALGID_SEED					= 0x20040000 | MC_KCMV,
	MC_ALGID_RC2_40BITKEY			= 0x20070001,
	MC_ALGID_RC2_128BITKEY			= 0x20070002,
	MC_ALGID_RC2_192BITKEY			= 0x20070003,
	MC_ALGID_RC2_256BITKEY			= 0x20070004,
	MC_ALGID_RC4					= 0x20080000,
	MC_ALGID_ARIA_128BITKEY			= 0x200B0002 | MC_KCMV,
	MC_ALGID_ARIA_192BITKEY			= 0x200B0003 | MC_KCMV,
	MC_ALGID_ARIA_256BITKEY			= 0x200B0004 | MC_KCMV,
	MC_ALGID_AES_128BITKEY			= 0x200C0002,
	MC_ALGID_AES_192BITKEY			= 0x200C0003,
	MC_ALGID_AES_256BITKEY			= 0x200C0004,
	MC_ALGID_LEA_128BITKEY			= 0x200D0002 | MC_KCMV,
	MC_ALGID_LEA_192BITKEY			= 0x200D0003 | MC_KCMV,
	MC_ALGID_LEA_256BITKEY			= 0x200D0004 | MC_KCMV,
	MC_ALGID_HIGHT					= 0x200E0000 | MC_KCMV,

	/* mac */
	MC_ALGID_MD5_HMAC				= 0x30030100,
	MC_ALGID_SHA1_HMAC				= 0x30030200,
	MC_ALGID_SHA256_HMAC			= 0x30030500 | MC_KCMV,
	MC_ALGID_SHA384_HMAC			= 0x30030600 | MC_KCMV,
	MC_ALGID_SHA512_HMAC			= 0x30030700 | MC_KCMV,
	/*MC_ALGID_DESCBC_MAC			= 0x30030003,*/ /*v2.2 이후 제거*/
	MC_ALGID_SEED_GMAC				= 0x30040000 | MC_KCMV,
	MC_ALGID_ARIA_128BITKEY_GMAC	= 0x300B0002 | MC_KCMV,
	MC_ALGID_ARIA_192BITKEY_GMAC	= 0x300B0003 | MC_KCMV,
	MC_ALGID_ARIA_256BITKEY_GMAC	= 0x300B0004 | MC_KCMV,
	MC_ALGID_AES_128BITKEY_GMAC		= 0x300C0002,
	MC_ALGID_AES_192BITKEY_GMAC		= 0x300C0003,
	MC_ALGID_AES_256BITKEY_GMAC		= 0x300C0004,
	MC_ALGID_LEA_128BITKEY_GMAC		= 0x300D0002 | MC_KCMV,
	MC_ALGID_LEA_192BITKEY_GMAC		= 0x300D0003 | MC_KCMV,
	MC_ALGID_LEA_256BITKEY_GMAC		= 0x300D0004 | MC_KCMV,

	/* key pair generate */
	MC_ALGID_RSA512					= 0x40010001,
	MC_ALGID_RSA1024				= 0x40010002,
	MC_ALGID_RSA2048				= 0x40010004 | MC_KCMV,
	MC_ALGID_RSA3072				= 0x40010006 | MC_KCMV,
	MC_ALGID_RSA4096				= 0x40010008,
	MC_ALGID_RSA1024P2V2			= 0x40020002,			/* RSA Multiprime(2) 키생성 */
	MC_ALGID_RSA2048P2V2			= 0x40020004 | MC_KCMV,	/* RSA Multiprime(2) 키생성 */
	MC_ALGID_RSA3072P2V2			= 0x40020006 | MC_KCMV,	/* RSA Multiprime(2) 키생성 */
	MC_ALGID_RSA4096P2V2			= 0x40020008,			/* RSA Multiprime(2) 키생성 */
	MC_ALGID_KCDSAWithParam			= 0x40050000 | MC_KCMV,	/* 파라미터(pqg)를 이용한 키생성 */
	MC_ALGID_KCDSA1024				= 0x40050002,			/* 1024,160bit 도메인 파라미터 생성 */
	MC_ALGID_KCDSA2048				= 0x40050004 | MC_KCMV,	/* 2048,256bit 도메인 파라미터 생성 */
	MC_ALGID_EC_B163_3				= 0x40060003,
	MC_ALGID_EC_B163_5				= 0x40060005,
	MC_ALGID_EC_P224_12				= 0x4006000C | MC_KCMV,
	MC_ALGID_EC_P256_r1				= 0x4006000D | MC_KCMV,
	MC_ALGID_EC_P256_k1				= 0x4006000E,
	MC_ALGID_EC_B283_r1				= 0x4060010  | MC_KCMV,
	MC_ALGID_EC_B283_k1				= 0x40060011 | MC_KCMV,
	MC_ALGID_EC_P384_r1				= 0x40060012,
	MC_ALGID_EC_P521_r1				= 0x40060013,
	MC_ALGID_DHWithParam			= 0x40070000 | MC_KCMV,	/* 파라미터(pqg)를 이용한 키생성 */
	MC_ALGID_DH1024					= 0x40070002,			/* 1024,160bit 도메인 파라미터 생성 */
	MC_ALGID_DH2048					= 0x40070004 | MC_KCMV,	/* 2048,256bit 도메인 파라미터 생성 */

	/* key agree */
	MC_ALGID_DH						= 0x50010000 | MC_KCMV,
	MC_ALGID_ECDH_P224_12			= 0x5002000C | MC_KCMV,
	MC_ALGID_ECDH_P256_r1			= 0x5002000D | MC_KCMV,

	/* pseudo random */
	MC_ALGID_SHA256DRBG				= 0x50030500 | MC_KCMV,

	/* key derive */
	MC_ALGID_SHA256WithPBKDF2		= 0x60010502 | MC_KCMV,
	MC_ALGID_SHA256WithKBKDF_CTR	= 0x60020500 | MC_KCMV,

	/* wrapkey */
	MC_ALGID_SHA1WithSEED			= 0x60040200,
	MC_ALGID_SHA256WithARIA128		= 0x600B0502 | MC_KCMV

} MC_ALGID;

typedef struct mc_algParam {
	MC_UCHAR *pNonce;
	MC_UCHAR *pAData;
	MC_UINT nNonce;
	MC_UINT nAData;
	MC_UINT nTLen;
	MC_UINT nDataLen;
} MC_ALGPARAM;

typedef struct mc_algorithm {
	MC_ALGID mcAlgId;
	MC_UCHAR *pParam;
	MC_UINT nParam;
} MC_ALGORITHM;

typedef enum mc_algMode {
	MC_ALGMODE_CBC					= 0x00000000,
	MC_ALGMODE_ECB					= 0x00000001,
	MC_ALGMODE_OFB					= 0x00000002,
	MC_ALGMODE_CFB					= 0x00000003,
	MC_ALGMODE_CTR					= 0x00000004,
	MC_ALGMODE_GCM					= 0x00000005,
	MC_ALGMODE_CCM					= 0x00000006
} MC_ALGMODE;

typedef enum mc_padType {
	MC_PADTYPE_PKCS5				= 0x00000000,
	MC_PADTYPE_PKCS7				= 0x00000100,
	MC_PADTYPE_SSL					= 0x00000200,
	MC_PADTYPE_WTLS					= 0x00000300,
	MC_PADTYPE_NONE					= 0x00000400,
	MC_PADTYPE_ONE					= 0x00000500,
	MC_PADTYPE_X923					= 0x00000600
} MC_PADTYPE;

typedef enum mc_apiMode {
	MC_MODE_NON_KCMV		= 0,
	MC_MODE_KCMV			= 1
} MC_APIMODE;

/* Log 함수 정의 */
typedef int(*MC_CALLBACK)(const char* path, const char* fmt, ...);

#ifndef NULL
#define NULL 0
#endif

#define IN	/* INPUT  */
#define OUT	/* OUTPUT */
#define IO	/* INPUT & OUTPUT */


#endif /* _MC_HEADER_708CA1EB_1BEB_4FE9_8408_BDD575B0D27B */
