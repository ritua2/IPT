#include<stdio.h>

int main(){
    int i;
    int j = 10;
    #pragma omp for firstprivate(j)
    for (i = 0; i < 10; i++)
        printf("%d\n", j);
}

