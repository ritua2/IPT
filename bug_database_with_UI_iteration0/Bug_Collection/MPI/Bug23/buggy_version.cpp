#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>
#define M 1000      //Number of times
#define N 2000      //Quantity of random numbers
#define R 1000      //Max value of random numbers

double SumaDeRaices (double*);

int main(int argc, char* argv[]) {
    int         yo;            /* rank of process      */
    int         p;             /* number of processes  */
    int         dest;          /* rank of receiver     */

    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank  */
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);

    /* Find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Request  reqs[p-1];
    MPI_Status   stats[p-1];   

    if (yo == 0) {
        int i,j;
        double result;
        clock_t inicio,fin;

        inicio = clock();

        for(i = 0; i<M; i++){ //M times
            for(j = 1; j<p; j++){ //for every slave
                MPI_Irecv(&result, sizeof(double), MPI_DOUBLE, j, i, MPI_COMM_WORLD, &reqs[j-1]);
            }
            MPI_Waitall(p-1,reqs,stats); //wait all slaves (SEG_FAULT)
        }
        fin = clock()-inicio;

        printf("Tiempo total de ejecucion %f segundos \n", ((double)fin)/CLOCKS_PER_SEC);   
    }
    else {
        double* numAleatorios = (double*) malloc( sizeof(double) * ((double) N) ); //array with numbers
        int i,j;
        double resultado; 
        dest=0;

        for(i=0; i<M; i++){ //again, M times
            for(j=0; j<N; j++){
                numAleatorios[j] = rand() % R ;
            }
            resultado = SumaDeRaices(numAleatorios);
            MPI_Isend(&resultado,sizeof(double), MPI_DOUBLE, dest, i, MPI_COMM_WORLD,&reqs[p-1]); //send result to master
        }
    }

    /* Shut down MPI */
    MPI_Finalize();

    exit(0);
} /* main */



double SumaDeRaices (double* valores){
    int i;
    double sumaTotal = 0.0;

    //Raices cuadradas de los valores y suma de estos   
    for(i=0; i<N; i++){
        sumaTotal = sqrt(valores[i]) + sumaTotal;
    }

    return sumaTotal;
}
