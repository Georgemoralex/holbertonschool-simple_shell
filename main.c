#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int command_exists(char *cmd);

/**
 * main - Entry point for a simple UNIX command line interpreter.
 *        Reads commands using read() and attempts to execute them,
 *        displaying a prompt in interactive mode.
 *        It prints a specific error message if a command is not found.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    pid_t pid;
    ssize_t read_bytes;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        memset(cmd, 0, sizeof(cmd));
        read_bytes = read(STDIN_FILENO, cmd, MAX_COMMAND_LENGTH - 1);
        if (read_bytes <= 0)
        {
            if (isatty(STDIN_FILENO))
            {
                printf("\n");
            }
            break; /* EOF or read error */
        }

        if (cmd[read_bytes - 1] == '\n')
        {
            cmd[read_bytes - 1] = '\0'; /* Replace newline with null terminator */
        }

        pid = fork();
        if (pid == 0) /* Child process */
        {
            if (!command_exists(cmd))
            {
                fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
                exit(127);
            }
            char *argv[] = {cmd, NULL};
            execvp(cmd, argv);
            exit(EXIT_FAILURE); /* Exec should not return */
        }
        else if (pid > 0) /* Parent process */
        {
            int status;
            wait(&status); /* Wait for the child process to finish */
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

/**
 * command_exists - Checks if a command exists in the PATH.
 * @cmd: Command to check.
 *
 * Return: 1 if command exists, 0 otherwise.
 */
int command_exists(char *cmd)
{
    return access(cmd, X_OK) == 0;
}
