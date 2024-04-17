#include "hpck.h"

char * __hpck_compiler  __attribute__((weak)) = "n/a";
char * __hpck_cflags    __attribute__((weak)) = "n/a";
char * __hpck_ldflags   __attribute__((weak)) = "n/a";
char * __hpck_includes  __attribute__((weak)) = "n/a";
char * __hpck_libraries __attribute__((weak)) = "n/a";

void __hpck_print(const char* fmt, va_list args)
{
   vprintf(fmt, args);
}

void hpck_print_usage(int argc, char *argv[]){
   printf("Usage: %s [OPTIONS]\n", argv[0]);
}

void hpck_print_rule(void)
{
   printf("================================================================\n");
}

void hpck_print_header (const char* str)
{
   hpck_print_rule();
   printf("= %-60s =\n", str);
   hpck_print_rule();
}

void hpck_print_settings(const char* name, const char *format, ...)
{
   va_list args;
   va_start(args,format);
   printf("%-30s: ",name);
   __hpck_print(format, args);
   printf("\n");
   va_end(args);
}

void hpck_print_results(const char* name, const char *format, ...)
{
   va_list args;
   va_start(args,format);
   printf("%-30s: ",name);
   __hpck_print(format, args);
   printf("\n");
   va_end(args);
}

int main(int argc, char *argv[])
{
   char *str_result[3] = {"fail","n/a","pass"};
   struct timeval tv1, tv2;
   struct timezone tz;
   double kernel_time, initialize_time, finalize_time;

   gettimeofday(&tv1, &tz);
   void * args = hpck_initialize(argc, argv);
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   hpck_print_settings("Compiler", "%s", __hpck_compiler);
   hpck_print_settings("Compiler flags", "%s", __hpck_cflags);
   hpck_print_settings("Linker flags", "%s", __hpck_ldflags);
   hpck_print_settings("Includes", "%s", __hpck_includes);
   hpck_print_settings("Libraries", "%s", __hpck_libraries);

   hpck_print_rule();

   gettimeofday(&tv1, &tz);
   hpck_kernel(args);
   gettimeofday(&tv2, &tz);
   kernel_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   gettimeofday(&tv1, &tz);
   int result = hpck_finalize(args);
   gettimeofday(&tv2, &tz);
   finalize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

   hpck_print_rule();

   printf("%-30s: %.3lf\n", "Initialization time (seconds)", initialize_time);
   printf("%-30s: %.3lf\n", "Kernel compute time (seconds)", kernel_time);
   printf("%-30s: %.3lf\n", "Kernel throughput (GFLOPs/s)", (kernel_time) / 1E+9);// TODO
   printf("%-30s: %.3lf\n", "Finalization time (seconds)", finalize_time);
   printf("%-30s: %s\n", "Result verification", str_result[result+1] );

   hpck_print_rule();

   return EXIT_SUCCESS;
}
