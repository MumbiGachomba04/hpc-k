#include "hpck.h"
#include <math.h>

#define A_INIT (REAL) (11.11/13)
#define X_INIT (REAL) (22.22/23)
#define Y_INIT (REAL) (11.11/43)

#define EPSILON 1E-03

char *hpck_kernel_name="AXPY Kernel: Y=a*X+Y";

char  hpck_kernel_args_count = 2;
char *hpck_kernel_args_key[] = {NULL};
char *hpck_kernel_args_desc[] = {"Array size","Repetitions"};
char  hpck_kernel_args_needed[] = {TRUE,TRUE};

typedef struct {
   long n;
   long r;
   REAL alpha;
   REAL *X;
   REAL *Y;
} args_t;

void hpck_kernel (void *args)
{
   long n     = ((args_t*)args)->n;
   long r     = ((args_t*)args)->r;
   REAL alpha = ((args_t*)args)->alpha;
   REAL *X    = ((args_t*)args)->X;
   REAL *Y    = ((args_t*)args)->Y;

   for(long j=0; j<r; j++)
      for(long i=0; i<n; i++)
         Y[i] = alpha*X[i]+Y[i];
}

void * hpck_initialize(void)
{
   args_t *args = (args_t *) malloc(sizeof(args_t));

   args->n = atol(hpck_get_arg_idx(0));
   args->r = atol(hpck_get_arg_idx(1));
   args->alpha = A_INIT;

   args->X = (REAL *) malloc(args->n*sizeof(REAL));
   if(!args->X) hpck_error("memory failed allocating Array X",20);

   args->Y = (REAL *) malloc(args->n*sizeof(REAL));
   if(!args->Y) hpck_error("memory failed allocating Array Y");

   for(long i=0; i<args->n; i++) {
      args->X[i] = X_INIT;
      args->Y[i] = Y_INIT;
   }

   hpck_print_settings("Array size (elements)", "%ld", args->n);
   hpck_print_settings("Repetitions", "%ld", args->r);
   hpck_print_settings("Element size (bytes)","%ld", sizeof(REAL));
   hpck_print_settings("Number of threads","%d", omp_get_max_threads());

   return (void *) args;
}

int hpck_finalize(void *args)
{
   long n  = ((args_t*)args)->n;
   long r  = ((args_t*)args)->r;
   REAL *X = ((args_t*)args)->X;
   REAL *Y = ((args_t*)args)->Y;

   double abs_err = 0.0, rel_err = 0.0;

   REAL expected = Y_INIT;
   for(int i=0; i<r; i++) expected = A_INIT*X_INIT+expected; // Algorithmic solution (sequential)

   for(long i=0; i<n; i++) abs_err += fabs(Y[i] - expected);

   abs_err = abs_err / (REAL) n;
   rel_err = abs_err / expected;

   free(X);
   free(Y);

   hpck_print_results("Expected result", "%.6lf", expected);
   hpck_print_results("Mean absolut error", "%.6lf", abs_err);
   hpck_print_results("Relative error", "%.6lf", rel_err);

   return rel_err<EPSILON?1:-1;
}
