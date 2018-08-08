#include<stdio.h>
#include<math.h>

#define ESP 0.0001

int main(int argc, char **argv){
  double a[3][3];
  double x[3], x_old[3];
  double c[] = {1, 28, 76};

  int i, j, k, iter, N, M;
  double sum;
  int maxiter = 6;
  double Ea[3], EaMax;
  N=3;
  M=3;

 //initial guess for x[]

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

  for(iter = 1; iter <= maxiter; iter++){
	EaMax =0.0;
	for(i=0; i<N; i++){
		sum = 0.0;
		for(j=0; j<M; j++){
		   if(i!=j){
				sum = a[i][j] * x_old[j] + sum;
                printf("\n check....iteration: %d, sum: %lf\n", iter, sum);
		    }
		}
		x[i] = (c[i] - sum)/a[i][i];
		printf("\nSum: %lf, a[%d][%d]: %lf, c[i]: %lf, x[i]: %lf\n", sum, i, i, a[i][i], c[i], x[i]);
        Ea[i] = fabs((x[i] - x_old [i])/x[i]) * 100;
        printf("\nIn iteration number: %d, X[%d]: %lf, Error: %lf\n", iter, i,x[i], Ea[i] );
		x_old[i] = x[i];

	}

    for(i=0;i<N; i++){
		if (Ea[i] > EaMax){
			EaMax = Ea[i];
		}
	}
	printf("\nEaMax: %lf\n", EaMax);
	if(EaMax < ESP){
		break;
	}
  }

  return 0;
}


