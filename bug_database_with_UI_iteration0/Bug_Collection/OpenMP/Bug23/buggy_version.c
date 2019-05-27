#include <stdio.h>

int main(){
  int T[5];
  int sum = 0;

  // initializing array T 
  for (int  i = 0; i < 10; i ++) {
    T[i] = i;
  }

  // running the loop 10 times using openmp
#pragma omp parallel for shared (T,sum) reduction (+ : sum)
  for ( int i = 0; i < 100; i ++) {
    
    // assign value for elements in array T
    for (int j =0; j < 5; j++) {
      T[j]  = i ;
    }

    // increase "sum" by the toal of T array module by 2
    sum += (T[0] + T[1] + T[2] + T[3] + T[4]) % 2;
  }

  printf("sum = %d\n",sum );
  
}

