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

  
  // calculating the number of iterations that each process will execute
  int range = 100/size;
  int sendbuf[range];
  int counter =0;
  // calculating the start and end value of the for loop for each process
  int p_start_value = rank*range;
  int p_end_value = p_start_value + range;

  for (int i = p_start_value ; i < p_end_value; i++) {

    // compute all elements at even indexes first
    if (i<50) {
      arr[i*2] =2 ;
      sendbuf[counter++]=arr[i*2];   
    }

    // compute all elements at odd indexes second
    else {
      arr[(i-50)*2+1] =3;
      sendbuf[counter++]=arr[(i-50)*2+1];
    }
  }

  // Gathering the elements from each process to get the final array
  int rev [100];
  MPI_Gather(sendbuf,range, MPI_INT,rev, range,MPI_INT,0, MPI_COMM_WORLD);

  for (int i=0;i<100;i++){
    if (i < 50)
    {
        arr[i*2]=rev[i] ;
    }else{  
      arr[(i-50)*2+1]=rev[i];
    }
  }

  if (rank == 0){
    for (int i =0; i < 100; i++)
      printf("arr[%d] = %d\n",i,arr[i]);
  
  }
  MPI_Finalize();
  return 0;
}