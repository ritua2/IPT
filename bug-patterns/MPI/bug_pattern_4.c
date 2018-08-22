#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int *arr;
  if (rank  == 0 ) {
    arr  = (int*) malloc (10*sizeof(int));
    for (int i = 0; i < 10; i++) {
      arr[i] = i;
    }
  }
  int range = 10/size;
  int rev [range];
  MPI_Scatter(arr, range, MPI_INT,
               rev, range, MPI_INT, 0,
               MPI_COMM_WORLD);
  for (int i = 0; i < range; i++) {
    rev[i] += i*rank;
  }
  MPI_Gather(rev, range, MPI_INT,
               arr, range, MPI_INT,
               0, MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}
