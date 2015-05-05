//proc is process

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATRIX_SIZE 8

#define ALMOST_ZERO 0.0000001

#define MAX_FILENAME_SIZE 50
#define FILENAME_TEMPLATE "data/ps/p%d"

#define allocate(type, size) (type*)malloc(sizeof(type) * size)
#define fill_array(arr, size, default_value) for (int i = 0; i < size; ++i) {arr[i] = default_value;}

typedef struct advanced_row
{
	int was_sent;
	int iters_passed;
	int size;
	double* row;
} advanced_row;


void print_advanced_row(advanced_row a_row);
int is_almost_zero(double value);
// int find_proc_with_appropriate_row();


int main (int argc, char* argv[])
{
	//initializing MPI
	MPI_Init (&argc, &argv);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int PROCS_AMOUNT;
	MPI_Comm_size(MPI_COMM_WORLD, &PROCS_AMOUNT);

	int ROW_SIZE = MATRIX_SIZE + 1;

	const int MAX_ROWS_IN_FILE = (MATRIX_SIZE / PROCS_AMOUNT) + 1;

	//allocate memory for row pointers
	double** rows = allocate(double*, MAX_ROWS_IN_FILE);
	for (int i = 0; i < MAX_ROWS_IN_FILE; ++i)
	{
		rows[i] = NULL;
	}

	//open file
	char filename[50];
	sprintf(filename, FILENAME_TEMPLATE, rank);
	FILE * fp;
	fp = fopen (filename, "r+");

	//init rows with numbers from file
	double test_number;
	int row_number = 0;
	while(1)
	{
		int res = fscanf(fp, "%lf", &test_number);   //res checks whether it is end of file
		if (res == 1)
		{
			// row_size = MATRIX_SIZE + 1;
			rows[row_number] = allocate(double, ROW_SIZE);

			rows[row_number][0] = test_number;
			for (int i = 1; i < ROW_SIZE; ++i)
			{
				fscanf(fp, "%lf", rows[row_number] + i);
			}
		} else {
			break;
		}

		++row_number;
	}

	// //print initialized rows
	// for (int i = 0; i < MAX_ROWS_IN_FILE; ++i)
	// {
	// 	if(rows[i] == NULL)
	// 	{
	// 		break;
	// 	}
	// 	printf("rank: %d, row: %d, number_1: %.3lf, number_2: %.3lf, number_-2: %.3lf, number_-2 %.3lf\n", 
	// 		rank, i, rows[i][0], rows[i][1], rows[i][ROW_SIZE - 2], rows[i][ROW_SIZE - 1]);
	// }

	//calculate actual amount of rows
	int rows_amount = MAX_ROWS_IN_FILE;
	if (rows[MAX_ROWS_IN_FILE - 1] == NULL)
	{
		--rows_amount; 
	}

	//init advanced_rows
	advanced_row* adv_rows = allocate(advanced_row, rows_amount);
	for (int i = 0; i < rows_amount; ++i)
	{
		adv_rows[i].size = ROW_SIZE;
		adv_rows[i].iters_passed = 0;
		adv_rows[i].was_sent = 0;
		adv_rows[i].row = rows[i];
	}


	//print advanced rows
	for (int i = 0; i < rows_amount; ++i)
	{
		print_advanced_row(adv_rows[i]);
	}


	MPI_Barrier(MPI_COMM_WORLD);

	if(rank == 0) 
	{
		printf("all processes were initialized");
	}

	int iterations_completed = 0


	int first_part_iters_amount = MATRIX_SIZE - 1  //amount of iterations to make matrix low-triangle


	int next_proc = 0;   //next proc that will send row. it's iterated to make calculations more evenly distributed
	for (int i = 0; i < first_part_iters_amount; ++i)
	{
		int is_there_appropriate_row = 0;   //variable defines is there row to be sent in current process
											//appropriate row is row that hasn't been sent yet
		for (int i = 0; i < rows_amount; ++i)
		{
			if (adv_rows[i].was_sent == 0 && !is_almost_zero(adv_rows[i][iterations_completed]))
			{
				is_there_appropriate_row = 1;
				break;
			}
		}

		int* can_procs_be_senders;   //variable is used in process #0. 
									//it's array, if arr[i] = 1 then proc #i has appropriate row

		if (rank == 0)
		{
			can_procs_be_senders = allocate(int, PROCS_AMOUNT);
		}

		MPI_Gather(&is_there_appropriate_row, 1, MPI_INT,
					&can_procs_be_senders, 1, MPI_INT
					0, MPI_COMM_WORLD);


		int* array_with_checked_proc = allocate(int, PROCS_AMOUNT);  //array, all elements of which are 0s
								//except one element which is 1 - index of 1 is checked process
								//array will be used in MPI_Scatter function
								//it will be filled in proc #0
								//and then received by others procs


		if(rank == 0) 
		{
			int attempts_amount = 0;   //if attempts_amount is bigger that amount of procs - it means
				//that in every row number with current index is equal (or almost equal - look at ALMOST_ZERO ) to zero
				//so system can't be solved or it is linearly dependent

			int chosen_proc_id = -1;  //if there won't be found appropriate proc variable will be equal to -1
			
			for (; attempts_amount < PROCS_AMOUNT; ++attempts_amount) //loop for finding proc that has appropriate row
			{
				if (can_procs_be_senders[next_proc])
				{
					chosen_proc_id = next_proc++;
					break;
				}
			}

			if (chosen_proc_id == -1)
			{
				printf("Error. There is no appropriate row.\n");
				printf("System of equations can't be solved or it is linearly dependent\n");
				return 1;
			}

			// array_with_checked_proc = allocate(int, PROCS_AMOUNT);
			fill_array(array_with_checked_proc, PROCS_AMOUNT, 0);
			array_with_checked_proc[chosen_proc_id] = 1;
			
		}

		int proc_checked_to_send_row;    // checks if this proc is checked for sending row to others

		MPI_Bcast(array_with_checked_proc, 1, MPI_INT, 
					proc_checked_to_send_row, 1, MPI_INT,
					0, MPI_COMM_WORLD);

		int sender_proc_id

	}
	








	MPI_Finalize();
	return 0;
}

int is_almost_zero(double value)
{
	return fabs(value) < ALMOST_ZERO;
}

// int find_proc_with_appropriate_row();

void print_advanced_row(advanced_row a_row)
{
	printf("ADVANCED ROW:\n");
	printf("size: %d\n", a_row.size);
	printf("iters_passed: %d\n", a_row.iters_passed);
	printf("was_sent: %d\n", a_row.was_sent);
	for (int i = 0; i < a_row.size; ++i)
	{
		printf("%lf ", a_row.row[i]);
	}
	printf("\n");
}