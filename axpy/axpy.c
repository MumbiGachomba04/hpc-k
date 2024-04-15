#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

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

void AXPY (long n, long reps, REAL alpha, REAL *X, REAL *Y)
{
   long r, i;

   for(r=0; r<reps; r++)
      for(i=0; i<n; i++)
         Y[i] = alpha*X[i]+Y[i];

}

void usage(int argc, char *argv[]){
   printf("Usage: %s <array-size> <repetitions>\n", argv[0]);
}

int main(int argc, char *argv[])
{
   long n, reps;
   double N;
   REAL *X, *Y;
   int i=0, j=0;
   struct timeval tv1, tv2;
   struct timezone tz;
   double kernel_time, initialize_time, finalize_time;

   if (argc != 3) {
      printf("Error: parsing command line arguments.\n");
      usage(argc, argv);
      exit(EXIT_FAILURE);
   }

   n  = atoi(argv[1]);
   reps = atoi(argv[2]);

   N = (double) n;

   printf("================================================================\n");
   printf("= %-60s =\n", "AXPY Kernel: Y=a*X+Y");
   printf("================================================================\n");
   //printf("%-30s: %s\n", "Compiler", HPCK_Compiler );
   printf("%-30s: %ld\n", "Array size (elements)", n);
   printf("%-30s: %ld\n", "Repetitions", reps);
   printf("%-30s: %ld\n", "Element size (bytes)", sizeof(REAL));
   printf("%-30s: %d\n", "Number of threads", omp_get_max_threads());
   printf("----------------------------------------------------------------\n");
   gettimeofday(&tv1, &tz);
   ////////////////////// Array X ///////////////////////////
   X = (REAL *) malloc(n*sizeof(REAL));
   if(!X) {
      printf("Error: memory failed allocating Array X.\n");
      exit(EXIT_FAILURE);
   }
   ////////////////////// Array Y ///////////////////////////
   Y = (REAL *) malloc(n*sizeof(REAL));
   if(!Y) {
      printf("Error: memory failed allocating Array Y.\n");
      exit(EXIT_FAILURE);
   }
   /////////////////// Initializing ////////////////////////
   for(i=0; i<n; i++) {
         X[i] = X_INIT;
         Y[i] = Y_INIT;
      }
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Matrix Multiply //////////////////////
   gettimeofday(&tv1, &tz);
   AXPY(n,reps,A_INIT,X,Y);
   gettimeofday(&tv2, &tz);
   kernel_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Verification /////////////////////////
   gettimeofday(&tv1, &tz);
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
   gettimeofday(&tv2, &tz);
   finalize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Print results ////////////////////////
   printf("%-30s: %.3lf\n", "Initialization time (seconds)", initialize_time);
   printf("%-30s: %.3lf\n", "Kernel compute time (seconds)", kernel_time);
   printf("%-30s: %.3lf\n", "Kernel throughput (GFLOPs/s)", ((N*2*reps)/kernel_time) / 1E+9);
   printf("%-30s: %.3lf\n", "Finalization time (seconds)", finalize_time);
   printf("%-30s: %.4f\n", "Expected result", expected);
   printf("%-30s: %.6lf\n", "Mean absolut error", abs_err);
   printf("%-30s: %.6lf\n", "Relative error", rel_err);
   printf("%-30s: %s\n", "Result verification", rel_err < EPSILON?"pass":"fail");
   printf("================================================================\n");
   ////////////////////// EXIT ////////////////////////////
   return EXIT_SUCCESS;
}
