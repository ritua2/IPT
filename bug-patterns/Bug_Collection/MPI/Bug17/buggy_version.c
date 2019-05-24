#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int n[100];
    MPI_Status status;
    int i=0,m;    
    int total[size];
    char str[100],a[100],a1[100];
    if (rank == 0) {
        str =  "abcd";        
    }

    MPI_Scatter(str,1,MPI_CHAR,a,1,MPI_CHAR,0,MPI_COMM_WORLD);

    while(i<=rank){
        a1[i]=a[0];
        i++;
    }
    int disps[size];
    int y=rank;
    disps[rank]=0;
    while(y!=0){
        disps[rank]+=y;
        y--;
    }
    total[rank]=rank+1;
    printf("%s\n",a1 );
    char k[100];

    printf("%d %d\n",total[rank],disps[rank] );
    MPI_Gatherv(a1,rank+1,MPI_CHAR,k,total,disps,MPI_CHAR,0,MPI_COMM_WORLD);

    if(rank==0){
        printf("Total %s\n",k);
    }



    MPI_Finalize();
    /* code */
    return 0;
}