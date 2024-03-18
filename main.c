#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define DELIM " "

/**
 * main - Entry point for a simple UNIX command line interpreter.
 * Reads commands using read() and executes each space-separated command,
 * displaying a prompt in interactive mode.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    int num_read;
    pid_t pid;
    char *argv[2];
    char *token;

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
        if (cmd[num_read - 1] == '\n')
        {
            cmd[num_read - 1] = '\0';
        }
        else
        {
            cmd[num_read] = '\0';
        }

        token = strtok(cmd, DELIM);
        while (token != NULL)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
            {
                argv[0] = token;
                argv[1] = NULL;
                execvp(token, argv);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
            token = strtok(NULL, DELIM);
        }
    }

    return (0);
}
