#include<stdio.h>

int main(){
  int sum = 1;
  int i;

  // increase sum by 10 using openmp
#pragma omp parallel for private (i) reduction (+: sum)
  for ( i = 0; i < 10; i++) {
    sum +=i;
  }
}

