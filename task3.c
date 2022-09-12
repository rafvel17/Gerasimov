#include "stdio.h"
#include "omp.h"

int main () {
    int N = 10;
    int a[N];
    for (int i = 0; i < N; i++) {
        a[i] = i;
    }
    int N_max = omp_get_max_threads();
    int edge = 0;
    int wind[3];
    #pragma omp parallel shared(a) firstprivate(N_max,edge,wind) 
    {
        int pid = omp_get_thread_num();
        int StartPoint = (int)(1.0 * pid / N_max * N); 
        int EndPoint = (int)(1.0 * (pid+1) / N_max * N);
        if (pid == N_max - 1) {
            EndPoint = N;
        }
            if (StartPoint == 0) {
            wind[0] = a[StartPoint];
            wind[1] = a[StartPoint];
            wind[2] = a[StartPoint+1];
            edge = a[EndPoint];
            }
            else if (EndPoint != N) {
                wind[0] = a[StartPoint-1];
                wind[1] = a[StartPoint];
                wind[2] = a[StartPoint+1];
                edge = a[EndPoint];
            }
             else if (EndPoint == N) {
                wind[0] = a[StartPoint-1];
                wind[1] = a[StartPoint];
                wind[2] = a[StartPoint+1];
                edge = a[EndPoint-1];
            }
        #pragma omp barrier
        for (int i = StartPoint; i < EndPoint; i++) {
            if (i == N-1) {
                a[i] = (int)(a[i]*a[i]*wind[0] / 3.0);
            }
            else{
            a[i] = wind[0] * wind[1] * wind[2] / 3;
            wind[0] = wind[1];
            wind[1] = wind[2];
            if (i == EndPoint - 2) {
                wind[2] = edge;
            }
            else {
            wind[2] = a[i+2];
            }
            }
        }
    }
     for (int i = 0; i < N; i++) {
         printf("%d\n", a[i]);
     }
    
    return 0;
}
   