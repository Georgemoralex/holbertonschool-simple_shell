#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_COMMAND_LENGTH 100

void prompt() {
	printf("$ ");
}

#include <stdlib.h>

void execute_command(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char **argv = malloc(sizeof(char*) * 2);
        if (argv == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        argv[0] = command;
        argv[1] = NULL;

        extern char **environ;
        execve(command, argv, environ);
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
    printf("\n");
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        prompt();
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\n");
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0) {
            continue;
        }

        execute_command(command);
    }

    return 0;
}
