#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define PATH_MAX 4096

int command_exists(const char *cmd) {
    char *paths[] = {
        "/bin/",
        "/usr/bin/",
        "/sbin/",
        "/usr/sbin/",
        "/usr/local/bin/",
        NULL
    };
    char full_path[PATH_MAX];
    int i;

    for (i = 0; paths[i] != NULL; i++) {
        snprintf(full_path, sizeof(full_path), "%s%s", paths[i], cmd);
        if (access(full_path, X_OK) == 0) {
            return 1;
        }
    }

    return 0;
}

int main(void) {
    char cmd[MAX_COMMAND_LENGTH];
    pid_t pid;
    ssize_t read_bytes;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            printf("$ ");
            fflush(stdout);
        }

        memset(cmd, 0, sizeof(cmd));
        read_bytes = read(STDIN_FILENO, cmd, MAX_COMMAND_LENGTH - 1);
        if (read_bytes <= 0) {
            if (isatty(STDIN_FILENO)) {
                printf("\n");
            }
            break;
        }

        cmd[read_bytes - 1] = (cmd[read_bytes - 1] == '\n') ? '\0' : cmd[read_bytes - 1];

        char *space_pos = strchr(cmd, ' ');
        char command_name[MAX_COMMAND_LENGTH];
        if (space_pos != NULL) {
            strncpy(command_name, cmd, space_pos - cmd);
            command_name[space_pos - cmd] = '\0';
        } else {
            strcpy(command_name, cmd);
        }

        if (!command_exists(command_name)) {
            fprintf(stderr, "Command not found\n");
            continue;
        }

        pid = fork();
        if (pid == 0) {
            char *argv[4];

            argv[0] = "/bin/sh";
            argv[1] = "-c";
            argv[2] = cmd;
            argv[3] = NULL;

            execvp(argv[0], argv);
            fprintf(stderr, "Failed to execute command\n");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) {
            wait(NULL);
        }
        else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
