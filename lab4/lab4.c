#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE 8

#define MAX_FILENAME_SIZE 50
#define FILENAME_TEMPLATE "data/ps/p%d"

#define allocate(type, size) (type*)malloc(sizeof(type) * size)



int main (int argc, char* argv[])
{
	//initializing MPI
	MPI_Init (&argc, &argv);
	
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int PROCS_AMOUNT;
	MPI_Comm_size(MPI_COMM_WORLD, &PROCS_AMOUNT);

	int ROW_SIZE = MATRIX_SIZE + 1;

	int max_rows_in_file = (MATRIX_SIZE / PROCS_AMOUNT) + 1;

	//allocate memory for row pointers
	double** rows = allocate(double*, max_rows_in_file);
	for (int i = 0; i < max_rows_in_file; ++i)
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

	//print initialized rows
	// for (int i = 0; i < max_rows_in_file; ++i)
	// {
	// 	if(rows[i] == NULL)
	// 	{
	// 		break;
	// 	}
	// 	printf("rank: %d, row: %d, number_1: %.3lf, number_2: %.3lf, number_-2: %.3lf, number_-2 %.3lf\n", 
	// 		rank, i, rows[i][0], rows[i][1], rows[i][ROW_SIZE - 2], rows[i][ROW_SIZE - 1]);
	// }



	// if (rank == 0) {
	// 	MPI_
	// }








	MPI_Finalize();
	return 0;
}