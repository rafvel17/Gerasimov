#include "omp.h"
#include "stdio.h"

int main (int argc, char** argv) {
    int N = omp_get_max_threads(); //get the number of all processes 
    int i = N-1; //will be used as a number to compare with processes ids  
    #pragma omp parallel shared(i)
    { 
             while(i != omp_get_thread_num()) {} //searching for the proc id equal to the current value of i, putting aside others
                 printf ("Hello, world! %d\n", omp_get_thread_num());
             i--; //decrease of i value to move to the followong process 
    }
return 0;
}