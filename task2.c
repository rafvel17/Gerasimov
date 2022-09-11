#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main (int argc, char** argv) {
    int N = 100;
    int sum = 0;
    int num = omp_get_max_threads();
    int* x = (int*)malloc(num*sizeof(int)); //here we will store local sums counted by threads
    #pragma omp parallel shared (x) firstprivate(sum)
    {
    #pragma omp for  
    for (int i = 1; i <= N; i++) {
        sum+=i;                             //building local sums
    }
    x[omp_get_thread_num()] = sum;          //writing local sums into the array
    }
    sum = 0;
    for (int i = 0; i < num; i++) {
        sum += x[i];
    }
    printf("%d\n", sum);
    free (x);
    return 0;
}

