#include <mpi.h>


int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank,size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  int tag = 0;
  int sendbuf[2];
  MPI_Status status;
  for (int i =0; i < 2; i++) {
    sendbuf[i] = rank +1;
  }
  int recvbuf[2];
  int count = 2;
  if (rank == 0) {
     MPI_Send (sendbuf, count, MPI_INT, 1, tag, MPI_COMM_WORLD);
     MPI_Recv (recvbuf, count, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
  } else if (rank == 1) {
     MPI_Send (sendbuf, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
     MPI_Recv (recvbuf, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
  }
  MPI_Finalize();
  return 0;
}