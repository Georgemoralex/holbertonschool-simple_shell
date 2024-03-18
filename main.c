#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    int pipe_fd[2];
    pid_t pid;
    ssize_t read_len;
    char output[MAX_COMMAND_LENGTH];
    char *argv[2];
    size_t len; /* Declare len here for C90 compliance */

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        if (!fgets(cmd, MAX_COMMAND_LENGTH, stdin))
        {
            if (isatty(STDIN_FILENO))
            {
                printf("\n");
            }
            break;
        }

        len = strlen(cmd); /* Moved len initialization here from its declaration */
        if (len > 0 && cmd[len - 1] == '\n')
        {
            cmd[len - 1] = '\0';
        }

        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], STDOUT_FILENO);
            dup2(pipe_fd[1], STDERR_FILENO); /* Optional: Redirect stderr to pipe */
            close(pipe_fd[1]);

            argv[0] = cmd;
            argv[1] = NULL;
            execvp(cmd, argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            close(pipe_fd[1]);
            while ((read_len = read(pipe_fd[0], output, sizeof(output) - 1)) > 0)
            {
                output[read_len] = '\0';
                write(STDOUT_FILENO, output, read_len);
            }
            close(pipe_fd[0]);
            wait(NULL);
        }
    }

    return (0);
}

