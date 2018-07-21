#include <stdio.h>
#include <sys/time.h>


#define N 30000

int main(){
  int i, j;
  double x[N+2][N+2], y[N+2][N+2], tmp[N+2][N+2];
  double sum=0;

  //for timing the code section
  struct timeval start,end;
  float delta;

  for(i=0; i <= N+1; i++){
     for(j=0; j <= N+1; j++){
         x[i][j] = (double) ((i+j)%3) - 0.9999;
         y[i][j]= x[i][j] + 0.0001;
     }
  }

  //start timer and calculation
  gettimeofday(&start, NULL);

  for(j=1; j<N+1; j++){
    for(i=1; i<N+1; i++ ){
       tmp[i][j] = 0.167 * (x[i][j] + x[i-1][j] + x[i+1][j] + x[i][j-1] + x[i][j+1] + y[i+1][j]);
       y[i][j] = tmp [i][j];
       sum = sum + tmp[i][j];
    }
  }
  //stop timer and calculation
  gettimeofday(&end, NULL);
  delta = ((end.tv_sec-start.tv_sec)*1000000u + end.tv_usec-start.tv_usec)/1.e6;
  printf("\nThe total sum is: %lf\n", sum);
  //print time to completion  
  printf("run time    = %fs\n", delta);
  return 0;
}


