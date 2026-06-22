#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUF_SIZE 256

// help mac recognize sbrk
void *sbrk(intptr_t increment);

// Helper method to handle system call errors
void handle_error(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

// define header struct
struct header {
  uint64_t size;
  struct header *next;
};

// helper function to help print byte
void print_byte(uint8_t b) {
  char buf[16];
  ssize_t len = snprintf(buf, sizeof(buf), "%u\n", b);
  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  // increase heap size by 256
  void *heap_start = sbrk(BUF_SIZE);
  if (heap_start == (void *)-1) {
    handle_error("sbrk failed");
  }

  // starting address of block 1 and 2
  void *block1_addr = heap_start;
  void *block2_addr = (char *)heap_start + 128;

  // create pointer of type header
  struct header *h1 = (struct header *)block1_addr;
  struct header *h2 = (struct header *)block2_addr;

  // initialize headers
  h1->size = 128;
  h1->next = NULL;

  h2->size = 128;
  h2->next = h1;

  // calculate data
  uint8_t *data1 = (uint8_t *)(h1 + 1);
  uint8_t *data2 = (uint8_t *)(h2 + 1);

  size_t payload_size = 128 - sizeof(struct header);

  // initialize data inside block
  memset(data1, 0, payload_size);
  memset(data2, 1, payload_size);

  print_out("first block:   %p\n", &block1_addr, sizeof(block1_addr));
  print_out("second block:  %p\n", &block2_addr, sizeof(block2_addr));

  print_out("first block size: %lu\n", &(h1->size), sizeof(h1->size));
  // cast void pointer
  void *h1_next = (void *)h1->next;
  print_out("first block next: %p\n", &h1_next, sizeof(h1_next));

  print_out("second block size: %lu\n", &(h2->size), sizeof(h2->size));
  void *h2_next = (void *)h2->next;
  print_out("second block next: %p\n", &h2_next, sizeof(h2_next));

  for (size_t i = 0; i < payload_size; i++) {
    print_byte(data1[i]);
  }

  for (size_t i = 0; i < payload_size; i++) {
    print_byte(data2[i]);
  }

  return 0;
}
