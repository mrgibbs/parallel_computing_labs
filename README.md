# parallel_computing_labs


To compile lab4

```
mpicc lab4.c -o lab4 -std=c99 -lm -fopenmp
```

To install MPI on Ubuntu:

```
https://jetcracker.wordpress.com/2012/03/01/how-to-install-mpi-in-ubuntu/
```

To compile mpi lab:
```
mpicc lab4.c -o lab4 -std=c99 -lm
```
To run:
```
mpirun -np 6 ./lab4
```

To compile OpenMP:
```
gcc openmp1.c -o ./opmlab -fopenmp -std=c99 -lm
```
