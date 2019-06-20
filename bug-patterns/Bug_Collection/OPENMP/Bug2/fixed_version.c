#include<stdio.h>

int main(){

  int T[10];

  // initializing array T using openmp
#pragma omp parallel for shared(T) 
  for ( int i = 0; i < 10; i ++) {
  T[i] = i;
}
}

