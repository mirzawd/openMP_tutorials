#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main() {
   int tid = -1;
#pragma omp parallel private(tid) // Start of parallel region: forks threads
   {
     tid = omp_get_thread_num();  // default is number of CPUs on machine
     printf("Hello from Thread %d\n",tid);
     if(tid ==0) {
        printf("Number of threads = %d\n", omp_get_num_threads());
     }
   }  // ** end of the the parallel: joins threads
   return 0;
}
