# parallel_computing_labs

To install MPI on Ubuntu:

```
https://jetcracker.wordpress.com/2012/03/01/how-to-install-mpi-in-ubuntu/
```

To compile mpi lab:
```
mpicc labmpi.c -o labmpi -std=c99 -lm
```
To run:
```
mpirun -np 6 ./labmpi
```

To compile OpenMP:
```
gcc openmp1.c -o ./opmlab -fopenmp -std=c99 -lm
```
