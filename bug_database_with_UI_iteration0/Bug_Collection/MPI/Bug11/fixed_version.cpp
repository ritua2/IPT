#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
//#define ARRAYSIZE   16000000
#define ARRAYSIZE 64

int main (int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int myrank;
    char name[100];
    int result;
    int size = 0;
    int number;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Request request;
    int buffer;
    int count;
    double t1, t2;
    MPI_Comm_size (MPI_COMM_WORLD,&size);
    int cells = ARRAYSIZE/size;
    float array[cells];
    int i;
    float resultado;
    float   data[ARRAYSIZE];    /* the intial array */

    if (myrank == 0) {
        t1 = MPI_Wtime();
        //t2 = MPI_Wtime();
        MPI_Get_processor_name(name, &result);

        // ********************** INICIALIZANDO ARRAY **********************
        int     i;          /* loop variable */
        //float   data[ARRAYSIZE];    /* the intial array */

        printf("Starting serial array example...\n");
        printf("Using array of %d floats. Requires %ld bytes\n",ARRAYSIZE,sizeof(data));

        /* Initialize the array */
        printf("Initializing array...\n");
        for(i=0; i<ARRAYSIZE; i++)
          data[i] =  i * 1.0;

        /* Print a few sample results */
        printf("Sample results\n");
        printf("   data[1]=%e\n",  data[1]);
        printf("   data[32]=%e\n",  data[32]);
        //printf("   data[1000]=%e\n",  data[1000]);
        //printf("   data[10000]=%e\n",  data[10000]);
        //printf("   data[100000]=%e\n",  data[100000]);
        //printf("   data[1000000]=%e\n",  data[1000000]);
        printf("\nAll Done!\n");
        // ********************** ARRAY INICIALIZADO **********************

        MPI_Comm_size (MPI_COMM_WORLD,&size);
        printf("Total of tasks: %d\n", size);
        printf("Each task process %d cells\n", ARRAYSIZE/size);
         //cells was redefined below and hence, commenting it out
        //int cells = ARRAYSIZE/size;
        int id_task;
        for(id_task = 0; id_task < size; id_task++) {
            //float array[cells];
            int i=0;
            for(i=0; i<cells; i++)
                array[i] =  i * (id_task+1.0);
            if(id_task!=0){ 
                MPI_Send(&array, cells, MPI_FLOAT, id_task, 0, MPI_COMM_WORLD);
            }
    }

        printf("master: %d at processor: %s\n",myrank, name);

    }
    if(myrank!=0){
        MPI_Recv(array, cells, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for(i=0; i<cells; i++)
        array[i] =  i * (myrank+1.0);

    MPI_Allgather(array, cells, MPI_FLOAT, data, cells,MPI_FLOAT,MPI_COMM_WORLD);
    

    if (myrank == 0) {
    printf("Sample results\n");
        printf("   data[1]=%e\n",  data[1]);
        printf("   data[32]=%e\n",  data[32]);
        t2 = MPI_Wtime();
        MPI_Get_processor_name(name, &result);
        printf("master: %d at processor: %s\ntime: %lf\n",myrank, name,t2-t1);
    }
    MPI_Finalize();

}
