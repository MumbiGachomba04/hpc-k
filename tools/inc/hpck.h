#ifndef __HPCK
#define __HPCK
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdarg.h>

#ifndef _OPENMP
#define omp_get_max_threads() 1
#define omp_get_num_threads() 1
#define omp_get_thread_num()  0
#endif

#define TRUE  1
#define FALSE 0

#ifndef REAL
#define REAL double
#endif

#ifndef INTEGER
#define INTEGER long
#endif

void*hpck_initialize(void);
void hpck_kernel(void *);
int  hpck_finalize(void *);

void hpck_print_rule(void);
void hpck_print_header(const char*);
void hpck_print_settings(const char*, const char*, ... );
void hpck_print_results(const char*, const char*, ... );

char*hpck_get_arg_idx(int);

#endif
