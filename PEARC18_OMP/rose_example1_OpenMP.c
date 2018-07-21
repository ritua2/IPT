#include <omp.h> 
#include <stdio.h>
#include <sys/time.h>
#define N 30000

int main()
{
  int i;
  int j;
  double x[30000 + 2][30000 + 2];
  double y[30000 + 2][30000 + 2];
  double sum;
  double tmp;
//for timing the code section
  struct timeval start;
  struct timeval end;
  float delta;
  for (i = 0; i <= 30000 + 1; i++) {
    for (j = 0; j <= 30000 + 1; j++) {
      x[i][j] = ((double )((i + j) % 3)) - 0.9999;
    }
  }
  printf("\nMemory allocation done successfully\n");
//start timer and calculation
  gettimeofday(&start,((void *)0));
{
 double Openmp_time_start, Openmp_time_end, Openmp_delta_time;
 Openmp_time_start = omp_get_wtime( );
  
#pragma omp parallel default(none) shared(sum,x,y) private(j,i,tmp)
{
  
#pragma omp for reduction ( + :sum)
  for (j = 1; j < 30000 + 1; j++) {
    for (i = 1; i < 30000 + 1; i++) {
      tmp = 0.2 * (x[i][j] + x[i - 1][j] + x[i + 1][j] + x[i][j - 1] + x[i][j + 1]);
      y[i][j] = tmp;
      sum = sum + tmp;
    }
  }
}
Openmp_time_end = omp_get_wtime( );
Openmp_delta_time = Openmp_time_end - Openmp_time_start;
printf("Time taken for the process is %f\n",Openmp_delta_time);
}
//stop timer and calculation
  gettimeofday(&end,((void *)0));
  delta = (((end . tv_sec - start . tv_sec) * 1000000u + end . tv_usec - start . tv_usec) / 1.e6);
  printf("\nThe total sum is: %lf\n",sum);
//print time to completion 
  printf("run time    = %fs\n",delta);
  return 0;
}
