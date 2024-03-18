#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void display_prompt() {
    fflush(stdout);
}

int main() {
    char buffer[BUFFER_SIZE];
    pid_t pid;
    int pipefd[2];
    char output_buffer[BUFFER_SIZE];

    ssize_t bytes_read;

    while (1) {
        display_prompt();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }

        pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);

            execlp("sh", "sh", "-c", buffer, NULL);
            perror("execlp failed");
            exit(EXIT_FAILURE);
        } else {
            close(pipefd[1]);
            waitpid(pid, NULL, 0);

            bytes_read = read(pipefd[0], output_buffer, sizeof(output_buffer));
            if (bytes_read == -1) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
            output_buffer[bytes_read] = '\0';

            if (bytes_read > 0 && output_buffer[bytes_read - 1] == '\n') {
                output_buffer[bytes_read - 1] = '\0';
            }

            printf("%s\n", output_buffer);
        }
    }

    return (0);
}