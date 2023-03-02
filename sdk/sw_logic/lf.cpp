#include <lf.h>

static inline void _internal_passphrase(char *passphrase) {
    int i, olen;

    // openssl rand 64 | base64 -w 0
    char aa[88+1] = "itasmMpNlyV7nE5FgyaFNSdFQtHRbykqunKlOqxBy6hpTw02k0jWbbbLKWwLvRlYCpLm9lob7cucAREOpzG10w==";
    long long p = 264233017;
    long long q = 218561699;

    unsigned char* decoded; // = (char *)base64_decode(aa, strlen(aa), &olen);
    olen = lf_base64_decode(aa, &decoded);

    strncpy(passphrase, (const char*) decoded, olen);
    for (i = 0; i< olen; i++) {
        passphrase[i] = (passphrase[i] * p) % q;
    }
    free(decoded);
}

/**
* read /dev/urandom
*/
void lf_urandom_value(char *outdata, int count) {
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(outdata, sizeof(char), count, fp);
	fclose(fp);
}

int lf_rsa_passphrase_to_sd()
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

int lf_rsa_load_passphrase(char *passphrase, int *passphrase_len) {
    // If passphrase file doesn't exist, generate random passphrase and save it
    if (access(PATH_SSL_PASSPHRASE_NAND, F_OK) != 0) {
        char passphrase_new[SHA256_DIGEST_LENGTH*2+1] = {'\0', };
        int olen;

        char buf[64];
        lf_urandom_value(buf, sizeof(buf));
        char* encoded; // = (unsigned char *)base64_encode(buf, sizeof(buf), &olen);
        olen = lf_base64_encode((const unsigned char *)buf, sizeof(buf), &encoded);
        strncpy(passphrase_new, (char*)encoded, olen);

        TRACE_INFO("%s doesn't exist. Creating with a random bytes. %s\n", PATH_SSL_PASSPHRASE_NAND, passphrase_new);
        lf_rsa_save_passphrase(passphrase_new);
        free(encoded);
    }

    FILE *f = fopen(PATH_SSL_PASSPHRASE_NAND, "r");
    if (!f) {
        TRACE_INFO("unable to read PATH_SSL_PASSPHRASE_NAND: %s\n", PATH_SSL_PASSPHRASE_NAND);
        return FAIL;
    }

    // Read passphrase from file
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (fread(passphrase, fsize, 1, f) != 1) {
        TRACE_INFO("Unable to read passphrase from %s\n", PATH_SSL_PASSPHRASE_NAND);
        fclose(f);
        return FAIL;
    }
    fclose(f);

    if (fsize > 80) { // 64 + 16
        TRACE_INFO("Passphrase file size is too big: %ld\n", fsize);
        return FAIL;
    }

    {
        // Decrypt passphrase using internal passphrase
        char internal_passphrase[64];
        unsigned char aes_128_key[16] = {0,};
        unsigned char aes_128_iv[16] = {0,};

        _internal_passphrase(internal_passphrase);
        if (lf_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
            TRACE_INFO("Failed to derive AES key from internal passphrase\n");
            return FAIL;
        }

        AES_KEY aes_key;
        AES_set_decrypt_key(aes_128_key, KEY_BIT, &aes_key);
        AES_cbc_encrypt((unsigned char *)passphrase, (unsigned char *)passphrase, fsize, &aes_key, (unsigned char*)&aes_128_iv, AES_DECRYPT);
        OPENSSL_cleanse(&aes_key, sizeof(AES_KEY));
    }

    *passphrase_len = 64; // fixed;

    return SUCC;
}

int lf_rsa_save_passphrase(const char* pw) {
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
    _internal_passphrase(internal_passphrase);
    if (lf_aes128_derive_key(internal_passphrase, sizeof(internal_passphrase), aes_128_key, aes_128_iv) == FAIL) {
        return FAIL;
    }
    int len = strlen(mdString);
    int padding_len = BLOCK_SIZE - len % BLOCK_SIZE;
    char buf[SHA256_DIGEST_LENGTH*2+1 + BLOCK_SIZE];
    memset(buf+len, padding_len, padding_len);
    AES_KEY aes_key;
    AES_set_encrypt_key(aes_128_key, KEY_BIT, &aes_key);
    AES_cbc_encrypt((unsigned char *)mdString, (unsigned char *)buf, len + padding_len, &aes_key, (unsigned char*)&aes_128_iv, AES_ENCRYPT);
    OPENSSL_cleanse(&aes_key, sizeof(AES_KEY));

    // Write the encrypted hashed passphrase to PATH_SSL_PASSPHRASE_NAND
    FILE *fp = fopen(PATH_SSL_PASSPHRASE_NAND, "w");
    if (!fp) {
        return FAIL;
    }
    fwrite(buf, RW_SIZE, len + padding_len, fp);
    fclose(fp);

    // Copy the passphrase file to the SD card
    lf_rsa_passphrase_to_sd();

    return SUCC;
}


// base64 from chatGPT(with bugs..)
int lf_base64_encode(const unsigned char *input, int length, char **output) {
	BIO *bmem, *b64;
	BUF_MEM *bptr;
	int base64_length;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);

	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer

	BIO_write(b64, input, length);
	BIO_flush(b64);

	BIO_get_mem_ptr(b64, &bptr);
	*output = (char *)malloc(bptr->length + 1);
	memcpy(*output, bptr->data, bptr->length);
	(*output)[bptr->length] = '\0';

	base64_length = bptr->length;
	BIO_free_all(b64);

	return base64_length;
}

int lf_base64_decode(char *input, unsigned char **output) {
	BIO *b64, *bmem;
	int length = strlen(input);

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);

	*output = (unsigned char *)malloc(length);
	BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	length = BIO_read(bmem, *output, length);

	BIO_free_all(bmem);

	return length;
}

// raw -> rsa encrypt -> base64 encode
int lf_base64_en(RSA* rsa, const unsigned char* input, int length, unsigned char *output) {
	int encBytes = RSA_public_encrypt(length, input, output, rsa, RSA_PKCS1_PADDING);
	
	char *base64_output;
	length = lf_base64_encode(output, encBytes, &base64_output);
	strncpy((char*)output, (const char*)base64_output, length);
	
	free(base64_output);
	return length;
}

// base64 decode -> rsa decrypt -> raw
int lf_base64_de(RSA* rsa, char *input, unsigned char *output) {
	unsigned char *base64_output;

	int len = lf_base64_decode(input, &base64_output);
	int decBytes = RSA_private_decrypt(len, base64_output, output, rsa, RSA_PKCS1_PADDING);
	output[decBytes] = '\0';

	free(base64_output);
	return decBytes;
}

int lf_aes128_derive_key(const char* str, const int str_len, unsigned char *key, unsigned char *iv) {


    // EVP_CIPHER *cipher = EVP_aes_128_cbc();
    EVP_MD *dgst = (EVP_MD *)EVP_sha256();

    unsigned char tmpkeyiv[EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH];
    int iklen = 16; //EVP_CIPHER_get_key_length(cipher);
    int ivlen = 16; //EVP_CIPHER_get_iv_length(cipher);

//    int islen = 0; // note : salt size.
    if (!PKCS5_PBKDF2_HMAC(str, str_len, NULL /*sptr*/, 0 /*islen*/, 10000 /*iter*/, dgst, iklen+ivlen, tmpkeyiv)) {
        char err[256];
        ERR_error_string(ERR_get_error(), err);
        TRACE_INFO("fail to derive the key and iv by PKCS5_PBKDF2_HMAC(). %s\n", err);
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
}