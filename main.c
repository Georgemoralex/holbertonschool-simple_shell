#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

int main(void)
{
  char cmd[MAX_COMMAND_LENGTH];
  pid_t pid;
  ssize_t read_bytes;
  int status;

  while (1)
  {
    if (isatty(STDIN_FILENO))
    {
      printf("$ ");
      fflush(stdout);
    }

    memset(cmd, 0, sizeof(cmd));
    read_bytes = read(STDIN_FILENO, cmd, MAX_COMMAND_LENGTH - 1);
    if (read_bytes <= 0)
    {
      if (isatty(STDIN_FILENO))
      {
        printf("\n");
      }
      break;
    }

    cmd[read_bytes - 1] = '\0';
    pid = fork();
    if (pid == 0)
    {
      char *argv[4] = {"/bin/sh", "-c", NULL, NULL};
      argv[2] = cmd;
      if (execvp(argv[0], argv) == -1)
      {
        fprintf(stderr, "./hsh: 1: %s: not found\n", cmd);
        exit(127);
      }
    }
    else if (pid > 0)
    {
      wait(&status);
    }
    else
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
