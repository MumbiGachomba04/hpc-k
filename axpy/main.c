#include "hpck.h"

void usage(int argc, char *argv[]){
   printf("Usage: %s [OPTIONS]\n", argv[0]);
}

int main(int argc, char *argv[])
{
   struct timeval tv1, tv2;
   struct timezone tz;
   double kernel_time, initialize_time, finalize_time;

   gettimeofday(&tv1, &tz);
   void * args = hpck_initialize(argc, argv);
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   gettimeofday(&tv1, &tz);
   hpck_kernel(args);
   gettimeofday(&tv2, &tz);
   kernel_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   gettimeofday(&tv1, &tz);
   hpck_finalize(args);
   gettimeofday(&tv2, &tz);
   finalize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   ////////////////// Print results ////////////////////////
   printf("%-30s: %.3lf\n", "Initialization time (seconds)", initialize_time);
   printf("%-30s: %.3lf\n", "Kernel compute time (seconds)", kernel_time);
   printf("%-30s: %.3lf\n", "Kernel throughput (GFLOPs/s)", (kernel_time) / 1E+9);// TODO
   printf("%-30s: %.3lf\n", "Finalization time (seconds)", finalize_time);
#if 0
   printf("%-30s: %.4f\n", "Expected result", expected);
   printf("%-30s: %.6lf\n", "Mean absolut error", abs_err);
   printf("%-30s: %.6lf\n", "Relative error", rel_err);
   printf("%-30s: %s\n", "Result verification", rel_err < EPSILON?"pass":"fail");
   printf("================================================================\n");
#endif
   ////////////////////// EXIT ////////////////////////////
   return EXIT_SUCCESS;
}
