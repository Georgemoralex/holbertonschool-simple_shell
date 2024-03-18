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
 * in interactive mode.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    int pipe_fd[2], num_read;
    pid_t pid;
    char output[MAX_COMMAND_LENGTH];
    char *argv[2];

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        /* Clear the command buffer */
        memset(cmd, 0, MAX_COMMAND_LENGTH);

        /* Read command from stdin using read */
        num_read = read(STDIN_FILENO, cmd, MAX_COMMAND_LENGTH - 1);
        if (num_read <= 0) /* Check for read error or EOF */
        {
            if (isatty(STDIN_FILENO))
            {
                printf("\n");
            }
            break;
        }

        /* Handle newline character at the end */
        if (cmd[num_read - 1] == '\n')
        {
            cmd[num_read - 1] = '\0';
        }
        else
        {
            cmd[num_read] = '\0';
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

        if (pid == 0) /* Child process */
        {
            close(pipe_fd[0]);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);

            argv[0] = cmd;
            argv[1] = NULL;
            execvp(cmd, argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else /* Parent process */
        {
            close(pipe_fd[1]);
            while ((num_read = read(pipe_fd[0], output, sizeof(output) - 1)) > 0)
            {
                output[num_read] = '\0';
                write(STDOUT_FILENO, output, num_read);
            }
            close(pipe_fd[0]);
            wait(NULL);
        }
    }

    return (0);
}


