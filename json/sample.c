#include <cjson/cJSON.h>
#include <stdio.h>

// Prevent link failure with cjson
void __stack_chk_fail() {}

int getConfigurationDefaults(char* in_llq, char* out_extension)
{
    FILE *file = fopen("../configuration/default.json", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char *json_string = malloc(file_size + 1);
    fread(json_string, 1, file_size, file);
    json_string[file_size] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(json_string);
        return 1;
    }

    cJSON *global = cJSON_GetObjectItem(root, "global");
    if (global == NULL) {
        fprintf(stderr, "Global settings not found\n");
        cJSON_Delete(root);
        free(json_string);
        return 1;
    }
    cJSON *global_allocation_defaults = cJSON_GetObjectItem(global, "allocation_defaults");
    if (global_allocation_defaults == NULL) {
        fprintf(stderr, "Global allocation defaults not found\n");
        cJSON_Delete(root);
        free(json_string);
        return 1;
    }
    char *codepage = cJSON_GetObjectItem(global_allocation_defaults, "codepage")->valuestring;
    const char *type = cJSON_GetObjectItem(global_allocation_defaults, "type")->valuestring;

    cJSON *dataset_settings = cJSON_GetObjectItem(root, "dataset_settings");
    if (dataset_settings == NULL) {
        printf("Dataset settings not found, using global settings\n");
    } else {
        cJSON *COBOL = cJSON_GetObjectItem(dataset_settings, in_llq);
        if (COBOL == NULL) {
            printf("COBOL settings not found, using global settings\n");
        } else {
            cJSON *COBOL_allocation_defaults = cJSON_GetObjectItem(COBOL, "extension");
            if (COBOL_allocation_defaults == NULL) {
                printf("COBOL allocation defaults not found, using global settings\n");
            } else {
                printf("Value: %s\n", COBOL_allocation_defaults->valuestring);
            }
        }
    }

    // Free the cJSON object and json string
    cJSON_Delete(root);
    free(json_string);

    return 0;
}

int main() {
  getConfigurationDefaults("DATA", NULL);
  getConfigurationDefaults("COBOL", NULL);
}
