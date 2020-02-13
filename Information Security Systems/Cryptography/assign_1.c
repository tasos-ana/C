/*
 * HY-457 / Exercise 1
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>
#include <assert.h>

#include "assign_1.h"

#define BLOCK_SIZE 16

/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len) {
    size_t i;

    if (!data)
        printf("NULL data\n");
    else {
        for (i = 0; i < len; i++) {
            if (!(i % 16) && (i != 0))
                printf("\n");
            printf("%02X ", data[i]);
        }
        printf("\n");
    }
}

/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len) {
    size_t i;

    if (!data)
        printf("NULL data\n");
    else {
        for (i = 0; i < len; i++)
            printf("%c", data[i]);
        printf("\n");
    }
}

/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void) {
    printf(
            "\n"
            "Usage:\n"
            "    assign_1 -i in_file -o out_file -p passwd -b bits"
            " [-d | -e | -s | -v]\n"
            "    assign_1 -h\n"
            );
    printf(
            "\n"
            "Options:\n"
            " -i    path    Path to input file\n"
            " -o    path    Path to output file\n"
            " -p    psswd   Password for key generation\n"
            " -b    bits    Bit mode (128 or 256 only)\n"
            " -d            Decrypt input and store results to output\n"
            " -e            Encrypt input and store results to output\n"
            " -s            Encrypt+sign input and store results to output\n"
            " -v            Decrypt+verify input and store results to output\n"
            " -h            This help message\n"
            );
    exit(EXIT_FAILURE);
}

/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password,
        int bit_mode, int op_mode) {
    if (!input_file) {
        printf("Error: No input file!\n");
        usage();
    }

    if (!output_file) {
        printf("Error: No output file!\n");
        usage();
    }

    if (!password) {
        printf("Error: No user key!\n");
        usage();
    }

    if ((bit_mode != 128) && (bit_mode != 256)) {
        printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
        usage();
    }

    if (op_mode == -1) {
        printf("Error: No mode\n");
        usage();
    }
}

/*
 * Generates a key using a password
 */
int
keygen(unsigned char *password, unsigned char *key, unsigned char *iv,
        int bit_mode) {

    /* Task A */

    const EVP_CIPHER *cipher;
    const EVP_MD *md;
    int succeed = 0;

    /* Choose  mode depends on bit_mode */
    if (bit_mode == 128) cipher = EVP_aes_128_ecb();
    else cipher = EVP_aes_256_ecb();

    if (!cipher) {
        printf("Error: something goes wrong with cipher\n");
        return 0;
    }
    /* Choosing the sha1 hash function */
    md = EVP_sha1();
    if (!md) {
        printf("Error: something goes wrong with md\n");
        return 0;
    }

    /* Convert the password with the previous data on key */
    succeed = EVP_BytesToKey(cipher, md, iv,
            password, strlen((char*) password), 1, key, iv);
    /* Error checking */
    if (!succeed) {
        printf("Error: Failed to convert password on key \n");
        return 0;
    }
    return 1;
}

/*
 * Encrypts the data
 */
int
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
        unsigned char *iv, unsigned char *ciphertext, int bit_mode) {

    /* Task B */

    EVP_CIPHER_CTX ctx;
    const EVP_CIPHER *cipher;
    int len;
    int padd_len;

    /* Create a new encryption context*/
    EVP_CIPHER_CTX_init(&ctx);

    /* Choose  mode depends on bit_mode */
    if (bit_mode == 128) cipher = EVP_aes_128_ecb();
    else cipher = EVP_aes_256_ecb();

    /* 
     * Initiliaze it with the appropriate mode and key 
     * Error checking too
     */
    if (!EVP_EncryptInit_ex(&ctx, cipher, NULL, key, iv)) {
        printf("Error: something goes wrong with encrypt init\n");
        return -1;
    }

    /*
     * Update the ciphertext
     * Error checking too
     */
    if (!EVP_EncryptUpdate(&ctx, ciphertext, &len, plaintext, plaintext_len)) {
        printf("Error: something goes wrong with encrypt update\n");
        return -1;
    }

    /* 
     * Finalize the encryption
     * Error checking
     */
    if (!EVP_EncryptFinal_ex(&ctx, ciphertext + len, &padd_len)) {
        printf("Error: something goes wrong with encrypt final\n");
        return -1;
    }
    /* Add the padding on length */
    len += padd_len;

    /*
     * Free the context
     */
    EVP_CIPHER_CTX_cleanup(&ctx);

    return len;
}

/*
 * Decrypts the data and returns the plaintext size
 */
int
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
        unsigned char *iv, unsigned char *plaintext, int bit_mode) {

    /* Task C */

    int plaintext_len, len;
    EVP_CIPHER_CTX *ctx;
    const EVP_CIPHER *cipher;

    plaintext_len = 0;
    len = 0;

    /* 
     * Create a new decryption context
     */
    ctx = EVP_CIPHER_CTX_new();

    /* Choose  mode depends on bit_mode */
    if (bit_mode == 128) cipher = EVP_aes_128_ecb();
    else cipher = EVP_aes_256_ecb();

    /* 
     * Initialize it with the appropriate mode and key 
     * Error checking
     */
    if (!EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) {
        printf("Error: something goes wrong with decrypt init\n");
        return -1;
    }

    /*
     * Update the plaintext
     * Error checking
     */
    if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
        printf("Error: something goes wrong with decrypt update\n");
        return -1;
    }

    /* Update the plaintext_len with decrypted len */
    plaintext_len = len;

    /*
     * Finalize the decryption
     * Error checking
     */
    if (!EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        printf("Error: something goes wrong with decrypt final\n");
        return -1;
    }

    /* Add the padding on current plaintext_len */
    plaintext_len += len;

    /* Free the context */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

/*
 * Generates a CMAC
 */
int
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key,
        unsigned char *cmac, int bit_mode) {

    /* Task D */

    size_t cmac_len;
    CMAC_CTX *ctx;
    const EVP_CIPHER *cipher;

    cmac_len = 0;

    /*
     * Create a new CMAC context
     * Error checking
     */
    if (!(ctx = CMAC_CTX_new())) {
        printf("Error: something goes wrong on create cmac context\n");
        return -1;
    }

    /* Choose  mode depends on bit_mode */
    if (bit_mode == 128) cipher = EVP_aes_128_ecb();
    else cipher = EVP_aes_256_ecb();

    /*
     * Initialize it with the appropriate mode and key
     * Error checking
     */
    if (!CMAC_Init(ctx, key, bit_mode / 8, cipher, NULL)) {
        printf("Error: something goes wrong with cmac init\n");
        return -1;
    }

    /*
     * Update the CMAC
     * Error checking
     */
    if (!CMAC_Update(ctx, data, data_len)) {
        printf("Error: something goes wrong with cmac update\n");
        return -1;
    }

    /*
     * Finalize the CMAC
     * Error checking
     */
    if (!CMAC_Final(ctx, cmac, &cmac_len)) {
        printf("Error: something goes wrong with cmac final\n");
        return -1;
    }

    /*
     * Free the context
     */
    CMAC_CTX_free(ctx);

    return 1;
}

/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2) {
    /* Task E */
    int verify;

    verify = 0;

    /*
     * Compare the 2 cmac's
     */
    verify = !CRYPTO_memcmp(cmac1, cmac2, BLOCK_SIZE);

    return verify;
}

/* TODO Develop your functions here... */

/*
 * Reading the data from input_file and store them on plaintext
 */
int readText(unsigned char **text, char *input_file) {
    int len = -1, text_len = 0;
    unsigned char * buff;
    FILE *fp;
    buff = (unsigned char*) malloc(1024);

    if (buff == NULL) {
        free(buff);
        printf("\nERROR: Memory allocation did not complete successfully!");
    }

    /* open the file for read byte */
    if (!(fp = fopen(input_file, "rb"))) {
        printf("Unable to open file!");
        return -1;
    }

    /* Read 1024 bytes each time */
    while (1) {
        len = fread(buff, 1, 1024, fp);

        text_len += len;
        if (len == 1024) {
            buff = (unsigned char*) realloc(buff, 1024);
        } else {
            break;
        }
    }
    /* 
     * Malloc the exactly bytes for plaintext 
     * Move the data from buffer on plaintext
     */
    *text = (unsigned char*) malloc(text_len);
    /* check malloc*/
    if (*text == NULL) {
        free(*text);
        printf("\nERROR: Memory allocation did not complete successfully!");
    }
    memmove(*text, buff, text_len);

    free(buff);
    buff = NULL;
    fclose(fp);

    return text_len;
}

/*
 * Write the data from ciphertext on the output_file
 */
int writeText(unsigned char *text, int text_len, char* output_file) {
    FILE *fp;
    if (!(fp = fopen(output_file, "wb"))) {
        printf("Unable to open file for write!");
        return 0;
    }
    fwrite(text, 1, text_len, fp);

    fclose(fp);
    return 1;
}

int appendText(unsigned char *text, int text_len, char* output_file) {
    FILE *fp;
    if (!(fp = fopen(output_file, "ab"))) {
        printf("Unable to open file for append!");
        return 0;
    }
    fwrite(text, 1, text_len, fp);

    fclose(fp);
    return 1;
}

void cleanUp(char* in, char* out, unsigned char* pw,
        unsigned char* plain, unsigned char*cipher) {
    /* Clean up */
    free(in);
    free(out);
    free(pw);
    free(plain);
    free(cipher);
}

/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv) {
    int opt; /* used for command line arguments */
    int bit_mode; /* defines the key-size 128 or 256 */
    int op_mode; /* operation mode */
    char *input_file; /* path to the input file */
    char *output_file; /* path to the output file */
    unsigned char *password; /* the user defined password */
    unsigned char *plaintext; /* the user given input text */
    int plaintext_len;
    unsigned char *ciphertext;
    int ciphertext_len;
    unsigned char* cmac;
    unsigned char cmac1[BLOCK_SIZE];
    unsigned char cmac2[BLOCK_SIZE];
    unsigned char key[EVP_MAX_KEY_LENGTH];

    /* Init arguments */
    input_file = NULL;
    output_file = NULL;
    password = NULL;
    plaintext = NULL;
    plaintext_len = -1;
    ciphertext = NULL;
    ciphertext_len = -1;
    cmac = NULL;
    bit_mode = -1;
    op_mode = -1;
    memset(cmac1, 0, BLOCK_SIZE);
    memset(cmac2, 0, BLOCK_SIZE);
    memset(key, 0, EVP_MAX_KEY_LENGTH);

    /*
     * Get arguments
     */
    while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
        switch (opt) {
            case 'b':
                bit_mode = atoi(optarg);
                break;
            case 'i':
                input_file = strdup(optarg);
                break;
            case 'o':
                output_file = strdup(optarg);
                break;
            case 'p':
                password = (unsigned char *) strdup(optarg);
                break;
            case 'd':
                /* if op_mode == 1 the tool decrypts */
                op_mode = 1;
                break;
            case 'e':
                /* if op_mode == 0 the tool encrypts */
                op_mode = 0;
                break;
            case 's':
                /* if op_mode == 2 the tool signs */
                op_mode = 2;
                break;
            case 'v':
                /* if op_mode == 3 the tool verifies */
                op_mode = 3;
                break;
            case 'h':
            default:
                usage();
        }
    }

    /* check arguments */
    check_args(input_file, output_file, password, bit_mode, op_mode);

    /* Initialize the library */

    /* Keygen from password */
    keygen(password, key, NULL, bit_mode);

    /* Operate on the data according to the mode */
    switch (op_mode) {
        case 0:
            /* STEP 1: Read data from input_file */
            if (!(plaintext_len = readText(&plaintext, input_file))) {
                printf("Unable to read plainText\n");
                break;
            }

            ciphertext = (unsigned char*) malloc(plaintext_len + plaintext_len / 2);
            if (ciphertext == NULL) {
                printf("\nERROR: Memory allocation did not complete successfully!");
                break;
            }

            /* STEP 2: Encrypt plaintext */
            ciphertext_len = encrypt(plaintext, plaintext_len, key, NULL, ciphertext, bit_mode);
            if (ciphertext_len < 0) break;

            /* STEP 3: Write data on output_file */
            if (!writeText(ciphertext, ciphertext_len, output_file)) {
                printf("Unable to write cipherText\n");
                break;
            }
            break;
        case 1:
            /* STEP 1: Read data from input_file */
            if (!(ciphertext_len = readText(&ciphertext, input_file))) {
                printf("Unable to read cipherText\n");
                break;
            }

            plaintext = (unsigned char*) malloc(ciphertext_len + ciphertext_len / 2);
            if (plaintext == NULL) {
                printf("\nERROR: Memory allocation did not complete successfully!");
                break;
            }

            /* STEP 2: Decrypt */
            plaintext_len = decrypt(ciphertext, ciphertext_len, key, NULL, plaintext, bit_mode);
            if (plaintext_len < 0) break;

            /* STEP 3: Write data on output_file */
            if (!writeText(plaintext, plaintext_len, output_file)) {
                printf("Unable to write plaintextText\n");
                break;
            }
            break;
        case 2:
            /* STEP 1: Read data from input_file */
            if (!(plaintext_len = readText(&plaintext, input_file))) {
                printf("Unable to read plainText\n");
                break;
            }

            /*
             * STEP 2: Ecrypt the input data
             */
            ciphertext = (unsigned char*) malloc(plaintext_len + plaintext_len / 2);
            if (ciphertext == NULL) {
                printf("\nERROR: Memory allocation did not complete successfully!");
                break;
            }
            ciphertext_len = encrypt(plaintext, plaintext_len, key, NULL, ciphertext, bit_mode);
            if (ciphertext_len < 0) break;

            /*
             * Initialize the cmac buffer,length
             */
            cmac = (unsigned char*) malloc(BLOCK_SIZE);
            if (cmac == NULL) {
                printf("\nERROR: Memory allocation did not complete successfully!");
                break;
            }

            /* STEP 3: Create the sign */
            if (gen_cmac(plaintext, plaintext_len, key, cmac, bit_mode) < 0) {
                break;
            }

            /* STEP 4: Write data on output_file */
            if (!writeText(ciphertext, ciphertext_len, output_file)) {
                printf("Unable to write cipherText\n");
                break;
            }

            /* STEP 5: Append cmac on output_file */
            if (!appendText(cmac, BLOCK_SIZE, output_file)) {
                printf("Unable to append cmac \n");
                break;
            }
            break;
        case 3:
            /* STEP 1: Read ciphertext from input */
            if (!(ciphertext_len = readText(&ciphertext, input_file))) {
                printf("Unable to read cipherText\n");
                break;
            }

            /* STEP 2: Keep on cmac1 the original mac */
            memmove(cmac1, ciphertext + ciphertext_len - BLOCK_SIZE, BLOCK_SIZE);

            /* STEP 3: Decrypt the cipher text */
            plaintext = (unsigned char*) malloc(ciphertext_len + ciphertext_len / 2);
            if (plaintext == NULL) {
                printf("\nERROR: Memory allocation did not complete successfully!");
                break;
            }
            plaintext_len = decrypt(ciphertext, ciphertext_len - BLOCK_SIZE, key, NULL, plaintext, bit_mode);
            if (plaintext_len < 0) break;

            /* STEP 5: Create the sign */
            if (gen_cmac(plaintext, plaintext_len, key, cmac2, bit_mode) < 0) {
                break;
            }

            /* STEP 7: verify the 2 mac's*/
            if (verify_cmac(cmac1, cmac2)) {
                printf("CMAC's matched\n");
                /* STEP 8 Write the plaintext on output*/
                if (!writeText(plaintext, plaintext_len, output_file)) {
                    printf("Unable to write plaintextText\n");
                    break;
                }
            } else {
                /* STEP 8 Print error msg*/
                printf("Who are you? CMAC's it's wrongs\n");
            }

            break;
        default:
            assert(0);
    }

    free(input_file);
    free(output_file);
    free(password);
    free(plaintext);
    free(ciphertext);
    free(cmac);
    /* END */
    return 0;
}
/*
 * HY-457 / Exercise 1
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */