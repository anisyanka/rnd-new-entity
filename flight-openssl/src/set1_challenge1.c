#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "set1_challenge1.h"
#include "cJSON.h"
#include "main.h"

static const char * default_base64_input_file = "./test/base64-test.json";

// -----------------------------------------------------------------------------
static void PrintFileInfo(FILE * fd, int fsize) {
#ifdef DEBUG
  int c = 0;
  
  c = fgetc(fd);
  while (c != EOF) {
    printf("%c", c);
    c = fgetc(fd); 
  }

  fseek(fd, 0, SEEK_SET);
  printf("\nFile size = %d byte(s)\n", fsize);
#else
  (void)fd;
  (void)fsize;
#endif
}

// -----------------------------------------------------------------------------
static void CopyFileIntoBuffer(FILE * fd, char * buf) {
  int c = 0;
  int i = 0;

  c = fgetc(fd);
  while (c != EOF) {
    buf[i++] = c;
    c = fgetc(fd);
  }
  buf[i] = '\0';
}

// -----------------------------------------------------------------------------
static int GetFileSize(FILE * fd) {
  int file_len = 0;

  fseek(fd, 0, SEEK_END); // seek to end of file
  file_len = ftell(fd);   // get current file pointer
  fseek(fd, 0, SEEK_SET); // seek back to beginning of file
  return file_len;
}

// -----------------------------------------------------------------------------
static void ReadJsonFileIntoString(const char * file_name, char ** str) {
  FILE * fp;
  int fsize = 0;
  
  fp = fopen(file_name, "r");
  if (fp != NULL) {
    fsize = GetFileSize(fp);
    if (fsize < 0) {
      fprintf(stderr, "Cannot determine size of %s: %s\n",
              file_name, strerror(errno));
    } else {
      PrintFileInfo(fp, fsize);
      *str = (char* )malloc((sizeof(char)) * (fsize + 1));
      if (*str != NULL) {
        CopyFileIntoBuffer(fp, *str);
      }
    }
  }
  fclose(fp);
}

// -----------------------------------------------------------------------------
static int CheckJsonItemAndCopyIt2UserBuf(cJSON * item,
                                          char ** buf) {
  int res = 0;

  if (cJSON_IsString(item) && (item->valuestring != NULL)) {
    size_t len = strlen(item->valuestring);
    *buf = (char* )malloc(sizeof(char) * len);
    if (*buf != NULL) {
      memcpy((void *)*buf, (void *)item->valuestring, len);
    } else {
      ++res;
    }
  } else {
    ++res;
  }
  return res;
}

// -----------------------------------------------------------------------------
static int ParseSerializedJson(char * json_str,
                                char ** input, 
                                char ** output) {
  int res = 0;

  cJSON * json = cJSON_Parse((const char* )json_str);
  if (json != NULL) {
    cJSON * in = cJSON_GetObjectItem(json, "in");
    if (CheckJsonItemAndCopyIt2UserBuf(in, input) != 0) {
      ++res;
    }

    cJSON * out = cJSON_GetObjectItem(json, "out");
    if (CheckJsonItemAndCopyIt2UserBuf(out, output) != 0) {
      ++res;
    }
  } else {
    ++res;
  }
  cJSON_Delete(json);
  return res;
}

// -----------------------------------------------------------------------------
static void OpensslEncode2Base64(char * encode_this,
                                int encode_this_cnt_bytes,
                                char ** out) {
  BIO *bio;
  BIO *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  BIO_push(b64, bio);

  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(b64, encode_this, encode_this_cnt_bytes);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bufferPtr);

  *out = (char *)malloc((sizeof(char)) * (*bufferPtr).length);
  memcpy((void *) *out, (const void *)(*bufferPtr).data, (*bufferPtr).length);
  
  BIO_set_close(b64, BIO_CLOSE);
  BIO_free_all(b64);
  BIO_free_all(bio);
}

// -----------------------------------------------------------------------------
static int ConvertHexAsciiStr2RawBytes(char * ascii, 
                                       size_t len, 
                                       char ** out,
                                       int * hex_len) {
  int index = 0;
  int j = 0;
  char c[3] = { '\0' };

  *out = (char *)malloc(((len / 2) + 1) * sizeof(char));
  if (*out == NULL) {
    return 1;
  }

  for (int i = 0; i < len; ++i) {
     c[index++] = ascii[i];

    if (index == 2) {
      *(*out + j) = strtol(c, NULL, 16);
      index = 0;
      ++j;
    }
  }
  *(*out + j) = '\0';
  *hex_len = strlen(*out);
  return 0;
}

// -----------------------------------------------------------------------------
ChallengeStatus_t BaseChallenge1DoWork(void * args) {
  (void)args;
  ChallengeStatus_t st = CHALLENGE_FAULT;
  int hex_input_len = 0;

  // for this variables a memory is allocted in the heap, 
  // thats why need free()
  char * json_str = NULL;
  char * json_in = NULL;
  char * json_out = NULL;
  char * json_hex_in = NULL;
  char * lib_encodeb64 = NULL;
  char * lib_decodeb64 = NULL;
  char * my_encodeb64 = NULL;
  char * my_decodeb64 = NULL;

  ReadJsonFileIntoString(default_base64_input_file, &json_str);
  if (json_str) {
    if (!ParseSerializedJson(json_str, &json_in, &json_out)) {
      printf("Input for encode to base64: %s\n", json_in);
      printf("Output for comparing:       %s\n", json_out);
      printf("\n---------------------------------------\n");
      printf("*** OpenSSL realization has started *** \n");
      printf("---------------------------------------\n");
      
      if (!ConvertHexAsciiStr2RawBytes(json_in, 
                                       strlen(json_in), 
                                       &json_hex_in,
                                       &hex_input_len)) {
        OpensslEncode2Base64(json_hex_in, 
                             hex_input_len,
                             &lib_encodeb64);
        if (lib_encodeb64) {
          printf("Data for encoding (already in ASCII): %s\n", json_hex_in);
          printf("Encoded data:                         %s\n", lib_encodeb64);
          if (!strcmp(lib_encodeb64, json_out)) {
            st = CHALLENGE_SUCESS;
            printf("Encode2base64 SUCCESS!\n");
          } else {
            printf("Encode2base64 ERROR!\n");
          }
        }
      }
    }
  }

  free(json_str);
  free(json_in);
  free(json_out);
  free(json_hex_in);
  free(lib_encodeb64);
  free(lib_decodeb64);
  free(my_encodeb64);
  free(my_decodeb64);
  return st;
}
