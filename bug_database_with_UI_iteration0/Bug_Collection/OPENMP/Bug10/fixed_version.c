#include <stdio.h>


int main(){
    int sum =0;
    int temp[100];
    int DATA_MAG = 100;
    int H[100];
    int scale_factor = 10;
    int i;
    int LUT[100];

    #pragma omp parallel default (none) private(i) shared (temp, LUT, H, scale_factor, sum, DATA_MAG)
    {
    #pragma omp for
    for (i =0; i < DATA_MAG;i++) {
        H[i] = i;
        temp[i]=0;
    }


    #pragma omp single
    for (i = 0; i < DATA_MAG; i++)
    {
        sum += H[i];
        temp[i]=sum;
    }

    #pragma omp for
    for (i = 0; i < DATA_MAG; i++){
        LUT[i] = temp[i] * scale_factor;
    }

    }
    for (i = 0; i < 100; i++) {
        printf("%d  ",LUT[i]);
    }
        return 0;
}


