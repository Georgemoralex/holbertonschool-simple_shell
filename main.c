#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    char *newline;
    pid_t pid;
    int status;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("");
        }

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        newline = strchr(cmd, '\n');
        if (newline)
            *newline = '\0';

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            char *argv[2]; 
            argv[0] = cmd;
            argv[1] = NULL;

            if (execvp(cmd, argv) == -1)
            {
                printf("simple_shell: command not found: %s\n", cmd);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    return (0);
}