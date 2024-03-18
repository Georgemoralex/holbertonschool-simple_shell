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
 * in interactive mode. Designed to handle multi-line input, executing
 * each line as a separate command.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    pid_t pid;
    int num_read;
    char *argv[3]; /* Adjusted size for NULL termination */
    int i;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        memset(cmd, 0, MAX_COMMAND_LENGTH);
        num_read = read(STDIN_FILENO, cmd, MAX_COMMAND_LENGTH - 1);
        if (num_read <= 0)
        {
            if (isatty(STDIN_FILENO))
            {
                printf("\n");
            }
            break;
        }

        /* Iterate through read characters to replace '\n' with '\0' */
        for (i = 0; i < num_read; i++)
        {
            if (cmd[i] == '\n')
            {
                cmd[i] = '\0';
                /* Fork and execute command */
                pid = fork();
                if (pid == 0) /* Child process */
                {
                    argv[0] = "/bin/sh";
                    argv[1] = "-c";
                    argv[2] = cmd; /* Execute command */
                    if (execvp(argv[0], argv) == -1)
                    {
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                }
                else if (pid > 0) /* Parent process */
                {
                    wait(NULL); /* Wait for child process */
                }
                else
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return (0);
}
