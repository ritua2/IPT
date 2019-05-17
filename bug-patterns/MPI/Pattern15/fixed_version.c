#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc,&argv);
    int rank,comm_size;

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_size);

    MPI_Request reqs[4];

    int sbuff1[10];
    int sbuff2[10];
    int rbuff1[10];
    int rbuff2[10];

    int count = 0;

    if(rank>0)
    {

        MPI_Irecv(rbuff1,10,MPI_INT,rank-1,0,MPI_COMM_WORLD,&reqs[count]);
        count++;
        MPI_Isend(sbuff1,10,MPI_INT,rank-1,1,MPI_COMM_WORLD,&reqs[count]);
        count++;
    }

    if(rank<comm_size-1)
    {
        /// changed tag 2 -> 1 
        // MPI_Irecv(rbuff2,10,MPI_INT,rank+1,2,MPI_COMM_WORLD,&reqs[count]);
        MPI_Irecv(rbuff2,10,MPI_INT,rank+1,1,MPI_COMM_WORLD,&reqs[count]);
        count++;

        /// changed tag 3 -> 0 
        // MPI_Isend(sbuff2,10,MPI_INT,rank+1,3,MPI_COMM_WORLD,&reqs[count]);
        MPI_Isend(sbuff2,10,MPI_INT,rank+1,0,MPI_COMM_WORLD,&reqs[count]);
        count++;
    }

    MPI_Waitall(count,reqs,MPI_STATUS_IGNORE);

    if(rank==0)
            cout << "Successfully exchanged data." << endl;

    MPI_Finalize();

    return 0;
}