#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,size);
  MPI_Comm_rank(MPI_COMM_WORLD,rank);
  // calculating the number of iterations that each process will execute
  int range = (20/size);

  // calculating the start and end value of the for loop for each process
  int start =  rank*range;
  int end = start + range ;

  // adding extra iterations to the last process
  if (rank == size - 1) {
    end = end + (20 % size);
  }

  // looking to see if i ever equals 19
  for (int i =start; i < end; i++) {
    int flag = 0;

    // if i equals 19, set flag to 1 and broadcasting 
    // the result to other MPI processes
    if (i  == 19) {
      flag =1;
    }
    int result = 0;
    // the allreduce calls are used to broadcasting the flag
    MPI_Allreduce(&flag,&result,1,MPI_INT,MPI_MAX,MPI_COMM_WORLD);
    
    // break the loop if the broadcasting is set
    if (result == 1) {
      break;
    }
  }
  MPI_Finalize();
  return 0;
}
