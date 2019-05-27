#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

void barrier(){    
    //Get rank and number of processors
    int my_rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); 
    //MPI Communication status var
    MPI_Status status;
    char b_req[1];
    //**Barier implementation for P0**
    if(my_rank == 0){
        double exec_start_all = MPI_Wtime();
        bool *nodes_barrier = (bool *)malloc(num_procs*sizeof(bool));  
        *(nodes_barrier)  = true;
        int start_count = 1; int end_count = 1; int i;
        //Set all values array, except for P0, on false
        for(i=1; i<num_procs; i++)
            *(nodes_barrier + i)  = false;
        //Receive msg from all procs which started their barrier
        while(start_count < num_procs){ 
            MPI_Recv(&b_req,sizeof(b_req),MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);            
            if(*(nodes_barrier + status.MPI_SOURCE) == false){                
                *(nodes_barrier + status.MPI_SOURCE) = true;
                start_count ++;  
            }
        }        
        //Once all procs started their barrier, send msg to all procs to release
        int j;
        for(j=1; j<num_procs; j++)
            MPI_Send(&b_req,sizeof(b_req),MPI_CHAR,j,2,MPI_COMM_WORLD);
        //Get execution time of barrier from all procs and calculate the overal barrier execution time
        double *tmp = (double *)malloc(2*sizeof(double));
        double exec_end_all = MPI_Wtime();
        while(end_count < num_procs){ 
            MPI_Recv((double *)tmp,(2*sizeof(double)),MPI_DOUBLE, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);            
            if(*(nodes_barrier + status.MPI_SOURCE) == true){
                double start = ((double)*tmp+0);
                double end = ((double)*tmp+1);
                printf("P(%d) has start: %lf end: %lf \n",status.MPI_SOURCE, start, end);
                if(start < exec_start_all)
                    exec_start_all = start;                
                if(end > exec_end_all)
                    exec_end_all = end;                               
                *(nodes_barrier + status.MPI_SOURCE) = false;
                end_count ++;  
            }
        }        
        /*if(MPI_Wtime() > exec_end_all){
            exec_end_all = MPI_Wtime();
        }&*/
        printf("Barrier finished, start: %lf End: %lf Execution time: %lf \n", exec_start_all, exec_end_all,  (exec_end_all - exec_start_all));
        free(nodes_barrier);        
        free(tmp);


    //**Barier implementation for Pn-1 (except P0)**
    } else {
        double *execution = (double *)malloc(2*sizeof(double));
        *(execution + 0) = MPI_Wtime();
        //Send P0 that this proc starts its barrier
        MPI_Send(&b_req,sizeof(b_req),MPI_CHAR,0,1,MPI_COMM_WORLD);
        //Receive command from P0 that it can release its barrier
        MPI_Recv(&b_req,sizeof(b_req),MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);
        //Measure and send execution time to P0   
        *(execution + 1) = MPI_Wtime();
        MPI_Send((double *)execution,(2*sizeof(double)),MPI_DOUBLE,0,3,MPI_COMM_WORLD); 
        free(execution);
    }    
}

int main(int argc, char *argv[])
{    
    //Initialize the infrastructure necessary for communication
    MPI_Init(&argc,&argv);
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    barrier();


    printf("FINAL %d \n", my_rank);
    MPI_Finalize();
    return 0;
}