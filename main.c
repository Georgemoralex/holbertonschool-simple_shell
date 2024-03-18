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
            char *exec_argv[4];
            exec_argv[0] = "/bin/sh";
            exec_argv[1] = "-c";
            exec_argv[2] = cmd;
            exec_argv[3] = NULL;

            execvp(exec_argv[0], exec_argv);
            fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
            exit(127);
        }
        else if (pid > 0)
        {
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status == 127)
                {
                    fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
                }
            }
        }
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
