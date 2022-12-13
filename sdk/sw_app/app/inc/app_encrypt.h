/******************************************************************************
 * XBX Board
 * Copyright by	UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file	app_encrypt.h
 * @brief
 * @author	hwjun
 * @section	MODIFY history
 */
/*****************************************************************************/

#ifndef _APP_ENCRYPT_H_
#define _APP_ENCRYPT_H_


/*----------------------------------------------------------------------------
 Defines referenced	header files
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 Declares a	function prototype
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

int encrypt_aes(const char *src, char *dst, int len) ;
char *SHA256_process(char *string) ;
void openssl_aes128_encrypt(char *src, char *dst) ;
int openssl_aes128_encrypt_fs(char *src, char *dst) ;

int openssl_aes128_derive_key(const char* str, const int str_len, unsigned char *key, unsigned char *iv);
#ifdef __cplusplus
}
#endif

#endif	/* _APP_ENCRYPT_H_ */
