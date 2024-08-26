//Note: this file is compiled in ASCII to interoperate with cJSON
#define _AE_BIMODAL 1
#define _ENHANCED_ASCII_EXT 0xFFFFFFFF
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "s99.h"
#include "dio.h"
#include "dioint.h"
#include "wrappers.h"
#include <_Nascii.h>
#include <unistd.h>
#include <cjson/cJSON.h>

//FIXME: CJSON missing symbol?
void __stack_chk_fail() {}

extern const char *build_default_json;


// Currently returns the extension, but could return default allocation parameters in the future
int initialize_configuration(char* in_llq, char* out_extension) {

  char* llq = strdup(in_llq);
  __e2a_s(llq);

  int prevMode = __ae_thread_swapmode(__AE_ASCII_MODE);
  if (get_user_configuration(llq, out_extension) == 0) {
    strlower(out_extension);
    free(llq);
    __ae_thread_setmode(prevMode);
    return 0;
  }

  if (get_configuration_defaults(llq, out_extension) == 0) {
    strlower(out_extension);
    free(llq);
    __ae_thread_setmode(prevMode);
    return 0;
  }

  // If all else fails, lower case llq
  __ae_thread_setmode(prevMode);
  strcpy(out_extension, in_llq);
  strlower(out_extension);
  free(llq);
  return 0;
}

int parse_configuration(cJSON *root, char* in_llq, char* out_extension)
{
  if (root == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    //TODO: Where should I print the errors?
    //if (error_ptr != NULL) {
      //fprintf(stderr, "JSON parsing error: %s\n", error_ptr);
    //}
    return 1;
  }

  //TODO: for allocation defaults
  /* cJSON *global = cJSON_GetObjectItem(root, "global"); */
  /* if (global != NULL) { */
  /*   cJSON *global_allocation_defaults = cJSON_GetObjectItem(global, "allocation_defaults"); */
  /*   if (global_allocation_defaults == NULL) { */
  /*       fprintf(stderr, "Global allocation defaults not found\n"); */
  /*       cJSON_Delete(root); */
  /*       //free(json_string); */
  /*       return 1; */
  /*   } */
  /*   char *codepage = cJSON_GetObjectItem(global_allocation_defaults, "codepage")->valuestring; */
  /*   const char *type = cJSON_GetObjectItem(global_allocation_defaults, "type")->valuestring; */
  /* } */

  cJSON *dataset_settings = cJSON_GetObjectItem(root, "dataset_settings");
  if (dataset_settings == NULL) {
    return 1;
  } else {
    cJSON *llq_object = cJSON_GetObjectItem(dataset_settings, in_llq);
    if (llq_object != NULL) {
      cJSON *llq_extension = cJSON_GetObjectItem(llq_object, "extension");
      if (llq_extension != NULL) {
        __a2e_s(llq_extension->valuestring);
        strcpy(out_extension, llq_extension->valuestring);
      }
      else {
        return 1;
      }
    }
    else {
      return 1;
    }
  }

  return 0;
}



int get_user_configuration(char* in_llq, char* out_extension)
{
  FILE *file = NULL;
  char *config_path = getenv("DIO_CONFIG");

  if (config_path != NULL) {
    file = fopen(config_path, "r");
  }

  // If DIO_CONFIG is not set or the file couldn't be opened, try $HOME/.dioconfig.json
  if (file == NULL) {
    char *home = getenv("HOME");
    if (home != NULL) {
      char home_config_path[_POSIX_PATH_MAX];
      sprintf(home_config_path, "%s/.dioconfig.json", home);
      file = fopen(home_config_path, "r");
    }
  }

  if (file == NULL) {
    return 1;
  }

  // Read the json as a string
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);
  char *json_string = malloc(file_size + 1);
  if (json_string == NULL) {
    //fprintf(stderr, "Memory allocation failed\n");
    fclose(file);
    return 1;
  }

  fread(json_string, 1, file_size, file);
  json_string[file_size] = '\0';
  fclose(file);

  cJSON *root = cJSON_Parse(json_string);
  int rc = parse_configuration(root, in_llq, out_extension);
  cJSON_Delete(root);

  free(json_string);
  return rc;
}


int get_configuration_defaults(char* in_llq, char* out_extension)
{
  cJSON *root = cJSON_Parse(build_default_json);
  int rc = parse_configuration(root, in_llq, out_extension);
  cJSON_Delete(root);

  return rc;
}
