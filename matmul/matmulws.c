#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>  

#ifndef _OPENMP
#define omp_get_max_threads() 1
#define omp_get_num_threads() 1
#define omp_get_thread_num()  0
#endif

#ifndef REAL
#define REAL double
#endif

#define A_INIT (REAL) (101.101/17)
#define B_INIT (REAL) (202.202/13)
#define C_INIT (REAL) 0.0

// Arithmetic solution computes the result via formula.
// Otherwise, it uses an algorithmic approac reproducing the
// sequential set of multiplications and additions.
//#define USE_ARITHMETIC_SOLUTION

#ifdef USE_ARITHMETIC_SOLUTION
#  define EPSILON 1E-3
#else
#  define EPSILON 1E-3
#endif

void MatMul (int n, int ts, REAL **a, REAL **b, REAL **c) // IKJ
{
   int ii, jj, kk, i, j, k;
   #pragma omp parallel for collapse(4) schedule(static) default(none) shared(a,b,c,n,ts) private(ii, jj, kk, i, j, k) 
   for(ii=0; ii<n; ii+=ts)
      for(kk=0; kk<n; kk+=ts)
         for(jj=0; jj<n; jj+=ts)   
            for(i=0; i<ts; i++)
               for(k=0; k<ts; k++) 
                  for(j=0; j<ts; j++)
                     c[ii+i][jj+j] += a[ii+i][kk+k] * b[kk+k][jj+j];

}

// #pragma omp parallel for schedule (static)

void usage(int argc, char *argv[]){
   printf("Usage: %s <matrix-size> <tile-size>\n", argv[0]);
   printf("Where: <matrix-size> must be a multiple of <tile-size>\n\n");
}

int main(int argc, char *argv[])
{
   int n, ts;
   double N;
   REAL **A, **B, **C;
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
   ts = atoi(argv[2]);

   N = (double) n;

   if ( n%ts ) {
      printf("Error: incorrect values, %d is not a multiple of %d.\n", n, ts);
      usage(argc, argv);
      exit(EXIT_FAILURE);
   }
   printf("================================================================\n");
   printf("= %-60s =\n", "Square tiled matrix multiplication: C=A*B");
   printf("================================================================\n");
   printf("%-30s: %dx%d\n", "Matrix size (elements)", n, n);
   printf("%-30s: %dx%d\n", "Tile size (elements)", ts, ts);
   printf("%-30s: %ld\n", "Element size (bytes)", sizeof(REAL));
   printf("%-30s: %d\n", "Number of threads", omp_get_max_threads());
   printf("----------------------------------------------------------------\n");
   ///////////////////// Matrix A //////////////////////////
   gettimeofday(&tv1, &tz);
   A = (REAL **) malloc(n*sizeof(REAL *));
   A[0] = (REAL *) malloc(n*n*sizeof(REAL));
   if(!A || !A[0]) {
      printf("Error: memory failed allocating Matrix A.\n");
      exit(EXIT_FAILURE);
   }
   for(i=1; i<n; i++) A[i] = A[0]+i*n;
   ///////////////////// Matrix B //////////////////////////
   B = (REAL **) malloc(n*sizeof(REAL *));
   B[0] = (REAL *) malloc(n*n*sizeof(REAL));
   if(!B || !B[0]) {
      printf("Error: memory failed allocating Matrix B.\n");
      exit(EXIT_FAILURE);
   }
   for(i=1; i<n; i++) B[i] = B[0]+i*n;
   ///////////////////// Matrix C //////////////////////////
   C = (REAL **) malloc(n*sizeof(REAL *));
   C[0] = (REAL *) malloc(n*n*sizeof(REAL));
   if(!C || !C[0]) {
      printf("Error: memory failed allocating Matrix C.\n");
      exit(EXIT_FAILURE);
   }
   for(i=1; i<n; i++) C[i] = C[0]+i*n;
   /////////////////// Initializing ////////////////////////
   for(i=0; i<n; i++)
      for(j=0; j<n; j++) {
         A[i][j] = A_INIT;
         B[i][j] = B_INIT;
         C[i][j] = C_INIT;
      }
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Matrix Multiply //////////////////////
   gettimeofday(&tv1, &tz);
   MatMul(n,ts,A,B,C);
   gettimeofday(&tv2, &tz);
   kernel_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Verification /////////////////////////
   gettimeofday(&tv1, &tz);
   double abs_err = 0.0, rel_err = 0.0;

#ifdef USE_ARITHMETIC_SOL
   REAL expected = C_INIT+N*A_INIT*B_INIT; // Arithmetic solution (real)
#else // algorithmic solution
   REAL expected = C_INIT;
   for(i=0; i<n; i++) expected += A_INIT * B_INIT; // Algorithmic solution (sequential)
#endif

   for(i=0; i<n; i++)
      for(j=0; j<n; j++) {
         abs_err += fabs(C[i][j] - expected);
      }

   abs_err = abs_err / (N * N);
   rel_err = abs_err / expected;

   ////////////////// Deallocating /////////////////////////
   free(A[0]); free(A);
   free(B[0]); free(B);
   free(C[0]); free(C);
   gettimeofday(&tv2, &tz);
   finalize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Print results ////////////////////////
   printf("%-30s: %.3lf\n", "Initialization time (seconds)", initialize_time);
   printf("%-30s: %.3lf\n", "Kernel compute time (seconds)", kernel_time);
   printf("%-30s: %.3lf\n", "Kernel throughput (GFLOPs/s)", ((N*N*N*2)/kernel_time) / 1E+9);
   printf("%-30s: %.3lf\n", "Finalization time (seconds)", finalize_time);
   printf("%-30s: %.4lf\n", "Expected result", expected);
   printf("%-30s: %.6lf\n", "Mean absolut error", abs_err);
   printf("%-30s: %.6lf\n", "Relative error", rel_err);
   printf("%-30s: %s\n", "Result verification", rel_err < EPSILON?"pass":"fail");
   printf("================================================================\n");
   ////////////////////// EXIT ////////////////////////////
   return EXIT_SUCCESS;
}
