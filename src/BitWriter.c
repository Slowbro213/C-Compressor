#include "../lib/BitWriter.h"
#include <stdio.h>


void bw_init(BitWriter *bw, FILE *file) {
    bw->byte = 0;
    bw->file = file;
    bw->bit_pos = 0;
}


void bw_write_bit(BitWriter *bw, uint8_t bits,int length) {
    
    for (int i = 0; i < length; i++) {
        bw->byte = (bw->byte << 1) | ((bits >> (length - 1 - i)) & 1);
        bw->bit_pos++;
        if (bw->bit_pos == 8) {
            fwrite(&bw->byte, sizeof(uint8_t), 1, bw->file);
            bw->byte = 0;
            bw->bit_pos = 0;
        }
    }
}


int bw_flush(BitWriter *bw) {
    if (bw->bit_pos > 0) {
        bw->byte <<= (8 - bw->bit_pos);  
        fwrite(&bw->byte, sizeof(uint8_t), 1, bw->file);
        bw->byte = 0;
    }
    return 8 - bw->bit_pos;
}
