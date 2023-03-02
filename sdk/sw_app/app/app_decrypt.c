/******************************************************************
  This is the source code for decryption using the latest AES algorithm.
  AES algorithm is also called Rijndael algorithm. AES algorithm is
  recommended for non-classified use by the National Institute of Standards
  and Technology(NIST), USA. Now-a-days AES is being used for almost
  all encryption applications all around the world.
  THE MAIN FEATURE OF THIS AES ENCRYPTION PROGRAM IS NOT EFFICIENCY; IT
  IS SIMPLICITY AND READABILITY. THIS SOURCE CODE IS PROVIDED FOR ALL
  TO UNDERSTAND THE AES ALGORITHM.
  Comments are provided as needed to understand the program. But the
  user must read some AES documentation to understand the underlying
  theory correctly.
  It is not possible to describe the complete AES algorithm in detail
  here. For the complete description of the algorithm, point your
  browser to:
  http://www.csrc.nist.gov/publications/fips/fips197/fips-197.pdf
  Find the Wikipedia page of AES at:
  http://en.wikipedia.org/wiki/Advanced_Encryption_Standard
  ******************************************************************
*/
// Include stdio.h for standard input/output.
//  Used for giving output to the screen.
//

#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include "app_main.h"
#include "app_set.h"
#include "app_comm.h"

// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
// The number of rounds in AES Cipher. It is simply initiated to zero. The actual value is recieved in the program.
static int Nr = 0;
// The number of 32 bit words in the key. It is simply initiated to zero. The actual value is recieved in the program.
static int Nk = 0;

// in - it is the array that holds the plain text to be encrypted.
// out - it is the array that holds the output CipherText after encryption.
// state - the array that holds the intermediate results during encryption.
static unsigned char in[32], out[32], state[4][4];

// The array that stores the round keys.
static unsigned char RoundKey[240];

// The Key input to the AES Program
static unsigned char Key[16];

// The round constant word array, Rcon[i], contains the values given by 
// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
static const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

static const uint8_t rsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

#define getSBoxInvert(num) (rsbox[(num)])

static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
 
#define getDSBoxValue(num) (sbox[(num)])

// The round constant word array, DRcon[i], contains the values given by
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(28)
// Note that i starts at 1, not 0).
int DRcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb };

// This function produces DNb(DNr+1) round keys. The round keys are used in each round to encrypt the states.
static void DKeyExpansion()
{
    unsigned i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk; ++i)
    {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (i = Nk; i < Nb * (Nr + 1); ++i)
    {
        {
            k = (i - 1) * 4;
            tempa[0]=RoundKey[k + 0];
            tempa[1]=RoundKey[k + 1];
            tempa[2]=RoundKey[k + 2];
            tempa[3]=RoundKey[k + 3];
        }

        if (i % Nk == 0)
        {
        // This function shifts the 4 bytes in a word to the left once.
        // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

        // Function RotWord()
        {
            const uint8_t u8tmp = tempa[0];
            tempa[0] = tempa[1];
            tempa[1] = tempa[2];
            tempa[2] = tempa[3];
            tempa[3] = u8tmp;
        }

        // SubWord() is a function that takes a four-byte input word and 
        // applies the S-box to each of the four bytes to produce an output word.

        // Function Subword()
        {
            tempa[0] = getDSBoxValue(tempa[0]);
            tempa[1] = getDSBoxValue(tempa[1]);
            tempa[2] = getDSBoxValue(tempa[2]);
            tempa[3] = getDSBoxValue(tempa[3]);
        }

        tempa[0] = tempa[0] ^ Rcon[i/Nk];
        }
        
        j = i * 4; k=(i - Nk) * 4;
        RoundKey[j + 0] = RoundKey[k + 0] ^ tempa[0];
        RoundKey[j + 1] = RoundKey[k + 1] ^ tempa[1];
        RoundKey[j + 2] = RoundKey[k + 2] ^ tempa[2];
        RoundKey[j + 3] = RoundKey[k + 3] ^ tempa[3];
    }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void DAddRoundKey(int round)
{
   uint8_t i,j;
    
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            state[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
            //printf("%2x ", RoundKey[(round * Nb * 4) + (i * Nb) + j]);
        }
    }
    //printf("\n");
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes()
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            state[j][i] = getSBoxInvert(state[j][i]);
        }
    }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void InvShiftRows()
{
    uint8_t temp;

    // Rotate first row 1 columns to right  
    temp = state[3][1];
    state[3][1] = state[2][1];
    state[2][1] = state[1][1];
    state[1][1] = state[0][1];
    state[0][1] = temp;

    // Rotate second row 2 columns to right 
    temp = state[0][2];
    state[0][2] = state[2][2];
    state[2][2] = temp;

    temp = state[1][2];
    state[1][2] = state[3][2];
    state[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = state[0][3];
    state[0][3] = state[1][3];
    state[1][3] = state[2][3];
    state[2][3] = state[3][3];
    state[3][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
    return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

// MixColumns function mixes the columns of the state matrix
// The method used may look complicated, but it is easy if you know the underlying theory.
// Refer the documents specified above.
void InvMixColumns()
{
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i)
    { 
        a = state[i][0];
        b = state[i][1];
        c = state[i][2];
        d = state[i][3];

        state[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}

// InvCipher is the main function that decrypts the CipherText
static void InvCipher()
{
    int i, j, round = 0;

    //Copy the input PlainText to state array.
    for (i = 0; i<4; i++)
    {
        for (j = 0; j<4; j++)
        {
            state[i][j] = in[i * 4 + j];
        }
    }
    
    // Add the First round key to the state before starting the rounds.
    DAddRoundKey(Nr);

    // There will be  rounds.
    // The first DNr-1 rounds are identical.
    // These DNr-1 rounds are executed in the loop below.
     
    for (round = Nr - 1; round > 0; round--)
    {
        InvShiftRows();
        InvSubBytes();
        DAddRoundKey(round);
        InvMixColumns();
    }

    // The last is given below.
    // The MixColumns function is not here in the last round.
    InvShiftRows();
    InvSubBytes();
    DAddRoundKey(0);

    // The encryption process is over.
    // Copy the state array to output array.
    for (i = 0; i<4; i++)
    {
        for (j = 0; j<4; j++)
        {
            out[i * 4 + j] = state[i][j];
        }
    }
}

int decrypt_aes(const char *src, char *dst, int length)
{
    int i;
    // Receive the length of key here.
/*
    while (DNr != 128 && DNr != 192 && DNr != 256)
    {
        printf("Enter the length of Key(128, 192 or 256 only): ");
        scanf("%d", &DNr);
    }
*/
    Nr = 128 ;

    Nk = Nr / 32;
    Nr = Nk + 6;

    // The array temp stores the key.
    // The array temp2 stores the plaintext.

    unsigned char temp[16] = { 0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};

    for (i = 0; i<Nk * 4; i++)
    {
        Key[i] = temp[i];
    }
   
    memset(in, 0x00, 32) ;
    memset(out, 0x00, 32) ;

    //strncpy(in, src, length) ; 
    memcpy(in, src, length); 

    //printf("Dec:cipertext");
    //util_hexdump(in, length); 
    DKeyExpansion();

    InvCipher();
    
	//printf("Dec:invcipertext");
	//util_hexdump(out, length); 
    //strncpy(dst, out, length) ;
    memcpy(dst, out, length);

    return 0;
}

int openssl_aes128_decrypt(char* src, char*dst, int type)
{
    AES_KEY dec_key_128;   
    int total_len = 0, dst_len = 0, i = 0;

    unsigned char aes_128_key[16] = {0, }; 
    unsigned char iv[16] = {0, }; 
    if(!type)
    {
        sprintf(aes_128_key, "%s", app_set->sys_info.aes_key) ;
    }
    else
    {
//        unsigned char aes_128_key[] = {0x61,0x61,0x61,0x61,0x61,0x61,0x61,0x61,0x62,0x62,0x62,0x62,0x62,0x62,0x62,0x62}; 
        for(i = 0; i < 16; i++)
        {
            if(i < 8)
                aes_128_key[i] = 0x61 ;
            else
                aes_128_key[i] = 0x62 ;
        }
    }

    if(AES_set_decrypt_key(aes_128_key, sizeof(aes_128_key)*8, &dec_key_128) < 0){
        // print Error  
    }; 

    printf("aes_128_key = %s\n",aes_128_key) ;
    printf("aes iv = %s\n",iv) ;
    printf("aes128 decrypt src = %s\n",src) ;
    printf("aes128_decrypt length of src = %d\n",strlen(src)); 
    printf("aes128_decrypt length of src = %d\n",(strlen(src) + 16)/16*16); 
#if 1
    AES_cbc_encrypt(src, dst, (strlen(src) + 16)/16*16, &dec_key_128, aes_128_key, AES_DECRYPT);
#else
    AES_cbc_encrypt(src, dst, (strlen(src) + 16)/16*16, &dec_key_128, iv, AES_DECRYPT);
#endif

    total_len = strlen(dst) ;
	dst_len = strlen(src) - dst[strlen(src)-1];
    printf("total_len = %d dst_len = %d  dec padding size %d\n",total_len, dst_len);

//    printf("aes128_decrypt dst value = %s dst_len = %d\n",dst, strlen(dst)) ;
    return dst_len ;
}

int openssl_aes128_decrypt_fs(char *src,char *dst)
{
    unsigned char aes_128_key[16] = {0,};
    unsigned char aes_128_iv[16] = {0,};
    char passphrase[64] = {'\0', };
    int passphrase_len = 0;
    if (lf_rsa_load_passphrase(passphrase, &passphrase_len) != SUCC) {
        return FAIL;
    }

    if (lf_aes128_derive_key(passphrase, passphrase_len, aes_128_key, aes_128_iv) == FAIL) {
        return FAIL;
    }

    char buf[FREAD_COUNT+BLOCK_SIZE];
	int len=0;
	int total_size=0;
	int save_len=0;
    int w_len=0, i = 0;

    FILE *fp=fopen(src,"rb");
	if( fp == NULL ){
	    fprintf(stderr,"[ERROR] %d can not fopen('%s')\n",__LINE__,src);
	    return FAIL;
    }

	FILE *wfp=fopen(dst,"wb");
    if( wfp == NULL ){
        fprintf(stderr,"[ERROR] %d can not fopen('%s')\n",__LINE__,dst);
        return FAIL;
    }

	AES_set_decrypt_key(aes_128_key ,KEY_BIT ,&aes_key_128);

    fseek(fp ,0 ,SEEK_END);
	total_size=ftell(fp);
	fseek(fp ,0 ,SEEK_SET);
    printf("total_size %d\n",total_size);

    while( len = fread( buf ,RW_SIZE ,FREAD_COUNT ,fp) ){
        if( FREAD_COUNT == 0 ){
            break;
        }
		save_len+=len;
        w_len=len;

        AES_cbc_encrypt(buf ,buf ,len ,&aes_key_128 , &aes_128_iv ,AES_DECRYPT);
		if( save_len == total_size ){ // check last block
	        w_len=len - buf[len-1];
	        printf("dec padding size %d\n" ,buf[len-1]);
        }
        fwrite(buf ,RW_SIZE ,w_len ,wfp);
    }

    fclose(wfp);
    fclose(fp);

    return SUCC;
}









































        




