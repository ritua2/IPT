#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define IN 16   //input size

int main(int argc, char** argv){
   // Initialize the MPI environment
   MPI_Init(&argc, &argv);
   MPI_Win win;
   // Find out rank, size
   int id;  //process id
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   int p;   //number of processes
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   srand(time(0));
   int mapper[IN];
   int toMap[IN];
   int result[IN];
   if(id==0){
       for(int n=0; n<IN; n++){   //predecided map values
           toMap[n] = (rand()%IN);
           mapper[n] = (rand()%IN) ;
       }
   }
   if(id == 0){
       for(int n=0; n<IN; n++){   //map results
           printf("[%d -> %d]\n", toMap[n], mapper[n]);
       }
   }
   MPI_Bcast(mapper,IN,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Bcast(toMap,IN,MPI_INT,0,MPI_COMM_WORLD);

   int i = 0;
   while(i<IN){
        result[i] = mapper[toMap[i]];
        i++;
   }
   MPI_Barrier(MPI_COMM_WORLD);
   if(id == 1){
       for(int n=0; n<IN; n++){   //map results
           printf("[%d -> %d]\n", toMap[n], result[n]);
       }
   }
   MPI_Finalize();
   return 0;
}
