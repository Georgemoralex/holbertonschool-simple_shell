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

        if (cmd[read_bytes - 1] == '\n')
        {
            cmd[read_bytes - 1] = '\0';
        }
        else
        {
            cmd[read_bytes] = '\0';
        }

        pid = fork();
        if (pid == 0)
        {
            char *env_path = getenv("PATH");
            if (env_path != NULL && strlen(env_path) == 0)
            {
                char *common_dirs[] = {"/bin/", "/usr/bin/", NULL};
                int i = 0;
                while (common_dirs[i] != NULL)
                {
                    char full_path[MAX_COMMAND_LENGTH];
                    snprintf(full_path, sizeof(full_path), "%s%s", common_dirs[i], cmd);
                    if (access(full_path, X_OK) == 0)
                    {
                        execl(full_path, cmd, (char *)NULL);
                        fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
                        exit(127);
                    }
                    i++;
                }
                fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
                exit(127);
            }
            else
            {
                char *exec_argv[4];
                exec_argv[0] = "/bin/sh";
                exec_argv[1] = "-c";
                exec_argv[2] = cmd;
                exec_argv[3] = NULL;

                execvp(exec_argv[0], exec_argv);
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0)
                {
                    continue;
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