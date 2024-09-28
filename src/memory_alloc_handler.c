#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event_handler.h"

void handle_memory_alloc(FILE *read_fp, FILE *write_fp) {
    long long total_memory = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        char *size_str = strstr(buffer, "SIZE:");
        if (size_str) {
            size_str += 5;  // Move pointer past "SIZE:"
            total_memory += atoll(size_str);
        }
    }

    fprintf(write_fp, "%lld\n", total_memory);  // Send total memory allocated to the parent
}
