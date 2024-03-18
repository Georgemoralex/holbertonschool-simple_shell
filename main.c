#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void display_prompt() {
    /* printf("simple_shell$ ");*/
    fflush(stdout);
}

int main() {
    char buffer[BUFFER_SIZE];
    pid_t pid;
    size_t length; // Declare length here

    while (1) {
        display_prompt();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            break;
        }

        length = strlen(buffer);
        while (length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == ' ' || buffer[length - 1] == '\t')) {
            buffer[length - 1] = '\0';
            length--;
        }

        pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execlp(buffer, buffer, NULL) == -1) {
                perror("execlp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
