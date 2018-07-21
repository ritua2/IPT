#include <omp.h> 
#include <stdio.h>
#include <sys/time.h>
#define N 30000

int main()
{
  int i;
  int j;
  double x[30002UL][30002UL];
  double y[30002UL][30002UL];
  double sum;
  double tmp;
//for timing the code section
  struct timeval start;
  struct timeval end;
  float delta;
  for (i = 0; i <= 30000 + 1; i++) {
    for (j = 0; j <= 30000 + 1; j++) {
      x[i][j] = (((double )((i + j) % 3)) - 0.9999);
    }
  }
  printf("\nMemory allocation done successfully\n");
//start timer and calculation
  gettimeofday(&start,0);
  
#pragma omp parallel default(none) shared(sum,x,y) private(j,i,tmp)
{
  
#pragma omp for reduction ( + :sum)
  for (j = 1; j < 30000 + 1; j++) {
    for (i = 1; i < 30000 + 1; i++) {
      tmp = (0.2 * ((((x[i][j] + x[i - 1][j]) + x[i + 1][j]) + x[i][j - 1]) + x[i][j + 1]));
      y[i][j] = tmp;
      sum = (sum + tmp);
    }
  }
}
//stop timer and calculation
  gettimeofday(&end,0);
  delta = (((((end.tv_sec - start.tv_sec) * 1000000u) + end.tv_usec) - start.tv_usec) / 1.e6);
  printf("\nThe total sum is: %lf\n",sum);
//print time to completion 
  printf("run time    = %fs\n",delta);
  return 0;
}
