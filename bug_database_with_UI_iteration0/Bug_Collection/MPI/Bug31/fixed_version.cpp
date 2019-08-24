#include <stdio.h>
#include <mpi.h> 
#include <math.h>
int main()
{
  int size;
  int rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  int arr [99] = {8};
  int rev[99] = {9};
  int start, end;

  //int range = 50/size;
  int range;

  if( ((99/size)%2)!=0){
      range = ((99/size)-1)/2;
   }else{
      range = ((99/size))/2;
   }
  int recvcounts [size];
  int displs [size]; 
  start =rank*range;
  if (rank == (size-1)) {
    range = range + (99-(range*size*2))/2 +1;
  }
  end = start + range;
  //printf("\nrank:%d, start:%d, end:%d, range:%d\n", rank, start, end, range);
  for (int i=0;i < size;i++) {
    if (i== 0){
         displs[0]=0;
    }else{
         displs[i]= displs[i-1]+ recvcounts[i-1]; 
    }
    if( ((99/size)%2)!=0){
      recvcounts[i] = ((99/size)-1);
      if(i==size-1){
        recvcounts[i] = recvcounts[i] + (99-((99/size)-1)*size);
      }
    }else{
      recvcounts[i] = ((99/size));	
      if(i==size-1){
        recvcounts[i] = recvcounts[i] + (99-ceil((99/size))*size);
       }
    }
  }
  
  int sendbuff[recvcounts[rank]];
  for (int i = 0; i < recvcounts[rank] ; i++){
     sendbuff[i]=7;
  }
  int counter = 0;
  //printf("\nstart:%d, end: %d\n", start, end);
  for (int i = start; i < end; i++)
  {
    arr[i*2]=1;
    sendbuff[counter]=1;
    counter++; 
    if (i < 50) {
       arr[i*2+1] =2;
       sendbuff[counter]=2;
       counter++;
    }
  }

  int sendcount=recvcounts[rank];
  //printf("\nrank: %d, sendcount:%d, displs[rank]:%d\n", rank,sendcount, displs[rank]);
  //MPI_Gatherv(&arr[rank*range *2],sendcount,MPI_INT,rev, recvcounts,displs,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Gatherv(sendbuff,sendcount,MPI_INT,rev, recvcounts,displs,MPI_INT,0,MPI_COMM_WORLD);
  if (rank == 0){
    
    for (int i =0; i < 99; i++){
      printf("\na[%d] = %d\n",i,rev[i]);
    }
  }
  MPI_Finalize();
  return 0;
}
