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
#include "lf.h"

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
typedef struct {
	int excnt ;
	int passphrase_len ;
	char passphrase[64] ;
	unsigned char aes_128_key[16] ;
	unsigned char aes_128_iv[16] ;
} encrypt_struct ;

static encrypt_struct encrypt_obj ;
encrypt_struct *encrypt_str = &encrypt_obj ;


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

void openssl_aes128_encrypt_nopad_nosalt(char *src, char *dst, int type)
{
    int i = 0, len=1024, padding_len=0 ;
    char buf[1024 + BLOCK_SIZE] = {0, };
//    char tbuf[1500] = {0, };

    if(encrypt_str->excnt == 0)
    {
	    if (lf_rsa_load_passphrase(encrypt_str->passphrase, &encrypt_str->passphrase_len) != SUCC) {
           return FAIL;
        }
        if (openssl_aes128_derive_key(encrypt_str->passphrase, encrypt_str->passphrase_len, encrypt_str->aes_128_key, encrypt_str->aes_128_iv) == FAIL)			{
            return FAIL;
        }
        encrypt_str->excnt = 1 ;
    }

    AES_set_encrypt_key(encrypt_str->aes_128_key, KEY_BIT, &aes_key_128); 
    memcpy(buf, src, len) ;

#if 1
    AES_cbc_encrypt((unsigned char *)buf ,dst ,len ,&aes_key_128, encrypt_str->aes_128_iv, AES_ENCRYPT);
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
    
    encrypt_str->excnt = 0 ;
//	app_file_add(fname) ;
}


int avi_file_write(FILE *favi, stream_info_t *ifr, int encrypt_vid)
{
	AVI_FRAME_PARAM frame;
	int enc_size=0, sz/*, i = 0*/;
	char *tmp=NULL;
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
