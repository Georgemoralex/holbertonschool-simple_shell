#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 1024

int main(void) {
	char input[MAX_INPUT_LENGTH];
	char *token;

	while (1) {
		
		printf("$ ");
		fflush(stdout);
		
		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("\nGoodbye!\n");
			break;
		}

		input[strcspn(input, "\n")] = '\0';

		token = strtok(input, " ");
		while (token != NULL) {
			token = strtok(NULL, " ");
		}
	}

	return 0;
}
