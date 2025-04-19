#include "../lib/filebatch.h"


void process_file_batches(FILE* file, void (*process_chunk)(uint8_t*, size_t, void*), void* args) 
{
  uint8_t buffer[BUFFER_SIZE];
  size_t bytes_read;

#if PRINT_TIME
  clock_t start_time = clock();
#endif

  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    process_chunk(buffer, bytes_read, args);

    if (ferror(file)) {
      perror("File read error");
      break;
    }
  }

#if PRINT_TIME

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Time taken: %f seconds\n", elapsed_time);
#endif
}
