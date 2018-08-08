#include<stdio.h>
#include<math.h>
#include<mpi.h>

#define ESP 0.0001

int main(int argc, char **argv){
  double a[3][3];
  double x[3], x_old[3];
  double c[] = {1, 28, 76};

  int i, j, k, iter, N, M;
  double sum;
  int maxiter = 6;
  double Ea[3], EaMax, EaMaxGlobal;

  int rank, size;
  int start, end;
  MPI_Request r[2];
  MPI_Status statuses[2];
  MPI_Status status;

  N=3;
  M=3;

 //initial guesses for x[]

  x[0] = 1.00;
  x[1] = 0.00;
  x[2] = 1.00;

  x_old[0] = 1.00;
  x_old[1] = 0.00;
  x_old[2] = 1.00;

  a[0][0] = 12.00;
  a[0][1] = 3.00;
  a[0][2] = -5.00;

  a[1][0] = 1.00;
  a[1][1] = 5.00;
  a[1][2] = 3.00;

  a[2][0] = 3.00;
  a[2][1] = 7.00;
  a[2][2] = 13.00;


  printf("\nTESTING\n");
  for(i=0; i<M;i++){
      for(j=0; j<N;j++){
		printf(" %lf ", a[i][j]);
      }
      printf("\n");
  }

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  start = rank*N/size;
  end = start + N/size;

  printf("\nrank: %d, start: %d, end: %d\n", rank, start, end);

  for(iter = 1; iter <= maxiter; iter++){
	EaMax =0.0;
	for(i=start; i<end; i++){
		sum = 0.0;
		for(k=0; k<N; k++){
        	if(k!=rank && k < rank ){
                    MPI_Recv( &x_old[k], 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD, &status);
                    printf("\nProcessor %d, received from %d, value of X_old[%d]: %lf", rank, k, k, x_old[k]);
            }
        }

		for(j=0; j<M; j++){
		   if(i!=j){
				sum = a[i][j] * x_old[j] + sum;
            	printf("\n check....iteration: %d, sum: %lf, x_old[%d]: %lf\n", iter-1, sum, j, x_old[j]);
		   }
		}
		x[i] = (c[i] - sum)/a[i][i];
		printf("\nSum: %lf, a[%d][%d]: %lf, c[i]: %lf, x[i]: %lf\n", sum, i, i, a[i][i], c[i], x[i]);
        Ea[i] = fabs((x[i] - x_old [i])/x[i]) * 100;
        printf("\nIn iteration number: %d, X[%d]: %lf, Error: %lf\n", iter, i,x[i], Ea[i] );
		x_old[i] = x[i];
		EaMax = Ea[i];
		for(k=0; k<N; k++){
			if(k!=rank){
				printf("\nProcessor %d, sending to %d, value of X[%d]: %lf", rank, k, i, x[i]);
				MPI_Send(&x[i], 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
			}
		}
		if(rank!=(size-1)){
			for(k=0; k<N; k++){
                if(k!=rank && k > rank){
					MPI_Recv( &x_old[k], 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD, &status);
					printf("\nProcessor %d, received from %d, value of X_old[%d]: %lf", rank, k, k, x_old[k]);
				}
			}
		}
	}

	MPI_Allreduce(&EaMax, &EaMaxGlobal, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD );

	printf("\nEaMax: %lf\n", EaMaxGlobal);
	if(EaMax < ESP){
		break;
	}
  }
  MPI_Finalize();
  return 0;
}


