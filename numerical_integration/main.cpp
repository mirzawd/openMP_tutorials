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
   double x_1 = 0.; 
   double x_2 = 1.;
   int steps  = 1E6;
   double delta_x = (x_2-x_1)/steps;
   int N = 1000;
   // Avg time of 100 runs for one processor 
   double time_one_proc =1; 
   for(int num_proc_idx=2;num_proc_idx<13;num_proc_idx++)
   {
	double time = 0;
	double sum = 0;
        omp_set_num_threads(num_proc_idx);
	double offset = ceil(steps/num_proc_idx);
   	for (int run_idx = 0 ; run_idx<N;run_idx++)
   	{
  		double t_start  = omp_get_wtime();

		#pragma omp parallel private(proc) // Start of parallel region: forks threads
   		{
     			proc = omp_get_thread_num();  
     			double loc_sum=0;
			//#pragma omp for nowait schedule(static) 
		        for (int step_Idx= proc*offset;  step_Idx< (proc+1)*offset  ; step_Idx++)
			{
		    	    double x      =  delta_x*step_Idx;
			    loc_sum += 4.0*delta_x/(1. + x*x);  
			}
			
			#pragma omp atomic
     			sum = sum + loc_sum; 
		}
		
   		time = time + omp_get_wtime()-t_start;
   	}

	if (num_proc_idx==1) time_one_proc = time/N; 
   	time = time/N/time_one_proc;
   	std::cout<<"time time taken by proc "<<num_proc_idx<<" is "<<time<<" and the value of pi is calculated to be  "<<sum/N<<std::endl;
   }
   return 0;
}
