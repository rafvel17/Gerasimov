#include "stdio.h"
#include "omp.h"

int main () {
    int N = 100000;
    int a[N];
    for (int i = 0; i < N; i++) {
        a[i] = i;
    }
    int b[N];

    #pragma omp shared(b) firstprivate(a) schedule(static) for 
    {
        for (int i = 0; i < N; i++) {
            if (i == 0) {
                b[i] = a[i] * a[i] * a[i+1] / 3.0;
            }
            else if (i == N-1) {
                b[i] = a[i-1] * a[i] * a[i] / 3.0;
            } 
            else {
                b[i] = a[i-1] * a[i] * a[i+1] / 3.0;
            }
        }
    }
return 0;
}