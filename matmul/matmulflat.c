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

#ifdef USE_ARITHMETIC_SOLUTION
#  define EPSILON 1E-3
#else
#  define EPSILON 1E-3
#endif


void MatMul(int n, int ts, REAL *a, REAL *b, REAL *c) // IKJ
{
    int ii, jj, kk, i, j, k;

    #pragma omp target enter data map(to: a[0:n*n], b[0:n*n]) map(alloc: c[0:n*n])
    {
        #pragma omp target teams distribute parallel for collapse(2)
        for (ii = 0; ii < n; ii += ts)
            for (kk = 0; kk < n; kk += ts)
                for (jj = 0; jj < n; jj += ts)
                    for (i = 0; i < ts; i++)
                        for (k = 0; k < ts; k++)
                            for (j = 0; j < ts; j++)
                                c[(ii + i) * n + (jj + j)] += a[(ii + i) * n + (kk + k)] * b[(kk + k) * n + (jj + j)];
    }
    #pragma omp target exit data map(from: c[0:n*n]) map(delete: a[0:n*n], b[0:n*n])
}

void usage(int argc, char *argv[]){
   printf("Usage: %s <matrix-size> <tile-size>\n", argv[0]);
   printf("Where: <matrix-size> must be a multiple of <tile-size>\n\n");
}

int main(int argc, char *argv[])
{
   int n, ts;
   double N;
   REAL *A, *B, *C;
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
   A = (REAL *) malloc(n * n * sizeof(REAL));
   if(!A) {
      printf("Error: memory failed allocating Matrix A.\n");
      exit(EXIT_FAILURE);
   }
   
   ///////////////////// Matrix B //////////////////////////
   B = (REAL *) malloc(n * n * sizeof(REAL));
   if(!B) {
      printf("Error: memory failed allocating Matrix B.\n");
      exit(EXIT_FAILURE);
   }

   ///////////////////// Matrix C //////////////////////////
   C = (REAL *) malloc(n * n * sizeof(REAL));
   if(!C) {
      printf("Error: memory failed allocating Matrix C.\n");
      exit(EXIT_FAILURE);
   }
 
   /////////////////// Initializing ////////////////////////
   for(i=0; i<n; i++)
      for(j=0; j<n; j++) {
         A[i * n + j] = A_INIT;
         B[i * n + j] = B_INIT;
         C[i * n + j] = C_INIT;
      }
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Matrix Multiply //////////////////////
   gettimeofday(&tv1, &tz);
   MatMul(n, ts, A, B, C);
   gettimeofday(&tv2, &tz);
   kernel_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
   ////////////////// Verification /////////////////////////
   gettimeofday(&tv1, &tz);
   double abs_err = 0.0, rel_err = 0.0;

#ifdef USE_ARITHMETIC_SOL
   REAL expected = C_INIT + N * A_INIT * B_INIT; // Arithmetic solution (real)
#else // algorithmic solution
   REAL expected = C_INIT;
   for(i=0; i<n; i++) expected += A_INIT * B_INIT; // Algorithmic solution (sequential)
#endif

   for(i=0; i<n; i++)
      for(j=0; j<n; j++) {
         abs_err += fabs(C[i * n + j] - expected);
      }

   abs_err = abs_err / (N * N);
   rel_err = abs_err / expected;

   ////////////////// Deallocating /////////////////////////
   free(A);
   free(B);
   free(C);
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
