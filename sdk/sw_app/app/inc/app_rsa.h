#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "app_main.h"
#include "app_comm.h"


RSA* app_rsa_get_rsa();
int app_rsa_passphrase_to_sd(void);
int app_rsa_load_passphrase(char *passphrase, int *passphrase_len);
int app_rsa_save_passphrase(const char *pw);
int app_rsa_generate_privatekey(char *pw);
int app_rsa_encrypt_by_private(RSA *rsa, unsigned char *data, int len, unsigned char *encrypted);
int app_rsa_decrypt_by_private(RSA *rsa, unsigned char *encrypted, int len, unsigned char *decrypted);
int app_rsa_encrypt_fs_by_private(char *src, char *dst);
int app_rsa_decrypt_fs_by_private(char *src, char *dst);
