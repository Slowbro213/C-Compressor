#include "../lib/BitWriter.h"
#include "../lib/hashmap.h"
#include "../lib/filebatch.h"


#include <stdio.h>

#define DEBUG 0

int pack_file(FILE *input_file, FILE *output_file, const ByteFreqMap *map) {
    if (input_file == NULL || output_file == NULL || map == NULL) {
        return -1;  // Invalid arguments
    }

    BitWriter writer;
    bw_init(&writer, output_file);

        // Reset the input file position to the beginning
    fseek(input_file, 0, SEEK_SET);


    // Write the frequency map to the output file
    fwrite(map, sizeof(ByteFreqMap), 1, output_file);
    
    // Read the input file and write the compressed data  
    int byte;

    while((byte = fgetc(input_file)) != EOF) {
        // Get the Huffman code for the byte
        uint32_t code = bfm_get_code(map, byte);
        uint8_t length = map->freqs[byte].length;

        // Write the code to the output file
        bw_write_bit(&writer, code, length);
    }

    // Flush the BitWriter to ensure all bits are written
    bw_flush(&writer);

#if DEBUG
    FILE *debug_file = fopen("tree.bin", "wb");
    if (debug_file) {
        fwrite(map, sizeof(ByteFreqMap), 1, debug_file);
        fclose(debug_file);
    } else {
        perror("Failed to open debug file");
    }
#endif

    return 0;  // Success
    
}
