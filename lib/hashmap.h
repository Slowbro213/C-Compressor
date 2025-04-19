#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>  

typedef struct { 
    uint8_t order[256];
    uint8_t count;
    struct {
        unsigned int freq;
        uint32_t code;
        uint8_t length;
    } freqs[256];
} ByteFreqMap;

void bfm_init(ByteFreqMap *map);

int  bfm_get(const ByteFreqMap *map, uint8_t key);
void bfm_set(ByteFreqMap *map, uint8_t key, int value);

void bfm_increment(ByteFreqMap *map, uint8_t key);
void bfm_decrement(ByteFreqMap *map, uint8_t key);

void bfm_sort_by_freq(ByteFreqMap *map);
void bfm_huffman_tree(ByteFreqMap *map);


void bfm_print(const ByteFreqMap *map);
void bfm_print_by_order(const ByteFreqMap *map);


#endif // HASHMAP_H
