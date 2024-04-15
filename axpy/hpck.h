#ifndef __HPCK
#define __HPCK
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef _OPENMP
#define omp_get_max_threads() 1
#define omp_get_num_threads() 1
#define omp_get_thread_num()  0
#endif

#ifndef REAL
#define REAL double
#endif

#define A_INIT (REAL) (11.11/13)
#define X_INIT (REAL) (22.22/23)
#define Y_INIT (REAL) (11.11/43)

#define EPSILON 1E-3

void * hpck_initialize (int, char *[]);
void hpck_kernel(void *);
char hpck_finalize(void *);

#endif
