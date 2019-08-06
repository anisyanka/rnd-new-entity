#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

#include "main.h"
#include "args_input.h"
#include "set1_challenge1.h"

// -----------------------------------------------------------------------------
static int LibcryptoInit(void) {
  int res = 0;

  /* Load the human readable error strings for libcrypto */
  if (ERR_load_crypto_strings() == 0) {
    ERR_print_errors_fp(stderr);
    ++res;
  }

  /* Load all digest and cipher algorithms */
  if (OpenSSL_add_all_algorithms() == 0) {
    ERR_print_errors_fp(stderr);
    ++res;
  }
  return res;
}

// -----------------------------------------------------------------------------
static void LibcryptoCleanupAll(void) {
  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO 
  (low level API) for e.g. base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  ERR_free_strings();
}

// -----------------------------------------------------------------------------
int main (int argc, char * argv[]) {
  ArgsSummary_t summary_args;

  memset(&summary_args, 0, sizeof(summary_args));
  if (ArgsCheckInput(argc, argv, &summary_args) == ARGS_WRONG) {
    exit(EXIT_FAILURE);
  }

  if (!LibcryptoInit()) {
    printf("Hello, from OpenSSL flight!\n");
  } else {
    printf("Init OpenSSL error!\n");
    LibcryptoCleanupAll();
    exit(EXIT_FAILURE);
  }
  
  /* Start do challenges here */
  BaseChallenge1DoWork(NULL);

  /* free allocaed memory by OpenSSL */
  LibcryptoCleanupAll();
  return 0;
}
