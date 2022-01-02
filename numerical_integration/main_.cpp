#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iostream>
#include <cmath>
#include <math.h>
#include <stdio.h>




void integrate(double &loc_sum, double delta_x, int start_step, int offset_step, int procs)
{
    double start_x      =  delta_x*start_step;
    double end_x        =  delta_x*(start_step+offset_step);
    //std::cout<<start_x<<" "<<end_x<<" "<<offset_step<<std::endl;
    double x = start_x;
    double tol = 1E-6;
    if (x<1. + tol)
    {
        for (int step_Idx= start_step ; step_Idx<start_step + offset_step; step_Idx++)
    	{	 
	    loc_sum += 4.0*delta_x/(1. + x*x);
	    x = x + delta_x;
            if (x>1+tol)
		break;
            //std::cout<<x<<std::endl; 
        }
    }

}


int main() 
{
   int proc = -1;
   int num_procs = -1;
   double x_1 = 0.; 
   double x_2 = 1.;
   int steps  = 1E7;
   double delta_x = (x_2-x_1)/steps;
   int N = 1E3;
   double time{};
   double time_one_proc{};// =0.00327256; 
   for(int num_proc_idx=1;num_proc_idx<omp_get_num_procs();num_proc_idx++)
   {
	time = 0; 
  	for (int run_idx = 0 ; run_idx<N;run_idx++)
   	{
   		double sum = 0.;
  		double t_start  = omp_get_wtime();
		omp_set_num_threads(num_proc_idx);
		#pragma omp parallel private(proc) // Start of parallel region: forks threads
   		{
     			proc = omp_get_thread_num();  
     			num_procs  = num_proc_idx;	
     			int offset_steps =  ceil( (double) steps/ (double) num_procs);
     			int start_step   =  proc*offset_steps;
     			double loc_sum=0;

     			//std::cout<<start_step<<std::endl;
     			//if (proc==2)
     			integrate(loc_sum, delta_x, start_step, offset_steps,proc);
     			//printf("Hello from Thread %f\n",loc_sum);

     			//for (int loc_sum_idx = 0 ; loc_sum_idx<num_procs ;loc_sum_idx++)	
     			//{
			#pragma omp atomic
     			sum = sum + loc_sum; 
     			//}
   		}  // ** end of the the parallel: joins threads
		//std::cout<<sum<<std::endl;
   		time = time + omp_get_wtime()-t_start;
   		//printf("Area under the curve is %f in %f sec",sum,omp_get_wtime()-t_start);
   		//std::cout<<sum<<std::endl;

   	}
	if (num_proc_idx==1) time_one_proc = time/N;
   	time = time/N/time_one_proc;
   	std::cout<<"time take for num. of procs "<<num_proc_idx<<" is "<<time<<" "<<std::endl;
   }
   return 0;
}
