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
            break;
        }

        cmd[read_bytes - 1] = '\0';

        pid = fork();
        if (pid == 0)
        {
            char *argv[4];
            argv[0] = "/bin/sh";
            argv[1] = "-c";
            argv[2] = cmd;
            argv[3] = NULL;

            execvp(argv[0], argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}