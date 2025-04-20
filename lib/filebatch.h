#ifndef FILEBATCH_H
#define FILEBATCH_H

#include <stdint.h>  // For uint8_t
#include <stdio.h>   // For FILE
#include <stddef.h>  // For size_t

#define BUFFER_SIZE ( 1 << 16 )
#define PRINT_TIME 0

#if PRINT_TIME
#include <time.h>   
#endif


void process_file_batches(FILE* file, 
                         void (*process_chunk)(uint8_t* buffer, size_t size, void* args),
                         void* args);


#endif // FILEBATCH_H

