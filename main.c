#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define PATH_MAX 4096 /* Assuming a maximum path length */

/**
 * command_exists - Checks if a command exists in the system's PATH.
 * @cmd: The command to check.
 *
 * Return: 1 if the command exists, 0 otherwise.
 */
int command_exists(const char *cmd)
{
    char *path = getenv("PATH");
    char path_copy[PATH_MAX];
    char *dir;
    char full_path[PATH_MAX];

    strcpy(path_copy, path);

    for (dir = strtok(path_copy, ":"); dir != NULL; dir = strtok(NULL, ":"))
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0)
            return (1); /* Command found */
    }
    return (0); /* Command not found */
}

/**
 * main - Entry point for a simple UNIX command line interpreter.
 * Reads commands using read() and executes them, displaying a prompt
 * in interactive mode. It handles command execution in a child process,
 * and checks the PATH before executing.
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
                printf("\n");
            break; /* EOF or read error */
        }

        cmd[read_bytes - 1] = (cmd[read_bytes - 1] == '\n') ? '\0' : cmd[read_bytes - 1];

        if (!command_exists(cmd))
        {
            fprintf(stderr, "Command not found\n");
            continue; /* Skip fork() if command does not exist */
        }

        pid = fork();
        if (pid == 0) /* Child process */
        {
            char *argv[4];

            argv[0] = "/bin/sh";
            argv[1] = "-c";
            argv[2] = cmd;
            argv[3] = NULL;

            execvp(argv[0], argv);
            /* If execvp returns, it failed */
            fprintf(stderr, "Failed to execute command\n");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) /* Parent process */
        {
            wait(NULL); /* Wait for the child process to finish */
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    return (0);
}
