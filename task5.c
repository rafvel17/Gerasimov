#include "stdio.h"
#include "omp.h"
#include "math.h"
#include "stdlib.h"
long double _factorial(int x);

int main(int argc, char *argv[]) {
  double x = 0; //calculating e^x and sin(x)
  printf("Enter x\n");
  scanf("%lf",&x); 
  int N = atoi(argv[1]);
  long double sum_exp = 0;
  long double sum_sin = 0;
  
  #pragma omp parallel firstprivate(N, x) 
  {
    #pragma omp for schedule(dynamic) reduction(+:sum_exp) //dynamic because not all the steps of summarizing are equal

        for (int i = 0; i < N; i++)
        {
            sum_exp += pow(x,(double)i) / _factorial(i);
        }
    #pragma omp for schedule(dynamic) reduction(+:sum_sin) 

        for (int i = 0; i < N; i++)
        {
            sum_sin += pow(-1.0, (double)((i % 2)))* pow(x,(2*i+1)) / _factorial(2*i+1);
        }
  }

  printf("%Lf\n%Lf\n", sum_exp, sum_sin);
  return 0;
}

long double _factorial(int x) {
  if (x == 0) return 1;
  return x * _factorial(x - 1);
}

