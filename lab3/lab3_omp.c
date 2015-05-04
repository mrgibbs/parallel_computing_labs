#define VECTOR_SIZE 5

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int num_threads, thread_id;

int getRandom(int min, int max)
{
	return min + rand() % (max - min + 1);
}

void printVector(int* vect, int size)
{
	printf("[ ");
	for (int i = 0; i < size - 1; ++i) 
	{
		printf(" %d,", vect[i]);
	}
	printf("%d ]", vect[size - 1]);
}
 
int main()
{
	int* vect_1 = (int*)malloc(sizeof(int) * VECTOR_SIZE);
	int* vect_2 = (int*)malloc(sizeof(int) * VECTOR_SIZE);
	int* vect_sum = (int*)malloc(sizeof(int) * VECTOR_SIZE);

	#pragma opm parallel for
	for (int i = 0; i < VECTOR_SIZE; ++i)
	{
		vect_1[i] = getRandom(0, 10);
		vect_2[i] = getRandom(0, 10);
	}


	#pragma omp parallel for
	for(int i = 0; i < VECTOR_SIZE; ++i)
	{
	  vect_sum[i] = vect_1[i] + vect_2[i];
	}
	
	printf("Vector 1: ");
	printVector(vect_1, VECTOR_SIZE);
	printf("\nVector 2: ");
	printVector(vect_2, VECTOR_SIZE);
	printf("\nResult Vector: ");
	printVector(vect_sum, VECTOR_SIZE);
	printf("\n");
	
	// #pragma omp threadprivate(thread_id)
	// #pragma omp parallel
	// {
	// 	printf("%d: before!\n", thread_id);
	// 	thread_id = omp_get_thread_num();
	// 	printf("%d: Hello world!\n", thread_id);
	 
	// 	if(thread_id == 0)
	// 	{
	// 		num_threads = omp_get_num_threads();
	// 		printf("Number of threads: %d\n", num_threads);
	// 	}
	// }
 
	return 0;
} 