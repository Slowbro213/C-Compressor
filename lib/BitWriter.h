#ifndef BITWRITER_H
#define BITWRITER_H

#include <stdio.h>   // For FILE
#include <stdint.h>  // For uint8_t



typedef struct {
    int byte;      // Current byte being written to
    FILE *file;      // File pointer for writing
    size_t bit_pos;   // Current position in bits
} BitWriter;


void bw_init(BitWriter *bw, FILE *file);
void bw_write_bit(BitWriter *bw, uint8_t bit, int length);
int bw_flush(BitWriter *bw);


#endif // BITWRITER_H
