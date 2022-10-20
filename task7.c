#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "omp.h"

double CalcAverage(double **area, int i, int j, unsigned int N, unsigned int M, int (*fu) (int, unsigned int), int (*flo) (int), int (*fle) (int), int (*fr) (int, unsigned int));
int CheckAccuracy(double **area_1, double ** area_2, double eps, unsigned int N, unsigned int M);
void DepicIter(double **area, unsigned int N, unsigned int M, FILE *fd);
int IsUpperEdge(int i, unsigned int M);
int IsLowerEdge(int i);
int IsLeftEdge(int j);
int IsRightEdge(int j, unsigned int N);

int main (int argc, char *argv[])
{
    struct timespec t_start, t_end;
    clock_gettime (CLOCK_REALTIME, &t_start);
    FILE *fd;
    fd = fopen("results.txt", "w");
    double eps = 0.001;
    unsigned int N = 0; // horizontal
    unsigned int M = 0; // vertical
    N = (int)atoi(argv[1]);
    M = (int)atoi(argv[2]);

    double **area = (double**)calloc(M, sizeof(double*)); //contains data of a current iteration
    for (int i = 0; i < M; i++) 
    area[i] = calloc(N, sizeof(double));

    double **area_prev = (double**)calloc(M, sizeof(double*)); //contains data of a previous iteration
    for (int i = 0; i < M; i++) 
    area_prev[i] = calloc(N, sizeof(double));
    //initial conditions
    for (int i = 0; i < N; i++)
    area[0][i] = 100;

    for (int i = 0; i < M; i++)
    area[i][0] = 100;

    for (int i = 0; i < M; i++)
    area[i][N-1] = 100;

    

    //the process starts
    do 
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            area_prev[i][j] = area[i][j];
        }
        
        #pragma omp parallel firstprivate(N, M, eps, area_prev) shared(area)
        {
            #pragma omp for
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                area[i][j] = CalcAverage(area_prev, i, j, N, M, IsUpperEdge, IsLowerEdge, IsLeftEdge, IsRightEdge); //putting the average of the neighbours into the element
            }
        }

        DepicIter(area, N, M, fd); //depicting the current situation
    
    } while (!CheckAccuracy(area, area_prev, eps, N, M)); //checking if current and previous iterations differ from each other no more than by eps

    clock_gettime (CLOCK_REALTIME, &t_end);

    fclose(fd);
    printf("Execution time = %lf\n", (t_end.tv_nsec - t_start.tv_nsec) / 1000000000.0);
    
    return 0;
}

//Average
double CalcAverage(double **area, int i, int j, unsigned int N, unsigned int M, int (*fu) (int, unsigned int), int (*flo) (int), int (*fle) (int), int (*fr) (int, unsigned int))
{
    int isUp = fu(i, M);
    int isLow = flo(i);
    int isLeft = fle(j);
    int isRight = fr(j, N);

    if (isUp)
    {
        if (isRight)
            return (area[i-1][j] + area[i][j-1]) / 4.0;
        else if (isLeft)
            return (area[i-1][j] + area[i][j+1]) / 4.0;
        else
            return (area[i-1][j] + area[i][j-1] + area[i][j+1]) / 4.0; 
    }

    else if (isLow)
    {
         if (isRight)
            return (area[i+1][j] + area[i][j-1]) / 4.0;
        else if (isLeft)
            return (area[i+1][j] + area[i][j+1]) / 4.0;
        else 
            return (area[i+1][j] + area[i][j-1] + area[i][j+1]) / 4.0; 
    }

    else if (isLeft)
    {
        return (area[i+1][j] + area[i-1][j] + area[i][j+1]) / 4.0;
    }

    else if (isRight)
    {
        return (area[i+1][j] + area[i-1][j] + area[i][j-1]) / 4.0;
    }

    else 
        return (area[i+1][j] + area[i-1][j] + area[i][j-1] + area[i][j+1]) / 4.0;
}
//Accuracy
int CheckAccuracy(double **area_1, double **area_2, double eps, unsigned int N, unsigned int M)
{
    int check = 1;
    #pragma omp parallel shared(check)
    {
        #pragma omp for 
            for (int i = 0; i < M; i++) 
            {
                for (int j = 0; j < N; j++)
                {
                    if (fabs(area_1[i][j] - area_2[i][j]) > eps) {check = 0;}
                }
            }
    }
    return check;
}
//Depiction
void DepicIter(double **area, unsigned int N, unsigned int M, FILE *fd)
{
    for (int i = M - 1; i >= 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            if ( j != N - 1)
                fprintf(fd, "%f ", area[i][j]);
            else
                fprintf(fd, "%f\n", area[i][j]);
        }
    }
    fprintf(fd, "\n\n");
}

//Positions
int IsUpperEdge(int i, unsigned int M)
{
    if (i == M-1)
        return 1;
    else 
        return 0;
}

int IsLowerEdge(int i)
{
    if (i == 0)
        return 1;
    else 
        return 0;
}


int IsLeftEdge(int j)
{
    if (j == 0)
        return 1;
    else 
        return 0;
}


int IsRightEdge(int j, unsigned int N)
{
    if (j == N-1)
        return 1;
    else 
        return 0;
}