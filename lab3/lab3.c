#define _POSIX_C_SOURCE 2008090L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

void print_history(char *history[], int count) {

  int start = 0;
  int items_to_print = count;

  if (count > HISTORY_SIZE) {
    start = count % HISTORY_SIZE;
    items_to_print = HISTORY_SIZE;
  }

  for (int i = 0; i < items_to_print; i++) {
    int index = (start + i) % HISTORY_SIZE;
    printf("%s", history[index]);
  }
}

void free_history(char *history[], int count) {
  int max = (count < HISTORY_SIZE) ? count : HISTORY_SIZE;

  for (int i = 0; i < max; i++) {
    free(history[i]);
    history[i] = NULL;
  }
}

int main() {

  char *history[HISTORY_SIZE] = {NULL};
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int count = 0;
  while (1) {

    printf("Enter input: ");
    read = getline(&line, &len, stdin);

    int index = count % HISTORY_SIZE;
    if (history[index] != NULL) {
      free(history[index]);
    }

    history[index] = line;
    count++;

    if (strcmp(line, "print\n") == 0) {
      print_history(history, count);
    }

    // reset point
    line = NULL;
    len = 0;
  }

  free_history(history, count);
  return 0;
}
