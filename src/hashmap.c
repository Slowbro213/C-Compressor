#include "../lib/hashmap.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h> 
#include <limits.h>
#include <ctype.h>  
#include <assert.h>
typedef struct {
  unsigned int freq;
  uint32_t code;    // Optional: for code storage
  uint8_t byte;
  uint8_t length;   // Optional: for code length
  int16_t left;     // Child indices (-1 for none)
  int16_t right;
  int16_t parent;   // Parent index (-1 for root)
} HuffmanNode;
void bfm_init(ByteFreqMap *map) {
  assert(map != NULL);
  memset(map, 0, sizeof(ByteFreqMap));
}

int bfm_get(const ByteFreqMap *map, uint8_t key) {
  return map->freqs[key].freq;
}

void bfm_set(ByteFreqMap *map, uint8_t key, int value) {
  assert(value >= 0);

  map->freqs[key].freq = value;
}

void bfm_increment(ByteFreqMap *map, uint8_t key) {
  if( map->freqs[key].freq++ == 0) {
    map->order[map->count] = key;
    ++map->count;
  }
}

void bfm_decrement(ByteFreqMap *map, uint8_t key) {
  if (map->freqs[key].freq > 0){ map->freqs[key].freq--; --map->count; }
}

void bfm_print(const ByteFreqMap *map) {
  for (int i = 0; i < 256; i++) {
    if (map->freqs[i].freq > 0) {
      printf("0x%02X: %d", i, map->freqs[i].freq);
      if (isprint(i)) printf(" ('%c')", (char)i);
      printf("\n");
    }
  }
  printf("Total unique bytes: %d\n", map->count);
}


void bfm_print_by_order(const ByteFreqMap *map) {
  for (int i = 0; i < map->count; i++) {
    int index = map->order[i];
    printf("0x%02X: %d", index, map->freqs[index].freq);
    if (isprint(index)) printf(" ('%c')", (char)index);
    printf("\n");
  }
  printf("Total unique bytes: %d\n", map->count);
}

void bfm_sort_by_freq(ByteFreqMap *map) {
  //insert sort function here
  for (int i = 1; i < map->count; i++) {
    uint8_t current_byte = map->order[i];
    unsigned int current_freq = map->freqs[current_byte].freq;
    int j = i - 1;
    while (j >= 0 && map->freqs[map->order[j]].freq > current_freq) {
      map->order[j + 1] = map->order[j];
      j--;
    }
    map->order[j + 1] = current_byte;
  }
}


static void assign_huffman_codes(const HuffmanNode *nodes, int node_count,ByteFreqMap *map) {



  // Stack for iterative traversal
  struct StackFrame {
    int node_idx;
    uint32_t current_code;
    uint8_t current_length;
  } stack[512];


  int stack_top = 0;
  stack[stack_top++] = (struct StackFrame){node_count - 1, 0, 0};  // Start at root
  //

  while (stack_top > 0) {
    struct StackFrame frame = stack[--stack_top];
    const HuffmanNode *node = &nodes[frame.node_idx];

    if (node->left == -1 && node->right == -1) {
      // Leaf node - store code in ByteFreqMap
      map->freqs[node->byte].code = frame.current_code;
      map->freqs[node->byte].length = frame.current_length;
      continue;
    }

    // Push right then left to process left first
    if (node->right != -1) {
      stack[stack_top++] = (struct StackFrame){
        node->right,
        (frame.current_code << 1) | 1,
        frame.current_length + 1
      };
    }
    if (node->left != -1) {
      stack[stack_top++] = (struct StackFrame){
        node->left,
        frame.current_code << 1,
        frame.current_length + 1
      };
    }
  }
}
static void print_binary(uint32_t code, uint8_t length) {
  for (int i = length - 1; i >= 0; i--) {
    putchar((code & (1 << i)) ? '1' : '0');
  }
}

void bfm_huffman_tree(ByteFreqMap *map) {


  HuffmanNode nodes[512];  // 256 leaves + 255 internal nodes
  int node_count = map->count;

  // Step 1: Initialize leaf nodes
  for (int i = 0; i < map->count; i++) {
    uint8_t byte = map->order[i];
    nodes[i] = (HuffmanNode){
      .freq = map->freqs[byte].freq,
      .code = 0,
      .byte = byte, 
      .length = 0,
      .left = -1,
      .right = -1,
      .parent = -1
    };
  }

  // Step 2: Build tree by merging nodes
  while (node_count < 512) {
    // Find two nodes with smallest frequencies
    int16_t min1 = -1, min2 = -1;
    unsigned int freq1 = UINT_MAX, freq2 = UINT_MAX;

    for (int i = 0; i < node_count; i++) {
      if (nodes[i].parent == -1) {  // Only consider unmerged nodes
        if (nodes[i].freq < freq1) {
          min2 = min1;
          freq2 = freq1;
          min1 = i;
          freq1 = nodes[i].freq;
        } else if (nodes[i].freq < freq2) {
          min2 = i;
          freq2 = nodes[i].freq;
        }
      }
    }

    if (min2 == -1) break;  // Only one node left -> done

    // Create new internal node
    nodes[node_count] = (HuffmanNode){
      .freq = nodes[min1].freq + nodes[min2].freq,
      .code = 0,
      .byte = 0,  // No byte for internal nodes
      .length = 0,
      .left = min1,
      .right = min2,
      .parent = -1
    };

    // Set parents of merged nodes
    nodes[min1].parent = node_count;
    nodes[min2].parent = node_count;

    node_count++;
  }


  assert(map->count <= 256);
  assert(map->count > 0);
  assert(node_count <= 512);
  assert(node_count > 0);

  // Step 3: Generate codes (optional)
  if (node_count > 0) {
    // Traverse from root (last node) to leaves
    for (int i = node_count - 1; i >= 0; i--) {
      if (nodes[i].left != -1) {
        nodes[nodes[i].left].code = (nodes[i].code << 1) | 0;
        nodes[nodes[i].left].length = nodes[i].length + 1;
      }
      if (nodes[i].right != -1) {
        nodes[nodes[i].right].code = (nodes[i].code << 1) | 1;
        nodes[nodes[i].right].length = nodes[i].length + 1;
      }
    }    }

  // Step 4: Print Huffman tree (optional)

  assign_huffman_codes(nodes, node_count, map);

   for (int i = 0; i < map->count; i++) {
       uint8_t byte = map->order[i];
       printf("Byte %c: Code ", 
              byte);
       print_binary(map->freqs[byte].code, map->freqs[byte].length);
       printf(" (Length: %d) ", map->freqs[byte].length);
       printf("Frequency: %d\n", map->freqs[byte].freq);
   } 
}


void count_frequencies(uint8_t* chunk, size_t size, void* args) {
  ByteFreqMap* map = (ByteFreqMap*)args;
  for (size_t i = 0; i < size; i++) {
    bfm_increment(map, chunk[i]);
  }
}


uint8_t bfm_get_code(const ByteFreqMap *map, uint8_t key) {
  return map->freqs[key].code;
}
