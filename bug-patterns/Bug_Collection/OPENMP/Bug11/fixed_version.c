#include <stdio.h>


int main(){
    int sum  = 0;
    int DATA_MAG = 100;
    int H[100];
    int scale_factor = 10;
    #pragma omp parallel for reduction(+: sum)
    for (int i =0; i < DATA_MAG;i++) {
        H[i] = i;
    }
    int LUT[100];
    for (int i = 0; i < DATA_MAG; i++)
    {
        sum += H[i];

        LUT[i] = sum * scale_factor;
    }
    for (int i = 0; i < 100; i++) {
        printf("%d \n",LUT[i]);
    }
	return 0;
}


