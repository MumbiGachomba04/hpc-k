#include "hpck.h"
#include <math.h>

#define A_INIT (REAL) (11.11/13)
#define X_INIT (REAL) (22.22/23)
#define Y_INIT (REAL) (11.11/43)

#define EPSILON 1E-3

typedef struct args_s{
   long n;
   long reps;
   REAL alpha;
   REAL *X;
   REAL *Y;
} args_t;


void hpck_kernel (void *a)
{
   args_t *args = (args_t *) a;
   long n = args->n;
   long reps = args->reps;
   REAL alpha = args->alpha;
   REAL *X = args->X;
   REAL *Y = args->Y;

   long r, i;

   for(r=0; r<reps; r++)
      for(i=0; i<n; i++)
         Y[i] = alpha*X[i]+Y[i];

}

void * hpck_initialize(int argc, char *argv[])
{
   args_t *args = (args_t *) malloc(sizeof(args_t));

   int i=0, j=0;

   if (argc != 3) {
      printf("Error: parsing command line arguments.\n");
      usage(argc, argv);
      exit(EXIT_FAILURE);
   }

   args->n  = atoi(argv[1]);
   args->reps = atoi(argv[2]);

   printf("================================================================\n");
   printf("= %-60s =\n", "AXPY Kernel: Y=a*X+Y");
   printf("================================================================\n");
   //printf("%-30s: %s\n", "Compiler", HPCK_Compiler );


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
   /////////////////// Initializing ////////////////////////
   for(i=0; i<args->n; i++) {
         args->X[i] = X_INIT;
         args->Y[i] = Y_INIT;
      }

   args->alpha = A_INIT;

   printf("%-30s: %ld\n", "Array size (elements)", args->n);
   printf("%-30s: %ld\n", "Repetitions", args->reps);
   printf("%-30s: %ld\n", "Element size (bytes)", sizeof(REAL));
   printf("%-30s: %d\n", "Number of threads", omp_get_max_threads());
   printf("----------------------------------------------------------------\n");

   return args;
}

char hpck_finalize(void *a){
   args_t *args = (args_t *) a;
   long n = args->n;
   long reps = args->reps;
   REAL alpha = args->alpha;
   REAL *X = args->X;
   REAL *Y = args->Y;

   int i;
   REAL N = (REAL) n;
   double abs_err = 0.0, rel_err = 0.0;

   REAL expected = Y_INIT;
   for(i=0; i<reps; i++) expected = A_INIT*X_INIT+expected; // Algorithmic solution (sequential)

   for(i=0; i<n; i++)
         abs_err += fabs(Y[i] - expected);

   abs_err = abs_err / N;
   rel_err = abs_err / expected;

   ////////////////// Deallocating /////////////////////////
   free(X);
   free(Y);

   return rel_err < EPSILON?1:-1;
}
