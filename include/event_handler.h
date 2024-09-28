#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdio.h>  // For FILE

#define NUM_CHILDREN 5        // Number of child processes
#define MAX_LINE_LENGTH 512   // Maximum length of a line in the event file
#define MAX_FILES 100         // Maximum number of files to track
#define MAX_USERS 100         // Maximum number of users to track

// Array of event names corresponding to each child process
extern const char *event_names[];

// Enumeration for event types to use as indices
enum {
    PROCESS_CREATE,
    MEMORY_ALLOC,
    FILE_OPEN,
    USER_LOGIN,
    SYSTEM_BOOT
};

// Function prototypes for each event handler
void handle_process_create(FILE *read_fp, FILE *write_fp);
void handle_memory_alloc(FILE *read_fp, FILE *write_fp);
void handle_file_open(FILE *read_fp, FILE *write_fp);
void handle_user_login(FILE *read_fp, FILE *write_fp);
void handle_system_boot(FILE *read_fp, FILE *write_fp);

// Function prototypes for utility functions
void close_unused_pipes(int child_index, int parent_to_child[NUM_CHILDREN][2], int child_to_parent[NUM_CHILDREN][2]);
int get_event_type(const char *line);
void process_events(int event_type, int read_fd, int write_fd);
void collect_results(int child_to_parent[NUM_CHILDREN][2], int event_counts[], int total_events);
void print_event_statistics(int event_type, FILE *read_fp);

#endif // EVENT_HANDLER_H
