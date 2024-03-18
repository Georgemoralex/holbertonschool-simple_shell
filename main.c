#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Entry point for a simple UNIX command line interpreter.
 * Reads commands using read() and executes them, displaying a prompt
 * in interactive mode. It handles command execution in a child process.
 *
 * Return: Always 0 (Success).
 */
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
            break; /* EOF or read error */
        }

        if (cmd[read_bytes - 1] == '\n') {
            cmd[read_bytes - 1] = '\0'; /* Replace newline with null terminator */
        }

        pid = fork();
        if (pid == 0) { /* Child process */
            char *argv[] = {"/bin/sh", "-c", NULL, NULL};
            argv[2] = cmd; /* Assign cmd to the array after its declaration */
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
