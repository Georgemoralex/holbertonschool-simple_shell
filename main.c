#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            should_run = 0;
            continue;
        }

        printf("Executing command: %s\n", buffer);
    }

    return 0;
}
