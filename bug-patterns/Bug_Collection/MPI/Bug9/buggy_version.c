#include <mpi.h>
#define N 4

void transform_row(int* row, const int k) {
  for (int i = 0; i < N; ++i) {
    row[i] *= k;
  }
}
const int root = 0;


int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, ranksize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ranksize);
  if (rank == root) {
    int* arr = new int[N];
    for (int i = 0; i < N; ++i) {
      arr[i] = i * i + 1;
    }
    MPI_Bcast(arr, N, MPI_INT, root, MPI_COMM_WORLD);
  }
  int* arr = new int[N];
  MPI_Bcast(arr, N, MPI_INT, root, MPI_COMM_WORLD);
  transform_row(arr, rank * 100);
  int* transformed = new int[N * ranksize];
  MPI_Gather(arr, N, MPI_INT, transformed, N, MPI_INT, root, MPI_COMM_WORLD);
  if (rank == root) {
    for (int i = 0; i < ranksize; ++i) {
      for (int j = 0; j < N ; j++) {
        printf("%i ", transformed[i * N + j]);
      }
      printf("\n");
    }
  }
  MPI_Finalize();
  return 0;
}