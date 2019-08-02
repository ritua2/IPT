#include<math.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    int sum  = 0;
    int DATA_MAG = 100;
    int H[100];
    int LUT[100];
    int scale_factor = 10;

    // Gets the number of OpenMP threads used
    char *str_omp_threads = getenv("OMP_NUM_THREADS");
    int omp_threads = atoi(str_omp_threads);

    #pragma omp parallel for
    for (int i =0; i < DATA_MAG;i++) {
        H[i] = i;
    }

    // Creates an array equal to the number of threads
    int *thread_base_sum = (int*) malloc(omp_threads*sizeof(sum));

    for (int j=0; j < omp_threads; j++) {
        // Gauss' addition formula
        int final_N = j*(DATA_MAG/omp_threads);
        // Removes the number itself
        thread_base_sum[j] = final_N*(final_N + 1)/2 - final_N;
    }


    #pragma omp parallel for
    // Divides the work to each thread
    for (int j=0; j < omp_threads; j++) {

        int sum = thread_base_sum[j];

        // Last thread also adds the last elements
        if (j == (omp_threads-1)) {

            for (int k=j*(DATA_MAG/omp_threads); k < DATA_MAG; k++) {
                //printf("%d\n", k);
                sum += H[k];
                LUT[k] = sum*scale_factor;
            }

        } else {
            for (int k=j*(DATA_MAG/omp_threads); k < (j+1)*(DATA_MAG/omp_threads); k++) {
                //printf("%d\n", k);
                sum += H[k];
                LUT[k] = sum*scale_factor;
            }
        }
    }


    for (int i = 0; i < 100; i++) {
        printf("%d\n",LUT[i]);
    }


    free(thread_base_sum);
    return 0;
}


