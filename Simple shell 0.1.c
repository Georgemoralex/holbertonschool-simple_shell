#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(void) {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        // Display prompt
        printf("$ ");
        fflush(stdout);

        // Read command from user
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            printf("\n");
            break; // End of file (Ctrl+D) or error
        }

        // Remove newline character
        command[strlen(command) - 1] = '\0';

        // Check if the command is "/bin/ls"
        if (strcmp(command, "/bin/ls") == 0) {
            // Fork a child process
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process
                execl("/bin/ls", "/bin/ls", NULL);
                // If execl returns, an error occurred
                perror("execl");
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                // Wait for the child process to finish
                int status;
                waitpid(pid, &status, 0);
            }
        } else {
            printf("Command not supported: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}
