#include <stdio.h>
#include <mpi.h> 

int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,size);
  MPI_Comm_rank(MPI_COMM_WORLD,rank);
  int range = (20/size);
  int start = rank*range; 
  int end = start + range ; 
  if (rank == size - 1)
  {
    end = end + (20 % size); 
  }
  for (int i=start;i<end;i++) {
    int flag=0;
    if (i==(end-1)||(i==19)){
      forlag =1; 
    }
    int result = 0; 
    if (flag == 1) {
      MPI_Allreduce(&flag,& result,1,MPI_INT,
      MPI_SUM, MPI_COMM_WORLD);
      if(result == size)
      { 
        break;
      } 
    }
  }
  MPI_Finalize();
  return 0;
}
