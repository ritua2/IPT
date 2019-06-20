#include<stdio.h>

int main(){
  int sum = 1;
  int i =1;

  // increase sum by one each iteratiob using openmp 
  #pragma omp parallel for private(i) reduction( + : sum )
  for (int j  = i; j < 100; j++) {
    sum +=1;
  }

  printf("sum is %d\n",sum);
}

