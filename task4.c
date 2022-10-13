#include "stdio.h"
#include "stdlib.h"
#include "time.h" 
#include "omp.h"
#define gap 10

int main ()
{
    int N = 5;
    int M = 5;
    int K = 10;
    int A[N][M];
    int B[M][K];
    int C[N][K];
    srand(time(NULL));
    int N_max = omp_get_max_threads();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            A[i][j] = rand() % gap; 
        }

        for (int k = 0; k < K; k++)
        {
            B[i][k] = rand() % gap;
            C[i][k] = 0;
        } 
    }

    #pragma omp parallel firstprivate(A, B) shared (C)
    {
        for (int p = 0; p < N_max; p++)  //this cycle combinate different packs of rows and columns of the matricies  
        {
            int pid = omp_get_thread_num();
            int startPoint_A = (int) (1.0 * pid * N / N_max);
            int endPoint_A = (int) (1.0 * (pid + 1) * N / N_max);
            int startPoint_B = (int) (1.0 * p * K / N_max);
            int endPoint_B = (int) (1.0 * (p + 1) * K / N_max);
            #pragma omp barrier
            for (int i = startPoint_A; i < endPoint_A; i++) //going through all the rows in a pack 
            {
                for (int j = startPoint_B; j < endPoint_B; j++) //going through all the columns in a pack 
                {
                    for (int k = 0; k < M; k++)
                    {
                        C[i][j] += A[i][k]*B[k][j]; //getting the (i,j) element of a new matrix
                    }
                }  
            }
        }
    }

    //output
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (j != M - 1)
            printf ("[%d] ", A[i][j]);
            else 
            printf ("[%d] \n", A[i][j]);
        }
        if (i == N - 1)
        printf ("\n");
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < K; j++)
        {
            if (j != K - 1)
            printf ("[%d] ", B[i][j]);
            else 
            printf ("[%d] \n", B[i][j]);
        }
        if (i == M - 1)
        printf ("\n");
    }


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
        {
            if (j != K - 1)
            printf ("[%d] ", C[i][j]);
            else 
            printf ("[%d] \n", C[i][j]);
        }
        if (i == N - 1)
        printf ("\n");
    }

    return 0;
}