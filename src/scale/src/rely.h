#ifndef _RELY_H_
#define _RELY_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#undef __STDC_LIMIT_MACROS

#include <stdlib.h>
#include <stdio.h>

typedef int32_t rely_int;
typedef float rely_float;

#define RELY_ARRAY(type, n)\
struct {\
  type *ptr;\
  rely_int dims[n];\
} 

typedef struct {
  rely_int *ptr;
  rely_int dims[1];
} rely_int_array_1D;

typedef struct {
  rely_float *ptr;
  rely_int dims[1];
} rely_float_array_1D;






#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN

#endif


EXTERN void rely_add_array_counter(size_t cnt);
EXTERN unsigned long rely_get_array_counter();
EXTERN void rely_add_array_counter_name(void * mem, size_t cnt, const char * name);



#define RELY_ARRAY_ALLOCATED_1D(array) \
  rely_add_array_counter(sizeof(array.ptr[0]) * array.dims[0]);	\

#define RELY_ARRAY_NALLOCATED_1D(array, name)				\
  rely_add_array_counter_name(array.ptr, sizeof(array.ptr[0]) * array.dims[0], name); \


#define RELY_ARRAY_MALLOC_1D(array, n, type)\
  array.ptr = (rely_##type*) malloc(sizeof(array.ptr[0]) * n);\
  rely_add_array_counter(sizeof(array.ptr[0]) * n); \
  if (0 == array.ptr) {\
      fprintf(stderr, "Failed to allocate array %s\n", #array);\
      exit(1);\
  }\
  array.dims[0] = n; \
  rely_add_array_counter_name(array.ptr, sizeof(array.ptr[0]) * array.dims[0], #array); \


#define RELY_ARRAY_FREE(array)\
  free(array.ptr);


#define FO_INDEX_ERR -1



EXTERN rely_int rely_array_bounds_check_1D(rely_int *dims, rely_int idx, const char* file, rely_int line_number, const char *array_name);
EXTERN rely_int rely_array_bounds_check_fo_1D(rely_int *dims, rely_int idx, const char* file, rely_int line_number, const char *arname);

#define RELY_ARRAY_WRITE_1D(array, idx, e)\
  array.ptr[rely_array_bounds_check_1D(&array.dims[0], idx, __FILE__, __LINE__, #array)] = e;
 
#define RELY_ARRAY_READ_1D(array, idx)\
   array.ptr[rely_array_bounds_check_1D(&array.dims[0], idx, __FILE__, __LINE__, #array)]

#define RELY_ARRAY_PRINT_1D(array, idx)\
   printf("%f\n", array.ptr[rely_array_bounds_check_1D(&array.dims[0], idx, __FILE__, __LINE__, #array)]);




typedef struct {
  rely_int *ptr;
  rely_int dims[2];
} rely_int_array_2d;

typedef struct {
  rely_int *ptr;
  rely_int dims[3];
} rely_int_array_3d;


EXTERN float get_reliability_env();

#endif

