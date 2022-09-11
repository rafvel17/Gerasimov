#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

int main (int argc, char** argv) {
    int N = (int)atoi(argv[1]);
    int sum = 0;
    #pragma omp parallel reduction(+: sum) //sum will have a value of the total sum when the parallel section ends working 
    {
        sum = 0;
        #pragma omp for schedule(static) 
            for (int i = 1; i <= N; i++) {
            sum+=i;
        }
    }
    printf("%d\n", sum);
    return 0;
}
