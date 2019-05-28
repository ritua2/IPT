#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h> 

int **alloc2d(int n, int m) {
      int i;
      int **array = malloc(n * sizeof(int*));
      array[0] = malloc(n * m * sizeof(int));
      for(i = 1; i < n; i++) 
          array[i] = array[i-1] + m;
      return array;
}

int *genSendc(int dim, int numprocs) {
    int* sendc = (int*)malloc(sizeof(int)*numprocs); 
    int i;
    int subsize = dim/numprocs;
    for(i=0; i<numprocs; ++i)
        sendc[i] = subsize;
    for(i=0; i<dim-subsize*numprocs; ++i)
        sendc[i]+=1;
    return sendc;
}

int *genDispl(int numprocs, int*sendc) {
    int* displ = (int*)malloc(sizeof(int)*numprocs);
    int i;
    displ[0]=0;
    for(i=1; i<numprocs; ++i)
        displ[i] = displ[i-1]+sendc[i-1];
    return displ;
}

int main(int argc, char *argv[]){
    int numprocs, rank, i, j, N=5, M=4;
    int* displMat, *sendcMat;
    int **txMatrix, **rxMatrix;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    sendcMat = genSendc(N, numprocs);
    for(i=0; i<numprocs; ++i)
        sendcMat[i] *= M;
    displMat = genDispl(numprocs, sendcMat);

    rxMatrix = alloc2d(sendcMat[rank]/M, M);
    if (rank == 0) {
        srand(time(NULL));
        txMatrix = alloc2d(N, M);
        for (i=0; i < N; ++i)
            for(j=0; j < M; ++j)
               txMatrix [i][j] = (rand() % 10)+1;
    }
    if (rank == 0) {
      MPI_Scatterv(&txMatrix[0][0], sendcMat, displMat, MPI_INT, &rxMatrix[0][0], sendcMat[rank], MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
      MPI_Scatterv(NULL, sendcMat, displMat, MPI_INT, &rxMatrix[0][0], sendcMat[rank], MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
