#include "stdio.h"
#include "omp.h"
#include "math.h"
#include "stdlib.h"
long double _factorial(int x);

int main(int argc, char *argv[]) {
  double x = 0; //calculating e^x
  printf("Enter x\n");
  scanf("%lf",&x); 
  int N = atoi(argv[1]);
  long double sum = 0;
  
  #pragma omp parallel firstprivate(N, x) reduction (+:sum) //we'll have the answer in sum
  {
      #pragma omp for schedule(dynamic) //dynamic because not all the steps of summarizing are equal

          for (int i = 0; i < N; i++)
          {
              sum += pow(x,(double)i) / _factorial(i);
          }

  }

  printf("%Lf\n", sum);
  return 0;
}

long double _factorial(int x) {
  if (x == 0) return 1;
  return x * _factorial(x - 1);
}
