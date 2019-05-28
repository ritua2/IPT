#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int numprocs,myid;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &myid);
  int i,j, anzahl, rest;
  MPI_Status stat;
  int ASpalten = 10;
  int AZeilen=30;
  int BSpalten=4;
  int TAG = 0;
  int*meta;
  double *MA;
  double *MB;
  meta = (int *)malloc(3 * sizeof(int));
  if(myid == 0)
  {
      meta[0] = ASpalten;
      meta[1] = AZeilen;
      meta[2] = BSpalten;

      for (i = 0; i < numprocs; i++) //masternode distributes matrix A to every single core
      {   
        MA = (double*)malloc(ASpalten*AZeilen*sizeof(double));
        for (int i = 0; i < ASpalten*AZeilen; i++)
          MA[i] = 1;
        MB = (double*)malloc(ASpalten*BSpalten*sizeof(double));
        for (int i = 0; i < ASpalten*BSpalten; i++)
          MA[i] = 2;
        MPI_Send(&meta[0], 3, MPI_INT, i, TAG, MPI_COMM_WORLD);
        printf("%d: debug04\n", myid);
        MPI_Send(&MA[0], ASpalten*AZeilen, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
        printf("%d: debug05\n", myid);
        MPI_Send(&MB[0], ASpalten*BSpalten, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
        printf("%d: debug06\n", myid);
      }
  }
  else
  {
     MPI_Recv(meta, 3, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
     printf("%d: debug01\n", myid);
     ASpalten = meta[0];
     AZeilen = meta[1];
     BSpalten=meta[2];
     printf("%d: debug02\n", myid);
     MA = (double*)malloc(ASpalten*AZeilen*sizeof(double));
     MB = (double*)malloc(ASpalten*BSpalten*sizeof(double));
     MPI_Recv(MA, ASpalten*AZeilen, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &stat);
     MPI_Recv(MB, ASpalten*BSpalten, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &stat);
     printf("%d: debug03\n", myid);
     // printf("%d: %f\n", myid, *(MA + _index(1, 1, ASpalten))); //funktioniert
  }
  MPI_Finalize();
  return 0;
}