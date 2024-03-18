int main() {
    char buffer[BUFFER_SIZE];
    pid_t pid;

    while (1) {
        display_prompt();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execlp(buffer, buffer, NULL) == -1) {
                printf("Command not found: %s\n", buffer);
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
                printf("Command executed successfully\n");
            } else {
                printf("Command failed\n");
            }
        }
    }

    return 0;
}

