#ifndef _SW_LOGIC_H_
#define _SW_LOGIC_H_

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

#define RSA_BIT                  (2048)                             // aes128 rsa bit

#define BLOCK_SIZE 16
#define FREAD_COUNT 4096
#define KEY_BIT 128
#define IV_SIZE 16
#define RW_SIZE 1
#define SUCC 1
#define FAIL -1 

// defined in app_main.h
// #define CFG_DIR_MMC             "/mmc/cfg"
#define CFG_DIR_NAND            "/media/nand/cfg"
#define PATH_SSL_PASSPHRASE_NAND CFG_DIR_NAND"/passphrase"	        // fixme : better secure place?
#define PATH_SSL_PASSPHRASE_MMC  "/mmc/cfg""/passphrase"

#define SERVER_TEMP_PUBKEY_PATH  "/tmp/tempkey.pub"
#define SERVER_TEMP_PRIKEY_PATH  "/tmp/tempkey.pem"

// defined in app_comm.h
/** ANSI printf color **/
/** black \033[30m **/
/** red   \033[31m **/
/** green \033[32m **/
/** yellow\033[33m **/
/** blue  \033[34m **/
/** pink  \033[35m **/
/** teal  \033[36m **/
/** white \033[37m **/

#ifdef __DEBUG__
#define TRACE_INFO(x,...) do { printf(" [app ] \033[32m%s: \033[0m", __func__); printf(x, ##__VA_ARGS__); fflush(stdout); } while(0)
#define TRACE_ERR(x,...) do { printf(" [app err!] \033[31m%s: \033[0m", __func__); printf(x, ##__VA_ARGS__); fflush(stdout); } while(0)

#	if USE_SYSLOGD
#	define LOGD(x...)	do {printf(" [app ] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_INFO, x);} while(0)
#	define LOGE(x...)	do {printf(" [app err!] %s: ", __func__); printf(x); fflush(stdout); syslog(LOG_ERR, x);} while(0)
#	else
#	define LOGD(x...)	do {printf(" [app ] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	define LOGE(x...)	do {printf(" [app err!] %s: ", __func__); printf(x); fflush(stdout);} while(0)
#	endif
#else
#define TRACE_INFO(x,...)
#define TRACE_ERR(x,...)

#	define LOGD(x...)	
#	define LOGE(x...)
#endif


#ifdef __cplusplus
extern "C"
{
#endif
void lf_urandom_value(char *outdata, int count);   // read /dev/urandom
int lf_rsa_passphrase_to_sd(void);
int lf_rsa_load_passphrase(char *passphrase, int *passphrase_len);
int lf_rsa_save_passphrase(const char *pw);
int lf_base64_encode(const unsigned char *input, int length, char **output);
int lf_base64_decode(char *input, unsigned char **output);
int lf_base64_en(RSA* rsa, char *input, int length, unsigned char *output);
int lf_base64_de(RSA* rsa, char *input, unsigned char *output);
int lf_aes128_derive_key(const char* str, const int str_len, unsigned char *key, unsigned char *iv);
#ifdef __cplusplus
}
#endif

#endif	/* _SW_LOGIC_H_ */
