#include <stdio.h>


int main(){
    int sum  = 0;
    int DATA_MAG = 100;
    int H[100];
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
	return 0;
}


