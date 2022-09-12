gcc -fopenmp task2_add.c -o task2_add
OMP_NUM_THREADS=4 ./task2_add $1
