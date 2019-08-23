#include <stdio.h>
#include <mpi.h> 

#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int arr [100];
  int range = 100/size;
  int start =rank*range; 
  int end = start + range; 
  int recvcounts [size];
  int displs [size];
  if (rank == size - 1) { 
    end += 100 % size; 
    range += 100 % size;
  }
  recvcounts[0]=100/size;
  displs[0]=0;
  for (int i=1;i < size;i++) {
    if(i<(size-1)){
    	recvcounts[i] = 100/size;
    }else if (i== (size-1)){
	     recvcounts[i]=100/size + (100%size);
    }
    displs[i]= displs[i-1]+ recvcounts[i-1];
    //printf("\ni: %d, rank: %d, displs[i]: %d, recvcounts[i]: %d\n",i, rank, displs[i], recvcounts[i]);
  }
  
  int recvbuf[range];
  for (int i=0;i < range;i++) {
    recvbuf[i] = i; 
  }
  MPI_Gatherv(recvbuf,range, MPI_INT,arr,recvcounts, displs, MPI_INT, 0,MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i =0; i < 100; i++) {
     printf(" arr[%d] = %d\n",i,arr[i]);
    }
  }
  
  MPI_Finalize();
  return 0;
}
