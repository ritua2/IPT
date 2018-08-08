#include<stdio.h>
#include<math.h>
#include<omp.h> //added this
# include <time.h>
#include <sys/types.h>
#include <sys/time.h>


#define ESP 0.0001


double gettime(void) {
	struct timeval tval;
	gettimeofday(&tval, NULL);
	return( (double)tval.tv_sec + (double)tval.tv_usec/1000000.0 );
}

int main(int argc, char **argv){
double a[3][3];
double x[3], x_old[3];
double c[] = {1, 28, 76};
int i, j, k, iter, N, M;
	double sum[3];
	int maxiter = 6;
	double Ea[3], EaMax;
    double wtime1, wtime2;
	int flag =0;
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

	for(i=0; i<M;i++){
		for(j=0; j<N;j++){
			printf(" %lf ", a[i][j]);
		}
		printf("\n");
	}
	wtime1 = gettime();
	omp_set_num_threads(N);
	for(iter = 1; iter <= maxiter; iter++){
		EaMax =0.0;
		flag = 0;
		#pragma omp parallel for shared (a, x_old, N, M, sum, Ea ) private ( i, j )
		for(i=0; i<N; i++){
			sum[i] = 0.0;
			for(j=(i+1); j<M; j++){
				sum[i] = a[i][j] * x[j] + sum[i];
			}
			if (i == 0){
				x[i] = (c[i] - sum[i])/a[i][i];
				#pragma omp atomic update
				flag++;
				Ea[i] = fabs((x[i] - x_old [i])/x[i]) * 100;
				x_old[i] = x[i];
			}else if (i > 0){
				#pragma omp flush(flag)
				while(flag < i)
				{
					#pragma omp flush(flag)
				}
				for(j=0; j<i; j++){
					sum[i] = sum[i] + a[i][j]*x[j];
				}
				x[i] = (c[i] - sum[i])/a[i][i];
				#pragma omp atomic update
				flag++;
				Ea[i] = fabs((x[i] - x_old [i])/x[i]) * 100;
				x_old[i] = x[i];
			}
		}

		for(i = 0; i < N; i++){
			if(Ea[i] > EaMax){
				EaMax = Ea[i];
			}
			printf("\nIn iteration number: %d, thread_is: %d, X[%d]: %lf, X_old[%d]: %lf, Error: %lf, EaMax: %lf, Sum: %lf\n", iter, omp_get_thread_num(), i,x[i], i, x_old[i], Ea[i],EaMax, sum[i] );
		}
		printf("\nEaMax: %lf\n", EaMax);
		if(EaMax < ESP){
				break;
		}
	}
	wtime2 = gettime();
	printf ( "Elapsed wall clock time (seconds) %f\n", (wtime2 - wtime1) );
	return 0;
}
