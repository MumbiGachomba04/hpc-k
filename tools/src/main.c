#include "hpck.h"

char * hpck_kernel_name __attribute__((weak)) = "<not defined>";

extern char  hpck_kernel_args_count;
extern char* hpck_kernel_args_key[];
extern char* hpck_kernel_args_desc[];
extern char  hpck_kernel_args_needed[];

char ** __hpck_kernel_args_values = NULL;

char * __hpck_compiler  __attribute__((weak)) = "n/a";
char * __hpck_cflags    __attribute__((weak)) = "n/a";
char * __hpck_ldflags   __attribute__((weak)) = "n/a";
char * __hpck_includes  __attribute__((weak)) = "n/a";
char * __hpck_libraries __attribute__((weak)) = "n/a";

int    __hpck_argc = 0;
char **__hpck_argv = NULL;

void __hpck_print(const char* fmt, va_list args)
{
   vprintf(fmt, args);
}

void __hpck_print_usage(void)
{
   printf("Usage: %s [options]\n", __hpck_argv[0]);
   printf("Where [options] are:\n");
   if (!hpck_kernel_args_key[0]) {
      for (int i=0; i < hpck_kernel_args_count; i++) {
         fprintf(stderr, "   <arg-%d>: %s (%s)\n",
               i+1, hpck_kernel_args_desc[i], hpck_kernel_args_needed[i]?"needed":"optional");
      }
   } else {
      // TODO: named arguments
   }
}

void __hpck_parse_arguments(void)
{
   int count = hpck_kernel_args_count;

   __hpck_kernel_args_values = (char **) malloc (count*sizeof(char *));

   if (!hpck_kernel_args_key[0]) {
      int i = 0;
      while (i<count && (i+1)<__hpck_argc) {
         __hpck_kernel_args_values[i] = __hpck_argv[i+1];
         i++;
      }
      while (i<count) {
         __hpck_kernel_args_values[i] = NULL;
         if (hpck_kernel_args_needed[i]) {
            __hpck_print_usage();
            printf("Error: Argument <arg-%d> needed and not provided.\n",i+1);
            exit(EXIT_FAILURE);
         }
         i++;
      }
   } else {
      // TODO: named arguments
   }
}

char*hpck_get_arg_idx(int idx)
{
   return __hpck_kernel_args_values[idx];
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
   __hpck_argc = argc;
   __hpck_argv = (char **) argv;

   char *str_result[3] = {"fail","n/a","pass"};
   struct timeval tv1, tv2;
   struct timezone tz;
   double kernel_time, initialize_time, finalize_time;

   __hpck_parse_arguments();

   hpck_print_header(hpck_kernel_name);

   hpck_print_settings("Compiler", "%s", __hpck_compiler);
   hpck_print_settings("Compiler flags", "%s", __hpck_cflags);
   hpck_print_settings("Linker flags", "%s", __hpck_ldflags);
   hpck_print_settings("Includes", "%s", __hpck_includes);
   hpck_print_settings("Libraries", "%s", __hpck_libraries);

   gettimeofday(&tv1, &tz);
   void * args = hpck_initialize();
   gettimeofday(&tv2, &tz);
   initialize_time = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;

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
