#include <mpi.h>


int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
  MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
  if (my_rank == 0) {
     MPI_Send (sendbuf, count, MPI_INT, 1, tag, MPI_COMM_WORLD);
     MPI_Recv (recvbuf, count, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
  } else if (my_rank == 1) {
     MPI_Send (sendbuf, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
     MPI_Recv (recvbuf, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
  }
  MPI_Finalize();
  return 0;
}