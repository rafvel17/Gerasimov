#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "omp.h"

void QuickSort(int *array, int r);
int SpecialCasesCheck(int *array, int N);
int NumberCount(FILE* input); //for to get a number of elements placed in the file 
void ReadArray(FILE* input, int size, int* array); //for to read an array from the file

int main(int argc, char **argv)
{
    char mode[9]; //getting an array mode
    int N = 0;
    printf("Choose the entering mode: FILE or KEYBOARD\n");
    scanf("%s", mode);
    if(mode[0] == 'K') 
    {
        printf("Enter the size\n");
        scanf("%d", &N);
        int *array = (int*)calloc(N, sizeof(int));
        printf("Enter %d elements\n", N);
        for (int i = 0; i < N; i++)
            scanf("%d", &array[i]);
        int case_num = SpecialCasesCheck(array, N); //accounting for special degenerate cases
        switch(case_num)
        {
            case 1: printf("It is only one element: %d\n", array[0]); return 0;
            case 2: printf("It is sorted\n"); goto m;
            case 3: printf("It is reversly sorted\n"); int t = 0;
                    for(int i = 0; i < N/2; i++)
                    {   
                        t = array[i];
                        array[i] = array[N - 1 - i];
                        array[N - 1 - i] = t;
                    } goto m;
            case 4: printf("All elements are equal\n"); goto m;   
        }
        QuickSort(array, N);
        m:for(int i = 0; i < N; i++)
        {
            if (i == N - 1)
                printf("%d\n", array[i]);
            else
                printf("%d ", array[i]);
        }
        free(array);
    }

    else
    {
        FILE *fd;
        fd = fopen("input.txt", "r");
        N = NumberCount(fd);
        int* array = (int*)calloc(N, sizeof(int));
        ReadArray(fd, N, array);
        fclose(fd);
        int case_num = SpecialCasesCheck(array, N);
        switch(case_num)
        {
            case 1: printf("It is only one element: %d\n", array[0]); return 0;
            case 2: printf("It is sorted\n"); goto p;
            case 3: printf("It is reversly sorted\n"); int t = 0;
                    for(int i = 0; i < N/2; i++)
                    {   
                        t = array[i];
                        array[i] = array[N - 1 - i];
                        array[N - 1 - i] = t;
                    } goto p;
            case 4: printf("All elements are equal\n"); goto p;   
        }
        QuickSort(array, N);
        p:for(int i = 0; i < N; i++)
        {
            if (i == N - 1)
                printf("%d\n", array[i]);
            else
                printf("%d ", array[i]);
        }
        free(array);
    }

    return 0;
}


void QuickSort(int *array, int r)
{
    #pragma omp parallel shared(array)
    {
        #pragma omp single //one thread will assign independent tasks to other free threads; especially effecient in case of recursion
        {
            int tmp = 0;
            int pivot = array[r/2]; 
            int i = 0, j = r - 1;
            do 
            {
                while(array[i] < pivot)
                i++; 
                while(array[j] > pivot)
                j--; 
                if (i <= j)
                {
                    tmp = array[i];
                    array[i] = array[j];
                    array[j] = tmp;
                    j--;
                    i++;
                }}while(i <= j); //splitting an array with a "wall" of a pivot element value
        

                #pragma omp task shared(array) //working with the retrieved parts the same way
                {
                    if (j > 0)
                    QuickSort(array, j + 1);
                }
                #pragma omp task shared(array)
                {
                    if(i < r)
                    QuickSort(array+i, r - i);
                }
                #pragma omp taskwait
        }
    }
}

int SpecialCasesCheck(int *array, int N)
{
    int check_forw = 0;
    int check_rev = 0;
    int check_equ = 0;

    if (N == 1) 
        return 1; //just one element


    for (int i = 0; i < N - 1; i++)
    {
        if (array[i] > array[i+1])
            {check_forw = 1; break;}
    }
    if (check_forw == 0)
        return 2; //already sorted


    for (int i = 0; i < N - 1; i++)
    {
        if (array[i] < array[i+1])
            {check_rev = 1; break;}
    }
    if (check_rev == 0)
        return 3; //reversely sorted


    for (int i = 0; i < N - 1; i++)
    {
        if (array[i] != array[i+1])
            {check_equ = 1; break;}
    }
    if (check_equ == 0)
        return 4; //all elements are equal

    return 0;
}


int NumberCount(FILE* input) 
{
    fseek(input, 0, SEEK_SET);
    int counter = 0;
    while (1) {
        int value;
        if (fscanf(input, "%d", &value) == 1)
        counter++;
        if (feof(input))
        break;
    }
    return counter;
}

void ReadArray(FILE* input, int size, int* array) 
{
    fseek(input, 0, SEEK_SET);
    for (int i = 0; i < size; i++) 
    {
        fscanf(input, "%d", &array[i]);
    }
}