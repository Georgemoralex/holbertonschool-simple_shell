#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024
#define OUTPUT_BUFFER_SIZE 4096 /* Adjust based on expected maximum command output */

int main(void) {
    char cmd[MAX_COMMAND_LENGTH];
    int pipe_fd[2];
    pid_t pid;
    ssize_t read_len;
    char output[OUTPUT_BUFFER_SIZE];
    char *argv[2]; /* Declare argv here, initializing it later */

    while (1) {
        printf("$");
        fflush(stdout);

        if (!fgets(cmd, MAX_COMMAND_LENGTH, stdin)) {
            printf("\n");
            break;
        }

        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0'; /* Remove trailing newline */
        }

        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            continue;
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            /* Child process */
            close(pipe_fd[0]); /* Close unused read end */
            dup2(pipe_fd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
            dup2(pipe_fd[1], STDERR_FILENO); /* Optional: Redirect stderr to pipe */
            close(pipe_fd[1]);

            argv[0] = cmd; /* Assign cmd to argv[0] here */
            argv[1] = NULL;

            execvp(cmd, argv);
            perror("execvp"); /* execvp only returns on error */
            exit(EXIT_FAILURE);
        } else {
            /* Parent process */
            close(pipe_fd[1]); /* Close unused write end */

            /* Read and process the output from the child process */
            while ((read_len = read(pipe_fd[0], output, sizeof(output) - 1)) > 0) {
                output[read_len] = '\0';
                printf("%s", output);
            }
            close(pipe_fd[0]);

            wait(NULL); /* Wait for the child process to finish */
        }
    }

    return (0);
}
