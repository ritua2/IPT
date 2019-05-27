#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){

    int nameLen;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    int myrank;           // Rank of processor
    int numprocs;         // Number of processes
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processorName,&nameLen);
    MPI_Status status;

    time_t t;
    srand((unsigned)time(NULL)+myrank*numprocs+nameLen);

    long c = rand()%100;

    printf("Processor %d has %li particles\n", myrank, c);

    long oldcount[numprocs];

    // Every processor gets the random number of the other processors
    MPI_Allgather(&c, 1, MPI_LONG, &oldcount, 1, MPI_LONG, MPI_COMM_WORLD);

    for(int i=0; i<numprocs; i++){
         printf("Processor %d: %d entry of list is %li\n", myrank, i, oldcount[i]);
    }

    MPI_Finalize();
    return 0;
}