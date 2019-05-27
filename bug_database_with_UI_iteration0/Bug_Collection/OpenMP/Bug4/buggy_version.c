#include<stdio.h>

int main(){
  int sum = 1;


  // increase sum by 10 using openmp
#pragma omp parallel 
  {
#pragma omp for reduction (+: sum)
    for ( int i = 0; i < 10; i ++) {
      #pragma omp critical 
      {
	      ddsum +=1;
      }
    }
  }
}

