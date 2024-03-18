#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * command_exists - Checks if a command exists in the system's PATH.
 * @cmd: Command to check.
 *
 * Return: 1 if command exists, 0 otherwise.
 */
int command_exists(char *cmd) {
    if (access(cmd, X_OK) != -1) {
        return 1; /* Command exists */
    } else {
        return 0; /* Command does not exist */
    }
}

int main(void) {
    char cmd[MAX_COMMAND_LENGTH];
    pid_t pid;
    ssize_t read_bytes;
    char *argv[3]; /* Correction: Should be 4 elements for NULL termination. */

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
            break; /* EOF or read error */
        }

        if (cmd[read_bytes - 1] == '\n') {
            cmd[read_bytes - 1] = '\0'; /* Replace newline with null terminator */
        }

        pid = fork();
        if (pid == 0) { /* Child process */
            if (!command_exists(cmd)) {
                fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
                exit(127);
            }
            argv[0] = "/bin/sh";
            argv[1] = "-c";
            argv[2] = cmd;
            argv[3] = NULL; /* Ensure the array is properly NULL-terminated */
            execvp(argv[0], argv);
            /* If execvp returns, it means it failed */
            fprintf(stderr, "Failed to execute command\n");
            exit(EXIT_FAILURE);
        } else if (pid > 0) { /* Parent process */
            wait(NULL); /* Wait for the child process to finish */
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
