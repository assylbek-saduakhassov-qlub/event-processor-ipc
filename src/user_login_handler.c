#include <stdio.h>
#include <string.h>
#include "event_handler.h"

void handle_user_login(FILE *read_fp, FILE *write_fp) {
    char users[MAX_USERS][256];
    int num_users = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), read_fp) != NULL) {
        char *user_str = strstr(buffer, "USER:");
        if (user_str) {
            user_str += 5;  // Move pointer past "USER:"
            char *end = strchr(user_str, ' ');
            if (!end) end = user_str + strlen(user_str);
            size_t len = end - user_str;
            char username[256];
            strncpy(username, user_str, len);
            username[len] = '\0';

            if (username[0] == '"' && username[len - 1] == '"') {
                memmove(username, username + 1, len - 2);
                username[len - 2] = '\0';
            }

            int found = 0;
            for (int i = 0; i < num_users; i++) {
                if (strcmp(users[i], username) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found && num_users < MAX_USERS) {
                strcpy(users[num_users], username);
                num_users++;
            }
        }
    }

    fprintf(write_fp, "%d\n", num_users);  // Send the number of unique users to the parent
}
