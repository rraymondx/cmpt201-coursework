#define _POSIX_C_SOURCE 2008090L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  char *input = NULL;
  size_t n = 0;

  while (true) {

    printf("Enter programs to run.\n");
    ssize_t len = getline(&input, &n, stdin);

    if (len == -1) {
      perror("getlen failed");
      break;
    }

    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0'; // replace newline with null terminator
    }

    pid_t cpid = fork(); // creates new process, child process
    if (cpid < 0) {      // if child process is less than 0, fork failed
      perror("fork failed\n");
      continue;

    } else if (cpid > 0) { // parent process waits for child process

      int status = 0;
      if (waitpid(cpid, &status, 0) == -1) {
        perror("waitpid failed\n");
        exit(EXIT_FAILURE);
      }

      if (WIFEXITED(status)) {
        printf("Child exited.\n");
      }
    } else { // child will run command
      if (execl(input, input, (char *)NULL) == -1) {
        perror("child process failed");
        free(input);
        exit(EXIT_FAILURE);
      }
    }
  }

  free(input);
  return 0;
}
