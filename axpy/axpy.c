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
   long reps;
   REAL alpha;
   REAL *X;
   REAL *Y;
} args_t;

void hpck_kernel (void *args)
{
   long n     = ((args_t*)args)->n;
   long reps  = ((args_t*)args)->reps;
   REAL alpha = ((args_t*)args)->alpha;
   REAL *X    = ((args_t*)args)->X;
   REAL *Y    = ((args_t*)args)->Y;

   for(long r=0; r<reps; r++)
      for(long i=0; i<n; i++)
         Y[i] = alpha*X[i]+Y[i];
}

void * hpck_initialize(int argc, char *argv[])
{
   args_t *args = (args_t *) malloc(sizeof(args_t));

   args->n = atoi(argv[1]);
   args->reps = atoi(argv[2]);
   args->alpha = A_INIT;

   ////////////////////// Array X ///////////////////////////
   args->X = (REAL *) malloc(args->n*sizeof(REAL));
   if(!args->X) {
      printf("Error: memory failed allocating Array X.\n");
      exit(EXIT_FAILURE);
   }
   ////////////////////// Array Y ///////////////////////////
   args->Y = (REAL *) malloc(args->n*sizeof(REAL));
   if(!args->Y) {
      printf("Error: memory failed allocating Array Y.\n");
      exit(EXIT_FAILURE);
   }
   ///////////////////// Initialize /////////////////////////
   for(int i=0; i<args->n; i++) {
      args->X[i] = X_INIT;
      args->Y[i] = Y_INIT;
   }

   hpck_print_settings("Array size (elements)", "%ld", args->n);
   hpck_print_settings("Repetitions", "%ld", args->reps);
   hpck_print_settings("Element size (bytes)","%ld", sizeof(REAL));
   hpck_print_settings("Number of threads","%d", omp_get_max_threads());

   return (void *) args;
}

int hpck_finalize(void *args)
{
   long n     = ((args_t*)args)->n;
   long reps  = ((args_t*)args)->reps;
   REAL *X    = ((args_t*)args)->X;
   REAL *Y    = ((args_t*)args)->Y;

   double abs_err = 0.0, rel_err = 0.0;

   REAL expected = Y_INIT;
   for(int i=0; i<reps; i++) expected = A_INIT*X_INIT+expected; // Algorithmic solution (sequential)

   for(int i=0; i<n; i++)
         abs_err += fabs(Y[i] - expected);

   abs_err = abs_err / (REAL) n;
   rel_err = abs_err / expected;

   ////////////////// Deallocating /////////////////////////
   free(X);
   free(Y);

   hpck_print_results("Expected result", "%.6lf", expected);
   hpck_print_results("Mean absolut error", "%.6lf", abs_err);
   hpck_print_results("Relative error", "%.6lf", rel_err);

   return rel_err<EPSILON?1:-1;
}
