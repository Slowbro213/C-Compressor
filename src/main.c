#include "../lib/hashmap.h"
#include "../lib/filebatch.h"
#include "../lib/BitWriter.h"
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <errno.h>   
#include <string.h> 


// Processor function
void count_frequencies(uint8_t* chunk, size_t size, void* args) {
  ByteFreqMap* freq_map = (ByteFreqMap*)args;
  for (size_t i = 0; i < size; i++) {
    bfm_increment(freq_map, chunk[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  ByteFreqMap freq_map;
  bfm_init(&freq_map);

  FILE *file = fopen(argv[1], "rb");  // Note: "rb" for binary mode
  if (file == NULL) {
    fprintf(stderr, "Error opening %s: %s\n", argv[1], strerror(errno));
    return 1;
  }


  process_file_batches(file, count_frequencies, &freq_map);

  if (ferror(file)) {
    fprintf(stderr, "Error reading %s\n", argv[1]);
    fclose(file);
    return 1;
  }

  // Sort the frequency map by frequency
  bfm_sort_by_freq(&freq_map);


  fclose(file);


  bfm_huffman_tree(&freq_map);

  //Test for bit Writer
  FILE *output_file = fopen("output.bin", "wb");


  typedef struct {
    uint8_t code;
    uint8_t length;
  } HuffCode;

  HuffCode huff_codes[2];
  huff_codes[0].code = 0b110;
  huff_codes[0].length = 3;
  huff_codes[1].code = 0b1011;
  huff_codes[1].length = 4;


  BitWriter bw;
  bw_init(&bw, output_file);

  // Write the Huffman codes to the output file
  for (int i = 0; i < 2; i++) {
    bw_write_bit(&bw, huff_codes[i].code, huff_codes[i].length);
  }


  // Flush any remaining bits in the buffer
  bw_flush(&bw);
  fclose(output_file);    


  return 0;
}
