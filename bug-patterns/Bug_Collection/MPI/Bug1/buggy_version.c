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
  // calculating the number of iterations that each process will execute
  int range = 10/size;

  // calculating the start and end value of the for loop for each process
  int start =  rank*range;
  int end = start + range;

  // at rank 0, initializing the value for array "arr"
  if (rank == 0) {
    for (int i =0; i < 10; i++) {
      arr[i] = i;
    }
  }

  // Scatter portion of the array to other MPI processes
  int recvbuf[range];
  MPI_Scatter(arr,range, MPI_INT, recvbuf, range,MPI_INT,0, MPI_COMM_WORLD);

  // Increase the value of each element by 1
  for (int i = 0 ; i < range ; i++) {
    recvbuf[i] += 1;
  }
  // Gather the updated elements from all processes to get the final array
  MPI_Gather(recvbuf,range, MPI_INT,arr, range,MPI_INT,0, MPI_COMM_WORLD);  

  if (rank == 0) {
    for (int i = 0 ; i < 10 ; i++) {
      printf("recvbuf [%d] is %d",i,recvbuf[i]);
    }
  }

  MPI_Finalize();
  return 0;
}
