#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "event_handler.h"

// Function to close unused pipes in child process
void close_unused_pipes(int child_index, int parent_to_child[NUM_CHILDREN][2], int child_to_parent[NUM_CHILDREN][2]) {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        close(parent_to_child[i][1]);
        close(child_to_parent[i][0]);
        if (i != child_index) {
            close(parent_to_child[i][0]);
            close(child_to_parent[i][1]);
        }
    }
}

// Function to get event type from the line
int get_event_type(const char *line) {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (strstr(line, event_names[i]) != NULL) {
            return i;
        }
    }
    return -1;  // Unknown event type
}

// Function to process events
void process_events(int event_type, int read_fd, int write_fd) {
    FILE *read_fp = fdopen(read_fd, "r");
    FILE *write_fp = fdopen(write_fd, "w");
    if (!read_fp || !write_fp) {
        perror("fdopen");
        exit(EXIT_FAILURE);
    }

    switch (event_type) {
        case PROCESS_CREATE:
            handle_process_create(read_fp, write_fp);
            break;
        case MEMORY_ALLOC:
            handle_memory_alloc(read_fp, write_fp);
            break;
        case FILE_OPEN:
            handle_file_open(read_fp, write_fp);
            break;
        case USER_LOGIN:
            handle_user_login(read_fp, write_fp);
            break;
        case SYSTEM_BOOT:
            handle_system_boot(read_fp, write_fp);
            break;
    }

    fclose(read_fp);
    fclose(write_fp);
}

// Function to collect results from child processes and print statistics
void collect_results(int child_to_parent[NUM_CHILDREN][2], int event_counts[], int total_events) {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        FILE *read_fp = fdopen(child_to_parent[i][0], "r");
        if (!read_fp) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }

        print_event_statistics(i, read_fp);
        fclose(read_fp);
    }

    int max_event_count = 0, max_event_type = -1;
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (event_counts[i] > max_event_count) {
            max_event_count = event_counts[i];
            max_event_type = i;
        }
    }

    printf("Total events processed: %d\n", total_events);
    printf("Most common event type: %s (%d occurrences)\n", event_names[max_event_type], max_event_count);
}

// Function to print event statistics based on event type
void print_event_statistics(int event_type, FILE *read_fp) {
    if (event_type == PROCESS_CREATE) {
        int process_create_count;
        fscanf(read_fp, "%d", &process_create_count);
        printf("PROCESS_CREATE events: %d\n", process_create_count);
    } else if (event_type == MEMORY_ALLOC) {
        long long total_memory;
        fscanf(read_fp, "%lld", &total_memory);
        printf("MEMORY_ALLOC events: Total memory allocated: %lld bytes\n", total_memory);
    } else if (event_type == FILE_OPEN) {
        char most_accessed_file[256];
        int file_access_count;
        fgets(most_accessed_file, sizeof(most_accessed_file), read_fp);
        most_accessed_file[strcspn(most_accessed_file, "\n")] = '\0';
        fscanf(read_fp, "%d", &file_access_count);
        printf("FILE_OPEN events: Most accessed file: \"%s\" (%d times)\n", most_accessed_file, file_access_count);
    } else if (event_type == USER_LOGIN) {
        int unique_users;
        fscanf(read_fp, "%d", &unique_users);
        printf("USER_LOGIN events: Number of unique users: %d\n", unique_users);
    } else if (event_type == SYSTEM_BOOT) {
        int system_boot_count;
        fscanf(read_fp, "%d", &system_boot_count);
        printf("SYSTEM_BOOT events: Number of system boots: %d\n", system_boot_count);
    }
}
