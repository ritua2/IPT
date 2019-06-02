#include <mpi.h>

MPI_Datatype MPI_A;

struct A
{ 
    int a;
};  

int main()
{   
    MPI_Init(NULL, NULL);

    A a;

    int nblock = 1;
    int block_count = 1;
    MPI_Aint offset = 0;
    MPI_Datatype block_type = MPI_INT;

    MPI_Type_struct(nblock, &block_count, &offset, &block_type, &MPI_A);
    MPI_Type_commit(&MPI_A);

    MPI_Aint extent;
    MPI_Aint lb;
    MPI_Type_get_extent(MPI_A, &lb, &extent);
    MPI_Finalize();
    return 0;
}