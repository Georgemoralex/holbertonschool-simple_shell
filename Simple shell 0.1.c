#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h> // Include string.h for strlen function

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

        command[strlen(command) - 1] = '\0'; // This line removes the newline character

        if (strcmp(command, "/bin/ls") == 0) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                execl("/bin/ls", "/bin/ls", NULL);
                perror("execl");
                exit(EXIT_FAILURE);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        } else {
            printf("Command not supported: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}
