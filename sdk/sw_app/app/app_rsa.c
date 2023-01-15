/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_rsa.h"

static inline void _internal_passphrase(char *passphrase) {
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

int app_rsa_passphrase_to_sd()
{
    char cmd[256];
    if(access(PATH_SSL_PASSPHRASE_NAND, F_OK) ==0)
    {
        sprintf(cmd, "cp -f %s %s", PATH_SSL_PASSPHRASE_NAND, PATH_SSL_PASSPHRASE_MMC);
        TRACE_INFO("copy the passphrase file to sdcard. cmd: %s\n", cmd);
        system(cmd);
    }
    return SUCC ;
}

int app_rsa_load_passphrase(char *passphrase, int *passphrase_len) {
    FILE *f = fopen(PATH_SSL_PASSPHRASE_NAND, "r");
    if (!f) {
        TRACE_INFO("unable to read PATH_SSL_PASSPHRASE_NAND: %s\n", PATH_SSL_PASSPHRASE_NAND);
        return FAIL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);

    fseek(f, 0, SEEK_SET);
    fread(passphrase, fsize, 1, f);
    fclose(f);

    if (fsize > 80) { // 64 + 16
        TRACE_INFO("wrong size\n");
        return FAIL;
    }

    {
        char internal_passphrase[64];
        unsigned char aes_128_key[16] = {0,};
        unsigned char aes_128_iv[16] = {0,};

        // 내부 암호문구 가져오기
        _internal_passphrase(internal_passphrase);
        TRACE_INFO("_internal_passphrase ok\n");
        if (openssl_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
            TRACE_INFO("openssl_aes128_derive_key error\n");
            return FAIL;
        }
        TRACE_INFO("openssl_aes128_derive_key ok : %s(%d)\n", internal_passphrase, sizeof(internal_passphrase));

        AES_KEY aes_key;
        AES_set_decrypt_key(aes_128_key, KEY_BIT, &aes_key);
        AES_cbc_encrypt((unsigned char *)passphrase ,(unsigned char *)passphrase ,fsize ,&aes_key, &aes_128_iv, AES_DECRYPT);
    }

    *passphrase_len = 64; // fixed;

    return SUCC;
}

int app_rsa_save_passphrase(char *pw) {
	char cmd[256];
    char mdString[SHA256_DIGEST_LENGTH*2+1];

	if( access(CFG_DIR_NAND , F_OK) != 0) {
		mkdir(CFG_DIR_NAND, 0775);
	}

    // sha256sum
    {
        int i;
        unsigned char digest[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)pw, strlen(pw), (unsigned char*)&digest);

        for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
            sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    }
    // TRACE_INFO("sha256 hashed the passphrase : %s(%d) %s(%d)\n", pw, strlen(pw), mdString, strlen(mdString));

    int len, padding_len;
    char buf[SHA256_DIGEST_LENGTH*2+1 + BLOCK_SIZE];    // big enough for passphrase
    {
        unsigned char aes_128_key[16] = {0,};
        unsigned char aes_128_iv[16] = {0,};
        char internal_passphrase[64];   // 64 fixed

        // 내부 암호문구 가져오기
        _internal_passphrase(internal_passphrase);
        TRACE_INFO("_internal_passphrase ok \n");
        if (openssl_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
            TRACE_INFO("openssl_aes128_derive_key error\n");
            return FAIL;
        }
        TRACE_INFO("openssl_aes128_derive_key ok : %s(%d)\n", internal_passphrase, sizeof(internal_passphrase));

        len = sizeof(mdString);
        padding_len=BLOCK_SIZE - len % BLOCK_SIZE;
        memset(buf+len, padding_len, padding_len);

        AES_KEY aes_key;
        AES_set_encrypt_key(aes_128_key, KEY_BIT, &aes_key);
        AES_cbc_encrypt((unsigned char *)mdString ,(unsigned char *)buf, len+padding_len, &aes_key, &aes_128_iv, AES_ENCRYPT);
    }

    // nand->passphrase 파일에 암호문구 저장.
    TRACE_INFO("Save the passphrase to PATH_SSL_PASSPHRASE_NAND : %s\n", PATH_SSL_PASSPHRASE_NAND);
    FILE *fp = fopen(PATH_SSL_PASSPHRASE_NAND, "w");
    if(!fp) {
        TRACE_INFO("Failed to save the passphrase to PATH_SSL_PASSPHRASE_NAND : %s\n", PATH_SSL_PASSPHRASE_NAND);
        return -1;
    }

    fwrite(buf, RW_SIZE, len+padding_len, fp);
    fclose(fp);

	// copy the passphrase file to sdcard
	app_rsa_passphrase_to_sd();

    return SUCC;
}

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
    char passphrase[64] = {'\0', };
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

    if (app_rsa_load_passphrase(passphrase, &passphrase_len) == FAIL) {
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
