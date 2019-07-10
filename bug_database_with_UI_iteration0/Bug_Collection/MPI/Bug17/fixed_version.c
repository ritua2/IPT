#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
using namespace std;
int main(int argc, char *argv[])
{
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int i=0,m;    
    
    int totlen = 0;
    int *displs = NULL;
    char *totalstring = NULL;
    int *recvcounts = NULL;

    /* Only root has the received data */
    if (rank == 0){
        recvcounts = (int*)malloc( size * sizeof(int)) ;
    }
    char str[100]="",a[100]="",a1[100]="";
    
    if (rank == 0) {
        strcpy(str, "abcd");        
    }
    
   
    MPI_Scatter(str,1,MPI_CHAR,a,1,MPI_CHAR,0,MPI_COMM_WORLD);

    while(i<=rank){
        a1[i]=a[0];
        i++;
    }
    MPI_Gather(&i, 1, MPI_INT,recvcounts, 1, MPI_INT,0, MPI_COMM_WORLD);

    if (rank == 0) {
        displs = (int*)malloc( size * sizeof(int) );

        displs[0] = 0;
        totlen += recvcounts[0]+1;

        for (i=1; i<size; i++) {
           totlen += recvcounts[i]+1;   /* for adding extra space after each set of characters*/
           displs[i] = displs[i-1] + recvcounts[i-1] + 1;
        }

        totalstring = (char*)malloc(totlen * sizeof(char));            
        
	for (i=0; i<totlen-1; i++)
            totalstring[i] = ' ';

        totalstring[totlen-1] = '\0';
    }

    MPI_Gatherv(a1, (rank+1), MPI_CHAR,totalstring, recvcounts, displs, MPI_CHAR,0, MPI_COMM_WORLD);
    if(rank==0){
        printf("\nTotal string from rank %d is %s\n",rank, totalstring);
    }



    MPI_Finalize();
    /* code */
    return 0;
}
