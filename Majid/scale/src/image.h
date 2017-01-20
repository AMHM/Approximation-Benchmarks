#ifndef IMAGE_H
#define IMAGE_H
#include "stddef.h"
#include "stdint.h"
typedef struct TYPEDEF_color_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

#ifdef __cplusplus 

extern "C" int* read_image(char * filename, size_t* w, size_t* h, int *num_elems);
extern "C" void write_image(char * filename, int * data, size_t w, size_t h); 

#else

extern int* read_image(char * filename, size_t* w, size_t* h, int *num_elems);
extern void write_image(char * filename, int * data, size_t w, size_t h); 


#endif

#endif
