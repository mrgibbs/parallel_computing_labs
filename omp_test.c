#include <omp.h>
#include <stdio.h>
 
int num_threads, thread_id;
 
int main()
{
	#pragma omp threadprivate(thread_id)
	#pragma omp parallel
	{
		printf("%d: before!\n", thread_id);
		thread_id = omp_get_thread_num();
		printf("%d: Hello world!\n", thread_id);
	 
		if(thread_id == 0)
		{
			num_threads = omp_get_num_threads();
			printf("Number of threads: %d\n", num_threads);
		}
	}
 
return 0;
} 