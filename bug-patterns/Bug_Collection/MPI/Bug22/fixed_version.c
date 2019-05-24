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
  int rev [10];
  int range = 10/size;
  int start = rank*range; int end = start + range; int sendbuff[range];
  int counter = 0;
  for (int i=start;i<end;i++) {
    if (i < 5) {
      arr[i*2] =2 ; sendbuff[counter++]=arr[i*2];
    } 
    else {
      arr[(i-5)*2+1] =3; 
      sendbuff[counter++]=arr[i*2];
    }
  }
  MPI_Gather(sendbuff,range, MPI_INT,rev,range, MPI_INT,0, MPI_COMM_WORLD); 
  for (int i=0;i<10;i++)
  {
    if (i < 5)
    {
      arr[i*2]=rev[i] ;
    }
    else
  {  
    arr[(i-5)*2+1]=rev[i];
  }
  return 0;
}
