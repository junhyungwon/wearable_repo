/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_rsa.h"

int app_rsa_generate_privatekey(char *pw) {
	char cmd[256];

    // rsa key 생성
    TRACE_INFO("Generate a private key with passphrase:%s\n", pw);
    sprintf(cmd, "openssl genrsa -aes128 -passout pass:%s -out %s %d", pw, PATH_SSL_PRIVATE_NAND, RSA_BIT);
	TRACE_INFO("cmd: %s\n", cmd);
	system(cmd);

    // export용 mmc에 저장
    sprintf(cmd, "cp -f %s %s", PATH_SSL_PRIVATE_NAND, PATH_SSL_PRIVATE_MMC);
    system(cmd);

    return SUCC;
}

RSA* app_rsa_get_rsa() {
    char passphrase[SHA256_DIGEST_LENGTH*2+BLOCK_SIZE] = {'\0', };
    int passphrase_len = 0;
    RSA *rsa = NULL;
    BIO *bio;

    if(access(PATH_SSL_PASSPHRASE_NAND, F_OK) !=0) {
        TRACE_INFO("passphrase file is not exist. %s\n", PATH_SSL_PASSPHRASE_NAND);
        return NULL;
    }
    if(access(PATH_SSL_PRIVATE_NAND, F_OK) !=0) {
        TRACE_INFO("private file is not exist. %s\n", PATH_SSL_PASSPHRASE_NAND);
        return NULL;
    }

    if (lf_rsa_load_passphrase(passphrase, &passphrase_len) == FAIL) {
        return NULL;
    }

    FILE *f = fopen(PATH_SSL_PRIVATE_NAND, "r");
    // note for public key : PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    rsa = PEM_read_RSAPrivateKey(f, NULL, 0, passphrase);
    fclose(f);

    if (rsa == NULL) {
        char err[256];
        ERR_error_string(ERR_get_error(), err);
        TRACE_INFO("fail to read the rsa private key. %s\n", err);
    }

    TRACE_INFO("the private rsa key loaded.: %s\n", PATH_SSL_PRIVATE_NAND);
    return rsa;
}

int app_rsa_encrypt_by_private(RSA *rsa, unsigned char *data, int len, unsigned char *encrypted) {
    // note : encrypt by the public from the private rsa key
    int encBytes = RSA_public_encrypt(len, data, encrypted, rsa, RSA_PKCS1_PADDING);
    return encBytes;
}

int app_rsa_decrypt_by_private(RSA *rsa, unsigned char *encrypted, int len, unsigned char *decrypted) {
    int decBytes = RSA_private_decrypt(len, encrypted, decrypted, rsa, RSA_PKCS1_PADDING);
    return decBytes;
}

// https://www.openssl.org/docs/man1.1.1/man3/RSA_private_encrypt.html
int app_rsa_encrypt_fs_by_private(char *src, char *dst) {
    // validate
    if(access(src, F_OK) !=0) {
        TRACE_INFO("src file is not exist. %s\n", src);
        return NULL;
    }

    RSA *rsa = app_rsa_get_rsa();
	if(rsa == NULL)
		return 0 ;

    int rsa_size = RSA_size(rsa);
    int size = rsa_size - RSA_PKCS1_PADDING_SIZE;
    unsigned char buf[rsa_size];
    unsigned char enc[rsa_size];

    FILE *fp = fopen(src, "rb") ;
    if(fp == NULL) {
        TRACE_INFO("[ERROR] can not fopen('%s')\n", src);
        return FAIL;
    }

    FILE *wfp = fopen(dst,"wb");
    if( wfp == NULL ){
        TRACE_INFO("[ERROR] can not fopen('%s')\n", dst);
        return FAIL;
    }

    int len, encBytes;
    while((len = fread(buf, 1, size, fp))) {
        encBytes = RSA_public_encrypt(len, (const unsigned char*)buf, enc, rsa, RSA_PKCS1_PADDING);
        // debug
        TRACE_INFO("encrypted. size: %d, len: %d, encBytes: %d\n", size, len, encBytes);
        if (encBytes == -1) {
            break;
        }

        fwrite(enc, 1, encBytes, wfp) ;
    }

    RSA_free(rsa);
	fclose(wfp);
	fclose(fp);
    sync();

    if (encBytes == -1) {
        char err[256];
        ERR_error_string(ERR_get_error(), err);
        TRACE_INFO("fail to encrypt. %s\n", err);
        
        return FAIL;
    }

    TRACE_INFO("encrypt a file done. from %s to %s , rsa size: %d\n", src, dst, rsa_size);
    return SUCC;
}


int app_rsa_decrypt_fs_by_private(char *src, char *dst) {  
    // validate
    if(access(src, F_OK) !=0) {
        TRACE_INFO("src file is not exist. %s\n", src);
        return NULL;
    }

    RSA *rsa = app_rsa_get_rsa();
	if(rsa == NULL)
		return 0 ;

    int rsa_size = RSA_size(rsa);
    unsigned char enc[rsa_size];
    unsigned char dec[rsa_size];

    FILE *fp = fopen(src, "rb") ;
    if(fp == NULL) {
        TRACE_INFO("[ERROR] can not fopen('%s')\n", src);
        return FAIL;
    }

    FILE *wfp = fopen(dst,"wb");
    if( wfp == NULL ){
        TRACE_INFO("[ERROR] can not fopen('%s')\n", dst);
        return FAIL;
    }

    int len, decBytes;
    while((len = fread(enc, 1, rsa_size, fp))) {
        decBytes = RSA_private_decrypt(len, (const unsigned char*)enc, dec, rsa, RSA_PKCS1_PADDING);
        // debug
        TRACE_INFO("decrtyped. rsa_size: %d, len: %d, decBytes: %d\n", rsa_size, len, decBytes);        
        if (decBytes == -1) {
            break;
        }
        fwrite(dec, 1, decBytes, wfp) ;
    }

    RSA_free(rsa);
	fclose(wfp);
	fclose(fp);
    sync();

    if (decBytes == -1) {
        char err[256];
        ERR_error_string(ERR_get_error(), err);
        TRACE_INFO("fail to decrypt. %s\n", err);
        
        return FAIL;
    }

    TRACE_INFO("decrypt a file done. from %s to %s , rsa size: %d\n", src, dst, rsa_size);
    return SUCC;
}
