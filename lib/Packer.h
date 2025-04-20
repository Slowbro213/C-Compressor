#ifndef PACKER_H
#define PACKER_H

int pack_file(FILE *input_file, FILE *output_file, const ByteFreqMap *map);


int unpack_file(FILE *input_file, FILE *output_file, const ByteFreqMap *map);


#endif // PACKER_H
