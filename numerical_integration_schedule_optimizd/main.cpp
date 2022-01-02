#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdio.h>



int main() 
{
   int proc = -1;
   int num_procs = -1;
   double x_1 = 0.; 
   double x_2 = 1.;
   int steps  = 1E6;
   double delta_x = (x_2-x_1)/steps;
   int N = 1000;
   double time{};
   // Avg time of 100 runs for one processor 
   double time_one_proc =0.0105018; 
   for(int num_proc_idx=1;num_proc_idx<2;num_proc_idx++)
   {
   	for (int run_idx = 0 ; run_idx<N;run_idx++)
   	{
   		double sum = 0.;
  		double t_start  = omp_get_wtime();
		#pragma omp parallel private(proc) // Start of parallel region: forks threads
   		{
     			proc = omp_get_thread_num();  
     			num_procs  = omp_get_num_threads();	
     			double loc_sum=0;
			#pragma omp for schedule(static)
		        for (int step_Idx= 0 ; step_Idx<steps; step_Idx++)
			{
		    	    double x      =  delta_x*step_Idx;
			    loc_sum += 4.0*delta_x/(1. + x*x);  
			}
			
			#pragma omp atomic
     			sum = sum + loc_sum; 
     			//}
   			  // ** end of the the parallel: joins threads
		}
		//std::cout<<sum<<std::endl;
   		time = time + omp_get_wtime()-t_start;
   		//printf("Area under the curve is %f in %f sec",sum,omp_get_wtime()-t_start);
		//std::cout<<sum<<std::endl;	
   	}

   	time = time/N/time_one_proc;
   	std::cout<<time<<std::endl;
   }
   return 0;
}
