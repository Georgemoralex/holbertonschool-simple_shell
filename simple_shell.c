#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 100

int main(void) {
    char input[MAX_INPUT_LENGTH];

    while (1) {

        printf("simple_shell> ");
        fflush(stdout);


        if (fgets(input, sizeof(input), stdin) == NULL) {

            printf("\nGoodbye!\n");
            break;
        }

        input[strlen(input) - 1] = '\0';

      
        pid_t child_pid = fork();
        if (child_pid == 0) {

            execlp(input, input, NULL); 
            perror("Error");
            exit(EXIT_FAILURE);
        } else if (child_pid > 0) {

            wait(NULL);
        } else {
            perror("Fork failed");
        }
    }

    return 0;
}
