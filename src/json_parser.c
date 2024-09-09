//Note: this file is compiled in ASCII to interoperate with cJSON
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "s99.h"
#include "dio.h"
#include "dioint.h"
#include "wrappers.h"
#include <_Ccsid.h>
#include <_Nascii.h>
#include <unistd.h>
#include <fnmatch.h>
#include <cjson/cJSON.h>

// FIXME: CJSON missing symbol?
void __stack_chk_fail() {}

extern const char *build_default_json;

void set_default_extension(char *in_llq, char *out_extension) {
  strcpy(out_extension, in_llq);
  strlower(out_extension);
  __a2e_s(out_extension);
}

// Currently returns the extension, but could return default allocation
// parameters in the future
int initialize_configuration(char *in_dsname, char *in_llq, char *out_extension,
                             int *out_txtflag, int *out_ccsid) {

  char *dsname = strdup(in_dsname);
  __e2a_s(dsname);
  char *llq = strdup(in_llq);
  __e2a_s(llq);

  int prevMode = __ae_thread_swapmode(__AE_ASCII_MODE);

  // Try user configuration first
  if (get_user_configuration(dsname, llq, out_extension, out_txtflag,
                             out_ccsid) == 0) {
    free(dsname);
    free(llq);
    __ae_thread_setmode(prevMode);
    return 0;
  }

  // Fall back to default configuration
  if (get_configuration_defaults(dsname, llq, out_extension, out_txtflag,
                                 out_ccsid) == 0) {
    free(dsname);
    free(llq);
    __ae_thread_setmode(prevMode);
    return 0;
  }

  // If no configuration is found, default extension and type
  set_default_extension(llq, out_extension);
  *out_txtflag = 1;
  *out_ccsid = 0;
  free(dsname);

  __ae_thread_setmode(prevMode);

  return 0;
}

int parse_configuration(cJSON *root, char *in_dsname, char *in_llq,
                        char *out_extension, int *out_txtflag, int *out_ccsid) {
  if (root == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    return 1;
  }

  cJSON *dataset_settings = cJSON_GetObjectItem(root, "dataset_settings");
  if (dataset_settings == NULL) {
    return 1;
  }

  cJSON *dsname_object = NULL;
  cJSON *dsname_extension = NULL;

  // Initialize defaults
  *out_txtflag = 1;
  *out_ccsid = 0;

  cJSON *item = NULL;
  cJSON_ArrayForEach(item, dataset_settings) {
    const char *key = item->string;

    // Allow for wildcards
    if (fnmatch(key, in_dsname, 0) == 0) {
      dsname_object = item;
      break;
    }
  }

  if (dsname_object != NULL) {
    dsname_extension = cJSON_GetObjectItem(dsname_object, "extension");
    cJSON *dsname_type = cJSON_GetObjectItem(dsname_object, "type");
    cJSON *dsname_codepage = cJSON_GetObjectItem(dsname_object, "codepage");
#ifdef DEBUG
    fprintf(stderr, "Found codepage and type: %s %s\n", dsname_codepage->valuestring, dsname_type->valuestring);
#endif

    if (dsname_extension != NULL)
      set_default_extension(dsname_extension->valuestring, out_extension);
    else
      set_default_extension(in_llq, out_extension);

    if (dsname_type != NULL) {
      if (strcmp(dsname_type->valuestring, "binary") == 0) {
        *out_txtflag = 0;
      } else {
        *out_txtflag = 1;
      }
    }

    if (dsname_codepage != NULL) {
      // Convert codepage to CCSID
      *out_ccsid = __toCcsid(dsname_codepage->valuestring);
    }

    return 0;
  }

  return 1;
}

int get_user_configuration(char *in_dsname, char *in_llq, char *out_extension,
                           int *out_txtflag, int *out_ccsid) {
  FILE *file = NULL;
  char *config_path = getenv("DIO_CONFIG");

  if (config_path != NULL) {
    file = fopen(config_path, "r");
  }

  // If DIO_CONFIG is not set or the file couldn't be opened, try
  // $HOME/.dioconfig.json
  if (file == NULL) {
    char *home = getenv("HOME");
    if (home != NULL) {
      char home_config_path[_POSIX_PATH_MAX];
      sprintf(home_config_path, "%s/.dioconfig.json", home);
      file = fopen(home_config_path, "r");
#ifdef DEBUG
      fprintf(stderr, "Found %s\n", home_config_path);
#endif
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
    fclose(file);
    return 1;
  }

  fread(json_string, 1, file_size, file);
  json_string[file_size] = '\0';
  fclose(file);

  cJSON *root = cJSON_Parse(json_string);
  int rc = parse_configuration(root, in_dsname, in_llq, out_extension,
                               out_txtflag, out_ccsid);
  cJSON_Delete(root);

  free(json_string);
  return rc;
}

int get_configuration_defaults(char *in_dsname, char *in_llq,
                               char *out_extension, int *out_txtflag,
                               int *out_ccsid) {
  cJSON *root = cJSON_Parse(build_default_json);
  int rc = parse_configuration(root, in_dsname, in_llq, out_extension,
                               out_txtflag, out_ccsid);
  cJSON_Delete(root);

  return rc;
}
