#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  char *line = NULL;

  size_t n = 0;

  printf("Please enter some text: ");
  ssize_t length = getline(&line, &n, stdin);

  if (length < 0) {
    perror("getline failed");
    free(line);
    exit(EXIT_FAILURE);
  }

  char *saveptr = NULL;

  char *str = strtok_r(line, " ", &saveptr);
  printf("\nTokens: \n");
  printf("%s\n", str);

  while ((str = strtok_r(NULL, " ", &saveptr))) {
    printf("%s\n", str);
  }

  free(line);

  return 0;
}
