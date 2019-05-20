#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  // calculating the number of iterations that each process will execute
  int range = 100/size;

  // calculating the start and end value of the for loop for each process
  int p_start_value = rank*range;
  int p_end_value = p_start_value + range;

  int sum = (rank == 0) ? 1: 0;

  // increase sum in parallel in total of 10 times
  for (int i = p_start_value; i <  p_end_value; i++) {
    sum  +=1;
  }

  // Reduce (compute total sum) from the partly "sum" values of every processes
  int receive_placeholder = 0;
  MPI_Allreduce(&sum,&receive_placeholder,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

// Reassign total sum value to "sum" variable 
  sum = receive_placeholder;

  MPI_Finalize();
  return 0;
}
