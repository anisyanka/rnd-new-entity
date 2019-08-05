#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "set1_challenge1.h"
#include "cJSON.h"
#include "main.h"

static const char * default_base64_input_file = "./test/base64-test.json";

// -----------------------------------------------------------------------------
static void PrintOpenJsonFileInfo(FILE * fd, int fsize) {
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
static void CopyOpenJsonFileIntoBuffer(FILE * fd, char * buf) {
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
static char * ReadStringForCodingFromJsonFile(const char * file_name) {
  FILE * fp;
  int fsize = 0;
  char * json_str = NULL;
  
  fp = fopen(file_name, "r");
  if (fp != NULL) {
    fsize = GetFileSize(fp);
    if (fsize < 0) {
      fprintf(stderr, "Cannot determine size of %s: %s\n",
              file_name, strerror(errno));
    } else {
      PrintOpenJsonFileInfo(fp, fsize);
      json_str = (char* )malloc((sizeof(char)) * (fsize + 1));
      if (json_str != NULL) {
        CopyOpenJsonFileIntoBuffer(fp, json_str);
      }
    }
  }
  fclose(fp);
  return json_str;
}

// -----------------------------------------------------------------------------
static int ParseSerializedJson(char * json_str,
                                char ** in_b64, 
                                char ** out_b64) {
  cJSON * json = cJSON_Parse((const char* )json_str);
  if (json == NULL) {
    return 1;
  }
  cJSON * in = cJSON_GetObjectItem(json, "in");
  cJSON * out = cJSON_GetObjectItem(json, "out");

  if (cJSON_IsString(in) && (in->valuestring != NULL)) {
    size_t in_len = strlen(in->valuestring);
    *in_b64 = (char* )malloc(sizeof(char) * in_len);
    memcpy(*in_b64, in->valuestring, in_len);
  } else {
    return 1;
  }

  if (cJSON_IsString(out) && (out->valuestring != NULL)) {
    size_t out_len = strlen(in->valuestring);
    *out_b64 = (char* )malloc(sizeof(char) * out_len);
    memcpy(*out_b64, out->valuestring, out_len);
  } else {
    return 1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
ChallengeStatus_t BaseChallenge1DoWork(void * args) {
  (void)args;

  /* for this variables allocted memoru in the heap, thats need free() */
  char * json_str = NULL;
  char * in_b64 = NULL;
  char * out_b64 = NULL;

  json_str = ReadStringForCodingFromJsonFile(default_base64_input_file);
  if (json_str != NULL) {
    if (ParseSerializedJson(json_str, &in_b64, &out_b64) == 0) {
      printf("In for encode_base64: %s\n", in_b64);
      printf("Out for comparing: %s\n", out_b64);
      printf("Start encode with OpenSSL...\n");
    }
  }

  free(json_str);
  free(in_b64);
  free(out_b64);
  return CHALLENGE_SUCESS;
}