#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>  
#include <stddef.h>

typedef struct { 
    uint8_t order[256];
    int count;
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
uint8_t bfm_get_code(const ByteFreqMap *map, uint8_t key);


void bfm_print(const ByteFreqMap *map);
void bfm_print_by_order(const ByteFreqMap *map);

void count_frequencies(uint8_t* chunk, size_t size, void* args);


#endif // HASHMAP_H
