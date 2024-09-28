#include <stdio.h>
#include "event_handler.h"

void handle_system_boot(FILE *read_fp, FILE *write_fp) {
    int count = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        count++;
    }

    fprintf(write_fp, "%d\n", count);  // Send the count of system boot events to the parent
}
