#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int arr [12];

  // initializing array "arr" at rank 0
  if (rank == 0) {
    for (int i =0; i < 12; i++) {
      arr[i] = i;
    }
  }

  // calculating the number of iterations that each process will execute
  int range = (6/size);

  // calculating the start and end value of the for loop for each process
  int start =  rank*range;
  int end = start + range ;

  // adding extra iterations to the last process
  if (rank == size - 1) {
    end = end + (6 % size);
  }

  // calculating the number of elements each process is getting
  // as well their displacements in regard to the original array
  int displacements[size], count[size];
  for (int i = 0; i < size ; i++) {

    // The formula for "displacement" and "count" is similar to 
    // computing start and end (i.e each process has the same 
    // number of elements, only the last process may have extra ones)
    displacements[i] = i*(12/size);
    if (i < size - 1) {
      count[i] = 12/size;
    }
    else {
      count[i] = 12 - i*(12/size);
    }
  }

  // Scatter portion of the array to other MPI processes
  int recvbuf[count[rank]];
  MPI_Scatterv(arr,count,displacements, MPI_INT, recvbuf,count[rank],MPI_INT,0, MPI_COMM_WORLD);

  // increase the elements by one ( 2 at a time)
  for (int i = start ; i < range ; i++) {
    recvbuf[i] += 1;
    recvbuf[i+1] += 1;
  }

  // Gathering the elements from each process to get the final array 
  MPI_Gatherv(recvbuf,range, MPI_INT,arr, count,displacements,MPI_INT,0, MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
