#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Entry point for a simple UNIX command line interpreter.
 * This shell executes commands in both interactive and non-interactive mode
 * and displays a prompt in interactive mode only.
 *
 * Return: Always 0 (Success).
 */
int main(void)
{
    char cmd[MAX_COMMAND_LENGTH];
    int pipe_fd[2];
    pid_t pid;
    ssize_t read_len;
    char output[MAX_COMMAND_LENGTH];
    char *argv[2];

    while (1)
    {
        /* Display prompt only in interactive mode */
        if (isatty(STDIN_FILENO))
        {
            printf("$ ");
            fflush(stdout);
        }

        if (!fgets(cmd, MAX_COMMAND_LENGTH, stdin))
        {
            if (isatty(STDIN_FILENO))
            {
                printf("\n"); /* Print newline on EOF in interactive mode */
            }
            break;
        }

        /* Remove trailing newline */
        size_t len = strlen(cmd);
        if (len > 0 && cmd[len - 1] == '\n')
        {
            cmd[len - 1] = '\0';
        }

        /* Setup pipe for capturing command output */
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

        if (pid == 0)
        {
            /* Child process */
            close(pipe_fd[0]); /* Close the read end of the pipe */
            dup2(pipe_fd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
            close(pipe_fd[1]); /* Close the write end, no longer needed */

            /* Prepare and execute command */
            argv[0] = cmd;
            argv[1] = NULL;
            execvp(cmd, argv);
            perror("execvp"); /* execvp only returns on error */
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
            close(pipe_fd[1]); /* Close the write end of the pipe */

            /* Read and directly output command's result to stdout */
            while ((read_len = read(pipe_fd[0], output, sizeof(output) - 1)) > 0)
            {
                output[read_len] = '\0';
                write(STDOUT_FILENO, output, read_len);
            }
            close(pipe_fd[0]); /* Close the read end of the pipe */

            wait(NULL); /* Wait for the child process to finish */
        }
    }

    return (0);
}
