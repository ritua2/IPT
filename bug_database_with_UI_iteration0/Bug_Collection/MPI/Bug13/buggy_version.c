#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include<math.h>

double a[15][15];

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
    //Nxl=((Nx-2)/nprocess)+2;
    //printf("NXL=%d\n",Nxl);
    //  printf("process_id=%d\n",process_id);
    if(process_id==0)
    {
        for(i=1;i<=5;i++)
        {
            for(j=1;j<=Ny;j++)
            {
                a[i][j]=3*2*i;
                MPI_Send(&a[Nxl-1][j],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
            }
        }
        for(i=1;i<=5;i++)
        {
            for(j=1;j<=Ny;j++)
            {
                printf("matrices=%f\n",a[i][j]);
                MPI_Recv(&a[1][j],1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
                printf("PROCESS_ID=%d\n",process_id);
            }
        }
    }
    if(process_id==1)
    {
        for(i=6;i<=10;i++)
        {
            for(j=1;j<Ny;j++)
            {
                a[i][j]=4*2;
                MPI_Send(&a[2][j],1,MPI_DOUBLE,1,2,MPI_COMM_WORLD);
            }
        }
        for(i=6;i<=10;i++)
        {
            for(j=1;j<Ny;j++)
            {
                MPI_Recv(&a[Nxl][j],1,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
                printf("PROCESS_ID=%d\n",process_id);
            }
        }
    }
    MPI_Finalize();
}