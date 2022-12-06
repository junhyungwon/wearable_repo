/*----------------------------------------------------------------------------
 Defines referenced header files
-----------------------------------------------------------------------------*/
#include "app_rsa.h"

int app_rsa_generate_privatekey(char *pw) {

	char cmd[256];

	if( access(CFG_DIR_NAND , F_OK) != 0) {
		mkdir(CFG_DIR_NAND, 0775);
	}

    // nand->passphrase 파일에 암호문구 저장.
    TRACE_INFO("Save the passphrase to PATH_SSL_PASSPHRASE_NAND : %s\n", PATH_SSL_PASSPHRASE_NAND);
    FILE *fp = fopen(PATH_SSL_PASSPHRASE_NAND, "w");
    if(!fp) {
        TRACE_INFO("Failed to save the passphrase to PATH_SSL_PASSPHRASE_NAND : %s\n", PATH_SSL_PASSPHRASE_NAND);
        return -1;
    }

    fputs(pw, fp);
    fclose(fp);

    // rsa key 생성
    TRACE_INFO("Generate a private key with passphrase:%s\n", pw);
    sprintf(cmd, "openssl genrsa -aes128 -passout pass:%s -out %s %d", pw, PATH_SSL_PRIVATE_NAND, RSA_BIT);
	TRACE_INFO("cmd: %s\n", cmd);
	system(cmd);

    // export용 mmc에 저장
    sprintf(cmd, "cp -f %s %s", PATH_SSL_PRIVATE_NAND, PATH_SSL_PRIVATE_MMC);
    system(cmd);

    return 0;
}

static int pem_password_callback(char *buf, int max_len, int flag, void *ctx)
{
    char passphrase[1024] = {0, };

    FILE *f = fopen(PATH_SSL_PASSPHRASE_NAND, "r");
    if (!f) {
        TRACE_INFO("unable to read PATH_SSL_PASSPHRASE_NAND: %s\n", PATH_SSL_PASSPHRASE_NAND);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    if (fsize > 1024)
        return 0;
    fseek(f, 0, SEEK_SET);
    fread(passphrase, fsize, 1, f);
    fclose(f);

    if(fsize > max_len)
        return 0;

    memcpy(buf, passphrase, fsize);
    return passphrase;
}


RSA* app_rsa_get_rsa() {
    RSA *rsa = NULL;
    BIO *keybio;

    char *pem;
    {
        FILE *f = fopen(PATH_SSL_PRIVATE_NAND, "r");
        if (!f) {
            TRACE_INFO("unable to read PATH_SSL_PRIVATE_NAND: %s\n", PATH_SSL_PRIVATE_NAND);
            return NULL;
        }

        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        pem = malloc(fsize + 1);
        fread(pem, fsize, 1, f);
        fclose(f);

        pem[fsize] = 0;
    }

    keybio = BIO_new_mem_buf(pem, -1);
    free(pem);

    if (keybio == NULL) {
        TRACE_INFO("fail to create the bio buf.\n");
        return NULL;
    }

    // note for public key : PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, pem_password_callback, NULL);
    if (rsa == NULL) {
        TRACE_INFO("fail to read the rsa private key.\n");
    }

    return rsa;
}

int app_rsa_encrypt_by_private(RSA *rsa, unsigned char *data, int len, unsigned char *encrypted)
{
    if (rsa == NULL) {
        rsa = app_rsa_get_rsa();
        if (rsa == NULL)
            return -1;

    }
    int result = RSA_private_encrypt(len, data, encrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}

int app_rsa_decrypt_by_private(RSA *rsa, unsigned char *encrypted, int len, unsigned char *decrypted)
{
    if (rsa == NULL) {
        rsa = app_rsa_get_rsa();
        if (rsa == NULL)
            return -1;

    }

    int  result = RSA_private_decrypt(len, encrypted, decrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}