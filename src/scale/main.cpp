#include "scale.h"
#include "image.h"
#include "stdio.h"
#include <math.h>
//#include "rely.h"
//for software analysis.
#include "time.h"
#include "float.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef AMHM_APPROXIMATION
int reliability_level = 0;
#endif 

extern "C" float my_rand(){
	return ((float) rand())/RAND_MAX;
}

extern "C" float my_rand_float(){
	return (my_rand()*(FLT_MAX - FLT_MIN) + FLT_MIN);
}

extern "C" float my_floor(float v)
{
   return (float) floor((double)v);
}

extern "C" float my_ceil(float v)
{
   return (float) ceil((double)v);
}


extern "C" void print_float_coord(float x, float y)
{
  printf("(%f, %f)", x, y);
}
extern "C" void print_int_coord(int x, int y)
{
  printf("(%d, %d)", x, y);
}
extern  "C" void print_newline()
{
  printf("\n");
}

int * allocate_transform_image(double scale_factor, size_t sw, size_t sh, size_t *dw, size_t *dh, int *num_elements){
	int * result;
	(*dw) = ceil(sw*scale_factor);
	(*dh) = ceil(sh*scale_factor);
	
	printf("transformed image size: %zu x %zu\n", *dw, *dh);
	*num_elements = (*dw)*(*dh)*3;
	result = (int*) malloc(*num_elements*sizeof(int));
	if(result == NULL){
		fprintf(stderr, ">> Could not allocate destination image..\n");
		exit(1);
	}
	
#ifdef AMHM_APPROXIMATION
	m5_add_approx((uint32_t)&result[0], (uint32_t)(&result[*num_elements]+sizeof(int) - 1), reliability_level);
#endif 
	
	return result;
}

int main(int argc, char**argv){
	char * evar = getenv("RELY_SRAND_DATA");
	if(evar != NULL) srand(atoi(evar));
	else srand(0);
	
	#ifdef AMHM_APPROXIMATION 
	if(argc <= 4){
		printf("USAGE: scale FACTOR INPUT OUTPUT RELIABILITY_LEVEL\n");
		return 1;
	}
	#endif 
	
	#ifndef AMHM_APPROXIMATION 
	if(argc <= 3){
		printf("USAGE: scale FACTOR INPUT OUTPUT\n");
		return 1;
	}
	#endif 
	
	double scale_factor = atof(argv[1]);
	char* in_filename = argv[2];
	char * out_filename = argv[3];
	
	#ifdef AMHM_APPROXIMATION 
	reliability_level = atoi(argv[4]);
	#endif 
	
	printf("scale by %f: %s -> %s\n", scale_factor, in_filename, out_filename);

	//rely_int_array_1D src, transformed;
	int *src, *transformed;
	int src_dim, transformed_dim;

	size_t sw, sh, dw, dh;
	printf("read from \"%s\" ...\n", in_filename);
	src = read_image(in_filename, &sw, &sh, &src_dim);
	//RELY_ARRAY_NALLOCATED_1D(src, "src")

	if(src == NULL){
		fprintf(stderr, ">> Failed to read image %s\n", in_filename);
		exit(1);
	}
	transformed = allocate_transform_image(scale_factor, sw, sh, &dw, &dh, &transformed_dim);
	//RELY_ARRAY_NALLOCATED_1D(transformed, "transformed")
	
	scale(scale_factor, src, sw, sh, transformed, dw, dh);
	printf("write to \"%s\" ...\n", out_filename);
	write_image(out_filename, transformed, dw, dh);

#ifdef AMHM_APPROXIMATION 
    m5_remove_approx((uint32_t)&src[0], (uint32_t)(&src[src_dim]+sizeof(int) - 1), reliability_level);
#endif  

#ifdef AMHM_APPROXIMATION 
    m5_remove_approx((uint32_t)&transformed[0], (uint32_t)(&transformed[transformed_dim]+sizeof(int) - 1), reliability_level);
#endif 
	
	free((void *) src);
	free((void *) transformed);
}
