#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "event_handler.h"

// Define the array of event names
const char *event_names[] = {
    "PROCESS_CREATE",
    "MEMORY_ALLOC",
    "FILE_OPEN",
    "USER_LOGIN",
    "SYSTEM_BOOT"
};

int main() {
    int parent_to_child[NUM_CHILDREN][2];  // pipes from parent to child
    int child_to_parent[NUM_CHILDREN][2];  // pipes from child to parent
    pid_t pids[NUM_CHILDREN];              // array to store child PIDs
    int total_events = 0;                  // total number of events processed
    int event_counts[NUM_CHILDREN] = {0};  // counts of each event type

    // Create pipes for communication between parent and children
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (pipe(parent_to_child[i]) == -1 || pipe(child_to_parent[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Fork child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            close_unused_pipes(i, parent_to_child, child_to_parent);
            process_events(i, parent_to_child[i][0], child_to_parent[i][1]);
            exit(0);
        } else {
            pids[i] = pid; // Store child PID in the parent
        }
    }

    // Close unused pipe ends in parent
    for (int i = 0; i < NUM_CHILDREN; i++) {
        close(parent_to_child[i][0]);
        close(child_to_parent[i][1]);
    }

    // Open the event log file for reading
    FILE *fp = fopen("events_log.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read lines from the file and distribute each one to the right child process
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp)) {
        int event_type = get_event_type(line);
        if (event_type >= 0 && event_type < NUM_CHILDREN) {
            event_counts[event_type]++;
            total_events++;
            write(parent_to_child[event_type][1], line, strlen(line));
        } else {
            fprintf(stderr, "Unknown event type: %s", line);
        }
    }
    fclose(fp);

    // Close write ends of pipes to signal end of data to children
    for (int i = 0; i < NUM_CHILDREN; i++) {
        close(parent_to_child[i][1]);
    }

    // Collect results from children
    collect_results(child_to_parent, event_counts, total_events);

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}
