#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include<math.h>


double a[10][10];

int main(int argc, char **argv)
{
    int process_id,nprocess;
    int i,j;
    int Nxl=5,Nx=10,Ny=10;
    MPI_Status status;
    MPI_Datatype line;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocess);
    MPI_Comm_rank(MPI_COMM_WORLD,&process_id);
    
    if(process_id==0)
    {
        for( i=0;i <5;i++)
        {
            for(j=0;j <Ny;j++)
            {
                a[i][j]=3*2*i;
                MPI_Send(&a[i][j],1,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
                //printf("Process %d sent: %lf", process_id, a[i][j]);
            }
        }
        for(i=5;i <Nx;i++)
        {
            for(j=0;j <Ny;j++)
            {
                MPI_Recv(&a[i][j],1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
            }
        }
    }
    if(process_id==1)
    {
        for(i = 0;i < 5;i++)
        {
            for(j=0;j<Ny;j++)
            {
                MPI_Recv(&a[i][j],1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
            }
        }
	for(i=5;i < Nx;i++)
        {
            for(j = 0;j<Ny;j++)
            {
                a[i][j]=4*2;
                MPI_Send(&a[i][j],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
            }
        }

    }
    if(process_id==1){
    	for(i=0;i < Nx;i++){
            for(j = 0;j<Ny;j++){
		printf("%lf ", a[i][j]);
	    }
	    printf("\n");
	}
    }
    MPI_Finalize();
}
