#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATRIX_SIZE 8

#define ALMOST_ZERO 0.0000001

#define MAX_FILENAME_SIZE 50
#define FILENAME_TEMPLATE "data/ps/p%d"

#define allocate(type, size) (type*)malloc(sizeof(type) * size)

typedef struct advanced_row
{
	int was_sent;
	int iters_passed;
	int size;
	double* row;
} advanced_row;


void print_advanced_row(advanced_row a_row);


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

	if(rank == 0) {
		printf("all processes were initialized");
	}



	// if (rank == 0) {
	// 	MPI_
	// }








	MPI_Finalize();
	return 0;
}

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