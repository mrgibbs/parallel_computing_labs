#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_SIZE 10
#define MIN_RAND 0
#define MAX_RAND 10
 
void copyArrPart(int* dest_arr, int start_index, int* src_arr, int src_arr_size);
void addVectors(int* v1, int* v2, int* v_sum, int size);
int getRandom(int min, int max);
void printVector(int* vect, int size);


int main (int argc, char* argv[])
{
	MPI_Init (&argc, &argv);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int PROCS_AMOUNT;
	MPI_Comm_size(MPI_COMM_WORLD, &PROCS_AMOUNT);

	const int VECT_PART_BUFFER_SIZE = VECTOR_SIZE / (PROCS_AMOUNT);


	if (rank == PROCS_AMOUNT - 1)     // Master code part
	{
		printf("Number of CPUs %d\n", PROCS_AMOUNT);
		
		
		//Initinializing data
		int* vect_1 = (int*)malloc(sizeof(int) * VECTOR_SIZE);
		int* vect_2 = (int*)malloc(sizeof(int) * VECTOR_SIZE);
		int* vect_sum = (int*)malloc(sizeof(int) * VECTOR_SIZE);
		for (int i = 0; i < VECTOR_SIZE; ++i)
		{
			vect_1[i] = getRandom(MIN_RAND, MAX_RAND);
			vect_2[i] = getRandom(MIN_RAND, MAX_RAND);
		}



		printf("\n\nvect_1: ");
		printVector(vect_1, VECTOR_SIZE);
		printf("\n\nvect_2: ");
		printVector(vect_2, VECTOR_SIZE);



		//sending tasks to other threads / processes / computers
		const int SLAVES_AMOUNT	= PROCS_AMOUNT - 1;	
		for (int i = 0; i < SLAVES_AMOUNT; ++i)
		{
			int starting_index = VECT_PART_BUFFER_SIZE * i; 
			MPI_Send(vect_1 + starting_index, VECT_PART_BUFFER_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(vect_2 + starting_index, VECT_PART_BUFFER_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
		}



		// printf("\n\nMaster before any calculations\n");
		// printf("Res vect:");
		// printVector(vect_sum, VECTOR_SIZE);



		//calculating sum of own vector part (last part)
		int last_part_starting_index = SLAVES_AMOUNT * VECT_PART_BUFFER_SIZE;
		int last_part_size = VECT_PART_BUFFER_SIZE + (VECTOR_SIZE % PROCS_AMOUNT);
		addVectors( vect_1 + last_part_starting_index,
					vect_2 + last_part_starting_index,
					vect_sum + last_part_starting_index,
					last_part_size
					);


		// printf("\n\nMaster after completing own calculations\n");
		// printf("Res vect:");
		// printVector(vect_sum, VECTOR_SIZE);


		//receiving results from slaves and copying it to vect_sum array
		MPI_Status status;
		int vect_sum_part = (int*)malloc(sizeof(int) * VECT_PART_BUFFER_SIZE);
		for (int i = 0; i < SLAVES_AMOUNT; ++i)
		{
			MPI_Recv(vect_sum_part, VECT_PART_BUFFER_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			int starting_index = VECT_PART_BUFFER_SIZE * i;
			copyArrPart(vect_sum, starting_index, vect_sum_part, VECT_PART_BUFFER_SIZE);
			

			// printf("\n\nMaster received result from slave #%d\n", i);
			// printf("Res vect:");
			// printVector(vect_sum, VECTOR_SIZE);
		}



		//printing results
		printf("\n\nComputation completed!\n");
		printf("Vector 1: ");
		printVector(vect_1, VECTOR_SIZE);
		printf("\nVector 2: ");
		printVector(vect_2, VECTOR_SIZE);
		printf("\nResult Vector: ");
		printVector(vect_sum, VECTOR_SIZE);
		printf("\n");

	} 
	else   //Slave code part
	{

		//initializing and receiving task
		MPI_Status status;
		int master_rank = PROCS_AMOUNT - 1;
		int* vect_part_1 = (int*)malloc(sizeof(int) * VECT_PART_BUFFER_SIZE);
		int* vect_part_2 = (int*)malloc(sizeof(int) * VECT_PART_BUFFER_SIZE);
		int* vect_part_sum = (int*)malloc(sizeof(int) * VECT_PART_BUFFER_SIZE);
		MPI_Recv(vect_part_1, VECT_PART_BUFFER_SIZE, MPI_INT, master_rank, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(vect_part_2, VECT_PART_BUFFER_SIZE, MPI_INT, master_rank, 0, MPI_COMM_WORLD, &status);

		//working on task
		addVectors(vect_part_1, vect_part_2, vect_part_sum, VECT_PART_BUFFER_SIZE);

		//sending result to master
		MPI_Send(vect_part_sum, VECT_PART_BUFFER_SIZE, MPI_INT, master_rank, 0, MPI_COMM_WORLD);
	}


  MPI_Finalize();
  return 0;
}

void copyArrPart(int* dest_arr, int start_index, int* src_arr, int src_arr_size) {
	for (int i = 0, j = start_index; i < src_arr_size; ++i, ++j) {
		dest_arr[j] = src_arr[i];
	}
}

void addVectors(int* v1, int* v2, int* v_sum, int size)
{
	for (int i = 0; i < size; ++i)
	{
		v_sum[i] = v1[i] + v2[i];
	}
}

int getRandom(int min, int max)
{
	return min + rand() % (max - min + 1);
}

void printVector(int* vect, int size)
{
	printf("[");
	for (int i = 0; i < size - 1; ++i) 
	{
		printf(" %d,", vect[i]);
	}
	printf(" %d ]", vect[size - 1]);
}

