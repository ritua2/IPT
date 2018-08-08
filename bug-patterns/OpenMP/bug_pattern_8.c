#include<stdio.h>

int main(){
  int sum = 1;
  int i = 0;

  // increase sum by one each iteratiob using openmp 
#pragma omp parallel for private(i) reduction( + : sum )
  for (i; i < 10; i ++) {
    sum +=1;
  }

  int equal = (sum == i);
}

