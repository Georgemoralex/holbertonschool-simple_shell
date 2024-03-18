#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Entry point for a simple UNIX command line interpreter.
 * Reads commands using read() and executes them three times, displaying a prompt
 * in interactive mode.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    int num_read;
    pid_t pid;
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

        for (int i = 0; i < 3; i++) // Loop to execute the command three times
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) /* Child process */
            {
                argv[0] = cmd;
                argv[1] = NULL;
                execvp(cmd, argv);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else /* Parent process */
            {
                wait(NULL); /* Wait for the child process to finish */
            }
        }
    }

    return (0);
}

