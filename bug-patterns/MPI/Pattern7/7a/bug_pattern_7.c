#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int arr [10];
  int range = 10/size;

  int start =  rank*range;
  int end = start + range;

  int recvbuf[range];

  for (int i = 0 ; i < range ; i++) {
    recvbuf[i] = i;
  }

  MPI_Gather(recvbuf,range, MPI_INT,arr, range, MPI_INT, 0, MPI_COMM_WORLD)
 
  MPI_Finalize();
  return 0;
}