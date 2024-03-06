#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(void) {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            printf("\n");
            break;
        }

        command[strlen(command) - 1] = '\0';

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {

            execlp(command, command, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {

            int status;
            waitpid(pid, &status, 0);
        }
    }

    return EXIT_SUCCESS;
}
