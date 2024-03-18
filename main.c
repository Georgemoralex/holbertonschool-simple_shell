#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void display_prompt() {
    printf("simple_shell$ ");
    fflush(stdout);
}

int main() {
    char buffer[BUFFER_SIZE];
    int should_run = 1;

    while (should_run) {
        display_prompt();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execlp(buffer, buffer, NULL) == -1) {
                perror("execlp failed");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Command executed successfully\n");
            } else {
                printf("Command failed\n");
            }
        }
    }

    return 0;
}
