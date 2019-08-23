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
  int start =rank*range; 
  int end = start + range; 
  int recvcounts [size];
  int displs [size];
  if (rank == size - 1) { 
    end += 10 % size; 
    range += 10 % size;
  }
  for (int i=0;i < size;i++) {
    recvcounts[i] = range; 
    displs[i]= (i==0)? 0: recvcounts[i-1]; 
  }
  int recvbuf[range];
  for (int i=0;i < range;i++) {
    recvbuf[i] = i; 
  }
  MPI_Gatherv(recvbuf,range, MPI_INT,arr,displs,
  recvcounts, MPI_INT, 0,MPI_COMM_WORLD);
  
  if (rank == 0) {
    for (int i = 0 ; i < 10 ; i++) {
      printf("recvbuf [%d] is %d",i,recvbuf[i]);
    }
  }
  
  MPI_Finalize();
  return 0;
}
