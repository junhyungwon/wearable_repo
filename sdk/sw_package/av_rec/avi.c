/******************************************************************************
 * NEXTT360 Board
 * Copyright by UDWorks, Incoporated. All Rights Reserved.
 *---------------------------------------------------------------------------*/
 /**
 * @file    app_main.c
 * @brief
 * @section MODIFY history
 *     - 2020.07.08 : First Created
 */
/*****************************************************************************/

/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <cmem.h>

#include <fcntl.h>
#include "app_base64.h"
#include "main.h"
#include "avi.h"

/*----------------------------------------------------------------------------
 Definitions and macro
-----------------------------------------------------------------------------*/
#define MAX_ENCRYPT_BIT 1024
#define BLOCK_SIZE     16
#define KEY_BIT        128

/*----------------------------------------------------------------------------
 Declares variables
-----------------------------------------------------------------------------*/
static unsigned int gmem_addr;
extern char *enc_buf;

/*----------------------------------------------------------------------------
 Declares a function prototype
-----------------------------------------------------------------------------*/

static inline void Internal_passphrase(char *passphrase) {
    int i, olen;

    // openssl rand 64 | base64 -w 0
    char aa[88+1] = "itasmMpNlyV7nE5FgyaFNSdFQtHRbykqunKlOqxBy6hpTw02k0jWbbbLKWwLvRlYCpLm9lob7cucAREOpzG10w==";
    long long p = 264233017;
    long long q = 218561699;

    unsigned char* decoded = (char *)base64_decode(aa, strlen(aa), &olen);
    strncpy(passphrase, decoded, olen);
    for (i = 0; i< olen; i++) {
        passphrase[i] = (passphrase[i] * p) % q;
    }
    free(decoded);
}


/**
* read /dev/urandom
*/
void urandom_value(char *outdata, int count) {
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(outdata, sizeof(char), count, fp);
	fclose(fp);
}
int app_rsa_passphrase_to_sd()
{
    char cmd[256];
    if(access(PATH_SSL_PASSPHRASE_NAND, F_OK) ==0)
    {
        sprintf(cmd, "cp -f %s %s", PATH_SSL_PASSPHRASE_NAND, PATH_SSL_PASSPHRASE_MMC);
 //       TRACE_INFO("copy the passphrase file to sdcard. cmd: %s\n", cmd);
        system(cmd);
    }
    return SUCC ;
}



int app_rsa_save_passphrase(const char* pw) {
    int i;

    // Make sure CFG_DIR_NAND directory exists
    if (access(CFG_DIR_NAND , F_OK) != 0) {
        mkdir(CFG_DIR_NAND, 0775);
    }

    // Hash the passphrase using SHA256
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)pw, strlen(pw), digest);
    char mdString[SHA256_DIGEST_LENGTH * 2 + 1];
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&mdString[i * 2], "%02x", digest[i]);
    }

    // Encrypt the hashed passphrase using AES128
    unsigned char aes_128_key[16] = {0,};
    unsigned char aes_128_iv[16] = {0,};
    char internal_passphrase[64];
    Internal_passphrase(internal_passphrase);
    if (openssl_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
        return FAIL;
    }

    int len = strlen(mdString);
    int padding_len = BLOCK_SIZE - len % BLOCK_SIZE;
    char buf[SHA256_DIGEST_LENGTH*2+1 + BLOCK_SIZE];
    memset(buf+len, padding_len, padding_len);
    AES_KEY aes_key;
    AES_set_encrypt_key(aes_128_key, KEY_BIT, &aes_key);
    AES_cbc_encrypt((unsigned char *)mdString, (unsigned char *)buf, len + padding_len, &aes_key, &aes_128_iv, AES_ENCRYPT);
    OPENSSL_cleanse(&aes_key, sizeof(AES_KEY));

    // Write the encrypted hashed passphrase to PATH_SSL_PASSPHRASE_NAND
    FILE *fp = fopen(PATH_SSL_PASSPHRASE_NAND, "w");
    if (!fp) {
        return FAIL;
    }
//    fwrite(buf, RW_SIZE, len + padding_len, fp);
    fwrite(buf, 1, len + padding_len, fp);
    fclose(fp);

    // Copy the passphrase file to the SD card
    app_rsa_passphrase_to_sd();

    return SUCC;
}

void packet_dump(char* buf, int size, int unit)
{
    char     tmp[150];
    char     msg[256];
    int      i, j, lno;

    j = 7;
	lno = 0;
    memset((tmp+0), 0x00, 150);
								       
    for (i = 0; i < size; i++) {
        if((i%unit) == 0)
            memset((tmp+0), 0x20, ((unit*3)+unit+1+7+3));
	    if(j == 7)
	        sprintf((tmp+0), "[%4d] ", lno);
	    sprintf((tmp+j), "%02x", (unsigned char)*(buf+i));
        tmp[j+2] = 0x20;
	    if((unsigned char)*(buf+i) >= (unsigned char)0x20)
	        tmp[(j/3)+(unit*3)+1+6] = *(buf+i);
	    else
	        tmp[(j/3)+(unit*3)+1+6] = '.';
        j += 3;
        if((i%unit) == (unit-1)) {
	        printf("%s\n", (tmp+0));
	        j = 7;
            ++lno;
	    }
        else if((i%unit) == ((unit/2)-1))
	        tmp[j-1] = '-';
    }
    if ((i % unit) != 0) {
	     printf("%s\n", (tmp+0));
    }
}

int openssl_aes128_derive_key(const char* str, const int str_len, unsigned char *key, unsigned char *iv) {

    // EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_MD *dgst = (EVP_MD *)EVP_sha256();

    unsigned char tmpkeyiv[EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH];
    int iklen = 16; //EVP_CIPHER_get_key_length(cipher);
    int ivlen = 16; //EVP_CIPHER_get_iv_length(cipher);

//    int islen = 0; // note : salt size.
    if (!PKCS5_PBKDF2_HMAC(str, str_len, NULL /*sptr*/, 0 /*islen*/, 10000 /*iter*/, dgst, iklen+ivlen, tmpkeyiv)) {
        char err[256];
        ERR_error_string(ERR_get_error(), err);
        printf("fail to derive the key and iv by PKCS5_PBKDF2_HMAC(). %s\n", err);
        return FAIL;
    }

    /* split and move data back to global buffer */
    memcpy(key, tmpkeyiv, iklen);
    memcpy(iv, tmpkeyiv+iklen, ivlen);


    // caution : only for debug usage!!
#if 0 
    int i;
    printf("key=");
    for (i = 0; i < 16 /*EVP_CIPHER_get_key_length(cipher)*/; i++)
        printf("%02X", key[i]);
    printf("\n");
    printf("iv=");
    for (i = 0; i < 16 /*EVP_CIPHER_get_key_length(cipher)*/; i++)
        printf("%02X", iv[i]);
    printf("\n");
#endif
    return 0 ;
}

int app_rsa_load_passphrase(char *passphrase, int *passphrase_len) {
// If passphrase file doesn't exist, generate random passphrase and save it
    if (access(PATH_SSL_PASSPHRASE_NAND, F_OK) != 0) {
        char passphrase_new[SHA256_DIGEST_LENGTH*2+1] = {'\0', };
        int olen;

        char buf[64];
        urandom_value(buf, sizeof(buf));
        unsigned char* encoded = (unsigned char *)base64_encode(buf, sizeof(buf), &olen);
        strncpy(passphrase_new, (char*)encoded, olen);

        printf("%s doesn't exist. Creating with a random bytes. %s\n", PATH_SSL_PASSPHRASE_NAND, passphrase_new);
        app_rsa_save_passphrase(passphrase_new);
        free(encoded);
    }

		FILE *f = fopen(PATH_SSL_PASSPHRASE_NAND, "r");
    if (!f) {
        printf("unable to read PATH_SSL_PASSPHRASE_NAND: %s\n", PATH_SSL_PASSPHRASE_NAND);
        return FAIL;
    }

    // Read passphrase from file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (fread(passphrase, fsize, 1, f) != 1) {
        printf("Unable to read passphrase from %s\n", PATH_SSL_PASSPHRASE_NAND);
        fclose(f);
        return FAIL;
    }
    fclose(f);

    if (fsize > 80) { // 64 + 16
        printf("Passphrase file size is too big: %ld\n", fsize);
        return FAIL;
    }

	{
        // Decrypt passphrase using internal passphrase
        char internal_passphrase[64];
        unsigned char aes_128_key[16] = {0,};
        unsigned char aes_128_iv[16] = {0,};

        Internal_passphrase(internal_passphrase);
        if (openssl_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
            printf("Failed to derive AES key from internal passphrase\n");
            return FAIL;
        }

        AES_KEY aes_key;
        AES_set_decrypt_key(aes_128_key, KEY_BIT, &aes_key);
        AES_cbc_encrypt((unsigned char *)passphrase, (unsigned char *)passphrase, fsize, &aes_key, &aes_128_iv, AES_DECRYPT);
        OPENSSL_cleanse(&aes_key, sizeof(AES_KEY));
    }

    *passphrase_len = 64; // fixed;

    return SUCC;	
}

int excnt = 0 ;
unsigned char aes_128_key_stream[16] = {0, }; 
unsigned char aes_128_iv_stream[16] = {0, }; 
char passphrase[64] = {'\0', };
int passphrase_len = 0;

void openssl_aes128_encrypt_nopad_nosalt(char *src, char *dst, int type)
{
    int i = 0, len=0, padding_len=0 ;
    char buf[1024 + BLOCK_SIZE] = {0, };
    char tbuf[1500] = {0, };

    if(excnt == 0)
    {
	    if (app_rsa_load_passphrase(passphrase, &passphrase_len) != SUCC) {
           return FAIL;
        }
        if (openssl_aes128_derive_key(passphrase, passphrase_len, aes_128_key_stream, aes_128_iv_stream) == FAIL) {
            return FAIL;
        }
        excnt = 1 ;
    }

	len = 1024 ;
    AES_set_encrypt_key(aes_128_key_stream, KEY_BIT, &aes_key_128); 
    memcpy(buf, src, len) ;

#if 1
//    AES_cbc_encrypt((unsigned char *)buf ,dst ,len ,&aes_key_128, aes_128_iv, AES_ENCRYPT);
    AES_cbc_encrypt((unsigned char *)buf ,dst ,len ,&aes_key_128, aes_128_iv_stream, AES_ENCRYPT);
//  memcpy(tbuf, dst, 1500) ; 
//	packet_dump(tbuf, 1500, 16) ;
#else
    AES_cbc_encrypt(buf ,dst ,len+padding_len ,&aes_key_128, iv,AES_ENCRYPT);
#endif 

}

/*----------------------------------------------------------------------------
 audio codec function
-----------------------------------------------------------------------------*/
static int alg_ulaw_encode(unsigned short *dst, unsigned short *src, int bufsize)
{
    int i, isNegative;
    short data;
    short nOut;
    short lowByte = 1;
    int outputSize = bufsize / 2;

    for (i=0; i<outputSize; i++)
    {
        data = *(src + i);
        data >>= 2;
        isNegative = (data < 0 ? 1 : 0);

        if (isNegative)
            data = -data;

        if (data <= 1) 			nOut = (char) data;
        else if (data <= 31) 	nOut = ((data - 1) >> 1) + 1;
        else if (data <= 95)	nOut = ((data - 31) >> 2) + 16;
        else if (data <= 223)	nOut = ((data - 95) >> 3) + 32;
        else if (data <= 479)	nOut = ((data - 223) >> 4) + 48;
        else if (data <= 991)	nOut = ((data - 479) >> 5) + 64;
        else if (data <= 2015)	nOut = ((data - 991) >> 6) + 80;
        else if (data <= 4063)	nOut = ((data - 2015) >> 7) + 96;
        else if (data <= 7903)	nOut = ((data - 4063) >> 8) + 112;
        else 					nOut = 127;

        if (isNegative) {
            nOut = 127 - nOut;
        } else {
            nOut = 255 - nOut;
        }

        // Pack the bytes in a word
        if (lowByte)
            *(dst + (i >> 1)) = (nOut & 0x00FF);
        else
            *(dst + (i >> 1)) |= ((nOut << 8) & 0xFF00);

        lowByte ^= 0x1;
    }

	return (outputSize);
}

/*----------------------------------------------------------------------------
 avi file open/close function
-----------------------------------------------------------------------------*/
FILE *avi_file_open(char *filename, stream_info_t *ifr, int snd_on, int ch, int rate, int btime, int encrypt_vid)
{
    char msg[128] = {0, };
	AVI_SYSTEM_PARAM aviInfo;
	FILE *favi;
	int i;
	
	memset(&aviInfo, 0, sizeof(AVI_SYSTEM_PARAM));
	
	aviInfo.nVidCh	= REC_CH_NUM;
	aviInfo.bEnMeta	= TRUE; //# FALSE
	aviInfo.bEncrypt = encrypt_vid ;
	aviInfo.uVideoType	= ENCODING_H264;
	
	for (i = 0; i < aviInfo.nVidCh; i++) {
		aviInfo.nVidWi[i] = ifr->frm_wi;
		aviInfo.nVidHe[i] = ifr->frm_he;
	}
	/*
	 * 정수를 소수로 변환하는 과정에서 1->0.9xxx로 되면 재생할 때 문제가 발생함.
	 * 따라서 반올림 후 소수를 버림.(floor)
	 */
	aviInfo.fFrameRate	= floor((ifr->frm_rate*1000./1001.)+0.5L);

	if (snd_on) {
		aviInfo.bEnAudio 			= TRUE;
		aviInfo.nAudioType			= ENCODING_ULAW;
		aviInfo.nAudioChannel		= ch;
		aviInfo.nSamplesPerSec		= rate; //SND_PCM_SRATE;
		aviInfo.nAudioBitRate		= rate; //SND_PCM_SRATE;
		aviInfo.nAudioBitPerSample	= 16;   //# fixed 16bits
		aviInfo.nAudioFrameSize		= btime; //# fixed
	}
	
	favi = LIBAVI_createAvi(filename, &aviInfo);
	if (favi == NULL) {
		eprintf("LIBAVI_createAvi failed!\n");
	}

	return favi;
}

void avi_file_close(FILE *favi, char *fname)
{
	if (favi != NULL) {
		LIBAVI_closeAvi(favi);
	}
	favi = NULL;
    
    excnt = 0 ;
//	app_file_add(fname) ;
}


int avi_file_write(FILE *favi, stream_info_t *ifr, int encrypt_vid)
{
	AVI_FRAME_PARAM frame;
	int enc_size=0, sz, i = 0;
	char *tmp=NULL;
	char *video_data ;
	char encbuf[1500] = {0, };
	
	if (favi != NULL)
	{
		if (ifr->d_type == DATA_TYPE_VIDEO) 
		{
			frame.buf			= (char *)(gmem_addr+ifr->offset); //(ifr->addr);

#if 1   // TTA Encrypt video data
			if(encrypt_vid)
			{	
				openssl_aes128_encrypt_nopad_nosalt(frame.buf, encbuf, 1) ;
//			packet_dump(encbuf, 1024, 16) ;
				if(ifr->is_key)
				{
/*					
					for(i = 0 ; i < MAX_ENCODE_BIT; i++)
					{
						frame.buf[i] = ~(frame.buf[i]) ;
					}
*/
/*
					for(i = 0 ; i < MAX_ENCRYPT_BIT; i++)
					{
						frame.buf[i] = encbuf[i] ;
					}
*/
					memcpy(frame.buf, encbuf, 1024) ;
				}
			}
#endif 
			frame.size			= ifr->b_size;
			frame.data_type		= ifr->d_type;
			frame.iskey_frame 	= ifr->is_key;			
#if defined(NEXXB) && (REC_CH_NUM == 3)
			if (ifr->ch == 1) {
				/* invalid channel */
				return;
			} else if (ifr->ch >= 2) {
				frame.channel = ifr->ch - 1;
			} else 
				frame.channel = ifr->ch;
#else
			frame.channel = ifr->ch;
#endif
		}
		else if(ifr->d_type == DATA_TYPE_AUDIO) 
		{
			/* muraw encoding required */
			tmp = (char *)(gmem_addr+ifr->offset);
			sz  = ifr->b_size;
			enc_size=alg_ulaw_encode((unsigned short *)enc_buf, (unsigned short *)tmp, sz);
			
			frame.buf       = enc_buf;
			frame.size      = enc_size;
			frame.data_type	= ifr->d_type;
		}
		else if(ifr->d_type == DATA_TYPE_META) 
		{
			frame.buf		= (char *)(gmem_addr+ifr->offset); //(ifr->addr);
			frame.size		= ifr->b_size;
			frame.data_type	= ifr->d_type;
 		}
		else 
		{
			dprintf("unknown data type\n");
			return 0;
		}
		
		if (LIBAVI_write_frame(favi, &frame) < 0) {
			return -1;
		}
	}

	return 0;
}

int avi_file_init(unsigned int addr)
{
	/* ifr->offset을 사용할 경우 이 값을 더해야 함 */
	gmem_addr = addr;
	
	return 0;
}
