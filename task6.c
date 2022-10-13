#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "omp.h"

int main (int argc, char *argv[])
{
    int N = atoi(argv[1]);
    int isPrime = 1; //indicator of a prime number
    int counter = 0; 
    #pragma omp parallel firstprivate(isPrime) reduction(+:counter)
    {
        #pragma omp for schedule(dynamic) 
        for (int i = 5; i <= N; i+=2)  //here we exclude even numbers
        {
                for (int j = 3; j <= sqrt(i); j++)  //searching for dividers of i after sqrt(i) is useless because of the symmetry: we always need two numbers < sqrt(i) and > sqrt(i) as multipliers 
                {
                    if ((i % j) == 0)
                    isPrime = 0; 
                }
            counter+=isPrime;
            isPrime = 1; 
        }
    }
    printf("%d\n", counter+2); //counting 2 and 3
    return 0;
}
