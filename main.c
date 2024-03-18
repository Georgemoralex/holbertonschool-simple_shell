#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Entry point for a simple UNIX command line interpreter.
 *
 * Return: Always 0.
 */
int main(void)
{
	char cmd[MAX_COMMAND_LENGTH];
	char *newline;
	pid_t pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("$");
			fflush(stdout); /* Ensure the prompt is displayed immediately. */
		}

		if (!fgets(cmd, sizeof(cmd), stdin))
		{
			printf("\n"); /* Print newline on EOF. */
			break;
		}

		newline = strchr(cmd, '\n');
		if (newline)
			*newline = '\0'; /* Remove newline from input. */

		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{
			/* Child process executes the command. */
			char *argv[2] = {cmd, NULL};
			execvp(cmd, argv);
			/* If execvp returns, an error occurred. */
			printf("simple_shell: command not found: %s\n", cmd);
			exit(EXIT_FAILURE);
		}
		else
		{
			/* Parent process waits for the child to complete. */
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
			{
				/* Optionally handle the child's exit status. */
			}
		}
	}

	return (0);
}
