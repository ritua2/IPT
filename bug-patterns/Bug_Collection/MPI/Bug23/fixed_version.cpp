#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <time.h>
#define M 1000      //Number of times
#define N 2000      //Quantity of random numbers
#define R 1000      //Max value of random numbers

double SumaDeRaices (double* valores)
{

  int i;
  double sumaTotal = 0.0;

  //Raices cuadradas de los valores y suma de estos
  for(i=0; i<N; i++) {
    sumaTotal = sqrt(valores[i]) + sumaTotal;
  }

  return sumaTotal;
}


int main(int argc, char* argv[]) {
  int         yo;            /* rank of process      */
  int         p;             /* number of processes  */

  /* Start up MPI */
  MPI_Init(&argc, &argv);

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  double *result;
  clock_t inicio, fin;
  double *numAleatorios;
  if (yo == 0) {
    inicio = clock();
  }

  numAleatorios = (double*) malloc( sizeof(double) * ((double) N) ); //array with numbers
  result = (double *) malloc(sizeof(double) * p);

  for(int i = 0; i<M; i++){ //M times
    for(int j=0; j<N; j++) {
      numAleatorios[j] = rand() % R ;
    }
    double local_result = SumaDeRaices(numAleatorios);
    MPI_Gather(&local_result, 1, MPI_DOUBLE, result, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); //send result to master
  }

  if (yo == 0) {
    fin = clock()-inicio;

    printf("Tiempo total de ejecucion %f segundos \n", ((double)fin)/CLOCKS_PER_SEC);
  }

  free(numAleatorios);

  /* Shut down MPI */
  MPI_Finalize();
} /* main */