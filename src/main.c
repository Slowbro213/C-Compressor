#include "../lib/hashmap.h"
#include "../lib/filebatch.h"
#include "../lib/BitWriter.h"
#include "../lib/Packer.h"
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <errno.h>   
#include <string.h> 



int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <outputfile>\n", argv[0]);
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

  bfm_huffman_tree(&freq_map);


  // Open the output file for writing
  // Note: "wb" for binary mode
  FILE *output_file = fopen(argv[2], "wb");

  if (output_file == NULL) {
    fprintf(stderr, "Error opening %s: %s\n", argv[2], strerror(errno));
    return 1;
  }

  // Pack the file using the frequency map
  if (pack_file(file, output_file, &freq_map) != 0) {
    fprintf(stderr, "Error packing file\n");
    fclose(output_file);
    return 1;
  }

  // Close the files
  fclose(file);
  fclose(output_file);


  return 0;
}
