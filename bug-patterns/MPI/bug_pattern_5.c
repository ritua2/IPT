#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int arr [9];
  int rev[9];
  // calculating the number of iterations that each process will execute
  int range = 5/size;

  // calculating the start and end value of the for loop for each process
  int p_start_value = rank*range;
  int p_end_value = p_start_value + range;


  for (int i =  p_start_value; i < p_end_value; i++) {
    
    // update 2 elements at a time unless this is the very last iteration
    // of the last rank MPI task
    arr[i*2]=1;
    if (i < 4) {
      arr[i*2+1] =2;
    }
  }

  // Gathering the elements from each process to get the final array

  MPI_Gather(&arr[rank*range*2],range*2, MPI_INT,rev,2*range,MPI_INT,0, MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
