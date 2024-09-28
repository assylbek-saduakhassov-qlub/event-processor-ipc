#include <stdio.h>
#include "event_handler.h"

void handle_process_create(FILE *read_fp, FILE *write_fp) {
    int count = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        count++;
    }

    fprintf(write_fp, "%d\n", count);  // Send count of process create events to the parent
}
