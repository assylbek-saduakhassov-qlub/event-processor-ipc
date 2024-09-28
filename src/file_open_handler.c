#include <stdio.h>
#include <string.h>
#include "event_handler.h"

void handle_file_open(FILE *read_fp, FILE *write_fp) {
    struct file_access {
        char filename[256];
        int count;
    } files[MAX_FILES];

    int num_files = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        char *path_str = strstr(buffer, "PATH:");
        if (path_str) {
            path_str += 5;  // Move pointer past "PATH:"
            char *end = strchr(path_str, ' ');
            if (!end) end = path_str + strlen(path_str);
            size_t len = end - path_str;
            char filename[256];
            strncpy(filename, path_str, len);
            filename[len] = '\0';

            if (filename[0] == '"' && filename[len - 1] == '"') {
                memmove(filename, filename + 1, len - 2);
                filename[len - 2] = '\0';
            }

            int found = 0;
            for (int i = 0; i < num_files; i++) {
                if (strcmp(files[i].filename, filename) == 0) {
                    files[i].count++;
                    found = 1;
                    break;
                }
            }

            if (!found && num_files < MAX_FILES) {
                strcpy(files[num_files].filename, filename);
                files[num_files].count = 1;
                num_files++;
            }
        }
    }

    int max_count = 0;
    char most_accessed_file[256] = "";
    for (int i = 0; i < num_files; i++) {
        if (files[i].count > max_count) {
            max_count = files[i].count;
            strcpy(most_accessed_file, files[i].filename);
        }
    }

    fprintf(write_fp, "%s\n%d\n", most_accessed_file, max_count);
}
